<?php

/**
 * @file
 * FedEx Web Services Rate / Available Services Quote.
 *
 * Shipping quote module that interfaces with the FedEx Web Services API
 * to get rates for small package shipments.  Implements a SOAP Web Service
 * client.
 *
 * @author Tim Rohaly.    <http://drupal.org/user/202830>
 */


/******************************************************************************
 * Address Validation (aval service)                                          *
 ******************************************************************************/


/**
 * Constructs and executes a SOAP AddressValidationService request.
 * Returns Address Validation information.
 *
 * SOAP call parameters are set in the order they appear in the WSDL file.
 * Associative array of DOM returned.
 *
 * @param $address
 *   Object containing Ubercart address information.
 *
 * @return
 *   Associative array mirroring contents of SOAP object returned from server.
 */
function uc_fedex_address_request($address) {

  //drupal_set_message('<pre>' . var_export($address, TRUE) . '</pre>');

  // Set up SOAP call.
  // Allow tracing so details of request can be retrieved for error logging
  $client = new SoapClient(
    drupal_get_path('module', 'uc_fedex') .
      '/wsdl-' . variable_get('uc_fedex_server_role', 'testing') .
      '/AddressValidationService_v2.wsdl',
    array('trace' => 1)
  );

  // FedEx user key and password filled in by user on admin form
  $request['WebAuthenticationDetail'] = array(
    'UserCredential' => array(
      'Key'      => variable_get('uc_fedex_user_credential_key', 0),
      'Password' => variable_get('uc_fedex_user_credential_password', 0),
    )
  );

  // FedEx account and meter number filled in by user on admin form
  $request['ClientDetail'] = array(
      'AccountNumber' => variable_get('uc_fedex_account_number', 0),
      'MeterNumber'   => variable_get('uc_fedex_meter_number', 0),
  );

  // Optional parameter, contains anything
  $request['TransactionDetail'] = array(
    'CustomerTransactionId' => '*** Address Validation Request v2 from Ubercart ***'
  );

  // Address Validation Request v2.0.0
  $request['Version'] = array(
    'ServiceId'    => 'aval',
    'Major'        => '2',
    'Intermediate' => '0',
    'Minor'        => '0',
  );

  // Timestamp
  $request['RequestTimestamp'] = date('c');

  // Check address accuracy settings
  $request['Options'] = array(
    'VerifyAddresses'             => 1,
    'CheckResidentialStatus'      => 1,
    'MaximumNumberOfMatches'      => 5,
    'StreetAccuracy'              => 'LOOSE',
    'DirectionalAccuracy'         => 'LOOSE',
    'CompanyNameAccuracy'         => 'LOOSE',
    'ConvertToUpperCase'          => 0,
    'RecognizeAlternateCityNames' => 1,
    'ReturnParsedElements'        => 1
  );

  $country = uc_get_country_data(array('country_id' => $address->country));
  $address->country_iso_code_2 = $country[0]['country_iso_code_2'];

  $street_lines   = array();
  $street_lines[] = $address->street1;
  if (!empty($address->street2)) $street_lines[] = $address->street2;

  // Get address
// CompanyName needs to be unset if empty
  $request['AddressesToValidate'][] = array(
    'AddressId'   => $address->last_name . ', ' . $address->first_name,
//  'CompanyName' => $address->company,
    'Address'     => array(
      'StreetLines'         => $street_lines,
      'City'                => $address->city,
      'StateOrProvinceCode' => uc_get_zone_code($address->zone),
      'PostalCode'          => $address->postal_code,
      'CountryCode'         => $address->country_iso_code_2,
    ),
  );

  //
  // Send the SOAP request to the FedEx server
  //
  try {
    $response = $client->addressValidation($request);

    if ($response->HighestSeverity != 'FAILURE' &&
        $response->HighestSeverity != 'ERROR')     {
      print_request_response($client);
    }
    else {
      drupal_set_message(t('Error in processing FedEx Address Validation.'), 'error');
      foreach ($response->Notifications as $notification) {
        if (is_array($response->Notifications)) {
          drupal_set_message($notification->Severity . ': ' .
                             $notification->Message, 'error');
        }
        else {
          drupal_set_message($notification, 'error');
        }
      }
    }
    return $response;
  }
  catch (SoapFault $exception) {
    drupal_set_message('<h2>Fault</h2><br /><b>Code:</b>' . $exception->faultcode . '<br /><b>String:</b>' . $exception->faultstring . '<br />', 'error');
  }
}


/**
 * Convenience function to determine if an address is Residential or Commercial.
 *
 * This determination is made using the FedEx Address Validation API if
 * address validation is enabled in the module admin settings.  Otherwise,
 * the $default input parameter is used.
 *
 * @param $address
 *   An object holding address data.
 * @param $default
 *   TRUE if $address is Residential, FALSE otherwise.
 * @param $verbose
 *   If TRUE, display result of address determination in a Drupal message.
 *
 * @return
 *   TRUE if $address is Residential, FALSE otherwise.
 */
function uc_fedex_address_is_residential($address, $default = TRUE, $verbose = FALSE) {
  // If desired, use Address API to determine whether FedEx considers
  // the address to be Residential or Commercial
  if (variable_get('uc_fedex_address_validation', FALSE)) {
    // Ensure we have the information needed to validate the address
    if (empty($address->street1)     ||
        empty($address->city)        ||
        empty($address->zone)        ||
        empty($address->postal_code) ||
        empty($address->country)       ) {
      return $default;
    }
    // Make request to FedEx web service
    $response = uc_fedex_address_request($address);
    $residential_status = $response->AddressResults->ProposedAddressDetails->ResidentialStatus;

    switch ($residential_status) {
      case 'RESIDENTIAL':
        if ($verbose) {
          drupal_set_message(t('FedEx classifies this address as RESIDENTIAL'));
        }
        return TRUE;
        break;
      case 'BUSINESS':
        if ($verbose) {
          drupal_set_message(t('FedEx classifies this address as COMMERCIAL'));
        }
        return FALSE;
        break;
      default;
        // Any other result, fall through to use the store default
        if ($verbose) {
          drupal_set_message(t("FedEx can't determine the address type"));
        }
        break;
    }
  }

  // Address API validation not wanted, or failed, so return default choice
  return $default;
}


/**
 * Implements hook_form_[form_id]_alter().
 *
 * Adds validation of delivery addresses via the FedEx Address Validation
 * Web Service.  Validation is performed in two places:
 *  - 1: On the checkout page, via jQuery / Ajax, when the required address
 *    fields are filled in.
 *  - 2: Upon submission of the checkout form.
 */
function uc_fedex_form_uc_cart_checkout_form_alter(&$form, &$form_state) {

  if (variable_get('uc_fedex_checkout_validation', FALSE)) {
    // Add jQuery to run address validator
    //drupal_add_js(drupal_get_path('module', 'uc_fedex') .'/uc_fedex.js');
  }

  if (variable_get('uc_fedex_address_validation', FALSE)) {
    // Append address validator function to list of form validators
    //$form['#validate'][] = 'uc_fedex_address_validate';
  }
}


/**
 * Form validation function to validate delivery address entered into
 * uc_cart_checkout_form().
 *
 * @see uc_cart_checkout_form()
 */
function uc_fedex_address_validate($form, &$form_state) {

  // Build $address object from $form_state
  foreach ($form_state['values']['panes']['delivery'] as $key => $value) {
    if (substr($key, 0, 9) == 'delivery_') {
      $field = substr($key, 9);
      $address->$field = $value;
    }
  }

drupal_set_message('<pre>address to validate: ' . var_export($address, TRUE) . '</pre>');

  // Call FedEx Address Validation Web Service
  $response = uc_fedex_address_request($address);
  $address_details = $response->AddressResults->ProposedAddressDetails;

drupal_set_message('<pre>proposed address after validation: ' . var_export($address_details->Address, TRUE) . '</pre>');

drupal_set_message('<pre>response: ' . var_export($address_details->Address->StreetLines, TRUE) . '</pre>');

drupal_set_message('<pre>response: ' . var_export($form['panes']['delivery']['delivery_street1'], TRUE) . '</pre>');

  $form_state['values']['panes']['delivery']['delivery_street1'] = 'HELLO';

  // If Address Validation succeedes, rewrite address in canonical form
  form_set_value($form['panes']['delivery']['delivery_street1'],
                 $address_details->Address->StreetLines,
                 $form_state);
  $form_state['rebuild'] = TRUE;
  //form_set_error('', t("Delivery address has been changed to standard form.  Please verify that your delivery address is correct then re-submit this form."));

  // If Address Validation fails, highlight errors
//  if (!$response['valid']) form_set_error('', t('Error here!'));
  form_set_error('panes][delivery][delivery_company', t("Company Name doesn't match address"));

}
