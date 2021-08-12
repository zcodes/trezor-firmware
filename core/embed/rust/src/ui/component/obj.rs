use super::base::{DialogResult, PromptResult};
use crate::micropython::{
    obj::{Obj, ObjBase},
    qstr::Qstr,
    typ::Type,
};

#[repr(C)]
pub struct UiResultObj {
    base: ObjBase,
}

unsafe impl Sync for UiResultObj {}

impl From<&'static UiResultObj> for Obj {
    fn from(val: &'static UiResultObj) -> Self {
        // SAFETY:
        //  - we are 'static
        //  - we are `repr(C)`
        //  - we have a `base` as the first field with the correct type
        unsafe { Obj::from_static(val) }
    }
}

#[no_mangle]
pub static UI_RESULT_OBJ_TYPE: Type = obj_type! {name: Qstr::MP_QSTR_UiResult, };
#[no_mangle]
pub static UI_RESULT_CONFIRMED: UiResultObj = UiResultObj {
    base: UI_RESULT_OBJ_TYPE.as_base(),
};
#[no_mangle]
pub static UI_RESULT_CANCELLED: UiResultObj = UiResultObj {
    base: UI_RESULT_OBJ_TYPE.as_base(),
};

impl From<DialogResult> for Obj {
    fn from(val: DialogResult) -> Self {
        match val {
            DialogResult::Confirmed => (&UI_RESULT_CONFIRMED).into(),
            DialogResult::Cancelled => (&UI_RESULT_CANCELLED).into(),
        }
    }
}

impl<const MAX: usize> From<PromptResult<MAX>> for Obj {
    fn from(val: PromptResult<MAX>) -> Self {
        match val {
            PromptResult::Confirmed(response) => response.into(),
            PromptResult::Cancelled => (&UI_RESULT_CANCELLED).into(),
        }
    }
}
