<?php

/**
 * This function send a request to ADS Webservice.
 *
 * @param string $service
 *   The service we ask to ads
 * @param array $options
 *   The options for drupal_http_request second arguments.
 *
 * @return array
 */
function commerce_ads_request_ads($service, $options = array()) {
  // Get the default API call options.
  $default_options = commerce_ads_get_default_request_options();
  // Allow to override the default options with call-specific options.
  $options = array_replace_recursive($default_options, $options);

  // In one specific situation we will get 'mode' from the options, instead of
  // the variable (as it works by default) - this is when generic ADS module
  // configuration is being saved in the BO - the variable might not be set yet
  // (or the settings might just have changes), and we need to test the
  // connectivity based on the new settings, which temporarily are stored only
  // in the $form_state (see commerce_ads_configure_form_validate()).
  if (!empty($options['mode'])) {
    $base_url = commerce_ads_get_ads_base_url($options['mode']);
    unset($options['mode']);
  }
  else {
    $base_url = commerce_ads_get_ads_base_url();
  }
  $url = $base_url . $service;

  // 'Authorization' header should start with 'Basic ' string.
  if (
    !empty($options['headers']['Authorization'])
    && strpos($options['headers']['Authorization'], 'Basic ') !== 0
  ) {
    $options['headers']['Authorization'] = 'Basic ' . $options['headers']['Authorization'];
  }

  $response = drupal_http_request($url, $options);

  if (!empty($response->data)) {
    $response = json_decode($response->data, TRUE);

    // Log the API response if required.
    $commerce_ads_ws = variable_get('commerce_ads_ws', array());
    if (!empty($commerce_ads_ws['ads_debug']['log_api_responses'])) {
      watchdog('commerce_ads', '@service response: !response', array(
        '@service' => str_replace('api/', '', $service),
        '!response' => '<pre>' . var_export($response, TRUE) . '</pre>',
      ), WATCHDOG_DEBUG);
    }
  }
  elseif (!empty($response->error)) {
    watchdog('commerce_ads', '@service error: @error_message. Full response: !response', array(
      '@service' => str_replace('api/', '', $service),
      '@error_message' => $response->error,
      '!response' => '<pre>' . var_export($response, TRUE) . '</pre>',
    ), array(), WATCHDOG_ERROR);
  }
  return $response;
}

/**
 * Get base url for making calls with ADS WS.
 *
 * @param string $mode
 *   A mode to return the base url for.
 *
 * @return string
 *  Return the base url.
 */
function commerce_ads_get_ads_base_url($mode = '') {
  if (empty($mode)) {
    $commerce_ads_ws = variable_get('commerce_ads_ws');
    if (!empty($commerce_ads_ws['ads_ws_mode'])) {
      $mode = $commerce_ads_ws['ads_ws_mode'];
    }
  }
  if (!empty($mode) && $mode == COMMERCE_ADS_WS_MODE_LIVE) {
    return COMMERCE_ADS_WS_URL_LIVE;
  }
  else {
    return COMMERCE_ADS_WS_URL_TEST;
  }
}

/**
 * Returns default option array for ADS API call.
 *
 * @return array
 *   An array of default options for ADS API call.
 */
function commerce_ads_get_default_request_options() {
  $commerce_ads_ws = variable_get('commerce_ads_ws', array());
  return array(
    'method' => 'GET',
    'timeout' => 6000,
    'headers' => array(
      'Authorization' => !empty($commerce_ads_ws['ads_authorization_key']) ? $commerce_ads_ws['ads_authorization_key'] : '',
    ),
  );
}

/**
 * Send product(s) to ads.
 *
 * @param array $products
 *  Array of product objects to be sent to ADS.
 */
function commerce_ads_send_products($products = array()) {
  $commerce_ads_ws = variable_get('commerce_ads_ws', array());
  if (!empty($products) && !empty($commerce_ads_ws['ads_product_mapping']) && commerce_ads_check_login_ws()) {
    $return = array();
    $product_update_json = array();
    $product_create_json = array();
    foreach ($products as $product) {
      $product = entity_metadata_wrapper('commerce_product', $product);
      // Size.
      $dimension = commerce_ads_get_field_value('Size (Physical)', $product, $commerce_ads_ws['ads_product_mapping'], 'commerce_product');
      $weight = commerce_ads_get_field_value('Size (Weight)', $product, $commerce_ads_ws['ads_product_mapping'], 'commerce_product');
      $size_array = array();
      if (!empty($dimension)) {
        $size_array['Height'] = (int) $dimension['height'];
        $size_array['Length'] = (int) $dimension['length'];
        $size_array['Width'] = (int) $dimension['width'];
        $size_array['Volume'] = 0;
      }
      if (!empty($weight)) {
        $size_array['Weight'] = (int) $weight['weight'];
      }

      // Translations.
      $translation_array = array();
      foreach (commerce_ads_get_product_translation($product) as $langcode => $description) {
        $translation_array[] = array(
          'Label' => substr($description, 0, 100),
          'Language' => $langcode,
        );
      }

      // EAN.
      if (!$ean = commerce_ads_get_field_value('EanEntity', $product, $commerce_ads_ws['ads_product_mapping'], 'commerce_product')) {
        $ean = $product->sku->value();
      }

      $product_to_send = array(
        'ProductId' => substr($product->sku->value(), 0, 18),
        'Type' => substr($product->type->value(), 0, 30),
        'Picture' => substr(commerce_ads_get_field_value('Picture', $product, $commerce_ads_ws['ads_product_mapping'], 'commerce_product'), 0, 250),
        'Size' => $size_array,
        'Translations' => $translation_array,
        'Eans' => array(
          array(
            'Ean13' => substr($ean, 0, 32),
          )
        ),
      );

      drupal_alter('commerce_ads_product', $product_to_send);

      if ($product->commerce_ads_product_needs_update->value() == PRODUCT_TO_BE_UPDATED) {
        $product_update_json[] = $product_to_send;
      }
      elseif ($product->commerce_ads_product_needs_update->value() == PRODUCT_TO_BE_CREATED) {
        $product_create_json[] = $product_to_send;
      }
      $return[$product->getIdentifier()] = PRODUCT_SENT;
    }

    if (!empty($product_update_json)) {
      $options = array(
        'method' => 'POST',
        'data' => drupal_json_encode($product_update_json),
        'headers' => array(
          'Content-Type' => 'application/json',
          'Accept' => 'application/json',
        ),
      );
      commerce_ads_request_ads('api/items/updateMultiple', $options);
    }
    if (!empty($product_create_json)) {
      $options = array(
        'method' => 'POST',
        'data' => drupal_json_encode($product_create_json),
        'headers' => array(
          'Content-Type' => 'application/json',
          'Accept' => 'application/json',
        ),
      );
      commerce_ads_request_ads('api/items/createMultiple', $options);
    }
    return $return;
  }
}

/**
 * Send order(s) to ads.
 *
 * @param array $orders
 *  Array of order object to be sent to ADS.
 * @param string $order_type
 *  The type of order on ADS side (SHOP, PROD, ABO).
 *
 * @return array
 *  An array keyed by order id and valued by status (ORDER_SENT)
 */
function commerce_ads_send_order($orders = array(), $order_type = NULL) {
  $commerce_ads_ws = variable_get('commerce_ads_ws', array());
  if (!empty($orders) && !empty($commerce_ads_ws['ads_order_mapping']) && commerce_ads_check_login_ws()) {
    global $language;
    $return = array();
    $order_create_json = array();
    $order_update_json = array();
    foreach ($orders as $order) {
      $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
      if (isset($commerce_ads_ws['ads_order_update_end'])
      && in_array($order_wrapper->status->value(), $commerce_ads_ws['ads_order_update_end'])) {
        continue;
      }

      if (commerce_ads_order_needs_to_be_sent($order_wrapper, $commerce_ads_ws['ads_product_types'])) {
        $order_info = commerce_ads_get_order_info($order_wrapper);
        // Shipping.
        $shipping_fields = array('FirstName','LastName','CompanyName','AddressLine1','AddressLine2','AddressLine3','PostalCode','City','State','Country','MobilePhone','Phone','Floor','Apartment','Building','Digicode','PickupCode','PickupCountry');
        // Address.
        $customer_profile_type = 'commerce_customer_' . $commerce_ads_ws['ads_customer_profile_type'];
        $address_array = array();
        if (!empty($order_wrapper->$customer_profile_type)) {
          foreach ($shipping_fields as $shipping_field) {
            $address_array[$shipping_field] = commerce_ads_get_field_value($shipping_field, entity_metadata_wrapper('commerce_customer_profile', $order_wrapper->$customer_profile_type->value()), $commerce_ads_ws['ads_order_mapping'], 'commerce_customer_profile');
          }
          $address_array['Comment'] = substr(commerce_ads_get_field_value('Comment', entity_metadata_wrapper('commerce_customer_profile', $order_wrapper->$customer_profile_type->value()), $commerce_ads_ws['ads_order_mapping'], 'commerce_customer_profile'), 0, 40);
        }
        $address_array['Email'] = $order_wrapper->mail->value();

        // Product lines.
        $product_array = array();
        foreach ($order_info['products'] as $product) {
          $product_array[] = array(
            'Label' => $product['Label'],
            'ProductId' => $product['ProductId'],
            'Quantity' => (int) $product['Quantity'],
          );
        }
        $message = str_split(commerce_ads_get_field_value('GiftMessage', $order_wrapper, $commerce_ads_ws['ads_order_mapping'], 'commerce_order'), 50);
        $order_to_send = array(
          'ShopId' => !empty($commerce_ads_ws['ads_shopid']) ? $commerce_ads_ws['ads_shopid'] : '',
          'Type' => !empty($order_type) ? $order_type : 'SHOP',
          'Number' => $order_wrapper->order_id->value(),
          'Date' => date('d/m/Y', $order_wrapper->created->value()),
          'Language' => $language->language,
          'InvoiceUrl' => commerce_ads_get_field_value('InvoiceUrl', $order_wrapper, $commerce_ads_ws['ads_order_mapping'], 'commerce_order'),
          'Blocked' => commerce_ads_get_field_value('Blocked', $order_wrapper, $commerce_ads_ws['ads_order_mapping'], 'commerce_order'),
          'Gift' => commerce_ads_get_field_value('Gift', $order_wrapper, $commerce_ads_ws['ads_order_mapping'], 'commerce_order'),
          'GiftMessage' => array(
            'Line1' => isset($message[0]) ? $message[0] : '',
            'Line2' => isset($message[1]) ? $message[1] : '',
            'Line3' => isset($message[2]) ? $message[2] : '',
          ),
          'Shippings' => array(
            array(
              'Address' => $address_array,
              'DeliverymanId' => !empty($order_info['DeliverymanId']) ? $order_info['DeliverymanId'] : 'EXAPAQ',
              'ProductLines' => $product_array,
            ),
          ),
        );
        drupal_alter('commerce_ads_order', $order_to_send);
        if ($order->commerce_ads_order_needs_update == ORDER_TO_BE_UPDATED) {
          $order_update_json[] = $order_to_send;
        }
        elseif ($order->commerce_ads_order_needs_update == ORDER_TO_BE_CREATED) {
          $order_create_json[] = $order_to_send;
        }
        $return[$order->order_id] = ORDER_SENT;
      }
    }

    if (!empty($order_update_json)) {
      $options = array(
        'method' => 'POST',
        'data' => drupal_json_encode($order_update_json),
        'headers' => array(
          'Content-Type' => 'application/json',
          'Accept' => 'application/json',
        ),
      );
      commerce_ads_request_ads('api/customerOrders/updateMultiple', $options);
    }
    if (!empty($order_create_json)) {
      $options = array(
        'method' => 'POST',
        'data' => drupal_json_encode($order_create_json),
        'headers' => array(
          'Content-Type' => 'application/json',
          'Accept' => 'application/json',
        ),
      );
      commerce_ads_request_ads('api/customerOrders/createMultiple', $options);
    }
    return $return;
  }
}

/**
 * Update Drupal commerce_order with ADS info (tracking url, shipping status)
 *
 * @param $orders
 *  Array of order_id to be updated.
 */
function commerce_ads_update_order($orders = array()) {
  if (!empty($orders)) {
    foreach ($orders as $order_id) {
      $commerce_ads_ws = variable_get('commerce_ads_ws', array());
      $order_wrapper = entity_metadata_wrapper('commerce_order', $order_id);
      // Don't update order if it has reached a certain status.
      if (isset($commerce_ads_ws['ads_order_update_end'])
        && in_array($order_wrapper->status->value(), $commerce_ads_ws['ads_order_update_end'])) {
        continue;
      }
      $response = commerce_ads_request_ads('api/CustomerOrders/get?Number=' . $order_id);
      if ($response['Code'] === 0 && isset($commerce_ads_ws['ads_shipping_statuses'][$response['Data']['CustomerOrderState']['StateId']])) {
        $save = FALSE;
        if ($commerce_ads_ws['ads_shipping_statuses'][$response['Data']['CustomerOrderState']['StateId']] != $order_wrapper->status->value()) {
          $order_wrapper->status->set($commerce_ads_ws['ads_shipping_statuses'][$response['Data']['CustomerOrderState']['StateId']]);
          $save = TRUE;
        }
        if (!empty($response['Data']['Shippings'][0]['TrackingURL'])) {
          $order_wrapper->field_ads_tracking_url->set($response['Data']['Shippings'][0]['TrackingURL']);
          $save = TRUE;
        }
        if ($save) {
          commerce_order_save($order_wrapper->value());
        }
      }
    }
  }
}

/**
 * Update all ads orders that needs to be updated.
 */
function commerce_ads_update_all_orders() {
  $commerce_ads_ws = variable_get('commerce_ads_ws', array());

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'commerce_order')
    ->propertyCondition('commerce_ads_order_needs_update', array(ORDER_TO_BE_UPDATED, ORDER_SENT), 'IN');
  // Don't update orders that don't need it.
  if (isset($commerce_ads_ws['ads_order_update_end'])) {
    $query->propertyCondition('status', $commerce_ads_ws['ads_order_update_end'], 'NOT IN');
  }

  $result = $query->execute();
  if (isset($result['commerce_order'])) {
    commerce_ads_update_order(array_keys($result['commerce_order']));
  }
}

/**
 * Send all orders to ADS.
 */
function commerce_ads_send_all_orders() {
  $commerce_ads_ws = variable_get('commerce_ads_ws', array());
  if (!empty($commerce_ads_ws['ads_order_mapping'])) {
    // Get all orders that needs to be created / updated on ads.
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'commerce_order')
      ->propertyCondition('commerce_ads_order_needs_update', array(ORDER_TO_BE_CREATED, ORDER_TO_BE_UPDATED), 'IN');
    $result = $query->execute();
    if (isset($result['commerce_order'])) {
      $orders = commerce_order_load_multiple(array_keys($result['commerce_order']));
      // Send the order.
      $orders_sent = commerce_ads_send_order($orders);
      foreach ($orders_sent as $order_id => $value) {
        $order = $orders[$order_id];
        // Flag the order as sent.
        $order->commerce_ads_order_needs_update = $value;
        // Set a flag to bypass hook_commerce_order_update.
        $order->data['commerce_ads_send_all_orders'] = TRUE;
        commerce_order_save($order);
      }
    }
  }
  else {
    form_set_error('commerce_ads', t('You have to map order fields so it can be synchronize with ADS first.'));
  }
}

/**
 * Send all products to ADS.
 */
function commerce_ads_send_all_products() {
  $commerce_ads_ws = variable_get('commerce_ads_ws', array());
  if (!empty($commerce_ads_ws['ads_product_types'])) {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'commerce_product')
      ->propertyCondition('commerce_ads_product_needs_update', array(PRODUCT_TO_BE_UPDATED, PRODUCT_TO_BE_CREATED), 'IN')
      ->entityCondition('type', array_keys($commerce_ads_ws['ads_product_types']));
    $result = $query->execute();
    if (isset($result['commerce_product'])) {
      $products = commerce_product_load_multiple(array_keys($result['commerce_product']));
      // Send products to ads.
      $products_sent = commerce_ads_send_products($products);
      foreach ($products_sent as $product_id => $value) {
        $product = $products[$product_id];
        // Flag the order as sent.
        $product->commerce_ads_product_needs_update = $value;
        // Set a flag to bypass hook_commerce_order_update.
        $product->data['commerce_ads_send_all_products'] = TRUE;
        commerce_product_save($product);
      }
    }
  }
  else {
    form_set_error('commerce_ads', t('You have to select the product types which will be synchronize with ADS first.'));
  }
}

/**
 * Test that the ADS Webservice is reachable with the current configuration.
 *
 * @param string $url
 *  The url to request (with trailing slash).
 * @param string $authorization_key
 *  The authorization key to send in request header to test webservice login.
 *
 * @return bool
 *  Whether or not the webservice is reachable with those info.
 */
function commerce_ads_check_login_ws($mode = '', $authorization_key = '') {
  $commerce_ads_ws = variable_get('commerce_ads_ws', array());
  if (empty($authorization_key)) {
    if (!empty($commerce_ads_ws['ads_authorization_key'])) {
      $authorization_key = $commerce_ads_ws['ads_authorization_key'];
    }
  }

  $options = array(
    'method' => 'POST',
  );
  if (!empty($authorization_key)) {
    $options['headers']['Authorization'] = $authorization_key;
  }
  if (!empty($mode)) {
    $options['mode'] = $mode;
  }

  $response = commerce_ads_request_ads('api/Check/Login', $options);
  if (!is_object($response) && isset($response['Code']) && $response['Code'] === 0) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * Gets stock information for all products from the ADS Webservice.
 *
 * @param string $url
 *   The url to request (with trailing slash).
 * @param string $authorization_key
 *   The authorization key to send in request header to test webservice login.
 *
 * @return
 */
function commerce_ads_get_stock_for_all_products() {
  $response = commerce_ads_request_ads('api/items/getStockImage');

  if (
    !is_object($response)
    && isset($response['Code'])
    && $response['Code'] === 0
    && !empty($response['Data'])
  ) {
    // Key return array by ProductId.
    $return = array();
    foreach ($response['Data'] as $product_data) {
      $return[$product_data['ProductId']] = $product_data;
    }
    return $return;
  }
  else {
    return FALSE;
  }
}

/**
 * Cron task worker for updating stock of all products.
 *
 * @see commerce_ads_cron()
 */
function commerce_ads_update_stock_for_all_products() {
  // Get stock for all products from ADS.
  $stocks = commerce_ads_get_stock_for_all_products();

  // Load products from ProductIds received in stock information.
  $query = new EntityFieldQuery();
  $result = $query->entityCondition('entity_type', 'commerce_product')
    ->propertyCondition('sku', array_keys($stocks), 'IN')
    ->execute();

  // Set variable to mark batch update running. This will disable
  // parallel calls to ADS API while products are being updated.
  variable_set(COMMERCE_ADS_WS_PRODUCT_UPDATE_RUNNING, TRUE);

  // Update stock information for each product.
  if (!empty($result['commerce_product'])) {
    $products = commerce_product_load_multiple(array_keys($result['commerce_product']));
    if (!empty($products)) {
      foreach ($products as $product) {
        commerce_ads_update_product_stock($product, $stocks[$product->sku]);
      }
    }
  }

  // Delete variable marking batch product update as running.
  variable_del(COMMERCE_ADS_WS_PRODUCT_UPDATE_RUNNING);
}

/**
 * Fetches order delivery information from ADS.
 *
 * @param int $order_id
 *   The order ID for which to fetch the delivery information.
 *
 * @return array|bool
 *   An array containing the order delivery information, or FALSE if there was
 *   an error fetching the delivery information from ADS.
 */
function commerce_ads_get_order_delivery_info($order_id) {
  $response = commerce_ads_request_ads('api/CustomerOrders/getDeliveryInformations?Number=' . $order_id);

  if (
    isset($response['Code'])
    && $response['Code'] === 0
    && !empty($response['Data'])
  ) {
    return $response['Data'];
  }
  else {
    watchdog('commerce_ads', 'There was an error fetching delivery information for order @order_id from ADS. Full ADS response: !response', array(
      '@order_id' => $order_id,
      '!response' => '<pre>' . var_export($response, TRUE) . '</pre>',
    ), WATCHDOG_ERROR);
    return FALSE;
  }
}
