<?php

/**
 * Log4Php CLI locale renderer
 *
 * Uses log4php_t function so that the message is being translated
 *
 * WARNING: the messages logged by this logger are not sanitized and are meant to be viewed in
 * a terminal. Displaying these messages in a browser represents a potential security threat.
 */
class LoggerRendererDrupalPlainLocaleObject extends LoggerRendererDrupalLogObject {

  /**
   * @param LoggerRendererDrupalLogObject $drupalLogObject
   * @return string
   */
  public function render($drupalLogObject) {
    return "{$drupalLogObject->user_name} {$drupalLogObject->request_uri} {$drupalLogObject->ip} " .
      log4php_t($drupalLogObject->message, is_array($drupalLogObject->variables) ? $drupalLogObject->variables : array());
  }
}
