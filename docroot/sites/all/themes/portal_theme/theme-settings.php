<?php
/**
 * @file
 * Theme setting callbacks for the Portal theme.
 */

/**
 * Implements theme_settings().
 */
function portal_theme_form_system_theme_settings_alter(&$form, &$form_state) {

  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files']['form'] = drupal_get_path('theme', 'portal_theme') . '/theme-settings.php';

  $form['portal_theme_settings_title'] = array(
    '#markup' => t('Portal Theme Settings'),
  );

  $form['portal_theme_theme_settings'] = array(
    '#type' => 'vertical_tabs',
    '#title' => t('Portal Theme Settings'),
  );

  $form['portal_theme_logo_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Logo'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'portal_theme_theme_settings',
  );

  $form['portal_theme_logo_settings']['portal_theme_enable_logo'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable logo'),
    '#default_value' => (theme_get_setting('portal_theme_enable_logo')) ? theme_get_setting('portal_theme_enable_logo') : 0,
  );

  $form['portal_theme_theme_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Other'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'portal_theme_theme_settings',
  );

  $form['portal_theme_theme_options']['portal_theme_attribution'] = array(
    '#type' => 'checkbox',
    '#title' => t('Attribution (Logo and Text options)'),
    '#default_value' => theme_get_setting('portal_theme_attribution'),
  );

  $form['portal_theme_theme_options']['portal_theme_is_civicrm'] = array(
    '#type' => 'checkbox',
    '#title' => t('Using CiviCRM'),
    '#default_value' => theme_get_setting('portal_theme_is_civicrm'),
  );

  $form['portal_theme_theme_options']['portal_theme_client_name'] = array(
    '#type'          => 'textfield',
    '#title'         => t("Client's Name"),
    '#default_value' => theme_get_setting('portal_theme_client_name'),
    '#description'   => t("Set the client name used in the copyright bar."),
  );

  $form['portal_theme_theme_options']['portal_theme_client_url'] = array(
    '#type'          => 'textfield',
    '#title'         => t("Client's Website URL"),
    '#default_value' => theme_get_setting('portal_theme_client_url'),
    '#description'   => t("Set the client URL used in the copyright bar."),
  );

  $form['portal_theme_theme_options']['portal_theme_designer_name'] = array(
    '#type'          => 'textfield',
    '#title'         => t("Your Name"),
    '#default_value' => theme_get_setting('portal_theme_designer_name'),
    '#description'   => t("Set your name used in the copyright bar."),
  );

  $form['portal_theme_theme_options']['portal_theme_designer_url'] = array(
    '#type'          => 'textfield',
    '#title'         => t("Your Website URL"),
    '#default_value' => theme_get_setting('portal_theme_designer_url'),
    '#description'   => t("Set your URL used in the copyright bar."),
  );

  $form['portal_theme_theme_options']['portal_theme_show_front_page_title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show page title on front page'),
    '#default_value' => theme_get_setting('portal_theme_show_front_page_title'),
  );

  // Return the additional form widgets.
  return $form;
}
