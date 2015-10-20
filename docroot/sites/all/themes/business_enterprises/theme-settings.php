<?php

/**
 * @file
 * Theme setting callbacks for the Business theme.
 */

/**
 * Implements hook_form_system_theme_settings_alter().
 */
function business_enterprises_form_system_theme_settings_alter(&$form, &$form_state) {

  $form['business_enterprises_color_settings'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Color Scheme'),
    '#weight' => -2,
    '#description'   => t("Select a predesigned color scheme for the site."),
  );

  $form['business_enterprises_color_settings']['color_scheme'] = array(
    '#type'          => 'select',
    '#title'         => t('Color Scheme'),
    '#default_value' => theme_get_setting('color_scheme', 'business_enterprises'),
    '#description'   => t('Select a predesigned color scheme.'),
    '#options'       => array(
      'white' => t('Dark'),
      'dark' => t('White'),
    ),
  );

  $form['business_enterprises_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('business_enterprises Theme Settings'),
    '#weight' => -1,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['business_enterprises_settings']['breadcrumbs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show breadcrumbs in a page'),
    '#default_value' => theme_get_setting('breadcrumbs', 'business_enterprises'),
    '#description'   => t("Check this option to show breadcrumbs in page. Uncheck to hide."),
  );
  $form['business_enterprises_settings']['footer'] = array(
    '#type' => 'fieldset',
    '#title' => t('Footer'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['business_enterprises_settings']['footer']['footer_copyright'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show copyright text in footer'),
    '#default_value' => theme_get_setting('footer_copyright', 'business_enterprises'),
    '#description'   => t("Check this option to show copyright text in footer. Uncheck to hide."),
  );
  $form['business_enterprises_settings']['footer']['footer_credits'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show theme credits in footer'),
    '#default_value' => theme_get_setting('footer_credits', 'business_enterprises'),
    '#description'   => t("Check this option to show site credits in footer. Uncheck to hide."),
  );
}
