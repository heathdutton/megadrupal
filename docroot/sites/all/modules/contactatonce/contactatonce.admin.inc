<?php

/**
 * Custom callback to create the simple admin form
 * for the module found at admin/config/system/contactatonce.
 * @see contactatonce_menu()
 */
function contactatonce_admin_form($form, &$form_state) {

  $form = array();
  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('Contact-at-Once'),
  );
  $form['config'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings'),
  );
  $form['config']['contactatonce_merchant_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Merchant ID'),
    '#default_value' => variable_get('contactatonce_merchant_id', ''),
    '#size' => 8,
  );
  $form['config']['contactatonce_provider_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Provider ID'),
    '#default_value' => variable_get('contactatonce_provider_id', ''),
    '#size' => 8,
  );

  return system_settings_form($form);
}
