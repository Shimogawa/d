#include <Uefi.h>

#include <Guid/FileInfo.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Protocol/SimpleFileSystem.h>

#include "bmp.h"
#include "error.h"
#include "file.h"
#include "mem.h"
#include "render.h"

EFI_STATUS draw_img(EFI_HANDLE image_handle, graphics_output_info* info, CHAR16* filename) {
  EFI_FILE_PROTOCOL* file = open_file_readonly(image_handle, filename);
  IFERR_RETURN_STATUS();
  EFI_FILE_INFO* file_info = malloc(sizeof(EFI_FILE_INFO) + 128);
  stat_file(file, sizeof(EFI_FILE_INFO) + 128, file_info);
  IFERR_RETURN_STATUS();
  UINTN size = file_info->FileSize;
  free(file_info);
  VOID* buffer = malloc(size);
  read_file(file, size, buffer);
  IFERR_RETURN_STATUS();
  close_file(file);

  bmp_image* img = bmp_load(buffer, size);
  IFERR_RETURN_STATUS();

  Print(L"Image: %ux%u\n", img->header.width, img->header.height);

  bmp_pixel_bgra* buf = malloc(img->header.image_size);
  bmp_bufcp_bgr_to_bgra(img, buf, img->header.image_size);
  IFERR_RETURN_STATUS();

  UINTN x = (info->horizontal_res - img->header.width) / 2;
  UINTN y = (info->vertical_res - img->header.height) / 2;
  render_buf(info, buf, x, y, img->header.width, img->header.height);

  free(buf);

  bmp_free(img);

  free(buffer);
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
uefi_main(
  IN EFI_HANDLE image_handle,
  IN EFI_SYSTEM_TABLE* system_table) {
  graphics_output_info info;
  get_gop_info_res(image_handle, 1920, 1080, &info);
  IFERR_RETURN_STATUS();
  if (draw_img(image_handle, &info, L"\\test.bmp")) {
    Print(L"draw_img failed: (%r) %s\n", get_error(NULL), get_error_message());
    return EFI_ABORTED;
  }

  return EFI_SUCCESS;
}
