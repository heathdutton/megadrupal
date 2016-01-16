<?php
/**
 * Implements hook_form_system_theme_settings_alter().
 */
function wet4_SUBTHEME_form_system_theme_settings_alter(&$form, $form_state) {
  $form['wet4'] = array(
    '#type' => 'fieldset',
    '#title' => t('WET4 Theme Settings'),
    '#collapsible' => FALSE,
  );

  $form['wet4']['base'] = array(
    '#type'          => 'select',
    '#title'         => t('WET4 Base Theme'),
    '#options'       => array(
      0 => t('Government of Canada Web Usability'),
      1 => t('Government of Canada Intranet'),
    ),
    '#default_value' => theme_get_setting('base'),
  );
}