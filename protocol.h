#include <Uefi.h>

#include <Library/UefiBootServicesTableLib.h>

void* open_protocol(EFI_HANDLE image_handle, EFI_GUID* protocol);
