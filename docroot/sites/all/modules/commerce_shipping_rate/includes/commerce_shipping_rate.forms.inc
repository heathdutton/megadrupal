<?php

/**
 * @file
 * Admin form callbacks.
 */

/**
 * Form callback: Add/Edit a new shipping rate.
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form state array.
 * @param object $commerce_shipping_rate
 *   A fully qualified Commerce shipping rate array.
 *
 * @return array
 *   The form array.
 */
function commerce_shipping_rate_form($form, &$form_state, $commerce_shipping_rate) {
  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files']['form'] = drupal_get_path('module', 'commerce_shipping_rate') . '/includes/commerce_shipping_rate.forms.inc';

  // Attach assets.
  $form['#attached']['js'] = array(
    drupal_get_path('module', 'commerce_shipping_rate') . '/theme/js/commerce_shipping_rate_range.js',
  );
  $form['#attached']['library'] = array(
    array('system', 'ui.slider'),
  );
  $form['#attached']['css'] = array(
    drupal_get_path('module', 'commerce_shipping_rate') . '/theme/css/commerce_shipping_rate_add_rate.css',
  );

  // Store the shipping rate in the form state.
  $form_state['commerce_shipping_rate'] = $commerce_shipping_rate;
  field_attach_form('commerce_shipping_rate', $commerce_shipping_rate, $form, $form_state);


  // User is not allowed to change the value.
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Service name'),
    '#default_value' => $commerce_shipping_rate->name,
    '#required' => TRUE,
    '#size' => 32,
    '#maxlength' => 32,
    '#access' => FALSE,
    '#element_validate' => array('commerce_shipping_rate_element_validate_shipping_service'),
  );
  // User is not allowed to change the value.
  $form['method'] = array(
    '#type' => 'textfield',
    '#title' => t('Method name'),
    '#default_value' => $commerce_shipping_rate->method,
    '#required' => TRUE,
    '#size' => 255,
    '#maxlength' => 255,
    '#access' => FALSE,
    '#element_validate' => array('commerce_shipping_rate_element_validate_shipping_method'),
  );

  // Adds a container used as a range slider.
  $form['container'] = array(
    '#type' => 'container',
    '#id' => 'shipping-rates-range-slider',
    '#weight' => 5,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save the rate'),
    '#weight' => 55,
    '#op' => 'edit',
  );
  $path_redirect = 'admin/commerce/config/shipping/shipping-rates/' . strtr($commerce_shipping_rate->method, '_', '-') . '/shipping-rates-' . strtr($commerce_shipping_rate->name, '_', '-') . '/rates';
  $options_redirect = array();
  if (isset($_GET['destination']) && !url_is_external($_GET['destination'])) {
    $destination = drupal_parse_url($_GET['destination']);
    $path_redirect = $destination['path'];
    $options_redirect['query'] = $destination['query'];
    $options_redirect['fragment'] = $destination['fragment'];
  }
  $form['cancel'] = array(
    '#markup' => l(t('Cancel'), $path_redirect, $options_redirect),
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
function commerce_shipping_rate_form_validate($form, &$form_state) {
  // Ensure minimum weight is lower than maximum weight.
  if ($form_state['values']['shipping_rate_min_weight'][LANGUAGE_NONE][0]['weight'] >= $form_state['values']['shipping_rate_max_weight'][LANGUAGE_NONE][0]['weight']) {
    form_set_error(
      'shipping_rate_min_weight', t(
        '@minimum_title (@minimum_value) must be lower than @maximum_title (@maximum_value)', array(
          '@minimum_title' => $form['shipping_rate_min_weight'][LANGUAGE_NONE]['#title'],
          '@minimum_value' => $form_state['values']['shipping_rate_min_weight'][LANGUAGE_NONE][0]['weight'] . ' ' . $form_state['values']['shipping_rate_min_weight'][LANGUAGE_NONE][0]['unit'],
          '@maximum_title' => $form['shipping_rate_max_weight'][LANGUAGE_NONE]['#title'],
          '@maximum_value' => $form_state['values']['shipping_rate_max_weight'][LANGUAGE_NONE][0]['weight'] . ' ' . $form_state['values']['shipping_rate_max_weight'][LANGUAGE_NONE][0]['unit'],
        )
      )
    );
  }

  // Notify field widgets to validate their data.
  field_attach_form_validate('commerce_shipping_rate', $form_state['commerce_shipping_rate'], $form, $form_state);
}

/**
 * Form submission callback.
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form state array.
 */
function commerce_shipping_rate_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $commerce_shipping_rate = $form_state['commerce_shipping_rate'];
  // Storing this flah in a variable because after saving the object, the flag
  // is dropped.
  $is_new = (isset($commerce_shipping_rate->is_new)) ? TRUE : FALSE;

  if (!empty($values['name']) || !isset($commerce_shipping_rate)) {
    if ($is_new) {
      $commerce_shipping_rate->changed = $commerce_shipping_rate->created = REQUEST_TIME;
    }
    else {
      $commerce_shipping_rate->changed = REQUEST_TIME;
    }

    // Notify field widgets.
    field_attach_submit('commerce_shipping_rate', $commerce_shipping_rate, $form, $form_state);

    // Redirects customer to the relative shipping service page.
    $form_state['redirect'] = 'admin/commerce/config/shipping/shipping-rates/' . strtr($commerce_shipping_rate->method, '_', '-') . '/shipping-rates-' . strtr($commerce_shipping_rate->name, '_', '-') . '/rates';

    // Saves the rate.
    if (commerce_shipping_rate_save($commerce_shipping_rate)) {
      if ($is_new) {
        drupal_set_message(t('Rate @rate_id has been created.', array('@rate_id' => $commerce_shipping_rate->rate_id)));
        watchdog('commerce_shipping_rate', 'Created rate @rate_id.', array('@rate_id' => $commerce_shipping_rate->rate_id), WATCHDOG_NOTICE);
      }
      else {
        drupal_set_message(t('Rate @rate_id has been updated.', array('@rate_id' => $commerce_shipping_rate->rate_id)));
        watchdog('commerce_shipping_rate', 'Updated rate @rate_id.', array('@rate_id' => $commerce_shipping_rate->rate_id), WATCHDOG_NOTICE);
      }
    }
    else {
      if ($is_new) {
        drupal_set_message(t('Rate @rate_id could not be created.', array('@rate_id' => $commerce_shipping_rate->rate_id)), 'error');
      }
      else {
        drupal_set_message(t('Rate @rate_id could not be updated.', array('@rate_id' => $commerce_shipping_rate->rate_id)), 'error');
      }
    }
  }
}

/**
 * Element validation callback.
 *
 * Ensures the field value is a shipping service.
 *
 * @param array $element
 *   The form element array.
 * @param array $form_state
 *   The form state array.
 */
function commerce_shipping_rate_element_validate_shipping_service($element, &$form_state) {
  if (!array_key_exists($element['#value'], commerce_shipping_services())) {
    form_error($element, t('%name must be a a valid shipping service.', array('%name' => $element['#value'])));
  }
}

/**
 * Element validation callback.
 *
 * Ensures the field value is a shipping method.
 *
 * @param array $element
 *   The form element array.
 * @param array $form_state
 *   The form state array.
 */
function commerce_shipping_rate_element_validate_shipping_method($element, &$form_state) {
  if (!array_key_exists($element['#value'], commerce_shipping_methods())) {
    form_error($element, t('%name must be a a valid shipping method.', array('%name' => $element['#value'])));
  }
}

/**
 * Form callback: Delete a rate.
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form state array.
 * @param object $commerce_shipping_rate
 *   A fully loaded commerce_shipping_rate object.
 */
function commerce_shipping_rate_delete_rate_form($form, &$form_state, $commerce_shipping_rate) {
  $form_state['commerce_shipping_rate'] = $commerce_shipping_rate;

  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files']['form'] = drupal_get_path('module', 'commerce_shipping_rate') . '/includes/commerce_shipping_rate.forms.inc';

  $form['#submit'][] = 'commerce_shipping_rate_delete_rate_form_submit';

  $form = confirm_form(
    $form,
    t('Are you sure you want to delete rate @rate_id ?', array('@rate_id' => $commerce_shipping_rate->rate_id)),
    'admin/commerce/config/shipping/shipping-rates/' . strtr($commerce_shipping_rate->method, '_', '-') . '/shipping-rates-' . strtr($commerce_shipping_rate->name, '_', '-') . '/rates',
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
function commerce_shipping_delete_rate_form_submit($form, &$form_state) {
  $commerce_shipping_rate = $form_state['commerce_shipping_rate'];

  // Redirects customer to the relative shipping shipping service page.
  $form_state['redirect'] = 'admin/commerce/config/shipping/shipping-rates/' . strtr($commerce_shipping_rate->method, '_', '-') . '/shipping-rates-' . strtr($commerce_shipping_rate->name, '_', '-') . '/rates';

  if (commerce_shipping_rate_delete($commerce_shipping_rate->rate_id)) {
    drupal_set_message(t('Rate @rate_id has been deleted.', array('@rate_id' => $commerce_shipping_rate->rate_id)));
    watchdog('commerce_shipping_rate', 'Deleted rate @rate_id.', array('@rate_id' => $commerce_shipping_rate->rate_id), WATCHDOG_NOTICE);
  }
  else {
    drupal_set_message(t('Rate @rate_id could not be deleted.', array('@rate_id' => $commerce_shipping_rate->rate_id)), 'error');
  }
}
