<?php
/**
 * @file
 * CiviCRM Contact Match admin page
 */

/**
 * Admin settings form.
 */
function civicrm_contact_match_admin_form($form, &$form_state) {

  $form['civicrm_contact_match_redir'] = array(
    '#type' => 'textfield',
    '#title' => t('Destination after update (leave blank if no redirection required)'),
    '#default_value' => variable_get('civicrm_contact_match_redir', ''),
  );

  $form['civicrm_contact_match_civi_location'] = array(
    '#type' => 'textfield',
    '#title' => t('CiviCRM location/directory'),
    '#default_value' => variable_get('civicrm_contact_match_civi_location', '/civicrm'),
  );

  return system_settings_form($form);
}
