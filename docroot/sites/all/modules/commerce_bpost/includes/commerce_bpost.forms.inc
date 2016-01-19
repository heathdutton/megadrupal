<?php

/**
 * @file
 * Admin form callbacks.
 */

/**
 * Form callback: Add/Edit a new bpost rate.
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form state array.
 * @param object $commerce_bpost_rate
 *   A fully qualified bpost rate array.
 *
 * @return array
 *   The form array.
 */
function commerce_bpost_rate_form($form, &$form_state, $commerce_bpost_rate) {
  $commerce_bpost_settings = variable_get('commerce_bpost_settings', array());
  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files']['form'] = drupal_get_path('module', 'commerce_bpost') . '/includes/commerce_bpost.forms.inc';

  // Attach assets.
  $form['#attached']['js'] = array(
    drupal_get_path('module', 'commerce_bpost') . '/theme/js/commerce_bpost_range.js',
  );
  $form['#attached']['library'] = array(
    array('system', 'ui.slider'),
  );
  $form['#attached']['css'] = array(
    drupal_get_path('module', 'commerce_bpost') . '/theme/css/commerce_bpost_add_rate.css',
  );

  // Store the bpost rate in the form state.
  $form_state['commerce_bpost_rate'] = $commerce_bpost_rate;
  field_attach_form('commerce_bpost_rate', $commerce_bpost_rate, $form, $form_state);

  // User is not allowed to change the value.
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Service name'),
    '#default_value' => $commerce_bpost_rate->name,
    '#required' => TRUE,
    '#size' => 32,
    '#maxlength' => 32,
    '#access' => FALSE,
  );

  // Adds a container used as a range slider.
  $form['container'] = array(
    '#type' => 'container',
    '#id' => 'bpost-range-slider',
    '#weight' => 4,
  );

  $form['rates'] = array(
    '#type' => 'fieldset',
    '#title' => t('Rates per country'),
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('Set a price for this weight and for each country.'),
    '#weight' => 5,
  );
  if (!empty($commerce_bpost_settings['shipping_services']['country_list'])) {
    if (!empty($commerce_bpost_rate->rates)) {
      $rates = unserialize($commerce_bpost_rate->rates);
    }
    else {
      $rates = array();
    }
    if (($commerce_bpost_rate->name != "bpost_world")) {
      if (in_array('BE', $commerce_bpost_settings['shipping_services']['country_list'])) {
        $form['rates']['BE'] = array(
          '#type' => 'textfield',
          '#title' => t('Belgium'),
          '#default_value' => (isset($rates['BE'])) ? $rates['BE'] : '',
          '#size' => 10,
          '#maxlength' => 128,
          '#field_suffix' => t('EUR')
        );
      }
      else {
        $form['rates']['no_belgium'] = array(
          '#markup' => t('This shipping service is only available for Belgium and this country is not added to the country list:') . ' ' . l(t('here'), 'admin/commerce/config/commerce_bpost/shipping_services')
        );
      }
    }
    else {
      include_once DRUPAL_ROOT . '/includes/locale.inc';
      $country_get_list = country_get_list();
      foreach ($commerce_bpost_settings['shipping_services']['country_list'] as $country_code) {
        $form['rates'][$country_code] = array(
          '#type' => 'textfield',
          '#title' => check_plain($country_get_list[$country_code]),
          '#default_value' => (isset($rates[$country_code])) ? $rates[$country_code] : '',
          '#size' => 10,
          '#maxlength' => 128,
          '#field_suffix' => t('EUR')
        );
      }
    }
    if ($commerce_bpost_rate->name == "bpost_world" && isset($commerce_bpost_settings['shipping_services']['country_list']['BE'])) {
      unset($form['rates']['BE']);
    }
  }

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
function commerce_bpost_rate_form_validate($form, &$form_state) {
  $min = $form_state['values']['bpost_min_weight'][LANGUAGE_NONE][0]['weight'];
  $max = $form_state['values']['bpost_max_weight'][LANGUAGE_NONE][0]['weight'];

  $query = db_select('commerce_bpost_rate', 'rate');
  $query->condition('name', $form_state['values']['name'], '=');

  if (!empty($form_state['commerce_bpost_rate'])) {
    $query->condition('rate_id', $form_state['commerce_bpost_rate']->rate_id, '<>');
  }

  $query->join('field_data_bpost_min_weight', 'rate_min', 'rate_min.entity_id = rate.rate_id');
  $query->join('field_data_bpost_max_weight', 'rate_max', 'rate_max.entity_id = rate.rate_id');

  $db_and = db_and();
  $db_and->condition('rate_min.bpost_min_weight_weight', $max, '<');
  $db_and->condition('rate_max.bpost_max_weight_weight', $min, '>');

  $query->condition($db_and);

  // Return distinct rate.
  $query->distinct();

  // Return specific field: here rate_id.
  $query->fields('rate', array('rate_id'));

  $result = $query->execute();
  if ($result->rowCount()) {
    form_set_error('bpost_min_weight', t('Your min and max values intersect another interval. Please retry.'));
  }

  // Ensure minimum weight is lower than maximum weight.
  if ($form_state['values']['bpost_min_weight'][LANGUAGE_NONE][0]['weight'] >= $form_state['values']['bpost_max_weight'][LANGUAGE_NONE][0]['weight']) {
    form_set_error(
      'bpost_min_weight', t(
        '@minimum_title (@minimum_value) must be lower than @maximum_title (@maximum_value)', array(
          '@minimum_title' => $form['bpost_min_weight'][LANGUAGE_NONE]['#title'],
          '@minimum_value' => $form_state['values']['bpost_min_weight'][LANGUAGE_NONE][0]['weight'] . ' ' . $form_state['values']['bpost_min_weight'][LANGUAGE_NONE][0]['unit'],
          '@maximum_title' => $form['bpost_max_weight'][LANGUAGE_NONE]['#title'],
          '@maximum_value' => $form_state['values']['bpost_max_weight'][LANGUAGE_NONE][0]['weight'] . ' ' . $form_state['values']['bpost_max_weight'][LANGUAGE_NONE][0]['unit'],
        )
      )
    );
  }

  // Notify field widgets to validate their data.
  field_attach_form_validate('commerce_bpost_rate', $form_state['commerce_bpost_rate'], $form, $form_state);
}

/**
 * Submit callback for commerce_bpost_rate_form().
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form state array.
 */
function commerce_bpost_rate_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $commerce_bpost_rate = $form_state['commerce_bpost_rate'];
  // Storing this flah in a variable because after saving the object, the flag
  // is dropped.
  $is_new = (isset($commerce_bpost_rate->is_new)) ? TRUE : FALSE;

  if (!empty($values['name']) || !isset($commerce_bpost_rate)) {
    if ($is_new) {
      $commerce_bpost_rate->changed = $commerce_bpost_rate->created = REQUEST_TIME;
    }
    else {
      $commerce_bpost_rate->changed = REQUEST_TIME;
    }

    // Notify field widgets.
    field_attach_submit('commerce_bpost_rate', $commerce_bpost_rate, $form, $form_state);
    if (!empty($form_state['values']['rates'])) {
      $commerce_bpost_rate->rates = $form_state['values']['rates'];
    }

    // Redirects customer to the relative bpost shipping service page.
    $form_state['redirect'] = 'admin/commerce/config/shipping/services/bpost-' . strtr($commerce_bpost_rate->name, '_', '-') . '/rates';

    // Saves the rate.
    if (commerce_bpost_rate_save($commerce_bpost_rate)) {
      if ($is_new) {
        drupal_set_message(t('Rate @rate_id has been created.', array('@rate_id' => $commerce_bpost_rate->rate_id)));
        watchdog('commerce_bpost', 'Created rate @rate_id.', array('@rate_id' => $commerce_bpost_rate->rate_id), WATCHDOG_NOTICE);
      }
      else {
        drupal_set_message(t('Rate @rate_id has been updated.', array('@rate_id' => $commerce_bpost_rate->rate_id)));
        watchdog('commerce_bpost', 'Updated rate @rate_id.', array('@rate_id' => $commerce_bpost_rate->rate_id), WATCHDOG_NOTICE);
      }
    }
    else {
      if ($is_new) {
        drupal_set_message(t('Rate @rate_id could not be created.', array('@rate_id' => $commerce_bpost_rate->rate_id)), 'error');
      }
      else {
        drupal_set_message(t('Rate @rate_id could not be updated.', array('@rate_id' => $commerce_bpost_rate->rate_id)), 'error');
      }
    }
  }
}

/**
 * Form callback: Delete a bpost rate.
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form state array.
 * @param object $commerce_bpost_rate
 *   A fully loaded commerce_bpost_rate object.
 */
function commerce_bpost_delete_rate_form($form, &$form_state, $commerce_bpost_rate) {
  $form_state['commerce_bpost_rate'] = $commerce_bpost_rate;

  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files']['form'] = drupal_get_path('module', 'commerce_bpost') . '/includes/commerce_bpost.forms.inc';

  $form['#submit'][] = 'commerce_bpost_delete_rate_form_submit';

  $form = confirm_form(
    $form,
    t('Are you sure you want to delete rate @rate_id ?', array('@rate_id' => $commerce_bpost_rate->rate_id)),
    'admin/commerce/config/shipping/services/bpost-' . strtr($commerce_bpost_rate->name, '_', '-') . '/rates',
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
function commerce_bpost_delete_rate_form_submit($form, &$form_state) {
  $commerce_bpost_rate = $form_state['commerce_bpost_rate'];

  // Redirects customer to the relative bpost shipping service page.
  $form_state['redirect'] = 'admin/commerce/config/shipping/services/bpost-' . strtr($commerce_bpost_rate->name, '_', '-') . '/rates';

  if (commerce_bpost_rate_delete($commerce_bpost_rate->rate_id) !== FALSE) {
    drupal_set_message(t('Rate @rate_id has been deleted.', array('@rate_id' => $commerce_bpost_rate->rate_id)));
    watchdog('commerce_bpost', 'Deleted rate @rate_id.', array('@rate_id' => $commerce_bpost_rate->rate_id), WATCHDOG_NOTICE);
  }
  else {
    drupal_set_message(t('Rate @rate_id could not be deleted.', array('@rate_id' => $commerce_bpost_rate->rate_id)), 'error');
  }
}
