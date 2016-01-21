<?php
/**
 * @file
 * 
 * This class overrides the D7 core DefaultMailSystem class to allow control 
 * over whether or not filters are applied to the message body before being 
 * emailed.  This is necessary to allow embedded XML to be sent from a webform 
 * submission. 
 */

class WebformEmailRawMailSystem extends DefaultMailSystem {
  /**
   * Implements MailSystemInterface::format
   */
  public function format(array $message) {
    // Extract module settings from message, if any
    $settings = (!empty($message['params']['webform_email_raw'])) ? $message['params']['webform_email_raw'] : array();

    // If we need to send this eamil in its raw form, do it
    if (!empty($settings['webform_email_raw_enable'])) {
      // Join the body array into one string.
      $message['body'] = implode("\n", $message['body']);

      // Check to see if we need to strip any tags
      /*
      if (!empty($settings['webform_email_raw_allowed_tags'])) {
        $message['body'] = filter_xss($message['body'], $settings['webform_email_raw_allowed_tags']);
      }
      */

      // Check for alternate MIME type
      if (!empty($settings['webform_email_raw_mime'])) {
        $message['headers']['Content-Type'] = $settings['webform_email_raw_mime'];
      }
    }

    // ... otherwise use the default mail handler to process the message
    else {
      $message = parent::format($message);
    }

    return $message;
  }
};
