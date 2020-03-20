extern crate in3_sys;

struct In3 {
    ptr: *mut in3_sys::in3_t,
}

impl In3 {
    fn new() -> In3 {
        unsafe {
            In3 { ptr: in3_sys::in3_for_chain_auto_init(1) }
        }
    }
}

impl Drop for In3 {
    fn drop(&mut self) {
        unsafe {
            in3_sys::in3_free(self.ptr);
        }
    }
}

#[cfg(test)]
mod tests {
    use std::ffi;

    use super::*;

    #[test]
    fn test_eth_block_number() {
        unsafe {
            in3_sys::eth_blockNumber(In3::new().ptr);
        }
    }

    #[test]
    fn test_eth_get_balance() {
        let mut null: *mut i8 = std::ptr::null_mut();
        let mut res: *mut *mut i8 = &mut null;
        let err: *mut *mut i8 = &mut null;
        unsafe {
            let _ = in3_sys::in3_client_rpc(In3::new().ptr, ffi::CString::new("eth_getBalance").unwrap().as_ptr(),
                                            ffi::CString::new("[\"0xc94770007dda54cF92009BFF0dE90c06F603a09f\", \"latest\"]").unwrap().as_ptr(), res, err);
            // to view run with `cargo test -- --nocapture`
            println!("------> balance: {}", ffi::CStr::from_ptr(*res).to_str().unwrap());
        }
    }
}