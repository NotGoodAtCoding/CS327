/* You can't forward reference enums or array types, so most of the headers *
 * need this, thus we put it in it's own file.                              */

typedef enum dim {
  dim_x,
  dim_y,
  num_dims
} dim_t;

typedef int8_t pair_t[num_dims];

