<?php

/**
 * @file
 * Administration menu callbacks for Plista.
 */

/**
 * Configuration interface.
 *
 * @param array $form
 *   The form array
 * @param array $form_state
 *   The form_state array as reference
 *
 * @return mixed
 *   The configuration interface form
 */
function plista_admin_form($form, &$form_state) {

  $form['#validate'][] = 'plista_form_validate';

  $plista_basic = variable_get('plista_basic', array());

  $form['plista_basic'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic settings'),
    '#weight' => 5,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
  );

  $form['plista_basic']['plista_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Plista Integration'),
    '#default_value' => isset($plista_basic['plista_enabled']) ? $plista_basic['plista_enabled'] : 'false',
  );

  $form['plista_basic']['plista_javascript_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Plista javascript url'),
    '#default_value' => isset($plista_basic['plista_javascript_url']) ? $plista_basic['plista_javascript_url'] : '',
    '#size' => 60,
    '#maxlength' => 90,
    '#description' => t('Javascript url for your specific plista account. Usually starts with http://static.plista.com/'),
    '#required' => TRUE,
  );

  $form['plista_basic']['plista_widgetname'] = array(
    '#type' => 'textfield',
    '#title' => t('Plista widgetname'),
    '#default_value' => isset($plista_basic['plista_widgetname']) ? $plista_basic['plista_widgetname'] : '',
    '#size' => 60,
    '#maxlength' => 90,
    '#description' => t('You will receive it from plista'),
    '#required' => TRUE,
  );

  $form['plista_basic']['plista_field_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Node title'),
    '#default_value' => isset($plista_basic['plista_field_title']) ? $plista_basic['plista_field_title'] : '[node:title]',
    '#size' => 60,
    '#maxlength' => 90,
    '#description' => t('Title of the node'),
    '#required' => TRUE,
  );

  $form['plista_basic']['plista_field_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Node summary'),
    '#default_value' => isset($plista_basic['plista_field_text']) ? $plista_basic['plista_field_text'] : '[node:summary]',
    '#size' => 60,
    '#maxlength' => 90,
    '#description' => t('Summary of the node'),
    '#required' => TRUE,
  );

  $form['plista_basic']['plista_field_img'] = array(
    '#type' => 'textfield',
    '#title' => t('Node image'),
    '#default_value' => isset($plista_basic['plista_field_img']) ? $plista_basic['plista_field_img'] : '',
    '#size' => 60,
    '#maxlength' => 90,
    '#description' => t('Small image of the node'),
    '#required' => TRUE,
  );

  $form['plista_basic']['plista_field_category'] = array(
    '#type' => 'textfield',
    '#title' => t('Node category'),
    '#default_value' => isset($plista_basic['plista_field_category']) ? $plista_basic['plista_field_category'] : '[node:field_tags]',
    '#size' => 60,
    '#maxlength' => 90,
    '#description' => t('Category of the node'),
    '#required' => TRUE,
  );

  $form['plista_basic']['tokens'] = array(
    '#title' => t('Replacement patterns'),
    '#type' => 'fieldset',
    '#theme' => 'token_tree',
    '#token_types' => array('node'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $types = node_type_get_types();
  $options = array();
  foreach ($types as $type => $info) {
    $options[$type] = $info->name;
  }

  $form['plista_basic']['plista_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Node types'),
    '#options' => $options,
    '#default_value' => isset($plista_basic['plista_node_types']) ? $plista_basic['plista_node_types'] : array(),
    '#description' => t('Node types on which plista widget should be enabled'),
    '#required' => TRUE,
  );

  $form['plista_basic']['plista_hidden_paths'] = array(
    '#title' => t('Hidden paths'),
    '#type' => 'textarea',
    '#description' => t('Enter URL patterns on which the plista widget should be hidden'),
    '#default_value' => isset($plista_basic['plista_hidden_paths']) ? $plista_basic['plista_hidden_paths'] : '',
  );

  $form['plista_advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#weight' => 5,
    '#collapsible' => FALSE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
    '#description' => t("Used for updating and deleting nodes through plista API. The fields are not required. Widget is also working without them."),
  );

  $plista_advanced = variable_get('plista_advanced', array());
  $form['plista_advanced']['plista_update_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Update URL'),
    '#default_value' => isset($plista_advanced['plista_update_url']) ? $plista_advanced['plista_update_url'] : 'http://farm.plista.com/api/item/',
    '#size' => 60,
    '#maxlength' => 90,
    '#required' => TRUE,
  );

  $form['plista_advanced']['plista_domain_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Domain ID'),
    '#default_value' => isset($plista_advanced['plista_domain_id']) ? $plista_advanced['plista_domain_id'] : '',
    '#size' => 60,
    '#maxlength' => 90,
  );

  $form['plista_advanced']['plista_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Plista Api Key'),
    '#default_value' => isset($plista_advanced['plista_api_key']) ? $plista_advanced['plista_api_key'] : '',
    '#size' => 60,
    '#maxlength' => 90,
  );

  return system_settings_form($form);
}

/**
 * Form validation callback.
 *
 * @param array $form
 *   The form array
 * @param array $form_state
 *   The form_state array as reference
 */
function plista_form_validate($form, &$form_state) {

  if (!valid_url($form_state['values']['plista_basic']['plista_javascript_url'], TRUE)) {
    form_set_error('plista_javascript_url', t('Type in a valid URL'));
  }
}
