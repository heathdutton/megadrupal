<?php

/**
 * @file
 * Administrative pages for the advanced_page.
 */

/**
 * Build advanced_page_settings_form form.
 *
 * @param array $form_state
 *   An associative array containing the current state of the form.
 *
 * @return array
 *   The created form.
 */
function advanced_page_settings_form($form_state) {
  $form = array();

  $form['advanced_page_url'] = array(
    '#type' => 'checkbox',
    '#title' => t('Page URL'),
    '#default_value' => variable_get('advanced_page_url', FALSE),
    '#description' => t('Show/hide url text filed on content authoring form.'),
  );
  $form['advanced_page_url_alias'] = array(
    '#type' => 'textfield',
    '#title' => t('Page url alias prefix'),
    '#size' => 60,
    '#maxlength' => 128,
    '#default_value' => variable_get('advanced_page_url_alias', 'page/'),
    '#description' => t('Set page url alias prefix for the content node.'),
    '#states' => array(
      'visible' => array(
        ':input[name="advanced_page_url"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['advanced_page_javascript_field'] = array(
    '#type' => 'checkbox',
    '#title' => t('Javascript file field'),
    '#default_value' => variable_get('advanced_page_javascript_field', 0),
    '#description' => t('Show javascript file upload field on content authoring form.'),
  );
  $form['advanced_page_css_field'] = array(
    '#type' => 'checkbox',
    '#title' => t('Css file field'),
    '#default_value' => variable_get('advanced_page_css_field', 0),
    '#description' => t('Show CSS file upload on content authoring form.'),
  );
  $form['advanced_page_file_upload_field'] = array(
    '#type' => 'checkbox',
    '#title' => t('File Attachment'),
    '#default_value' => variable_get('advanced_page_file_upload_field', FALSE),
    '#description' => t('Show file attachment field on content authoring form.'),
  );
  $form['advanced_page_aux_content_field'] = array(
    '#type' => 'checkbox',
    '#title' => t('Auxiliary Content'),
    '#default_value' => variable_get('advanced_page_aux_content_field', FALSE),
    '#description' => t('Show Auxiliary Content field on content authoring form.'),
  );
  $form['advanced_page_aux_content_field_title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Title for Auxiliary Content'),
    '#default_value' => variable_get('advanced_page_aux_content_field_title', FALSE),
    '#description' => t('Show Title Field for Auxiliary Content'),
    '#states' => array(
      'invisible' => array(
        ':input[name="advanced_page_aux_content_field"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form['advanced_page_menu'] = array(
    '#type' => 'checkbox',
    '#title' => t('Menu list'),
    '#default_value' => variable_get('advanced_page_menu', FALSE),
    '#description' => t('Show/hide  site wide menu list on  content authoring form.'),
  );

  return system_settings_form($form);
}
