#include <Uefi.h>

#include <Guid/FileInfo.h>
#include <Library/UefiLib.h>
#include <Protocol/SimpleFileSystem.h>

EFI_FILE_PROTOCOL* open_file(
  IN EFI_HANDLE image_handle,
  IN CHAR16* filename,
  IN UINT64 mode,
  IN UINT64 attributes);
EFI_FILE_PROTOCOL* open_file_readonly(
  IN EFI_HANDLE image_handle,
  IN CHAR16* filename);
BOOLEAN stat_file(IN EFI_FILE_PROTOCOL* file, IN UINTN buf_size, OUT EFI_FILE_INFO* info);
BOOLEAN read_file(IN EFI_FILE_PROTOCOL* file, IN UINTN size, IN OUT VOID* buffer);

void close_file(IN EFI_FILE_PROTOCOL* file);
