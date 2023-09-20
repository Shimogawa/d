#include <Uefi.h>

#include <Protocol/GraphicsOutput.h>

typedef EFI_GRAPHICS_OUTPUT_BLT_PIXEL bmp_pixel_bgra;
typedef struct {
  UINT8 b;
  UINT8 g;
  UINT8 r;
} __attribute__((packed)) bmp_pixel_bgr;

typedef struct _bmp_header {
  UINT8 sig[2];
  UINT32 size;
  UINT16 reserved1;
  UINT16 reserved2;
  UINT32 offset;
  // dib header
  UINT32 header_size;
  UINT32 width;
  UINT32 height;
  UINT16 planes;
  UINT16 bpp;
  UINT32 compression;
  UINT32 image_size;
  UINT32 x_res;
  UINT32 y_res;
  UINT32 colors;
  UINT32 important_colors;
} __attribute__((packed)) bmp_header;

typedef struct _bmp_image {
  bmp_header header;
  void* data;
} bmp_image;

static inline BOOLEAN bmp_is_bgr(bmp_image* image) {
  return image->header.bpp == 24;
}

bmp_image* bmp_load(void* file, UINTN size);
void bmp_free(bmp_image* image);

void bmp_bufcp_bgr_to_bgra(bmp_image* image, bmp_pixel_bgra* buf, UINTN size);
void bmp_bufcp_bgra(bmp_image* image, bmp_pixel_bgra* buf, UINTN size);
