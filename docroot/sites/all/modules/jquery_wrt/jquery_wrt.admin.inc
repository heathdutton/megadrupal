<?php

/**
 * @file
 * JQuery jquery_wrt admin/configuration functionality.
 */

/**
 * jquery_wrt.admin settings form.
 */
function jquery_wrt_settings_form() {
  $module_path = drupal_get_path('module', 'jquery_wrt');
  // Selector
  $form['jquery_wrt_preserve_classes'] = array(
    '#type' => 'checkbox',
    '#title' => t('Preserve classes') . ' ' . t('(Recommended)'),
    '#description' => t(
      "Keep table components classes as far as possible for the responsive output."),
    '#default_value' => variable_get('jquery_wrt_preserve_classes', TRUE));
  
  // Dynamic
  $form['jquery_wrt_dynamic'] = array(
    '#type' => 'checkbox',
    '#title' => t('Dynamic table display switching') . ' ' . t('(Recommended)'),
    '#description' => t(
      "TRUE: Toggle table style if settings.dynamicSwitch() returns true. FALSE: Only convert to mobile (one way)"),
    '#default_value' => variable_get('jquery_wrt_dynamic', TRUE));
  
  // Show switch
  $form['jquery_wrt_showswitch'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show switch button') . ' ' . t('(Recommended)'),
    '#description' => t(
      "(Only used if dynamic!) Display a link to switch back from responsive version to original table version."),
    '#states' => array(
      'visible' => array(
        "input[name='jquery_wrt_dynamic']" => array('checked' => TRUE))),
    '#default_value' => variable_get('jquery_wrt_showswitch', TRUE));
  
  // Switch at
  $form['jquery_wrt_breakpoint'] = array(
    '#type' => 'textfield',
    '#title' => t('Dynamic breakpoint width (px)'),
    '#size' => 4,
    '#maxlength' => 4,
    '#description' => t(
      "If dynamic display is disabled and the default \"displayResponsiveCallback\"-Callback is used (see API Doc), then the mobile display variant is used if the screen width is less than this value (value in px). Default: 960"),
    '#required' => TRUE,
    '#states' => array(
      'visible' => array(
        "input[name='jquery_wrt_dynamic']" => array('checked' => TRUE))),
    '#default_value' => variable_get('jquery_wrt_breakpoint', 960));
  
  // Subselector
  $form['jquery_wrt_subselector'] = array(
    '#type' => 'textfield',
    '#title' => t('JQuery subselector'),
    '#description' => t(
      "You may optionally use this subselector to reduce the '&lt;table&gt;' DOM context which to use jQuery webks responsive table on. For example add '.my-table' to only use for tables with this class. These are added behind the jQuery('table')-Selector! Uses jQuery \".is()\" selector. Default: \":not(.sticky-header):not(.field-multiple-table):not(:has(tr.draggable))\""),
    '#required' => FALSE,
    '#default_value' => variable_get('jquery_wrt_subselector', ':not(.sticky-header):not(.field-multiple-table):not(:has(tr.draggable))'));
  
  // Selectors
  $form['jquery_wrt_header_selector'] = array(
    '#type' => 'textfield',
    '#title' => t('Header column selector'),
    '#description' => t(
      "The header columns selector. Default: 'thead td, thead th' other examples: 'tr th', ..."),
    '#required' => TRUE,
    '#default_value' => variable_get('jquery_wrt_header_selector', 'thead td, thead th'));
  
  $form['jquery_wrt_row_selector'] = array(
    '#type' => 'textfield',
    '#title' => t('Body row selector'),
    '#description' => t(
      "The body rows selector. Default: 'tbody tr'; Other examples: 'tr', ..."),
    '#required' => TRUE,
    '#default_value' => variable_get('jquery_wrt_row_selector', 'tbody tr'));
  
  // Elements
  $form['jquery_wrt_responsive_row_element'] = array(
    '#type' => 'textfield',
    '#title' => t('Responsive row element'),
    '#description' => t(
      "The responsive rows container element. Default: '&lt;dl>&lt;/dl&gt;'; Other examples: '&lt;ul&gt;&lt;/ul&gt;'. IMPORTANT: Filtered by defaults of filter_xss()."),
    '#required' => TRUE,
    '#default_value' => variable_get('jquery_wrt_responsive_row_element', '&lt;dl&gt;&lt;/dl&gt;'));
  
  // Title element
  $form['jquery_wrt_row_responsive_column_title_element'] = array(
    '#type' => 'textfield',
    '#title' => t('Responsive column title element'),
    '#description' => t(
      "The responsive column title container element. Default: '&lt;dt&gt;&lt;/dt&gt;'; Other examples: '&lt;li&gt;&lt;/li&gt;'. IMPORTANT: Filtered by defaults of filter_xss()."),
    '#required' => TRUE,
    '#default_value' => variable_get('jquery_wrt_row_responsive_column_title_element', '&lt;dt&gt;&lt;/dt&gt;'));
  
  // Value element
  $form['jquery_wrt_row_responsive_column_value_element'] = array(
    '#type' => 'textfield',
    '#title' => t('Responsive column value element'),
    '#description' => t(
      "The responsive column value container element. Default: '&lt;dd&gt;&lt;/dd&gt;'; Other examples: '&lt;li&gt;&lt;/li&gt;'. IMPORTANT: Filtered by defaults of filter_xss()."),
    '#required' => TRUE,
    '#default_value' => variable_get('jquery_wrt_row_responsive_column_value_element', '&lt;dd&gt;&lt;/dd&gt;'));
  
  // Exclude
  $form['jquery_wrt_path_match_exclude'] = array(
    '#type' => 'textarea',
    '#title' => t('Exclude the following path(s)'),
    '#description' => t(
      "Responsive tables will not be used for paths that match these patterns.") . ' ' . t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.") . ' ' . t("Default: 'admin/*'"),
    '#required' => TRUE,
    '#default_value' => variable_get('jquery_wrt_path_match_exclude', 'admin/*'));
  
  // Extra: Update on resize
  $form['jquery_wrt_update_on_resize'] = array(
    '#type' => 'checkbox',
    '#title' => t('EXTRA: Update on resize'),
    '#description' => t(
      "Re-Check display on window resize. Might be buggy in some browsers."),
    '#default_value' => variable_get('jquery_wrt_update_on_resize', FALSE));
  
  return system_settings_form($form);
}