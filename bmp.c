#include "bmp.h"
#include "error.h"
#include "mem.h"

#include <Library/UefiLib.h>

#define BMP_MAGIC ((UINT16) 0x4D42)

bmp_image* bmp_load(void* buf, UINTN size) {
  bmp_header header = *(bmp_header*) buf;
  if (*(UINT16*) header.sig != BMP_MAGIC) {
    set_error(EFI_INVALID_PARAMETER, L"Invalid BMP magic");
    return NULL;
  }
  // does not support 24-bit bgr for now
  if (header.bpp != 32 && header.bpp != 24) {
    set_error(EFI_INVALID_PARAMETER, L"Invalid BMP bpp: != 32 or 24");
    return NULL;
  }
  if (size != header.size) {
    set_error(EFI_INVALID_PARAMETER, L"Invalid BMP size");
    return NULL;
  }
  bmp_image* image = malloc(sizeof(bmp_image));
  image->header = header;
  image->data = (UINT8*) buf + header.offset;
  return image;
}

void bmp_free(bmp_image* image) {
  free(image);
}

void bmp_bufcp_bgra(bmp_image* image, bmp_pixel_bgra* buf, UINTN size) {
  UINTN image_size = image->header.image_size;
  if (size < image_size) {
    set_error(EFI_INVALID_PARAMETER, L"buffer too small");
    return;
  }
  for (UINTN i = image_size - 4, j = 0; i >= 0; i -= 4, j++) {
    buf[j] = *(bmp_pixel_bgra*) ((UINT8*) image->data + i);
  }
}

void bmp_bufcp_bgr_to_bgra(bmp_image* image, bmp_pixel_bgra* buf, UINTN size) {
  UINTN image_size = image->header.image_size;
  if (size < image_size) {
    set_error(EFI_INVALID_PARAMETER, L"buffer too small");
    return;
  }
  UINTN padding = image->header.width % 4;
  for (UINTN i = 0, j = 0; i < image_size; i += 3, j++) {
    if (padding > 0 && i % image->header.width == 0) {
      i += padding;
    }
    bmp_pixel_bgr* pixel = (bmp_pixel_bgr*) ((UINT8*) image->data + i);
    buf[j].Blue = pixel->b;
    buf[j].Green = pixel->g;
    buf[j].Red = pixel->r;
    buf[j].Reserved = 0;
  }
}
