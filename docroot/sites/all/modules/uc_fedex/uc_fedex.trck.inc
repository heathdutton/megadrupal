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
 * Tracking Functions (trck service)                                          *
 ******************************************************************************/


/**
 * Constructs and executes a SOAP TrackService request.
 * Returns Tracking information.
 *
 * SOAP call parameters are set in the order they appear in the WSDL file.
 * Associative array of DOM returned.
 *
 * @param $tracking_number
 *   FedEx Tracking number.
 *
 * @return
 *   Associative array mirroring contents of SOAP object returned from server.
 */
function uc_fedex_tracking_request($tracking_number) {

  // Set up SOAP call.
  // Allow tracing so details of request can be retrieved for error logging
  $client = new SoapClient(
    drupal_get_path('module', 'uc_fedex') .
      '/wsdl-' . variable_get('uc_fedex_server_role', 'testing') .
      '/TrackService_v4.wsdl',
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
    'CustomerTransactionId' => '*** Track Service Request v4 from Ubercart ***'
  );

  // Track Request v4.0.0
  $request['Version'] = array(
    'ServiceId'    => 'trck',
    'Major'        => '4',
    'Intermediate' => '0',
    'Minor'        => '0',
  );

  // Tracking Number
  $request['PackageIdentifier'] = array(
    'Value' => $tracking_number,
    'Type'  => 'TRACKING_NUMBER_OR_DOORTAG',
  );

  // Include Details
  $request['IncludeDetailedScans'] = TRUE;

  //
  // Send the SOAP request to the FedEx server
  //
  try {
    $response = $client->track($request);

    if ($response->HighestSeverity != 'FAILURE' &&
        $response->HighestSeverity != 'ERROR')     {
      print_request_response($client);
/*
      // Just an Example of how to use this information.
      // You would typically do the following in whatever routine
      // called this function, not here.
      $reply = $response->TrackDetails;

      print('Tracking Number = ' . $reply->TrackingNumber . "<br />");
      print('                  ' . $reply->StatusDescription);
      print('  by ' . $reply->ServiceInfo);
      print('  weight ' . $reply->PackageWeight->Value. ' ' . $reply->PackageWeight->Units . "<br />");
      print('Estimated Delivery ' . $reply->EstimatedDeliveryTimestamp . "<br />");

      foreach($reply->Events as $event) {
        print('    Event = ' . $event->EventDescription . " " . $event->Address->City . ", " . $event->Address->StateOrProvinceCode . " on " . $event->Timestamp. "<br />");
      }
*/
    }
    else {
      drupal_set_message(t('Error in processing FedEx tracking transaction.'), 'error');
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
