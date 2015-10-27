<?php

/**
 * @file
 * Theme settings file for the Reset theme.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 */
function reset_form_system_theme_settings_alter(&$form, &$form_state) {
  $form['theme_settings']['breadcrumb'] = array(
    '#type' => 'checkbox',
    '#title' => t('Breadcrumb'),
    '#default_value' => theme_get_setting('breadcrumb'),
  );

  $form['theme_settings']['feed_icon'] = array(
    '#type' => 'checkbox',
    '#title' => t('Feed icon'),
    '#default_value' => theme_get_setting('feed_icon'),
  );
}
