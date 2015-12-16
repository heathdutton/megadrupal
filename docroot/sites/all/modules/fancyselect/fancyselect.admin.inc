<?php
/**
 * @file
 * Administration form for fancySelect.
 */

/**
 * Page callback: fancySelect admin settings form.
 */
function fancyselect_settings() {
  $form = array();

  $form['fancyselect_dom_selector'] = array(
    '#type' => 'textfield',
    '#title' => t('fancySelect DOM selector'),
    '#default_value' => variable_get('fancyselect_dom_selector', 'fancySelect'),
    '#description' => t('jQuery style DOM selector for select element.'),
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
  );
  $form['fancyselect_load_default'] = array(
    '#type' => 'checkbox',
    '#title' => t('Load default'),
    '#default_value' => variable_get('fancyselect_load_default', TRUE),
    '#description' => t('Check this box to load fancySelect plug-in by default. If unchecked, it will need to be added by drupal_add_library or #attached.'),
  );

  return system_settings_form($form);
}
