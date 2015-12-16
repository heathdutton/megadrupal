<?php

/**
 * @file
 * Export and import stamps.com XML files.
 */

/**
 * Export queued shipments directly to the browser.
 *
 * This sets their status to 'exported'.
 */
function uc_stamps_export_to_browser() {
  $datestr = 'stamps' . date('YmdHi') . '.xml';

  $xml_doc = uc_stamps_shipments_export();
  if (is_null($xml_doc)) {
    drupal_set_message(t('No shipments to export.'), 'warning');
    return;
  }
  header('Content-Type: text/xml');
  header("Content-Disposition: attachment; filename=\"$datestr\"");
  echo $xml_doc->saveXML();
  drupal_exit();
}

/**
 * Export batched shipments as stamps.com XML.
 *
 * This finds all the packages in "queued" state, outputs XML
 * for their shipping labels, and changes their state to "exported".
 *
 * @return object
 *   DOMDocument object containing export data.
 *
 * @todo Deal with multiple COD packages, probably by putting a sanity
 *   check in the form.
 * @todo Add configuration and layout options.  This is already handled
 *   by the Stamps.com client, but it would be nice to have it here, too.
 */
function uc_stamps_shipments_export() {
  // Create the root document.
  $stamps_doc = new DOMDocument('1.0', 'UTF-8');
  // Nice for debugging.
  $stamps_doc->formatOutput = TRUE;
  // ...and the <Print> node.
  $print_node = $stamps_doc->createElementNS(
    'http://stamps.com/xml/namespace/2009/8/Client/BatchProcessingV1',
    'Print');
  $stamps_doc->appendChild($print_node);

  // *****
  // Add the batch's <Configuration> node.
  // *****
  $config_node = $stamps_doc->createElement('Configuration');
  $print_node->appendChild($config_node);

  // Toggle printing sample (fake test) labels.
  $sample_value = variable_get('uc_stamps_print_samples', TRUE) ?
    'true' : 'false';
  $sample_node = $stamps_doc->createElement('Sample', $sample_value);
  $config_node->appendChild($sample_node);

  // Set up notifications to sender (e-mail to the store).
  $sender_email_options_node = $stamps_doc->createElement('SenderEmailOptions');
  // Delivery notification.
  $delivery_notification_value
    = variable_get('uc_stamps_email_delivery_notification', FALSE) ?
    'true' : 'false';
  $sender_email_options_node->appendChild($stamps_doc->createElement(
    'DeliveryNotification', $delivery_notification_value));
  // Shipment notification (the labels were printed).
  $shipment_notification_value
    = variable_get('uc_stamps_email_print_notification', FALSE) ?
    'true' : 'false';
  $sender_email_options_node->appendChild($stamps_doc->createElement(
    'ShipmentNotification', $shipment_notification_value));
  $config_node->appendChild($sender_email_options_node);

  // Set up notifications to recipient (customer).
  $recipient_email_options_node
    = $stamps_doc->createElement('RecipientEmailOptions');
  $recipient_notification_value
    = variable_get('uc_stamps_email_customer', TRUE) ?
    'true' : 'false';
  $recipient_notification_node
    = $stamps_doc->createElement('ShipmentNotification',
                                $recipient_notification_value);
  $company_in_subject = variable_get('uc_stamps_email_company_in_subject', TRUE) ?
    'true' : 'false';
  $subject_attribute = $stamps_doc->createAttribute('companyInSubject');
  $subject_attribute->value = $company_in_subject;
  $recipient_notification_node->appendChild($subject_attribute);
  $company_in_from = variable_get('uc_stamps_email_company_in_from', TRUE) ?
    'true' : 'false';
  $from_attribute = $stamps_doc->createAttribute('fromCompany');
  $from_attribute->value = $company_in_from;
  $recipient_notification_node->appendChild($from_attribute);
  $recipient_email_options_node->appendChild($recipient_notification_node);

  $config_node->appendChild($recipient_email_options_node);

  // *****
  // Add the <Layout> node.
  // *****
  // TODO
  // Create the DesiredPrinter node.
  // Set the printer Name.
  // Set the printer Bin.
  $layout_node = $stamps_doc->createElement('Layout');
  $print_node->appendChild($layout_node);

  // *****
  // Add all the package <Item> nodes.
  // *****
  // Load the shipments containing those packages.
  module_load_include('inc', 'uc_stamps', 'uc_stamps.shared');
  $shipments = uc_stamps_load_shipments_by_status(array('queued'));

  if (empty($shipments)) {
    return NULL;
  }

  // Go through all the shipments, and create an <Item> node
  // for each package in each shipment.
  foreach ($shipments as $sid => $shipment) {
    $order = uc_order_load($shipment->order_id);

    // Sanity check.  It's possible to delete an order without deleting
    // the shipments or packages.
    if (empty($order)) {
      watchdog('uc_stamps', 'Attempt to export shipment %sid in non-existent order %oid.',
               array('%sid' => $sid, '%oid' => $shipment->order_id), WATCHDOG_ERROR);
      unset($shipments[$sid]);
      continue;
    }
    // The origin (Sender), destination (Recipient), and billing
    // information is
    // the same for each package in the shipment, so generate it just
    // once per shipment, save it in templates, and clone the templates
    // for each package.
    // Create a sender info template.
    $sender_node_template = $stamps_doc->createElement('Sender');

    // First and last name (possibly empty).
    $return_name_selection
      = variable_get('uc_stamps_return_address_selection', 'both');
    $first_name_element = $stamps_doc->createElement('FirstName');
    $last_name_element = $stamps_doc->createElement('LastName');
    if ($return_name_selection == 'personal' ||
        $return_name_selection == 'both') {
      $first_name_element->nodeValue = check_plain($shipment->o_first_name);
      $last_name_element->nodeValue = check_plain($shipment->o_last_name);
    }
    $sender_node_template->appendChild($first_name_element);
    $sender_node_template->appendChild($last_name_element);

    // Company name (possibly empty).
    $company_name_element = $stamps_doc->createElement('Company');
    if ($return_name_selection == 'company' ||
        $return_name_selection == 'both') {
      $company_name_element->nodeValue = check_plain($shipment->o_company);
    }
    $sender_node_template->appendChild($company_name_element);

    // Origin address.
    $sender_node_template->appendChild($stamps_doc->createElement(
      'Address1', check_plain($shipment->o_street1)));
    $sender_node_template->appendChild($stamps_doc->createElement(
      'Address2', check_plain($shipment->o_street2)));
    $sender_node_template->appendChild($stamps_doc->createElement(
      'City', check_plain($shipment->o_city)));
    // Add a state & ZIP if domestic, a province & postal code if not.
    if (uc_stamps_is_domestic($shipment->o_country)) {
      $zone_type = "State";
      $postal_code_type = "ZIP";
    }
    else {
      $zone_type = "Province";
      $postal_code_type = "PostalCode";
    }
    $zone_code = uc_get_zone_code($shipment->o_zone);
    $sender_node_template->appendChild($stamps_doc->createElement(
      $zone_type, check_plain($zone_code)));
    $sender_node_template->appendChild($stamps_doc->createElement(
      $postal_code_type, check_plain($shipment->o_postal_code)));
    $sender_node_template->appendChild($stamps_doc->createElement(
      'Country', uc_country_get_by_id($shipment->o_country)));

    // Add the sender e-mail address.  Yes, it really does have
    // this funky XML structure.
    // If the options are activated, this address gets notified when
    // the package is delivered and/or when the label is printed.
    // Takes the stamps.com configured e-mail address if available,
    // or the Ubercart store address if not.
    $sender_email_address = variable_get('uc_stamps_email', NULL);
    if (empty($sender_email_address)) {
      $sender_email_address = uc_store_email();
    }
    $sender_email_address_node = $stamps_doc->createElement(
      'Address', $sender_email_address);
    $sender_email_addresses = $stamps_doc->createElement('OrderedEmailAddresses');
    $sender_email_addresses->appendChild($sender_email_address_node);
    $sender_node_template->appendChild($sender_email_addresses);

    // Add the sender phone number.
    $sender_phone = $stamps_doc->createElement('Number', variable_get('uc_store_phone', NULL));
    $sender_phones = $stamps_doc->createElement('OrderedPhoneNumbers');
    $sender_phones->appendChild($sender_phone);
    $sender_node_template->appendChild($sender_phones);

    // Create a billing info template.
    // This information is mostly from the $order object.
    $billing_node_template = $stamps_doc->createElement('BillingAddress');
    $billing_node_template->appendChild($stamps_doc->createElement(
      'FirstName', check_plain($order->billing_first_name)));
    $billing_node_template->appendChild($stamps_doc->createElement(
      'LastName', check_plain($order->billing_last_name)));
    $billing_node_template->appendChild($stamps_doc->createElement(
      'Company', check_plain($order->billing_company)));
    $billing_node_template->appendChild($stamps_doc->createElement(
      'Address1', check_plain($order->billing_street1)));
    $billing_node_template->appendChild($stamps_doc->createElement(
      'Address2', check_plain($order->billing_street2)));
    $billing_node_template->appendChild($stamps_doc->createElement(
      'City', check_plain($order->billing_city)));
    // Add a state & ZIP if domestic, a province & postal code if not.
    if (uc_stamps_is_domestic($order->billing_country)) {
      $zone_type = "State";
      $postal_code_type = "ZIP";
    }
    else {
      $zone_type = "Province";
      $postal_code_type = "PostalCode";
    }
    $zone_code = uc_get_zone_code($order->billing_zone);
    $billing_node_template->appendChild($stamps_doc->createElement(
      $zone_type, check_plain($zone_code)));
    $billing_node_template->appendChild($stamps_doc->createElement(
      $postal_code_type, check_plain($order->billing_postal_code)));
    $billing_node_template->appendChild($stamps_doc->createElement(
      'Country', uc_country_get_by_id($order->billing_country)));

    // Add the billing e-mail address.
    $billing_email_address = $stamps_doc->createElement(
      'Address', check_plain($order->primary_email));
    $billing_email_addresses = $stamps_doc->createElement('OrderedEmailAddresses');
    $billing_email_addresses->appendChild($billing_email_address);
    $billing_node_template->appendChild($billing_email_addresses);

    // Add the billing phone number.
    $billing_phone = $stamps_doc->createElement('Number', $order->billing_phone);
    $billing_phones = $stamps_doc->createElement('OrderedPhoneNumbers');
    $billing_phones->appendChild($billing_phone);
    $billing_node_template->appendChild($billing_phones);

    // Create the recipient template.
    // Note that the Recipient address fields are encapsulated in
    // an AddressFields node, but the Sender address fields are not.
    $recipient_node_template = $stamps_doc->createElement('Recipient');
    $recipient_address_fields = $stamps_doc->createElement('AddressFields');
    $recipient_node_template->appendChild($recipient_address_fields);
    $recipient_address_fields->appendChild($stamps_doc->createElement(
      'FirstName', check_plain($shipment->d_first_name)));
    $recipient_address_fields->appendChild($stamps_doc->createElement(
      'LastName', check_plain($shipment->d_last_name)));
    $recipient_address_fields->appendChild($stamps_doc->createElement(
      'Company', check_plain($shipment->d_company)));
    $recipient_address_fields->appendChild($stamps_doc->createElement(
      'Address1', check_plain($shipment->d_street1)));
    $recipient_address_fields->appendChild($stamps_doc->createElement(
      'Address2', check_plain($shipment->d_street2)));
    $recipient_address_fields->appendChild($stamps_doc->createElement(
      'City', check_plain($shipment->d_city)));
    if (uc_stamps_is_domestic($shipment->d_country)) {
      $zone_type = "State";
      $postal_code_type = "ZIP";
    }
    else {
      $zone_type = "Province";
      $postal_code_type = "PostalCode";
    }
    $zone_code = uc_get_zone_code($shipment->d_zone);
    $recipient_address_fields->appendChild($stamps_doc->createElement(
      $zone_type, $zone_code));
    $recipient_address_fields->appendChild($stamps_doc->createElement(
      $postal_code_type, check_plain($shipment->d_postal_code)));
    $recipient_address_fields->appendChild($stamps_doc->createElement(
      'Country', uc_country_get_by_id($shipment->d_country)));

    // Get the phone number entered by the customer when creating the order.
    $recipient_phone = $stamps_doc->createElement(
      'Number', check_plain($order->delivery_phone));
    $recipient_phones = $stamps_doc->createElement('OrderedPhoneNumbers');
    $recipient_phones->appendChild($recipient_phone);
    $recipient_address_fields->appendChild($recipient_phones);

    // Get the e-mail address for the customer.
    // Note that the "shipment" form lets you enter a new address,
    // but does NOT save it anywhere, so just get it from the
    // original order record.
    $recipient_email_address = $stamps_doc->createElement(
      'Address', check_plain($order->primary_email));
    $recipient_email_addresses = $stamps_doc->createElement('OrderedEmailAddresses');
    $recipient_email_addresses->appendChild($recipient_email_address);
    $recipient_address_fields->appendChild($recipient_email_addresses);

    // Get the shipping class and options, which are the same for every
    // package in a single shipment.
    $ship_services = uc_stamps_services_array($shipment->accessorials);
    $services_node_template = $stamps_doc->createElement('Services');

    if (!empty($ship_services['CertifiedMail'])) {
      $services_node_template->appendChild($stamps_doc->createElement('CertifiedMail', 'true'));
    }
    // Charge the full order amount for a COD (to include the shipping charge).
    // This will mess up badly if there are multiple packages in the shipment.
    if (!empty($ship_services['COD'])) {
      $services_node_template->appendChild($stamps_doc->createElement('COD', $order->order_total));
    }
    // Only one of DeliveryConfirmation and SignatureConfirmation
    // is permitted at a time.
    if (!empty($ship_services['DeliveryConfirmation']) ||
        !empty($ship_services['SignatureConfirmation'])) {
      if (!empty($ship_services['SignatureConfirmation'])) {
        $services_node_template->appendChild($stamps_doc->createElement(
          'SignatureConfirmation', 'true'));
      }
      else {
        $services_node_template->appendChild($stamps_doc->createElement(
          'DeliveryConfirmation', 'true'));
      }
    }
    // No Saturday delivery of Express mail.
    if (!empty($ship_services['NoSaturdayDelivery'])) {
      $services_node_template->appendChild($stamps_doc->createElement(
        'NoSaturdayDelivery', 'true'));
    }
    // Return notice of non-delivery for COD shipments.
    if (!empty($ship_services['NoticeOfNonDelivery'])) {
      $services_node_template->appendChild($stamps_doc->createElement(
        'NoticeOfNonDelivery', 'true'));
    }
    // Delivery only to recipient, for Certified, Insured,
    // COD, or Registered Mail.
    if (!empty($ship_services['RestrictedDelivery'])) {
      $services_node_template->appendChild($stamps_doc->createElement(
        'RestrictedDelivery', 'true'));
    }
    // Return receipt (postcard) for non-merchandise.
    if (!empty($ship_services['ReturnReceipt'])) {
      $services_node_template->appendChild($stamps_doc->createElement(
        'ReturnReceipt', 'true'));
    }
    // Return receipt (postcard) for merchandise.
    if (!empty($ship_services['ReturnReceiptForMerchandise'])) {
      $services_node_template->appendChild($stamps_doc->createElement(
        'ReturnReceiptForMerchandise', 'true'));
    }
    // Sunday and Holiday delivery for Express Mail.
    if (!empty($ship_services['SundayHolidayDelivery'])) {
      $services_node_template->appendChild($stamps_doc->createElement(
        'SundayHolidayDelivery', 'true'));
    }
    // No signature required for domestic Express Mail.
    if (!empty($ship_services['WaiveDeliverySignature'])) {
      $services_node_template->appendChild($stamps_doc->createElement(
        'WaiveDeliverySignature', 'true'));
    }

    // Get some order information for this shipment.  It will
    // be the same for all packages in the shipment.
    $order_date_str = format_date($order->created, 'custom', 'Y-m-d');

    // Create an <Item> node for each package.
    foreach ($shipment->packages as $pid => $package) {
      // Only do the queued packages.
      if ($package->tracking_number != 'queued') {
        continue;
      }
      // Add the Sender and Recipient info nodes.
      $item_node = $stamps_doc->createElement('Item');
      $sender_node = $sender_node_template->cloneNode(TRUE);
      $item_node->appendChild($sender_node);
      $recipient_node = $recipient_node_template->cloneNode(TRUE);
      $item_node->appendChild($recipient_node);
      $billing_node = $billing_node_template->cloneNode(TRUE);
      $item_node->appendChild($billing_node);
      // Add the order ID and package ID info.
      $item_node->appendChild($stamps_doc->createElement(
        'OrderID', $package->order_id . '-' . $shipment->sid . '-' . $package->package_id));
      // Add the order date.
      $item_node->appendChild($stamps_doc->createElement(
        'OrderDate', $order_date_str));

      // Add the weight.  Tare weight should be included
      // in the package weight, but it's not, so add it
      // in here.
      $container = uc_stamps_container_load($package->pkg_type);
      $tare_weight = uc_weight_conversion($container->tare_units, 'oz') *
                     $container->tare;
      $weight_factor = uc_weight_conversion($package->weight_units, 'oz');
      $package_oz_weight = $weight_factor * $package->weight + $tare_weight;
      $item_node->appendChild($stamps_doc->createElement(
        'WeightOz', sprintf("%.0f", $package_oz_weight)));

      // Add the package dimensions.  These are in inches and must
      // be integers.
      $dimensions_node = $stamps_doc->createElement('PackageDimensions');
      $length_conversion = uc_length_conversion($package->length_units, 'in');
      $dimensions_node->appendChild($stamps_doc->createElement(
        'Length', (int) ($package->length * $length_conversion)));
      $dimensions_node->appendChild($stamps_doc->createElement(
        'Width', (int) ($package->width * $length_conversion)));
      $dimensions_node->appendChild($stamps_doc->createElement(
        'Height', (int) ($package->height * $length_conversion)));
      $boxlike_string = $container->boxlike ? 'true' : 'false';
      $dimensions_node->appendChild($stamps_doc->createElement(
        'BoxLike', $boxlike_string));
      $item_node->appendChild($dimensions_node);

      // Add mailpiece (package) type.
      $item_node->appendChild($stamps_doc->createElement(
        'Mailpiece', $container->container_type));

      // Add the service type.
      $item_node->appendChild($stamps_doc->createElement(
        'MailClass', $ship_services['class']));

      // Add the service options.
      $services_node = $services_node_template->cloneNode(TRUE);

      // Only one of SDCInsurance and USPSInsurance
      // is permitted at a time.
      // This value has to be calculated for each package separately,
      // as it is the value of the merchandise.  Max value is $2500 for
      // Stamps.com insurance and $5000 for USPS insurance.
      if (!empty($ship_services['SDCInsurance']) ||
          !empty($ship_services['USPSInsurance'])) {
        if (!empty($ship_services['SDCInsurance'])) {
          $insured_value = min($package->value, 2500);
          $services_node->appendChild($stamps_doc->createElement('SDCInsurance', $insured_value));
        }
        else {
          $insured_value = min($package->value, 5000);
          $services_node->appendChild($stamps_doc->createElement('USPSInsurance', $insured_value));
        }
      }

      // Registered mail offers insurance to $25,000.
      if (!empty($ship_services['RegisteredMail'])) {
        $insured_value = min($package->value, 25000);
        $services_node->appendChild($stamps_doc->createElement('RegisteredMail', $insured_value));
      }

      $item_node->appendChild($services_node);

      // Describe the contents of the package.  Stamps.com can
      // use this information to generate a packing slip.
      $order_contents_node = $stamps_doc->createElement('OrderContents');
      foreach ($package->products as $product) {
        $product_item_node = $stamps_doc->createElement('Item');
        $product_item_node->appendChild($stamps_doc->createElement(
            'ExternalID', $product->model));
        $product_item_node->appendChild($stamps_doc->createElement(
            'Name', $product->title));
        $product_item_node->appendChild($stamps_doc->createElement(
            'Price', $product->price));
        $product_item_node->appendChild($stamps_doc->createElement(
            'Quantity', $product->qty));
        $product_item_node->appendChild($stamps_doc->createElement(
            'Total', $product->qty * $product->price));
        $order_contents_node->appendChild($product_item_node);
      }
      $item_node->appendChild($order_contents_node);

      // Specify non-delivery options for foreign shipments.
      if (!uc_stamps_is_domestic($shipment->d_country)) {
        // Add the non-delivery option for international mail.
        if (!empty($ship_services['NonDeliveryOption'])) {
          $item_node->appendChild($stamps_doc->createElement(
            'NonDeliveryOption', $ship_services['NonDeliveryOption']));
        }
      }

      // Specify customs information for foreign, APO/FPO, and most
      // US territories.
      if (uc_stamps_shipment_needs_customs($shipment)) {
        // Create the customs info node.
        $customs_node = $stamps_doc->createElement('CustomsInfo');

        // Add the package-wide contents type field.
        // @TODO Handle other contents types.
        $customs_node->appendChild($stamps_doc->createElement(
          'ContentsType', 'merchandise'));

        // Declare the value of the package.  The declared value of the
        // individual items must add up to this.
        $customs_node->appendChild($stamps_doc->createElement(
          'DeclaredValue', $package->value));

        // Acknowledge acceptance of the USPS Privacy Act Statement and
        // also the Content Restrictions and Prohibitions of the destination
        // country.  This node must be here, and the acknowledgement
        // should be in the UI.
        $customs_node->appendChild($stamps_doc->createElement(
          'UserAcknowledged', 'true'));

        // Declare the contents of the package.
        $contents_node = $stamps_doc->createElement('Contents');
        // Go through each item in the package and add the fields.
        foreach ($package->products as $product) {
          $contents_item = $stamps_doc->createElement('Item');
          // Load the description if it has been configured.
          if (!empty($product->data['uc_stamps']) && !empty($product->data['uc_stamps']['description'])) {
            $contents_item->appendChild($stamps_doc->createElement(
                'Description', check_plain($product->data['uc_stamps']['description'])));
          }
          // Load the harmonized tariff code if it has been configured.
          if (!empty($product->data['uc_stamps']) && !empty($product->data['uc_stamps']['tariff_code'])) {
            $contents_item->appendChild($stamps_doc->createElement(
                'HarmonizedTariffCode', check_plain($product->data['uc_stamps']['tariff_code'])));
          }
          // Load the country of origin if it has been configured.
          if (!empty($product->data['uc_stamps']) && !empty($product->data['uc_stamps']['origin_country'])) {
            $contents_item->appendChild($stamps_doc->createElement(
                'CountryOfOrigin', uc_country_get_by_id($product->data['uc_stamps']['origin_country'])));
          }

          $contents_item->appendChild($stamps_doc->createElement(
                'Quantity', $product->qty));
          $contents_item->appendChild($stamps_doc->createElement(
                'Value', $product->price * $product->qty));

          $weight_factor = uc_weight_conversion($product->weight_units, 'oz');
          $product_oz_weight = $weight_factor * $product->weight;
          $contents_item->appendChild($stamps_doc->createElement(
            'WeightOz', sprintf("%.0f", $product_oz_weight)));
          $contents_node->appendChild($contents_item);
        }

        $customs_node->appendChild($contents_node);
        $item_node->appendChild($customs_node);
      }

      // Add the <Item> node for this package to the document.
      $print_node->appendChild($item_node);

      // Record that it was exported.
      $shipments[$sid]->packages[$pid]->tracking_number = 'exported';
    }
  }

  // In case all the shipments were bad and were removed.
  if (empty($shipments)) {
    return NULL;
  }

// Record that it was exported.
  $test_mode = variable_get('uc_stamps_test_mode', FALSE);
  if (!$test_mode) {
    foreach ($shipments as $shipment) {
      uc_shipping_shipment_save($shipment);
    }
  }

  // Return the base DOM document.
  return $stamps_doc;
}

/**
 * Import postback shipments as Stamps.com XML.
 *
 * This gets the tracking numbers and shipping costs and posts them back
 * into the database.
 *
 * @param object $file
 *   Managed file object containing the XML.
 */
function uc_stamps_xml_postback($file) {
  $test_mode = variable_get('uc_stamps_test_mode', FALSE);
  $doc = new DOMDocument();

  $doc->load($file->uri);

  // Get the base Stamps element.
  $root = $doc->documentElement;

  // Stash shipment/package information so it doesn't need to be reloaded
  // for each package in a shipment.
  $shipments = array();

  // Load the package nodes into an array.  Can't just use
  // getElementsByTagName because <Item> gets used at different levels for
  // different things.
  $package_nodes = array();
  foreach ($root->childNodes as $node) {
    if ($node->nodeName == 'Item') {
      $package_nodes[] = $node;
    }
  }

  foreach ($package_nodes as $package_node) {
    $raw_id = $package_node->getElementsByTagName('OrderID')
                                ->item(0)
                                ->nodeValue;
    if (empty($raw_id)) {
      drupal_set_message(t('Invalid postback XML.  Missing order ID.', 'error'));
      continue;
    }

    // Get the status of the label (what happened when printed).
    $print_result_nodes = $package_node->getElementsByTagName('PrintResult');
    // Make sure it's actually there.
    if ($print_result_nodes->length == 0) {
      drupal_set_message(t('Invalid postback XML.  PrintResult missing.  Stamps.com batch XML export does not contain postback information.  Use CSV.',
        array('@pid' => $package_id)), 'error');
      continue;
    }

    // Get the value and make sure the postage was actually printed.
    $print_result = $print_result_nodes->item(0)->nodeValue;

    // Get the tracking number.
    $tracking_number_nodes = $package_node->getElementsByTagName('TrackingNumber');
    if ($tracking_number_nodes->length == 0) {
      drupal_set_message(t('Missing tracking number.  Stamps.com batch does not export tracking numbers in XML.'), 'error');
      return;
    }
    $tracking_number = $tracking_number_nodes->item(0)->nodeValue;

    // Extract oid, sid, and pid from 123-456-789 format.
    if (!preg_match('/\d+-\d+-\d+/', $raw_id)) {
      drupal_set_message(t('Invalid order ID %oid.  Postback must be from Ubercart export.',
        array('%oid' => $raw_id)), 'error');
      continue;
    }
    list($order_id, $shipment_id, $package_id) = preg_split('/-/', $raw_id);

    // Get the shipment (with this package) if it is not already loaded.
    if (!array_key_exists($shipment_id, $shipments)) {
      $shipments[$shipment_id] = uc_shipping_shipment_load($shipment_id);
      if (empty($shipments[$shipment_id])) {
        drupal_set_message(t('Invalid postback XML.  No shipment @sid.',
          array('@sid' => $shipment_id)), 'error');
        continue;
      }
    }
    if (empty($shipments[$shipment_id]->packages[$package_id])) {
      drupal_set_message(t('Invalid postback XML.  No package @pid.',
        array('@pid' => $package_id)), 'error');
      continue;
    }

    // If we don't have "printed", then there will be no cost or tracking
    // number, so record it and stop here.
    if ($print_result != 'printed' && $print_result != 'printed_sample') {
      drupal_set_message(t('Problem with package @pid:  @print_result.',
        array('@pid' => $package_id, '@print_result' => $print_result)), 'warning');
      $shipments[$shipment_id]->tracking_number = $print_result;
      $shipments[$shipment_id]->packages[$package_id]->tracking_number = $print_result;
      continue;
    }

    // Store the tracking number in both the shipment and package records.
    $shipments[$shipment_id]->tracking_number = $tracking_number;
    $shipments[$shipment_id]->packages[$package_id]->tracking_number = $tracking_number;


    // Add the cost of this package to the cost of the shipment.
    $usps_cost_elements = $package_node->getElementsByTagName('PostageCost');
    if ($usps_cost_elements->length) {
      $usps_cost_element = $usps_cost_elements->item(0);
      $usps_total_elements = $usps_cost_element->getElementsByTagName('Total');
      if ($usps_total_elements->length) {
        $shipments[$shipment_id]->cost += $usps_total_elements->item(0)->nodeValue;
      }
    }
    // Add the cost of Stamp.com insurance.
    $sdc_cost_elements = $package_node->getElementsByTagName('SDCCost');
    if ($sdc_cost_elements->length) {
      $sdc_cost_element = $sdc_cost_elements->item(0);
      $sdc_insurance_elements = $sdc_cost_element->getElementsByTagName('SDCInsurance');
      if ($sdc_insurance_elements->length) {
        $shipments[$shipment_id]->cost += $sdc_insurance_elements->item(0)->nodeValue;
      }
    }
  }

  // Save the updated shipments and packages.
  if (!$test_mode) {
    foreach ($shipments as $shipment) {
      uc_shipping_shipment_save($shipment);
    }
  }
  drupal_set_message(t('Posted back @count packages from %file',
    array('@count' => sizeof($package_nodes), '%file' => basename($file->uri))));
}

/**
 * Import postback shipments as Stamps.com CSV.
 *
 * This gets the tracking numbers and shipping costs and posts them back
 * into the database.
 *
 * @param object $file
 *   Managed file object containing the CSV.
 */
function uc_stamps_csv_postback($file) {
  $test_mode = variable_get('uc_stamps_test_mode', FALSE);
  $handle = fopen($file->uri, 'r');

  // Check the header line for the right number of fields.
  $data = fgetcsv($handle);
  if (sizeof($data) != 23) {
    drupal_set_message(t('Invalid CSV file.  You must export from Stamps.com batch.'));
    return;
  }

  $shipments = array();
  while ($data = fgetcsv($handle)) {
    // Skip the possible last blank line.
    if (sizeof($data) == 0) {
      continue;
    }
    $order_id_raw = $data[0];
    $cost_raw = $data[1];
    $tracking_number = (string) $data[19];

    // Remove the $ from $1.23 format.
    $cost = drupal_substr($cost_raw, 1);

    // Extract oid, sid, and pid from 123-456-789 format.
    if (!preg_match('/\d+-\d+-\d+/', $order_id_raw)) {
      drupal_set_message(t('Invalid order ID %oid.  Postback must be from Ubercart export.',
        array('%oid' => $order_id_raw)), 'error');
      continue;
    }
    list($order_id, $shipment_id, $package_id) = preg_split('/-/', $order_id_raw);

    // Get the shipment (with this package) if it is not already loaded.
    if (!array_key_exists($shipment_id, $shipments)) {
      $shipments[$shipment_id] = uc_shipping_shipment_load($shipment_id);
      if (empty($shipments[$shipment_id])) {
        drupal_set_message(t('Invalid postback CSV.  No shipment @sid.',
          array('@sid' => $shipment_id)), 'error');
        continue;
      }
    }
    if (empty($shipments[$shipment_id]->packages[$package_id])) {
      drupal_set_message(t('Invalid postback CSV.  No package @pid.',
        array('@pid' => $package_id)), 'error');
      continue;
    }

    // Store the tracking number.
    $shipments[$shipment_id]->tracking_number = $tracking_number;
    $shipments[$shipment_id]->packages[$package_id]->tracking_number = $tracking_number;

    // Add in the cost of the package to the shipment.
    $shipments[$shipment_id]->cost += $cost;
  }

  // Clean up.
  fclose($handle);

  // Save the updated shipments and packages.
  if (!$test_mode) {
    foreach ($shipments as $shipment) {
      uc_shipping_shipment_save($shipment);
    }
  }

  drupal_set_message(t('Posted back %file', array('%file' => basename($file->uri))));
}
