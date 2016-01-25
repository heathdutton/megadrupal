<?php
/**
 * @file
 * Admin forms for mailcheck.
 */


/**
 * Builds configuration form.
 */
function mailcheck_settings_form($form_state) {
  $form = array();

  $conf = mailcheck_get_config();

  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General'),
  );

  $form['general']['mailcheck_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#description' => t('Global toggle for mailcheck.'),
    '#default_value' => $conf['mailcheck_enabled'],
  );

  $form['general']['mailcheck_message'] = array(
    '#type' => 'textfield',
    '#title' => t('Message'),
    '#description' => 'Message to user when a suggestion is available. Use !suggestion as a placeholder for the suggestion.',
    '#default_value' => $conf['mailcheck_message'],
  );

  $form['general']['mailcheck_domains'] = array(
    '#type' => 'textarea',
    '#title' => t('Domains'),
    '#description' => t('Domains, separated with whitespace or commas.'),
    '#default_value' => implode("\r\n", $conf['mailcheck_domains']),
    '#rows' => 10,
  );

  $form['general']['mailcheck_domains_threshold'] = array(
    '#type' => 'textfield',
    '#title' => t('Domains Distance Threshold'),
    '#description' => t('The maximum string distance which will return a domain suggestion.'),
    '#default_value' => $conf['mailcheck_domains_threshold'],
    '#size' => 2,
  );

  $form['general']['mailcheck_tlds'] = array(
    '#type' => 'textarea',
    '#title' => t('Top-level Domains'),
    '#description' => t('Top-level domains, separated with whitespace or commas.'),
    '#default_value' => implode("\r\n", $conf['mailcheck_tlds']),
    '#rows' => 10,
  );

  $form['general']['mailcheck_tlds_threshold'] = array(
    '#type' => 'textfield',
    '#title' => t('Top-level Domains Distance Threshold'),
    '#description' => t('The maximum string distance which will return a top-level domain suggestion.'),
    '#default_value' => $conf['mailcheck_tlds_threshold'],
    '#size' => 2,
  );

  if (module_exists('webform')) {
    $form['general']['mailcheck_webform'] = array(
      '#type' => 'checkbox',
      '#title' => t('Mailcheck on webform'),
      '#default_value' => $conf['mailcheck_webform'],
    );
  }

  $form['general']['mailcheck_show_type'] = array(
    '#type' => 'select',
    '#title' => t('Show message in'),
    '#options' => array(
      0 => 'Prefix',
      1 => 'Description',
      2 => 'Suffix',
    ),
    '#default_value' => $conf['mailcheck_show_type'],
  );

  $form['general']['gestures'] = array(
    '#type' => 'fieldset',
    '#title' => t('Gestures'),
  );

  $form['general']['gestures']['mailcheck_lock'] = array(
    '#type' => 'checkbox',
    '#title' => t('Submit Lock'),
    '#description' => t('Locks the submit button for two seconds, on typo.'),
    '#default_value' => $conf['mailcheck_lock'],
  );

  $form['general']['gestures']['mailcheck_shake'] = array(
    '#type' => 'checkbox',
    '#title' => t('Shake'),
    '#description' => t('Shake the message, on typo.'),
    '#default_value' => $conf['mailcheck_shake'],
  );

  $form['regform'] = array(
    '#type' => 'fieldset',
    '#title' => t('Register form'),
  );

  $form['regform']['mailcheck_register_show'] = array(
    '#type' => 'checkbox',
    '#title' => t('Mailcheck on register form'),
    '#default_value' => $conf['mailcheck_register_show'],
  );

  $form['regform']['mailcheck_register_lock'] = array(
    '#type' => 'checkbox',
    '#title' => t('Submit Lock'),
    '#description' => t('If you only want lock on the register form, remember to uncheck the one in gestures.'),
    '#default_value' => $conf['mailcheck_register_lock'],
  );

  return system_settings_form($form);
}

/**
 * Validate configuration form.
 *
 * @param array $form
 *   Array containing structure of the form
 * @param array $form_state
 *   Array containing info about the form state
 */
function mailcheck_settings_form_validate($form, &$form_state) {
  // Validate domains is non-empty.
  if (count(_mailcheck_split($form_state['values']['mailcheck_domains'])) == 1) {
    form_set_error('mailcheck_domains', t('You must include at least one domain.'));
  }
  // Validate top-level domains is non-empty.
  if (count(_mailcheck_split($form_state['values']['mailcheck_tlds'])) == 1) {
    form_set_error('mailcheck_tlds', t('You must include at least one top-level domain.'));
  }
}
