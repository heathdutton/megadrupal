<?php
/**
 * @file
 * Parcel Monkey administration menu items.
 */

/**
 * Parcel Monkey Quote settings.
 *
 * Record Parcel Monkey account information necessary to use the service.
 * Configure which Parcel Monkey services are quoted to customers.
 *
 * @ingroup forms
 * @see uc_admin_settings_validate()
 */
function commerce_parcel_monkey_settings_form() {
  $form = array();

  $form['commerce_parcel_monkey_user_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Parcel Monkey User ID'),
    '#default_value' => variable_get('commerce_parcel_monkey_user_id', ''),
    '#required' => TRUE,
  );

  $form['commerce_parcel_monkey_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Parcel Monkey API Key'),
    '#description' => t('Create an account at <a href="http://www.parcelmonkey.co.uk/register.php" title="Parcel Monkey" target="_blank">http://www.parcelmonkey.co.uk/register.php</a> and request your API Key.'),
    '#default_value' => variable_get('commerce_parcel_monkey_api_key', ''),
    '#required' => TRUE,
  );

  foreach (_commerce_parcel_monkey_service_list() as $key => $service) {
    $array_options[$key] = $service;
  }

  $form['commerce_parcel_monkey_services'] = array(
    '#type' => 'select',
    '#title' => t('Parcel Monkey Services'),
    '#options' => $array_options,
    '#default_value' => variable_get('commerce_parcel_monkey_services', array()),
    '#description' => t('Select the Parcel Monkey services that are available to customers. Hold down CMD on Mac or CTRL on PC to select multiples.'),
    '#multiple' => TRUE,
  );

  $form['commerce_parcel_monkey_collection_town'] = array(
    '#type' => 'textfield',
    '#title' => t('Collection Town'),
    '#default_value' => variable_get('commerce_parcel_monkey_collection_town', ''),
    '#required' => TRUE,
  );

  $form['commerce_parcel_monkey_collection_postcode'] = array(
    '#type' => 'textfield',
    '#title' => t('Collection Postcode'),
    '#default_value' => variable_get('commerce_parcel_monkey_collection_postcode', ''),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}
