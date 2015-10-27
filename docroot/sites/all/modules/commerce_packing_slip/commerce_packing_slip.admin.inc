<?php
/**
 * @file
 * Administrative page callbacks for Commerce Packing Slip.
 */

/**
 * Configuration form for Commerce Packing Slip.
 */
function commerce_packing_slip_settings() {
  $form = array();

  $form['packing_slip'] = array(
    '#title' => t('Packing slip settings'),
    '#type' => 'fieldset',
  );
  $form['packing_slip']['commerce_packing_slip_logo_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Logo path'),
    '#description' => t('The full Drupal path to the logo for the packing slip.'),
    '#default_value' => variable_get('commerce_packing_slip_logo_path'),
  );
  $form['packing_slip']['commerce_packing_slip_menu_location'] = array(
    '#type' => 'select',
    '#title' => t('Menu item location'),
    '#description' => t('Change the location of the packing slip page. Choosing "primary tab" will also make the packing slip appear in the context menu.'),
    '#options' => array(
      'primary' => t('Primary tab'),
      'secondary' => t('Secondary tab'),
    ),
    '#default_value' => variable_get('commerce_packing_slip_menu_location', 'primary'),
  );

  // We need to build the settings form here so we can append our submit handler.
  $form = system_settings_form($form);
  // Add a second submit handler to rebuild the menu after the variables have
  // been set by system_settings_form_submit().
  $form['#submit'][] = 'commerce_packing_slip_settings_submit';

  return $form;
}

/**
 * Submit handler for commerce_packing_slip_settings().
 */
function commerce_packing_slip_settings_submit($form, &$form_state) {
  // Clear the menu cache to force changing the packing slip location.
  menu_rebuild();
  drupal_set_message(t('Menu cache cleared.'), 'status');
}
