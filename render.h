#include <Uefi.h>

#include <Protocol/GraphicsOutput.h>

typedef struct _graphics_output_info {
  EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
  EFI_PHYSICAL_ADDRESS fb_base;
  UINTN fb_size;
  UINT32 stride;
  UINT32 horizontal_res;
  UINT32 vertical_res;
} graphics_output_info;

BOOLEAN get_gop_info_res(IN EFI_HANDLE image_handle, IN UINTN hor, IN UINTN vert, OUT graphics_output_info* info);
BOOLEAN get_gop_info_mode_id(IN EFI_HANDLE image_handle, IN UINTN mode_id, OUT graphics_output_info* info);

BOOLEAN render_buf(
  IN graphics_output_info* info,
  IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL* buf,
  IN UINTN x, IN UINTN y,
  IN UINTN width, IN UINTN height);
