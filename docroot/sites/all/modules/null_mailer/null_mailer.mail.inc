<?php
/**
 * @file
 * The code processing mail in the smtp module.
 */

/**
 * Disable drupal mail system.
 */
class NullMailSystem implements MailSystemInterface {
  
  public function format(array $message) {
    return $message;
  }
  public function mail(array $message) {
    return TRUE;
  }
}
