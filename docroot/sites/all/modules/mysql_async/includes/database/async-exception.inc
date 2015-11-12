<?php
/**
 * @file
 * Exception class.
 */

/**
 * Mysql Async exception class.
 */
class MysqlAsyncException extends PDOException {
  const E_KEY_REUSE          = 0x00000001;
  const E_TIMEOUT            = 0x00000002;
  const E_MODE_UNSUPPORTED   = 0x00000003;
  const E_METHOD_UNSUPPORTED = 0x00000004;
  const E_RETURN_UNSUPPORTED = 0x00000005;

}
