<?php

/**
 * @file
 * Provides page callback functions for Fortissimmo
 */

/**
 * Callback function to render the Fortissimmo property detail page
 */
function fortissimmo_property_detail_page($fortissimmo_id) {
  $accountid = fortissimmo_property_get_accountid($fortissimmo_id);
  // @TODO add Context integration (general + transaction type)

  // get the selected detail View for properties of this account
  $view = variable_get('fortissimmo_views_detail_view_' . $accountid, 'fortissimmo_' . $accountid . '_property_detail');
  $output['views'] = views_embed_view($view, 'default', $fortissimmo_id);
  drupal_alter('fortissimmo_property_detail', $output);
  return implode('', $output);
}

/**
 * Fortissimmo request form
 */
function fortissimmo_request_form() {
  $accountid = variable_get('fortissimmo_default_account', 0);
  $form = array();

  $form['contact'] = array(
    '#type' => 'fieldset',
    '#title' => t('Contact information'),
    '#weight' => -10,
  );

  $form['contact']['title'] = array(
    '#type' => 'select',
    '#title' => t('Title'),
    '#required' => TRUE,
    '#multiple' => FALSE,
    '#options' => fortissimmo_get_titles($accountid),
  );

  $form['contact']['firstname'] = array(
    '#type' => 'textfield',
    '#title' => t('First name'),
    '#required' => TRUE,
  );

  $form['contact']['lastname'] = array(
    '#type' => 'textfield',
    '#title' => t('Last name'),
    '#required' => TRUE,
  );

  $form['contact']['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#required' => TRUE,
  );

  $form['contact']['address'] = array(
    '#type' => 'textfield',
    '#title' => t('Address'),
    '#required' => FALSE,
  );

  $form['contact']['zipcode'] = array(
    '#type' => 'textfield',
    '#title' => t('Zipcode'),
    '#required' => FALSE,
  );

  $form['contact']['city'] = array(
    '#type' => 'textfield',
    '#title' => t('City'),
    '#required' => FALSE,
  );

  $form['contact']['phone'] = array(
    '#type' => 'textfield',
    '#title' => t('Telephone'),
    '#required' => FALSE,
  );

  $form['property'] = array(
    '#type' => 'fieldset',
    '#title' => t('Property information'),
    '#weight' => -9,
  );


  $form['property']['bedroom_min'] = array(
    '#type' => 'select',
    '#title' => t('Min. number of bedrooms'),
    '#required' => FALSE,
    '#multiple' => FALSE,
    '#options' => drupal_map_assoc(range(0, 10)),
    '#default_value' => 0,
  );

  $form['property']['bedroom_max'] = array(
    '#type' => 'select',
    '#title' => t('Max. number of bedrooms'),
    '#required' => FALSE,
    '#multiple' => FALSE,
    '#options' => drupal_map_assoc(range(0, 10)),
    '#default_value' => 10,
  );

  $location_type = variable_get('fortissimmo_request_form_location_search_type', 'area_search');
  if ($location_type == 'area_search') {
    $form['property']['city'] = array(
      '#type' => 'fieldset',
      '#title' => t('Search range'),
    );
    $form['property']['city']['city_round'] = array(
      '#type' => 'select',
      '#title' => t('Search an area of'),
      '#options' => fortissimmo_get_city_round(),
    );

    $form['property']['city']['cityValue'] = array(
      '#type' => 'textfield',
      '#title' => t('around city'),
      '#required' => FALSE,
    );
  }
  else {
    $form['property']['region'] = array(
      '#type' => 'select',
      '#title' => t('Region'),
      '#required' => FALSE,
      '#multiple' => FALSE,
      '#options' => fortissimmo_get_regions($accountid),
    );
  }


  $form['property']['price'] = array(
    '#type' => 'fieldset',
    '#title' => t('Price range'),
  );
  $form['property']['price']['price_min'] = array(
    '#type' => 'textfield',
    '#title' => t('Between'),
    '#required' => FALSE,
    '#suffix' => ' &euro;',
  );
  $form['property']['price']['price_max'] = array(
    '#type' => 'textfield',
    '#title' => t('and'),
    '#required' => FALSE,
    '#suffix' => ' &euro;',
  );

  $fortissimmo_form_transactions = fortissimmo_get_transactions($accountid);
  drupal_alter('fortissimmo_form_transactions', $fortissimmo_form_transactions);
  $form['property']['transaction'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Transaction type'),
    '#options' => $fortissimmo_form_transactions,
    '#default_value' => array(1, 2),
  );

  $types = fortissimmo_get_types($accountid);
  drupal_alter('fortissimmo_form_types', $types);
  $form['property']['typeValue'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Type'),
    '#options' => $types,
  );


  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * Validation handler for fortissimmo_request_form
 */
function fortissimmo_request_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  if (!valid_email_address($values['email'])) {
    form_set_error('email', t('%value is not a valid email address.', array('%value' => $values['email'])));
  }

  if ($values['bedroom_max'] < $values['bedroom_min']) {
    form_set_error('bedroom_max', t('The maximum number of bedrooms needs to be higher than the minimum.'));
  }

  if ($values['price_min'] && !is_numeric($values['price_min'])) {
    form_set_error('price_min', t('The price needs to be numeric'));
  }

  if ($values['price_max'] && !is_numeric($values['price_max'])) {
    form_set_error('price_max', t('The price needs to be numeric'));
  }

  if ($values['price_max'] < $values['price_min']) {
    form_set_error('price_max', t('The maximum price needs to be higher than the minimum.'));
  }
}

/**
 * Submit handler for fortissimmo_request_form
 */
function fortissimmo_request_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  fortissimmo_save_request($values);
  $form_state['redirect'] = 'fortissimmo/form/done';
  }

/**
 * Renders form to get more information about a certain property
 */
function fortissimmo_property_request_form($form, $form_state, $fortissimmo_id) {
  $accountid = variable_get('fortissimmo_default_account', 0);
  $form = array();

  $form['titleid'] = array(
    '#type' => 'select',
    '#title' => t('Title'),
    '#required' => TRUE,
    '#multiple' => FALSE,
    '#options' => fortissimmo_get_titles($accountid),
  );

  $form['firstname'] = array(
    '#type' => 'textfield',
    '#title' => t('First name'),
    '#required' => TRUE,
  );

  $form['lastname'] = array(
    '#type' => 'textfield',
    '#title' => t('Last name'),
    '#required' => TRUE,
  );

  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#required' => TRUE,
  );

  $form['street'] = array(
    '#type' => 'textfield',
    '#title' => t('Street'),
    '#required' => FALSE,
  );

  $form['number'] = array(
    '#type' => 'textfield',
    '#title' => t('Number'),
    '#required' => FALSE,
    '#size' => 20,
  );

  $form['postalcode'] = array(
    '#type' => 'textfield',
    '#title' => t('Zipcode'),
    '#required' => FALSE,
  );

  $form['city'] = array(
    '#type' => 'textfield',
    '#title' => t('City'),
    '#required' => FALSE,
  );

  $form['phone'] = array(
    '#type' => 'textfield',
    '#title' => t('Telephone'),
    '#required' => FALSE,
  );

  $form['fortissimmo_id'] = array(
    '#type' => 'hidden',
    '#value' => $fortissimmo_id,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * Validation handler for fortissimmo_property_request_form
 */
function fortissimmo_property_request_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  if (!valid_email_address($values['email'])) {
    form_set_error('email', t('%value is not a valid email address.', array('%value' => $values['email'])));
  }
}

/**
 * Submit handler for fortissimmo_property_request_form
 */
function fortissimmo_property_request_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  fortissimmo_save_property_request($values);
  $form_state['redirect'] = 'fortissimmo/form/done/' . $values['fortissimmo_id'];
}

/**
 * Render a landing page to be shown after a contact form has been submitted.
 */
function fortissimmo_form_landingpage($property_id = NULL) {
  return theme('fortissimmo_form_landingpage', array('property_id' => $property_id));
}