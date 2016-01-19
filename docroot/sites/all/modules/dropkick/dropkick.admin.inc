<?php

/**
 * @file
 * Dropkick administration pages.
 */

/**
 * General configuration form for Dropkick.
 */
function dropkick_admin_settings() {
  $form = array();

  $form['dropkick_jquery_selector'] = array(
    '#type' => 'textarea',
    '#title' => t('Apply DropKick to the following elements'),
    '#description' => t('A jQuery selector to find elements to apply Dropkick
      to, such as <code>.dropkick-select</code>. Use <code>select</code> to
      apply Dropkick to all <code>&lt;select&gt;</code> elements. For multiple
      selector use comma separated selector.'),
    '#default_value' => variable_get('dropkick_jquery_selector', ''),
  );

  $form['dropkick_advance_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advance Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['dropkick_advance_settings']['dropkick_mobile_device_support'] = array(
    '#type' => 'checkbox',
    '#title' => t('Mobile device support for DropKick'),
    '#description' => t('Render the Dropkick element for mobile devices.'),
    '#default_value' => variable_get('dropkick_mobile_device_support', ''),
  );

  $form['dropkick_advance_settings']['dropkick_ie8_support'] = array(
    '#type' => 'checkbox',
    '#title' => t('IE 8 support for DropKick'),
    '#description' => t("Dropkick's own IE8 polyfill."),
    '#default_value' => variable_get('dropkick_ie8_support', ''),
  );

  $form['dropkick_advance_settings']['dropkick_ui_theme'] = array(
    '#type' => 'select',
    '#title' => t('DropKick UI theme'),
    '#options' => array(
      'default' => t('Default'),
      'dropkick-classic' => t('DropKick Classic'),
    ),
    '#default_value' => variable_get('dropkick_ui_theme', 'default'),
  );

  return system_settings_form($form);
}
