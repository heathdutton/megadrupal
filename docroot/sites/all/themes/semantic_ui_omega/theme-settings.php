<?php

/**
 * @file
 * Theme settings file for the semantic_ui_omega theme.
 */

require_once dirname(__FILE__) . '/template.php';

/**
 * Implements hook_form_FORM_alter().
 */
function semantic_ui_omega_form_system_theme_settings_alter(&$form, $form_state) {
  $form['semantic_ui_omega_color'] = array(
    '#type' => 'select',
    '#title' => t('Semantic UI color'),
    '#options'=> array('green' => t('Green'),'teal'=>t('Teal'), 'red'=> t('Red'),'black'=>t('Black')),
    '#default_value' =>  theme_get_setting('semantic_ui_omega_color'),
  );
}
