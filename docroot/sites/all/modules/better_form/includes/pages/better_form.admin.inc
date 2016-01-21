<?php

/**
 * @file
 * Provides the Better Form Module administrative interface.
 */

/**
 * Form builder for the "better-form" configuration page.
 */
function better_form_admin_settings_page($form, &$form_state) {
  $radios_options = array(
    FALSE => t('No'),
    TRUE => t('Yes'),
  );
  $is_enable_on_admin_pages = (int) variable_get('better_form_is_enable_on_admin_pages', FALSE);
  $form['better_form_is_enable_on_admin_pages'] = array(
    '#type' => 'radios',
    '#title' => t('Should the module work on admin pages?'),
    '#description' => t('Check the option "Yes" and then the module will work on admin pages.'),
    '#options' => $radios_options,
    '#default_value' => $is_enable_on_admin_pages,
  );
  $placeholder_is_enable = (int) variable_get('better_form_placeholder_is_enable', FALSE);
  $form['better_form_placeholder_is_enable'] = array(
    '#type' => 'radios',
    '#title' => t('Should the module add a placeholder attribute?'),
    '#description' => t('Check the option "Yes" and then the module will add a placeholder attribute.'),
    '#options' => $radios_options,
    '#default_value' => $placeholder_is_enable,
  );
  return system_settings_form($form);
}
