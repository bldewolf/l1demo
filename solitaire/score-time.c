/* GIMP RGB C-Source image dump (score-time.c) */

static const struct {
  unsigned int 	 width;
  unsigned int 	 height;
  unsigned int 	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  unsigned char	 pixel_data[103 * 10 * 3 + 1];
} gimp_image = {
  103, 10, 3,
  "\377\377\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\0\0\0\0\0\0\0\0\0\0\0\0\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\377\377\377\0\0\0\0\0\0\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\0\0\0\0\0\0\0\0\0\0"
  "\0\0\377\377\377\0\0\0\0\0\0\377\377\377\377\377\377\377\377\377\0\0\0\0"
  "\0\0\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377\377\377\0\0\0\0"
  "\0\0\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377\377\377\377\377"
  "\377\377\377\377\0\0\0\0\0\0\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377\377\377\0\0\0\0"
  "\0\0\0\0\0\0\0\0\377\377\377\377\377\377\377\377\377\0\0\0\0\0\0\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\0\0\0\0"
  "\0\0\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\0\0\0\0\0\0\377\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\0\0\0\0\0\0\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\0\0\0\0"
  "\0\0\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\0\0\0\0\0\0\377\377\377\377\377\377\0\0\0\0\0\0\0\0\0\0\0\0\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\0\0\0\0\0\0\0\0\0\0\0\0\377\377\377\377\377\377\377\377"
  "\377\0\0\0\0\0\0\0\0\0\0\0\0\377\377\377\377\377\377\377\377\377\0\0\0\0"
  "\0\0\0\0\0\0\0\0\377\377\377\377\377\377\0\0\0\0\0\0\0\0\0\0\0\0\377\377"
  "\377\377\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377\377"
  "\377\0\0\0\0\0\0\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377\377"
  "\377\377\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377\377\377\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\377\377\377\377\377\377\377"
  "\377\377\377\377\377\0\0\0\0\0\0\0\0\0\0\0\0\377\377\377\377\377\377\377"
  "\377\377\0\0\0\0\0\0\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377\377\377\0\0\0\0\0\0\377"
  "\377\377\0\0\0\0\0\0\0\0\0\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\0\0\0\0\0\0\377\377\377\377\377\377\0\0\0\0\0\0\377\377\377"
  "\0\0\0\0\0\0\377\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377\377\377"
  "\0\0\0\0\0\0\0\0\0\377\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377\377"
  "\377\0\0\0\0\0\0\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\0\0\0\0"
  "\0\0\377\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\0\0\0\0"
  "\0\0\377\377\377\377\377\377\377\377\377\377\377\377\0\0\0\0\0\0\377\377"
  "\377\377\377\377\0\0\0\0\0\0\377\377\377\377\377\377\0\0\0\0\0\0\377\377"
  "\377\377\377\377\0\0\0\0\0\0\377\377\377\377\377\377\0\0\0\0\0\0\377\377"
  "\377\377\377\377\0\0\0\0\0\0\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\0\0\0\0\0\0\377\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377\377"
  "\377\377\377\377\0\0\0\0\0\0\0\0\0\377\377\377\377\377\377\377\377\377\0"
  "\0\0\0\0\0\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\0"
  "\0\0\0\0\0\377\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377\377\377\0"
  "\0\0\0\0\0\377\377\377\377\377\377\377\377\377\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\0\0\0\0\0\0\377"
  "\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\0\0\0\0\0\0\377"
  "\377\377\377\377\377\377\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377"
  "\377\377\0\0\0\0\0\0\377\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377"
  "\377\377\0\0\0\0\0\0\377\377\377\377\377\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\0\0\0\0\0\0\377"
  "\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377\377\377\0\0\0\0\0\0\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\0\0\0\0\0\0\377"
  "\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377\377\377\0\0\0\0\0\0\377"
  "\377\377\377\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\0\0"
  "\0\0\0\0\377\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\0\0"
  "\0\0\0\0\377\377\377\377\377\377\377\377\377\377\377\377\0\0\0\0\0\0\377"
  "\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377\377\377\0\0\0\0\0\0\377"
  "\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377\377\377\0\0\0\0\0\0\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377\377\377\0\0\0\0\0\0\0\0"
  "\0\0\0\0\377\377\377\377\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377"
  "\377\377\0\0\0\0\0\0\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\0\0\0\0\0\0\377\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377"
  "\377\377\0\0\0\0\0\0\377\377\377\377\377\377\377\377\377\0\0\0\0\0\0\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377\377\377\0\0\0\0\0\0\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377\377\377\377\377\377\377"
  "\377\377\0\0\0\0\0\0\377\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377"
  "\377\377\0\0\0\0\0\0\377\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377"
  "\377\377\0\0\0\0\0\0\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377"
  "\377\377\0\0\0\0\0\0\0\0\0\0\0\0\377\377\377\377\377\377\377\377\377\0\0"
  "\0\0\0\0\377\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377\377\377\0\0"
  "\0\0\0\0\377\377\377\0\0\0\0\0\0\377\377\377\377\377\377\0\0\0\0\0\0\377"
  "\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377\377\377\377\377\377\0\0"
  "\0\0\0\0\377\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377\377\377\0\0"
  "\0\0\0\0\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\0\0\0\0\0\0\377\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\0\0\0\0\0\0\377\377\377\377\377\377\377\377\377\377\377\377\0\0"
  "\0\0\0\0\377\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377\377\377\0\0"
  "\0\0\0\0\377\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377\377\377\0\0"
  "\0\0\0\0\377\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377\377\377\0\0"
  "\0\0\0\0\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\0\0\0\0\0\0\377\377\377\377\377\377\0\0\0\0\0\0\377\377\377\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\377\377\377\377\377\377\377\377\377\377\377\377"
  "\0\0\0\0\0\0\0\0\0\0\0\0\377\377\377\377\377\377\377\377\377\0\0\0\0\0\0"
  "\0\0\0\0\0\0\377\377\377\377\377\377\377\377\377\0\0\0\0\0\0\377\377\377"
  "\377\377\377\377\377\377\377\377\377\0\0\0\0\0\0\0\0\0\0\0\0\377\377\377"
  "\377\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\0\0\0\0\0\0\0\0\0\0\0\0"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377\377\377"
  "\377\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377\377\377\0\0\0\0\0\0"
  "\377\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377\377\377\0\0\0\0\0\0"
  "\377\377\377\377\377\377\377\377\377\0\0\0\0\0\0\0\0\0\0\0\0\377\377\377"
  "\377\377\377\377\377\377\0\0\0\0\0\0\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\0\0\0\0\0\0\0\0\0\0\0\0"
  "\377\377\377",
};
