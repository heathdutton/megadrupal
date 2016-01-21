<?php

/**
 * @file
 * API documentation for Webform Capture+.
 */

/**
 * Allows modules to alter settings object.
 *
 * @param \WebformCapturePlusSettings $settings
 */
function hook_webform_captureplus_settings_alter(WebformCapturePlusSettings $settings) {
  // Create message catalogue.
  $message_catalogue = new WebformCapturePlusMessageCatalogue('en', array('messages' => array('NORESULTS' => 'Looks like the address is not available. Please enter your address manually.')));
  $settings->setMessages($message_catalogue);
}
