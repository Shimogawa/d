#include <Uefi.h>

#include <Library/UefiBootServicesTableLib.h>

void* malloc(UINTN size);

void free(void* ptr);

EFI_PHYSICAL_ADDRESS alloc_page(UINTN size);
