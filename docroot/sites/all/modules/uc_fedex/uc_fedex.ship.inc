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
 * Label Printing (ship service)                                              *
 ******************************************************************************/


/**
 * Constructs and executes a SOAP ShipService request.
 *
 * @param $packages
 *   Array of packages received from the cart.
 * @param $origin
 *   Delivery origin address information.
 * @param $destination
 *   Delivery destination address information.
 * @param $fedex_service
 *   FedEx service code (refers to FedEx Ground, Standard Overnight, etc.).
 *
 * @return
 *   Response object from SOAP request.
 */
function uc_fedex_shipment_request($packages, $origin, $destination, $fedex_service) {

  // Assemble information about origin
  $country = uc_get_country_data(array('country_id' => $origin->country));
  $origin->country_iso_code_2       = $country[0]['country_iso_code_2'];

  // Assemble information about destination
  $country = uc_get_country_data(array('country_id' => $destination->country));
  $destination->country_iso_code_2  = $country[0]['country_iso_code_2'];

  // Set up SOAP call.
  // Allow tracing so details of request can be retrieved for error logging
  $client = new SoapClient(
    drupal_get_path('module', 'uc_fedex') .
      '/wsdl-' . variable_get('uc_fedex_server_role', 'testing') .
      '/ShipService_v7.wsdl',
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
    'CustomerTransactionId' => '*** Shipping Request v7 from Ubercart ***'
  );

  // Shipping Request v7.0.0
  $request['Version'] = array(
    'ServiceId'    => 'ship',
    'Major'        => '7',
    'Intermediate' => '0',
    'Minor'        => '0',
  );

  // Timestamp
  $request['RequestedShipment']['ShipTimestamp'] = date('c');

  // Drupal 6.x version of format_date() doesn't support 'c', so until
  // then we use date() directly.  date() doesn't take care of site
  // timezone, though.
  //$request['RequestedShipment']['ShipTimestamp'] = format_date(time(), 'custom', 'c');

  // Set Pickup/Dropoff type
  $request['RequestedShipment']['DropoffType'] = variable_get('uc_fedex_dropoff_type', 'REGULAR_PICKUP');

  // Set Packaging type
  // First element of the packages array is used because all packages
  // in a multi-package shipment must have the same packaging type
  $request['RequestedShipment']['PackagingType'] = reset($packages)->pkg_type;

  // Set Service
  $request['RequestedShipment']['ServiceType']   = $fedex_service;

  $street_lines   = array();
  $street_lines[] = $origin->street1;
  if (!empty($origin->street2)) $street_lines[] = $origin->street2;

  // Get address
  $request['RequestedShipment']['Shipper'] = array(
    'Contact' => array(
      'CompanyName' => $origin->company,
      'PhoneNumber' => $origin->phone,
    ),
    'Address' => array(
      'StreetLines'         => $street_lines,
      'City'                => $origin->city,
      'StateOrProvinceCode' => uc_get_zone_code($origin->zone),
      'PostalCode'          => $origin->postal_code,
      'CountryCode'         => $origin->country_iso_code_2,
    )
  );

  $street_lines   = array();
  $street_lines[] = $destination->street1;
  if (!empty($destination->street2)) $street_lines[] = $destination->street2;

  // Grab details of package destination
  $request['RequestedShipment']['Recipient'] = array(
    'Contact' => array(
      'PersonName'  => $destination->first_name . ' ' . $destination->last_name,
      'CompanyName' => $destination->company,
      'PhoneNumber' => $destination->phone,
    ),
    'Address' => array(
      'StreetLines'         => $street_lines,
      'City'                => $destination->city,
      'StateOrProvinceCode' => uc_get_zone_code($destination->zone),
      'PostalCode'          => $destination->postal_code,
      'CountryCode'         => $destination->country_iso_code_2,
      'Residential'         => $destination->residential,
    )
  );
  // CompanyName needs to be unset if empty
  if (empty($destination->company)) {
    unset($request['RequestedShipment']['Recipient']['Contact']['CompanyName']);
  }

  // Label specifications
  $request['RequestedShipment']['LabelSpecification'] = array(
    'LabelFormatType'          => 'COMMON2D',
    'ImageType'                => variable_get('uc_fedex_label_image_format',
                                               'PDF'),
    'LabelStockType'           => variable_get('uc_fedex_label_stock',
                                               'PAPER_7X4.75'),
    'LabelPrintingOrientation' => variable_get('uc_fedex_label_orientation',
                                               'TOP_EDGE_OF_TEXT_FIRST'),
  );

  // Bill shipping to?
  $request['RequestedShipment']['ShippingChargesPayment'] = array(
    'PaymentType' => 'SENDER',
    'Payor'       => array(
      'AccountNumber' => variable_get('uc_fedex_account_number', 0),
      'CountryCode'   => $origin->country_iso_code_2,
    ),
  );

  // Note that ACCOUNT rates *require* a valid account number
  // and return accurate answers on the production server
  $request['RequestedShipment']['RateRequestTypes'] = strtoupper(variable_get('uc_fedex_quote_type', 'list'));

  $request['RequestedShipment']['PackageDetail'] = 'INDIVIDUAL_PACKAGES';
  $request['RequestedShipment']['PackageCount']  = count($packages);

  // Fill in Package details
  $request['RequestedShipment']['RequestedPackageLineItems'] = array();

  // Determine weight and length units to send to FedEx
  $weight_units = strtoupper(variable_get('uc_weight_unit', 'LB'));
  $weight_conversion_factor = 1;
  if ($weight_units != 'LB' && $weight_units != 'KG') {
    $weight_conversion_factor = uc_weight_conversion($weight_units, 'LB');
    $weight_units = 'LB';
  }

  $length_units = strtoupper(variable_get('uc_length_unit', 'IN'));
  $length_conversion_factor = 1;
  if ($length_units != 'IN' && $length_units != 'CM') {
    $length_conversion_factor = uc_length_conversion($length_units, 'IN');
    $length_units = 'IN';
  }

  // Iterate over $packages to account for multi-package shipments
  $sequence = 0;
  $package_properties = array();
  foreach ($packages as $package) {
    $sequence++;
    $package_properties[$sequence] = array(
      'SequenceNumber'     => $sequence,
      'CustomerReferences' => array(
        '0' => array(
          'CustomerReferenceType' => 'INVOICE_NUMBER',
          // Gag - shouldn't be getting $order_id out of _SESSION
          'Value'                 => $_SESSION['fedex']['order_id'],
        ),
        '1' => array(
          'CustomerReferenceType' => 'CUSTOMER_REFERENCE',
          'Value'                 => $_SESSION['fedex']['reference'],
        ),
      ),
      // Weights must be rounded up to nearest integer value
      'Weight' => array(
        'Value'  => ceil($package->weight * $weight_conversion_factor),
        'Units'  => $weight_units,
      ),
      // Lengths must be rounded up to nearest integer value
      'Dimensions' => array(
        'Length' => ceil($package->length * $length_conversion_factor),
        'Width'  => ceil($package->width  * $length_conversion_factor),
        'Height' => ceil($package->height * $length_conversion_factor),
        'Units'  => $length_units,
      ),
    );
    // Only need Package Dimensions if using our own packaging.
    if ($request['RequestedShipment']['PackagingType'] != 'YOUR_PACKAGING') {
      unset($package_properties[$sequence]['Dimensions']);
    }

    // Add Insurance if requested
    if (variable_get('uc_fedex_insurance', FALSE)) {
      $package_properties[$sequence]['InsuredValue'] = array(
        'Amount'   => $package->value,
        'Currency' => 'USD',  // variable_get('uc_currency_code', 'USD'),
      );
    }

    // Multi-package shipments need to reference master tracking #
    if ($sequence > 1) {
      $request['RequestedShipment']['MasterTrackingId']  = $master_tracking_id;
    }

    // Fill in SOAP request with $package_properties
    $request['RequestedShipment']['RequestedPackageLineItems'][0] = $package_properties[$sequence];

    //
    // For Multi-Package shipments, need to send one SOAP request
    // *per package* to the FedEx server
    //
    try {
      $response[$sequence] = $client->processShipment($request);

      if ($response[$sequence]->HighestSeverity != 'FAILURE' &&
          $response[$sequence]->HighestSeverity != 'ERROR')     {
        print_request_response($client);

        // Save master tracking # to use for remaining package in shipment
        if ($sequence == 1 && count($packages) > 1) {
          $master_tracking_id = $response[$sequence]->CompletedShipmentDetail->MasterTrackingId;
        }
      }
      else {
        drupal_set_message(t('Error in processing FedEx Shipping Request.'), 'error');
        foreach ($response[$sequence]->Notifications as $notification) {
          if (is_array($response[$sequence]->Notifications)) {
            drupal_set_message($notification->Severity . ': ' . $notification->Message, 'error');
          }
          else {
            drupal_set_message($notification, 'error');
          }
        }
      }
    }
    catch (SoapFault $exception) {
      drupal_set_message('<h2>Fault</h2><br /><b>Code:</b>' . $exception->faultcode . '<br /><b>String:</b>' . $exception->faultstring . '<br />', 'error');
    }
  }
  return $response;
}


/**
 * Last chance for user to review shipment.
 *
 * @see theme_uc_fedex_confirm_shipment()
 * @see uc_fedex_confirm_shipment_submit()
 * @ingroup forms
 */
function uc_fedex_confirm_shipment($form, &$form_state, $order) {
  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Generate Label')
  );
  return $form;
}


/**
 * Displays final shipment information for review.
 *
 * @see uc_fedex_confirm_shipment()
 * @ingroup themeable
 */
function theme_uc_fedex_confirm_shipment($variables) {
  $form = $variables['form'];

  $output = '';

  $output .= '<div class="shipping-address"><b>' . t('Ship from') . ':</b><br />';
  $output .= uc_address_format(
    check_plain($_SESSION['fedex']['origin']->first_name),
    check_plain($_SESSION['fedex']['origin']->last_name),
    check_plain($_SESSION['fedex']['origin']->company),
    check_plain($_SESSION['fedex']['origin']->street1),
    check_plain($_SESSION['fedex']['origin']->street2),
    check_plain($_SESSION['fedex']['origin']->city),
    check_plain($_SESSION['fedex']['origin']->zone),
    check_plain($_SESSION['fedex']['origin']->postal_code),
    check_plain($_SESSION['fedex']['origin']->country)
  );
  $output .= '<br />' . check_plain($_SESSION['fedex']['origin']->email);
  $output .= '</div>';

  $output .= '<div class="shipping-address"><b>' . t('Ship to') . ':</b><br />';
  $output .= uc_address_format(
    check_plain($_SESSION['fedex']['destination']->first_name),
    check_plain($_SESSION['fedex']['destination']->last_name),
    check_plain($_SESSION['fedex']['destination']->company),
    check_plain($_SESSION['fedex']['destination']->street1),
    check_plain($_SESSION['fedex']['destination']->street2),
    check_plain($_SESSION['fedex']['destination']->city),
    check_plain($_SESSION['fedex']['destination']->zone),
    check_plain($_SESSION['fedex']['destination']->postal_code),
    check_plain($_SESSION['fedex']['destination']->country)
  );
  $output .= '<br />' . check_plain($_SESSION['fedex']['destination']->email);
  $output .= '</div>';
  $output .= '<div class="shipment-data"><b>' . t('Shipping method') . ':</b><br />';
  $method = uc_fedex_uc_shipping_method();
  $accessorials = array_merge(
    $method['fedex']['quote']['accessorials'],
    $method['fedex_ground']['quote']['accessorials'],
    $method['fedex_freight']['quote']['accessorials']
  );
  $output .= $accessorials[$_SESSION['fedex']['service']] . '<br />';
  $output .= '<i>' . check_plain($_SESSION['fedex']['rate']['type']) . '</i>: ' . uc_currency_format($_SESSION['fedex']['rate']['amount']) . ' (' . check_plain($_SESSION['fedex']['rate']['currency']) . ') -- ';
  $output .= '<i>' . t('Paid') . '</i>: ' . $_SESSION['fedex']['paid'] . '<br />';
  $ship_date = $_SESSION['fedex']['ship_date'];
  $output .= 'Ship date: ' . format_date(gmmktime(12, 0, 0, $ship_date['month'], $ship_date['day'], $ship_date['year']), 'custom', variable_get('uc_date_format_default', 'm/d/Y'));
  $output .= "</div>\n<br style=\"clear: both;\" />";
  $output .= drupal_render_children($form);

  return $output;
}


/**
 * Generates shipping labels.
 *
 * @see uc_fedex_confirm_shipment()
 */
function uc_fedex_confirm_shipment_submit($form, &$form_state) {
  // Recover shipment data from session
  $order_id    = $_SESSION['fedex']['order_id'];
  $packages    = $_SESSION['fedex']['packages'];
  $origin      = $_SESSION['fedex']['origin'];
  $destination = $_SESSION['fedex']['destination'];

  //
  // Assemble the label request and send it off to FedEx
  //
  $mp_response = uc_fedex_shipment_request($_SESSION['fedex']['packages'], $origin, $destination, $_SESSION['fedex']['service']);

  //
  // Loop over responses, to handle case of multi-package shipments
  //
  foreach ($mp_response as $response) {
    // Package tracking numbers
    $tracking[]     = $response->CompletedShipmentDetail->CompletedPackageDetails->TrackingIds->TrackingNumber;
    if (isset($response->CompletedShipmentDetail->RoutingDetail->DeliveryDate)) {
      $exp_delivery[] = date_parse($response->CompletedShipmentDetail->RoutingDetail->DeliveryDate);
    }
    else {
      $exp_delivery[] = array('month' => 0, 'day' => 0, 'year' => 0);
      // Ground and Home Delivery don't have DeliveryDate,
      // they have TransitTime instead
    }
  }

  // Final package in shipment contains summary information
  $last_package = end($mp_response);
  $rating = $last_package->CompletedShipmentDetail->ShipmentRating;
  // Check to see if we're quoting ACCOUNT or LIST rates
  if (variable_get('uc_fedex_quote_type', 'list') == 'list') {  // LIST rate
    // LIST quotes return ACCOUNT rates (in ShipmentRateDetails[0])
    // and LIST rates (in ShipmentRateDetails[1])
    $ratedetail = $rating->ShipmentRateDetails[1];
  }
  else {  // ACCOUNT rate
    // ACCOUNT quotes may return either ACCOUNT rates only OR
    // ACCOUNT rates and LIST rates.  Check.
    if (is_array($rating->ShipmentRateDetails)) {
      $ratedetail = $rating->ShipmentRateDetails[0];
    }
    else {
      $ratedetail = $rating->ShipmentRateDetails;
    }
  }
  $shipment_charge = $ratedetail->TotalNetCharge;

  // Build $shipment object to return to uc_shipment
  $shipment = new stdClass();
  $shipment->order_id        = $order_id;
  $shipment->origin          = clone($_SESSION['fedex']['origin']);
  $shipment->destination     = clone($_SESSION['fedex']['destination']);
  $shipment->packages        = $_SESSION['fedex']['packages'];
  $shipment->shipping_method = 'fedex';
  $shipment->accessorials    = $_SESSION['fedex']['service'];
  $shipment->carrier         = t('FedEx');
  $shipment->cost            = (string) $shipment_charge->Amount;
  $shipment->tracking_number = (string) $tracking[0];

  $ship_date    = $_SESSION['fedex']['ship_date'];
  $shipment->ship_date         = gmmktime(12, 0, 0, $ship_date['month'],
                                                    $ship_date['day'],
                                                    $ship_date['year']);
  $shipment->expected_delivery = gmmktime(12, 0, 0, $exp_delivery[0]['month'],
                                                    $exp_delivery[0]['day'],
                                                    $exp_delivery[0]['year']);

  // Loop over packages in shipment to store labels and tracking numbers
  foreach ($mp_response as $package_results) {
    $package =& current($shipment->packages);
    $package->tracking_number = (string) $package_results->CompletedShipmentDetail->CompletedPackageDetails->TrackingIds->TrackingNumber;

    $label_image = $package_results->CompletedShipmentDetail->CompletedPackageDetails->Label->Parts->Image;
    // Save the label
    $directory = 'public://fedex_labels';
    if (file_prepare_directory($directory, FILE_CREATE_DIRECTORY)) {
      $label_path = $directory . '/label-' . $package->tracking_number . '.' . strtolower(variable_get('uc_fedex_label_image_format', 'PDF'));
      if ($label_file = file_save_data($label_image, $label_path, FILE_EXISTS_REPLACE)) {
        file_usage_add($label_file, 'uc_shipping', 'package', $package->package_id);
        $package->label_image = $label_file;
      }
      else {
        drupal_set_message(t('Could not open a file to save the label image.'), 'error');
      }
    }
    else {
      drupal_set_message(t('Could not find or create the directory %directory in the file system path.', array('%directory' => $directory)), 'error');
    }
    unset($package);
    next($shipment->packages);
  }

  uc_shipping_shipment_save($shipment);

  unset($_SESSION['fedex']);
  $form_state['redirect'] = 'admin/store/orders/' . $order_id . '/shipments';
}


/**
 * Displays the shipping label for printing.
 *
 * Each argument is a component of the file path to the image.
 *
 * @ingroup themeable
 */
function uc_fedex_label_image() {
  $args = explode('/', $_GET['q'], 8);
  if (count($args) != 8) {
    return MENU_NOT_FOUND;
  }

  $image_path = file_stream_wrapper_uri_normalize($args[7]);

  file_transfer($image_path, file_get_content_headers($image_path));
}


/**
 * Shipment creation callback.
 *
 * Confirms shipment data before requesting a shipping label.
 *
 * @param $order
 *   The order object for the shipment.
 * @param $package_ids
 *   Array of package ids to shipped.
 *
 * @see uc_fedex_fulfill_order_validate()
 * @see uc_fedex_fulfill_order_submit()
 * @ingroup forms
 */
function uc_fedex_fulfill_order($form, &$form_state, $order, $package_ids) {
  $form = array();
  $pkg_types = _uc_fedex_package_types();
  $form['order_id'] = array('#type' => 'value', '#value' => $order->order_id);
  $packages  = array();
  $addresses = array();

  // Container for package data
  $form['packages'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Packages'),
    '#collapsible' => TRUE,
    '#tree'        => TRUE
  );
  foreach ($package_ids as $id) {
    $package = uc_shipping_package_load($id);
    if ($package) {
      foreach ($package->addresses as $address) {
        if (!in_array($address, $addresses)) {
          $addresses[] = $address;
        }
      }

      // Create list of products and get a representative product (last one in
      // the loop) to use for some default values
      $product_list = array();
      $declared_value = 0;
      foreach ($package->products as $product) {
        $product_list[]  = $product->qty . ' x ' . $product->model;
        $declared_value += $product->qty * $product->price;
      }
      // Use last product in package to determine package type
      $fedex_data = db_query("SELECT pkg_type FROM {uc_fedex_products} WHERE nid = :nid", array(':nid' => $product->nid))->fetchAssoc();
      $product->fedex = $fedex_data;
      $pkg_form = array(
        '#type'  => 'fieldset',
        '#title' => t('Package !id', array('!id' => $id)),
      );
      $pkg_form['products'] = array(
        '#theme' => 'item_list',
        '#items' => $product_list,
      );
      $pkg_form['package_id'] = array(
        '#type'  => 'hidden',
        '#value' => $id
      );
      $pkg_form['pkg_type'] = array(
        '#type'          => 'select',
        '#title'         => t('Package type'),
        '#options'       => $pkg_types,
        '#default_value' => $product->fedex['pkg_type'],
        '#required'      => TRUE,
      );
      $pkg_form['declared_value'] = array(
        '#type'          => 'textfield',
        '#title'         => t('Declared value'),
        '#default_value' => $declared_value,
        '#required'      => TRUE,
      );
      $pkg_form['weight'] = array(
        '#type'        => 'container',
        '#description' => t('Weight of the package. Default value is sum of product weights in the package.'),
        '#states'      => array(
          'invisible' => array(
            'input[name="shippable"]' => array('checked' => FALSE),
          ),
        ),
        '#weight'      => 15,
        '#theme'       => 'uc_product_inline_form',
      );
      $pkg_form['weight']['weight'] = array(
        '#type'          => 'textfield',
        '#title'         => t('Weight'),
        '#default_value' => isset($package->weight) ? $package->weight : 0,
        '#size'          => 10,
        '#maxlength'     => 15,
      );
      $pkg_form['weight']['units'] = array(
        '#type'          => 'select',
        '#title'         => t('Units'),
        '#options'       => array(
          'lb' => t('Pounds'),
          'kg' => t('Kilograms'),
          'oz' => t('Ounces'),
          'g'  => t('Grams'),
        ),
        '#default_value' => isset($package->weight_units) ?
                            $package->weight_units        :
                            variable_get('uc_weight_unit', 'lb'),
      );
      $pkg_form['dimensions'] = array(
        '#type'          => 'container',
        '#description'   => t('Physical dimensions of the package.'),
        '#states'        => array(
          'invisible' => array(
            'input[name="shippable"]' => array('checked' => FALSE),
          ),
        ),
        '#weight'        => 20,
        '#theme'         => 'uc_product_inline_form',
      );
      $pkg_form['dimensions']['length'] = array(
        '#type'          => 'textfield',
        '#title'         => t('Length'),
        '#default_value' => isset($package->length) ? $package->length : 1,
        '#size'          => 10,
      );
      $pkg_form['dimensions']['width'] = array(
        '#type'          => 'textfield',
        '#title'         => t('Width'),
        '#default_value' => isset($package->width) ? $package->width : 1,
        '#size'          => 10,
      );
      $pkg_form['dimensions']['height'] = array(
        '#type'          => 'textfield',
        '#title'         => t('Height'),
        '#default_value' => isset($package->height) ? $package->height : 1,
        '#size'          => 10,
      );
      $pkg_form['dimensions']['units'] = array(
        '#type'    => 'select',
        '#title'   => t('Units'),
        '#options' => array(
          'in' => t('Inches'),
          'ft' => t('Feet'),
          'cm' => t('Centimeters'),
          'mm' => t('Millimeters'),
        ),
        '#default_value' => isset($package->length_units) ?
                            $package->length_units        :
                            variable_get('uc_length_unit', 'in'),
      );

      $form['packages'][$id] = $pkg_form;
    }
  }

  $form = array_merge($form, uc_shipping_address_form($form, $form_state, $addresses, $order));

  // Mark fields needed by the FedEx API as 'required'
  foreach (array('email', 'last_name', 'phone', 'street1', 'city', 'zone', 'country', 'postal_code') as $field) {
    $form['destination']['delivery_address'][$field]['#required'] = TRUE;
  }

  // Allow selection of address type
  $form['destination']['delivery_residential'] = array(
    '#type'          => 'radios',
    '#title'         => t('Address type'),
    '#default_value' => variable_get('uc_fedex_residential_quotes', TRUE),
    '#options'       => array(
      0 => t('Commercial'),
      1 => t('Residential'),
    ),
    '#required'      => TRUE,
  );

  // Determine shipping option chosen by the customer
  $method  = $order->quote['method'];
  $methods = module_invoke_all('uc_shipping_method');
  if (isset($methods[$method])) {
    $services = $methods[$method]['quote']['accessorials'];
    $method   = $services[$order->quote['accessorials']];
  }

  // Container for shipment data
  $form['shipment'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Shipment data'),
    '#collapsible' => TRUE,
  );

  // Inform user of customer's shipping choice
  $form['shipment']['shipping_choice'] = array(
    '#type'   => 'markup',
    '#prefix' => '<div>',
    '#markup' => t('Customer selected "@method" as the shipping method and paid @rate', array('@method' => $method, '@rate' => uc_currency_format($order->quote['rate']))),
    '#suffix' => '</div>',
  );

  // Pass shipping charge paid information on to validation function so it
  // can be displayed alongside actual costs
  $form['shipment']['paid'] = array(
    '#type'  => 'value',
    '#value' => uc_currency_format($order->quote['rate']),
  );

  $services = array_merge(
    _uc_fedex_ground_services(),
    _uc_fedex_express_services(),
    _uc_fedex_freight_services()
  );
  $default_service = '';
  $method          = $order->quote['method'];
  if ($method == 'fedex_ground' ||
      $method == 'fedex'        ||
      $method == 'fedex_freight') {
    $default_service = $order->quote['accessorials'];
  }

  $form['shipment']['service'] = array(
    '#type'          => 'select',
    '#title'         => t('FedEx service'),
    '#options'       => $services,
    '#default_value' => $default_service,
  );
  $today = getdate();
  $form['shipment']['ship_date'] = array(
    '#type'          => 'date',
    '#title'         => t('Ship date'),
    '#default_value' => array(
      'year'  => $today['year'],
      'month' => $today['mon'],
      'day'   => $today['mday']
    ),
  );
  $form['shipment']['reference'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Reference Number'),
    '#maxlength'     => 40,  // FedEx limit
    '#default_value' => '',
    '#required'      => FALSE,
    '#description'   => t('Optional information to include on shipping label.  Limited to 40 characters.'),
  );

  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Review shipment')
  );

  return $form;
}


/**
 * Passes final information into shipment object.
 *
 * @see uc_fedex_fulfill_order()
 * @see uc_fedex_confirm_shipment()
 */
function uc_fedex_fulfill_order_validate($form, &$form_state) {

  $errors = form_get_errors();
  if (isset($errors)) {
    // Some required elements are missing - don't bother with making
    // a FedEx API call until that gets fixed.
    return;
  }

  $origin      = new stdClass();
  $destination = new stdClass();
  $packages    = array();

  foreach ($form_state['values'] as $key => $value) {
    if (substr($key, 0, 7) == 'pickup_') {
      $field = substr($key, 7);
      $origin->$field = $value;
    }
    elseif (substr($key, 0, 9) == 'delivery_') {
      $field = substr($key, 9);
      $destination->$field = $value;
    }
  }

  // This is a total hack to work around changes made in the return value
  // from uc_shipping_address_form().  That function needs to be fixed, but
  // until then this should do the trick.
  $origin        = $form_state['values']['pickup_address'];
  $origin->phone = $form_state['values']['phone'];
  $origin->first_name = $form_state['values']['first_name'];
  $origin->last_name = $form_state['values']['last_name'];
  $origin->company = $form_state['values']['company'];
  $origin->street1 = $form_state['values']['street1'];
  $origin->street2 = $form_state['values']['street2'];
  $origin->city = $form_state['values']['city'];
  $origin->zone = $form_state['values']['zone'];
  $origin->country = $form_state['values']['country'];
  $origin->postal_code = $form_state['values']['postal_code'];
  $origin->email = $form_state['values']['pickup_email'];

  // Populate $_SESSION variable with information we'll need
  // later to generate the label
  $_SESSION['fedex']           = array();
  $_SESSION['fedex']['origin'] = $origin;

  // Determine if address is Residential or Commercial
  // If Address Validation API is not used or fails, default to form choice
  $destination->residential = uc_fedex_address_is_residential($destination, $destination->residential, FALSE);

  $_SESSION['fedex']['destination'] = $destination;
  foreach ($form_state['values']['packages'] as $id => $pkg_form) {
    $package               = uc_shipping_package_load($id);
    $package->pkg_type     = $pkg_form['pkg_type'];
    $package->value        = $pkg_form['declared_value'];
    $package->weight       = $pkg_form['weight']['weight'];
    $package->weight_units = $pkg_form['weight']['units'];
    $package->length       = $pkg_form['dimensions']['length'];
    $package->width        = $pkg_form['dimensions']['width'];
    $package->height       = $pkg_form['dimensions']['height'];
    $package->length_units = $pkg_form['dimensions']['units'];
    $package->qty          = 1;
    $_SESSION['fedex']['packages'][$id] = $package;
  }
  $_SESSION['fedex']['service']   = $form_state['values']['service'];
  $_SESSION['fedex']['paid']      = $form_state['values']['paid'];
  $_SESSION['fedex']['ship_date'] = $form_state['values']['ship_date'];
  $_SESSION['fedex']['reference'] = $form_state['values']['reference'];
  $_SESSION['fedex']['order_id']  = $form_state['values']['order_id'];

  //
  // Assemble a quote request and send it off to FedEx
  //
  $response = uc_fedex_shipment_request($_SESSION['fedex']['packages'], $origin, $destination, $form_state['values']['service']);

  // Response may contain multiple packages
  foreach ($response as $package) {

    // Need to fail validation if SOAP request returns ERROR
    if ($package->HighestSeverity == 'FAILURE' ||
        $package->HighestSeverity == 'ERROR')     {
      form_set_error('uc_fedex_fulfill_order', $package->Notifications->Message);
    }

    // Shipping Charge
    $rating = $package->CompletedShipmentDetail->CompletedPackageDetails->PackageRating;
    // Check to see if we're quoting ACCOUNT or LIST rates
    if (variable_get('uc_fedex_quote_type', 'list') == 'list') {  // LIST rate
      // LIST quotes return ACCOUNT rates (in PackageRateDetails[0])
      // and LIST rates (in PackageRateDetails[1])
      $ratedetail = $rating->PackageRateDetails[1];
    }
    else {  // ACCOUNT rate
      // ACCOUNT quotes may return either ACCOUNT rates only OR
      // ACCOUNT rates and LIST rates.  Check.
      if (is_array($rating->PackageRateDetails)) {
        $ratedetail = $rating->PackageRateDetails[0];
      }
      else {
        $ratedetail = $rating->PackageRateDetails;
      }
    }
    $charge[] = $ratedetail->NetCharge;
  }

  $last_package = end($response);
  $rating = $last_package->CompletedShipmentDetail->ShipmentRating;
  // Check to see if we're quoting ACCOUNT or LIST rates
  if (variable_get('uc_fedex_quote_type', 'list') == 'list') {  // LIST rate
    // LIST quotes return ACCOUNT rates (in ShipmentRateDetails[0])
    // and LIST rates (in ShipmentRateDetails[1])
    $ratedetail = $rating->ShipmentRateDetails[1];
    $_SESSION['fedex']['rate']['type'] = t('List Rates');
  }
  else {  // ACCOUNT rate
    // ACCOUNT quotes may return either ACCOUNT rates only OR
    // ACCOUNT rates and LIST rates.  Check.
    if (is_array($rating->ShipmentRateDetails)) {
      $ratedetail = $rating->ShipmentRateDetails[0];
    }
    else {
      $ratedetail = $rating->ShipmentRateDetails;
    }
    $_SESSION['fedex']['rate']['type'] = t('Account Rates');
  }
  $shipment_charge = $ratedetail->TotalNetCharge;


  $_SESSION['fedex']['rate']['currency'] = (string) $shipment_charge->Currency;
  $_SESSION['fedex']['rate']['amount']   = (string) $shipment_charge->Amount;
}


/**
 * Passes final information into shipment object.
 *
 * @see uc_fedex_fulfill_order()
 * @see uc_fedex_confirm_shipment()
 */
function uc_fedex_fulfill_order_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/store/orders/' . $form_state['values']['order_id'] . '/shipments/fedex';
}


/**
 * Convenience function to get FedEx codes for payor.
 *
 * @return
 *   An array of human-friendly names for available FedEx payor option codes.
 */
function _uc_fedex_payor_types() {
  return array(
    'RECIPIENT'   => t('Shipping billed to Recipient'),
    'SENDER'      => t('Shipping billed to Sender'),
    'THIRD_PARTY' => t('Shipping billed to third party'),
  );
}


/**
 * Convenience function to get FedEx codes for label image format.
 *
 * @return
 *   An array of human-friendly names for available FedEx label image formats.
 */
function _uc_fedex_label_image_types() {
  return array(
    'PNG'   => t('PNG - Portable Next Generation'),
    'PDF'   => t('PDF - Adobe Portable Document Format'),
    'EPL2'  => t('EPL2 - Eltron Thermal Printer Format'),
    'DPL'   => t('DPL - Unimark Thermal Printer Format'),
    'ZPLII' => t('ZPLII - Zebra Thermal Printer Format'),
  );
}


/**
 * Convenience function to get FedEx codes for label orientation.
 *
 * @return
 *   An array of human-friendly names for the different FedEx label orientation.
 */
function _uc_fedex_label_orientation_types() {
  return array(
    'BOTTOM_EDGE_OF_TEXT_FIRST' => t('BOTTOM_EDGE_OF_TEXT_FIRST'),
    'TOP_EDGE_OF_TEXT_FIRST'    => t('TOP_EDGE_OF_TEXT_FIRST'),
  );
}


/**
 * Convenience function to get FedEx codes for label paper stock type.
 *
 * @return
 *   An array of human-friendly names for available FedEx label
 *   paper stock types.
 */
function _uc_fedex_label_stock_types() {
  return array(
    'PAPER_4X6'                      => t('Laser 4" x 6"'),
    'PAPER_4X8'                      => t('Laser 4" x 8"'),
    'PAPER_4X9'                      => t('Laser 4" x 9"'),
    'PAPER_7X4.75'                   => t('Laser 7" x 4.75"'),
    'PAPER_8.5X11_BOTTOM_HALF_LABEL' => t('Laser 8.5" x 11", Label on Bottom Half'),
    'PAPER_8.5X11_TOP_HALF_LABEL'    => t('Laser 8.5" x 11", Label on Top Half'),
    'STOCK_4X6'                      => t('Thermal 4" x 6"'),
    'STOCK_4X6.75_LEADING_DOC_TAB'   => t('Thermal 4" x 6.75", Leading DocTab'),
    'STOCK_4X6.75_TRAILING_DOC_TAB'  => t('Thermal 4" x 6.75", Trailing DocTab'),
    'STOCK_4X8'                      => t('Thermal 4" x 8"'),
    'STOCK_4X9_LEADING_DOC_TAB'      => t('Thermal 4" x 9", Leading DocTab'),
    'STOCK_4X9_TRAILING_DOC_TAB'     => t('Thermal 4" x 9", Trailing DocTab'),
  );
}
