#include <Uefi.h>

#define IFERR_RETURN_NULL(status, message) \
  clear_error();                           \
  if (EFI_ERROR(status)) {                 \
    set_error((status), (message));        \
    return NULL;                           \
  }

#define IFERR_RETURN(status, message, ret) \
  clear_error();                           \
  if (EFI_ERROR(status)) {                 \
    set_error((status), (message));        \
    return (ret);                          \
  }

#define IF_FAIL_RETURN_NULL(ptr) \
  if (!(ptr)) {                  \
    return NULL;                 \
  }

#define IF_FAIL_RETURN_FALSE(ptr) \
  if (!(ptr)) {                   \
    return FALSE;                 \
  }

#define IFERR_RETURN_STATUS() \
  if (has_error()) {          \
    return get_error(NULL);   \
  }

void clear_error();
void set_error(IN EFI_STATUS status, IN OPTIONAL const CHAR16* message);
EFI_STATUS get_error(OUT const CHAR16** message);
const CHAR16* get_error_message();
BOOLEAN has_error();
