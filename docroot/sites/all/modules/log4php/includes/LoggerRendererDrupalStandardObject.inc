<?php

/**
 * Log4Php Drupal Standard Renderer - messages logged by this renderer can be safely viewed in a browser
 *
 * Will pass log messages through t() function which in turn does check plain() and format_string().
 */
class LoggerRendererDrupalStandardObject extends LoggerRendererDrupalLogObject {

  /**
   * @param LoggerRendererDrupalLogObject $drupalLogObject
   * @return string
   */
  public function render($drupalLogObject) {
    return "{$drupalLogObject->user_name} {$drupalLogObject->request_uri} {$drupalLogObject->ip} " .
      t($drupalLogObject->message, is_array($drupalLogObject->variables) ? $drupalLogObject->variables : array());
  }
}
