<?php

/**
 * @file
 * DropShark administrative pages.
 */

/**
 * Administrative configuration page.
 */
function dropshark_admin_config() {
  $form['dropshark_site_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Site ID'),
    '#description' => t('Find your site identifier on your DropShark dashboard.'),
    '#default_value' => variable_get('dropshark_site_id'),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}
