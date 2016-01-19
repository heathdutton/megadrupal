<?php

/**
 * Log4Php Drupal Plain Renderer
 *
 * WARNING: the messages logged by this logger are not sanitized and are meant to be viewed in
 * a terminal. Displaying these messages in a browser represents a potential security threat.
 */
class LoggerRendererDrupalPlainObject extends LoggerRendererDrupalLogObject {

  /**
   * @param LoggerRendererDrupalLogObject $drupalLogObject
   * @return string
   */
  public function render($drupalLogObject) {
    return "{$drupalLogObject->user_name} {$drupalLogObject->request_uri} {$drupalLogObject->ip} " .
      strtr($drupalLogObject->message, is_array($drupalLogObject->variables) ? $drupalLogObject->variables : array());
  }
}
