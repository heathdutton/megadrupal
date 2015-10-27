<?php

/**
 * @file
 * File containing the administration form for the opensearchplugin module.
 */

/**
 * Display help and module information.
 *
 * @param section which section of the site we're displaying help
 * @return help text for section
 */
function opensearchplugin_help($path, $arg) {
  $output = '';
  switch ($path) {
    case "admin/help#opensearchplugin":
    case 'admin/config/search/opensearchplugin':
      $output .= '<p>' . t("OpenSearch provides browser search integration via the OpenSearch plugin protocal.") . '</p>';
      break;
  }
  return $output;
}

/**
 * The administration settings for OpenSearch Plugin
 */
function opensearchplugin_admin() {
  $form = array();
  $form['opensearchplugin_short_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Short name'),
    '#default_value' => variable_get('opensearchplugin_short_name', variable_get('site_name', 'Drupal')),
  );
  $form['opensearchplugin_long_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Long name'),
    '#default_value' => variable_get('opensearchplugin_long_name', variable_get('site_name', 'Drupal')),
  );
  // Custom search path
  $form['path'] = array(
    '#type' => 'fieldset',
    '#title' => t('Path Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['path']['opensearchplugin_custom_path'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use a custom search path'),
    '#default_value' => variable_get('opensearchplugin_custom_path', FALSE),
  );
  $form['path']['opensearchplugin_search_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Search path'),
    '#default_value' => variable_get('opensearchplugin_search_path', ''),
    '#description' => t('E.g. my/view/ if searching on "foo" must result in visiting my/view/foo'),
    '#states' => array(
      'visible' => array(
        ':input[name="opensearchplugin_custom_path"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['path']['opensearchplugin_search_form'] = array(
    '#type' => 'textfield',
    '#title' => t('Search form'),
    '#default_value' => variable_get('opensearchplugin_search_form', ''),
    '#description' => t('The relative path to the page containing the search form. E.g. my/view/.'),
    '#states' => array(
      'visible' => array(
        ':input[name="opensearchplugin_custom_path"]' => array('checked' => TRUE),
      ),
    ),
  );
  // Image Settings
  $form['image'] = array(
    '#type' => 'fieldset',
    '#title' => t('Image Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['image']['opensearchplugin_image'] = array(
    '#type' => 'textarea',
    '#title' => t('Image'),
    '#default_value' => variable_get('opensearchplugin_image', ''),
    '#description' => t('The icon to associate with the OpenSearch plugin. It is recommended you use a <a href="@dataurischeme">Data URI Scheme</a>. If left blank, will reference the favicon used by your theme.'),
  );
  $form['image']['opensearchplugin_image_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => variable_get('opensearchplugin_image_width', 16),
    '#description' => t('The width of the image.'),
  );
  $form['image']['opensearchplugin_image_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#default_value' => variable_get('opensearchplugin_image_height', 16),
    '#description' => t('The height of the image.'),
  );
  return system_settings_form($form);
}

