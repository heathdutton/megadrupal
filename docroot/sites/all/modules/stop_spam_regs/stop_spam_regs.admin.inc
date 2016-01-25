<?php

/**
 * @file
 * Provides settings form(s) for Stop Spam Registrations module.
 */

/**
 * Form with list of spam email domains.
 */
function stop_spam_regs_settings_form($form) {

  stop_spam_regs_include('blacklist');
  $spam_list = variable_get('stop_spam_regs_spam_list', _stop_spam_regs_blacklist());
  $domains_count = count($spam_list);

  $form['spam_list'] = array(
    '#type' => 'textarea',
    '#title' => t('List of spam domains'),
    '#description' => t('Enter here list of domains for which registration should be blocked. One domain per line.'),
    '#default_value' => $spam_list ? implode("\r\n", $spam_list) : '',
    '#rows' => $domains_count > 25 ? 25 : $domains_count,
  );

  $form['actions'] = array(
    '#type' => 'action'
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Submit callback for the form with spam list.
 */
function stop_spam_regs_settings_form_submit($form, &$form_state) {

  // Make array of spam domains (instead of string).
  $spam_list = $form_state['values']['spam_list'];
  $domains = explode("\n", $spam_list);

  // Remove whitespaces and empty strings.
  $processed_spam_list = array();
  foreach ($domains as $domain) {
    $domain_trimmed = trim($domain);
    if (!empty($domain_trimmed)) {
      $processed_spam_list[] = $domain_trimmed;
    }
  }

  // Save spam list as an array.
  variable_set('stop_spam_regs_spam_list', $processed_spam_list);
}
