#include "librust_qstr.h"

mp_obj_t protobuf_type_for_name(mp_obj_t name);
mp_obj_t protobuf_type_for_wire(mp_obj_t wire_id);
mp_obj_t protobuf_decode(mp_obj_t buf, mp_obj_t def,
                         mp_obj_t enable_experimental);
mp_obj_t protobuf_len(mp_obj_t obj);
mp_obj_t protobuf_encode(mp_obj_t buf, mp_obj_t obj);

mp_obj_t ui_layout_new_example(mp_obj_t param);
mp_obj_t ui_layout_pindialog();

extern const mp_obj_type_t UI_RESULT_OBJ_TYPE;

struct _ui_result {
    mp_obj_base_t base;
};

extern const struct _ui_result UI_RESULT_CONFIRMED;
extern const struct _ui_result UI_RESULT_CANCELLED;
