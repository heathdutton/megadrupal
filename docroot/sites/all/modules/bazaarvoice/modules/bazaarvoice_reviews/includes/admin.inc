<?php
/**
 * @file
 * Provides the Bazaarvoice Reviews administrative interface.
 */

/**
 * Review settings form.
 */
function bazaarvoice_reviews_settings_form($form, &$form_state) {
  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Product Review Settings'),
    '#description' => t('Settings for submitting product reviews'),
  );

  $form['settings']['bazaarvoice_reviews_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable product reviews'),
    '#default_value' => variable_get('bazaarvoice_reviews_enable', TRUE),
  );

  $form['settings']['bazaarvoice_reviews_secure'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use secure page for reviews form.'),
    '#description' => t('If the review submission page should use HTTPS. Requires site to be !link', array('!link' => l(t('configured to use HTTPS.'), 'https://drupal.org/https-information'))),
    '#default_value' => variable_get('bazaarvoice_reviews_secure', TRUE),
  );

  $form['settings']['bazaarvoice_reviews_method'] = array(
    '#type' => 'select',
    '#title' => t('Reviews Method'),
    '#description' => t('Select which method to use for displaying review form and submitting reviews.'),
    '#options' => array(
      'hosted' => t('Hosted UI'),
      'conversation' => t('Conversations API (self hosted form)'),
    ),
    '#default_value' => variable_get('bazaarvoice_reviews_method', 'hosted'),
  );

  $form['settings']['hosted'] = array(
    '#type' => 'fieldset',
    '#title' => t('Hosted UI settings'),
    '#description' => t('Settings needed for Bazaarvoice hosted reviews'),
    '#states' => array(
      'visible' => array(
        ':input[name="bazaarvoice_reviews_method"]' => array('value' => 'hosted'),
      ),
    ),
  );

  $form['settings']['hosted']['bazaarvoice_reviews_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Reviews Base URL'),
    '#description' => t('Base url for writing reviews, supplied by Bazaarvoice. ex: reviews.client.com (do not include http(s)://)'),
    '#default_value' => variable_get('bazaarvoice_reviews_url', ''),
  );

  $form['settings']['hosted']['bazaarvoice_reviews_display_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Display Code'),
    '#description' => t('Display code for writing reviews. Supplied by Bazaarvoice.'),
    '#default_value' => variable_get('bazaarvoice_reviews_display_id', ''),
  );

  $form['settings']['hosted']['bazaarvoice_reviews_shared_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Shared Key'),
    '#default_value' => variable_get('bazaarvoice_reviews_shared_key', ''),
  );

  $form['url_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Review URL Settings'),
    '#description' => t('Settings for the urls for review pages.'),
  );

  $form['url_settings']['bazaarvoice_reviews_reviews_uri'] = array(
    '#type' => 'textfield',
    '#title' => t('Reviews path'),
    '#description' => t('Path for viewing the reviews for a product entity. Relative to the entity page.'),
    '#default_value' => bazaarvoice_reviews_get_reviews_uri(),
  );

  $form['url_settings']['bazaarvoice_reviews_write_uri'] = array(
    '#type' => 'textfield',
    '#title' => t('Write A Review path'),
    '#description' => t('Path for the write a review page, relative to the entity page.'),
    '#required' => TRUE,
    '#default_value' => bazaarvoice_reviews_get_write_a_review_uri(),
  );

  $form['display'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display Settings'),
  );

  $form['display']['bazaarvoice_reviews_per_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Reviews Per Page'),
    '#description' => t('How many reviews to display per page. (Cannot exceed 100)'),
    '#default_value' => variable_get('bazaarvoice_reviews_per_page', 10),
  );

  $form['cache'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cache Settings'),
  );

  $form['cache']['bazaarvoice_reviews_cache'] = array(
    '#type' => 'select',
    '#title' => t('Review Cache Lifetime'),
    '#description' => t('Amount of time to cache reviews for a product before calling from API.'),
    '#options' => array(
      0 => t('Never'),
      3600 => t('Hourly'),
      43200 => t('Twice a Day'),
      86400 => t('Daily'),
      604800 => t('Weekly'),
    ),
    '#default_value' => variable_get('bazaarvoice_reviews_cache', 0),
  );

  $form = system_settings_form($form);

  return $form;
}

/**
 * Validation callback for settings form.
 */
function bazaarvoice_reviews_settings_form_validate($form, &$form_state) {
  $per_page = $form_state['values']['bazaarvoice_reviews_per_page'];

  if (!is_numeric($per_page) || !is_int($per_page + 0) || ($per_page < 0)) {
    form_set_error('bazaarvoice_reviews_per_page', t('Must Provide an integer value greater then 0.'));
  }
  elseif ($per_page > 100) {
    form_set_error('bazaarvoice_reviews_per_page', t('Must Provide an integer value less then 100.'));
  }

  // Make sure value is safe for use in a url.
  if (!valid_url($form_state['values']['bazaarvoice_reviews_reviews_uri'])) {
    form_set_error('bazaarvoice_reviews_reviews_uri', t('Reviews path is not a valid uri string.'));
  }

  // Make sure value is safe for use in a url.
  if (!valid_url($form_state['values']['bazaarvoice_reviews_write_uri'])) {
    form_set_error('bazaarvoice_reviews_write_uri', t('Write a Review path is not a valid uri string.'));
  }
}
