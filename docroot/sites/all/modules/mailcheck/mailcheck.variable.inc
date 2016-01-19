<?php
/**
 * @file
 * Declare basic info to Variable API module.
 */


/**
 * Implements hook_variable_info().
 */
function mailcheck_variable_info($options) {
  $variables = array();

  $variables['mailcheck_enabled'] = array(
    'title' => t('Enabled', array(), $options),
    'type' => 'boolean',
    'description' => t('Global toggle for mailcheck.', array(), $options),
    'default' => variable_get('mailcheck_enabled', TRUE),
  );

  $variables['mailcheck_domains'] = array(
    'title' => t('Domains', array(), $options),
    'type' => 'string',
    'description' => t('Domains, separated with whitespace or commas.', array(), $options),
    'default' => variable_get('mailcheck_domains', 'yahoo.com, google.com, hotmail.com, gmail.com, me.com, aol.com, mac.com, live.com, comcast.net, googlemail.com, msn.com, hotmail.co.uk, yahoo.co.uk, facebook.com, verizon.net, sbcglobal.net, att.net, gmx.com, mail.com'),
  );

  $variables['mailcheck_domains_threshold'] = array(
    'title' => t('Domains Distance Threshold', array(), $options),
    'type' => 'number',
    'description' => t('The maximum string distance which will return a domain suggestion.', array(), $options),
    'default' => variable_get('mailcheck_domains_threshold', 2),
  );

  $variables['mailcheck_tlds'] = array(
    'title' => t('Top-level Domains', array(), $options),
    'type' => 'string',
    'description' => t('Top-level domains, separated with whitespace or commas.', array(), $options),
    'default' => variable_get('mailcheck_tlds', "co.jp, co.uk, com, net, org, info, edu, gov, mil, ca"),
  );

  $variables['mailcheck_tlds_threshold'] = array(
    'title' => t('Top-level Domains Distance Threshold', array(), $options),
    'type' => 'number',
    'description' => t('The maximum string distance which will return a top-level domain suggestion.', array(), $options),
    'default' => variable_get('mailcheck_domains_threshold', 1),
  );

  $variables['mailcheck_message'] = array(
    'title' => t('Message', array(), $options),
    'type' => 'string',
    'description' => t('Message to user when a suggestion is available. Use !suggestion as a placeholder for the suggestion.', array(), $options),
    'default' => variable_get('mailcheck_message', 'Did you mean !suggestion?'),
    'localize' => TRUE,
  );

  $variables['mailcheck_webform'] = array(
    'title' => t('Mailcheck on webform', array(), $options),
    'type' => 'boolean',
    'default' => variable_get('mailcheck_webform', FALSE),
  );

  $variables['mailcheck_show_type'] = array(
    'title' => t('Show message in', array(), $options),
    'type' => 'select',
    'options' => array(
      0 => 'Prefix',
      1 => 'Description',
      2 => 'Suffix',
    ),
    'default' => variable_get('mailcheck_show_type', 1),
  );

  $variables['mailcheck_lock'] = array(
    'title' => t('Submit Lock', array(), $options),
    'type' => 'string',
    'description' => t('Locks the submit button for two seconds, on typo.', array(), $options),
    'default' => variable_get('mailcheck_lock', FALSE),
  );

  $variables['mailcheck_shake'] = array(
    'title' => t('Shake', array(), $options),
    'type' => 'string',
    'description' => t('Shake the message, on typo.', array(), $options),
    'default' => variable_get('mailcheck_shake', FALSE),
  );

  $variables['mailcheck_register_show'] = array(
    'title' => t('Mailcheck on register form', array(), $options),
    'type' => 'boolean',
    'default' => variable_get('mailcheck_register_show', TRUE),
  );

  $variables['mailcheck_register_lock'] = array(
    'title' => t('Submit Lock', array(), $options),
    'type' => 'boolean',
    'description' => t('If you only want lock on the register form, remember to uncheck the one in gestures.', array(), $options),
    'default' => variable_get('mailcheck_register_lock', FALSE),
  );

  return $variables;
}
