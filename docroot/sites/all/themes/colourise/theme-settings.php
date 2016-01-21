<?php

/**
 * @file
 * Theme setting callbacks for the colourise theme.
 */

  //function phptemplate_settings($saved_settings) {
function colourise_form_system_theme_settings_alter(&$form, $form_state) {
  //$settings = theme_get_settings('colourise');

/**
 * The default values for the theme variables. Make sure $defaults exactly
 * matches the $defaults in the template.php file.
 */
  $defaults = array(
    'colourise_page_class'     => 'wide',
    'colourise_iepngfix'       => 0,
    'colourise_custom'         => 0,
    'colourise_breadcrumb'     => 0,
    'colourise_totop'          => 0,
  );

  // Merge the saved variables and their default values
  //$settings = array_merge($defaults, $settings);

  // Create theme settings form widgets using Forms API
  // colourise Fieldset
  $form['colourise_container'] = array(
    '#type' => 'fieldset',
    '#title' => t('Colourise Theme Settings'),
    '#description' => t('Use these settings to change what and how information is displayed in <strong>Colourise</strong> theme.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['colourise_container']['colourise_page_class'] = array(
    '#type' => 'radios',
    '#title' => t('Page Width'),
    '#description'   => t('Select the page width you need. <strong>Be careful</strong>, the Narrow and Medium Width may not suite 2 sidebars.'),
    '#default_value' => theme_get_setting('colourise_page_class'),
    '#options' => array(
    'narrow' => t('Narrow (Fixed width: 780px)'),
    'medium' => t('Medium (Fixed width: 840px)'),
    'wide' => t('Wide (Fixed width: 960px)'),
    'super-wide' => t('Super Wide (Fixed width: 1020px)'),
    'extreme-wide' => t('Extreme Wide (Fixed width: 1140px)'),
    'fluid' => t('Fluid (min-width: 780px)'),
    ),
  );

  $form['colourise_container']['colourise_features'] = array(
    '#type' => 'fieldset',
    '#title' => t('Other Features'),
    '#description'   => t('Check / Uncheck each themes features you want to activate or deactivate for your site.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['colourise_container']['colourise_features']['colourise_iepngfix'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use <strong>IE Transparent PNG Fix</strong>'),
    '#default_value' => theme_get_setting('colourise_iepngfix'),
  );

  $form['colourise_container']['colourise_features']['colourise_custom'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add <strong>Customized Stylesheet (custom.css)</strong>'),
    '#default_value' => theme_get_setting('colourise_custom'),
  );

  $form['colourise_container']['colourise_features']['colourise_breadcrumb'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show <strong>Breadcrumbs</strong>'),
    '#default_value' => theme_get_setting('colourise_breadcrumb'),
  );

  $form['colourise_container']['colourise_features']['colourise_totop'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show <strong>Back to Top link</strong> (the link will appear at footer)'),
    '#default_value' => theme_get_setting('colourise_totop'),
  );

  return $form;
}