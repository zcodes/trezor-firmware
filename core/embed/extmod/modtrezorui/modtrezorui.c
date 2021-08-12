/*
 * This file is part of the Trezor project, https://trezor.io/
 *
 * Copyright (c) SatoshiLabs
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "py/runtime.h"

#if MICROPY_PY_TREZORUI

#include "librust.h"
#include "modtrezorui-display.h"

/// class Layout:
///     """Rust layout"""
///     def set_timer_fn(self, fn: Callable[[int, int], None]) -> None:
///         """Set timer callback"""
///
///     def touch_start(self, x: int, y: int) -> None:
///         """Send touch start event"""
///
///     def touch_move(self, x: int, y: int) -> None:
///         """Send touch move event"""
///
///     def touch_end(self, x: int, y: int) -> None:
///         """Send touch end event"""
///
///     def timer(self, token: int) -> None:
///         """Timer tick event"""
///
///     def paint(self) -> None:
///         """Redraw layout"""

/// mock:global

/// def layout_new_example(label: str) -> Layout:
///     """Rust example layout"""
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_trezorui_layout_new_example_obj,
                                 ui_layout_new_example);

/// def layout_pindialog() -> Layout:
///     """Rust pin dialog layout"""
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_trezorui_layout_pindialog_obj,
                                 ui_layout_pindialog);


STATIC const mp_rom_map_elem_t mp_module_trezorui_globals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_trezorui)},
    {MP_ROM_QSTR(MP_QSTR_Display), MP_ROM_PTR(&mod_trezorui_Display_type)},

    {MP_ROM_QSTR(MP_QSTR_layout_new_example),
     MP_ROM_PTR(&mod_trezorui_layout_new_example_obj)},
    {MP_ROM_QSTR(MP_QSTR_layout_pindialog),
     MP_ROM_PTR(&mod_trezorui_layout_pindialog_obj)},

    {MP_ROM_QSTR(MP_QSTR_UiResult), MP_ROM_PTR(&UI_RESULT_OBJ_TYPE)},
    {MP_ROM_QSTR(MP_QSTR_CONFIRMED), MP_ROM_PTR(&UI_RESULT_CONFIRMED)},
    {MP_ROM_QSTR(MP_QSTR_CANCELLED), MP_ROM_PTR(&UI_RESULT_CANCELLED)},
};

STATIC MP_DEFINE_CONST_DICT(mp_module_trezorui_globals,
                            mp_module_trezorui_globals_table);

const mp_obj_module_t mp_module_trezorui = {
    .base = {&mp_type_module},
    .globals = (mp_obj_dict_t*)&mp_module_trezorui_globals,
};

MP_REGISTER_MODULE(MP_QSTR_trezorui, mp_module_trezorui, MICROPY_PY_TREZORUI);

#endif  // MICROPY_PY_TREZORUI
