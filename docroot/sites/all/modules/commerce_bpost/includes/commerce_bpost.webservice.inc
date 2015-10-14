<?php

/**
 * @file
 * Define webservices methods to call Bpost services.
 */

define('COMMERCE_BPOST_BPACK_CREDENTIALS', 'aW5mb0Bjb21tZXJjZWd1eXMuY29tOmMwbW1lcmNlR3V5c0YwckJwMHN0');

/**
 * This function send a request to bpost Webservice.
 *
 * @param string $service
 *   The service we request.
 * @param array $options
 *   The options for drupal_http_request second arguments.
 * @param string $url
 *   The base url if different than the one set on the administration page.
 *
 * @return array
 */
function commerce_bpost_request_bpost($service, $options = array(), $url = '') {
  $commerce_bpost_settings = variable_get('commerce_bpost_settings');
  if (empty($url)) {
    $url = commerce_bpost_get_bpost_base_url() . $service;
  }
  else {
    $url .= $service;
  }
  $authorization = isset($commerce_bpost_settings['account']['account_id']) ? $commerce_bpost_settings['account']['account_id'] : '';
  $authorization .= ':';
  $authorization .= isset($commerce_bpost_settings['account']['passphrase']) ? $commerce_bpost_settings['account']['passphrase'] : '';
  $options += array(
    'method' => 'GET',
    'headers' => array(),
  );
  $options['headers'] += array('Authorization' => 'Basic ' . base64_encode($authorization));

  return drupal_http_request($url, $options);
}

/**
 * Get base url for making calls with bpost WS.
 *
 * @return string
 *  Return the base url.
 */
function commerce_bpost_get_bpost_base_url() {
  $commerce_bpost_settings = variable_get('commerce_bpost_settings');
  return !empty($commerce_bpost_settings['account']['api_url']) ? $commerce_bpost_settings['account']['api_url'] : '';
}

/**
 * Validate API credentials
 *
 * @param string $login
 *   API login
 * @param string $pass
 *   API pass
 * @param string $url
 *   API url
 *
 * @return bool
 *   Whether credentials are valid or not.
 */
function commerce_bpost_webservice_validate_credentials($login, $pass, $url) {
  $offset = 0;
  $parsed_url = parse_url($url);
  if (!isset($parsed_url['scheme'])) {
    $parsed_url['scheme'] = 'http';
  }
  else {
    $offset = drupal_strlen($parsed_url['scheme']) + 3;
  }
  $response = commerce_bpost_request_bpost(NULL, array(), $parsed_url['scheme'] . '://' . $login . ':' . $pass . '@' . substr($url, $offset));

  switch ($response->code) {
    case 405:
      /**
       * 405 (Method Not Allowed) is the valid code for
       * https://api.bpost.be/services/shm since we don't provide
       * the method to call
       * Otherwise we hit a 401 error code if the Authorization is invalid
       */
      $valid = TRUE;
      break;
    default:
      $valid = FALSE;
  }
  variable_set('commerce_bpost_credentials', $valid);
  return $valid;
}

function commerce_bpost_webservice_create_order($order, $order_type, $options = array()) {
  $options += array(
    'return' => FALSE,
    'send_only' => FALSE,
  );
  $commerce_bpost_settings = variable_get('commerce_bpost_settings', array());
  $field_mapping = $commerce_bpost_settings['mapping']['profile']['fields'];
  $order_wrapper = entity_metadata_wrapper($order_type, $order);
  switch ($order_type) {
    case 'commerce_order':
      $customer_address = $order_wrapper->commerce_customer_shipping;
      $customer_address = array(
        'name' => commerce_bpost_get_field_value('name', $customer_address, $field_mapping, 'commerce_customer_profile'),
        'company' => commerce_bpost_get_field_value('company', $customer_address, $field_mapping, 'commerce_customer_profile'),
        'streetName' => substr(commerce_bpost_get_field_value('streetName', $customer_address, $field_mapping, 'commerce_customer_profile'), 0, 40),
        'number' => substr(commerce_bpost_get_field_value('number', $customer_address, $field_mapping, 'commerce_customer_profile'), 0, 40),
        'box' => substr(commerce_bpost_get_field_value('box', $customer_address, $field_mapping, 'commerce_customer_profile'), 0, 8),
        'postalCode' => substr(commerce_bpost_get_field_value('postalCode', $customer_address, $field_mapping, 'commerce_customer_profile'), 0, 40),
        'locality' => substr(commerce_bpost_get_field_value('locality', $customer_address, $field_mapping, 'commerce_customer_profile'), 0, 40),
        'mail' => substr($customer_address->user->mail->value(), 0, 50),
        'countryCode' => substr(commerce_bpost_get_field_value('countryCode', $customer_address, $field_mapping, 'commerce_customer_profile'), 0, 2),
      );
      $country_code = $customer_address['countryCode'];
      $sender_address = $commerce_bpost_settings['mapping']['sender'];
      $line_item_field = 'commerce_line_items';
      $total_field = 'commerce_order_total';
      $original_order = $order_wrapper;
      $weight = commerce_physical_order_weight($order, 'g');
      break;
    default:
      return FALSE;
  }

  if ($options['return']) {
    $tmp = $sender_address;
    $sender_address = $customer_address;
    $customer_address = $tmp;
    unset($tmp);
  }

  if (!$weight) {
    $weight = array('weight' => 10);
  }

  $shipping_line_item = _commerce_bpost_get_shipping_line_item($order);

  if (!isset($shipping_line_item, $sender_address)) {
    watchdog('commerce_bpost', 'Webservice > createOrder method: required data not found. Nothing has been sent.', array(), WATCHDOG_ERROR);
    return FALSE;
  }

  // Retrieve the recipient phone number
  $shipping_item_data = $shipping_line_item->value();
  if (!empty($shipping_item_data->data['notification_message_phone'])) {
    // Shipping service: COMMERCE_BPOST_SERVICE_POSTPOINT (@bpost) and COMMERCE_BPOST_SERVICE_PARCEL_LOCKER.
    $customer_address['phoneNumber'] = $shipping_item_data->data['notification_message_phone'];
  }
  else {
    $customer_address['phoneNumber'] = substr(commerce_bpost_get_field_value('phoneNumber', $order_wrapper->commerce_customer_shipping, $field_mapping, 'commerce_customer_profile'), 0, 20);
  }
  // Build the delivery data from the Bpost point details data.
  if (!empty($shipping_item_data->data['point_details'])) {
    $shipping_poi_data = $shipping_item_data->data['point_details']['Poi']['Record'];
    $point_id  = !empty($shipping_item_data->data['point_details']['Query']['Id']) ? $shipping_item_data->data['point_details']['Query']['Id'] : '';
    $point_office = !empty($shipping_poi_data['OFFICE']) ? $shipping_poi_data['OFFICE'] : '';
    $point_street_name = !empty($shipping_poi_data['STREET']) ? substr($shipping_poi_data['STREET'], 0, 40) : '';
    $point_number = !empty($shipping_poi_data['NR']) ? substr($shipping_poi_data['NR'], 0, 8) : '';
    $point_postal_code = !empty($shipping_poi_data['ZIP']) ? substr($shipping_poi_data['ZIP'], 0, 40) : '';
    $point_locality = !empty($shipping_poi_data['CITY']) ? substr($shipping_poi_data['CITY'], 0, 40) : '';
  }

  $shipping_product = commerce_bpost_get_shipping_product($shipping_line_item->commerce_shipping_service->value(), $country_code, $options['return']);

  $xml = new XMLWriter();
  $xml->openMemory();
  $xml->startDocument('1.0', 'UTF-8');
  $xml->startElement('tns:order');
  $xml->writeAttribute('xmlns', 'http://schema.post.be/shm/deepintegration/v3/national');
  $xml->writeAttribute('xmlns:common', 'http://schema.post.be/shm/deepintegration/v3/common');
  $xml->writeAttribute('xmlns:tns', 'http://schema.post.be/shm/deepintegration/v3/');
  $xml->writeAttribute('xmlns:international', 'http://schema.post.be/shm/deepintegration/v3/international');
  $xml->writeAttribute('xmlns:xsi', 'http://www.w3.org/2001/XMLSchema-instance');
  $xml->writeAttribute('xsi:schemaLocation', 'http://schema.post.be/shm/deepintegration/v3/');

  $xml->writeElement('tns:accountId', $commerce_bpost_settings['account']['account_id']);
  $xml->writeElement('tns:reference', _commerce_bpost_get_bpost_id($order));
  $xml->writeElement('tns:costCenter', '');

  // Line items
  foreach ($order_wrapper->$line_item_field as $line_item) {
    if ($line_item->getBundle() == 'product') {
      $xml->startElement('tns:orderLine');
      $xml->writeElement('tns:text', substr($line_item->commerce_product->title->value(), 0, 50));
      $xml->writeElement('tns:nbOfItems', (int) $line_item->quantity->value());
      $xml->endElement();
    }
  }

  // Customer shipping address & service
  $shipping_service_name = $shipping_line_item->commerce_shipping_service->value();
  if ($options['return'] && $shipping_service_name != 'bpost_world') {
    $shipping_service_name = 'bpost_home';
  }

  // Merchant shipping address
  $xml->startElement('tns:box');
  $xml->startElement('tns:sender');
  $sender_address_rebuilt = _commerce_bpost_replace_address_fields($sender_address, !$options['return'], $shipping_service_name);
  $xml->writeElement('common:name', $sender_address_rebuilt['name']);
  if (!empty($sender_address_rebuilt['company'])) {
    $xml->writeElement('common:company', $sender_address_rebuilt['company']);
  }
  $xml->startElement('common:address');
  $xml->writeElement('common:streetName', $sender_address_rebuilt['streetName']);
  $xml->writeElement('common:number', substr($sender_address_rebuilt['number'], 0, 8));
  if (!empty($sender_address['box'])) {
    $xml->writeElement('common:box', $sender_address['box']);
  }
  $xml->writeElement('common:postalCode', $sender_address['postalCode']);
  $xml->writeElement('common:locality', $sender_address['locality']);
  $xml->writeElement('common:countryCode', $sender_address['countryCode']);
  $xml->endElement();
  if (!empty($sender_address['mail'])) {
    $xml->writeElement('common:emailAddress', $sender_address['mail']);
  }
  if (!empty($sender_address['phoneNumber'])) {
    $xml->writeElement('common:phoneNumber', $sender_address['phoneNumber']);
  }
  $xml->endElement();

  // Delivery date & Saturday delivery
  $send_saturday_delivery = FALSE;
  if (!$options['return'] && module_load_include('inc', 'commerce_bpost', 'commerce_bpost.commerce') &&
      module_load_include('inc', 'commerce_bpost', 'includes/commerce_bpost.admin')) {
    $delivery_date = commerce_bpost_load_delivery_date($order);
    $delivery_date_formatted = isset($delivery_date) ? $delivery_date->format('Y-m-d') : NULL;
    // Send Saturday delivery in XML if:
    // - 'saturday delivery' is checked in back-end settings
    // - AND ( delivery date is not displayed in the front-end OR delivery date displayed/selected in the order is effectively a saturday )
    $send_saturday_delivery = commerce_bpost_is_saturday_delivery_enabled($shipping_service_name) &&
        (empty($commerce_bpost_settings['options']['delivery_date']['display']) || ($delivery_date instanceof DateTime && commerce_bpost_is_saturday($delivery_date)));
  }

  switch ($shipping_service_name) {
    case COMMERCE_BPOST_SERVICE_HOME:
      $xml->startElement('tns:nationalBox');
      $xml->startElement('atHome');
      $xml->writeElement('product', $shipping_product);
      // box > nationalBox > atHome > options
      $xml->startElement('options');
      if (!$options['return'] && !empty($commerce_bpost_settings['options']['bpost_home']['second_presentation'])) {
        $xml->writeElement('common:automaticSecondPresentation', '');
      }
      if (!$options['return'] && !empty($commerce_bpost_settings['options']['bpost_home']['insurance'])) {
        $xml->startElement('common:insured');
        $xml->writeElement('common:basicInsurance', '');
        $xml->endElement();
      }
      elseif (!$options['return'] && !empty($commerce_bpost_settings['options']['bpost_home']['signature'])) {
        $xml->writeElement('common:signed', '');
      }
      if ($send_saturday_delivery) {
        $xml->writeElement('common:saturdayDelivery');
      }
      $xml->endElement();

      // box > nationalBox > atHome > weight
      $xml->writeElement('weight', $weight['weight']);

      // box > nationalBox > atHome > receiver
      $xml->startElement('receiver');
      $customer_address_rebuilt = _commerce_bpost_replace_address_fields($customer_address, $options['return'], $shipping_service_name);
      $xml->writeElement('common:name', $customer_address_rebuilt['name'] ? $customer_address_rebuilt['name'] : '.');
      $xml->writeElement('common:company', $customer_address_rebuilt['company']);
      $xml->startElement('common:address');
      $xml->writeElement('common:streetName', $customer_address_rebuilt['streetName']);
      if (!empty($customer_address_rebuilt['number'])) {
        $xml->writeElement('common:number', $customer_address_rebuilt['number']);
      }
      if (!empty($customer_address['box'])) {
        $xml->writeElement('common:box', $customer_address['box']);
      }
      $xml->writeElement('common:postalCode', $customer_address['postalCode']);
      $xml->writeElement('common:locality', $customer_address['locality']);
      $xml->writeElement('common:countryCode', $customer_address['countryCode']);
      $xml->endElement();
      if ($customer_address['mail']) {
        $xml->writeElement('common:emailAddress', $customer_address['mail']);
      }
      if (!empty($customer_address['phoneNumber'])) {
        $xml->writeElement('common:phoneNumber', $customer_address['phoneNumber']);
      }
      $xml->endElement();

      if (!empty($delivery_date_formatted)) {
        // box > nationalBox > atHome > requestedDeliveryDate
        $xml->writeElement('requestedDeliveryDate', $delivery_date_formatted);
      }
      $xml->endElement();
      $xml->endElement();
      break;

    case COMMERCE_BPOST_SERVICE_WORLD:
      $xml->startElement('tns:internationalBox');
      $xml->startElement('international:international');
      $xml->writeElement('international:product', $shipping_product);
      if (!empty($commerce_bpost_settings['options']['bpost_world']['insurance_basic'])) {
        $xml->startElement('international:options');
        if (!$options['return'] && !empty($commerce_bpost_settings['options']['bpost_world']['insurance_basic'])) {
          $xml->startElement('common:insured');
          $xml->writeElement('common:basicInsurance', '');
          $xml->endElement();
        }
        $xml->endElement();
      }
      $xml->startElement('international:receiver');
      $customer_address_rebuilt = _commerce_bpost_replace_address_fields($customer_address, $options['return'], $shipping_service_name);
      $xml->writeElement('common:name', $customer_address_rebuilt['name'] ? $customer_address_rebuilt['name'] : '.');
      $xml->writeElement('common:company', $customer_address_rebuilt['company']);
      $xml->startElement('common:address');
      $xml->writeElement('common:streetName', $customer_address_rebuilt['streetName']);
      if (!empty($customer_address_rebuilt['number'])) {
        $xml->writeElement('common:number', $customer_address_rebuilt['number']);
      }
      if (!empty($customer_address['box'])) {
        $xml->writeElement('common:box', $customer_address['box']);
      }
      $xml->writeElement('common:postalCode', $customer_address['postalCode']);
      $xml->writeElement('common:locality', $customer_address['locality']);
      $xml->writeElement('common:countryCode', $customer_address['countryCode']);
      $xml->endElement();
      if (!empty($customer_address['mail'])) {
        $xml->writeElement('common:emailAddress', $customer_address['mail']);
      }
      if (!empty($customer_address['phoneNumber'])) {
        $xml->writeElement('common:phoneNumber', $customer_address['phoneNumber']);
      }
      $xml->endElement();
      $xml->writeElement('international:parcelWeight', $weight['weight']);
      if ($shipping_product != 'bpack Europe Business') {
        $shipment_type = array(
          'sample' => 'SAMPLE',
          'gift' => 'GIFT',
          'documents' => 'GOODS',
          'other' => 'OTHER'
        );
        $return_instructions = array(
          'rta' => 'RTA',
          'rts' => 'RTS',
          'abandoned' => 'ABANDONED',
        );
        $xml->startElement('international:customsInfo');
        // Get total including shipping cost.
        $order_total = $order_wrapper->$total_field->amount->value();
        // Get shipping cost.
        if (!empty($order_wrapper->$total_field->data->value()['components'])) {
          foreach ($order_wrapper->$total_field->data->value()['components'] as $total_component) {
            if ($total_component['name'] == 'shipping') {
              // Set total without shipping cost.
              $order_total = $order_total - $total_component['price']['amount'];
            }
          }
        }
        $xml->writeElement('international:parcelValue', $order_total);
        // Aggregate all product title to get a content description.
        $content_description = NULL;
        foreach ($order_wrapper->$line_item_field as $line_item) {
          if ($line_item->getBundle() == 'product') {
            $content_description .= $line_item->commerce_product->sku->value() . ",";
          }
        }
        if (!empty($content_description)) {
          $content_description = substr($content_description, 0, -1);
        }
        $xml->writeElement('international:contentDescription', substr($content_description, 0, 50));
        $xml->writeElement('international:shipmentType', $shipment_type['documents']);
        $xml->writeElement('international:parcelReturnInstructions', $return_instructions['rts']);
        $xml->writeElement('international:privateAddress', 'false');
        $xml->endElement();
      }
      $xml->endElement();
      $xml->endElement();
      break;

    case COMMERCE_BPOST_SERVICE_POSTPOINT:
      $xml->startElement('tns:nationalBox');
      $xml->startElement('atBpost');
      $xml->writeElement('product', $shipping_product);
      $xml->startElement('options');
      // tns:nationalBox > atBpost > options
      $xml->startElement('common:keepMeInformed');
      // tns:nationalBox > atBpost > options > common:keepMeInformed
      $xml->writeAttribute('language', commerce_bpost_get_current_language());
      if ($shipping_item_data->data['notification_message'] == "phone") {
        $xml->writeElement('common:mobilePhone', $customer_address['phoneNumber']);
      }
      else {
        $xml->writeElement('common:emailAddress', $customer_address['mail']);
      }
      $xml->endElement();

      if (!empty($commerce_bpost_settings['options']['bpost_postoffice_postpoint']['insurance'])) {
        $xml->startElement('common:insured');
        // tns:nationalBox > atBpost > options > common:insured
        $xml->writeElement('common:basicInsurance', '');
        $xml->endElement();
      }
      if ($send_saturday_delivery) {
        $xml->writeElement('common:saturdayDelivery');
      }
      $xml->endElement();

      // tns:nationalBox > atBpost > weight
      $xml->writeElement('weight', $weight['weight']);

      // Complete with webservice info.
      $xml->writeElement('pugoId', $point_id);
      $xml->writeElement('pugoName', $point_office);
      $xml->startElement('pugoAddress');
      $xml->writeElement('common:streetName', $point_street_name);
      $xml->writeElement('common:number', $point_number);
      $xml->writeElement('common:postalCode', $point_postal_code);
      $xml->writeElement('common:locality', $point_locality);
      $xml->writeElement('common:countryCode', 'BE');
      $xml->endElement();
      $xml->writeElement('receiverName', $customer_address['name']);
      $xml->writeElement('receiverCompany', $customer_address['company']);

      if (!empty($delivery_date_formatted)) {
        // box > nationalBox > atHome > requestedDeliveryDate
        $xml->writeElement('requestedDeliveryDate', $delivery_date_formatted);
      }

      $xml->endElement();
      $xml->endElement();
      break;

    case COMMERCE_BPOST_SERVICE_PARCEL_LOCKER:
      $xml->startElement('tns:nationalBox');
      $xml->startElement('at24-7');
      $xml->writeElement('product', $shipping_product);
      $xml->startElement('options');
      // tns:nationalBox > at24-7 > options
      if (!empty($commerce_bpost_settings['options']['bpost_bpack']['insurance'])) {
        $xml->startElement('common:insured');
        // tns:nationalBox > at24-7 > options > common:insured
        $xml->writeElement('common:basicInsurance');
        $xml->endElement();
      }
      if ($send_saturday_delivery) {
        $xml->writeElement('common:saturdayDelivery');
      }
      $xml->endElement();

      // tns:nationalBox > at24-7 > weight
      $xml->writeElement('weight', $weight['weight']);

      // Complete with webservice info.
      $xml->writeElement('parcelsDepotId', $point_id);
      $xml->writeElement('parcelsDepotName', $point_office);
      $xml->startElement('parcelsDepotAddress');
      // Use the parcel locker address data as first option.
      $xml->writeElement('common:streetName', $point_street_name);
      $xml->writeElement('common:number', $point_number);
      $xml->writeElement('common:postalCode', $point_postal_code);
      $xml->writeElement('common:locality', $point_locality);
      $xml->writeElement('common:countryCode', 'BE');
      $xml->endElement();

      $xml->startElement('unregistered');
      // 2 letters language ISO code (NL/FR/EN/DE)
      $xml->writeElement('language', $shipping_item_data->data['notification_language']);
      if (!empty($customer_address['phoneNumber'])) {
        $xml->writeElement('mobilePhone', $customer_address['phoneNumber']);
      }
      $xml->writeElement('emailAddress', $customer_address['mail']);
      if (!empty($shipping_item_data->data['reduced_mobility'])) {
        $xml->writeElement('reducedMobilityZone');
      }
      $xml->endElement();

      $xml->writeElement('receiverName', $customer_address['name']);
      $xml->writeElement('receiverCompany', $customer_address['company']);

      if (!empty($delivery_date_formatted)) {
        // box > nationalBox > atHome > requestedDeliveryDate
        $xml->writeElement('requestedDeliveryDate', $delivery_date_formatted);
      }

      $xml->endElement();
      $xml->endElement();
      break;
    default:
      watchdog('commerce_bpost', 'Webservice > createOrder method: shipping service do not match. Nothing has been sent.', array(), WATCHDOG_ERROR);
      return FALSE;
  }

  $drupal_version = constant('VERSION');
  if (!empty($drupal_version)) {
    $xml->writeElement('tns:additionalCustomerReference', 'Drupal_v' . $drupal_version);
  }

  // End box element.
  $xml->endElement();

  $xml->endDocument();
  $xml = $xml->outputMemory();

  // Call the webservice
  $request_options = array(
    'method' => 'POST',
    'data' => $xml,
    'headers' => array(
      'Content-Type' => 'application/vnd.bpost.shm-order-v3.3+XML',
    ),
  );
  watchdog('commerce_bpost', 'Webservice > createOrder method: request for order n°!order_id.', array(
    '!order_id' => $order_wrapper->getIdentifier()));
  // watchdog('commerce_bpost', 'Bpost order creation request: <pre>!request</pre>', array('!request' => print_r(htmlspecialchars($request_options['data']), TRUE)), WATCHDOG_DEBUG);
  $response = commerce_bpost_request_bpost('orders', $request_options, $commerce_bpost_settings['account']['api_url'] . $commerce_bpost_settings['account']['account_id'] . '/');

  if ($response->code == 201) {
    watchdog('commerce_bpost', 'Webservice > createOrder method: success.');
    if ($options['send_only']) {
      $order->type = 'commerce_order';
      return TRUE;
    }

    if (isset($response->headers['location'])) {
      preg_match('#' . $commerce_bpost_settings['account']['api_url'] . '\d+/orders/(\w+)#', $response->headers['location'], $matches);
      if (isset($matches[1])) {
        _commerce_bpost_set_bpost_id($order, $matches[1]);
      }
    }
    return TRUE;
  }
  else {
    $error = simplexml_load_string($response->data);
    if (isset($error->message)) {
      $error_msg = (string) $error->message;
    }
    else {
      $error_msg = $response->data;
    }
    drupal_set_message(t('An error occured while sending order n°@order_id to bpost: @error_msg', array(
      '@order_id' => $order_wrapper->getIdentifier(), '@error_msg' => $error_msg)), 'error');
    watchdog('commerce_bpost', 'Webservice > createOrder method: error !code > @error_msg.', array('!code' => $response->code,));
  }

  return FALSE;
}

function commerce_bpost_webservice_cancel_order($order) {
  $commerce_bpost_settings = variable_get('commerce_bpost_settings');
  if (!empty($commerce_bpost_settings['mapping']['canceling_status'])) {
    $order->status = $commerce_bpost_settings['mapping']['canceling_status'];
  }
  if (isset($commerce_bpost_settings['account']['account_id'])) {
    $xml = new XMLWriter();
    $xml->openMemory();
    $xml->startDocument('1.0', 'UTF-8');
    $xml->startElement('orderUpdate');
    $xml->writeAttribute('xmlns', 'http://schema.post.be/shm/deepintegration/v3/');
    $xml->writeAttribute('xmlns:xsi', 'http://www.w3.org/2001/XMLSchema-instance');
    $xml->writeAttribute('xsi:schemaLocation', 'http://schema.post.be/shm/deepintegration/v3/');
    $xml->writeElement('status', 'CANCELLED');
    $xml->endElement();
    $xml = $xml->outputMemory();
    $options = array(
      'method' => 'POST',
      'data' => $xml,
      'headers' => array(
        'Content-Type' => 'application/vnd.bpost.shm-orderUpdate-v3+XML',
      ),
    );
    $bpost_id = _commerce_bpost_get_bpost_id($order);
    watchdog('commerce_bpost', 'Webservice > cancelOrder method: request for order n°!order_id.', array(
      '!order_id' => $order->order_id));
    $response = commerce_bpost_request_bpost('orders/' . $bpost_id, $options, $commerce_bpost_settings['account']['api_url'] . $commerce_bpost_settings['account']['account_id'] . '/');
    if ($response->code == 200) {
      watchdog('commerce_bpost', 'Webservice > cancelOrder method: order n°@order_id cancelled.', array(
        '@order_id' => $order->order_id));
      drupal_set_message(t('Order @order_id cancelled', array('@order_id' => $order->order_id)));
    }
    else {
      watchdog('commerce_bpost', 'Webservice > cancelOrder method: Error while cancelling order n°!order_id.', array(
        '!order_id' => $order->order_id));
      drupal_set_message(t('Error while cancelling order @order_id', array('@order_id' => $order->order_id)), 'error');
    }
  }
}

/**
 * Get nearest service points of an address thanks to geo6 webservice.
 * @param $address
 * @param $type
 * @param integer $delivery_ts The delivery timestamp for which the service point must be open.
 */
function commerce_bpost_webservice_get_nearest_service_points($address, $type, $delivery_ts = NULL) {
  global $language;
  $point_list = array();
  // Settings.
  $get = array(
    'Function' => 'search',
    'Partner' => '999999',
    'AddId' => 'A001',
  );

  // Language.
  if ($language->language == "FR" || $language->language == "NL") {
    $get['Language'] = $language->language;
  }
  else {
    $get['Language'] = 'NL';
  }

  // Address.
  if (!is_array($address)) {
    $get['Zone'] = $address;
  }
  else {
    if (!empty($address['postal_code'])) {
      $get['Zone'] = $address['postal_code'];
    }
    elseif (!empty($address['city'])) {
      $get['Zone'] = $address['city'];
    }
    elseif (!empty($address['zone'])) {
      $get['Zone'] = $address['zone'];
    }
  }
  //$get .= '&Number=';
  // Postoffice = 1, Post point = 2, Postoffice-PostPoint = 3, Bpack = 4.
  $get['Type'] = $type;
  $get['Limit'] = 10;

  module_load_include('inc', 'commerce_bpost', 'includes/commerce_bpost.admin');
  // For parcel locker and saturday delivery, check points are opened.
  if ($type == 3 && !is_null($delivery_ts) && commerce_bpost_is_saturday_delivery_enabled('bpost_postoffice_postpoint')) {
    $get['CheckDate'] = 1;
    $get['CheckOpen'] = 1;
    $get['DD'] = date('d-m-Y', $delivery_ts);
  }

  $options = array(
    'method' => 'GET',
    'headers' => array(
      'Content-Type' => 'text/xml; charset=utf-8',
    ),
  );

  $get = drupal_http_build_query($get);

  watchdog('commerce_bpost', 'Webservice > get_nearest_service_points method: request sent.');
  $response = commerce_bpost_request_bpost($get, $options, 'http://taxipost.geo6.be/Locator?');
  watchdog('commerce_bpost', 'Webservice > get_nearest_service_points method: reply with code !code.', array(
    '!code' => $response->code));
  if ($response->code == 200) {
    $responsetoarray = json_decode(json_encode((array) simplexml_load_string($response->data)), 1);
    if (!empty($responsetoarray['PoiList'])) {
      foreach ($responsetoarray['PoiList']['Poi'] as $point) {
        $point_list[] = $point;
      }
    }
  }
  return $point_list;
}

function commerce_bpost_get_point_details($id, $type) {
  $responsetoarray = array();
  global $language;
  $get = 'Function=info';
  $get .= '&Partner=999998';
  $get .= '&AddId=A001';
  $get .= '&Id=' . $id;
  $get .= '&Type=' . $type;
  // Language.
  $get .= '&Language=';
  if ($language->language == "FR" || $language->language == "NL") {
    $get .= $language->language;
  }
  else {
    $get .= 'NL';
  }
  $options = array(
    'method' => 'GET',
    'headers' => array(
      'Content-Type' => 'text/xml; charset=utf-8',
    ),
  );
  watchdog('commerce_bpost', 'Webservice > get_point_details method: request sent for point !point_id.', array(
    '!point_id' => $id));
  $response = commerce_bpost_request_bpost($get, $options, 'http://taxipost.geo6.be/Locator?');
  watchdog('commerce_bpost', 'Webservice > get_point_details method: reply with code !code.', array(
    '!code' => $response->code));
  if ($response->code == 200) {
    $responsetoarray = json_decode(json_encode((array) simplexml_load_string($response->data)), 1);
  }
  return $responsetoarray;
}

/**
 * Get the bpost label for a specific order.
 *
 * @param $order_id
 *  The order we want to get the label from.
 *
 * @return bool
 */
function commerce_bpost_create_label($order, $entity_type = 'commerce_order') {
  $order_wrapper = entity_metadata_wrapper($entity_type, $order);
  $order_id = $order_wrapper->getIdentifier();
  $bpost_id = _commerce_bpost_get_bpost_id($order);
  $commerce_bpost_settings = variable_get('commerce_bpost_settings');
  $get = "/" . $commerce_bpost_settings['account']['account_id'] . "/orders/" . $bpost_id . "/labels/" . $commerce_bpost_settings['label']['format'];
  if (isset($commerce_bpost_settings['label']['return']) && $commerce_bpost_settings['label']['return']
      == 'include') {
    $get .= '/withReturnLabels';
  }
  $options = array(
    'method' => 'GET',
    'headers' => array(
      'Accept' => 'application/vnd.bpost.shm-label-pdf-v3+XML',
    ),
  );
  
  watchdog('commerce_bpost', 'Webservice > createLabel method: request for order n°!order_id.', array(
    '!order_id' => $order_id));
  $response = commerce_bpost_request_bpost($get, $options, $commerce_bpost_settings['account']['api_url']);
  watchdog('commerce_bpost', 'Webservice > createLabel method: reply with code !code.', array('!code' => $response->code));

  $barcode_success = FALSE;
  $file_success = FALSE;
  if ($response->code == 200) {
    $data = simplexml_load_string($response->data);

    $barcodes = $order_wrapper->field_barcode->value();
    $labels = $order_wrapper->field_bpost_label->value();
    foreach ($data->label as $label) {
      $has_return_barcode = FALSE;
      $has_shipment_barcode = FALSE;
      // Barcode
      if (isset($label->barcode)) {
        foreach ($label->barcode as $barcode) {
          $barcodes[] = (string) $barcode;
          if (_commerce_bpost_is_return_barcode($order_wrapper, $barcode)) {
            $has_return_barcode = TRUE;
          }
          else {
            $has_shipment_barcode = TRUE;
          }
        }
        $barcode_success = TRUE;
      }
      // Label
      if (isset($label->bytes)) {
        $file_path = 'private://bpost_labels';
        if (!$has_shipment_barcode && $has_return_barcode) {
          $file_name = 'label_return_order_' . $order_id . '.pdf';
        }
        elseif ($has_shipment_barcode && $has_return_barcode) {
          $file_name = 'label_order_with_return_' . $order_id . '.pdf';
        }
        else {
          $file_name = 'label_order_' . $order_id . '.pdf';
        }
        if (file_prepare_directory($file_path, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS)) {
          $file_content_64 = (string) $label->bytes;
          $file = file_save_data(base64_decode($file_content_64), $file_path . '/' . $file_name);
          if (isset($file->fid)) {
            $file->display = 0;
            $labels[] = (array) $file;
            $order->status = 'bpost_printed';
            $file_success = TRUE;
          }
        }
      }
    }
    $order_wrapper->field_barcode->set($barcodes);
    $order_wrapper->field_bpost_label->set($labels);
  }
  return $barcode_success || $file_success;
}

/**
 * Generate bpost tracking urls for a specific order.
 *
 * @param $order_id
 *  The order we want to generate tracking urls for.
 *
 * @return string|null
 */
function commerce_bpost_get_order_tracking_urls($order_id, $entity_type = 'commerce_order') {
  $order_wrapper = entity_metadata_wrapper($entity_type, $order_id);
  $barrcodes = $order_wrapper->field_barcode->value();

  $output = NULL;

  foreach ($barrcodes as $barrcode) {
    // Do not output return barcodes.
    if (!_commerce_bpost_is_return_barcode($order_wrapper, $barrcode)) {
      $output .= commerce_bpost_get_tracking_url($barrcode) . '<br />';
    }
  }

  return $output;
}

/**
 * Generate tracking url for a specific barrcode.
 *
 * @param $barrcode
 *   Barrcode to generate url for.
 *
 * @return string
 *   Formated tracking url using l()
 */
function commerce_bpost_get_tracking_url($barrcode) {
  $query = drupal_http_build_query(array(
    'searchByItemCode' => 'true',
    'itemCodes' => $barrcode,
    'oss_language' => 'en',
  ));

  $url = 'http://track.bpost.be/etr/light/performSearch.do?' . $query;
  return t('Parcel n°') . l($barrcode, $url, array('external' => TRUE, 'attributes' => array('target' => '_blank')));
}

/**
 * Get Shop info.
 */
function commerce_bpost_get_shop_info() {
  if ($data = cache_get('commerce_bpost_shopinfo')) {
    $data = simplexml_load_string($data->data);
  }
  else {
    $commerce_bpost_settings = variable_get('commerce_bpost_settings');
    watchdog('commerce_bpost', 'Webservice > getShopInfo method');
    $options = array(
      'headers' => array(
        'Accept' => 'application/vnd.bpost.shm-productConfiguration-v3+XML',
      ),
    );
    $response = commerce_bpost_request_bpost($commerce_bpost_settings['account']['account_id'] . '/productconfig', $options);
    watchdog('commerce_bpost', 'Webservice > getShopInfo method: reply with code !code.', array('!code' => $response->code));
    if ($response->code == 200) {
      $data = simplexml_load_string($response->data);
      cache_set('commerce_bpost_shopinfo', $response->data, 'cache', strtotime('now +1 hour'));
    }
    else {
      $data = FALSE;
    }
  }
  return $data;
}

/**
 * Return the bpost shipping product for a given service and country.
 *
 * @param string $service
 *  Shipping service machine name (Drupal side).
 * @param $country
 *  The user shipping address country.
 * @param bool $return
 *  Whether its a return or not.
 *
 * @return string
 *  The bpost shipping product
 */
function commerce_bpost_get_shipping_product($service, $country, $return = FALSE) {
  $commerce_bpost_settings = variable_get('commerce_bpost_settings');
  $international = !empty($commerce_bpost_settings['options']['international_delivery']) ? $commerce_bpost_settings['options']['international_delivery'] : 'bpack World Business';
  $default_shipping_products = array(
    'delivery' => array(
      'bpost_home' => 'bpack 24h Pro',
      'bpost_bpack' => 'bpack 24h Pro',
      'bpost_postoffice_postpoint' => 'bpack@bpost',
      'bpost_world' => $international,
    ),
    'return' => array(
      'bpost_home' => 'bpack Easy Retour',
      'bpost_bpack' => 'bpack Easy Retour',
      'bpost_postoffice_postpoint' => 'bpack Easy Retour',
      'bpost_world' => 'bpack World Easy Return',
    )
  );
  $shipping_product = NULL;
  $shipping_product_key = $return ? 'return' : 'delivery';
  if (!$return) {
    $data = commerce_bpost_get_shop_info();
    $matching_products = array();
    if (!empty($data)) {
      $mapping_services = array(
        'bpost_bpack' => 'Parcel machine',
        'bpost_postoffice_postpoint' => 'pick-up point',
        'bpost_home' => 'home or office',
        'bpost_world' => 'home or office',
      );
      foreach ($data->deliveryMethod as $deliveryMethod) {
        foreach ($deliveryMethod->attributes() as $key => $value) {
          if (!($key == "name" && ((string) $value) == $mapping_services[$service])) {
            continue 2;
          }
        }
        foreach ($deliveryMethod->product as $product) {
          foreach ($product->attributes() as $key => $value) {
            if ($key == "name") {
              $matching_product = (string) $value;
              break;
            }
          }
          if (!empty($matching_product)) {
            foreach ($product->price as $price) {
              foreach ($price->attributes() as $key => $value) {
                if ($key == "countryIso2Code" && ((string) $value == $country)) {
                  $matching_products[] = $matching_product;
                }
              }
            }
          }
        }
      }
    }
    $matching_products = array_diff($matching_products, $default_shipping_products['return']);
    if (!empty($matching_products)) {
      if ($value = array_intersect($matching_products, $default_shipping_products[$shipping_product_key])) {
        $shipping_product = reset($value);
      }
      else {
        $shipping_product = reset($matching_products);
      }
    }
  }
  if (empty($shipping_product)) {
    $shipping_product = $default_shipping_products[$shipping_product_key][$service];
  }

  return $shipping_product;
}

/**
 * Get bpost order id.
 *
 * @param $order
 *   Order to generate bpost id for
 *
 * @return string
 *   Generated bpost order id
 */
function _commerce_bpost_get_bpost_id($order) {
  if (!empty($order->data['bpost_id'])) {
    return $order->data['bpost_id'];
  }

  return 'DRU_1_' . $order->order_id;
}

/**
 * Set id provided by bpost in the order
 *
 * @param $order
 *   Order to set the id for
 * @param $bpost_id
 *   Id to be setted
 */
function _commerce_bpost_set_bpost_id(&$order, $bpost_id) {
  $order->data['bpost_id'] = $bpost_id;
}

/**
 * Switch and rearrange some address fields.
 *
 * @param array $address
 * Customer address
 *
 * @param bool $return
 * Does order is a return ?
 *
 * @return array
 */
function _commerce_bpost_replace_address_fields($address, $return, $shipping_service = NULL) {
  $new_address = $address;

  if (!$return) {
    // If it's not a return, there is no numbers so switch fields.
    switch ($shipping_service) {
      case COMMERCE_BPOST_SERVICE_HOME:
      case COMMERCE_BPOST_SERVICE_WORLD:
        // Generate company address field based on issue #15994 comment #12.
        if (empty($address['number'])) {
          $new_address_company = $address['company'];
        }
        else {
          $new_address_company = $address['name'];
          if (!empty($address['company'])) {
            $new_address_company .= ' (' . $address['company'] . ')';
          }
        }

        // Replace address fields.
        $new_address = array(
          'company' => $new_address_company,
          'name' => empty($address['number']) ? $address['name'] : $address['number'],
          'streetName' => $address['streetName'],
          'number' => ',',
        );
        break;

      case COMMERCE_BPOST_SERVICE_POSTPOINT:
      case COMMERCE_BPOST_SERVICE_PARCEL_LOCKER:
        // Do not change the default field mapping
        break;
    }
  }
  else {
    // If it's a return, just keep address fields and be sure to have short number.
    $new_address['number'] = empty($address['number']) ? ',' : substr($address['number'], 0, 8);
  }
  return $new_address;
}

function commerce_bpost_request_order_status($order) {
  $commerce_bpost_settings = variable_get('commerce_bpost_settings');
  $account_id = $commerce_bpost_settings['account']['account_id'];
  $bpost_order_id = _commerce_bpost_get_bpost_id($order);
  $options = array(
    'headers' => array(
      'Accept' => 'application/vnd.bpost.shm-order-v3.3+XML',
    ),
  );
  return commerce_bpost_request_bpost($account_id . '/orders/' . $bpost_order_id, $options);
}

/**
 * Define if barcode is a return barcode.
 *
 * @param Object $order_wrapper
 * Order wrapper with shipping line item.
 * @param string $barcode
 * Barcode to check.
 *
 * @return bool
 */
function _commerce_bpost_is_return_barcode($order_wrapper, $barcode) {
  foreach($order_wrapper->commerce_line_items as $line_item_wrapper) {
    if ($line_item_wrapper->getBundle() == 'shipping') {
      $shipping_service = $line_item_wrapper->commerce_shipping_service->value();
    }
  }
  $return_barcode_suffixes = array(
    '050' => '050',
    '051' => '051',
    '125' => '125',
    '134' => '134',
  );

  // For bpost_world, return barcode starts with 3232 (defined in #15973).
  if ($shipping_service === COMMERCE_BPOST_SERVICE_WORLD) {
    return substr($barcode, 0, 4) == '3232' && isset($return_barcode_suffixes[substr($barcode, -3)]);
  }

  // For Belgium shipping, return barcode end with 050, 051, 125 or 134.
  return isset($return_barcode_suffixes[substr($barcode, -3)]);
}