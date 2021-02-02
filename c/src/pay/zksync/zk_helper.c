/*******************************************************************************
 * This file is part of the Incubed project.
 * Sources: https://github.com/slockit/in3-c
 * 
 * Copyright (C) 2018-2019 slock.it GmbH, Blockchains LLC
 * 
 * 
 * COMMERCIAL LICENSE USAGE
 * 
 * Licensees holding a valid commercial license may use this file in accordance 
 * with the commercial license agreement provided with the Software or, alternatively, 
 * in accordance with the terms contained in a written agreement between you and 
 * slock.it GmbH/Blockchains LLC. For licensing terms and conditions or further 
 * information please contact slock.it at in3@slock.it.
 * 	
 * Alternatively, this file may be used under the AGPL license as follows:
 *    
 * AGPL LICENSE USAGE
 * 
 * This program is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Affero General Public License as published by the Free Software 
 * Foundation, either version 3 of the License, or (at your option) any later version.
 *  
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY 
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
 * PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
 * [Permissions of this strong copyleft license are conditioned on making available 
 * complete source code of licensed works and modifications, which include larger 
 * works using a licensed work, under the same license. Copyright and license notices 
 * must be preserved. Contributors provide an express grant of patent rights.]
 * You should have received a copy of the GNU Affero General Public License along 
 * with this program. If not, see <https://www.gnu.org/licenses/>.
 *******************************************************************************/

#include "zk_helper.h"
#include "../../core/client/context_internal.h"
#include "../../core/client/keys.h"
#include "../../core/client/plugin.h"
#include "../../core/util/debug.h"
#include "../../core/util/mem.h"
#include "../../third-party/zkcrypto/lib.h"
#include "zksync.h"
#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

void set_quoted_address(char* c, uint8_t* address) {
  bytes_to_hex(address, 20, c + 3);
  c[0] = c[43] = '"';
  c[1]         = '0';
  c[2]         = 'x';
  c[44]        = 0;
}
in3_ret_t send_provider_request(in3_ctx_t* parent, zksync_config_t* conf, char* method, char* params, d_token_t** result) {
  if (params == NULL) params = "";
  char* in3 = conf ? alloca(strlen(conf->provider_url) + 26) : NULL;
  if (in3) sprintf(in3, "{\"rpc\":\"%s\"}", conf->provider_url);
  return ctx_send_sub_request(parent, method, params, in3, result);
}

in3_ret_t zksync_get_account(zksync_config_t* conf, in3_ctx_t* ctx, uint8_t** account) {
  if (!conf->account) {
    in3_sign_account_ctx_t sctx = {.ctx = ctx, .accounts = NULL, .accounts_len = 0};
    if (in3_plugin_execute_first(ctx, PLGN_ACT_SIGN_ACCOUNT, &sctx) || !sctx.accounts_len) {
      if (sctx.accounts) _free(sctx.accounts);
      return ctx_set_error(ctx, "No account configured or signer set", IN3_ECONFIG);
    }
    conf->account = (uint8_t*) sctx.accounts;
  }

  if (account) *account = conf->account;
  return IN3_OK;
}

// sends a account_info request and updates the config
in3_ret_t zksync_update_account(zksync_config_t* conf, in3_ctx_t* ctx) {
  uint8_t*   account = NULL;
  d_token_t* result;
  char       adr[45];

  TRY(zksync_get_account(conf, ctx, &account))
  set_quoted_address(adr, account);
  TRY(send_provider_request(ctx, conf, "account_info", adr, &result))

  d_token_t* committed = d_get(result, key("committed"));
  conf->account_id     = d_get_intk(result, K_ID);
  conf->nonce          = d_get_longk(committed, K_NONCE);
  char* kh             = d_get_stringk(committed, key("pubKeyHash"));
  if (kh && strlen(kh) == 45)
    hex_to_bytes(kh + 5, 40, conf->pub_key_hash, 20);

  return IN3_OK;
}

// resolves the account_id
in3_ret_t zksync_get_account_id(zksync_config_t* conf, in3_ctx_t* ctx, uint32_t* account_id) {
  uint8_t* account    = NULL;
  char*    cache_name = NULL;
  TRY(zksync_get_account(conf, ctx, &account))

  if (in3_plugin_is_registered(ctx->client, PLGN_ACT_CACHE) && account) {
    cache_name = alloca(60);
    strcpy(cache_name, "zksync_ac_");
    bytes_to_hex(account, 20, cache_name + 9);
    in3_cache_ctx_t cctx = {.ctx = ctx, .key = cache_name, .content = NULL};
    TRY(in3_plugin_execute_first_or_none(ctx, PLGN_ACT_CACHE_GET, &cctx))
    if (cctx.content) {
      conf->account_id = bytes_to_int(cctx.content->data, 4);
      b_free(cctx.content);
    }
  }

  if (!conf->account_id) TRY(zksync_update_account(conf, ctx))
  if (!conf->account_id) return ctx_set_error(ctx, "This user has no account yet!", IN3_EFIND);
  if (account_id) *account_id = conf->account_id;

  // add to cache
  if (cache_name) {
    uint8_t data[4];
    bytes_t content = bytes(data, 4);
    int_to_bytes(conf->account_id, data);
    in3_cache_ctx_t cctx = {.ctx = ctx, .key = cache_name, .content = &content};
    TRY(in3_plugin_execute_first_or_none(ctx, PLGN_ACT_CACHE_SET, &cctx))
  }

  return IN3_OK;
}

in3_ret_t zksync_get_sync_key(zksync_config_t* conf, in3_ctx_t* ctx, uint8_t* sync_key) {
  if (!conf) return IN3_EUNKNOWN;
  if (!memiszero(conf->sync_key, 32)) {
    memcpy(sync_key, conf->sync_key, 32);
    return IN3_OK;
  }
  uint8_t* account = NULL;
  bytes_t  signature;
  char*    message = "\x19"
                  "Ethereum Signed Message:\n68"
                  "Access zkSync account.\n\nOnly sign this message for a trusted client!";
  TRY(zksync_get_account(conf, ctx, &account))
  assert(account);
  TRY(ctx_require_signature(ctx, SIGN_EC_HASH, &signature, bytes((uint8_t*) message, strlen(message)), bytes(account, 20)))
  if (signature.len == 65)
    signature.data[64] += 27;
  zkcrypto_pk_from_seed(signature, conf->sync_key);
  memcpy(sync_key, conf->sync_key, 32);
  return IN3_OK;
}

in3_ret_t zksync_get_contracts(zksync_config_t* conf, in3_ctx_t* ctx, uint8_t** main) {

  char* cache_name = NULL;
  if (!conf->main_contract) {
    // check cache first
    if (in3_plugin_is_registered(ctx->client, PLGN_ACT_CACHE)) {
      cache_name = alloca(100);
      sprintf(cache_name, "zksync_contracts_%x", key(conf->provider_url));
      in3_cache_ctx_t cctx = {.ctx = ctx, .key = cache_name, .content = NULL};
      TRY(in3_plugin_execute_first_or_none(ctx, PLGN_ACT_CACHE_GET, &cctx))
      if (cctx.content) {
        conf->main_contract = _malloc(20);
        conf->gov_contract  = _malloc(20);
        memcpy(conf->main_contract, cctx.content->data, 20);
        memcpy(conf->gov_contract, cctx.content->data + 20, 20);
        b_free(cctx.content);
      }
    }
  }

  if (!conf->main_contract) {
    d_token_t* result;
    TRY(send_provider_request(ctx, conf, "contract_address", "", &result))
    bytes_t* main_contract = d_get_bytesk(result, key("mainContract"));
    if (!main_contract || main_contract->len != 20) return ctx_set_error(ctx, "could not get the main_contract from provider", IN3_ERPC);
    memcpy(conf->main_contract = _malloc(20), main_contract->data, 20);

    bytes_t* gov_contract = d_get_bytesk(result, key("govContract"));
    if (!gov_contract || gov_contract->len != 20) return ctx_set_error(ctx, "could not get the gov_contract from provider", IN3_ERPC);
    memcpy(conf->gov_contract = _malloc(20), gov_contract->data, 20);

    if (cache_name) {
      uint8_t data[40];
      bytes_t content = bytes(data, 40);
      memcpy(data, main_contract->data, 20);
      memcpy(data + 20, gov_contract->data, 20);
      in3_cache_ctx_t cctx = {.ctx = ctx, .key = cache_name, .content = &content};
      TRY(in3_plugin_execute_first_or_none(ctx, PLGN_ACT_CACHE_SET, &cctx))
    }

    // clean up
    ctx_remove_required(ctx, ctx_find_required(ctx, "contract_address"), false);
  }

  if (main) *main = conf->main_contract;
  return IN3_OK;
}

in3_ret_t zksync_get_nonce(zksync_config_t* conf, in3_ctx_t* ctx, d_token_t* nonce_in, uint32_t* nonce) {
  if (nonce_in && (d_type(nonce_in) == T_INTEGER || d_type(nonce_in) == T_BYTES)) {
    *nonce = d_long(nonce_in);
    return IN3_OK;
  }
  TRY(zksync_update_account(conf, ctx))
  *nonce = conf->nonce;
  return IN3_OK;
}

in3_ret_t zksync_get_fee(zksync_config_t* conf, in3_ctx_t* ctx, d_token_t* fee_in, bytes_t to, d_token_t* token, char* type,
#ifdef ZKSYNC_256
                         uint8_t* fee
#else
                         uint64_t* fee
#endif
) {
  if (fee_in && (d_type(fee_in) == T_INTEGER || d_type(fee_in) == T_BYTES)) {
#ifdef ZKSYNC_256
    bytes_t b = d_to_bytes(fee_in);
    memcpy(fee + 32 - b.len, b.data, b.len);
#else
    *fee = d_long(fee_in);
#endif
    return IN3_OK;
  }
  d_token_t* result;
  bool       is_object_type = *type == '{';
  int        ss             = strlen(type) + 4 + 50 * 2 - is_object_type * 2;
  sb_t       sb             = {.allocted = ss, .data = alloca(ss), .len = 0};
  if (!is_object_type) sb_add_char(&sb, '"');
  sb_add_chars(&sb, type);
  sb_add_bytes(&sb, is_object_type ? "," : "\",", &to, 1, false);
  sb_add_char(&sb, ',');
  switch (d_type(token)) {
    case T_BYTES:
      sb_add_bytes(&sb, ",", d_bytes(token), 1, false);
      break;
    case T_STRING: {
      sb_add_char(&sb, '"');
      sb_add_chars(&sb, d_string(token));
      sb_add_char(&sb, '"');
      break;
    }
    default:
      return ctx_set_error(ctx, "invalid token-value", IN3_EINVAL);
  }
  TRY(send_provider_request(ctx, conf, "get_tx_fee", sb.data, &result))
#ifdef ZKSYNC_256
  memset(fee, 0, 32);
  long_to_bytes(d_get_longk(result, key("totalFee")), fee + 24);
#else
  *fee = d_get_longk(result, key("totalFee"));
#endif
  return IN3_OK;
}

in3_ret_t resolve_tokens(zksync_config_t* conf, in3_ctx_t* ctx, d_token_t* token_src, zksync_token_t** token_dst) {
  char* cache_name = NULL;
  if (!conf->token_len) {
    // check cache first
    if (in3_plugin_is_registered(ctx->client, PLGN_ACT_CACHE)) {
      cache_name = alloca(100);
      sprintf(cache_name, "zksync_tokens_%x", key(conf->provider_url));
      in3_cache_ctx_t cctx = {.ctx = ctx, .key = cache_name, .content = NULL};
      TRY(in3_plugin_execute_first_or_none(ctx, PLGN_ACT_CACHE_GET, &cctx))
      if (cctx.content) {
        conf->token_len = cctx.content->len / sizeof(zksync_token_t);
        conf->tokens    = (void*) cctx.content->data;
        _free(cctx.content);
      }
    }
  }

  // still no tokenlist?
  if (!conf->token_len) {
    d_token_t* result;
    TRY(send_provider_request(ctx, conf, "tokens", "", &result))
    conf->token_len = d_len(result);
    conf->tokens    = _calloc(conf->token_len, sizeof(zksync_token_t));
    int i           = 0;
    for (d_iterator_t it = d_iter(result); it.left; d_iter_next(&it), i++) {
      conf->tokens[i].id       = d_get_intk(it.token, K_ID);
      conf->tokens[i].decimals = d_get_intk(it.token, key("decimals"));
      char* name               = d_get_stringk(it.token, key("symbol"));
      if (!name || strlen(name) > 7) return ctx_set_error(ctx, "invalid token name", IN3_EINVAL);
      strcpy(conf->tokens[i].symbol, name);
      bytes_t* adr = d_get_bytesk(it.token, K_ADDRESS);
      if (!adr || !adr->data || adr->len != 20) return ctx_set_error(ctx, "invalid token addr", IN3_EINVAL);
      memcpy(conf->tokens[i].address, adr->data, 20);
    }

    // clean up
    ctx_remove_required(ctx, ctx_find_required(ctx, "tokens"), false);

    if (cache_name) {
      bytes_t         data = bytes((void*) conf->tokens, conf->token_len * sizeof(zksync_token_t));
      in3_cache_ctx_t cctx = {.ctx = ctx, .key = cache_name, .content = &data};
      TRY(in3_plugin_execute_first_or_none(ctx, PLGN_ACT_CACHE_SET, &cctx))
    }
  }

  if (!token_dst) return IN3_OK;

  for (unsigned int i = 0; i < conf->token_len; i++) {
    if (d_type(token_src) == T_STRING) {
      if (strcmp(d_string(token_src), conf->tokens[i].symbol) == 0) {
        *token_dst = conf->tokens + i;
        return IN3_OK;
      }
    }
    else if (d_type(token_src) == T_BYTES && token_src->len == 20 && memcmp(token_src->data, conf->tokens[i].address, 20) == 0) {
      *token_dst = conf->tokens + i;
      return IN3_OK;
    }
  }

  return ctx_set_error(ctx, "could not find the specifed token", IN3_EFIND);
}