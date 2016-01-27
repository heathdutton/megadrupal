<?php
/**
 * @file
 * Batch rebuild of rarely changing data sourced from the API server. This data
 * is normally added during the install process but can also be built manually.
 */

/**
 * Batch initialisation function.
 */
function _uc_auspost_pac_batch_init() {
  $batch = array(
    'title' => t('Preparing to rebuild static information'),
    'init_message' => t('Preparing API functions to call'),
    'finished' => '_uc_auspost_pac_batch_finished',
    'file' => drupal_get_path('module', 'uc_auspost_pac') . '/uc_auspost_pac.batch.inc',

    'operations' => array(
      // Get valid countries for delivery.
      array('_uc_auspost_pac_batch_api_call', array(t('Retrieving country list'), 'country')),
      array('_uc_auspost_pac_batch_process_country', array(t('Processing country list'))),

      // Gather source data.
      array('_uc_auspost_pac_batch_api_call', array(t('Domestic Letter Thickness'), 'letter/domestic/thickness')),
      array('_uc_auspost_pac_batch_api_call', array(t('Domestic Letter Weight'), 'letter/domestic/weight')),
      array('_uc_auspost_pac_batch_api_call', array(t('Domestic Letter Encolope Size'), 'letter/domestic/size')),
      array('_uc_auspost_pac_batch_api_call', array(t('International Letter Weight'), 'letter/international/weight')),
      array('_uc_auspost_pac_batch_api_call', array(t('International Parcel Weight'), 'parcel/international/weight')),
      array('_uc_auspost_pac_batch_api_call', array(t('Domestic Parcel Weight'), 'parcel/domestic/weight')),
      array('_uc_auspost_pac_batch_api_call', array(t('Domestic Parcel Box Type'), 'parcel/domestic/type')),
      array('_uc_auspost_pac_batch_api_call', array(t('Domestic Parcel Box Size'), 'parcel/domestic/size')),

      // Prepare source values for API calls.
      array('_uc_auspost_pac_batch_domestic_letter_prepare', array(t('Preparing options for Domestic Letter Service'))),
      array('_uc_auspost_pac_batch_domestic_parcel_prepare', array(t('Preparing options for Domestic Parcel Service'))),
      array('_uc_auspost_pac_batch_intnl_prepare', array(
        t('Preparing options for International Letter Service'),
        'letter/international/weight',
        'intnl_letter_query_values',
      )),
      array('_uc_auspost_pac_batch_intnl_prepare', array(
        t('Preparing options for International Parcel Service'),
        'parcel/international/weight',
        'intnl_parcel_query_values',
      )),

      // Query the API for all of the generated values to ensure a complete
      // list of all services.
      array('_uc_auspost_pac_batch_process_services', array(
        t('Domestic Letter Service list'),
        'letter/domestic/service',
        'domestic_letter_query_values',
        'auspost_domestic_letter',
      )),
      array('_uc_auspost_pac_batch_process_services', array(
        t('Domestic Parcel Service list'),
        'parcel/domestic/service',
        'domestic_parcel_query_values',
        'auspost_domestic_parcel',
      )),
      array('_uc_auspost_pac_batch_process_services', array(
        t('International Letter Service list'),
        'letter/international/service',
        'intnl_letter_query_values',
        'auspost_intnl_letter',
      )),
      array('_uc_auspost_pac_batch_process_services', array(
        t('International Parcel Service list'),
        'parcel/international/service',
        'intnl_parcel_query_values',
        'auspost_intnl_parcel',
      )),
    ),
  );

  return $batch;
}

/**
 * Call the API and store the result in the sandbox. Does not take query.
 */
function _uc_auspost_pac_batch_api_call($message, $api_call, &$context) {

  require_once dirname(__FILE__) . '/uc_auspost_pac.quote.inc';
  $context['results'][$api_call] = _uc_auspost_pac_api($api_call);

  $context['message'] = $message;
}

/**
 * Process the country list in the sandbox into a result.
 */
function _uc_auspost_pac_batch_process_country($message, &$context) {

  if ($context['results']['country']['error']) {
    $context['error'] = t('Error downloading the country list.');
    $context['results']['country'] = FALSE;
  }
  else {
    $context['message'] = $message;
    $perma_countries = array();
    foreach ($context['results']['country']['countries']['country'] as $country) {
      $perma_countries[$country['code']] = $country['name'];
    }
    // Replace the existing country array with the processed one.
    $context['results']['country'] = $perma_countries;
  }
}

/**
 * Process the domestic letter services into a set of values to call the
 * service list request on. 
 */
function _uc_auspost_pac_batch_domestic_letter_prepare($message, &$context) {

  // Using all of those values, build an exhaustive list of requests to call
  // which will give the full list of available accessorials.
  $context['results']['domestic_letter_query_values'] = FALSE;
  $domestic_letter_services = array();
  if (!$context['results']['letter/domestic/thickness']['error'] && !$context['results']['letter/domestic/weight']['error'] && !$context['results']['letter/domestic/size']['error']) {
    foreach ($context['results']['letter/domestic/thickness']['thicknesses']['thickness'] as $thickness) {
      if (!isset($thickness['value'])) continue;

      foreach ($context['results']['letter/domestic/weight']['weights']['weight'] as $weight) {
        if (!isset($weight['value'])) continue;

        foreach ($context['results']['letter/domestic/size']['sizes']['size'] as $size) {
          if (!isset($size['value'])) continue;

          // Technical spec says this is the name of the variables, but it
          // doesn't match the parameters gives to the services listing. Yay.
          list($width, $height) = explode('x', $size['value']);

          // Create list of items to ask the API for the services list for.
          $domestic_letter_services[] = array(
            'length' => $height,
            'width' => $width,
            'thickness' => $thickness['value'],
            'weight' => $weight['value'],
          );
        }
      }
    }

    $context['results']['domestic_letter_query_values'] = $domestic_letter_services;

    $context['message'] = $message;
  }
  else {
    $context['error'] = t('Domestic letter data sources caused error.');
  }
}

/**
 * Prepare a list of options to test the domestic parcel service list.
 */
function _uc_auspost_pac_batch_domestic_parcel_prepare($message, &$context) {

  // Need the stored values for the source/destination postcode to test
  // against. Values are Sydney and Melbourne by default.
  $from_postcode = variable_get('uc_auspost_pac_var_from_postcode', 2000);
  $to_postcode = variable_get('uc_auspost_pac_var_to_postcode', 3000);

  // Using all of those values, build an exhaustive list of requests to call
  // which will give the full list of available accessorials.
  $context['results']['domestic_parcel_query_values'] = FALSE;
  $domestic_parcel_services = array();
  if (!$context['results']['parcel/domestic/weight']['error'] && !$context['results']['parcel/domestic/type']['error'] && !$context['results']['parcel/domestic/size']['error']) {
    // parcel/domestic/type does not have any values for use in calculations at
    // time of writing.
    foreach ($context['results']['parcel/domestic/weight']['weights']['weight'] as $weight) {
      if (!isset($weight['value'])) continue;

      foreach ($context['results']['parcel/domestic/size']['sizes']['size'] as $size) {
        if (!isset($size['value'])) continue;

        // Technical spec says this is the name of the variables, but it
        // doesn't match the parameters gives to the services listing. Yay.
        list($width, $height, $depth) = explode('x', $size['value']);

        // Create list of items to ask the API for the services list for.
        $domestic_parcel_services[] = array(
          'from_postcode' => $from_postcode,
          'to_postcode' => $to_postcode,
          'length' => $depth,
          'width' => $width,
          'height' => $height,
          'weight' => $weight['value'],
        );
      }
    }

    $context['results']['domestic_parcel_query_values'] = $domestic_parcel_services;

    $context['message'] = $message;
  }
  else {
    $context['error'] = t('Domestic parcel data sources caused error.');
  }
}

/**
 * Prepare a list of options to test the international letter and parcel
 * service lists.
 */
function _uc_auspost_pac_batch_intnl_prepare($message, $source_key, $result_key, &$context) {

  // A list of countries is required to test against. By default we use the US
  // and the UK (US,GB).
  $source_countries = variable_get('uc_auspost_pac_var_source_countries', array('US', 'GB'));

  // Using all of those values, build an exhaustive list of requests to call
  // which will give the full list of available accessorials.
  $context['results'][$result_key] = FALSE;
  $services = array();
  if (!$context['results'][$source_key]['error']) {

    foreach ($context['results'][$source_key]['weights']['weight'] as $weight) {
      // Value can be missing legitimately from this as postcards have none.
      $weight_value = isset($weight['value']) ? $weight['value'] : '';

      foreach ($source_countries as $country_code) {
        $services[] = array(
          'country_code' => $country_code,
          'weight' => $weight_value,
        );
      }
    }

    $context['results'][$result_key] = $services;

    $context['message'] = $message;
  }
  else {
    $context['error'] = t('%message caused error.', array('%message' => $message));
  }
}

/**
 * Prepare a list of options to test the international parcel service list.
 */
function _uc_auspost_pac_batch_intnl_parcel_prepare($message, &$context) {

  // A list of countries is required to test against. By default we use the US
  // and the UK (US,GB).
  $source_countries = variable_get('uc_auspost_pac_var_source_countries', array('US', 'GB'));

  // Using all of those values, build an exhaustive list of requests to call
  // which will give the full list of available accessorials.
  $context['results']['intnl_letter_query_values'] = FALSE;
  $intnl_letter_services = array();
  if (!$context['results']['letter/international/weight']['error']) {

    foreach ($context['results']['letter/international/weight']['weights']['weight'] as $weight) {
      // Value can be missing legitimately from this as postcards have none.
      $weight_value = isset($weight['value']) ? $weight['value'] : '';

      foreach ($source_countries as $country_code) {
        $intnl_letter_services[] = array(
          'country_code' => $country_code,
          'weight' => $weight_value,
        );
      }
    }

    $context['results']['intnl_letter_query_values'] = $intnl_letter_services;

    $context['message'] = $message;
  }
  else {
    $context['error'] = t('International letter data sources caused error.');
  }
}

/**
 * Process each of the source values for a service to determine all the
 * possible values.
 */
function _uc_auspost_pac_batch_process_services($message, $api_call, $source_key, $result_key, &$context) {
  // Setup progress tracking.
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['max'] = count($context['results'][$source_key]);
    $context['results'][$result_key] = array();
  }
  $context['message'] = $message;

  // Grab the set of values to use and pass the to the API.
  $values = $context['results'][$source_key][$context['sandbox']['progress']];

  require_once dirname(__FILE__) . '/uc_auspost_pac.quote.inc';
  $request = _uc_auspost_pac_api($api_call, $values);
  $result = &$context['results'][$result_key];
  if (!$request['error']) {
    // LOOK OUT! If there's only a single service or option, there's no index.
    if (!isset($request['services']['service'][1])) {
      $request['services']['service'] = array($request['services']['service']);
    }

    foreach ($request['services']['service'] as $service) {
      $service_code = $service['code'];
      if (!isset($result[$service_code])) {
        $result[$service_code] = array(
          'name' => $service['name'],
        );
      }

      if (isset($service['max_extra_cover'])) {
        $result[$service_code]['max_extra_cover'] = $service['max_extra_cover'];
      }

      if (isset($service['options']['option'])) {
        // LOOK OUT! If there's only one option, there's no index.
        if (!isset($service['options']['option'][1])) {
          $service['options']['option'] = array($service['options']['option']);
        }
        foreach ($service['options']['option'] as $option) {
          $option_code = $option['code'];
          if (!isset($result[$service_code]['options'][$option_code])) {
            $result[$service_code]['options'][$option_code] = array(
              'name' => $option['name'],
            );
          }

          if (isset($option['suboptions']['option'])) {
            // LOOK OUT! If there's only one suboption, there's no index.
            if (!isset($option['suboptions']['option'][1])) {
              $option['suboptions']['option'] = array($option['suboptions']['option']);
            }
            foreach ($option['suboptions']['option'] as $suboption) {
              $suboption_code = $suboption['code'];
              if (!isset($result[$service_code]['options'][$option_code]['suboptions'][$suboption_code])) {
                $result[$service_code]['options'][$option_code]['suboptions'][$suboption_code] = array(
                  'name' => $suboption['name'],
                );
              }
            }
          }
        }
      }
    }
  }

  // Update the progress indicators.
  $context['sandbox']['progress']++;
  if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

/**
 * Batch processing is finished. Store the values or report errors.
 */
function _uc_auspost_pac_batch_finished($success, $results, $operations) {
  if ($success) {
    $errors = 0;

    $cids = array(
      // Save the country.
      'country' => t('Failed to retrieve country listing.'),
      // Save the Domestic Letter Services.
      'auspost_domestic_letter' => t('Failed to retrieve Domestic Letter Services listing.'),
      // Save the Domestic Parcel Services.
      'auspost_domestic_parcel' => t('Failed to retrieve Domestic Parcel Services listing.'),
      // Save the International Letter Services.
      'auspost_intnl_letter' => t('Failed to retrieve International Letter Services listing.'),
      // Save the International Parcel Services.
      'auspost_intnl_parcel' => t('Failed to retrieve International Parcel Services listing.'),
    );

    foreach ($cids as $cid => $error_msg) {
      if ($results[$cid]) {
        cache_set('appac:' . $cid, $results[$cid], 'cache_uc_auspost_pac_glacial', CACHE_PERMANENT);
      }
      else {
        $errors++;
        drupal_set_message($error_msg, 'error');
      }
    }

    if (!$errors) {
      drupal_set_message(t('Glacial Cache rebuilt from API server.'));

      // Save the cheerleader, save the world. Cache rebuilt marker.
      cache_set('appac:built-marker', array('source' => 'generated', 'created' => REQUEST_TIME), 'cache_uc_auspost_pac_glacial', CACHE_PERMANENT);
    }
  }
  else {
    $error_op = reset($operations);
    drupal_set_message(t('An error occured while processing %error_op', array('%error_op' => $error_op[0])), 'error');
  }
}
