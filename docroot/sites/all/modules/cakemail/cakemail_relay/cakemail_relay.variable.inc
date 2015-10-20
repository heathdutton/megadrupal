<?php
/**
 * @file
 *   Variable hooks implementations for the Cakemail Relay module.
 */

/**
 * Implements of hook_variable_info().
 */
function cakemail_relay_variable_info($options) {
  return array(
    'cakemail_relay_user_key' => array(
      'type' => 'string',
      'title' => t('CakeMail Relay User Key', array(), $options),
      'default' => '',
      'localize' => FALSE,
    ),
    'cakemail_relay_client_id' => array(
      'type' => 'number',
      'title' => t('CakeMail Relay Client ID', array(), $options),
      'default' => NULL,
      'localize' => FALSE,
    ),
    'cakemail_relay_template_id' => array(
      'type' => 'number',
      'title' => t('CakeMail Relay Template ID', array(), $options),
      'description' => t('The ID of the template to use when sending mails through CakeMail.', array(), $options),
      'default' => NULL,
      'localize' => TRUE,
    ),
  );
}