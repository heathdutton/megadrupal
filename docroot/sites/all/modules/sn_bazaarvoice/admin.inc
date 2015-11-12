<?php

/**
 * @file
 * This file would have all admin configuration methods.
 */

/**
 * Creating a drupal config form for settings.
 */
function sn_bazaarvoice_getConfigurationForm() {
  $form = array();
  $form['panel'] = array(
    '#title' => t('Bazaarvoice parameters'),
    '#description' => t('Provide bazaarvoice related parameters for drupal integration.'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
  );

  $form['panel']['sn_bazaarvoice_bzrvoice_api_version'] = array(
    '#title' => t('Bazaarvoice API version'),
    '#description' => t('Latest bazaarvoice API version.'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('sn_bazaarvoice_bzrvoice_api_version', 5.4),
  );

  $form['panel']['sn_bazaarvoice_bzrvoice_customer_name'] = array(
    '#title' => t('Bazaarvoice customer name'),
    '#description' => t('Provide client/customer name here.'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('sn_bazaarvoice_bzrvoice_customer_name', NULL),
  );

  $form['panel']['sn_bazaarvoice_bzrvoice_user_id'] = array(
    '#title' => t('Bazaarvoice user id'),
    '#description' => t('Provide bazaarvoice login userid.'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('sn_bazaarvoice_bzrvoice_user_id', NULL),
  );

  $form['panel']['sn_bazaarvoice_bzrvoice_app_name'] = array(
    '#title' => t('Application name'),
    '#description' => t('Provide application name created at Bazaarvoice.'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('sn_bazaarvoice_bzrvoice_app_name', NULL),
  );

  $form['panel']['sn_bazaarvoice_bzrvoice_reviews_fetching_url'] = array(
    '#title' => t('Bazaarvoice reviews fetching URL'),
    '#description' => t('Provide bazaarvoice API URL to fetch reviews. Default is sandbox url for development environment.'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('sn_bazaarvoice_bzrvoice_reviews_fetching_url', 'http://stg.api.bazaarvoice.com/data/reviews.json'),
  );

  $form['panel']['sn_bazaarvoice_bzrvoice_reviews_submission_url'] = array(
    '#title' => t('Bazaarvoice reviews submission URL'),
    '#description' => t('Provide bazaarvoice API URL to submit reviews. Default is sandbox url for development environment.'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('sn_bazaarvoice_bzrvoice_reviews_submission_url', 'http://stg.api.bazaarvoice.com/data/submitreview.json'),
  );

  $form['panel']['sn_bazaarvoice_bzrvoice_consumer_key'] = array(
    '#title' => t('Consumer key'),
    '#description' => t('Consumer key retrieved from Bazaarvoice.'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('sn_bazaarvoice_bzrvoice_consumer_key', NULL),
  );

  $form['panel']['sn_bazaarvoice_bzrvoice_cache_interval'] = array(
    '#title' => t('Caching interval'),
    '#description' => t('Interval between every data request call (in minutes)'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('sn_bazaarvoice_bzrvoice_cache_interval', 15),
  );

  $form['rating_panel'] = array(
    '#title' => t('Rating parameters'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
  );

  $form['rating_panel']['sn_bazaarvoice_bzrvoice_rating_content_types'] = array(
    '#title' => t('Content types'),
    '#description' => t('Select content types where you want to include bazaarvoice rating & reviews.'),
    '#type' => 'checkboxes',
    '#options' => sn_bazaarvoice_getContentTypes(),
    '#required' => TRUE,
    '#default_value' => variable_get('sn_bazaarvoice_bzrvoice_rating_content_types', array('page')),
  );

  return system_settings_form($form);
}

/**
 * Returns the list of content types.
 *
 * This method is used to get the content types list to select for 
 * bazaarvoice rating integration.
 *
 * @return mixed
 *   The value from the cache, or FALSE on failure.
 */
function sn_bazaarvoice_getContentTypes() {
  // Including required file to get node types.
  module_load_include('inc', 'node', 'content_types');

  $options = array();
  // Getting list of all content types.
  $types = node_type_get_types();
  // Sorting array alphabetically.
  sort($types);
  // Creating output array.
  foreach ($types as $type) {
    $options[$type->type] = $type->name;
  }

  return $options;
}
