/* bytearray_stubs.c : C routines for bytearray.ml */
/* Copyright Jérôme Vouillon 1999-2010 (see LICENCE for distribution conditions) */

#define CAML_NAME_SPACE

#include <string.h>

#include "caml/intext.h"
#include "caml/bigarray.h"

#define Array_data(a, i) (((char *) a->data) + Long_val(i))
#define Floatarray_data(a, i) (((char *) a->data) + 8 * Long_val(i))

CAMLprim value ml_marshal_to_bigarray(value v, value flags)
{
  char *buf;
  long len;
  caml_output_value_to_malloc(v, flags, &buf, &len);
  return caml_ba_alloc(CAML_BA_UINT8 | CAML_BA_C_LAYOUT | CAML_BA_MANAGED,
                       1, buf, &len);
}

CAMLprim value ml_marshal_to_bigarray_buffer(value b, value ofs,
                                             value v, value flags)
{
  struct caml_ba_array *b_arr = Caml_ba_array_val(b);
  return Val_long(caml_output_value_to_block(v, flags, Array_data (b_arr, ofs),
					     b_arr->dim[0] - Long_val(ofs)));
}


CAMLprim value ml_unmarshal_from_bigarray(value b, value ofs)
{
  struct caml_ba_array *b_arr = Caml_ba_array_val(b);
  return caml_input_value_from_block (Array_data (b_arr, ofs),
                                      b_arr->dim[0] - Long_val(ofs));
}

CAMLprim value ml_blit_string_to_bigarray
(value s, value i, value a, value j, value l)
{
  const char *src = String_val(s) + Int_val(i);
  char *dest = Array_data(Caml_ba_array_val(a), j);
  memcpy(dest, src, Long_val(l));
  return Val_unit;
}

CAMLprim value ml_blit_floatarray_to_bigarray
(value fa, value i, value a, value j, value l)
{
  int w = 8;
  char *src = Bp_val(fa) + Long_val(i)*w;
  char *dest = Floatarray_data(Caml_ba_array_val(a), j);
  memcpy(dest, src, Long_val(l)*w);
  return Val_unit;
}

CAMLprim value ml_blit_bigarray_to_floatarray
(value a, value i, value fa, value j, value l)
{
  int w = 8;
  char *src = Floatarray_data(Caml_ba_array_val(a), i);
  char *dest = Bp_val(fa) + Long_val(j)*w;
  memcpy(dest, src, Long_val(l)*w);
  return Val_unit;
}
