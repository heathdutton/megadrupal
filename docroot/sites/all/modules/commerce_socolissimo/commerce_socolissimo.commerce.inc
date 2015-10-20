<?php

/**
 * @file
 * All relatives commerce methods for commerce_socolissimo module.
 */

/**
 * Implements hook_commerce_shipping_method_info().
 */
function commerce_socolissimo_commerce_shipping_method_info() {
  $shipping_methods = array();

  $shipping_methods['socolissimo_flexibility'] = array(
    'title' => t('So Colissimo Flexibility'),
    'display_title' => t('So Colissimo Flexibility'),
    'description' => t('Defines a shipping method that handles French and Belgium shipping services.'),
  );

  return $shipping_methods;
}

/**
 * Implements hook_commerce_shipping_service_info().
 */
function commerce_socolissimo_commerce_shipping_service_info() {
  $shipping_services = array();

  $shipping_services['socolissimo_home_delivery'] = array(
    'title' => t('So Colissimo: Home Delivery'),
    'description' => t('Delivery to the address of your choice.'),
    'display_title' => t('home delivery'),
    'shipping_method' => 'socolissimo_flexibility',
    'price_component' => 'shipping',
    'callbacks' => array(
      'rate' => 'commerce_socolissimo_default_service_rate',
      'details_form' => 'commerce_socolissimo_service_details_form',
      'details_form_validate' => 'commerce_socolissimo_service_details_form_validate',
      'details_form_submit' => 'commerce_socolissimo_service_details_form_submit',
    ),
  );

  $shipping_services['socolissimo_appointment'] = array(
    'title' => t('So Colissimo: Appointment'),
    'description' => t('Delivery by appointment between 5 p.m. and 9:30 p.m. to the address of your choice.'),
    'display_title' => t('appointment'),
    'shipping_method' => 'socolissimo_flexibility',
    'price_component' => 'shipping',
    'callbacks' => array(
      'rate' => 'commerce_socolissimo_appointment_service_rate',
      'details_form' => 'commerce_socolissimo_service_details_form',
      'details_form_validate' => 'commerce_socolissimo_service_details_form_validate',
      'details_form_submit' => 'commerce_socolissimo_service_details_form_submit',
    ),
  );

  $shipping_services['socolissimo_cityssimo'] = array(
    'title' => t('So Colissimo: Cityssimo'),
    'description' => t('Delivery in one of 31 Cityssimo areas.'),
    'display_title' => t('cityssimo'),
    'shipping_method' => 'socolissimo_flexibility',
    'price_component' => 'shipping',
    'callbacks' => array(
      'rate' => 'commerce_socolissimo_default_service_rate',
      'details_form' => 'commerce_socolissimo_delivery_point_details_form',
      'details_form_validate' => 'commerce_socolissimo_delivery_point_details_form_validate',
      'details_form_submit' => 'commerce_socolissimo_delivery_point_details_form_submit',
    ),
  );

  $shipping_services['socolissimo_post_office'] = array(
    'title' => t('So Colissimo: Post office'),
    'description' => t('Delivery in one of the 10,000 post offices of your choice.'),
    'display_title' => t('post office'),
    'shipping_method' => 'socolissimo_flexibility',
    'price_component' => 'shipping',
    'callbacks' => array(
      'rate' => 'commerce_socolissimo_default_service_rate',
      'details_form' => 'commerce_socolissimo_delivery_point_details_form',
      'details_form_validate' => 'commerce_socolissimo_delivery_point_details_form_validate',
      'details_form_submit' => 'commerce_socolissimo_delivery_point_details_form_submit',
    ),
  );

  $shipping_services['socolissimo_local_store'] = array(
    'title' => t('So Colissimo: Local store'),
    'description' => t('Delivery to one of the merchant store from our network partners.'),
    'display_title' => t('local store pick up'),
    'shipping_method' => 'socolissimo_flexibility',
    'price_component' => 'shipping',
    'callbacks' => array(
      'rate' => 'commerce_socolissimo_default_service_rate',
      'details_form' => 'commerce_socolissimo_delivery_point_details_form',
      'details_form_validate' => 'commerce_socolissimo_delivery_point_details_form_validate',
      'details_form_submit' => 'commerce_socolissimo_delivery_point_details_form_submit',
    ),
  );

  return $shipping_services;
}

/**
 * Callback: calculate the base rate of the selected shipping service.
 *
 * @param string $shipping_service
 *   Shipping service name.
 * @param array $order
 *   A fully qualified order object.
 *
 * @return int
 *   The shipping service rate.
 */
function commerce_socolissimo_default_service_rate($shipping_service, $order) {
  // Check if the passed shipping service is actived or not.
  if ($shipping_service_status = commerce_socolissimo_service_status_load($shipping_service['name'])) {
    if ($shipping_service_status['enabled'] == 0) {
      return FALSE;
    }
  }

  // Shipping services are only available for France and Belgium.
  $wrapper_order = entity_metadata_wrapper('commerce_order', $order);
  $country_code = $wrapper_order->commerce_customer_shipping->commerce_customer_address->country->value();
  switch ($country_code) {
    case 'FR':
      $socolissimo_rate_name = 'socolissimo_flex_rate_fr';
      break;

    case 'BE':
      $socolissimo_rate_name = 'socolissimo_flex_rate_be';
      $socolissimo_settings = variable_get('commerce_socolissimo_settings', array());
      if (!isset($socolissimo_settings['belgium']) || !$socolissimo_settings['belgium'] || !_commerce_socolissimo_is_belgian_service($shipping_service['name'])) {
        return FALSE;
      }
      break;

    default:
      return FALSE;
  }

  // Get the weight of the passed order.
  $weight = commerce_physical_order_weight($order, 'kg');

  // Depending of the shipping service, returns the correct rate.
  $rate = commerce_socolissimo_get_rate_by_weight($shipping_service['name'], $weight);
  $wrapper_rate = entity_metadata_wrapper('commerce_socolissimo_rate', $rate);

  return $wrapper_rate->{$socolissimo_rate_name}->value();

}

/**
 * Specific rate calculation for socolissimo appointment shipping service.
 *
 * @param string $shipping_service
 *   Shipping service name.
 * @param array $order
 *   A fully qualified order object.
 *
 * @return mixed
 *   The shipping service rate. False otherwise.
 */
function commerce_socolissimo_appointment_service_rate($shipping_service, $order) {
  $return = commerce_socolissimo_default_service_rate($shipping_service, $order);
  if ($return) {
    $wrapper_order = entity_metadata_wrapper('commerce_order', $order);
    $customer_address = $wrapper_order->commerce_customer_shipping->commerce_customer_address->value();

    if (!_commerce_socolissimo_check_parisian_zip_code($customer_address['postal_code'])) {
      $return = FALSE;
    }
  }

  return $return;
}

/**
 * Shipping service callback: home delivery & appointment.
 *
 * Checks every required information needed by SoColissimo.
 */
function commerce_socolissimo_service_details_form($pane_form, $pane_values, $checkout_pane, $order, $shipping_service) {
  $wrapper_order = entity_metadata_wrapper('commerce_order', $order);
  $customer_address = $wrapper_order->commerce_customer_shipping->commerce_customer_address->value();

  $form = array();
  // Ask for phone number if it isn't defined or invalid.
  if (empty($customer_address['phone_number']) || is_int(_commerce_socolissimo_check_phone_number($customer_address['phone_number'], $customer_address['country']))) {
    $description = t('France: cellphone number must begin with 06 or 07 followed by 8 numbers, without spaces, "." or any characters.') . PHP_EOL;
    $description .= t('Belgium: cellphone number must begin with character + followed by 324 and 8 numbers.');

    $form['phone_number'] = array(
      '#type' => 'textfield',
      '#title' => t('Phone number'),
      '#description' => $description,
      '#default_value' => "",
      '#maxlength' => 15,
      '#size' => 15,
      '#required' => TRUE,
    );
  }

  return $form;
}

/**
 * Shipping service callback: cityssimo, local store & post office.
 */
function commerce_socolissimo_delivery_point_details_form($pane_form, $pane_values, $checkout_pane, $order, $shipping_service) {
  // Settings.
  $socolissimo_settings = variable_get('commerce_socolissimo_settings', array());
  $socolissimo_map = _commerce_socolissimo_map_settings();

  // Address.
  $wrapper_order = entity_metadata_wrapper('commerce_order', $order);

  // Define allowed countries.
  $countries = array(
    'FR' => t('France'),
  );
  if (isset($socolissimo_settings['belgium']) && $socolissimo_settings['belgium'] && _commerce_socolissimo_is_belgian_service($shipping_service['name'])) {
    $countries['BE'] = t('Belgium');
  }

  // Customer address.
  if (!empty($pane_values['commerce_socolissimo']['filters']['thoroughfare']) ||
    !empty($pane_values['commerce_socolissimo']['filters']['postal_code']) ||
    !empty($pane_values['commerce_socolissimo']['filters']['city']) ||
    !empty($pane_values['commerce_socolissimo']['filters']['country'])
  ) {
    unset($pane_values['commerce_socolissimo']['filters']['submit']);
    $customer_address = $pane_values['commerce_socolissimo']['filters'];
  }
  else {
    $commerce_customer_address = $wrapper_order->commerce_customer_shipping->commerce_customer_address;
    $customer_address['thoroughfare'] = $commerce_customer_address->thoroughfare->value();
    $customer_address['postal_code'] = $commerce_customer_address->postal_code->value();
    $customer_address['city'] = $commerce_customer_address->locality->value();
    $customer_address['country'] = $commerce_customer_address->country->value();
  }

  // Selected point.
  $selected_point_id = NULL;
  $selected_point = NULL;
  if (!empty($pane_values['commerce_socolissimo']['bottom']['point_wrapper']['points']) && isset($pane_values['commerce_socolissimo']['points'][$pane_values['commerce_socolissimo']['bottom']['point_wrapper']['points']])) {
    $selected_point_id = $pane_values['commerce_socolissimo']['bottom']['point_wrapper']['points'];
    $selected_point = $pane_values['commerce_socolissimo']['points'][$pane_values['commerce_socolissimo']['bottom']['point_wrapper']['points']];
  }
  else {
    if ($shipping_line_item = _commerce_socolissimo_get_shipping_line_item($order)) {
      $selected_point_id = $shipping_line_item->data['service_details']['commerce_socolissimo_point']['identifiant'];
      $selected_point = $shipping_line_item->data['service_details']['commerce_socolissimo_point'];
    }
  }

  $socolissimo_map += array(
    'address' => implode(', ', $customer_address),
    'icons' => array(
      'domicile' => base_path() . drupal_get_path('module', 'commerce_socolissimo') . '/icons/domicile.png',
    ),
  );

  $form = array();
  $form['#attributes']['class'][] = 'commerce-socolissimo-flexibility';

  $form['filters'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
    '#attributes' => array(
      'class' => array(
        'container-inline',
        'commerce-socolissimo-flexibility-filters',
      ),
    ),
  );

  $form['filters']['thoroughfare'] = array(
    '#type' => 'textfield',
    '#title' => t('Thoroughfare'),
    '#default_value' => $customer_address['thoroughfare'],
    '#size' => 30,
    '#maxlength' => 64,
    '#element_validate' => array('_commerce_socolissimo_pane_value_hack'),
  );
  $form['filters']['postal_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Postal code'),
    '#default_value' => $customer_address['postal_code'],
    '#size' => 10,
    '#maxlength' => 10,
    '#element_validate' => array('_commerce_socolissimo_pane_value_hack'),
  );
  $form['filters']['city'] = array(
    '#type' => 'textfield',
    '#title' => t('City'),
    '#default_value' => $customer_address['city'],
    '#size' => 15,
    '#maxlength' => 32,
    '#element_validate' => array('_commerce_socolissimo_pane_value_hack'),
  );
  $form['filters']['country'] = array(
    '#type' => 'select',
    '#title' => t('Country'),
    '#default_value' => $customer_address['country'],
    '#options' => $countries,
    '#element_validate' => array('_commerce_socolissimo_pane_value_hack'),
  );
  $form['filters']['submit'] = array(
    '#type' => 'button',
    '#value' => t('Search'),
    '#ajax' => array(
      'callback' => 'commerce_socolissimo_pane_checkout_form_details_refresh',
      'wrapper' => 'commerce-shipping-service-details',
      'method' => 'replace',
    ),
  );

  $phone_number_form = commerce_socolissimo_service_details_form($pane_form, $pane_values, $checkout_pane, $order, $shipping_service);
  $form += $phone_number_form;

  $markup = NULL;
  if (!empty($selected_point)) {
    $markup = theme('commerce_socolissimo_selected_point', array('element' => _commerce_socolissimo_convert_into_render_array($selected_point)));
  }

  $form['point'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('point-wrapper'),
    ),
  );
  $form['point']['content'] = array(
    '#markup' => $markup,
  );

  // Delivery points.
  // Calculate shipping date.
  $order_dispatching_time = isset($socolissimo_settings['order_dispatching_time']) ? (int) $socolissimo_settings['order_dispatching_time'] : 0;
  $timestamp = time() + ($order_dispatching_time * 24 * 60 * 60);
  $date = date('d/m/Y', $timestamp);
  $weight = commerce_physical_order_weight($order, 'g');
  $variables = array(
    'address' => $customer_address['thoroughfare'],
    'zipCode' => $customer_address['postal_code'],
    'city' => $customer_address['city'],
    'countryCode' => $customer_address['country'],
    'weight' => isset($weight['weight']) ? $weight['weight'] : NULL,
    'shippingDate' => $date,
    'optionInter' => ($customer_address['country'] == 'BE') ? 2 : 0,
  );

  $point_types = array();
  switch ($shipping_service['name']) {
    case 'socolissimo_cityssimo':
      $point_types[] = 'CIT';
      break;

    case 'socolissimo_local_store':
      $point_types[] = 'A2P';
      $point_types[] = 'CMT';
      break;

    case 'socolissimo_post_office':
      $point_types[] = 'BPR';
      $point_types[] = 'ACP';
      $point_types[] = 'CDI';
      $point_types[] = 'BDP';
      break;

  }
  $map_markers = commerce_socolissimo_api('findRDVPointRetraitAcheminement', $variables, $point_types, $error);

  if (!empty($error)) {
    // Socolissimo API error.
    watchdog('commerce_socolissimo', $error, array(), WATCHDOG_ERROR);
    $report_access = user_access('access site reports');
    $message = $report_access ? l(t('Check reports'), 'admin/reports/dblog') : t('Please contact the site administrator.');
    $form['error'] = array(
      '#type' => 'item',
      '#markup' => '<div class="messages error">' . t('So Colissimo API error.') . ' ' . $message . '</div>',
    );
  }

  // Reset map position and zoom.
  $form['bottom']['home_map'] = array(
    '#type' => 'item',
    '#markup' => '<div class="commerce-socolissimo-map-home">' . theme('image', array('path' => $socolissimo_map['icons']['domicile'])) . '<span>' . t('Center map on home') . '</span></div>',
  );

  // Points.
  $form['bottom']['point_wrapper'] = array(
    '#type' => 'commerce_socolissimo_point_wrapper',
    '#tree' => TRUE,
    '#attributes' => array(
      'class' => array('commerce-socolissimo-point-wrapper'),
    ),
  );
  if (!empty($map_markers)) {
    foreach ($map_markers as $id => $marker) {
      // Convert marker into renderable array.
      $map_markers[$id]['default_value'] = (!empty($selected_point_id) && $selected_point_id == $id);
      $render_marker = _commerce_socolissimo_convert_into_render_array($map_markers[$id]);
      $form['bottom']['point_wrapper']['points'][$id] = array(
        '#type' => 'commerce_socolissimo_point',
        '#parents' => array(
          'commerce_shipping',
          'service_details',
          'bottom',
          'point_wrapper',
          'points',
        ),
      ) + $render_marker;
      foreach ($marker as $key => $item) {
        $form['points'][$marker['identifiant']][$key] = array(
          '#type' => 'value',
          '#value' => $item,
        );
      }
      $map_markers[$id]['content_map'] = theme('commerce_socolissimo_point_details', array('element' => $render_marker));
      $map_markers[$id]['content_top'] = theme('commerce_socolissimo_selected_point', array('element' => $render_marker));
    }
  }
  else {
    // Socolissimo API not configured.
    $form['bottom']['point_wrapper']['points']['no_points'] = array(
      '#type' => 'item',
      '#markup' => t('No delivery points available near your location for the service: !service_name.', array('!service_name' => $shipping_service['display_title'])),
    );
  }

  // Use json_encode to ensure markers array is reset javascript settings.
  $socolissimo_map += array('markers' => json_encode($map_markers));

  // Map.
  $form['bottom']['map'] = array(
    '#type' => 'commerce_socolissimo_map',
    '#map' => $socolissimo_map,
    '#attributes' => array(
      'class' => array('commerce-socolissimo-map'),
    ),
  );

  $form['#attached']['css'][] = drupal_get_path('module', 'commerce_socolissimo') . '/theme/css/commerce_socolissimo.css';
  drupal_add_js(array('commerce_socolissimo_map' => $socolissimo_map), array('type' => 'setting'));

  return $form;
}

/**
 * Helper function to build a point.
 *
 * @param array $data
 *   Object point data from Socollismo web service.
 *
 * @return array
 *   Same data with additional icon, address and content keys.
 */
function _commerce_socolissimo_build_point($data) {
  // Icons.
  $module_path = base_path() . drupal_get_path('module', 'commerce_socolissimo');
  $icons = array(
    'cityssimo' => $module_path . '/icons/cityssimo.png',
    'bureau_poste' => $module_path . '/icons/bureau_poste.png',
    'commercant' => $module_path . '/icons/commercant.png',
    'rdv' => $module_path . '/icons/rdv.png',
    'mobilite_reduite' => $module_path . '/icons/mobilite_reduite.gif',
    'parking' => $module_path . '/icons/parking.gif',
    'pret_manutention' => $module_path . '/icons/pret_manutention.gif',
  );

  switch ($data->typeDePoint) {
    // Cityssimo.
    case 'CIT':
      $point['icon'] = $icons['cityssimo'];
      break;

    // Post office.
    case 'BPR':
    case 'ACP':
    case 'CDI':
    case 'BDP':
      $point['icon'] = $icons['bureau_poste'];
      break;

    // Merchant (local store pick up).
    case 'A2P':
    case 'CMT':
      $point['icon'] = $icons['commercant'];
      break;

    default:
      $point['icon'] = NULL;
  }

  // Ease of access.
  $ease_of_access = array(
    'accesPersonneMobiliteReduite' => 'mobilite_reduite',
    'parking' => 'parking',
    'loanOfHandlingTool' => 'pret_manutention',
  );
  $point['ease_of_access'] = array();
  foreach ($ease_of_access as $key => $value) {
    if ($data->$key) {
      $point['ease_of_access'][$value] = $icons[$value];
    }
    unset($data->$key);
  }

  // Address.
  $point['adresse'] = $data->adresse1 . ' ' . $data->adresse2 . ' ' . $data->adresse3;
  unset($data->adresse1);
  unset($data->adresse2);
  unset($data->adresse3);

  // Shop hours.
  $hours_keys = array(
    'horairesOuvertureLundi' => t('Monday'),
    'horairesOuvertureMardi' => t('Tuesday'),
    'horairesOuvertureMercredi' => t('Wednesday'),
    'horairesOuvertureJeudi' => t('Thursday'),
    'horairesOuvertureVendredi' => t('Friday'),
    'horairesOuvertureSamedi' => t('Saturday'),
    'horairesOuvertureDimanche' => t('Sunday'),
  );
  foreach ($hours_keys as $hour_key => $hours_value) {
    $hours = trim(str_replace('00:00-00:00', NULL, $data->$hour_key));
    if (!empty($hours)) {
      $point['opening_hours'][] = array(
        'title' => $hours_value,
        'value' => $hours,
      );
    }
    unset($data->$hour_key);
  }

  $point += (array) $data;

  return $point;
}

/**
 * Helper function to convert an array to a renderable array properties.
 *
 * @param array $data
 *   Array, really simple.
 *
 * @return array
 *   Renderable array properties
 */
function _commerce_socolissimo_convert_into_render_array($data) {
  $keys = array_keys($data);
  array_walk($keys, '_commerce_socolissimo_transform_into_render_array');
  $data_render = array_combine($keys, $data);

  return $data_render;
}

/**
 * Helper function to convert array into renderable array.
 */
function _commerce_socolissimo_transform_into_render_array(&$item) {
  $item = '#' . $item;
}

/**
 * Shipping service validation callback.
 */
function commerce_socolissimo_service_details_form_validate($details_form, $details_values, $shipping_service, $order, $form_parents) {
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  $country_code = $order_wrapper->commerce_customer_shipping->commerce_customer_address->country->value();
  if (isset($details_values['phone_number']) && is_int($code_error = _commerce_socolissimo_check_phone_number($details_values['phone_number'], $country_code))) {
    include_once DRUPAL_ROOT . '/includes/locale.inc';
    $countries = country_get_list();
    switch ($code_error) {
      case 0:
        form_set_error(
          implode('][', array_merge($form_parents, array('phone_number'))),
          t(
            '@phone_number is not a valid @country cellphone number.',
            array(
              "@phone_number" => $details_values['phone_number'],
              '@country' => $countries[$country_code],
            )
          )
        );
        break;

      case 1:
        form_set_error(
          implode('][', array_merge($form_parents, array('phone_number'))),
          t('Eight or more same numbers are not allowed in cellphone number.')
        );
        break;

    }

    // Even though the form error is enough to stop the submission of the form,
    // it's not enough to stop it from a Commerce standpoint because of the
    // combined validation / submission going on per-pane in the checkout form.
    return FALSE;
  }

  return TRUE;
}

/**
 * Submission form callback.
 */
function commerce_socolissimo_service_details_form_submit($details_form, $details_values, $line_item) {
  // Get the customer address and set the new phone number. Based on default
  // field provided by drupal commerce.
  $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
  $customer_address = $line_item_wrapper->order->commerce_customer_shipping->commerce_customer_address->value();
  if (empty($customer_address['phone_number'])) {
    $customer_address['phone_number'] = $details_values['phone_number'];
    $line_item_wrapper->order->commerce_customer_shipping->commerce_customer_address = $customer_address;
    $line_item_wrapper->order->commerce_customer_shipping->save();
  }
}

/**
 * Shipping service validation callback.
 */
function commerce_socolissimo_delivery_point_details_form_validate($details_form, $details_values, $shipping_service, $order, $form_parents) {
  $return = TRUE;
  if (!commerce_socolissimo_service_details_form_validate($details_form, $details_values, $shipping_service, $order, $form_parents)) {
    $return = FALSE;
  }
  if (empty($details_values['bottom']['point_wrapper']['points']) || !isset($details_values['points'][$details_values['bottom']['point_wrapper']['points']])) {
    form_set_error('commerce_socolissimo_point_id', t('Please select a point.'));
    $return = FALSE;
  }

  return $return;
}

/**
 * Shipping service submit callback.
 */
function commerce_socolissimo_delivery_point_details_form_submit($details_form, $details_values, $line_item) {
  commerce_socolissimo_service_details_form_submit($details_form, $details_values, $line_item);
  $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
  $line_item_wrapper->value()->data['service_details'] = array('commerce_socolissimo_point' => $details_values['points'][$details_values['bottom']['point_wrapper']['points']]);
}

/**
 * Implements hook_commerce_checkout_pane_info().
 */
function commerce_socolissimo_commerce_checkout_pane_info() {
  $checkout_panes = array();

  $checkout_panes['commerce_socolissimo_shipping_profile'] = array(
    'title' => t('Shipping information'),
    'file' => 'includes/commerce_socolissimo.checkout_pane.inc',
    'base' => 'commerce_socolissimo_pane',
    'page' => 'checkout',
    'weight' => 8,
  );

  return $checkout_panes;
}
