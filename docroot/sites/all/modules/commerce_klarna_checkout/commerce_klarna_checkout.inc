<?php

/**
 * @file
 * Includes functions related to the communication with Klarna Checkout.
 */

$klarna_path = DRUPAL_ROOT . '/' . libraries_get_path('klarna_checkout', FALSE);
require_once $klarna_path . '/Checkout.php';

if (libraries_get_path('Mobile-Detect', FALSE)) {
  $mobile_path = DRUPAL_ROOT . '/' . libraries_get_path('Mobile-Detect', FALSE);
  require_once $mobile_path . '/Mobile_Detect.php';
}

/**
 * Helper function that initiates the Klarna Checkout object.
 *
 * @param mixed $settings
 *   An array of settings.
 * @param mixed $order
 *   The Drupal Commerce order object
 *
 * @return mixed
 *   A Klarna Order object.
 */
function commerce_klarna_checkout_init($settings, $order) {
  // Normalize settings.
  $settings['estore_id']
    = isset($settings['estore_id']) ? $settings['estore_id'] : '';
  $settings['secret']
    = isset($settings['secret']) ? $settings['secret'] : '';
  $settings['live_mode']
    = isset($settings['live_mode']) ? $settings['live_mode'] : 0;
  $settings['layout']
    = isset($settings['layout']) ? $settings['layout'] : 'desktop';

  // Load the order metadata.
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  $cart = array();

  // Add line items to the invoice.
  foreach ($order_wrapper->commerce_line_items as $key => $line_item) {
    // Array to store the settings for this article.
    $article = array(
      'quantity'      => (int) $line_item->quantity->value(),
      'reference'     => '',
      'name'          => '',
      'unit_price'    => 0,
      'tax_rate'      => 0,
      'discount_rate' => 0,
    );

    // Set the SKU and title that will be sent to Klarna.
    if ($line_item->type->value() == 'product') {
      $reference = $line_item->commerce_product->value()->sku;
      $name = $line_item->commerce_product->value()->title;
    }
    elseif ($line_item->type->value() == 'fee') {
      $line_item_type = commerce_line_item_type_load($line_item->type->value());
      $reference = $line_item_type['name'];
      $name = $line_item->line_item_label->value();
    }
    elseif ($line_item->type->value() == 'shipping') {
      $line_item_type = commerce_line_item_type_load($line_item->type->value());
      $reference = $line_item_type['name'];
      $name = $line_item->line_item_label->value();
    }
    else {
      $line_item_type = commerce_line_item_type_load($line_item->type->value());
      $reference = $line_item_type['name'];
      $name = $line_item_type['name'];
    }

    $article['reference'] = $reference;
    $article['name'] = $name;
    // Load the price data.
    $price = $line_item->commerce_unit_price->value();
    $coupon_amount = 0;
    $discount_rate = 0;

    // If the Coupon module is loaded, check for coupon price components.
    if (function_exists('commerce_coupon_get_types')) {
      $coupon_types = commerce_coupon_get_types();
      $coupon_components = array();
      foreach ($price['data']['components'] as $component) {
        // Looking for components that match one of the
        // defined coupon components.
        $component_type = commerce_price_component_type_load($component['name']);
        if (array_key_exists('coupon_type', $component_type) &&
          in_array($component_type['coupon_type'], array_keys($coupon_types))) {
          // Add the component to the coupon components array.
          $coupon_components[] = $component;
        }
      }

      // Summerize the coupon amounts.
      foreach ($coupon_components as $component) {
        $coupon_amount += abs($component['price']['amount']);
      }
      // Calculate the discount percentage.
      $discount_rate
        = $coupon_amount / ($price['amount'] + $coupon_amount) * 10000;
    }
    $article['discount_rate'] = (int) $discount_rate;
    $article['unit_price'] = (int) $price['amount'] + $coupon_amount;

    // If the tax module is enabled, try to get the tax rate.
    if (function_exists('commerce_tax_components')) {
      $tax = commerce_tax_components($price['data']['components']);
      $tax_rate
        = count($tax) ? $tax[0]['price']['data']['tax_rate']['rate'] * 10000 : 0;
      $article['tax_rate'] = $tax_rate;
    }

    // Add this line item to the cart array.
    array_push($cart, $article);
  }

  // Set currency.
  $order_total = $order_wrapper->commerce_order_total->value();
  $currency_code = $order_total['currency_code'];

  // Merchant ID.
  $eid = $settings['estore_id'];
  // Shared secret.
  $shared_secret = mb_convert_encoding($settings['secret'], 'ISO-8859-1');

  // Server URI.
  $uri = commerce_klarna_checkout_get_endpoint($settings);

  Klarna_Checkout_Order::$baseUri = $uri;
  Klarna_Checkout_Order::$contentType
    = "application/vnd.klarna.checkout.aggregated-order-v2+json";
  $connector = Klarna_Checkout_Connector::create($shared_secret);

  // Create Klarna order.
  $klarna_order = NULL;
  // Does an order already exist?
  if (array_key_exists('klarna_checkout', $_SESSION)) {
    // Resume session.
    $klarna_order = new Klarna_Checkout_Order(
      $connector,
      $_SESSION['klarna_checkout']
    );
    try {
      $klarna_order->fetch();

      // Is this the correct order?
      if ($klarna_order['merchant_reference'] != $order->order_id) {
        throw new Exception('New order');
      }

      // Reset cart.
      $update['cart']['items'] = array();
      foreach ($cart as $item) {
        $update['cart']['items'][] = $item;
      }
      $klarna_order->update($update);
    }
    catch (Exception $e) {
      // Reset session.
      $klarna_order = NULL;
      unset($_SESSION['klarna_checkout']);
    }
  }
  // Create a new order.
  if ($klarna_order == NULL) {
    // Start new session.
    // Check platform.
    if (libraries_get_path('Mobile-Detect', FALSE)) {
      $detect = new Mobile_Detect();
      $layout = $detect->isMobile() ? 'mobile' : 'desktop';
    }
    else {
      $layout = $settings['layout'];
    }
    $create['gui']['layout'] = $layout;

    // Get locale settings.
    $locale = $settings['language'];
    // The purchase country is based on the locale.
    $country = _commerce_klarna_checkout_locale_to_country($locale);
    $create['purchase_country'] = $country;
    $create['locale'] = $locale;
    $create['purchase_currency'] = $currency_code;
    $create['merchant_reference'] = array('orderid1' => $order->order_id);
    $create['merchant']['id'] = $eid;
    $create['merchant']['terms_uri']
      = url($settings['terms_path'], array('absolute' => TRUE));
    $create['merchant']['checkout_uri']
      = url($settings['checkout'], array('absolute' => TRUE));
    $create['merchant']['confirmation_uri']
      = url($settings['checkout_confirm'], array('absolute' => TRUE));
    // You can not recieve push notification on non publicly available uri.
    $create['merchant']['push_uri'] = $settings['callback'];

    foreach ($cart as $item) {
      $create['cart']['items'][] = $item;
    }
    try {
      $klarna_order = new Klarna_Checkout_Order($connector);
      $klarna_order->create($create);
      $klarna_order->fetch();
    }
    catch (Exception $e) {
      drupal_set_message(t('Klarna experienced a problem creating the payment.' .
        'Please contact the website administrator.'));
      watchdog('commerce_klarna_checkout',
        'Could not create Klarna order <pre>' . check_plain($e, TRUE) . '</pre>',
        array(), WATCHDOG_ERROR);
    }
  }
  // Store location of checkout session.
  $_SESSION['klarna_checkout'] = $klarna_order->getLocation();

  return $klarna_order;
}

/**
 * Helper function to return purchase_country for locale.
 *
 * The list is based on https://docs.klarna.com/en/rest-api#supported_locales
 *
 * @param string $locale
 *   A valid locale string.
 *
 * @return string
 *   A purchase country to be used in the API. Or false if invalid.
 */
function _commerce_klarna_checkout_locale_to_country($locale = 'sv-se') {
  $codes = array(
    'sv-se' => 'SE',
    'fi-fi' => 'FI',
    'sv-fi' => 'FI',
    'nb-no' => 'NO',
  );
  if (empty($codes[$locale])) {
    // If we get here, someone has done some hacking. And if some has done some
    // hacking, the Klarna call will probably fail due to unsupported locale, so
    // we just return FALSE.
    return FALSE;
  }
  return $codes[$locale];
}

/**
 * Helper function that returns the Klarna Checkout endpoint.
 *
 * @param mixed $settings
 *   An array of settings.
 *
 * @return strings
 *   The Klarna Checkout endpoint URI
 */
function commerce_klarna_checkout_get_endpoint($settings) {
  // Server URI.
  if ($settings['live_mode']) {
    $uri = 'https://checkout.klarna.com/checkout/orders';
  }
  else {
    $uri = 'https://checkout.testdrive.klarna.com/checkout/orders';
  }
  return $uri;
}


/**
 * Helper function that returns the Klarna Checkout object.
 *
 * @param mixed $order
 *   The Drupal Commerce order object.
 * @param int $checkout_id
 *   The checkout ID returned by Klarna.
 *
 * @return mixed
 *   A Klarna Order object.
 */
function commerce_klarna_checkout_get_order($order, $checkout_id) {

  $payment_method = commerce_payment_method_instance_load($order->data['payment_method']);
  $settings = $payment_method['settings'];
  $secret = $settings['secret'];
  $uri = commerce_klarna_checkout_get_endpoint($settings);

  Klarna_Checkout_Order::$baseUri = $uri;
  Klarna_Checkout_Order::$contentType
    = "application/vnd.klarna.checkout.aggregated-order-v2+json";

  $connector = Klarna_Checkout_Connector::create($secret);
  $klarna_order = new Klarna_Checkout_Order($connector, $checkout_id);

  try {
    $klarna_order->fetch();
  }
  catch (Exception $e) {
    drupal_set_message($e->getMessage());
    watchdog('commerce_klarna_checkout',
      'Could not fetch Klarna order <pre>' . check_plain($e, TRUE) . '</pre>',
      array(), WATCHDOG_ERROR);
  }
  return $klarna_order;
}


/**
 * Helper function that updates the Klarna Checkout object.
 *
 * @param mixed $order
 *   The Drupal Commerce order object.
 * @param mixed $klarna_order
 *   The Klarna Order object.
 *
 * @return mixed
 *   A Klarna Order object.
 */
function commerce_klarna_checkout_update_order(&$order, $klarna_order) {
  if ($klarna_order['status'] == "checkout_complete") {
    $payment_method = commerce_payment_method_instance_load($order->data['payment_method']);
    $charge = array(
      'currency_code' => $klarna_order['purchase_currency'],
      'amount'        => (int) $klarna_order['cart']['total_price_including_tax'] * 10,
    );
    commerce_klarna_checkout_transaction($payment_method, $order, $charge, $klarna_order);

    $update['status'] = 'created';
    $klarna_order->update($update);
    watchdog(
      'commerce_klarna_checkout',
      'Klarna Checkout order id %order_id successfully updated. Order status is %status',
      array(
        '%order_id'   => $order->order_id,
        '%status'     => $klarna_order['status'],
      ),
      WATCHDOG_NOTICE
    );
  }
  return $klarna_order;
}


/**
 * Helper function that updates the Commerce Payment.
 *
 * @param mixed $order
 *   The Drupal Commerce order object.
 * @param integer $transaction_id
 *   The Drupal Commerce payment transaction ID.
 *
 * @return boolean
 *   Was the status updated?
 */
function commerce_klarna_checkout_update_payment_status($order, $transaction_id) {
  $transaction = commerce_payment_transaction_load($transaction_id);

  // Check if the transaction exists
  if (!isset($transaction)) {
    return FALSE;
  }
  // Check if the transaction remote ID is set
  if (!isset($transaction->remote_id) || $transaction->remote_id == "") {
    return FALSE;
  }

  // Get the Klarna Order
  $klarna_order = commerce_klarna_checkout_get_order($order, $transaction->remote_id);
  if ($transaction->remote_status != $klarna_order['status']) {
    $transaction->remote_status = $klarna_order['status'];
    commerce_payment_transaction_save($transaction);
    return TRUE;
  }
  else {
    return FALSE;
  }
}
