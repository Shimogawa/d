#include "file.h"

#include "Library/UefiLib.h"
#include "error.h"
#include "protocol.h"
#include <Library/UefiBootServicesTableLib.h>

EFI_FILE_PROTOCOL* open_file(IN EFI_HANDLE image_handle, IN CHAR16* filename, IN UINT64 mode, IN UINT64 attributes) {
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* fs = open_protocol(image_handle, &gEfiSimpleFileSystemProtocolGuid);
  EFI_FILE_PROTOCOL* root;
  EFI_STATUS status = fs->OpenVolume(fs, &root);
  IFERR_RETURN_NULL(status, L"OpenVolume failed");
  EFI_FILE_PROTOCOL* file;
  status = root->Open(
    root,
    &file,
    filename,
    mode,
    attributes);
  IFERR_RETURN_NULL(status, L"Open failed");
  return file;
}

EFI_FILE_PROTOCOL* open_file_readonly(
  IN EFI_HANDLE image_handle,
  IN CHAR16* filename) {
  return open_file(image_handle, filename, EFI_FILE_MODE_READ, 0);
}

BOOLEAN stat_file(IN EFI_FILE_PROTOCOL* file, IN UINTN buf_size, OUT EFI_FILE_INFO* info) {
  EFI_STATUS status = file->GetInfo(file, &gEfiFileInfoGuid, &buf_size, info);
  IFERR_RETURN(status, L"GetInfo failed", FALSE);
  return TRUE;
}

BOOLEAN read_file(IN EFI_FILE_PROTOCOL* file, IN UINTN size, IN OUT VOID* buffer) {
  EFI_STATUS status = file->Read(file, &size, (VOID*) buffer);
  IFERR_RETURN(status, L"Read failed", FALSE);
  return TRUE;
}

void close_file(IN EFI_FILE_PROTOCOL* file) {
  file->Close(file);
}
