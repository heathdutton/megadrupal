<?php
/**
 * @file
 * Form builders, theme functions & callbacks related to the process of label
 * printing & shipping a parcel.
 */

/**
 * Displays the shipping label for printing.
 *
 * Each argument is a component of the file path to the image.
 */
function endicia_ui_label_image() {
  global $language;
  $args = func_get_args();
  $image_uri = file_stream_wrapper_uri_normalize('private://' . implode('/', $args));
  $image_path = file_create_url($image_uri);
  if (!file_exists($image_uri)) {
    return MENU_NOT_FOUND;
  }

  $file_info = pathinfo($image_path);
  switch($file_info['extension']) {
    case 'zpl':
    case 'epl':
      if (module_exists('jzebra')) {
        drupal_goto('jzebra/private://' . implode('/', $args));
      }
      else {
        drupal_set_message(t('The jZebra module is required in order to print ZPL labels. Please install it and then return the label archive to try printing this label again.'), 'error');
        drupal_goto('admin/endicia/label/archive');
      }
      break;

    case 'gif':
    case 'png':
    case 'jpg':
      $image_path = 'private://' . implode('/', $args);
      $output = <<<EOHTML
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="{$language->language}" lang="{$language->language}" dir="{$language->direction}">
<head>
  <title>Endicia Shipping Label</title>
  <style>
    .page-break {
      page-break-before: always;
    }
    img { width: 6in; height: 4in; padding: 0.15in 0.51in; }
  </style>
</head>
<body>
EOHTML;
      $output .= theme('image', array('path' => $image_uri));
      $output .= '</body></html>';
      print $output;
      exit();
      break;

    case 'pdf':
    default:
      $image_url = file_create_url('private://' . implode('/', $args));
      drupal_goto($image_url);
      break;
  }
}

/**
 * Generic "cloud" print label form.
 *
 * @see theme_endicia_print_label_form().
 */
function endicia_ui_print_label_form() {
  $form['#attached']['css'][] = drupal_get_path('module', 'endicia_ui') . '/theme/endicia_ui.css';
  $form['#attributes'] = array('class' => array('endicia-ship-form'));
  $form['instructions'] = array(
    '#value' => 'The information on this page has been pre-populated based on the information contained in the order. If any if it is incorrect or needs to be changed, update it now. Changes made here will not be re-applied to the order and will be used only for shipment label creation. When finished, click Continue to proceed to shipment confirmation which will include a price quote.',
  );

  $address = isset($_SESSION['endicia']['pickup_address']) ? $_SESSION['endicia']['pickup_address'] : new stdClass();
  $form['origin'] = array(
    '#type' => 'fieldset',
    '#title' => t('Origin address'),
  );
  $form['origin']['pickup_address'] = array(
    '#type' => 'addressfield',
    '#required' => TRUE,
    '#title' => t('Sender\'s Address'),
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
    '#description' => "Please enter the address you want to mail this package to.",
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

  $address = isset($_SESSION['endicia']['delivery_address']) ? $_SESSION['endicia']['delivery_address'] : new stdClass();
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
    '#description' => "Please enter the address you want to mail this package to.",
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
  $form['destination']['delivery_email'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail'),
    '#size' => 32,
    '#required' => FALSE,
    '#default_value' => isset($address->email) ? $address->email : '',
  );

  $package_info = isset($_SESSION['endicia']['package']) ? $_SESSION['endicia']['package'] : array();
  $form['package_info'] = array(
    '#type' => 'fieldset',
    '#title' => t('Package Information'),
    '#tree' => TRUE,
  );
  $form['package_info']['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#description' => t('This text will appear on the printed shipping label.'),
    '#default_value' => isset($package_info['description']) ? $package_info['description'] : '',
    '#required' => TRUE,
  );
  $form['package_info']['declared_value'] = array(
    '#type' => 'textfield',
    '#title' => t('Declared value'),
    '#size' => 10,
    '#default_value' => isset($package_info['declared_value']) ? $package_info['declared_value'] : 0,
    '#required' => TRUE,
    '#field_prefix'  => '$',
  );
  $form['package_info']['weight'] = array(
    '#type' => 'textfield',
    '#title' => t('Weight'),
    '#size' => 10,
    '#maxlength' => 15,
    '#default_value' => isset($package_info['weight']) ? $package_info['weight'] : 0,
    '#required' => TRUE,
    '#field_suffix' => 'oz.',
  );
  $form['package_info']['length'] = array(
    '#type' => 'textfield',
    '#title' => t('Dimensions'),
    '#default_value' => isset($package_info['length']) ? $package_info['length'] : 0,
    '#size' => 4,
    '#field_suffix' => 'x',
  );
  $form['package_info']['width'] = array(
    '#type' => 'textfield',
    '#default_value' => isset($package_info['width']) ? $package_info['width'] : 0,
    '#size' => 4,
    '#field_suffix' => 'x',
  );
  $form['package_info']['height'] = array(
    '#type' => 'textfield',
    '#description' => t("Enter the package's dimensions (L x W x H) in inches."),
    '#default_value' => isset($package_info['height']) ? $package_info['height'] : 0,
    '#size' => 4,
    '#field_suffix' => 'in.',
  );
  $form['package_info']['mail_shape'] = array(
    '#type' => 'select',
    '#title' => t('Mailpiece Shape'),
    '#description' => 'Select the mailpiece shape to use for this shipment.',
    '#options' => _endicia_pkg_types('enabled'),
    '#default_value' => isset($package_info['mail_shape']) ? $package_info['mail_shape'] : NULL,
  );

  $form['ship_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Shipping options'),
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $options = array(0 => 'N/A');
  $now = time();
  foreach (range(1,7) as $day) {
    $date = format_date($now + 86400 * $day, 'custom', 'l, F j');
    $variables = array('@num' => $day, '@formatted_date' => $date);
    $text = format_plural($day, '@num day - @formatted_date', '@num days - @formatted_date', $variables);
    $options[$day] = $text;
  }
  $form['ship_options']['date_advance'] = array(
    '#type' => 'select',
    '#title' => t('Date Advance'),
    '#description' => 'Enter the number of days advance the date shown on the printed label by.',
    '#options' => $options,
    '#default_value' => isset($package_info['date_advance']) ? $package_info['date_advance'] : 0,
  );
  $form['ship_options']['insurance'] = array(
    '#type' => 'select',
    '#title' => t('Insurance'),
    '#description' => 'USPS online insurance requires one of Delivery Confirmation or Signature Confirmation. Endicia Parcel Insurance offers a maximum insurable value of $10,000. Note that the cost of Endicia Parcel Insurance is NOT included in the postage quotes and is instead billed directly to your account.',
    '#options' => array(
      'None' => 'None requested',
      'USPS' => 'USPS Online Insurance',
      'Endicia' => 'Endicia Parcel Insurance',
    ),
    '#default_value' => isset($package_info['insurance']) ? $package_info['insurance'] : 'None',
  );
  $form['ship_options']['options'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Options'),
    '#options' => array(
      'pickup_confirmation' => 'Delivery confirmation',
      'signature_confirmation' => 'Signature confirmation',
      'hold_for_pickup' => 'Hold for pickup (up to 7 days)',
      'usps_form_2976' => t('Customs Information (USPS form 2976/2976A)'),
    ),
    '#default_value' => isset($package_info['options']) ? $package_info['options'] : array(),
  );


  $form['customs_info'] = array(
    '#type' => 'fieldset',
    '#title' => t('Customs Information'),
    '#description' => t('Enter the information of up to 5 items, if applicable, for use during customs inspection. The information entered here will be used to automatically complete the USPS form 2976 or 2976A (dependant on your selected shipping class). If you leave all of these fields blank, USPS form 2976 will be omitted.'),
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  foreach (range(1, 5) as $i) {
    $form['customs_info']['CustomsItem' . $i] = array(
      '#type' => 'fieldset',
      '#title' => 'Item ' . $i,
      '#tree' => TRUE,
    );
    $fields = array('Description', 'Quantity', 'Weight', 'Value');
    foreach($fields as $field) {
      $form['customs_info']['CustomsItem' . $i][$field] = array(
        '#type' => 'textfield',
        // This is normally A Bad Thing, but it's really reading the same string
        // literals from above so I think it's acceptable.
        '#title' => t($field),
        '#size' => 20,
        '#default_value' => isset($package_info['CustomsItem' . $i][$field]) ? $package_info['CustomsItem' . $i][$field] : '',
      );
    }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Continue',
  );
  return $form;
}

/**
 * Submit handler for the generic "cloud" print label form.
 */
function endicia_ui_print_label_form_validate($form, &$form_state) {
  if ($form_state['values']['package_info']['weight'] == 0) {
    form_error($form['package_info']['weight'], t("The package's entered weight must not be 0."));
  }

  foreach (array('origin' => 'pickup', 'destination' => 'delivery') as $parent_key => $type) {
    $digitsonly = preg_replace("/[^0-9]/", "", $form_state['values'][$type . '_phone']);
    if (strlen($digitsonly) > 10) {
      form_error($form[$parent_key][$type . '_phone'], t('Phone numbers must not contain more than 10 digits.'));
    }
  }

  if (count(form_get_errors())) {
    return;
  }

  // Prepare for API call and create session data
  $_SESSION['endicia'] = array();
  $_SESSION['endicia']['package'] = array();
  $_SESSION['endicia']['package'] += $form_state['values']['package_info'];
  $_SESSION['endicia']['package'] += $form_state['values']['ship_options'];
  $_SESSION['endicia']['package'] += $form_state['values']['customs_info'];

  foreach (array('pickup', 'delivery') as $type) {
    foreach ($form_state['values'][$type . '_address'] as $key => $value) {
      $form_state['values'][$type . '_' . $key] = $value;
    }
    unset($form_state['values'][$type . '_address']);
  }
  $_SESSION['endicia']['pickup_address'] = _endicia_get_address($form_state['values'], 'pickup');
  $_SESSION['endicia']['delivery_address'] = _endicia_get_address($form_state['values'], 'delivery');

  // Make API call, storing any results in session
  $destination = $_SESSION['endicia']['delivery_address'];
  $origin = $_SESSION['endicia']['pickup_address'];
  $package_info = $_SESSION['endicia']['package'];
  $is_international = ($destination->country != 'US');

  // Set request data for API call to calculate postage rates
  $countries = _endicia_country_get_predefined_list();
  $data = array(
    'AccountID' => variable_get('endicia_account_id', ''),
    'Password' => variable_get('endicia_passphrase', ''),
    'MailClass' => ($is_international ? "International" : "Domestic"),
    'MailpieceShape' => $package_info['mail_shape'],
    'Weight' => $package_info['weight'],
    'Sender' => array(
      'PostalCode' => $origin->postal_code,
    ),
    'Recipient' => array(
      'PostalCode' => $destination->postal_code,
      'CountryCode' => $destination->country,
      'Country' => $countries[$destination->country],
    ),
    'CODAmount' => 0,
    'InsuredValue' => $package_info['insurance'] == 'None' ? 0 : $package_info['declared_value'], // Same here.
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
    form_error($form);
    _endicia_communication_error_redirect();
    return;
  }
  // Check for errors
  if ($error = endicia_api_request_error_check($response, $response_node)) {
    $message = t('Could not retrieve postage rate calculations. Please ensure that a valid destination address was entered (check the site log for additional details).');
    form_error($form, $message);
    return;
  }
  // Keep tallies of rates by mail class
  $mail_class_rates = array();
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
  $_SESSION['endicia']['mail_class_rates'] = &$mail_class_rates;

  // Get their account balance
  $account_info = endicia_get_account_status();
  if ($account_info === FALSE || !$account_info) {
    $_SESSION['endicia']['account_balance'] = t('Unknown');
  }
  else {
    $_SESSION['endicia']['account_balance'] = sprintf('$%.2f', $account_info['PostageBalance']);
  }

  if (_endicia_in_test_mode()) {
    drupal_set_message(t('Reminder: Endicia is operating in test mode. Your account balance will not be deducted.'), 'warning');
  }
}

/**
 * Submit handler for the generic "cloud" print label form.
 */
function endicia_ui_print_label_form_submit($form, &$form_state) {
  // Only happens when form_state['rebuild'] !== FALSE
  $form_state['redirect'] = 'admin/endicia/label/print/confirm';
}


/**
 * Theme function for generic print label form.
 *
 * @see endicia_print_label_form().
 */
function theme_endicia_print_label_form($variables) {
  $form = $variables['form'];
  $output = '<div>' . drupal_render($form['instructions']) . '</div>';
  $output .= '<div class="address address-origin">' . drupal_render($form['origin']) . '</div>';
  $output .= '<div class="address address-delivery">' . drupal_render($form['destination']) . '</div>';
  $output .= '<div class="package-info">' . drupal_render($form['package_info']) . '</div>';
  $output .= '<div class="shipping-options">' . drupal_render($form['ship_options']) . '</div>';
  $output .= '<div class="customs-info">' . drupal_render($form['customs_info']) . '</div>';
  $output .= drupal_render_children($form);
  return $output;
}

function endicia_ui_print_label_confirm_form($form_state) {
  // Check to make sure request is valid
  if (!isset($_SESSION['endicia'])) {
    drupal_goto('admin/endicia/label/print');
  }

  // Setup some stuff for use later
  $destination = $_SESSION['endicia']['delivery_address'];
  $origin = $_SESSION['endicia']['pickup_address'];
  $package_info = $_SESSION['endicia']['package'];
  $mail_class_rates = $_SESSION['endicia']['mail_class_rates'];
  $balance = $_SESSION['endicia']['account_balance'];
  $is_international = ($destination->country != 'US');

  // Process pricing information returned from API request and add it to the form.
  $mail_classes = _endicia_mail_classes($is_international ? 'International' : 'Domestic');
  $mail_class_estimates = array();
  foreach ($mail_class_rates as $class => $total) {
    $class_text = isset($mail_classes[$class]) ? $mail_classes[$class] : t('[Title missing] @name', array('@name' => $class));
    $mail_class_estimates[$class] = sprintf('%s - $%.2f', $class_text, $total);
  }

  // Now that we have all the information we need, setup the form
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
    '#default_value' => _endicia_get_service_type($is_international), // FIXME default value not preserved upon error
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Confirm shipment'),
    '#suffix' => l(t('Go Back'), 'admin/endicia/label/print'),
  );

  return $form;
}

function endicia_ui_print_label_confirm_form_submit($form, &$form_state) {
  global $user;
  $origin = $_SESSION['endicia']['pickup_address'];
  $destination = $_SESSION['endicia']['delivery_address'];
  $package_info = $_SESSION['endicia']['package'];
  $is_international = ($destination->country != 'US');

  $countries = _endicia_country_get_predefined_list();
  $data = array(
    'AccountID' => variable_get('endicia_account_id', ''),
    'Password' => variable_get('endicia_passphrase', ''),
    'Class' => $form_state['values']['mail_class'],
    'Packaging' => $package_info['mail_shape'],
    'Weight' => $package_info['weight'], // ounces
    'Description' => $package_info['description'],
    'DeclaredValue' => $package_info['declared_value'], // not needed?
    'CustomerID' => $user->uid,
    'TransactionID' => sprintf('c-%s', time()),
    'Recipient' => array(
      'Name' => $destination->first_name . ' ' . $destination->last_name,
      'Company' => $destination->company,
      'Address1' => $destination->street1,
      'Address2' => $destination->street2,
      'City' => $destination->city,
      'State' => $destination->zone,
      'CountryCode' => $destination->country,
      'Country' => $countries[$destination->country],
      // 'PostalCode' added later
      'Phone' => preg_replace("/[^0-9]/", "", $destination->phone),
      'Email' => $destination->email,
    ),
    'Sender' => array(
      'Name' => $origin->first_name . ' ' . $origin->last_name,
      'Address1' => $origin->street1,
      'Address2' => $origin->street2,
      'City' => $origin->city,
      'State' => $origin->zone,
      'PostalCode' => $origin->postal_code,
      'Phone' => preg_replace("/[^0-9]/", "", $origin->phone),
      'Email' => $origin->email,
    ),
    'Test' => _endicia_in_test_mode() ? 'YES' : 'NO',
    'CostCenter' => 0,
    'LabelFormat' => variable_get('endicia_label_image_format', 'PNG'),
    'Services' => array(),
  );
  // Figure out if we are using a ZIP5 or international postal code.
  if ($is_international || !preg_match('/\d+-\d+/', $destination->postal_code)) {
      $data['Recipient']['PostalCode'] = $destination->postal_code;
  }
  else {
    list($zip5, $zip4) = explode('-', $destination->postal_code, 2);
    $data['Recipient']['PostalCode'] = $zip5;
    $data['Recipient']['PostalCodeZip4'] = $zip4;
  }
  // Determine shipping options
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
        case 'usps_form_2976':
          $data['CustomsInfo'] = array();
          break;
      }
    }
  }

  // Outdated but requests are refused without them.
  $data += array(
    'CustomsQuantity1' => 0, 'CustomsValue1' => 0, 'CustomsWeight1' => 0,
    'CustomsQuantity2' => 0, 'CustomsValue2' => 0, 'CustomsWeight2' => 0,
    'CustomsQuantity3' => 0, 'CustomsValue3' => 0, 'CustomsWeight3' => 0,
    'CustomsQuantity4' => 0, 'CustomsValue4' => 0, 'CustomsWeight4' => 0,
    'CustomsQuantity5' => 0, 'CustomsValue5' => 0, 'CustomsWeight5' => 0,
  );
  if (isset($data['CustomsInfo'])) {
    // Customs integrated forms, if applicable
    $customs_items = array();
    foreach (range(1, 5) as $i) {
      if (!empty($package_info['CustomsItem' . $i]['Description'])) {
        $customs_items[] = $package_info['CustomsItem' . $i];
      }
    }
    $label_type = $is_international ? 'International' : 'Domestic';
    $form_type = 'Form2976';
    // This is ugly, but there isn't much we can do.
    $general_case = ( $label_type == 'International' && in_array($data['Class'], array('PriorityMailInternational', 'ExpressMailInternational')) );
    $exception_int_flenv_prio = $label_type == 'International' && $data['Packaging'] == 'FlatRateEnvelope' && $data['Class'] == 'PriorityMailInternational';
    if ($general_case && !$exception_int_flenv_prio) {
      $form_type .= 'A';
    }
    if (count($customs_items)) {
      if ($form_type == 'Form2976' && count($customs_items) > 5) {
        drupal_set_message(t('Only 5 item descriptors are supported on the USPS Form 2976 due to 4x6 label size constraints.'), 'error');
        return FALSE;
      }
      $data += array(
        'LabelType' => $label_type,
        'LabelSubtype' => 'Integrated',
        'IntegratedFormType' => $form_type,
      );
      $data['CustomsInfo'] += array(
        'ContentsType' => 'Merchandise',
        'CustomsItems' => $customs_items,
      );
    }
  }

  // Determine label size: Endicia uses a WxH notation, we want a 6x4 in all cases
  if ($is_international) {
    $data['LabelSize'] = '4x6c'; // this returns a 6x4 label
  }
  else {
    $data['LabelSize'] = '4x6';
    if (!in_array($data['LabelFormat'], array('EPL2', 'ZPLII'))) {
      $data['LabelRotation'] = 'Rotate90';
    }
  }

  $uri = _endicia_in_test_mode() ? ENDICIA_TEST_URI : ENDICIA_PRODUCTION_URI;
  $response_node = 'LabelRequestResponse';
  $response = endicia_api_request($uri, 'GetPostageLabel', $data, $response_node);
  // Check for failures
  if ($response === FALSE) {
    // watchdog already notified.
    _endicia_communication_error_redirect('admin/endicia/label/print/confirm');
  }
  // Check for errors
  if ($error = endicia_api_request_error_check($response, $response_node)) {
    $message = 'Could not generate label due to error #@errornum: %errormsg (check the site log for additional details).';
    $variables = array(
      '@errornum' => $error['status'],
      '%errormsg' => $error['message'],
    );
    drupal_set_message(t($message, $variables), 'error');
    drupal_goto('admin/endicia/label/print/confirm');
  }

  // If this node is here, only one label was returned
  $image_format = variable_get('endicia_label_image_format', 'PNG');
  $extension = '.' . _endicia_get_file_extension($image_format);

  $result = FALSE;
  $tracking_num = $response->LabelRequestResponse->TrackingNumber;
  if (isset($response->LabelRequestResponse->Base64LabelImage)) {
    // Single image returned as base64
    $filename = 'label-cloud-' . $tracking_num . $extension;
    $data = base64_decode($response->LabelRequestResponse->Base64LabelImage);
    $result = endicia_save_label($filename, $data, 'endicia_ui', $tracking_num);
  }
  else {
    if (is_array($response->LabelRequestResponse->Label->Image)) {
      // Two+ labels returned as base64 under their own nodes
      foreach($response->LabelRequestResponse->Label->Image as $labelpart) {
        $filename = 'label-cloud-' . $tracking_num . '-' . $labelpart->PartNumber . $extension;
        $data = base64_decode($labelpart->_);
        $result = endicia_save_label($filename, $data, 'endicia_ui', $tracking_num);
        if (!$result) {
          break;
        }
      }
    }
    else {
      // One label returned  as base64 under its own node
      $labelpart = &$response->LabelRequestResponse->Label->Image;
      $filename = 'label-cloud-' . $tracking_num . '-' . $labelpart->PartNumber . $extension;
      $data = base64_decode($labelpart->_);
      $result = endicia_save_label($filename, $data, 'endicia_ui', $tracking_num);
    }
  }
  if (!$result) {
    drupal_set_message(t("Could not save the label image due to insufficient permissions. Please verify that Drupal's private files directory exists and is writable."), 'error');
    drupal_goto('admin/endicia/label/print/confirm');
  }
  /*
  $ship_date['year'] = substr($response->LabelRequestResponse->TransactionDateTime, 0, 4);
  $ship_date['month'] = substr($response->LabelRequestResponse->TransactionDateTime, 4, 2);
  $ship_date['day'] = substr($response->LabelRequestResponse->TransactionDateTime, 6, 2);
  $ship_date['hour'] = substr($response->LabelRequestResponse->TransactionDateTime, 8, 2);
  $ship_date['min'] = substr($response->LabelRequestResponse->TransactionDateTime, 10, 2);
  $ship_date['sec'] = substr($response->LabelRequestResponse->TransactionDateTime, 12, 2);
  $timestamp = gmmktime($ship_date['hour'], $ship_date['min'], $ship_date['sec'], $ship_date['month'], $ship_date['day'], $ship_date['year']);
  */

  unset($_SESSION['endicia']);
  // FIXME: What about multiple labels returned?
  $form_state['redirect'] = 'endicia/print/endicia_labels/' . $filename;
}
