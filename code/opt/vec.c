#define IDENT 0
#define OP +
#include "vec.h"
#include <stdio.h>
#include <stdlib.h>
/* Create vector of specified length */
vec_ptr new_vec(long len) {
  /* Allocate header structure */
  vec_ptr result = (vec_ptr)malloc(sizeof(vec_rec));
  data_t *data = NULL;
  if (!result) {
    return NULL; /* could not allocate storeage */
  }
  result->len = len;
  /* Allocate Array */
  if (len > 0) {
    data = (data_t *)calloc(len, sizeof(data_t));
    if (!data) {
      free((void *)result);
      return NULL; /* Couldn't allocate storage */
    }
  }
  /* Data will either be NULL or allocated array */
  result->data = data;
  return result;
}
/* Retrieve vector element and store at dest, Return 0(out of bounds) or 1
 * (successful)*/
int get_vec_element(vec_ptr v, long index, data_t *dest) {
  if (index < 0 || index >= v->len) {
    return 0;
  }
  *dest = v->data[index];
  return 1;
}

/* Return length of vector */
long vec_length(vec_ptr v) { return v->len; }

/* Implementation with maximum use of data abstraction */
void combine1(vec_ptr v, data_t *dest) {
  long i;
  *dest = IDENT;
  for (i = 0; i < vec_length(v); i++) {
    data_t val;
    get_vec_element(v, i, &val);
    *dest = *dest OP val;
  }
}

/* Move call to vec_length out of loop */
void combine2(vec_ptr v, data_t *dest) {
  long i;
  *dest = IDENT;
  long length = vec_length(v);
  for (i = 0; i < length; i++) {
    data_t val;
    get_vec_element(v, i, &val);
    *dest = *dest OP val;
  }
}
data_t *get_vec_start(vec_ptr v) { return v->data; }

/* Direct access to vector data */
void combine3(vec_ptr v, data_t *dest) {
  long i;
  long length = vec_length(v);
  data_t *data = get_vec_start(v);
  *dest = IDENT;
  for (i = 0; i < length; i++) {
    *dest = *dest OP data[i];
  }
}

/* Accumulate result in local variable */
void combine4(vec_ptr v, data_t *dest) {
  long i;
  long length = vec_length(v);
  data_t *data = get_vec_start(v);
  data_t acc = IDENT;
  for (i = 0; i < length; i++) {
    acc = acc OP data[i];
  }
  *dest = acc;
}
