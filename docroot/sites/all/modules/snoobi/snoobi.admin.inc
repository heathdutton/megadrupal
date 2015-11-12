<?php

/**
 * @file
 * Administration settings for Snoobi integration
 */

/**
 * Administration settings form.
 *
 * @return
 *   The completed form definition.
 */
function snoobi_admin_settings($form, &$form_state) {
  $form = array();

  $form['snoobi_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#default_value' => variable_get('snoobi_username', 'corporatename_fi'),
    '#description' => t('Your username for <a href="http://www.snoobi.com/">snoobi.com</a>. Example: corporatename_fi'),
  );
  
  $form['snoobi_help'] = array(
    '#markup' => t('Remember to grant <strong>view snoobi</strong> permission to all roles that should be tracked at <a href="!permissions">Permissions page</a>', array('!permissions' => url('admin/people/permissions', array('fragment' => 'module-snoobi'))))
  );
  
  return system_settings_form($form);
}
