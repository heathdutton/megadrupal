<?php

/**
 * @file
 * Theme setting callbacks for the seventeen theme.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 */
function seventeen_form_system_theme_settings_alter(&$form, &$form_state) {
  $form['seventeen'] = array(
    '#type' => 'fieldset',
    '#title' => t('Extra'),
  );
  $form['seventeen']['style_checkboxes'] = array(
    '#type' => 'checkbox',
    '#title' => t('Style checkboxes'),
    '#default_value' => theme_get_setting('style_checkboxes'),
  );
  $form['seventeen']['max_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum width'),
    '#size' => 30,
    '#default_value' => theme_get_setting('max_width'),
    '#element_validate' => array('element_validate_integer_positive'),
    '#description' => t('The maximum width of the content area in pixels.'),
  );
}
