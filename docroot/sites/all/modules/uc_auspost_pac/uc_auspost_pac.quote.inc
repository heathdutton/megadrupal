<?php
/**
 * @file
 * Functions used to generate the shipping quotes.
 */

/**
 * Call to remote API server and return result.
 *
 * @param $api_call
 *   The API function to call. This is the part of the URL after the postage
 *   section less the xml/json extension.
 * @param $query
 *   Query array to be added to the URL in key value pairs.
 * @param $reset
 *   boolean Reset the static and database caching of the requested value.
 * @param $cache_expiry
 *   integer The cache expiry value to set when adding to the cache.
 * @param $api_key
 *   Used to override the stored API Key, usually for testing.
 */
function _uc_auspost_pac_api($api_call, $query = array(), $reset = FALSE, $cache_expiry = CACHE_TEMPORARY, $api_key = FALSE) {
  $static_cache = &drupal_static(__FUNCTION__, array());

  // API is a requirement for this call so make this really loud.
  if (!$api_key) {
    $api_key = variable_get('uc_auspost_pac_var_api_key', FALSE);
  }
  if (!$api_key) {
    drupal_set_message(t('An internal error has occurred in the Aus Post Shipping module. Please contact the System Administrator to rectify the problem.'), 'error');
    return FALSE;
  }

  // Base URL. Add json encoding to api call.
  $url = UC_AUSPOST_PAC_API_URL . $api_call . '.json';

  // Add query params if required.
  $query_string = '';
  if ($query) {
    $query_string = drupal_http_build_query($query);
    $url .= '?' . $query_string;
  }

  // Build the cid to check if it's in the cache.
  $cid = $api_call . ':' . $query_string;
  $cid = substr($cid, 0, 220) . ':' . md5($cid . $api_key);

  // Static cache.
  if (!$reset && isset($static_cache[$cid])) {
    return $static_cache[$cid];
  }

  // Database cache.
  if (!$reset && $cache = cache_get($cid, 'cache_uc_auspost_pac')) {
    $expiry = $cache->expire;
    if (!empty($cache->data) && ($cache->expire <= 0 || $cache->expire >= REQUEST_TIME)) {
      // Add data to static cache and return;
      return $static_cache[$cid] = $cache->data;
    }
  }

  // Call the API, adding the API Key to the headers.
  $result = drupal_http_request($url, array(
    'method' => 'GET',
    'headers' => array(
      'AUTH-KEY' => $api_key,
    ),
  ));

  // On success, return a decoded json object.
  if ($result->code == 200) {
    $data = drupal_json_decode($result->data);
    $data['error'] = FALSE;

    // Add to DB and static caches, then return;
    if ($cache_expiry != UC_AUSPOST_PAC_CACHE_NONE) {
      cache_set($cid, $data, 'cache_uc_auspost_pac', $cache_expiry);
    }
    return $static_cache[$cid] = $data;
  }

  // API Key is invalid.
  elseif ($result->code == 403) {
    $data = drupal_json_decode($result->data);
    $error = isset($data['error']['errorMessage']) ? $data['error']['errorMessage'] : t('Unknown error');
    watchdog('uc_auspost_pac',
      'Access was denied to the Aus Post API. Check your API Key: @api_key. Err msg: @error',
      array(
        '@api_key' => $api_key,
        '@error' => $error,
      ),
      WATCHDOG_CRITICAL,
      l(t('settings'), 'admin/store/settings/quotes/auspost-pac')
    );
    return array('error' => check_plain($error));
  }

  // Required parameters were missing.
  elseif ($result->code == 404) {
    $data = drupal_json_decode($result->data);
    $error = isset($data['error']['errorMessage']) ? $data['error']['errorMessage'] : t('Unknown error');
    watchdog('uc_auspost_pac',
      'Some parameters were missing, or unknown API call. Err msg: @error',
      array(
        '@error' => $error,
      ),
      WATCHDOG_CRITICAL
    );
    return array('error' => check_plain($error));
  }

  // Failure.
  return array('error' => t('Unhandled error condition'));
}

/**
 * Callback for retrieving AusPost shipping quote.
 *
 * @param $products
 *   Array of cart contents.
 * @param $details
 *   Order details other than product information.
 * @param $method
 *   The shipping method to create the quote.
 *
 * @return
 *   JSON object containing rate, error, and debugging information.
 */
function uc_auspost_pac_quote($products, $details, $method) {
  // The uc_quote AJAX query can fire before the customer has completely
  // filled out the destination address, so check to see whether the address
  // has all needed fields. If not, abort.
  $destination = (object) $details;

  // Country code is always needed.
  if (empty($destination->country)) {
    // Skip this shipping method.
    return array();
  }

  // Shipments to Australia also need postal_code.
  if ($destination->country == 36 && empty($destination->postal_code)) {
    // Skip this shipping method.
    return array();
  }

  // If the destination is Australia, then only continue if this is a domestic
  // method. This should have already been filtered out by the conditions, but
  // that is not guaranteed. Vice versa for international.
  $method_is_international = (strpos($method['id'], 'intnl') !== FALSE);
  if (($method_is_international && $destination->country == 36) || (!$method_is_international && $destination->country != 36)) {
    // Warn the administrator as this should be fixed, but is otherwise
    // harmless.
    watchdog('uc_auspost_pac',
      'AusPost method %method has misconfigured conditions allowing it to attempt to validate when the shipping destination is@not Australia.',
      array(
        '%method' => $method['title'],
        '@not' => ($method_is_international ? '' : ' NOT'),
      ),
      WATCHDOG_WARNING,
      l(t('conditions'), 'admin/store/settings/quotes/manage/get_quote_from_' . $method['id'])
    );
    return array();
  }

  // Initialize $debug_data to prevent PHP notices here and in uc_quote.
  $debug_data = array('debug' => NULL, 'error' => array());
  $services = array();
  $addresses = array(variable_get('uc_quote_store_default_address', new UcAddress()));

  // Grab the method settings, and add them to the method.
  $method['settings'] = _uc_auspost_pac_get_method_settings($method['id']);

  // Using the provided packing method, package the products.
  $packing_method = isset($method['settings']['packing_method']) ? $method['settings']['packing_method'] : 'do_not_pack';
  $packing_method_info = uc_auspost_pac_get_packing_method($packing_method);

  // Include the file in the file path, if they are set. Only use include_once
  // to avoid WSOD, but still leave an error message to trace why things aren't
  // working. Only packing method developers can screw this one up.
  if (isset($packing_method_info['file'])) {
    include_once $packing_method_info['file path'] . '/' . $packing_method_info['file'];
  }

  // If the callback doesn't exist, we are unable to pack and unable to quote.
  // Return an error message if possible, or nothing otherwise.
  if (!function_exists($packing_method_info['callback'])) {
    return _uc_auspost_pac_quote_error($method, t('An error has occurred while generating a shipping quote. Please contact the website administrator to fix the configuration problem.'));
  }

  // Package up the products ready for quoting.
  $function = $packing_method_info['callback'];
  $addressed_packages = $function($method, $products, $addresses);

  if (!count($addressed_packages)) {
    return array();
  }

  // Quote for each of the packages.
  $first = TRUE;
  $quoted_accessorials = array();
  foreach ($addressed_packages as $address_id => $packages) {
    foreach ($packages as $package_index => $package) {
      // The package will be associated with a source shipping address at this
      // point. If that source is not in Australia, then this shipping method
      // is quite useless. 36 is the magic number of Australia in UC.
      if ($addresses[$address_id]->country != 36) {
        return _uc_auspost_pac_quote_error($method, t('Product source shipping address is not Australia. Unable to ship via AusPost. Please contact the website administrator to fix the configuration problem.'));
      }

      // Alter the weight on the packages prior to sending them off to the
      // AusPost PAC API.
      // @todo Offload this to rules? Does not currently do anything cmt out.
      //$package->grams = uc_auspost_pac_weight_markup($method, $package->grams);

      // Ask the AusPost PAC API for which services are available for this
      // package.
      $services = _uc_auspost_pac_api_quote_wrapper($method, $addresses[$address_id], $destination, $package);

      // If there is an error, we need to somehow do something about it. The
      // quote will not be complete and will not be valid. Ignore all gathered
      // data and return the provided error.
      if ($services['error']) {
        return _uc_auspost_pac_quote_error($method, t('An error has occurred while generating a shipping quote. The error was communications based and may be transient.'));
      }

      // Flatten the services and limit them to the allowed services.
      $services = _uc_auspost_pac_flatten_services($method, $services);

      // No result means that we can't ship with this method as nothing is
      // enabled. Bug out silently.
      if (!$services) {
        return array();
      }

      // Only query services that are already present in the quote results
      // array.
      $quotes = $first ? $services : array_intersect_key($services, $quoted_accessorials);

      // Eliminate any in the quote results array that are not present in this
      // quote array.
      if (!$first) {
        $eliminate = array_diff_key($quoted_accessorials, $quotes);
        foreach ($eliminate as $rmkey => $ignore) {
          unset($quoted_accessorials[$rmkey]);
        }
      }

      // Query all services to get the total cost and costs array. Static cache
      // takes care of repeated requests for us.
      foreach ($quotes as $service_key => $service) {
        $quote = _uc_auspost_pac_api_quote_wrapper($method, $addresses[$address_id], $destination, $package, $service);

        if ($quote['error']) {
          return _uc_auspost_pac_quote_error($method, t('An error has occurred while requesting shipping costs.'));
        }


        // Multiply total_cost and costs items by the package qty.
        $quote['postage_result']['total_cost'] *= $package->qty;
        if (!isset($quote['postage_result']['costs']['cost'][1])) {
          $quote['postage_result']['costs']['cost'] = array($quote['postage_result']['costs']['cost']);
        }
        foreach ($quote['postage_result']['costs']['cost'] as $i => $cost) {
          $quote['postage_result']['costs']['cost'][$i]['cost'] *= $package->qty;
        }

        $quoted_accessorials[$service_key][$address_id][$package_index] = $quote['postage_result'];
      }

      // Flag to indicate when we're done the first package.
      if ($quoted_accessorials) {
        $first = FALSE;
      }
    }
  }

  $quote_data = array();

  foreach ($quoted_accessorials as $key => $addresses) {
    $total_cost = 0.0;
    $costs = array();
    $msg = '';

    foreach ($addresses as $address_id => $package_quotes) {
      foreach ($package_quotes as $package_id => $quote) {
        if (isset($quote['delivery_time'])) {
          $msg = $quote['delivery_time'];
        }

        $total_cost += $quote['total_cost'];

        foreach ($quote['costs']['cost'] as $cost) {
          if (isset($costs[$cost['item']])) {
            $costs[$cost['item']] += $cost['cost'];
          }
          else {
            $costs[$cost['item']] = $cost['cost'];
          }
        }
      }
    }

    // Adjust the total cost by the markup amount and type.
    $total_cost = uc_auspost_pac_rate_markup($method, $total_cost);

    $quote_data[$key] = array(
      'rate' => $total_cost,
      'option_label' => $method['quote']['accessorials'][$key],
    );

    if ($msg) {
      $quote_data[$key]['error'][] = $msg;
    }
  }

  return $quote_data;
}

/**
 * Helper function to return a single error message to the site user. The index
 * of the returned array must be one of the accessorials of the method or it
 * will not be displayed.
 *
 * @param $method
 *   The shipping method.
 * @param $message
 *   Error message to be returned to the user.
 *
 * @return
 *   An array keyed with the first accessorial and an array of error messages.
 */
function _uc_auspost_pac_quote_error($method, $message) {
  $first_accessorial = reset(array_keys($method['quote']['accessorials']));
  if ($first_accessorial) {
    return array($first_accessorial => array(
      'error' => array($message),
    ));
  }
  // Absolute configuration or development failure.
  else {
    return array();
  }
}

/**
 * Helper function to flatten the services list provided by AusPost PAC into
 * an array that matches our internal configuration.
 *
 * @param array $method
 *   The method id provided by hook_uc_shipping_method().
 * @param array $services
 *   Array of services return by the AusPost PAC API.
 *
 * @return
 *   Array flattened into the same keys as the accessorials in methods.
 */
function _uc_auspost_pac_flatten_services($method, $services) {
  // Pull the accessorials for this method.
  $accessorials = _uc_auspost_pac_method_accessorials_options($method['id']);

  // Use the settings to limit the allowed accessorials.
  $allowed_services = $method['settings']['enabled_services'];

  $result = array();

  // LOOK OUT! If there's only a single service or option, there's no index.
  if (!isset($services['services']['service'][1])) {
    $services['services']['service'] = array($services['services']['service']);
  }

  foreach ($services['services']['service'] as $service) {
    $service_code = $service['code'];

    // Only add if it's an accessorial AND there's no filtering OR it's
    // enabled. This is the same all the way through.
    if (isset($accessorials[$service_code]) && (!$allowed_services || isset($allowed_services[$service_code]))) {
      $service['service_code'] = $service_code;
      $result[$service_code] = $service;
    }

    if (isset($service['options']['option'])) {
      // LOOK OUT! If there's only one option, there's no index.
      if (!isset($service['options']['option'][1])) {
        $service['options']['option'] = array($service['options']['option']);
      }

      foreach ($service['options']['option'] as $option) {
        $option_code = $service_code . '+' . $option['code'];

        if (isset($accessorials[$option_code]) && (!$allowed_services || isset($allowed_services[$option_code]))) {
          $option['service_code'] = $service_code;
          $option['option_code'] = $option['code'];
          $result[$option_code] = $option;
        }

        if (isset($option['suboptions']['option'])) {
          // LOOK OUT! If there's only one suboption, there's no index.
          if (!isset($option['suboptions']['option'][1])) {
            $option['suboptions']['option'] = array($option['suboptions']['option']);
          }

          foreach ($option['suboptions']['option'] as $suboption) {
            $suboption_code = $option_code . '+' . $suboption['code'];

            if (isset($accessorials[$suboption_code]) && (!$allowed_services || isset($allowed_services[$suboption_code]))) {
              $suboption['service_code'] = $service_code;
              $suboption['option_code'] = $option['code'];
              $suboption['suboption_code'] = $suboption['code'];
              $result[$suboption_code] = $suboption;
            }
          }
        }
      }
    }
  }

  return $result;
}

/**
 * Query the appropriate API service with the appropriate values.
 */
function _uc_auspost_pac_api_quote_wrapper($method, $source_address, $destination_address, $package, $service_options = array()) {
  $api = FALSE;
  $values = array();

  switch ($method['id']) {
    case 'auspost_domestic_letter':
      // Dimensions are in mm and grams so do not need conversion.
      $values = array(
        'length' => $package->length,
        'width' => $package->width,
        'thickness' => $package->height,
        'weight' => $package->grams,
      );

      $api = 'letter/domestic';
      break;

    case 'auspost_domestic_parcel':
      $from_postcode = $source_address->postal_code;
      $to_postcode = $destination_address->postal_code;

      if (!$from_postcode || !$to_postcode) {
        return array();
      }

      // Values need to be converted into cm and kg.
      $values = array(
        'from_postcode' => $from_postcode,
        'to_postcode' => $to_postcode,
        'length' => $package->length * MM_TO_CM,
        'width' => $package->width * MM_TO_CM,
        'height' => $package->height * MM_TO_CM,
        'weight' => $package->grams * G_TO_KG,
      );

      $api = 'parcel/domestic';
      break;

    case 'auspost_intnl_letter':
      // Requires only the country_code of the destination, and the weight of the
      // package.
      $destination_country = reset(uc_get_country_data(array('country_id' => $destination_address->country)));

      $values = array(
        'country_code' => $destination_country['country_iso_code_2'],
        'weight' => $package->grams,
      );

      $api = 'letter/international';
      break;

    case 'auspost_intnl_parcel':
      // Requires only the country_code of the destination, and the weight of the
      // package.
      $destination_country = reset(uc_get_country_data(array('country_id' => $destination_address->country)));

      $values = array(
        'country_code' => $destination_country['country_iso_code_2'],
        'weight' => $package->grams * G_TO_KG,
      );

      $api = 'parcel/international';
      break;

    default:
      // developer error
      return array();
  }

  if ($service_options) {
    _uc_auspost_pac_request_add_values($values, $service_options);
    $api .= '/calculate';
  }
  else {
    $api .= '/service';
  }

  $request = _uc_auspost_pac_api($api, $values);

  return $request;
}

/**
 * Helper function to add all of the optional codes to the values array.
 */
function _uc_auspost_pac_request_add_values(&$values, $service_options) {
  $values['service_code'] = $service_options['service_code'];

  if (isset($service_options['option_code'])) {
    $values['option_code'] = $service_options['option_code'];
  }

  if (isset($service_options['suboption_code'])) {
    $values['suboption_code'] = $service_options['suboption_code'];
  }

  if (isset($service_options['extra_cover']) && $service_options['extra_cover']) {
    $values['extra_cover'] = $service_options['extra_cover'];
  }
}

/**
 * Modifies the rate received from AusPost before displaying to the customer.
 *
 * @param $method
 *   The AusPost method to base the markup on.
 * @param $rate
 *   Shipping rate without any rate markup.
 *
 * @return
 *   Shipping rate after markup.
 */
function uc_auspost_pac_rate_markup($method, $rate) {

  $type = $method['settings']['markup_type'];

  if ($type != UC_AUSPOST_PAC_MARKUP_NONE) {
    $amount = $method['settings']['markup_amount'];

    switch ($type) {
      case UC_AUSPOST_PAC_MARKUP_PERCENT:
        return $rate * (1 + ($amount / 100));

      case UC_AUSPOST_PAC_MARKUP_FIXED:
        return $rate + $amount;
    }
  }

  return $rate;
}

/**
 * Modifies the weight of shipment before sending to AusPost for a quote.
 *
 * @param $method
 *   The AusPost method to base the markup on.
 * @param $weight
 *   Shipping weight without any weight markup.
 *
 * @return
 *   Shipping weight after markup.
 */
function uc_auspost_pac_weight_markup($method, $weight) {
  // @todo
  return $weight;
}

/**
 * Pseudo-constructor to set default values of a package.
 */
function _uc_auspost_pac_new_package() {
  $package = new stdClass();

  $package->price = 0;
  $package->qty = 1;

  // Weights are in grams.
  $package->grams = 0;

  // Sizes are in mm.
  $package->length = 0;
  $package->width = 0;
  $package->height = 0;

  return $package;
}
