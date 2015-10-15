<?php
/**
 * @file
 * Form renderer and submission callback functions for the uc_endicia module's
 * admin & configuration pages.
 */


/**
 * Form builder for the order fulfillment page using presets (skips confirmation
 * and immediately gets a price quote to submit a shipment).
 *
 * @see uc_endicia_fulfill_order()
 */
function uc_endicia_fulfill_order_preset($form, $form_state, $order, $package_ids) {
  // FIXME: We probably need to be moving the order-is-international to a dedicated function
  $is_international = ($order->delivery_country != 840);
  $preset_id = substr(arg(5), 15);
  $preset = endicia_label_preset_load($preset_id);
  foreach ($package_ids as $package_id) {
    $forced_state['values']['packages'][$package_id]['mail_shape'] = $preset['mail_shape'];
    $forced_state['values']['packages'][$package_id]['dimensions']['length'] = $preset['length'];
    $forced_state['values']['packages'][$package_id]['dimensions']['width'] = $preset['height'];
    $forced_state['values']['packages'][$package_id]['dimensions']['height'] = $preset['height'];
    if ($is_international) {
      $forced_state['values']['packages'][$package_id]['options'] = array('usps_form_2976' => 'usps_form_2976');
    }
  }
  // FIXME: We should validate with form_get_errors()
  drupal_form_submit('uc_endicia_fulfill_order', $forced_state, $order, $package_ids);
  $_SESSION['uc_endicia']['mail_class'] = $is_international ? $preset['mail_class_international'] : $preset['mail_class_domestic'];
  // Destination passing is handled by form automatically
  if (is_array($forced_state['redirect'])) {
    call_user_func_array('drupal_goto', $forced_state['redirect']);
  }
  else {
    drupal_goto($forced_state['redirect']);
  }
}

/**
 * Form builder for the order fulfillment page, confirming user information
 * before getting a price quote and submitting a shipment.
 *
 * @see uc_endicia_fulfill_order_submit()
 * @see uc_endicia_confirm_shipment()
 */
function uc_endicia_fulfill_order($form, &$form_state, $order, $package_ids) {
  $form = array();
  $form['#attached']['css'][] = drupal_get_path('module', 'endicia_ui') . '/endicia_ui.css';
  $form['#attributes'] = array('class' => array('endicia-ship-form'));
  $form['instructions'] = array(
    '#value' => t('The information on this page has been pre-populated based on the information contained in the order. If any if it is incorrect or needs to be changed, update it now. Changes made here will not be re-applied to the order and will be used only for shipment label creation. When finished, click Continue to proceed to shipment confirmation which will include a price quote.'),
    '#weight' => -3,
  );

  $form['order_id'] = array('#type' => 'value', '#value' => $order->order_id);
  $packages = array();
  $addresses = array();

  $address = variable_get('uc_quote_store_default_address', new UcAddress());
  $country_info = uc_get_country_data(array('country_id' => $address->country));
  $address->country = $country_info[0]['country_iso_code_2'];
  $address->zone = uc_get_zone_code($address->zone);
  $form['origin'] = array(
    '#type' => 'fieldset',
    '#title' => t('Origin address'),
  );
  $form['origin']['pickup_address'] = array(
    '#type' => 'addressfield',
    '#required' => TRUE,
    '#title' => t('Recipient\'s Address'),
    '#handlers' => array('address' => 'address', 'name-full' => 'name-full', 'organisation' => 'organisation'),
    '#default_value' => array(
      'first_name' => isset($address->first_name) ? $address->first_name : '',
      'last_name' => isset($address->last_name) ? $address->last_name : '',
      'organisation_name' => isset($address->company) ? $address->company : '',
      'thoroughfare' => isset($address->street1) ? $address->street1 : '',
      'premise' => isset($address->street2) ? $address->street2 : '',
      'locality' => isset($address->city) ? $address->city : '',
      'administrative_area' => isset($address->zone) ? $address->zone : '',
      'postal_code' => isset($address->postal_code) ? $address->postal_code : '',
      'country' => isset($address->country) ? $address->country : 'US',
    ),
    '#description' => "Please enter the address you want to mail this gift to.",
    '#maxlength' => 60,
  );
  $form['origin']['pickup_phone'] = array(
    '#type' => 'textfield',
    '#title' => t('Phone number'),
    '#size' => 16,
    '#maxlength' => 32,
    '#required' => FALSE,
    '#default_value' => isset($address->phone) ? $address->phone : '',
  );
  $form['origin']['pickup_email'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail'),
    '#size' => 32,
    '#required' => FALSE,
    '#default_value' => isset($address->email) ? $address->email : '',
  );

  $address = _uc_endicia_get_address($order, 'delivery');
  $country_info = uc_get_country_data(array('country_id' => $address->country));
  $address->country = $country_info[0]['country_iso_code_2'];
  $is_international = $address->country != 'US';
  $address->zone = uc_get_zone_code($address->zone);
  $form['destination'] = array(
    '#type' => 'fieldset',
    '#title' => t('Destination address'),
  );
  $form['destination']['delivery_address'] = array(
    '#type' => 'addressfield',
    '#required' => TRUE,
    '#title' => t('Recipient\'s Address'),
    '#handlers' => array('address' => 'address', 'name-full' => 'name-full', 'organisation' => 'organisation'),
    '#default_value' => array(
      'first_name' => isset($address->first_name) ? $address->first_name : '',
      'last_name' => isset($address->last_name) ? $address->last_name : '',
      'organisation_name' => isset($address->company) ? $address->company : '',
      'thoroughfare' => isset($address->street1) ? $address->street1 : '',
      'premise' => isset($address->street2) ? $address->street2 : '',
      'locality' => isset($address->city) ? $address->city : '',
      'administrative_area' => isset($address->zone) ? $address->zone : '',
      'postal_code' => isset($address->postal_code) ? $address->postal_code : '',
      'country' => isset($address->country) ? $address->country : 'US',
    ),
    '#description' => "Please enter the address you want to mail this gift to.",
    '#maxlength' => 60,
  );

  $form['destination']['delivery_phone'] = array(
    '#type' => 'textfield',
    '#title' => t('Phone number'),
    '#size' => 16,
    '#maxlength' => 32,
    '#required' => FALSE,
    '#default_value' => isset($address->phone) ? $address->phone : '',
  );
  $account = user_load($order->uid);
  $form['destination']['delivery_email'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail'),
    '#size' => 32,
    '#required' => FALSE,
    '#default_value' => isset($account->mail) ? $account->mail : '',
  );

  // Container for package data
  $form['packages'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Packages'),
    '#collapsible' => TRUE,
    '#tree'        => TRUE,
  );
  foreach ($package_ids as $package_id) {
    $package = uc_shipping_package_load($package_id);
    if ($package === FALSE) {
      drupal_set_message(t('Could not find package #@packageid.', array('@packageid' => $package_id)), 'error');
      continue;
    }

    // Create list of products and get a representative product (last one in
    // the loop) to use for some default values
    $product_list = array();
    foreach ($package->products as $product) {
      $product_list[] = $product->qty .' x '. $product->model;
    }
    // Use last product in package to determine package type
    $pkg_form = array(
      '#type'  => 'fieldset',
      '#title' => t('Package @id', array('@id' => $package_id)),
    );
    $pkg_form['products'] = array(
      '#value' => theme('item_list', $product_list)
    );
    $pkg_form['package_id'] = array(
      '#type'  => 'hidden',
      '#value' => $package_id,
    );
    $pkg_form['declared_value'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Declared value'),
      '#default_value' => uc_endicia_get_package_value($package),
      '#required'      => TRUE,
      '#field_prefix'  => '$',
    );
    $pkg_form['weight'] = array(
      '#type'          => 'container',
      '#attributes'    => array('class' => array('uc-inline-form', 'clearfix')),
      '#description'   => t('Weight of the package. Default value is sum of product weights in the package.'),
      '#weight'        => 15,
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
      '#type'        => 'container',
      '#attributes' => array('class' => array('uc-inline-form', 'clearfix')),
      '#description' => t('Physical dimensions of the package.'),
      '#weight'      => 20,
    );
    $pkg_form['dimensions']['length'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Length'),
      '#default_value' => isset($product->length) ? $product->length : 1,
      '#size'          => 8,
    );
    $pkg_form['dimensions']['width'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Width'),
      '#default_value' => isset($product->width) ? $product->width : 1,
      '#size'          => 8,
    );
    $pkg_form['dimensions']['height'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Height'),
      '#default_value' => isset($product->height) ? $product->height : 1,
      '#size'          => 8,
    );
    $pkg_form['dimensions']['units'] = array(
      '#type'          => 'select',
      '#title'         => t('Units of measurement'),
      '#options'       => array(
        'in' => t('Inches'),
        'ft' => t('Feet'),
        'cm' => t('Centimeters'),
        'mm' => t('Millimeters'),
      ),
      '#default_value' => isset($product->length_units) ?
                          $product->length_units :
                          variable_get('uc_length_unit', 'in'),
    );
    $pkg_form['mail_shape'] = array(
      '#type' => 'select',
      '#title' => t('Mailpiece Shape'),
      '#description' => t('Select the mailpiece shape to use for this shipment.'),
      '#options' => _endicia_pkg_types('enabled'),
    );
    $options = array(0 => 'N/A');
    $now = time();
    foreach (range(1,7) as $day) {
      $date = format_date($now + 86400 * $day, 'custom', 'l, F j');
      $variables = array('@num' => $day, '@formatted_date' => $date);
      $text = format_plural($day, '@num day - @formatted_date', '@num days - @formatted_date', $variables);
      $options[$day] = $text;
    }
    $pkg_form['date_advance'] = array(
      '#type' => 'select',
      '#title' => t('Date Advance'),
      '#description' => t('Enter the number of days advance the date shown on the printed label by.'),
      '#options' => $options,
      '#default_value' => 0,
    );
    $pkg_form['insurance'] = array(
      '#type' => 'select',
      '#title' => t('Insurance'),
      '#description' => t('USPS online insurance requires one of Delivery Confirmation or Signature Confirmation. Endicia Parcel Insurance offers a maximum insurable value of $10,000. Note that the cost of Endicia Parcel Insurance is NOT included in the postage quotes and is instead billed directly to your account.'),
      '#options' => array(
        'None' => t('None requested'),
        'USPS' => t('USPS Online Insurance'),
        'Endicia' => t('Endicia Parcel Insurance'),
      ),
    );
    $pkg_form['options'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Options'),
      '#options' => array(
        'delivery_confirmation' => t('Delivery confirmation'),
        'signature_confirmation' => t('Signature confirmation'),
        'hold_for_pickup' => t('Hold for pickup (up to 7 days)'),
        'usps_form_2976' => t('Customs Information (USPS form 2976/2976A)'),
      ),
    );
    if ($is_international) {
      $pkg_form['options']['#default_value'] = array('usps_form_2976');
    }

    $form['packages'][$package_id] = $pkg_form;
  }
  // Ensure we are shipping >= 1 package
  if (count(element_children($form['packages'])) == 0) {
    drupal_goto('admin/store/orders/' . $order->order_id . '/shipments/new');
  }

  foreach (array('delivery_email', 'delivery_last_name', 'delivery_street1', 'delivery_city', 'delivery_zone', 'delivery_country', 'delivery_postal_code') as $field) {
    $form['destination'][$field]['#required'] = TRUE;
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Continue'),
  );
  return $form;
}

/**
 * Form submit handler for the order fulfillment form. Passes data on to the
 * shipment confirmation form.
 *
 * @see uc_endicia_fulfill_order()
 * @see uc_endicia_confirm_shipment()
 */
function uc_endicia_fulfill_order_submit($form, &$form_state) {
  $_SESSION['uc_endicia']['packages'] = $form_state['values']['packages'];

  foreach (array('pickup', 'delivery') as $type) {
    foreach ($form_state['values'][$type . '_address'] as $key => $value) {
      $form_state['values'][$type . '_' . $key] = $value;
    }
    unset($form_state['values'][$type . '_address']);
  }
  $_SESSION['uc_endicia']['pickup_address'] = _endicia_get_address($form_state['values'], 'pickup');
  $_SESSION['uc_endicia']['delivery_address'] = _endicia_get_address($form_state['values'], 'delivery');
  $_SESSION['uc_endicia']['method'] = arg(5);

  // See uc_endicia_fulfill_order_preset()
  unset($_SESSION['uc_endicia']['mail_class']);
  // Pass on the destination to our next form
  $url = 'admin/store/orders/' . $form_state['values']['order_id'] . '/shipments/endicia';
  if (isset($_REQUEST['destination'])) {
    $orig_dest = $_REQUEST['destination'];
    unset($_REQUEST['destination']);
    $query = array('destination' => $orig_dest);
    $form_state['redirect'] = array($url, $query);
  }
  else {
    $form_state['redirect'] = $url;
  }
}

/**
 * Callback for Ubercart's shipping module. Generates the order fulfillment to
 * process a shipment with Endicia.
 *
 * @see uc_endicia_confirm_shipment_submit()
 * @see theme_uc_endicia_confirm_shipment()
 * @see uc_endicia_fulfill_order()
 */
function uc_endicia_confirm_shipment($form, &$form_state, $order) {
  // Check to make sure request is valid
  if (!isset($_SESSION['uc_endicia']['packages']) || !isset($_SESSION['uc_endicia']['method'])) {
    drupal_goto('admin/store/orders/' . $order->order_id . '/shipments/new');
  }

  if (_endicia_in_test_mode()) {
    drupal_set_message(t('Reminder: Endicia is operating in test mode. Your account balance will not be deducted.'), 'warning');
  }

  // Setup some stuff for use later
  $destination = $_SESSION['uc_endicia']['delivery_address'];
  $origin = $_SESSION['uc_endicia']['pickup_address'];
  $is_international = ($destination->country != 'US');
  // Convert country ID stored in DB to ISO country code
  $destination_country = uc_get_country_data(array('country_id' => $destination->country));
  // Mail classes available for the destination country
  $mail_classes = _endicia_mail_classes($is_international ? 'International' : 'Domestic');
  $mail_class_rates = array();

  // We may encounter an error below processing one of several packages, so this
  // will keep track of which package ids succeeded for the submit handler.
  $valid_package_ids = array();
  foreach ($_SESSION['uc_endicia']['packages'] as $package_id => $package_info) {
    $package = uc_shipping_package_load($package_id);
    if ($package === FALSE) {
      drupal_set_message(t('Could not find package #@packageid.', array('@packageid' => $package_id)), 'error');
      continue;
    }

    // Determine the package's weight
    $weight_unit_ratio = ($package->weight_units != 'oz' ? uc_weight_conversion($package->weight_units, 'oz') : 1); // FIXME use form weight instead of package weight
    $weight = round($package->weight * $weight_unit_ratio, 1);
    if ($weight == 0) {
      drupal_set_message(t('Package #@packageid has a total weight of 0. You must set product weights in order to use Endicia.', array('@packageid' => $package_id)), 'error');
      continue;
    }

    // Set request data for API call to calculate postage rates
    $countries = _endicia_country_get_predefined_list();
    $data = array(
      'AccountID' => variable_get('endicia_account_id', ''),
      'Password' => variable_get('endicia_passphrase', ''),
      'MailClass' => ($is_international ? "International" : "Domestic"),
      'MailpieceShape' => $package_info['mail_shape'],
      'Weight' => $weight,
      'Sender' => array(
        'PostalCode' => $origin->postal_code,
      ),
      'Recipient' => array(
        'PostalCode' => $destination->postal_code,
        'CountryCode' => $destination->country,
        'Country' => $countries[$destination->country],
      ),
      'CODAmount' => 0, // FIXME: API manual says this is optional, but it isn't?
      'InsuredValue' => $package_info['insurance'] == 'None' ? 0 : uc_endicia_get_package_value($package), // Same here.
      'RegisteredMailValue' => 0, // You guessed it, here also.
      'DateAdvance' => $package_info['date_advance'],
      'Services' => array(),
    );
    foreach($package_info['options'] as $option => $value) {
      if ($value) {
        switch ($option) {
          case 'signature_confirmation':
            $data['Services']['SignatureConfirmation'] = 'ON';
            break;
          case 'delivery_confirmation':
            $data['Services']['DeliveryConfirmation'] = 'ON';
            break;
          case 'hold_for_pickup':
            $data['Services']['HoldForPickup'] = 'ON';
            break;
        }
      }
    }
    // Send only the ZIP5 for US postal codes
    if (!$is_international && preg_match('/\d+-\d+/', $destination->postal_code)) {
      list($zip5, $zip4) = explode('-', $destination->postal_code, 2);
      $data['Recipient']['PostalCode'] = $zip5;
    }
    // Make the API request
    $uri = _endicia_in_test_mode() ? ENDICIA_TEST_URI : ENDICIA_PRODUCTION_URI;
    $response_node = 'PostageRatesResponse';
    $response = endicia_api_request($uri, 'CalculatePostageRates', $data, $response_node);
    // Check for failures
    if ($response === FALSE) {
      _endicia_communication_error_redirect('admin/store/orders/' . $order->order_id . '/shipments/new');
    }
    // Check for errors
    if ($error = endicia_api_request_error_check($response, $response_node)) {
      $message = 'Could not retrieve postage rate calculations for package #@packageid. Please ensure that your order contains a valid destination address (check the site log for additional details).';
      $variables = array(
        '@packageid' => $package_id,
      );
      drupal_set_message(t($message, $variables), 'error');
      continue;
    }
    $valid_package_ids[] = $package_id;

    // Keep tallies of rates by mail class
    if (is_array($response->PostageRatesResponse->PostagePrice)) {
      foreach ($response->PostageRatesResponse->PostagePrice as $rate) {
        // Avoid a PHP undefined index warnings; initialize or add to the existing index if necessary
        $mail_class_rates[$rate->MailClass] = isset($mail_class_rates[$rate->MailClass]) ? $mail_class_rates[$rate->MailClass] + $rate->TotalAmount : $rate->TotalAmount;
      }
    }
    else {
      $rate = &$response->PostageRatesResponse->PostagePrice;
      $mail_class_rates[$rate->MailClass] = isset($mail_class_rates[$rate->MailClass]) ? $mail_class_rates[$rate->MailClass] + $rate->TotalAmount : $rate->TotalAmount;
    }
  }

  if (count($valid_package_ids) == 0) {
    drupal_goto('admin/store/orders/' . $order->order_id . '/shipments/new');
  }

  // Process pricing information returned from API request and add it to the form.
  $mail_class_estimates = array();
  foreach ($mail_class_rates as $class => $total) {
    $class_text = isset($mail_classes[$class]) ? $mail_classes[$class] : t('[Title missing] @name', array('@name' => $class));
    $mail_class_estimates[$class] = $class_text . ' - ' . uc_currency_format($total);
  }

  // Get their account balance
  $account_info = endicia_get_account_status();
  if ($account_info === FALSE || $account_info == array()) {
    $balance = t('Unknown');
  }
  else {
    $balance = uc_currency_format($account_info['PostageBalance']);
  }

  // Now that we have all the information we need, setup the form
  $form['order_id'] = array(
    '#type' => 'hidden',
    '#value' => $order->order_id,
  );
  $form['package_ids'] = array(
    '#type' => 'hidden',
    '#value' => serialize($valid_package_ids),
  );

  $form['instructions'] = array(
    '#value' => t('Select a shipping class and click on Confirm Shipment to complete the shipment. Your account balance will be deducted the amount indicated next to the shipping class.'),
  );

  $form['account_balance'] = array(
    '#title' => t('Account Balance'),
    '#type' => 'item',
    '#markup' => $balance,
  );

  $form['mail_class'] = array(
    '#type' => 'select',
    '#title' => t('Shipping Class'),
    '#description' => t('Select the mail class to use for this shipment.'),
    '#options' => $mail_class_estimates,
    '#default_value' => isset($_SESSION['uc_endicia']['mail_class']) ? $_SESSION['uc_endicia']['mail_class'] : _endicia_get_service_type($is_international),
  );

  $url = 'admin/store/orders/' . $order->order_id . '/shipments/new';
  # Enable this once we have back functionality working - right now it just
  # reloads form, unsetting and user customizations
  #$url = 'admin/store/orders/' . $order->order_id . '/ship/' . $_SESSION['uc_endicia']['method'] . '/' . implode('/', array_keys($_SESSION['uc_endicia']['packages']));

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Confirm shipment'),
    '#suffix' => l(t('Cancel'), $url),
  );

  return $form;
}

/**
 * Submit handler for the order fulfillment confirmation form.
 *
 * @see uc_endicia_confirm_shipment()
 * @see uc_endicia_fulfill_order()
 */
function uc_endicia_confirm_shipment_submit($form, &$form_state) {
  $order = uc_order_load(intval($form_state['values']['order_id']));
  $package_ids = unserialize($form_state['values']['package_ids']);

  $origin = $_SESSION['uc_endicia']['pickup_address'];
  $destination = $_SESSION['uc_endicia']['delivery_address'];
  $packages = $_SESSION['uc_endicia']['packages'];

  foreach ($package_ids as $package_id) {
    $package = uc_shipping_package_load($package_id);
    if ($package === FALSE) {
      drupal_set_message(t('Could not find package #@packageid.', array('@packageid' => $package_id)), 'error');
      continue;
    }
    $package->pkg_type = $packages[$package_id]['mail_shape'];
    $package->value = $packages[$package_id]['declared_value'];
    if ($response = uc_endicia_shipping_label_request($package, $origin, $destination, $form_state['values']['mail_class'])) {
      $package->tracking_number = $response->LabelRequestResponse->TrackingNumber;

      $image_format = variable_get('endicia_label_image_format', 'PNG');
      $extension = '.' . _endicia_get_file_extension($image_format);

      if (isset($response->LabelRequestResponse->Base64LabelImage)) {
        // Single image returned as base64
        $filename = 'label-'. $package_id . '-' . $package->tracking_number . $extension;
        $data = base64_decode($response->LabelRequestResponse->Base64LabelImage);
        if ($label_file = endicia_save_label($filename, $data, 'uc_endicia', $package->tracking_number)) {
          file_usage_add($label_file, 'uc_shipping', 'package', $package->package_id);
          $package->label_image = $label_file;
        }
      }
      else {
        if (is_array($response->LabelRequestResponse->Label->Image)) {
          // Two+ labels returned as base64 under their own nodes
          foreach($response->LabelRequestResponse->Label->Image as $labelpart) {
            $filename = 'label-'. $package_id . '-' . $package->tracking_number . '-' . $labelpart->PartNumber . $extension;
            $data = base64_decode($labelpart->_);
            if ($label_file = endicia_save_label($filename, $data, 'uc_endicia', $package->tracking_number)) {
              file_usage_add($label_file, 'uc_shipping', 'package', $package->package_id);
              $package->label_image = $label_file;
            }
            else {
              break;
            }
          }
        }
        else {
          // One label returned  as base64 under its own node
          $labelpart = &$response->LabelRequestResponse->Label->Image;
          $filename = 'label-'. $package_id . '-' . $package->tracking_number . '-' . $labelpart->PartNumber . $extension;
          $data = base64_decode($labelpart->_);
          if ($label_file = endicia_save_label($filename, $data, 'uc_endicia', $package->tracking_number)) {
            file_usage_add($label_file, 'uc_shipping', 'package', $package->package_id);
            $package->label_image = $label_file;
          }
        }
      }
      if (!$label_file) {
        // FIXME: Display tracking number so they can refund
        drupal_set_message(t("Could not save the label image due to insufficient permissions. Please verify that Drupal's private files directory exists and is writable."), 'error');
        continue;
      }

      $shipment = new stdClass();
      $shipment->order_id = $order->order_id;
      $shipment->origin = clone $origin;
      $shipment->destination = clone $destination;
      $shipment->packages = array($package);

      $shipment->shipping_method = 'endicia';
      $shipment->accessorials = $form_state['values']['mail_class'];
      $shipment->carrier = t('USPS');
      $shipment->cost = $response->LabelRequestResponse->FinalPostage;
      $shipment->tracking_number = $response->LabelRequestResponse->TrackingNumber;
      $ship_date['year'] = substr($response->LabelRequestResponse->TransactionDateTime, 0, 4);
      $ship_date['month'] = substr($response->LabelRequestResponse->TransactionDateTime, 4, 2);
      $ship_date['day'] = substr($response->LabelRequestResponse->TransactionDateTime, 6, 2);
      $ship_date['hour'] = substr($response->LabelRequestResponse->TransactionDateTime, 8, 2);
      $ship_date['min'] = substr($response->LabelRequestResponse->TransactionDateTime, 10, 2);
      $ship_date['sec'] = substr($response->LabelRequestResponse->TransactionDateTime, 12, 2);
      $timestamp = gmmktime($ship_date['hour'], $ship_date['min'], $ship_date['sec'], $ship_date['month'], $ship_date['day'], $ship_date['year']);
      $shipment->ship_date = $timestamp;
      //$shipment->expected_delivery = gmmktime(. . .);
      uc_shipping_shipment_save($shipment);
    }
    else {
      // watchdog already notified.
      _endicia_communication_error_redirect();
      return FALSE;
    }
  }

  unset($_SESSION['uc_endicia']);
  $form_state['redirect'] = 'admin/store/orders/'. $order->order_id .'/shipments';
}


function theme_uc_endicia_fulfill_order($variables) {
  $form = $variables['form'];
  $output = '<div>' . drupal_render_children($form['instructions']) . '</div>';
  $output .= '<div class="address address-origin">' . drupal_render($form['origin']) . '</div>';
  $output .= '<div class="address address-delivery">' . drupal_render($form['destination']) . '</div>';
  $output .= '<div class="packages">' . drupal_render($form['packages']) . '</div>';
  $output .= drupal_render_children($form);
  return $output;
}
