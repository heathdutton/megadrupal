<?php

/**
 * @file
 * Builds placeholder replacement tokens for shipwire shipment related data.
 */

/**
 * Implements hook_token_info().
 */
function shipwire_token_info() {
  $type = array(
    'name' => t('Shipwire shipment'),
    'description' => t('Tokens related to Shipwire shipments.'),
    'needs-data' => 'shipwire-shipment',
  );

  $shipment = array();
  $shipment['shipwire_shipment_id'] = array(
    'name' => t('Shipment ID'),
    'description' => t('The unique numeric ID of the shipment.'),
  );
  $shipment['order-id'] = array(
    'name' => t('Order ID'),
    'description' => t("The unique ID of the Order for which the shipment was made."),
  );
  $shipment['shipwire-id'] = array(
    'name' => t('Shipwire ID'),
    'description' => t('The unique shipment ID used by Shipwire.'),
  );
  $shipment['carrier-code'] = array(
    'name' => t('Carrier code'),
    'description' => t('The code reprsesenting the company and shipping method used for the shipment.'),
  );
  $shipment['carrier-name'] = array(
    'name' => t('Carrier name'),
    'description' => t('The name and service of the company that was used for the shipment.'),
  );
  $shipment['date-submitted'] = array(
    'name' => t('Date submitted'),
    'description' => t('The date the shipment was submitted to Shipwire.'),
    'type' => 'date',
  );
  $shipment['date-shipped'] = array(
    'name' => t('Date created'),
    'description' => t('The date the shipment was shipped from Shipwire.'),
    'type' => 'date',
  );
  $shipment['date-expected'] = array(
    'name' => t('Date expected'),
    'description' => t('The date the shipment is estimated to arrive.'),
    'type' => 'date',
  );
  $shipment['date-delivered'] = array(
    'name' => t('Date delivered'),
    'description' => t('The date the shipment was delivered.'),
    'type' => 'date',
  );
  $shipment['tracking-number'] = array(
    'name' => t('Tracking number'),
    'description' => t('The tracking number for the shipment.'),
  );
  $shipment['tracking-uri'] = array(
    'name' => t('Tracking URL'),
    'description' => t('The URL to check the status of the shipment.'),
  );
  $shipment['tracking-number-link'] = array(
    'name' => t('Tracking number linked'),
    'description' => t('A formatted link with the tracking number as text linked to the Tracking URL.'),
  );
//  $shipment['products-plain-list'] = array(
//    'name' => t('Shipment products plain text'),
//    'description' => t('A list of products contained in the shipment in plain text format.'),
//  );
//  $shipment['products-unordered-list'] = array(
//    'name' => t('Shipment products HTML'),
//    'description' => t('A list of products contained in the shipment in HTML bullet list format.'),
//  );

  return array(
    'types' => array('shipwire-shipment' => $type),
    'tokens' => array('shipwire-shipment' => $shipment),
  );
}

/**
 * Implements hook_tokens().
 */
function shipwire_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $url_options = array('absolute' => TRUE);

  if (isset($options['language'])) {
    $url_options['language'] = $options['language'];
    $language_code = $options['language']->language;
  }
  else {
    $language_code = NULL;
  }

  $sanitize = !empty($options['sanitize']);

  $replacements = array();

  if ($type == 'shipwire-shipment' && !empty($data['shipwire-shipment'])) {
    $shipment = $data['shipwire-shipment'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        // Simple key values on the customer profile.
        case 'shipwire_shipment_id':
          $replacements[$original] = $shipment->shipwire_shipment_id;
          break;

        case 'order-id':
          $replacements[$original] = $shipment->order_id;
          break;

        case 'shipwire-id':
          $replacements[$original] = $shipment->shipwire_id;
          break;

        case 'carrier-code':
          $replacements[$original] = $shipment->carrier_code;
          break;

        case 'carrier-name':
          $replacements[$original] = Shipwire::getCarrierName($shipment->carrier_code);
          break;

        case 'date-submitted':
          $replacements[$original] = empty($shipment->date_submitted) ? '' : format_date($shipment->date_submitted, 'medium', '', NULL, $language_code);
          break;

        case 'date-shipped':
          $replacements[$original] = empty($shipment->date_shipped) ? '' : format_date($shipment->date_shipped, 'medium', '', NULL, $language_code);
          break;

        case 'date-expected':
          $replacements[$original] = empty($shipment->date_expected) ? '' : format_date($shipment->date_expected, 'medium', '', NULL, $language_code);
          break;

        case 'date-delivered':
          $replacements[$original] = empty($shipment->date_delivered) ? '' : format_date($shipment->date_delivered, 'medium', '', NULL, $language_code);
          break;

        case 'tracking-number':
          $replacements[$original] = $shipment->tracking_number;
          break;

        case 'tracking-uri':
          $replacements[$original] = $sanitize ? filter_xss($shipment->tracking_uri) : $shipment->tracking_uri;
          break;

        case 'tracking-number-link':
          $replacements[$original] = l($shipment->tracking_number, $shipment->tracking_uri);
          break;
      }
    }
  }

  return $replacements;
}
