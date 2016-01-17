<?php
/**
 * @file
 * UTMP: method and functions for Universal Tracking Measure Protocol.
 *
 * @see https://developers.google.com/analytics/devguides/collection/protocol/v1/devguide
 */

// Google UTMP Endpoint URL.
define('GA_PUSH_METHOD_UTMP_HOST_ENDPOINT', 'ssl.google-analytics.com/collect');

/**
 * GA Push Method callback: UTMP (php).
 */
function ga_push_method_utmp_php($push, $type, $options) {

  switch ($type) {

    case GA_PUSH_TYPE_EVENT:
      $event_data = ga_push_method_php_utmp_construct_event_request_data($push);
      // @TODO: Check response.
      ga_push_method_utmp_php_request($event_data, $options);
      break;

    case GA_PUSH_TYPE_ECOMMERCE:
      if (isset($push['trans']) && isset($push['items']) && count($push['items'])) {
        // Construct arrays.
        $ecommerce_data = ga_push_method_utmp_php_construct_ecommerce_request_data($push);
        // @TODO: Check request response.

        // Sending the transaction:
        ga_push_method_utmp_php_request($ecommerce_data['transaction'], $options);

        // Sending the items:
        foreach ($ecommerce_data['items'] as $item_data) {
          ga_push_method_utmp_php_request($item_data, $options);
        }
      }
      break;

    case GA_PUSH_TYPE_EXCEPTION:
      $event_data = ga_push_method_php_utmp_construct_exception_request_data($push);
      // @TODO: Check response.
      ga_push_method_utmp_php_request($event_data, $options);
      break;

    case GA_PUSH_TYPE_PAGEVIEW:
      $pageview_data = ga_push_method_php_utmp_construct_pageview_request_data($push);
      // @TODO: Check response.
      ga_push_method_utmp_php_request($pageview_data, $options);
      break;

    case GA_PUSH_TYPE_SOCIAL:
      $pageview_data = ga_push_method_php_utmp_construct_social_request_data($push);
      // @TODO: Check response.
      ga_push_method_utmp_php_request($pageview_data, $options);
      break;
  }
}

/**
 * Returns data array for a event tacking.
 *
 * @param array $push
 *   Array with event data.
 *
 * @return array
 *   Array with the event data preparad to be sent to GA.
 */
function ga_push_method_php_utmp_construct_event_request_data($push) {

  // Initiallization with required keys.
  $data = array(
    't' => 'event',
    'ec' => $push['category'],
    'ea' => $push['action'],
  );

  // Optional keys:
  if (isset($push['label'])) {
    $data['el'] = $push['label'];
  }

  if (isset($push['value'])) {
    $data['ev'] = $push['value'];
  }

  // @TODO: Check if this attribute is still used.
  // if (isset($push['non-interaction'])

  return $data;
}


/**
 * Returns data arrays for a eCommerce tacking.
 *
 * @param array $push
 *   Array with transaction and items data.
 *
 * @return array
 *   Array with two keys with eCommerce data preparad to be sent to GA:
 *     - transaction: transaction data to send to GA.
 *     - items: Array with data to send to GA for each item.
 */
function ga_push_method_utmp_php_construct_ecommerce_request_data($push) {

  $data = array();

  // Transaction data.
  // @TODO: All keys should be present? are they all required?
  $data['transaction'] = array(
    't' => 'transaction',
    'ti' => $push['trans']['order_id'],
    'ta' => $push['trans']['affiliation'],
    'tr' => $push['trans']['total'],
    'tt' => $push['trans']['total_tax'],
    'ts' => $push['trans']['total_shipping'],
    'cu' => $push['trans']['currency'],
  );

  // Items data.
  foreach ($push['items'] as $value) {
    $data['items'][] = array(
      't' => 'item',
      'ti' => $value['order_id'],
      'in' => $value['name'],
      'ip' => !empty($value['price']) ? $value['price'] : NULL,
      'iq' => !empty($value['quantity']) ? $value['quantity'] : NULL,
      'ic' => !empty($value['sku']) ? $value['sku'] : NULL,
      'iv' => !empty($value['category']) ? $value['category'] : NULL,
      'cu' => !empty($value['currency']) ? $value['currency'] : NULL,
    );
  }

  return $data;
}

/**
 * Returns data array for a exception tracking.
 *
 * @param array $push
 *   Array with pageview data.
 *
 * @return array
 *   Array with the pageview data ready to be sent to GA.
 */
function ga_push_method_php_utmp_construct_exception_request_data($push) {
  // @TODO: integrate with ga_push_analyticsjs_measurement_protocol_map().
  // Initiallization with required keys.
  $data = array(
    't' => 'exception',
    'exd' => !empty($push['exDescription']) ? $push['exDescription'] : NULL,
    'exf' => !empty($push['exFatal']) ? $push['exFatal'] : NULL,
  );

  return $data;
}

/**
 * Returns data array for a pageview tracking.
 *
 * @param array $push
 *   Array with pageview data.
 *
 * @return array
 *   Array with the pageview data ready to be sent to GA.
 */
function ga_push_method_php_utmp_construct_pageview_request_data($push) {

  // Initiallization with required keys.
  $data = array(
    't' => 'pageview',
    'dh' => !empty($push['location']) ? $push['location'] : NULL,
    'dp' => !empty($push['page']) ? $push['page'] : NULL,
    'dt' => !empty($push['title']) ? $push['title'] : NULL,
  );

  return $data;
}

/**
 * Returns data array for a social tracking.
 *
 * @param array $push
 *   Array with pageview data.
 *
 * @return array
 *   Array with the pageview data ready to be sent to GA.
 */
function ga_push_method_php_utmp_construct_social_request_data($push) {

  // Initiallization with required keys.
  $data = array(
    't' => 'social',
    'sn' => !empty($push['socialNetwork']) ? $push['socialNetwork'] : NULL,
    'sa' => !empty($push['socialAction']) ? $push['socialAction'] : NULL,
    'st' => !empty($push['socialTarget']) ? $push['socialTarget'] : NULL,
  );

  return $data;
}

/**
 * Sends tracking data to GA.
 *
 * @param array $data
 *   Data prepared for GA.
 * @param array $options
 *   Custom options from ga_push_add().
 * @param string $method
 *   Method to use (GET or POST).
 *
 * @see ga_push_add()
 */
function ga_push_method_utmp_php_request($data = array(), $options = array(), $method = 'POST') {

  $data['v']   = 1;
  $data['tid'] = !empty($options['tid']) ? $options['tid'] : variable_get('googleanalytics_account', '');
  $data['cid'] = !empty($options['cid']) ? $options['cid'] : ga_push_method_php_utmp_get_uuid();

  $url = 'https://' . GA_PUSH_METHOD_UTMP_HOST_ENDPOINT;

  $httpr_options = array(
    'method' => $method,
    'data' => drupal_http_build_query($data),
  );

  // @TODO: Check errors here so caller doesn't have to do it.
  $response = drupal_http_request($url, $httpr_options);
}

/**
 * Get an UUID ver 4 identifier form cookie or generates one on fallback.
 *
 * @return string
 *   UUID ver 4 identifier
 */
function ga_push_method_php_utmp_get_uuid() {
  if (isset($_COOKIE['_ga'])) {
    list($version, $domain_depth, $cid1, $cid2)
      = explode('.', $_COOKIE["_ga"], 4);

    $contents = array(
      'version' => $version,
      'domainDepth' => $domain_depth,
      'cid' => $cid1 . '.' . $cid2);
    $cid = $contents['cid'];
  }
  else {
    $cid = ga_push_method_php_utmp_gen_uuid();
  }

  return $cid;
}

/**
 * Generates an UUID ver 4 identifier.
 *
 * Thanks to Andrew Mooer at php.net.
 * http://www.php.net/manual/en/function.uniqid.php#94959
 *
 * @return string
 *   UUID.
 */
function ga_push_method_php_utmp_gen_uuid() {
  return sprintf('%04x%04x-%04x-%04x-%04x-%04x%04x%04x',
    // 32 bits for "time_low"
    mt_rand(0, 0xffff), mt_rand(0, 0xffff),
    // 16 bits for "time_mid"
    mt_rand(0, 0xffff),
    // 16 bits for "time_hi_and_version",
    // four most significant bits holds version number 4
    mt_rand(0, 0x0fff) | 0x4000,
    // 16 bits, 8 bits for "clk_seq_hi_res",
    // 8 bits for "clk_seq_low",
    // two most significant bits holds zero and one for variant DCE1.1
    mt_rand(0, 0x3fff) | 0x8000,
    // 48 bits for "node"
    mt_rand(0, 0xffff), mt_rand(0, 0xffff), mt_rand(0, 0xffff)
  );
}
