<?php

/**
 * @file
 * All relatives commerce methods for commerce_bpost module.
 */
define('COMMERCE_BPOST_TYPE_POSTOFFICE', 1);
define('COMMERCE_BPOST_TYPE_POSTPOINT', 2);
define('COMMERCE_BPOST_TYPE_POSTOFFICE_POSTPOINT', 3);
define('COMMERCE_BPOST_TYPE_BPACK', 4);

/**
 * Implements hook_commerce_shipping_method_info().
 */
function commerce_bpost_commerce_shipping_method_info() {
  $shipping_methods = array();
  $shipping_methods['bpost'] = array(
    'title' => t('bpost'),
    'display_title' => t('bpost'),
    'description' => t('Defines a shipping method that handles Belgium shipping services.'),
  );
  return $shipping_methods;
}

/**
 * Implements hook_commerce_shipping_service_info().
 */
function commerce_bpost_commerce_shipping_service_info() {
  $commerce_bpost_settings = variable_get('commerce_bpost_settings', array());
  if (isset($commerce_bpost_settings['options']['international_delivery'])) {
    switch ($commerce_bpost_settings['options']['international_delivery']) {
      case 'bpack World Express Pro':
        $world_name = 'BPACK World Express';
        break;

      default:
        $world_name = 'BPACK World Business';
        break;
    }
  }
  else {
    $world_name = 'BPACK World Business';
  }
  $shipping_services['bpost_home'] = array(
    'title' => t('Home delivery'),
    'description' => t('Your parcel is delivered to the address of your choice (home or work).'),
    'display_title' => t('Home delivery'),
    'shipping_method' => 'bpost',
    'price_component' => 'shipping',
    'callbacks' => array(
      'rate' => 'commerce_bpost_belgian_service_rate',
      'details_form' => 'commerce_bpost_service_details_form',
      'details_form_validate' => 'commerce_bpost_service_details_form_validate',
      'details_form_submit' => 'commerce_bpost_service_details_form_submit',
    ),
  );

  $shipping_services['bpost_postoffice_postpoint'] = array(
    'title' => t('Pick-up point'),
    'description' => t('Your parcel is delivered to a post office or a Post Point. You can choose from more than 1.250 bpost pick-up points.'),
    'display_title' => t('Pick-up point'),
    'shipping_method' => 'bpost',
    'price_component' => 'shipping',
    'callbacks' => array(
      'rate' => 'commerce_bpost_belgian_service_rate',
      'details_form' => 'commerce_bpost_service_details_map_form',
      'details_form_validate' => 'commerce_bpost_service_details_map_postpoint_form_validate',
      'details_form_submit' => 'commerce_bpost_service_details_map_form_submit',
    ),
  );

  $shipping_services['bpost_bpack'] = array(
    'title' => t('Parcel locker'),
    'description' => t('Your parcel is delivered to a bpost parcel locker. These lockers are open 24/7.'),
    'display_title' => t('Parcel locker'),
    'shipping_method' => 'bpost',
    'price_component' => 'shipping',
    'callbacks' => array(
      'rate' => 'commerce_bpost_belgian_service_rate',
      'details_form' => 'commerce_bpost_service_details_map_bpack_form',
      'details_form_validate' => 'commerce_bpost_service_details_map_bpack_form_validate',
      'details_form_submit' => 'commerce_bpost_service_details_map_form_submit',
    ),
  );

  $shipping_services[COMMERCE_BPOST_SERVICE_WORLD] = array(
    'title' => t('World'),
    'description' => t('Delivery at your invoice address.'),
    'display_title' => t('@name', array('@name' => $world_name)),
    'shipping_method' => 'bpost',
    'price_component' => 'shipping',
    'callbacks' => array(
      'rate' => 'commerce_bpost_world_service_rate',
      'details_form' => 'commerce_bpost_service_details_form',
      'details_form_validate' => 'commerce_bpost_service_details_form_validate',
      'details_form_submit' => 'commerce_bpost_service_details_form_submit',
    ),
  );

  return $shipping_services;
}

/**
 * Shipping service rate callback.
 *
 * @param $shipping_service
 * @param $order
 * @return array|bool
 */
function commerce_bpost_default_service_rate($shipping_service, $order) {
  // Get customer shipping address country.
  $wrapper_order = entity_metadata_wrapper('commerce_order', $order);
  $country_code = $wrapper_order->commerce_customer_shipping->commerce_customer_address->country->value();

  // Check shipping is enabled for this country.
  // Service is not available if country is disabled.
  $bpost_settings = variable_get('commerce_bpost_settings', array());
  if (isset($bpost_settings['shipping_services']) && !isset($bpost_settings['shipping_services']['country_list'][$country_code])) {
    return FALSE;
  }

  // Get order's weight.
  $weight = commerce_physical_order_weight($order, 'kg');

  // Load rate according to the shipping service and order's weight.
  $rate = commerce_bpost_get_rate_by_weight($shipping_service['name'], $weight);
  $wrapper_rate = entity_metadata_wrapper('commerce_bpost_rate', $rate);
  $rates = $wrapper_rate->rates->value();

  // Return country specific rate's price.
  return isset($rates[$country_code]) ? array('amount' => $rates[$country_code] * 100, 'currency_code' => 'EUR') : FALSE;
}

/**
 * Shipping service rate callback for non Belgium.
 *
 * @param $shipping_service
 * @param $order
 * @return array|bool
 */
function commerce_bpost_belgian_service_rate($shipping_service, $order) {
  if (variable_get('commerce_bpost_credentials', FALSE)) {
    // Shipping services are only available for Belgium.
    $wrapper_order = entity_metadata_wrapper('commerce_order', $order);
    $country_code = $wrapper_order->commerce_customer_shipping->commerce_customer_address->country->value();
    if ($country_code == 'BE') {
      return commerce_bpost_default_service_rate($shipping_service, $order);
    }
  }

  return FALSE;
}

/**
 * Shipping service rate callback for Belgium.
 *
 * @param $shipping_service
 * @param $order
 * @return array|bool
 */
function commerce_bpost_world_service_rate($shipping_service, $order) {
  if (variable_get('commerce_bpost_credentials', FALSE)) {
    // Shipping services are only available outside Belgium.
    $wrapper_order = entity_metadata_wrapper('commerce_order', $order);
    $country_code = $wrapper_order->commerce_customer_shipping->commerce_customer_address->country->value();
    if ($country_code != 'BE') {
      return commerce_bpost_default_service_rate($shipping_service, $order);
    }
  }

  return FALSE;
}

/**
 * Shipping service basic form callback.
 *
 * Checkout pane callback: customer profile edit form.
 * Ensure we have address number and name in separate fields.
 * Either try to automatically split those values or ask user to correct his/her
 * shipping address.
 */
function commerce_bpost_service_details_form($pane_form, &$pane_values, $checkout_pane, $order, $shipping_service) {
  // Address.
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  $address = $order_wrapper->commerce_customer_shipping->commerce_customer_address->value();

  $form = array();
  $modification_required = 0;
  if (empty($address['name_line']) or
      empty($address['thoroughfare']) or
      empty($address['postal_code']) or
      empty($address['locality']) or
      empty($address['country'])) {
    // We are missing at least one field, ask user to complete his/her
    // shipping address.
    $modification_required = 1;
  }

  if ($modification_required) {
    // Get current shipping profile.
    $profile = NULL;
    if (isset($order_wrapper->commerce_customer_shipping)) {
      $profile = $order_wrapper->commerce_customer_shipping->value();
    }
    // Create a new shipping profile if it hasn't already been made.
    if (empty($profile)) {
      $profile = commerce_customer_profile_new('shipping', $order->uid);
    }

    // Add the entity context of the current cart order.
    $profile->entity_context = array(
      'entity_type' => 'commerce_order',
      'entity_id' => $order->order_id,
    );

    $form['customer_profile'] = array(
      '#type' => 'value',
      '#value' => $profile,
    );

    // Build form_state since callback call doesn't give us the information.
    $form_state = form_state_defaults();
    $form_state['build_info']['form_id'] = 'commerce_bpost_service_details_form';
    $form_state['values'] = $pane_values;
    $form_state['input'] = $pane_values;

    // Add the field widgets for the profile.
    field_attach_form('commerce_customer_profile', $profile, $form, $form_state);
    unset($form['#parents']);
    $form['commerce_customer_address'][LANGUAGE_NONE][0]['street_block']['thoroughfare']['#title'] = t('Street name');
    $form['commerce_customer_address'][LANGUAGE_NONE][0]['street_block']['premise']['#title'] = t('Street number');
    $form['commerce_customer_address'][LANGUAGE_NONE][0]['locality_block']['postal_code']['#required'] = TRUE;
  }

  if ($shipping_service['name'] === COMMERCE_BPOST_SERVICE_WORLD && empty($address['phone_number'])) {
    // Display the input field 'phone number' if the shipping phone has not been filled in
    $form['world_extras'] = array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array('bpost-shipping-services'),
        'id' => array('delivery-parcel-automate'),
      ),
    );
    $form['world_extras']['description'] = array(
      '#markup' => t('Additional informations are required.'),
      '#prefix' => '<p>',
      '#suffix' => '</p>',
    );
    $form['world_extras']['phone'] = array(
      '#type' => 'textfield',
      '#title' => t('Contact phone'),
      '#required' => TRUE,
      '#size' => 20,
    );
  }
  // Do not display the delivery date for @world
  if ($shipping_service['name'] !== COMMERCE_BPOST_SERVICE_WORLD) {
    commerce_bpost_add_delivery_date_container($form, $shipping_service['name'], $order);
  }
  return $form;
}

/**
 * Shipping service basic form validate.
 *
 * Checkout pane callback: validate customer profile.
 */
function commerce_bpost_service_details_form_validate($details_form, $details_values, $shipping_service, $order, $form_parents) {
  if (isset($details_values['customer_profile'])) {
    $profile = $details_values['customer_profile'];
    $form_state = array('values' => $details_values);
    // Notify field widgets to validate their data.
    field_attach_form_validate('commerce_customer_profile', $profile, $details_form, $form_state);
  }
}

/**
 * Shipping service basic form submit.
 *
 * Checkout pane callback: save customer profile.
 */
function commerce_bpost_service_details_form_submit($details_form, $details_values, $line_item) {
  $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
  $order_wrapper = &$line_item_wrapper->order;

  // Store the contact phone into the order shipping profile for @world
  if (!empty($details_values['world_extras']['phone'])) {
    $contact_phone = $details_values['world_extras']['phone'];
    $commerce_customer_profile_wrapper = $order_wrapper->commerce_customer_shipping;
    $commerce_customer_profile_wrapper->commerce_customer_address->phone_number = $contact_phone;
    $profile = $commerce_customer_profile_wrapper->value();
    commerce_customer_profile_save($profile);
    $order_wrapper->commerce_customer_shipping = $profile;
  }

  if (isset($details_values['customer_profile'])) {
    _commerce_bpost_service_customer_profile_save($details_form, $details_values, $order_wrapper);
  }

  $commerce_bpost_settings = variable_get('commerce_bpost_settings', array());
  $is_delivery_date_displayed = !empty($commerce_bpost_settings['options']['delivery_date']['display']);
  if ($is_delivery_date_displayed) {
    // Delivery date is displayed
    commerce_bpost_delivery_date_submit($details_form, $details_values, $line_item);
  }
}

/**
 * Save customer profile.
 *
 * @param array $details_form
 *   Form details.
 * @param array $details_values
 *   Form values to save.
 * @param EntityMetadataWrapper $order_wrapper
 *   Order entity wrapper.
 */
function _commerce_bpost_service_customer_profile_save($details_form, $details_values, $order_wrapper) {
  // Ensure the profile is active.
  $profile = $details_values['customer_profile'];
  $profile->commerce_customer_address = $details_values['commerce_customer_address'];
  $profile->status = TRUE;

  // Set the profile's uid if it's being created at this time.
  if (empty($profile->profile_id)) {
    $profile->uid = $order_wrapper->uid->value();
  }

  // Notify field widgets.
  $form_state = array('values' => $details_values);
  field_attach_submit('commerce_customer_profile', $profile, $details_form, $form_state);

  // Save the profile.
  commerce_customer_profile_save($profile);

  // Store the profile ID for the related field as specified on the settings
  // form.
  $order_wrapper->commerce_customer_shipping = $profile;
}

/**
 * Shipping service bpack form callback.
 *
 * Checkout pane callback: bpack point selection.
 */
function commerce_bpost_service_details_map_bpack_form($pane_form, $pane_values, $checkout_pane, $order, $shipping_service) {

  $service_values = array();
  if (!empty($pane_values['commerce_bpost'])) {
    $service_values = $pane_values['commerce_bpost'][$pane_values['shipping_service']];
  }
  // Build the post point selection with map form.
  $form = _commerce_bpost_service_details_build_map_form($service_values, $pane_form, $pane_values, $checkout_pane, $order, $shipping_service);
  if (!isset($form['error'])) {
    $form['reduced_mobility'] = array(
      '#type' => 'checkbox',
      '#title' => t('I want to use the reduced mobility zone of the parcel locker.'),
      '#description' => '<div class="element-hidden description-popin">' . t('This zone allows people with reduced mobility to access the most convenient lockers of the parcel locker.') . '</div>',
    );
    if (!empty($service_values['reduced_mobility'])) {
      $form['reduced_mobility']['#default_value'] = $service_values['reduced_mobility'];
    }
    elseif (!empty($order->data['commerce_bpost']['reduced_mobility'])) {
      $form['reduced_mobility']['#default_value'] = $order->data['commerce_bpost']['reduced_mobility'];
    }

    // Notification language.
    $form['notification_language'] = array(
      '#type' => 'select',
      '#title' => t('Notification language'),
      '#options' => array(
        'EN' => 'EN',
        'FR' => 'FR',
        'NL' => 'NL',
      ),
      '#required' => TRUE,
    );
    if (!empty($service_values['notification_language'])) {
      $form['notification_language']['#default_value'] = $service_values['notification_language'];
    }
    elseif (!empty($order->data['commerce_bpost']['reduced_mobility'])) {
      $form['notification_language']['#default_value'] = $order->data['commerce_bpost']['notification_language'];
    }
    else {
      $bpost_current_language = commerce_bpost_get_current_language();
      $form['notification_language']['#default_value'] =  $bpost_current_language == 'DE' ? 'EN' : $bpost_current_language;
    }

    $form['notification_message_phone'] = array(
      '#title' => t('Phone Number'),
      '#type' => 'textfield',
      '#size' => 15,
      '#description' => t('Receive an additional notification via sms next to the standard email notification.'),
    );
    // Get the default phone number if it was edited.
    if (!empty($service_values['notification_message_phone'])) {
      $form['notification_message_phone']['#default_value'] = $service_values['notification_message_phone'];
    }
    elseif (!empty($order->data['commerce_bpost']['notification_message_phone'])) {
      $form['notification_message_phone']['#default_value'] = $order->data['commerce_bpost']['notification_message_phone'];
    }
    else {
      $commerce_bpost_settings = variable_get('commerce_bpost_settings', array());
      $wrapper_order = entity_metadata_wrapper('commerce_order', $order);
      $customer_address['phoneNumber'] = substr(commerce_bpost_get_field_value('phoneNumber', $wrapper_order->commerce_customer_shipping, $commerce_bpost_settings['mapping']['profile']['fields'], 'commerce_customer_profile'), 0, 20);
      $form['notification_message_phone']['#default_value'] = !empty($customer_address['phoneNumber']) ? $customer_address['phoneNumber'] : '';
    }
    // Scroll down to shipping service details.
    $form['#attached']['js'][] = array('type' => 'file', 'data' => drupal_get_path('module', 'commerce_bpost') . '/theme/js/commerce_bpost_goto.js');
  }
  return $form;
}

/**
 * Shipping service bpack form validate.
 *
 * Checkout pane callback: validate bpack point is selected.
 */
function commerce_bpost_service_details_map_bpack_form_validate($details_form, $details_values, $shipping_service, $order, $form_parents) {
  if (isset($details_values['customer_profile'])) {
    commerce_bpost_service_details_form_validate($details_form, $details_values, $shipping_service, $order, $form_parents);
    if (!form_get_errors()) {
      $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
      _commerce_bpost_service_customer_profile_save($details_form, $details_values, $order_wrapper);
    }
    return FALSE;
  }
  else {
    $return = TRUE;
    if (!empty($details_values['notification_message_phone']) && !_commerce_bpost_valid_phone_number($details_values['notification_message_phone'])
    ) {
      form_set_error('commerce_shipping][service_details][notification_message_phone', t('Please enter a valid phone number'));
      $return = FALSE;
    }

    if (empty($details_values['bpack_map']['points']['points_list']) || (!empty($details_values['bpack_map']['points']['points_list'])
        && !preg_match('/^[0-9]+\|[0-9]+$/', $details_values['bpack_map']['points']['points_list']))) {
      form_set_error('commerce_shipping][service_details][bpack_map][points][points_list', t('Please select a @label', array('@label' => $details_values['label'])));
      $return = FALSE;
    }

    return $return;
  }
}

/**
 * Shipping service postpoint form callback.
 *
 * Checkout pane callback: postpoint selection.
 */
function commerce_bpost_service_details_map_form($pane_form, $pane_values, $checkout_pane, $order, $shipping_service) {
  $service_values = array();
  if (!empty($pane_values['commerce_bpost'])) {
    $service_values = $pane_values['commerce_bpost'][$pane_values['shipping_service']];
  }
  // Build the post point selection with map form.
  $form = _commerce_bpost_service_details_build_map_form($service_values, $pane_form, $pane_values, $checkout_pane, $order, $shipping_service);
  if (!isset($form['error'])) {
    $form['notification_message'] = array(
      '#type' => 'radios',
      '#title' => t('Keep me informed via'),
      '#options' => array(
        'phone' => t('Sms'),
        'mail' => t('Email'),
      ),
      '#required' => TRUE,
      '#default_value' => 'phone',
      '#validated' => TRUE,
    );
    if (!empty($service_values['notification_message'])) {
      $form['notification_message']['#default_value'] = $service_values['notification_message'];
    }
    elseif (!empty($order->data['commerce_bpost']['notification_message'])) {
      $form['notification_message']['#default_value'] = $order->data['commerce_bpost']['notification_message'];
    }
    $form['notification_message_phone'] = array(
      '#title' => t('Phone Number'),
      '#type' => 'textfield',
      '#states' => array(
        'visible' => array(':input[name="commerce_shipping[service_details][notification_message]"]' => array(
            'value' => 'phone')),
        'required' => array(':input[name="commerce_shipping[service_details][notification_message]"]' => array(
            'value' => 'phone')),
      ),
    );
    // Get the default phone number if it was edited.
    if (!empty($service_values['notification_message_phone'])) {
      $form['notification_message_phone']['#default_value'] = $service_values['notification_message_phone'];
    }
    elseif (!empty($order->data['commerce_bpost']['notification_message_phone'])) {
      $form['notification_message_phone']['#default_value'] = $order->data['commerce_bpost']['notification_message_phone'];
    }
    else {
      $commerce_bpost_settings = variable_get('commerce_bpost_settings', array());
      $wrapper_order = entity_metadata_wrapper('commerce_order', $order);
      $customer_address['phoneNumber'] = substr(commerce_bpost_get_field_value('phoneNumber', $wrapper_order->commerce_customer_shipping, $commerce_bpost_settings['mapping']['profile']['fields'], 'commerce_customer_profile'), 0, 20);
      $form['notification_message_phone']['#default_value'] = !empty($customer_address['phoneNumber']) ? $customer_address['phoneNumber'] : '';
    }
    // Scroll down to shipping service details.
    $form['#attached']['js'][] = array('type' => 'file', 'data' => drupal_get_path('module', 'commerce_bpost') . '/theme/js/commerce_bpost_goto.js');
  }
  return $form;
}

/**
 * Bpost point selection with map.
 * Helper function for both Pick-up Point and Parcel locker shipping types.
 */
function _commerce_bpost_service_details_build_map_form($service_values, $pane_form, &$pane_values, $checkout_pane, $order, $shipping_service) {
  $commerce_bpost_settings = variable_get('commerce_bpost_settings', array());
  if (commerce_bpost_check_minimal_config_requirement($commerce_bpost_settings)) {
    // Address.
    $wrapper_order = entity_metadata_wrapper('commerce_order', $order);
    $commerce_customer_address = $wrapper_order->commerce_customer_shipping->commerce_customer_address;
    $customer_address['thoroughfare'] = $commerce_customer_address->thoroughfare->value();
    $customer_address['premise'] = $commerce_customer_address->premise->value();
    $customer_address['postal_code'] = $commerce_customer_address->postal_code->value();
    $customer_address['city'] = $commerce_customer_address->locality->value();
    $customer_address['country'] = $commerce_customer_address->country->value();

    // Customer address.
    if (!empty($service_values['bpack_map']['filters']['postal_code']) ||
      !empty($service_values['bpack_map']['filters']['city']) ||
      !empty($service_values['bpack_map']['filters']['country'])
    ) {
      unset($service_values['bpack_map']['filters']['submit']);
      $customer_address = $service_values['bpack_map']['filters'];
    }

    $points = array();
    $saved_values = NULL;
    if ($shipping_service['name'] == 'bpost_postoffice_postpoint') {
      $bpost_type = COMMERCE_BPOST_TYPE_POSTOFFICE_POSTPOINT;
      $form = array(
        '#type' => 'container',
        '#attributes' => array(
          'class' => array('bpost-shipping-services'),
          'id' => array('pickup-postoffice-postpoint'),
        ),
      );
      $fieldset_title = t('Select a Postpoint');
      $label = t('PostOffice or PostPoint');
    }
    elseif ($shipping_service['name'] == 'bpost_bpack') {
      $bpost_type = COMMERCE_BPOST_TYPE_BPACK;
      $form = array(
        '#type' => 'container',
        '#attributes' => array(
          'class' => array('bpost-shipping-services'),
          'id' => array('parcel-locker'),
        ),
      );
      $label = t('Parcel locker');
      $fieldset_title = t('Select a parcel locker');
    }
    if (!empty($bpost_type) && !empty($order->data['commerce_bpost']) && !empty($order->data['commerce_bpost'][$shipping_service['name']])) {
      $saved_values = $order->data['commerce_bpost'][$shipping_service['name']];
    }

    // Delivery date selection.
    commerce_bpost_add_delivery_date_container($form, $shipping_service['name'], $order);

    // Build the map and selection form.
    $form['bpack_map'] = array(
      '#type' => 'fieldset',
      '#title' => $fieldset_title,
      '#weight' => 0,
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#attributes' => array('id' => array('bpack-shipping-map')),
      '#prefix' => '<div id="bpack_map_wrapper">',
      '#suffix' => '</div>',
    );

    $delivery_ts = isset($service_values['delivery_date']['date']) ? $service_values['delivery_date']['date'] : !empty($form['delivery_date']['date']['#default_value']) ? $form['delivery_date']['date']['#default_value'] : NULL;
    list($map_markers, $point_options, $point_favorite) = _commerce_bpost_get_points($customer_address, $bpost_type, $points, $delivery_ts);

    // Points lists.
    $form['bpack_map']['points'] = array(
      '#type' => 'container',
      '#attributes' => array(
        'id' => array('points-wrapper'),
        'class' => array('points-list'),
      ),
    );
    $form['bpack_map']['points']['points_list'] = array(
      '#type' => 'radios',
      '#options' => $point_options,
      '#attributes' => array(
        'class' => array('points-list'),
      ),
    );
    // Default value for points list.
    if (!empty($pane_values['commerce_bpost']['bpack_map']['points']['points_list']) && array_key_exists($pane_values['commerce_bpost']['bpack_map']['points']['points_list'], $point_options)) {
      $form['bpack_map']['points']['points_list']['#default_value'] = $pane_values['commerce_bpost']['bpack_map']['points']['points_list'];
    }
    elseif (!empty($saved_values) && array_key_exists($saved_values['bpack_map']['points']['points_list'], $point_options)) {
      $form['bpack_map']['points']['points_list']['#default_value'] = $saved_values['bpack_map']['points']['points_list'];
    }

    if (empty($map_markers)) {
      $form['bpack_map']['nopoint'] = array(
        '#markup' => '<div class="noaddress">' . t('There is no') . ' ' . $label . ' ' . t('available for this address.') . '</div>',
      );
    }
    $form['label'] = array(
      '#type' => 'value',
      '#value' => $label,
    );
    $form['bpack_map']['filters'] = array(
      '#type' => 'container',
      '#tree' => TRUE,
      '#attributes' => array(
        'class' => array(
          'container-inline',
          'commerce-bpost-filters',
        ),
      ),
      '#prefix' => '<div class="map-container">',
    );
    $form['bpack_map']['filters']['postal_code'] = array(
      '#type' => 'textfield',
      '#title' => t('Postal code'),
      '#default_value' => $customer_address['postal_code'],
      '#size' => 10,
      '#maxlength' => 10,
      '#element_validate' => array('_commerce_bpost_pane_value_hack'),
      '#attributes' => array(
        'class' => array('field-postal-code'),
      ),
    );
    $form['bpack_map']['filters']['city'] = array(
      '#type' => 'textfield',
      '#title' => t('City'),
      '#default_value' => $customer_address['city'],
      '#size' => 15,
      '#maxlength' => 32,
      '#element_validate' => array('_commerce_bpost_pane_value_hack'),
      '#attributes' => array(
        'class' => array('field-city'),
      ),
    );
    $form['bpack_map']['filters']['country'] = array(
      '#type' => 'select',
      '#title' => t('Country'),
      '#default_value' => 'BE',
      '#options' => array('BE' => 'Belgium'),
      '#element_validate' => array('_commerce_bpost_pane_value_hack'),
    );
    // Coord of the center of the map. Filled by the JS after a D&D
    $form['bpack_map']['filters']['zone'] = array(
      '#type' => 'hidden',
      '#default_value' => isset($pane_values['commerce_bpost']['filters']['zone']) ? $pane_values['commerce_bpost']['filters']['zone'] : '',
      '#attributes' => array(
        'class' => array('field-zone'),
      ),
    );
    // Zoom level of the map
    $form['bpack_map']['filters']['zoom'] = array(
      '#type' => 'hidden',
    );
    $form['bpack_map']['filters']['submit'] = array(
      '#type' => 'button',
      '#value' => t('Search'),
      '#ajax' => array(
        'callback' => 'commerce_bpost_pane_checkout_form_details_refresh',
        'wrapper' => 'commerce-shipping-service-details',
        'method' => 'replace',
        'event' => 'click',
      ),
    );
    // Map.
    $map_address = array();
    // Geocoder needs in order : city, postal code, country.
    if (!empty($customer_address['city'])) {
      $map_address['city'] = $customer_address['city'];
    }
    if (!empty($customer_address['postal_code'])) {
      $map_address['postal_code'] = $customer_address['postal_code'];
    }
    $map_address['country'] = 'Belgium';
    $bpost_map = array(
      'address' => implode(', ', $map_address),
      'icons' => array(
        'domicile' => base_path() . drupal_get_path('module', 'commerce_bpost') . '/icons/domicile.png',
      ),
      'width' => '100%',
    );

    // Use json_encode to ensure markers array is reset javascript settings.
    $bpost_map += array('markers' => json_encode($map_markers));

    $bpost_map += _commerce_bpost_map_settings();
    $form['bpack_map']['map'] = array(
      '#type' => 'commerce_bpost_map',
      '#map' => $bpost_map,
      '#attributes' => array(
        'class' => array('commerce-bpost-map'),
      ),
      '#suffix' => '</div>', // Closing #map-container.
    );

    $form['#attached']['css'][] = drupal_get_path('module', 'commerce_bpost') . '/theme/css/commerce_bpost.css';
    drupal_add_js(array('commerce_bpost_map' => $bpost_map), array('type' => 'setting'));
  }
  else {
    $form['error'] = array(
      '#markup' => t('Commerce bpost module is not configured. Please contact the administrator.'),
    );
  }
  return $form;
}

/**
 * Returns the payment details element for display via AJAX.
 */
function commerce_bpost_pane_checkout_form_details_refresh($form, $form_state) {
  return $form['commerce_shipping']['service_details'];
}

/**
 * Shipping service pospoint form validate.
 *
 * Checkout pane callback: validate postpoint is selected.
 */
function commerce_bpost_service_details_map_postpoint_form_validate($details_form, $details_values, $shipping_service, $order, $form_parents) {
  if (isset($details_values['customer_profile'])) {
    commerce_bpost_service_details_form_validate($details_form, $details_values, $shipping_service, $order, $form_parents);
    if (!form_get_errors()) {
      $line_item_wrapper = _commerce_bpost_get_shipping_line_item($order);
      commerce_bpost_service_details_form_submit($details_form, $details_values, $line_item_wrapper->value());
      return FALSE;
    }
  }
  else {
    $return = TRUE;
    if ($details_values['notification_message'] == "phone" && !_commerce_bpost_valid_phone_number($details_values['notification_message_phone'])
    ) {
      form_set_error('notification_message_phone', t('Please enter a valid phone number'));
      $return = FALSE;
    }
    if (empty($details_values['bpack_map']['points']['points_list'])) {
      form_set_error('commerce_bpost_point_id', t('Please select a @label', array('@label' => $details_values['label'])));
      $return = FALSE;
    }
    else {
      $point = explode('|', $details_values['bpack_map']['points']['points_list']);
      $point_details = commerce_bpost_get_point_details($point[1], $point[0]);
      if (!empty($point_details)) {
        $order->data['commerce_bpost']['point_details'] = $point_details;
        commerce_order_save($order);
      }
      else {
        form_set_error('commerce_bpost_point_id', t('An error occured, please try again or contact the administrator.'));
        $return = FALSE;
      }
    }

    return $return;
  }
}

/**
 * Shipping service postpoint & bpack form submit.
 *
 * Checkout pane callback: save post or bpack delivery point
 *   in the shipping line item.
 */
function commerce_bpost_service_details_map_form_submit($details_form, $details_values, $line_item) {
  // Save the point address into the line_item.
  $point = explode('|', $details_values['bpack_map']['points']['points_list']);
  $point_details = commerce_bpost_get_point_details($point[1], $point[0]);
  $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
  if (!empty($point_details)) {
    $line_item_wrapper->value()->data['point_details'] = $point_details;
  }
  if (isset($details_values['notification_message'])) {
    $line_item_wrapper->value()->data['notification_message'] = $details_values['notification_message'];
    $line_item_wrapper->value()->data['notification_message_phone'] = $details_values['notification_message_phone'];
    $line_item_wrapper->order->value()->data['commerce_bpost']['notification_message'] = $details_values['notification_message'];
    $line_item_wrapper->order->value()->data['commerce_bpost']['bpost_postoffice_postpoint'] = $details_values;
  }
  if (!empty($details_values['notification_message_phone'])) {
    $line_item_wrapper->value()->data['notification_message_phone'] = $details_values['notification_message_phone'];
    $line_item_wrapper->order->value()->data['commerce_bpost']['notification_message_phone'] = $details_values['notification_message_phone'];
  }
  if (!empty($details_values['reduced_mobility'])) {
    $line_item_wrapper->value()->data['reduced_mobility'] = $details_values['reduced_mobility'];
    $line_item_wrapper->order->value()->data['commerce_bpost']['reduced_mobility'] = $details_values['reduced_mobility'];
  }
  // Set ISO language depending of current session.
  if (!empty($details_values['notification_language'])) {
    $line_item_wrapper->value()->data['notification_language'] = $details_values['notification_language'];
    $line_item_wrapper->order->value()->data['commerce_bpost']['notification_language'] = $details_values['notification_language'];
  }

  $commerce_bpost_settings = variable_get('commerce_bpost_settings', array());
  $is_delivery_date_displayed = !empty($commerce_bpost_settings['options']['delivery_date']['display']);
  if ($is_delivery_date_displayed) {
    // Delivery date is displayed
    commerce_bpost_delivery_date_submit($details_form, $details_values, $line_item);
  }
}

/**
 * Implements hook_commerce_checkout_pane_info().
 */
function commerce_bpost_commerce_checkout_pane_info() {
  $checkout_panes = array();

  $checkout_panes['commerce_bpost_shipping_profile'] = array(
    'title' => t('bpost shipping'),
    'file' => 'includes/commerce_bpost.checkout_pane.inc',
    'base' => 'commerce_bpost_pane',
    'page' => 'checkout',
    'weight' => 8,
  );
  return $checkout_panes;
}

/**
 * Get postpoints hours from webservice.
 *
 * @param string $url
 *   Url of webservice.
 *
 * @return \SimpleXMLElement
 *   Result webservice call.
 */
function commerce_bpost_get_postpoint_hours($url) {
  $xml = simplexml_load_string(file_get_contents($url));
  $xml = json_decode(json_encode($xml), 1);
  return $xml;
}

/**
 * Check phone number is valid for belgium.
 *
 * Valid phone numbers are:
 * 04xxxxxxxx
 * +324xxxxxxxx
 * 4xxxxxxxx
 * 324xxxxxxxx
 * 00324xxxxxxxx
 * 
 * @param string $phone
 *   Phone number to check.
 *
 * @return bool
 *   Phone number is valid.
 */
function _commerce_bpost_valid_phone_number($phone) {
  return (empty($phone) || !preg_match('/^(((\+|00)?32)|0)?4[0-9]{8}$/', $phone)) ? FALSE : TRUE;
}

/**
 * Check date is valid.
 *
 * @param string $date
 * Birth date string at format dd/mm/yyyy.
 *
 * @return bool
 */
function _commerce_bpost_valid_birthdate($date) {
  return _commerce_bpost_valid_date($date);
}

/**
 * Check date is valid.
 *
 * @param string $date
 * Date string at format dd/mm/yyyy.
 * @param string $format
 * Format of date to valid, default d/m/Y.
 *
 * @return bool
 */
function _commerce_bpost_valid_date($date, $format = 'd/m/Y') {
  $d = DateTime::createFromFormat($format, $date);
  return $d && $d->format($format) == $date;
}

/**
 * Check birthdate is higher or equal than 18 years.
 *
 * @param $date
 * Birth date string at format dd/mm/yyyy.
 *
 * @return bool
 */
function _commerce_bpost_is_adult_birthdate($date) {
  $now = new DateTime();
  $birthdate = DateTime::createFromFormat('Y-m-d', $date);
  $interval = $birthdate->diff($now);
  return $interval->format('%y') >= 18;
}

/**
 * Returns the nearest service points of the customer shipping address and open on the $delivery date.
 * 
 * @param type $customer_address The customer shipping address.
 * @param type $type
 * @param type $points
 * @param integer $delivery_ts The delivery timestamp for which the service point must be open.
 * @return array
 */
function _commerce_bpost_get_points($customer_address, $type, $points = array(), $delivery_ts = NULL) {
  $points = array_merge($points, commerce_bpost_webservice_get_nearest_service_points($customer_address, $type, $delivery_ts));

  if ($type == COMMERCE_BPOST_TYPE_POSTOFFICE_POSTPOINT && empty($points) && isset($customer_address['postal_code'])) {
    unset($customer_address['thoroughfare']);
    unset($customer_address['premise']);
    drupal_set_message(t('Warning: No points was found with your address. The current search was made only by using your postcode. Try again if needed.'), 'warning');
    $points = commerce_bpost_webservice_get_nearest_service_points(array('postal_code' => $customer_address['postal_code']), $type, $delivery_ts);
  }

  $map_markers = array();
  $point_options = array();
  $point_favorite = NULL;
  if (!empty($points)) {
    foreach ($points as $point) {
      if (!empty($point['Id'])) {
        $point['Record'] = $point;
      }
      if (!is_array($point)|| empty($point['Record'])) {
         continue;
      }
      if ($type == COMMERCE_BPOST_TYPE_POSTOFFICE_POSTPOINT) {
        $point_details = commerce_bpost_get_postpoint_hours($point['Info']['@attributes']['ServiceRef']);
        if (!empty($point_details['Poi']['Record']['Hours'])) {
          if (!empty($delivery_ts) &&
              !commerce_bpost_is_delivery_date_an_open_hour($delivery_ts, $point_details['Poi']['Record']['Hours'])) {
            // The service point must be open on the delivery date
            continue;
          }
          $point['Record']['Hours'] = $point_details['Poi']['Record']['Hours'];
        }
      }
      if (!empty($point['Record']['Id']) && empty($map_markers[$point['Record']['Id']])) {
        $id = $point['Record']['Id'];
        $icon = base_path() . drupal_get_path('module', 'commerce_bpost') . '/icons/bpost-point.png';
      }
      elseif (!empty($point['Record']['ID'])) {
        $id = $point['Record']['ID'];
        $icon = base_path() . drupal_get_path('module', 'commerce_bpost') . '/icons/bpost-point-favorite.png';
      }
      if (!empty($point['Record']['Latitude']) && !empty($point['Record']['Longitude']) && !empty($point['Record']['Type'])) {
        $map_markers[$id] = array(
          'coordGeolocalisationLatitude' => $point['Record']['Latitude'],
          'coordGeolocalisationLongitude' => $point['Record']['Longitude'],
          'identifiant' => $id,
          'infoWindow' => theme_commerce_bpost_point(array('point' => $point)),
          'icon' => $icon,
          'type' => $point['Record']['Type'],
        );
        $point_options[$point['Record']['Type'] . '|' . $id] = theme_commerce_bpost_point(array('point' => $point,
          'schedule' => $type == COMMERCE_BPOST_TYPE_POSTOFFICE_POSTPOINT ? FALSE : TRUE));
        if (!$point_favorite && isset($point['Favorite']) && $point['Favorite']) {
          $point_favorite = $point['Record']['Type'] . '|' . $id;
        }
      }
    }
  }

  return array(
    $map_markers,
    $point_options,
    $point_favorite,
  );
}

/**
 * Returns TRUE if the delivery date is an open hour.
 * 
 * @param integer $delivery_ts Delivery timestamp
 * @param array open_hours
 * @return bool
 */
function commerce_bpost_is_delivery_date_an_open_hour($delivery_ts, array $open_hours) {
  $daynb2dayname = array(
    0 => 'Sunday',
    1 => 'Monday',
    2 => 'Tuesday',
    3 => 'Wednesday',
    4 => 'Thursday',
    5 => 'Friday',
    6 => 'Saturday',
  );
  $delivery_date_day_nb = date('w', $delivery_ts);
  $delivery_date_day_name = isset($daynb2dayname[$delivery_date_day_nb]) ? $daynb2dayname[$delivery_date_day_nb] : NULL;
  // Checks that at least one open hour is defined for the delivery day
  return !empty($open_hours[$delivery_date_day_name]['AMOpen']) || !empty($open_hours[$delivery_date_day_name]['PMOpen']);
}

/**
 * Return points to display on map based on zone.
 */
function commerce_bpost_get_geo6_points_ajax($zone, $type) {
  module_load_include('inc', 'commerce_bpost', 'includes/commerce_bpost.webservice');

  // Sanitize.
  $zone = check_plain($zone);
  $type = check_plain($type);

  if ($type == 'bpost_bpack') {
    $type = COMMERCE_BPOST_TYPE_BPACK;
  }
  else {
    $type = COMMERCE_BPOST_TYPE_POSTOFFICE_POSTPOINT;
  }

  list($map_markers, $point_options, $point_favorite) = _commerce_bpost_get_points($zone, $type);

  $options = drupal_get_form('commerce_bpost_points_list_form', $point_options);
  drupal_json_output(array(
    'markers' => $map_markers,
    'options_list' => drupal_render($options),
  ));
}

function commerce_bpost_points_list_form($form, &$form_state, $point_options) {
  $form['bpack_map']['points'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => array('points-wrapper'),
      'class' => array('points-list'),
    ),
  );
  $form['bpack_map']['points']['points_list'] = array(
    '#type' => 'radios',
    '#options' => $point_options,
    '#attributes' => array(
      'class' => array('points-list'),
    ),
  );
  return $form;
}

/**
 * Adds the delivery date contaner to the details form.
 * 
 * @param array $form
 * @param string $shipping_service_name
 * @param boolean $refuse_saturday [optional] Force the value of the form param 'Refuse saturday delivery'.
 * If not set, the value is read from the POST parameter. This parameter should be used only to
 * display the preview of the details form into the admin settings form (@see commerce_bpost_options_form()).
 */
function commerce_bpost_add_delivery_date_container(&$form, $shipping_service_name = NULL, $order = NULL, $refuse_saturday = NULL, $settings = NULL) {
  $options = filter_input(INPUT_POST, 'commerce_shipping', FILTER_DEFAULT, FILTER_REQUIRE_ARRAY);
  if (!module_load_include('inc', 'commerce_bpost', 'includes/commerce_bpost.admin')) {
    watchdog('commerce_bpost', 'Cannot load commerce_bpost.admin.inc', array(), WATCHDOG_CRITICAL);
    return FALSE;
  }
  if (!isset($settings)) {
    $settings = variable_get('commerce_bpost_settings', array());
  }
  if (!empty($settings['options'][$shipping_service_name]['saturday_delivery'])) {
    // The seller manages Saturday deliveries
    if ($refuse_saturday === NULL) {
      $refuse_saturday = isset($options['service_details']['delivery_date']['refuse_saturday']) ?
          (bool) $options['service_details']['delivery_date']['refuse_saturday'] : FALSE;
    }
  }
  else {
    $refuse_saturday = TRUE;
  }
  $display_delivery_date = !empty($settings['options']['delivery_date']['display']) ?
      (bool) $settings['options']['delivery_date']['display'] : FALSE;
  $choose_delivery_date = !empty($settings['options']['delivery_date']['choose']) ?
      (bool) $settings['options']['delivery_date']['choose'] : FALSE;

  // Store the shipping service name to be retrieved when processing the submit of the details form
  $form['shipping_service_name'] = array(
    '#type' => 'hidden',
    '#value' => $shipping_service_name,
  );

  // Always display this block to replace it by the preview of delivery date, if necessary
  $form['delivery_date'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => array('delivery-date'),
      'class' => isset($form['#type']) && $form['#type'] === 'container' ? array() : array('display-border'),
    ),
    '#weight' => -1,
  );

  if ($display_delivery_date &&
      module_load_include('inc', 'commerce_bpost', 'includes/commerce_bpost.admin')) {
    $form['delivery_date']['description'] = array(
      '#markup' => t('Delivery date'),
      '#prefix' => '<p class="title">',
      '#suffix' => '</p>',
    );

    if (!$choose_delivery_date) {
      $delivery_date = commerce_bpost_get_delivery_date($refuse_saturday, 1, $settings);
      $form['delivery_date']['date'] = array(
        '#markup' => ($delivery_date instanceof DateTime) ? $delivery_date->format('D, Y-m-d') : NULL,
        '#prefix' => '<div id="delivery-date-date-wrapper">',
        '#suffix' => '</div>',
      );
    }
    else {
      // Customer can choose the delivery date
      $delivery_date_shown_nb = isset($settings['options']['delivery_date']['shown_number']) ?
          $settings['options']['delivery_date']['shown_number'] : COMMERCE_BPOST_DELIVERY_DATE_SHOWN_NUMBER;
      $delivery_dates = commerce_bpost_get_delivery_date($refuse_saturday, $delivery_date_shown_nb, $settings);
      $delivery_date_options = array();
      foreach ($delivery_dates as $date) {
        $delivery_date_tpl = '<span class="fullday">' . t($date->format('l')) . '</span>';
        $delivery_date_tpl .= '<span class="daynum">' . $date->format('j') . '</span>';
        $delivery_date_tpl .= '<span class="month">' . $date->format('F') . '</span>';
        $delivery_date_tpl .= '<span class="extra-cost">';
        if (commerce_bpost_is_saturday($date)) {
          $extra_cost_price = commerce_bpost_get_saturday_shipping_extra_cost($shipping_service_name, $settings);
          $delivery_date_tpl .= '<span class="extra-cost-title">' . t('Extra-cost') . '</span><span class="extra-cost-price">' . $extra_cost_price . ' €' . '</span>';
        }
        $delivery_date_tpl .= '</span>';
        $delivery_date_options[$date->getTimestamp()] = $delivery_date_tpl;
      }

      $form['delivery_date']['date'] = array(
        '#type' => 'radios',
        '#options' => $delivery_date_options,
        '#required' => FALSE,
        '#prefix' => '<div id="delivery-date-date-wrapper">',
        '#suffix' => '</div>',
        '#attributes' => array(
          'class' => array('date-picker'),
        ),
      );
      if (empty($order)) {
        $form['delivery_date']['date']['#disabled'] = TRUE;
      }
      else {
        if (!empty($order->data['commerce_bpost']) && !empty($order->data['commerce_bpost']['delivery_date']) && in_array($order->data['commerce_bpost']['delivery_date']['date'], $delivery_date_options)) {
          $selected_date = $order->data['commerce_bpost']['delivery_date']['date'];
        }
        else {
          $selected_date = $delivery_dates[0]->getTimestamp();
        }
        if (array_key_exists($selected_date, $delivery_date_options)) {
          $form['delivery_date']['date']['#default_value'] = $selected_date;
        }
      }

      // IF a map is displayed, set the AJAX callback on selected date
      if ($shipping_service_name == 'bpost_postoffice_postpoint') {
        $form['delivery_date']['date']['#ajax'] = array(
          'callback' => 'commerce_bpost_pane_checkout_form_details_refresh',
          'wrapper' => 'commerce-shipping-service-details',
        );
        $form['delivery_date']['date']['#element_validate'] = array('_commerce_bpost_pane_value_hack');
      }
    }

    if (!$choose_delivery_date &&
        commerce_bpost_is_saturday_delivery_enabled($shipping_service_name) &&
        $delivery_date instanceof DateTime &&
        (commerce_bpost_is_saturday($delivery_date) || $refuse_saturday)) {
      // Get the extra cost for the selected shipping method
      $extra_cost = commerce_bpost_get_saturday_shipping_extra_cost($shipping_service_name);
      $form['delivery_date']['refuse_saturday'] = array(
        '#type' => 'checkbox',
        '#title' => t("I don't want my parcel to be delivered on a Saturday."),
        '#description' => t('(Extra cost Saturday delivery: @price EUR)', array('@price' => $extra_cost)),
        '#ajax' => array(
          'callback' => 'commerce_bpost_pane_checkout_form_details_refresh',
          'wrapper' => 'commerce-shipping-service-details',
        ),
      );
    }
  }

  $element_children = element_children($form['delivery_date'], FALSE);
  // Unset delivery_date container if no children.
  if (!$element_children) {
    unset($form['delivery_date']);
  }

  $form['#attached']['css'][] = drupal_get_path('module', 'commerce_bpost') . '/theme/css/shipping_services.css';
}

/**
 * Returns the delivery date(s) from the back-end settings.
 * 
 * @param bool $refuse_saturday
 * @param integer $date_nb Number of dates to return.
 * @param array $settings Override variable settings (used by the admin preview)
 * @return DateTime | array Returns a DateTime object if $date_nb is 1 or an array of DateTime objects.
 */
function commerce_bpost_get_delivery_date($refuse_saturday, $date_nb = 1, $settings = NULL) {
  if (!isset($settings)) {
    $settings = variable_get('commerce_bpost_settings', array());
  }
  $delivery_date = new DateTime();
  $delivery_date->setTime(0, 0, 0);

  // Process the shipping delay
  $shipping_offset = isset($settings['options']['delivery_date']['delay']) ?
      $settings['options']['delivery_date']['delay'] : 0;

  // Limit time
  $limit_time = isset($settings['options']['delivery_date']['time_limit']) ?
    $settings['options']['delivery_date']['time_limit'] : COMMERCE_BPOST_DELIVERY_DATE_TIME_LIMIT;
  // Increase the shipping delay for a greater current day time than time limit.
  if (date('H:i') > $limit_time) {
    $shipping_offset++;
  }

  while ($shipping_offset) {
    // Saturdays, Sundays and Holidays are not to be taken into account as "Days between order and shipment".
    $delivery_date->add(new DateInterval('P1D'));
    if (!commerce_bpost_is_saturday($delivery_date) &&
        !commerce_bpost_is_sunday($delivery_date) &&
        !commerce_bpost_is_holiday($delivery_date)) {
      $shipping_offset--;
    }
  }

  // Add the 24h necessary delay
  $delivery_date->add(new DateInterval('P1D'));

  // Get the requested number of delivery dates to show to the customer
  $delivery_date_options = array();
  while ($date_nb) {
    // Is Saturday delivery accepted?
    if ($refuse_saturday) {
      // ... get the next opened day
      while (commerce_bpost_is_saturday($delivery_date) || commerce_bpost_is_sunday($delivery_date) || commerce_bpost_is_holiday($delivery_date)) {
        $delivery_date->add(new DateInterval('P1D'));
      }
    }
    else {
      while (commerce_bpost_is_sunday($delivery_date) || commerce_bpost_is_holiday($delivery_date)) {
        $delivery_date->add(new DateInterval('P1D'));
      }
    }
    $delivery_date_options[] = clone $delivery_date;
    $delivery_date->add(new DateInterval('P1D'));
    $date_nb--;
  }

  return count($delivery_date_options) === 1 ? current($delivery_date_options) : $delivery_date_options;
}

/**
 * Returns TRUE if the given date is a saturday.
 * 
 * @param array $date
 * @return bool
 */
function commerce_bpost_is_saturday(DateTime $date) {
  return date('w', $date->getTimestamp()) == 6;
}

/**
 * Returns TRUE if $date is a sunday.
 * 
 * @param DateTime $date
 * @return bool
 */
function commerce_bpost_is_sunday(DateTime $date) {
  return date('w', $date->getTimestamp()) == 0;
}

/**
 * Returns TRUE if $date is a holiday.
 * 
 * @param DateTime $date
 * @return bool
 */
function commerce_bpost_is_holiday(DateTime $date) {
  $holidays = array(
    array(2015, 1, 1),
    array(2015, 4, 5),
    array(2015, 4, 6),
    array(2015, 5, 1),
    array(2015, 5, 14),
    array(2015, 5, 24),
    array(2015, 5, 25),
    array(2015, 7, 21),
    array(2015, 8, 15),
    array(2015, 11, 1),
    array(2015, 11, 11),
    array(2015, 12, 25),
    array(2016, 1, 1),
    array(2016, 3, 27),
    array(2016, 3, 28),
    array(2016, 5, 1),
    array(2016, 5, 5),
    array(2016, 5, 15),
    array(2016, 5, 16),
    array(2016, 7, 21),
    array(2016, 8, 15),
    array(2016, 11, 1),
    array(2016, 11, 11),
    array(2016, 12, 25),
    array(2017, 1, 1),
    array(2017, 4, 16),
    array(2017, 4, 17),
    array(2017, 5, 1),
    array(2017, 5, 25),
    array(2017, 6, 4),
    array(2017, 6, 5),
    array(2017, 7, 21),
    array(2017, 8, 15),
    array(2017, 11, 1),
    array(2017, 11, 11),
    array(2017, 12, 25),
    array(2018, 1, 1),
    array(2018, 4, 1),
    array(2018, 4, 2),
    array(2018, 5, 1),
    array(2018, 5, 10),
    array(2018, 5, 20),
    array(2018, 5, 21),
    array(2018, 7, 21),
    array(2018, 8, 15),
    array(2018, 11, 1),
    array(2018, 11, 11),
    array(2018, 12, 25),
    array(2019, 1, 1),
    array(2019, 4, 21),
    array(2019, 4, 22),
    array(2019, 5, 1),
    array(2019, 5, 30),
    array(2019, 6, 9),
    array(2019, 6, 10),
    array(2019, 7, 21),
    array(2019, 8, 15),
    array(2019, 11, 1),
    array(2019, 11, 11),
    array(2019, 12, 25),
    array(2020, 1, 1),
    array(2020, 4, 12),
    array(2020, 4, 13),
    array(2020, 5, 1),
    array(2020, 5, 21),
    array(2020, 5, 31),
    array(2020, 6, 1),
    array(2020, 7, 21),
    array(2020, 8, 15),
    array(2020, 11, 1),
    array(2020, 11, 11),
    array(2020, 12, 25),
  );
  $i = 0;
  $is_holiday = FALSE;
  $holiday = new DateTime();
  do {
    list($year, $month, $day) = $holidays[$i];
    $holiday->setDate($year, $month, $day);
    $holiday->setTime(0, 0, 0);
    $is_holiday = ($date == $holiday);
    $i++;
  } while ($i < count($holidays) && !$is_holiday && $holiday <= $date);
  return $is_holiday;
}

/**
 * Returns TRUE if $date is not a saturday, nor a sunday, nor a holiday.
 * 
 * @param DateTime $date
 * @return bool
 */
function commerce_bpost_is_opened_day(DateTime $date) {
  $day_nb = date('w', $date->getTimestamp());
  return $day_nb != 6 && $day_nb != 0 && !commerce_bpost_is_holiday($date);
}

/**
 * Process the shipping details form, when the delivery date has been displayed to the customer.
 * 
 * @param array $form
 * @param array $form_state
 */
function commerce_bpost_delivery_date_submit($details_form, $details_values, $line_item) {
  $commerce_bpost_settings = variable_get('commerce_bpost_settings', array());
  if (isset($commerce_bpost_settings['options']['delivery_date']['display'])) {
    // Parameter "Display delivery date" = YES => Save the delivery date into the order data
    $refuse_saturday = isset($details_values['delivery_date']['refuse_saturday']) ?
        (bool) $details_values['delivery_date']['refuse_saturday'] : FALSE;
    $tz = new DateTimeZone(date_default_timezone(FALSE));
    $delivery_date = !empty($details_values['delivery_date']['date']) ?
        new DateTime('@' . $details_values['delivery_date']['date']) :
        commerce_bpost_get_delivery_date($refuse_saturday);
    $delivery_date->setTimeZone($tz);

    if ($delivery_date instanceof DateTime) {
      // Store the delivery date and the drop date into the order
      commerce_bpost_save_delivery_date($line_item, $delivery_date);

      if (commerce_bpost_is_saturday($delivery_date) &&
          module_load_include('inc', 'commerce_bpost', 'includes/commerce_bpost.admin')) {
        $shipping_service_name = isset($details_values['shipping_service_name']) ? $details_values['shipping_service_name'] : NULL;
        $extra_cost = commerce_bpost_get_saturday_shipping_extra_cost($shipping_service_name);
        $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);

        // Build a price array for the saturday delivery extra cost
        $express_price = array(
          'amount' => commerce_currency_decimal_to_amount($extra_cost, 'EUR'),
          'currency_code' => 'EUR',
          'data' => array(),
        );

        // Add the saturday delivery extra cost component to the unit price.
        $line_item_wrapper->commerce_unit_price->data = commerce_price_component_add(
            $line_item_wrapper->commerce_unit_price->value(), 'commerce_bpost_saturday_delivery', $express_price, TRUE, FALSE
        );
      }
    }
    else {
      watchdog('commerce_order', 'Cannot retrieve the delivery date - MUST be an instance of DateTime', array(), WATCHDOG_ERROR);
    }
  }
}

/**
 * Implements hook_commerce_price_component_type_info().
 */
function commerce_bpost_commerce_price_component_type_info() {
  return array(
    'commerce_bpost_saturday_delivery' => array(
      'title' => t('Saturday delivery'),
      'weight' => 20,
    ),
  );
}

/**
 * Saves the delivery date and the drop date into the order data.
 * 
 * @param stdClass $shipping_line_item
 * @param DateTime $delivery_date
 */
function commerce_bpost_save_delivery_date(stdClass $shipping_line_item, DateTime $delivery_date) {
  $drop_date = commerce_bpost_get_drop_date($delivery_date);
  $order = commerce_order_load($shipping_line_item->order_id);
  $order->data['commerce_bpost']['delivery_date']['date'] = $delivery_date->getTimestamp();
  $order->data['commerce_bpost']['delivery_date']['dropdate'] = $drop_date->getTimestamp();
  commerce_order_save($order);
}

/**
 * Retrieve the delivery date from an order.
 * 
 * @param stdClass $order
 * @return DateTime | NULL
 */
function commerce_bpost_load_delivery_date(stdClass $order) {
  $delivery_date_ts = isset($order->data['commerce_bpost']['delivery_date']['date']) ? $order->data['commerce_bpost']['delivery_date']['date'] : NULL;

  if (isset($delivery_date_ts)) {
    $delivery_date = new DateTime('@' . $delivery_date_ts);
    $tz = new DateTimeZone(date_default_timezone(FALSE));
    $delivery_date->setTimezone($tz);
  }
  return isset($delivery_date) ? $delivery_date : NULL;
}

/**
 * Retrieve the drop date from the delivery date.
 * 
 * @param DateTime $delivery_date
 * @return DateTime
 */
function commerce_bpost_get_drop_date(DateTime $delivery_date) {
  // Drop date is delivery date - 1d
  $drop_date = clone $delivery_date;
  $drop_date->sub(new DateInterval('P1D'));

  // Drop date has to be a weekday and cannot be a holiday
  while (!commerce_bpost_is_opened_day($drop_date)) {
    $drop_date->sub(new DateInterval('P1D'));
  }

  return $drop_date;
}
