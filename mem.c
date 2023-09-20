#include "mem.h"
#include "error.h"

inline void* malloc(UINTN size) {
  void* ptr;
  EFI_STATUS status = gBS->AllocatePool(EfiLoaderData, size, &ptr);
  IFERR_RETURN_NULL(status, L"error in malloc");
  return ptr;
}

inline void free(void* ptr) {
  gBS->FreePool(ptr);
}

inline EFI_PHYSICAL_ADDRESS alloc_page(UINTN size) {
  EFI_PHYSICAL_ADDRESS addr;
  EFI_STATUS status = gBS->AllocatePages(AllocateAnyPages, EfiLoaderData, size, &addr);
  IFERR_RETURN(status, L"error in alloc_page", 0);
  return addr;
}
