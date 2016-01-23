<?php

/**
 * @file
 * Admin form callbacks.
 */

/**
 * Form callback: Add/Edit a new socolissimo rate.
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form state array.
 * @param object $commerce_socolissimo_rate
 *   A fully qualified So Colissimo Flexibility rate array.
 *
 * @return array
 *   The form array.
 */
function commerce_socolissimo_rate_form($form, &$form_state, $commerce_socolissimo_rate) {
  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files']['form'] = drupal_get_path('module', 'commerce_socolissimo') . '/includes/commerce_socolissimo.forms.inc';

  // Attach assets.
  $form['#attached']['js'] = array(
    drupal_get_path('module', 'commerce_socolissimo') . '/theme/js/commerce_socolissimo_range.js',
  );
  $form['#attached']['library'] = array(
    array('system', 'ui.slider'),
  );
  $form['#attached']['css'] = array(
    drupal_get_path('module', 'commerce_socolissimo') . '/theme/css/commerce_socolissimo_add_rate.css',
  );

  // Store the socolissimo rate in the form state.
  $form_state['commerce_socolissimo_rate'] = $commerce_socolissimo_rate;
  field_attach_form('commerce_socolissimo_rate', $commerce_socolissimo_rate, $form, $form_state);

  // Do not expose belgium rate to non-belgium services.
  if (!_commerce_socolissimo_is_belgian_service($commerce_socolissimo_rate->name)) {
    unset($form['socolissimo_flex_rate_be']);
  }

  // User is not allowed to change the value.
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Service name'),
    '#default_value' => $commerce_socolissimo_rate->name,
    '#required' => TRUE,
    '#size' => 32,
    '#maxlength' => 32,
    '#access' => FALSE,
    '#element_validate' => array('commerce_socolissimo_element_validate_shipping_service'),
  );

  // Adds a container used as a range slider.
  $form['container'] = array(
    '#type' => 'container',
    '#id' => 'socolissimo-range-slider',
    '#weight' => 5,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save the rate'),
    '#weight' => 50,
  );

  return $form;
}

/**
 * Form validation callback.
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form state array.
 */
function commerce_socolissimo_rate_form_validate($form, &$form_state) {
  // Ensure minimum weight is lower than maximum weight.
  if ($form_state['values']['socolissimo_flex_min_weight'][LANGUAGE_NONE][0]['weight'] >= $form_state['values']['socolissimo_flex_max_weight'][LANGUAGE_NONE][0]['weight']) {
    form_set_error(
      'socolissimo_flex_min_weight', t(
        '@minimum_title (@minimum_value) must be lower than @maximum_title (@maximum_value)', array(
          '@minimum_title' => $form['socolissimo_flex_min_weight'][LANGUAGE_NONE]['#title'],
          '@minimum_value' => $form_state['values']['socolissimo_flex_min_weight'][LANGUAGE_NONE][0]['weight'] . ' ' . $form_state['values']['socolissimo_flex_min_weight'][LANGUAGE_NONE][0]['unit'],
          '@maximum_title' => $form['socolissimo_flex_max_weight'][LANGUAGE_NONE]['#title'],
          '@maximum_value' => $form_state['values']['socolissimo_flex_max_weight'][LANGUAGE_NONE][0]['weight'] . ' ' . $form_state['values']['socolissimo_flex_max_weight'][LANGUAGE_NONE][0]['unit'],
        )
      )
    );
  }

  // Notify field widgets to validate their data.
  field_attach_form_validate('commerce_socolissimo_rate', $form_state['commerce_socolissimo_rate'], $form, $form_state);
}

/**
 * Form submission callback.
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form state array.
 */
function commerce_socolissimo_rate_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $commerce_socolissimo_rate = $form_state['commerce_socolissimo_rate'];
  // Storing this flah in a variable because after saving the object, the flag
  // is dropped.
  $is_new = (isset($commerce_socolissimo_rate->is_new)) ? TRUE : FALSE;

  if (!empty($values['name']) || !isset($commerce_socolissimo_rate)) {
    if ($is_new) {
      $commerce_socolissimo_rate->changed = $commerce_socolissimo_rate->created = REQUEST_TIME;
    }
    else {
      $commerce_socolissimo_rate->changed = REQUEST_TIME;
    }

    // Notify field widgets.
    field_attach_submit('commerce_socolissimo_rate', $commerce_socolissimo_rate, $form, $form_state);

    // Redirects customer to the relative socolissimo shipping service page.
    $form_state['redirect'] = 'admin/commerce/config/shipping/services/socolissimo-flexibility-' . strtr($commerce_socolissimo_rate->name, '_', '-') . '/rates';

    // Saves the rate.
    if (commerce_socolissimo_rate_save($commerce_socolissimo_rate)) {
      if ($is_new) {
        drupal_set_message(t('Rate @rate_id has been created.', array('@rate_id' => $commerce_socolissimo_rate->rate_id)));
        watchdog('commerce_socolissimo', 'Created rate @rate_id.', array('@rate_id' => $commerce_socolissimo_rate->rate_id), WATCHDOG_NOTICE);
      }
      else {
        drupal_set_message(t('Rate @rate_id has been updated.', array('@rate_id' => $commerce_socolissimo_rate->rate_id)));
        watchdog('commerce_socolissimo', 'Updated rate @rate_id.', array('@rate_id' => $commerce_socolissimo_rate->rate_id), WATCHDOG_NOTICE);
      }
    }
    else {
      if ($is_new) {
        drupal_set_message(t('Rate @rate_id could not be created.', array('@rate_id' => $commerce_socolissimo_rate->rate_id)), 'error');
      }
      else {
        drupal_set_message(t('Rate @rate_id could not be updated.', array('@rate_id' => $commerce_socolissimo_rate->rate_id)), 'error');
      }
    }
  }
}

/**
 * Element validation callback.
 *
 * Ensures the field value is a SoColissimo shipping service.
 *
 * @param array $element
 *   The form element array.
 * @param array $form_state
 *   The form state array.
 */
function commerce_socolissimo_element_validate_shipping_service($element, &$form_state) {
  if (!array_key_exists($element['#value'], commerce_shipping_services('socolissimo_flexibility'))) {
    form_error($element, t('%name must be a a valid So Colissimo Flexibility shipping service.', array('%name' => $element['#value'])));
  }
}


/**
 * Form callback: Delete a socolissimo flexibility rate.
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form state array.
 * @param object $commerce_socolissimo_rate
 *   A fully loaded commerce_socolissimo_rate object.
 */
function commerce_socolissimo_delete_rate_form($form, &$form_state, $commerce_socolissimo_rate) {
  $form_state['commerce_socolissimo_rate'] = $commerce_socolissimo_rate;

  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files']['form'] = drupal_get_path('module', 'commerce_socolissimo') . '/includes/commerce_socolissimo.forms.inc';

  $form['#submit'][] = 'commerce_socolissimo_delete_rate_form_submit';

  $form = confirm_form(
    $form,
    t('Are you sure you want to delete rate @rate_id ?', array('@rate_id' => $commerce_socolissimo_rate->rate_id)),
    'admin/commerce/config/shipping/services/socolissimo-flexibility-' . strtr($commerce_socolissimo_rate->name, '_', '-') . '/rates',
    '<p>' . t('Deleting this rate cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submit callback for commerce_order_order_delete_form().
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form state array.
 */
function commerce_socolissimo_delete_rate_form_submit($form, &$form_state) {
  $commerce_socolissimo_rate = $form_state['commerce_socolissimo_rate'];

  // Redirects customer to the relative socolissimo shipping service page.
  $form_state['redirect'] = 'admin/commerce/config/shipping/services/socolissimo-flexibility-' . strtr($commerce_socolissimo_rate->name, '_', '-') . '/rates';

  if (commerce_socolissimo_rate_delete($commerce_socolissimo_rate->rate_id)) {
    drupal_set_message(t('Rate @rate_id has been deleted.', array('@rate_id' => $commerce_socolissimo_rate->rate_id)));
    watchdog('commerce_socolissimo', 'Deleted rate @rate_id.', array('@rate_id' => $commerce_socolissimo_rate->rate_id), WATCHDOG_NOTICE);
  }
  else {
    drupal_set_message(t('Rate @rate_id could not be deleted.', array('@rate_id' => $commerce_socolissimo_rate->rate_id)), 'error');
  }
}


/**
 * Form callback: Activate or disable a shipping service.
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form state array.
 * @param string $service_name
 *   The shipping service name.
 */
function commerce_socolissimo_status_form($form, &$form_state, $service_name) {
  // Get shipping service definition.
  $shipping_service = commerce_shipping_service_load($service_name);

  // Attempting to get the status of passed shipping service.
  if (!$shipping_service_status = commerce_socolissimo_service_status_load($service_name)) {
    $shipping_service_status = commerce_socolissimo_service_status_new($service_name);
  }

  $status = ($shipping_service_status['enabled'] == 0) ? t('enable') : t('disable');

  $form_state['socolissimo_flexibility_service_status'] = $shipping_service_status;
  $form_state['shipping_service'] = $shipping_service;

  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files']['form'] = drupal_get_path('module', 'commerce_socolissimo') . '/includes/commerce_socolissimo.forms.inc';

  $form['#submit'][] = 'commerce_socolissimo_status_form_submit';

  $form = confirm_form(
    $form,
    t('@service_human_name status', array('@service_human_name' => $shipping_service['display_title'])),
    'admin/commerce/config/shipping/services/socolissimo-flexibility',
    t(
      'Are you sure to @status the @service_human_name shipping service ?', array(
        '@service_human_name' => $shipping_service['display_title'],
        '@status' => $status,
      )
    ),
    drupal_ucfirst($status),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submit callback for commerce_socolissimo_status_form().
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form state array.
 */
function commerce_socolissimo_status_form_submit($form, &$form_state) {
  $shipping_service_status = $form_state['socolissimo_flexibility_service_status'];
  $shipping_service = $form_state['shipping_service'];

  if ($shipping_service_status['enabled'] == 0) {
    $status = t('enabled');
    $shipping_service_status['enabled'] = 1;
  }
  else {
    $status = t('disabled');
    $shipping_service_status['enabled'] = 0;
  }

  // Redirects customer to the relative socolissimo shipping services page.
  $form_state['redirect'] = 'admin/commerce/config/shipping/services/socolissimo-flexibility';

  if (commerce_socolissimo_service_status_save($shipping_service_status)) {
    drupal_set_message(
      t(
        'The shipping service @service_human_name has been @status.', array(
          '@service_human_name' => $shipping_service['display_title'],
          '@status' => $status,
        )
      )
    );
    watchdog(
      'commerce_socolissimo', 'Disabled shipping service: @service_human_name.', array(
        '@service_human_name' => $shipping_service['display_title'],
        '@status' => $status,
      ), WATCHDOG_NOTICE
    );
  }
  else {
    drupal_set_message(
      t(
        'Shipping service @service_human_name could not be @status.', array(
          '@service_human_name' => $shipping_service['display_title'],
          '@status' => $status,
        )
      ), 'error'
    );
  }
}
