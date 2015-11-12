<?php
/**
 * @file
 * Theme setting callbacks for Themage.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 */
function themage_form_system_theme_settings_alter(&$form, $form_state) {

  // Give users options regarding breadcrumbs.
  $form['breadcrumb'] = array(
    '#type' => 'fieldset',
    '#title' => t('Breadcrumb Settings'),
  );

  $form['breadcrumb']['breadcrumb_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display the breadcrumb'),
    '#default_value' => theme_get_setting('breadcrumb_display'),
  );

  $form['breadcrumb']['breadcrumb_title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display the title in the breadcrumb'),
    '#default_value' => theme_get_setting('breadcrumb_title'),
    '#states' => array(
      'disabled' => array(
        ':input[name="breadcrumb_display"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form['breadcrumb']['breadcrumb_separator'] = array(
    '#type' => 'textfield',
    '#title' => t('Breadcrumb separator'),
    '#size' => 4,
    '#default_value' => theme_get_setting('breadcrumb_separator'),
    '#states' => array(
      'disabled' => array(
        ':input[name="breadcrumb_display"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form['breadcrumb']['breadcrumb_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Text before the breadcrumb'),
    '#default_value' => theme_get_setting('breadcrumb_prefix'),
    '#states' => array(
      'disabled' => array(
        ':input[name="breadcrumb_display"]' => array('checked' => FALSE),
      ),
    ),
  );
}
