<?php

/**
 * @file
 * Handles admin settings page for Commerce NZ Post module.
 */

/**
 * Implements hook_settings_form().
 */
function commerce_nzpost_settings_form($form, &$form_state) {
  $form['api'] = array(
    '#type' => 'fieldset',
    '#title' => t('NZ Post API credentials'),
    '#collapsible' => TRUE,
    '#description' => t('In order to obtain shipping rate estimates, you must have an account with NZ Post. You can apply for NZ Post API credentials at !nzpost',
      array(
        '!nzpost' => l(t('nzpost.co.nz'),
          'http://www.nzpost.co.nz/products-services/iphone-apps-apis/locator-api/how-the-international-ratefinder-api-works',
          array(
            'attributes' => array(
              'target' => '_blank',
            ),
          )
        ),
      )
    ),
  );

  $form['api']['commerce_nzpost_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('NZ Post API Key'),
    '#default_value' => variable_get('commerce_nzpost_api_key'),
    '#required' => TRUE,
  );

  $form['services'] = array(
    '#type' => 'fieldset',
    '#title' => t('Enable NZ Post Shipping Services'),
    '#collapsible' => TRUE,
  );

  foreach (_commerce_nzpost_service_list() as $key => $service) {
    $array_options[$key] = $service['title'];
  }
  $form['services']['commerce_nzpost_services'] = array(
    '#type' => 'checkboxes',
    '#options' => $array_options,
    '#default_value' => variable_get('commerce_nzpost_services', array()),
  );

  // Fields for default package size (cm).
  $form['default_package_size'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default maximum package size (cm)'),
    '#collapsible' => FALSE,
    '#description' => t('NZ Post requires a package size when determining estimates.'),
  );
  $form['default_package_size']['commerce_nzpost_default_package_size_length'] = array(
    '#type' => 'textfield',
    '#title' => t('Length'),
    '#size' => 5,
    '#required' => TRUE,
    '#default_value' => variable_get('commerce_nzpost_default_package_size_length'),
  );
  $form['default_package_size']['commerce_nzpost_default_package_size_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#size' => 5,
    '#required' => TRUE,
    '#default_value' => variable_get('commerce_nzpost_default_package_size_width'),
  );
  $form['default_package_size']['commerce_nzpost_default_package_size_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#size' => 5,
    '#required' => TRUE,
    '#default_value' => variable_get('commerce_nzpost_default_package_size_height'),
  );

  $markup = t('The package size is used to determine the number of packages necessary to create a ' .
    'NZ Post shipping cost estimate. <strong> NZ Post shipping will not be ' .
    'available to items with no dimensions or weight.</strong> The logic implemented works as: ' .
    '<ul>' .
    '<li>If the total products exceed the size of one package,</li> ' .
    '<li>Pack them into the smallest box possible filling up the maximum size first.</li> ' .
    '<li>This logic is suited to some packing decisions - see commerce_nzpost_build_rate_request() for implmentation ' .
    'details. If you have fixed box sizes this will need to be modified to suit your needs.</li> ' .
    '</ul>');

  $form['default_package_size']['details'] = array(
    '#markup' => $markup,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Implements hook_form_validate().
 */
function commerce_nzpost_settings_form_validate($form, &$form_state) {

  $values = $form_state['values'];

  if (!is_numeric($values['commerce_nzpost_default_package_size_length']) || $values['commerce_nzpost_default_package_size_length'] <= 0) {
    form_set_error('commerce_nzpost_default_package_size_length', t('Length must be a positive number.'));

  }
  if (!is_numeric($values['commerce_nzpost_default_package_size_width']) || $values['commerce_nzpost_default_package_size_width'] <= 0) {
    form_set_error('commerce_nzpost_default_package_size_width', t('Width must be a positive number.'));

  }
  if (!is_numeric($values['commerce_nzpost_default_package_size_height']) || $values['commerce_nzpost_default_package_size_height'] <= 0) {
    form_set_error('commerce_nzpost_default_package_size_height', t('Height must be a positive number.'));

  }

  // Ensure the default package size is less than the maximum.
  $dimensions = array(
    $values['commerce_nzpost_default_package_size_length'],
    $values['commerce_nzpost_default_package_size_width'],
    $values['commerce_nzpost_default_package_size_height'],
  );
  sort($dimensions);
  list($height, $width, $length) = $dimensions;
  //$girth = 2 * $width + 2 * $height;
  if ($length > 105) {
    form_set_error('commerce_nzpost_default_package_size_length', t('The greatest dimension of the package size must be 150 cm or less.'));
  }
}

/**
 * Implements hook_form_submit().
 */
function commerce_nzpost_settings_form_submit($form, &$form_state) {

  $services = variable_get('commerce_nzpost_services', NULL);

  // If the selected services have changed then rebuild caches.
  if ($services !== $form_state['values']['commerce_nzpost_services']) {
    variable_set('commerce_nzpost_services', $form_state['values']['commerce_nzpost_services']);
    commerce_shipping_services_reset();
    entity_defaults_rebuild();
    rules_clear_cache(TRUE);
    menu_rebuild();
  }

  variable_set('commerce_nzpost_api_key', $form_state['values']['commerce_nzpost_api_key']);
  variable_set('commerce_nzpost_default_package_size_length', $form_state['values']['commerce_nzpost_default_package_size_length']);
  variable_set('commerce_nzpost_default_package_size_width', $form_state['values']['commerce_nzpost_default_package_size_width']);
  variable_set('commerce_nzpost_default_package_size_height', $form_state['values']['commerce_nzpost_default_package_size_height']);

  drupal_set_message(t('The NZ Post configuration options have been saved.'));
}
