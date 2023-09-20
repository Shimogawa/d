#include "render.h"
#include "error.h"
#include "protocol.h"

#include <Library/UefiBootServicesTableLib.h>

BOOLEAN get_gop_info_mode_id(IN EFI_HANDLE image_handle, IN UINTN mode_id, OUT graphics_output_info* info) {
  EFI_GRAPHICS_OUTPUT_PROTOCOL* gop = open_protocol(NULL, &gEfiGraphicsOutputProtocolGuid);
  IF_FAIL_RETURN_FALSE(gop);
  if (mode_id >= gop->Mode->MaxMode) {
    set_error(EFI_INVALID_PARAMETER, L"Invalid mode ID");
    return FALSE;
  }
  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* gop_mode_info;
  UINTN info_size = sizeof(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION);
  EFI_STATUS status = gop->QueryMode(gop, mode_id, &info_size, &gop_mode_info);
  IFERR_RETURN(status, L"QueryMode failed", FALSE);
  status = gop->SetMode(gop, mode_id);
  IFERR_RETURN(status, L"SetMode failed", FALSE);

  info->gop = gop;
  info->horizontal_res = gop_mode_info->HorizontalResolution;
  info->vertical_res = gop_mode_info->VerticalResolution;
  info->stride = gop_mode_info->PixelsPerScanLine;
  info->fb_base = gop->Mode->FrameBufferBase;
  info->fb_size = gop->Mode->FrameBufferSize;
  return TRUE;
}

BOOLEAN get_gop_info_res(IN EFI_HANDLE image_handle, IN UINTN hor, IN UINTN vert, OUT graphics_output_info* info) {
  EFI_GRAPHICS_OUTPUT_PROTOCOL* gop = open_protocol(NULL, &gEfiGraphicsOutputProtocolGuid);
  IF_FAIL_RETURN_FALSE(gop);
  for (UINTN i = 0; i < gop->Mode->MaxMode; i++) {
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* gop_mode_info;
    UINTN info_size = sizeof(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION);
    EFI_STATUS status = gop->QueryMode(gop, i, &info_size, &gop_mode_info);
    IFERR_RETURN(status, L"QueryMode failed", FALSE);
    if (gop_mode_info->HorizontalResolution == hor && gop_mode_info->VerticalResolution == vert) {
      status = gop->SetMode(gop, i);
      IFERR_RETURN(status, L"SetMode failed", FALSE);

      info->gop = gop;
      info->horizontal_res = gop_mode_info->HorizontalResolution;
      info->vertical_res = gop_mode_info->VerticalResolution;
      info->stride = gop_mode_info->PixelsPerScanLine;
      info->fb_base = gop->Mode->FrameBufferBase;
      info->fb_size = gop->Mode->FrameBufferSize;
      return TRUE;
    }
  }

  set_error(EFI_NOT_FOUND, L"Could not find mode with resolution");
  return FALSE;
}

BOOLEAN render_buf(
  IN graphics_output_info* info,
  IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL* buf,
  IN UINTN x, IN UINTN y,
  IN UINTN width, IN UINTN height) {
  if (width > info->horizontal_res || height > info->vertical_res) {
    set_error(EFI_INVALID_PARAMETER, L"Buffer too large");
    return FALSE;
  }

  EFI_STATUS status = info->gop->Blt(
    info->gop,
    buf,
    EfiBltBufferToVideo,
    0, 0,
    x, y,
    width, height,
    0);
  IFERR_RETURN(status, L"Blt failed", FALSE);
  return TRUE;
}
