<?php

/**
 * @file
 * Includes only functions used in administration pages.
 */

/**
 * Configuration page.
 */
function virtual_keyboard_admin_settings() {
  $form = array();

  $rows = array(
    array('input, textarea', t('Use all single line text fields and textareas on site.')),
    array('.your-form-class *', t('Use all text fields in given form class.')),
    array('#your-form-id *', t('Use all text fields in given form id.')),
    array('#your-form-id *:not(textarea)', t('Use all single line text fields but  not textareas in given form id.')),
    array('#your-form-id input:not(input[type=password])', t('Use all single line text fields but not password text fields in given form id.')),
  );

  $table = array(
    'header' => array(t('CSS selector'), t('Description')),
    'rows' => $rows,
  );

  $form['selectors'] = array(
    '#type' => 'fieldset',
    '#title' => t('Visibility settings'),
  );

  $form['selectors']['virtual_keyboard_method'] = array(
    '#type' => 'select',
    '#title' => t('Method of displaying the keyboard'),
    '#options' => array(
      'icon' => t('Open keyboard by clicking the icon'),
      'focus' => t('Open keyboard when focusing on text field'),
      'icon_and_focus' => t('Both: by clicking the icon and focus'),
    ),
    '#default_value' => variable_get('virtual_keyboard_method', 'icon'),
  );

  $form['selectors']['virtual_keyboard_include'] = array(
    '#type' => 'textarea',
    '#title' => t('Include text fields matching the pattern'),
    '#description' => t('CSS selectors (one per line).'),
    '#default_value' => variable_get('virtual_keyboard_include', ''),
  );

  $form['selectors']['virtual_keyboard_exclude'] = array(
    '#type' => 'textarea',
    '#title' => t('Exclude text fields matching the pattern'),
    '#description' => t('CSS selectors (one per line).'),
    '#default_value' => variable_get('virtual_keyboard_exclude', ''),
  );

  $form['selectors']['examples'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Examples'),
  );

  $form['selectors']['examples']['content'] = array(
    '#markup' => theme('table', $table),
  );

  return system_settings_form($form);
}
