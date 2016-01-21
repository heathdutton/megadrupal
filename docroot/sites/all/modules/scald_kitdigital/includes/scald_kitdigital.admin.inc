<?php

/**
 * @file
 * Defines Backoffice form
 */

/**
 * Defines kitdigital form settings.
 */
function scald_kitdigital_admin_form() {
  $form = array();
  $form['scald_kitdigital_appkey'] = array(
    '#title' => t('Kitdigital application key'),
    '#type' => 'textfield',
    '#default_value' => variable_get('scald_kitdigital_appkey', ''),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}
