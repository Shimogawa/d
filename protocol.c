#include "protocol.h"
#include "error.h"

inline VOID* open_protocol(EFI_HANDLE image_handle, EFI_GUID* protocol) {
  UINTN handle_count;
  EFI_HANDLE* handles;
  EFI_STATUS status = gBS->LocateHandleBuffer(
    ByProtocol,
    protocol,
    NULL,
    &handle_count,
    &handles);
  IFERR_RETURN_NULL(status, L"LocateHandleBuffer failed");
  VOID* interface;
  status = gBS->OpenProtocol(
    handles[0],
    protocol,
    &interface,
    image_handle,
    NULL,
    EFI_OPEN_PROTOCOL_GET_PROTOCOL);
  IFERR_RETURN_NULL(status, L"OpenProtocol failed");
  return interface;
}
