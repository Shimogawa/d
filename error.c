#include "error.h"

static EFI_STATUS error_status;
static OPTIONAL const CHAR16* error_message;

void clear_error() {
  error_status = EFI_SUCCESS;
  error_message = NULL;
}

void set_error(IN EFI_STATUS status, IN OPTIONAL const CHAR16* message) {
  error_status = status;
  error_message = message;
}

EFI_STATUS get_error(OUT const CHAR16** message) {
  if (message) {
    *message = error_message;
  }
  return error_status;
}

const CHAR16* get_error_message() {
  return error_message;
}

inline BOOLEAN has_error() {
  return error_status != EFI_SUCCESS;
}
