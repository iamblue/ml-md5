#include <string.h>

#include "jerry-api.h"
#include "hal_md5.h"
#include "microlattice.h"

DELCARE_HANDLER(__md5) {
  char str_buffer [50];
  if (args_cnt == 1 && jerry_value_is_string(args_p[0])) {
    jerry_size_t value_req_sz = jerry_get_string_size(args_p[0]);
    jerry_char_t value_buffer[value_req_sz];
    jerry_string_to_char_buffer (args_p[0], value_buffer, value_req_sz);
    value_buffer[value_req_sz] = '\0';

    uint8_t digest[HAL_MD5_DIGEST_SIZE] = {0};

    hal_md5_context_t context = {0};
    hal_md5_init(&context);
    hal_md5_append(&context, value_buffer, strlen(value_buffer));
    hal_md5_end(&context, digest);

    uint8_t i;
    strcpy(str_buffer, "");
    for (i = 0; i < sizeof(digest); i++) {
      if (i % 16 == 0) {
          printf("\r\n");
      }
      char buffer [2];
      sprintf (buffer, "%02x", digest[i]);
      strcat(str_buffer, buffer);
    }
  }
  return jerry_create_string(str_buffer);
}

void ml_md5_init(void) {
  REGISTER_HANDLER(__md5);
}