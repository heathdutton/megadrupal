<?php

/**
 * @file
 * Module's configuration form.
 */

/**
 * Page callback for admin/config/user-interface/jquery-custom-scrollbar.
 */
function jquery_custom_scrollbar_settings() {

  $selectors_description = 'The scrollable element must have fixed dimensions or max-width, or max-height properties set.<br/>
                           Given that fact, it won\'t work on a dynamic body element.';

  $form['jquery_custom_scrollbar_pages'] = array(
    '#title' => t('Pages (one per line)'),
    '#type' => 'textarea',
    '#description' => t('Url are relatives, use * for every page'),
    '#default_value' => variable_get('jquery_custom_scrollbar_pages', ''),
    '#attributes' => array(
      'placeholder' => JQUERY_CUSTOM_SCROLLBAR_CONFIG_PATH,
    ),
  );
  $form['jquery_custom_scrollbar_selectors'] = array(
    '#title' => t('Selectors for scrollable elements (commas separated)'),
    '#type' => 'textfield',
    '#description' => t($selectors_description),
    '#default_value' => variable_get('jquery_custom_scrollbar_selectors', ''),
    '#attributes' => array(
      'placeholder' => '.jcs_demo',
    ),
  );
  $form['jquery_custom_scrollbar_skin'] = array(
    '#title' => t('Scrollbar skin'),
    '#type' => 'radios',
    '#options' => array(
      0 => t('none'),
      'default-skin' => t('Default skin'),
      'gray-skin' => t('Gray skin'),
      'modern-skin' => t('Modern skin'),
    ),
    '#default_value' => variable_get('jquery_custom_scrollbar_skin', 'default-skin'),
  );
  $form['jquery_custom_scrollbar_demo'] = array(
    '#title' => t('jQuery Custom Scrollbar demo'),
    '#markup' => theme('jquery_custom_scrollbar_drupal_integration_example'),
  );

  return system_settings_form($form);
}
