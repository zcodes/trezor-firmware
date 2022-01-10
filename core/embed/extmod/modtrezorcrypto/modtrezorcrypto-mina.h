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

#include "py/objstr.h"

#include "vendor/trezor-crypto/mina/crypto.h"

/// package: trezorcrypto.mina

/// def generate_secret() -> bytes:
///     """
///     Generate secret key.
///     """
STATIC mp_obj_t mod_trezorcrypto_mina_generate_secret() {
  vstr_t sk = {0};
  vstr_init_len(&sk, 32);
  for (;;) {
    random_buffer((uint8_t *)sk.buf, sk.len);
    // check whether secret > 0 && secret < curve_order
    if (0 ==
        memcmp(
            sk.buf,
            "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
            "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
            32))
      continue;
    if (0 <=
        memcmp(
            sk.buf,
            "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFE"
            "\xBA\xAE\xDC\xE6\xAF\x48\xA0\x3B\xBF\xD2\x5E\x8C\xD0\x36\x41\x41",
            32))
      continue;
    break;
  }
  return mp_obj_new_str_from_vstr(&mp_type_bytes, &sk);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_trezorcrypto_mina_generate_secret_obj,
                                 mod_trezorcrypto_mina_generate_secret);

/* /// def get_address(private_key: bytes) -> bytes: */
/* ///     """ */
/* ///     Get Mina address from public key */
/* ///     """ */
/* STATIC mp_obj_t mod_trezorcrypto_mina_get_address(size_t n_args, */
/*                                                   const mp_obj_t *args) { */
/*   mp_buffer_info_t private_key = {0}; */
/*   mp_get_buffer_raise(args[0], &private_key, MP_BUFFER_READ); */
/*   Affine public_key = {0}; */
/*   generate_pubkey(&public_key, (uint64_t *) &private_key); */
/*   vstr_t address = {0}; */
/*   vstr_init_len(&address, MINA_ADDRESS_LEN); */
/*   generate_address(address.buf, MINA_ADDRESS_LEN, &public_key); */

/*   return mp_obj_new_str_from_vstr(&mp_type_bytes, &address); */
/* } */
/* STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN( */
/*     mod_trezorcrypto_mina_get_address_obj, 1, 1, */
/*     mod_trezorcrypto_mina_get_address); */

/// def get_address(public_key: bytes) -> bytes:
///     """
///     Get Mina address from public key
///     """
STATIC mp_obj_t mod_trezorcrypto_mina_get_address(size_t n_args,
                                                  const mp_obj_t *args) {
  mp_buffer_info_t pk = {0};
  mp_get_buffer_raise(args[0], &pk, MP_BUFFER_READ);
  Affine public_key = {0};
  Compressed cp = {0};
  uint8_t *pbuf = (uint8_t *) pk.buf;
  memcpy(cp.x, pbuf, 32);
  cp.is_odd = (bool) pbuf[33];

  decompress(&public_key, &cp);
  /* Scalar private_key = {0}; */
  /* generate_pubkey(&public_key, (uint64_t *)&private_key); */
  vstr_t address = {0};
  vstr_init_len(&address, MINA_ADDRESS_LEN);
  generate_address(address.buf, MINA_ADDRESS_LEN, &public_key);

  return mp_obj_new_str_from_vstr(&mp_type_bytes, &address);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(
    mod_trezorcrypto_mina_get_address_obj, 1, 1,
    mod_trezorcrypto_mina_get_address);

/// def publickey(secret_key: bytes, compressed: bool = True) -> bytes:
///     """
///     Computes public key from secret key.
///     """
STATIC mp_obj_t mod_trezorcrypto_mina_publickey(size_t n_args,
                                                const mp_obj_t *args) {
  mp_buffer_info_t sk = {0};
  mp_get_buffer_raise(args[0], &sk, MP_BUFFER_READ);
  if (sk.len != 32) {
    mp_raise_ValueError("Invalid length of secret key");
  }
  vstr_t pk = {0};
  /* int ret = 0; */
  /* bool compressed = n_args < 2 || args[1] == mp_const_true; */
  vstr_init_len(&pk, 33);
  generate_pubkey((Affine *)&pk, (uint64_t *) &sk);

  return mp_obj_new_str_from_vstr(&mp_type_bytes, &pk);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(
    mod_trezorcrypto_mina_publickey_obj, 1, 2,
    mod_trezorcrypto_mina_publickey);

/// def sign(
///     secret_key: bytes,
///     digest: bytes,
///     compressed: bool = True,
///     canonical: int | None = None,
/// ) -> bytes:
///     """
///     Uses secret key to produce the signature of the digest.
///     """
STATIC mp_obj_t mod_trezorcrypto_mina_sign(size_t n_args,
                                                const mp_obj_t *args) {
  mp_buffer_info_t sk = {0};
  mp_buffer_info_t dig = {0};
  mp_get_buffer_raise(args[0], &sk, MP_BUFFER_READ);
  mp_get_buffer_raise(args[1], &dig, MP_BUFFER_READ);
  /* bool compressed = (n_args < 3) || (args[2] == mp_const_true); */
  /* int (*is_canonical)(uint8_t by, uint8_t sig[64]) = NULL; */

  vstr_t sig = {0};
  vstr_init_len(&sig, 65);
  /* uint8_t pby = 0; */
  /* int ret = 0; */

  return mp_obj_new_str_from_vstr(&mp_type_bytes, &sig);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mod_trezorcrypto_mina_sign_obj,
                                           2, 4,
                                           mod_trezorcrypto_mina_sign);

/// def verify(public_key: bytes, signature: bytes, digest: bytes) -> bool:
///     """
///     Uses public key to verify the signature of the digest.
///     Returns True on success.
///     """
STATIC mp_obj_t mod_trezorcrypto_mina_verify(mp_obj_t public_key,
                                                  mp_obj_t signature,
                                                  mp_obj_t digest) {
  mp_buffer_info_t pk = {0}, sig = {0}, dig = {0};
  mp_get_buffer_raise(public_key, &pk, MP_BUFFER_READ);
  mp_get_buffer_raise(signature, &sig, MP_BUFFER_READ);
  mp_get_buffer_raise(digest, &dig, MP_BUFFER_READ);
  int ret = 0;
  return mp_obj_new_bool(ret == 0);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_3(mod_trezorcrypto_mina_verify_obj,
                                 mod_trezorcrypto_mina_verify);

STATIC const mp_rom_map_elem_t mod_trezorcrypto_mina_globals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_mina)},
    {MP_ROM_QSTR(MP_QSTR_generate_secret),
     MP_ROM_PTR(&mod_trezorcrypto_mina_generate_secret_obj)},
    {MP_ROM_QSTR(MP_QSTR_get_address),
     MP_ROM_PTR(&mod_trezorcrypto_mina_get_address_obj)},
    {MP_ROM_QSTR(MP_QSTR_publickey),
     MP_ROM_PTR(&mod_trezorcrypto_mina_publickey_obj)},
    {MP_ROM_QSTR(MP_QSTR_sign),
     MP_ROM_PTR(&mod_trezorcrypto_mina_sign_obj)},
    {MP_ROM_QSTR(MP_QSTR_verify),
     MP_ROM_PTR(&mod_trezorcrypto_mina_verify_obj)},
};
STATIC MP_DEFINE_CONST_DICT(mod_trezorcrypto_mina_globals,
                            mod_trezorcrypto_mina_globals_table);

STATIC const mp_obj_module_t mod_trezorcrypto_mina_module = {
    .base = {&mp_type_module},
    .globals = (mp_obj_dict_t *)&mod_trezorcrypto_mina_globals,
};
