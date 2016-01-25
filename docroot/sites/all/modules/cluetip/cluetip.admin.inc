<?php

/**
 * @file
 * Functionality for cluetip administration.
 */

/**
 * Settings form as implemented by hook_menu.
 */
function cluetip_admin_settings($form, &$form_state) {
  $form['cluetip_every_page'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use On Every Page'),
    '#description' => t('This will add cluetip library for every page. Turn this
      off if you are using cluetip library using drupal_add_library() function.
      If cluetip is loaded you can
      <a class="cluetip-title" title="Cluetip - title |test description">
      mouse over this.</a>'),
    '#default_value' => variable_get('cluetip_every_page', 1),
  );
  
  $form['cluetip_theme_class'] = array(
    '#type' => 'select',
    '#title' => t('Cluetip theme class'),
    '#description' => t('Using a predefined cluetip class defines the looks
      of cluetip.'),
      '#options' => array(
          'default' => t('Default'),
          'jtip' => t('Jtip'),
          'rounded' => t('Rounded')),
    '#default_value' => variable_get('cluetip_theme_class', 'rounded'),
  );
   return system_settings_form($form);
}