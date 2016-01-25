<?php

/**
 * @file
 * Administraton callback functions.
 */

/**
 * Form constructor for the admin settings form.
 *
 * @see commerce_empty_cart_paths_menu()
 */
function commerce_empty_cart_paths_admin_settings_form() {
  $form['commerce_empty_cart_paths'] = array(
    '#title' => t('Paths to keep the cart intact'),
    '#description' => t('One or more paths on which to NOT clear the cart. Enter each path on a new line.'),
    '#type' => 'textarea',
    '#required' => TRUE,
    '#default_value' => variable_get('commerce_empty_cart_paths'),
  );

  return system_settings_form($form);
}
