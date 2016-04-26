#include <string.h>

#include "jerry.h"
#include "hal_md5.h"
#include "microlattice.h"


// char md5_result_dump(uint8_t *result, uint8_t length)
// {
//     uint8_t i;
//     char str_buffer [50];
//     strcpy(str_buffer, "");
//     for (i = 0; i < length; i++) {
//       if (i % 16 == 0) {
//           printf("\r\n");
//       }
//       char buffer [2];
//       sprintf (buffer, "%02x", result[i]);
//       strcat(str_buffer, buffer);
//     }
//     return str_buffer;
// }

DELCARE_HANDLER(__md5) {
  if (args_cnt == 1 && args_p[0].type == JERRY_API_DATA_TYPE_STRING) {

    // int value_req_sz = jerry_api_string_to_char_buffer(args_p[0].v_string, NULL, 0);
    // value_req_sz *= -1;
    // char value_buffer [value_req_sz+1]; // 不能有*
    // value_req_sz = jerry_api_string_to_char_buffer (args_p[0].v_string, (jerry_api_char_t *) value_buffer, value_req_sz);
    // value_buffer[value_req_sz] = '\0';

    int value_req_sz = -jerry_api_string_to_char_buffer(args_p[0].v_string, NULL, 0);
    char * value_buffer = (char*) malloc (value_req_sz);
    value_req_sz = jerry_api_string_to_char_buffer (args_p[0].v_string, (jerry_api_char_t *) value_buffer, value_req_sz);
    value_buffer[value_req_sz] = '\0';

    uint8_t digest[HAL_MD5_DIGEST_SIZE] = {0};

    hal_md5_context_t context = {0};
    hal_md5_init(&context);
    hal_md5_append(&context, value_buffer, strlen(value_buffer));
    hal_md5_end(&context, digest);

    uint8_t i;
    char str_buffer [50];
    strcpy(str_buffer, "");
    for (i = 0; i < sizeof(digest); i++) {
      if (i % 16 == 0) {
          printf("\r\n");
      }
      char buffer [2];
      sprintf (buffer, "%02x", digest[i]);
      strcat(str_buffer, buffer);
    }

    jerry_api_string_t *result = jerry_api_create_string(str_buffer);
    ret_val_p->type = JERRY_API_DATA_TYPE_STRING;
    ret_val_p->v_string = result;
    free(value_buffer);
    return true;
  }
}

void ml_md5_init(void) {
  REGISTER_HANDLER(__md5);
}