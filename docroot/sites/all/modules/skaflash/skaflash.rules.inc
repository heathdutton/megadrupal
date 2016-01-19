<?php

/**
 * @file
 * Rules integration hooks/functions for the SkaFlash module.
 */


/**
 * Implements hook_rules_data_info().
 */
function skaflash_rules_data_info() {
  return array(
    'skaflash_push' => array(
      'label' => t('skaflash push data'),
      'wrap' => TRUE,
      'property info' => array(
        'email' => array(
          'type' => 'text',
          'label' => t('The email address submitted by the fan.'),
        ),
        'phone' => array(
          'type' => 'text',
          'label' => t('The phone number used to submit the email address.'),
        ),
        'timestamp' => array(
          'type' => 'date',
          'label' => t('The date and time of the submission'),
        ),
      ),
    ),
  );
}


/**
 * Implements hook_rules_event_info().
 */
function skaflash_rules_event_info() {
  $defaults = array(
    'group' => t('skaflash'),
    'module' => 'skaflash',
  );
  return array(
    'skaflash_pushed' => $defaults + array(
      'label' => t('After receiving a push notification from SkaFlash'),
      'variables' => array(
        'skaflash_push' => array(
          'type' => 'skaflash_push',
          'label' => t('SkaFlash push notification'),
          'description' => t('The data returned by the SkaFlash API, including e-mail address and phone number.'),
        ),
      ),
    ),
  );
}
