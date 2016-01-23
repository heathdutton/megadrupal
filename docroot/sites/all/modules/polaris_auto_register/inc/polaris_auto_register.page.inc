<?php

/**
 * @file
 * Provides page related functions for the Polaris Auto Register module.
 */

/**
 * Main registration form
 */
function polaris_auto_register_form($form, $form_state) {

  $form = array();

  $form['#attached']['js'] = array(
    drupal_get_path('module', 'polaris_auto_register') . '/js/polaris_auto_register.min.js',
  );

  $form['polaris_subtitle'] = array(
    '#markup' => variable_get('polaris_auto_register_subtitle')
  );

  $form['polaris_markup1'] = array(
    '#markup' => variable_get('polaris_auto_register_markup1')
  );

  $form['polaris_markup2'] = array(
    '#markup' => variable_get('polaris_auto_register_markup2')
  );

  $form['polaris_markup3'] = array(
    '#markup' => variable_get('polaris_auto_register_markup3')
  );

  drupal_add_css(drupal_get_path('module', 'polaris_auto_register') . '/css/polaris_auto_register.css');

  $form['polaris_auto_register_name_first'] = array(
    '#type' => 'textfield',
    '#title' => t('First Name'),
    '#prefix' => '<div class="or_control">',
    '#suffix' => '</div>',
    '#size' => 26,
    '#maxlength' => 50,
    '#required' => TRUE,
  );

  $form['polaris_auto_register_name_middle'] = array(
    '#type' => 'textfield',
    '#title' => t('Middle Name'),
    '#prefix' => '<div class="or_control">',
    '#suffix' => '</div>',
    '#size' => 26,
    '#maxlength' => 50,
    '#required' => FALSE,
  );

  $form['polaris_auto_register_name_last'] = array(
    '#type' => 'textfield',
    '#title' => t('Last Name'),
    '#prefix' => '<div class="or_control">',
    '#suffix' => '</div>',
    '#size' => 26,
    '#maxlength' => 50,
    '#required' => TRUE,
  );

  $form['clear_float1'] = array(
    '#markup' => '<div class="or_clear"></div>',
  );

  $form['polaris_auto_register_phone_voice_1'] = array(
    '#type' => 'textfield',
    '#title' => t('Phone Number'),
    '#prefix' => '<div class="or_control">',
    '#suffix' => '</div>',
    '#size' => 20,
    '#maxlength' => 15,
    '#required' => TRUE,
  );

  $form['polaris_auto_register_phone_voice_2'] = array(
    '#type' => 'textfield',
    '#title' => t('Alternate Phone Number'),
    '#prefix' => '<div class="or_control phone2">',
    '#suffix' => '</div>',
    '#size' => 20,
    '#maxlength' => 15,
    '#required' => FALSE,
  );

  $form['polaris_auto_register_add_phone_num'] = array(
    '#markup' => '<div class ="or_control alt_phone"><a href="">' .
    'Add Alternate Phone Number</a></div>',
  );

  $form['clear_float1a'] = array(
    '#markup' => '<div class="or_clear"></div>',
  );

  $form['polaris_auto_register_phone1_validation_msg'] = array(
    '#markup' => '<div class="or_control phone1_error"></div>',
  );

  $form['polaris_auto_register_phone2_validation_msg'] = array(
    '#markup' => '<div class="or_control phone2_error"></div>',
  );

  $form['clear_float1b'] = array(
    '#markup' => '<div class="or_clear"></div>',
  );

  $form['polaris_auto_register_dob'] = array(
    '#type' => 'date',
    '#title' => t('Birth Date'),
    '#prefix' => '<div class="or_control">',
    '#suffix' => '</div>',
    '#size' => 12,
    '#required' => TRUE,
  );

  $form['clear_float3'] = array(
    '#markup' => '<div class="or_clear"></div>',
  );

  $form['polaris_auto_register_dob_warning'] = array(
    '#markup' => 'We\'re sorry. Customers under 13 need a parent\'s signature' .
    ' when applying for a library card. Please come in to a library' .
    ' <a href="/locations">location</a> to complete your application.',
    '#prefix' => '<div class="dob_error">',
    '#suffix' => '</div>',
  );

  $form['polaris_auto_register_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email Address'),
    '#prefix' => '<div class="or_control">',
    '#suffix' => '</div>',
    '#size' => 35,
    '#maxlength' => 50,
    '#required' => TRUE,
  );

  // AJAX progress indicator
  $image = drupal_get_path('module', 'polaris_auto_register') . '/images/ajax-loader.gif';

  $form['ajax_progress'] = array(
    '#markup' => '<div class="progress"><img src="/' . $image . '"" alt="Please wait....."></div>');

  $form['clear_float2'] = array(
    '#markup' => '<div class="or_clear"></div>',
  );

  $form['email_validation_msg'] = array(
    '#markup' => '<blockquote style="display:none" class="email_msg"></blockquote>',
  );


  $form['polaris_auto_register_street_one'] = array(
    '#type' => 'textfield',
    '#title' => t('Street Address'),
    '#prefix' => '<div class="or_control">',
    '#suffix' => '</div>',
    '#size' => 35,
    '#maxlength' => 50,
    '#required' => TRUE,
  );

  $form['clear_float4'] = array(
    '#markup' => '<div class="or_clear"></div>',
  );

  $form['polaris_auto_register_street_two'] = array(
    '#type' => 'textfield',
    '#title' => t('Street Address Line 2'),
    '#prefix' => '<div class="or_control">',
    '#suffix' => '</div>',
    '#size' => 35,
    '#maxlength' => 50,
    '#required' => FALSE,
  );

  $form['clear_float5'] = array(
    '#markup' => '<div class="or_clear"></div>',
  );

  $form['polaris_auto_register_city'] = array(
    '#type' => 'textfield',
    '#title' => t('City'),
    '#prefix' => '<div class="or_control">',
    '#suffix' => '</div>',
    '#size' => 25,
    '#maxlength' => 50,
    '#required' => TRUE,
  );

  if (module_exists('webform')) {
    // Include the file "webform.options.inc" from the Webform module. Allows use of the states function below.
    module_load_include('inc', 'webform', 'includes/webform.options');
    $webform_states = webform_options_united_states(NULL, NULL, NULL, NULL); // Pulls the list of states from the Webform module's list.
    $webform_states = array('' => '-Select-') + $webform_states;
    $form['polaris_auto_register_state'] = array(
      '#type' => 'select',
      '#title' => t('State'),
      '#prefix' => '<div class="or_control">',
      '#suffix' => '</div>',
      '#options' => $webform_states,
    );
  }
  else {
    $form['polaris_auto_register_state'] = array(
    '#type' => 'textfield',
    '#title' => t('State'),
    '#prefix' => '<div class="or_control">',
    '#suffix' => '</div>',
    '#size' => 5,
    '#maxlength' => 2,
    '#required' => TRUE,
  );
  }

  $form['polaris_auto_register_postal_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Zip Code'),
    '#prefix' => '<div class="or_control">',
    '#suffix' => '</div>',
    '#size' => 10,
    '#maxlength' => 5,
    '#required' => TRUE,
  );
  $form['polaris_auto_register_zip_plus_four'] = array(
    '#type' => 'hidden',
    '#title' => t('Zip Plus 4'),
    '#prefix' => '<div class="or_control">',
    '#suffix' => '</div>',
    '#size' => 10,
    '#maxlength' => 10,
  );
  $form['polaris_auto_register_county'] = array(
    '#type' => 'hidden',
    '#title' => t('County'),
    '#size' => 25,
    '#maxlength' => 25,
  );

  $form['polaris_auto_register_country'] = array(
    '#type' => 'select',
    '#title' => t('Country'),
    '#prefix' => '<div class="or_control">',
    '#suffix' => '</div>',
    '#options' => array(
      '2' => 'Canada',
      '3' => 'Mexico',
      '1' => 'United States',
    ),
    '#required' => TRUE,
    '#default_value' => '1',
  );


  // The following 5 controls will only show up if a question/label and a description have been defined in
  // the configuration settings for Polaris System Defined Fields User1 through User5.
  $user1_label = variable_get('polaris_auto_register_user1_label');
  $user1_description = variable_get('polaris_auto_register_user1_info');

  if (!empty($user1_label)) {
    $form['polaris_auto_register_user1_field'] = array(
      '#type' => 'textfield',
      '#title' => $user1_label,
      '#prefix' => '<div class="or_control">',
      '#suffix' => '</div>',
      '#description' => $user1_description,
      '#size' => 50,
      '#maxlength' => 50,
      '#required' => FALSE,
    );
  }

  $user2_label = variable_get('polaris_auto_register_user2_label');
  $user2_description = variable_get('polaris_auto_register_user2_info');

  if (!empty($user2_label)) {
    $form['polaris_auto_register_user2_field'] = array(
      '#type' => 'textfield',
      '#title' => $user2_label,
      '#prefix' => '<div class="or_control">',
      '#suffix' => '</div>',
      '#description' => $user2_description,
      '#size' => 50,
      '#maxlength' => 50,
      '#required' => FALSE,
    );
  }

  $form['clear_float7'] = array(
    '#markup' => '<div class="or_clear"></div>',
  );

  $user3_label = variable_get('polaris_auto_register_user3_label');
  $user3_description = variable_get('polaris_auto_register_user3_info');

  if (!empty($user3_label)) {
    $form['polaris_auto_register_user3_field'] = array(
      '#type' => 'textfield',
      '#title' => $user3_label,
      '#prefix' => '<div class="or_control">',
      '#suffix' => '</div>',
      '#description' => $user3_description,
      '#size' => 50,
      '#maxlength' => 50,
      '#required' => FALSE,
    );
  }

  $user4_label = variable_get('polaris_auto_register_user4_label');
  $user4_description = variable_get('polaris_auto_register_user4_info');

  if (!empty($user4_label)) {
    $form['polaris_auto_register_user4_field'] = array(
      '#type' => 'textfield',
      '#title' => $user4_label,
      '#prefix' => '<div class="or_control">',
      '#suffix' => '</div>',
      '#description' => $user4_description,
      '#size' => 50,
      '#maxlength' => 50,
      '#required' => FALSE,
    );
  }

  $form['clear_float8'] = array(
    '#markup' => '<div class="or_clear"></div>',
  );

  $user5_label = variable_get('polaris_auto_register_user5_label');
  $user5_description = variable_get('polaris_auto_register_user5_info');

  if (!empty($user5_label)) {
    $form['polaris_auto_register_user5_field'] = array(
      '#type' => 'textfield',
      '#title' => $user5_label,
      '#prefix' => '<div class="or_control">',
      '#suffix' => '</div>',
      '#description' => $user5_description,
      '#size' => 50,
      '#maxlength' => 50,
      '#required' => FALSE,
    );
  }

  // the password field will only be shown if the option to use the last four digits of the primary phone has not been
  // selected in the configuration settings

  if (variable_get('polaris_auto_register_user_pin') == 0) {
    $form['polaris_auto_register_password'] = array(
      '#type' => 'textfield',
      '#title' => t('PIN/Password'),
      '#description' => t('Enter a four digit number to be used as a PIN'),
      '#size' => 10,
      '#maxlength' => 4,
      '#required' => TRUE,
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;

} // end of function polaris_auto_register_form

/**
 * Validation function for selected fields;
 * make sure we have two letters for the state and
 * 5 digits for the zip code
 */
function  polaris_auto_register_form_validate($form, &$form_state) {
  $zip = $form_state['values']['polaris_auto_register_postal_code'];
  $zip_length = strlen((string)$zip);
  $address1 = $form_state['values']['polaris_auto_register_street_one'];
  $city = $form_state['values']['polaris_auto_register_city'];
  $state = $form_state['values']['polaris_auto_register_state'];
  $email = $form_state['values']['polaris_auto_register_email'];

  // Validate the dob - must be 13 to self register for a card
  // convert birthdate to string
  $dob = $form_state['values']['polaris_auto_register_dob']['month'] . '/';
  $dob .= $form_state['values']['polaris_auto_register_dob']['day'] . '/';
  $dob .= $form_state['values']['polaris_auto_register_dob']['year'];

  // format the date as yyyy-mm-dd for Polaris
  $year = $form_state['values']['polaris_auto_register_dob']['year'];
  $month = $form_state['values']['polaris_auto_register_dob']['month'];
  $day = $form_state['values']['polaris_auto_register_dob']['day'];

  // make sure month is two digits
  if (strlen($month) < 2) {
    $month = '0' . $month;
  }

  // make sure day is two digits
  if (strlen($day) < 2) {
    $day = '0' . $day;
  }

  $_SESSION['dob'] = $year . '-' . $month . '-' . $day;
  // check the age; this code adapted from http://stackoverflow.com/questions/3776682/php-calculate-age
  $dob = explode("/", $dob);

  $age = (date("md", date("U", mktime(0, 0, 0, $dob[0], $dob[1], $dob[2]))) > date("md")
    ? ((date("Y") - $dob[2]) - 1)
    : (date("Y") - $dob[2]));

  if ($age < 14 && !isset($form_state['values']['polaris_auto_register_barcode1'])) {
    form_set_error('polaris_auto_register_dob', t('We\'re sorry. Customers under 13 need a parent\'s signature when applying for a library card. Please come in to a <a href="/locations">library location</a> to complete your application.'));
  }

  // Validate the zip code
  if (!is_numeric($zip)) {
    form_set_error('polaris_auto_register_postal_code', t('The zip code you entered contained non-numeric. A valid US zip code is all numbers. Please re-enter your zip code and submit the form again.'));
  }

  if ($zip_length <> 5) {
    form_set_error('polaris_auto_register_postal_code', t('You entered <b>' . $zip_length . '</b> digits for a zip code.<br> A valid US zip code is 5 digits. Please re-enter your zip code and submit the form again.'));
  }

  // Validate the state
  if (!module_exists('webform')) {
    $us_states = array('AL', 'AK', 'AR', 'AZ', 'CA', 'CO', 'CT', 'DE', 'DC', 'FL', 'GA', 'HI', 'ID', 'IL', 'IN', 'IA', 'KS', 'KY', 'LA', 'MA', 'MD', 'MA', 'MI', 'MN', 'MS', 'MO', 'MT', 'NE', 'NV', 'NH', 'NJ', 'NM', 'NY', 'ND', 'NC', 'OH', 'OK', 'OR', 'PA', 'RI', 'SC', 'SD', 'TN', 'TX', 'UT', 'VA', 'VT', 'WA', 'WV', 'WI', 'WY');
    if (!in_array(strtoupper($state), $us_states)) {
      form_set_error('polaris_auto_register_state', t('You entered <b>' . $state . '</b> for state. <br>Please enter a valid, 2 letter abbreviation for a US state.'));
    }
  }

  // Validate the email
  $email = $form_state['values']['polaris_auto_register_email'];

  if (!filter_var($email, FILTER_VALIDATE_EMAIL)) {
    form_set_error('polaris_auto_register_email', t('We are having trouble validating the email address you entered. Please try again.'));
  }

  // Validate the password (if entered); must be all digits and four digits long
  if (variable_get('polaris_auto_register_user_pin') == 0) {
    if (!is_numeric($form_state['values']['polaris_auto_register_password'])) {
      form_set_error('polaris_auto_register_password', t('A valid PIN/Password is four numbers. Please try again.'));
    }
    else {
      $length = ceil(log10($form_state['values']['polaris_auto_register_password']));
      if ($length <> 4) {
        form_set_error('polaris_auto_register_password', t('A valid PIN/Password is four numbers. Please try again.'));
      }
    }
  }

  if ($form_state['values']['polaris_auto_register_country'] == 1) { // 1 = a US address. SmartStreets doesn't work with international addresses.
    // Validate the address; make sure they're in the necessary library county
    $location = $address1 . ', ' . $city . ', ' . $state . ' ' . $zip;

    $result_a = smartystreets_liveaddress_api_call($location);

    if (!$result_a) {
      form_set_error('polaris_auto_register_street_one', t('We are having trouble validating the address you entered. Please try again, <a href="/ask-us">ask a librarian</a>, or call ' . variable_get('polaris_auto_register_help_phone') . ' for help.'));
    }
    else {
      $county = strtoupper($result_a[0]->metadata->county_name);
      $approved_counties = str_replace(', ', ',', strtoupper(variable_get('polaris_auto_register_county')));
      $approved_counties = explode(',', $approved_counties);
      $state = strtoupper($result_a[0]->components->state_abbreviation);
      $approved_states = str_replace(', ', ',', strtoupper(variable_get('polaris_auto_register_state')));
      $approved_states = explode(',', $approved_states);

      if (!in_array($county, $approved_counties) || !in_array($state, $approved_states)) {
        form_set_error('polaris_auto_register_postal_code', t('The zip code you entered is not in the county ' .
        'or counties served by ' . variable_get('site_name') . '.<br>You can still get a ' . variable_get('site_name') .
        ' card by applying for an <a href="/about/get-library-card/library-card-registration">out-of-county card</a>.'));
      }
      else {
        // The county is right.
        $form_state['values']['polaris_auto_register_zip_plus_four'] = $result_a[0]->components->plus4_code;
        $form_state['values']['polaris_auto_register_county'] = $result_a[0]->metadata->county_name;
      }
    }
  }
  /*elseif (variable_get('polaris_auto_register_other_countries') == TRUE) {
    // Bypass address validation. Addresses outside of US are fine.
    $form_state['values']['polaris_auto_register_zip_plus_four'] = '';
    $form_state['values']['polaris_auto_register_county'] = '';
    $form_state['values']['polaris_auto_register_state'] = 'Unknown';
  }*/
  else {
    form_set_error('polaris_auto_register_country', t('The country you entered is not in the United States. Only US customers are currently able to use this application to apply for a library card from ' . variable_get('site_name') . '.<br>You can still get a ' . variable_get('site_name') .
      ' card by applying for an <a href="/about/get-library-card/library-card-registration">out-of-county card</a>.'));
  }

  // Validate the date of birth - make sure it's not today's day
  // Convert birthdate year to string
  $dob = $form_state['values']['polaris_auto_register_dob']['year'];
  if ($dob == date("Y")) {
    form_set_error('polaris_auto_register_dob', t('Please enter a valid birthdate.'));
  }

  // Validate the phone numbers
  $phone = $form_state['values']['polaris_auto_register_phone_voice_1'];
  $_SESSION['phone_voice1'] = polaris_auto_register_simplfy_phone($phone);

  if (strlen($_SESSION['phone_voice1']) != 12) {
    form_set_error('polaris_auto_register_phone_voice_1', t('The primary telephone number appears to be invalid. Please enter a valid phone number, area code first, with no parentheses around the area code and dashes in the appropriate spots.'));
  }

  if ($form_state['values']['polaris_auto_register_phone_voice_2']) {
    $phone = $form_state['values']['polaris_auto_register_phone_voice_2'];
    $_SESSION['phone_voice2'] = polaris_auto_register_simplfy_phone($phone);

    if (strlen($_SESSION['phone_voice2']) != 12) {
      form_set_error('polaris_auto_register_phone_voice_2', t('The alternate telephone number appears to be invalid. Please enter a valid phone number, area code first, with no parentheses around the area code and dashes in the appropriate spots.'));
    }
  }

} // end of function  polaris_auto_register_form_validate

/**
 * Submit form data to Polaris
 *
 * curl example adapted from http://uspswebtools.blogspot.com/
 *
 */
function  polaris_auto_register_form_submit($form, &$form_state) {

  // set value of the PIN/password;
  // first use last four of phone number if that option is set in configuration
  if (variable_get('polaris_auto_register_user_pin') == 1) {
    $password = $form_state['values']['polaris_auto_register_phone_voice_1'];
    $password = substr($password, -4);
  }
  else { // use the password entered by the user
    $password = $form_state['values']['polaris_auto_register_password'];
    $password = substr($password, -4);
  }

  // set values of the Polaris System Defined Fields
  if (isset($form_state['values']['polaris_auto_register_user1_field'])) {
    $user1 = $form_state['values']['polaris_auto_register_user1_field'];
  }
  else {
    $user1 = '';
  }

  if (isset($form_state['values']['polaris_auto_register_user2_field'])) {
    $user2 = $form_state['values']['polaris_auto_register_user2_field'];
  }
  else {
    $user2 = '';
  }

  if (isset($form_state['values']['polaris_auto_register_user3_field'])) {
    $user3 = $form_state['values']['polaris_auto_register_user3_field'];
  }
  else {
    $user3 = '';
  }

  if (isset($form_state['values']['polaris_auto_register_user4_field'])) {
    $user4 = $form_state['values']['polaris_auto_register_user4_field'];
  }
  else {
    $user4 = '';
  }

  if (isset($form_state['values']['polaris_auto_register_user5_field'])) {
    $user5 = $form_state['values']['polaris_auto_register_user5_field'];
  }
  else {
    $user5 = '';
  }

  // these four settings are set in the configuration settings for the module
  $logon_branch_id = variable_get('polaris_auto_register_logon_branch_id');
  $logon_user_id = variable_get('polaris_auto_register_logon_user_id');
  $logon_workstation_id = variable_get('polaris_auto_register_logon_branch_id');
  $patron_branch_id = variable_get('polaris_auto_register_patron_branch_id');

  // these values are supplied by the customer filling out the form, or are defaulted (ZipPlusFour is derived)
  $postal_code = $form_state['values']['polaris_auto_register_postal_code'];
  $zip_plus_four = $form_state['values']['polaris_auto_register_zip_plus_four'];
  $city = ucwords($form_state['values']['polaris_auto_register_city']);
  $state = strtoupper($form_state['values']['polaris_auto_register_state']);
  $county = $form_state['values']['polaris_auto_register_county'];
  $country_id = $form_state['values']['polaris_auto_register_country'];

  // these values are supplied by the customer filling out the form, or are defaulted
  $street_one = ucwords($form_state['values']['polaris_auto_register_street_one']);
  $street_two = ucwords($form_state['values']['polaris_auto_register_street_two']);
  $name_first = ucfirst($form_state['values']['polaris_auto_register_name_first']);
  $name_last = ucfirst($form_state['values']['polaris_auto_register_name_last']);
  $name_middle = ucfirst($form_state['values']['polaris_auto_register_name_middle']);

  // these values are supplied by the customer filling out the form, or are defaulted
  $gender = 'N';
  $dob = $_SESSION['dob'];

  $email_address = $form_state['values']['polaris_auto_register_email'];
  $language_id = NULL;
  $delivery_option_id = NULL;
  $user_name = NULL;  // future and not used

  // See if the user entered a barcode value on the form. Some users have a card in hand (e.g., ConnectED students)
  if (isset($form_state['values']['polaris_auto_register_barcode1'])) {
    $barcode = $form_state['values']['polaris_auto_register_barcode1'];
  }
  else { // User doesn't have a card in hand. Auto-generate the number for him/her.
    // Get the new barcode. Use the largest previously-recorded barcode as a starter.
    $result = db_query_range('SELECT pb.barcode FROM {polaris_barcodes} pb ORDER BY pb.barcode DESC', 0, 1);
    foreach ($result as $record) {
      $barcode = $record->barcode;
    }
  }

  $result = PolarisAPI::patronRegistrationCreate($logon_branch_id, $logon_user_id,
    $logon_workstation_id, $patron_branch_id, $postal_code, $zip_plus_four, $city,
    $state, $county, $country_id, $street_one, $street_two, $name_first, $name_last,
    $name_middle, $user1, $user2, $user3, $user4, $user5, $gender, $dob,
    $_SESSION['phone_voice1'], $_SESSION['phone_voice2'], $email_address, $language_id,
    $delivery_option_id, $user_name, $password, $barcode);

  // if we have a valid result, load the barcode, first name, email, and pin into a session variable
  if ($result['status'] == TRUE) {
    polaris_auto_register_write_barcode($result, $name_first, $name_last, $email_address, $password, $form_state['values']['polaris_auto_register_barcode1']);
    $form_state['redirect'] = 'polaris-auto-register/successful-registration';
  }

  if ($result['status'] == FALSE) { // check to see if there is a -3505 error with this $barcode in the watchdog table
    $my_result = db_query_range('SELECT wd.variables FROM {watchdog} wd WHERE variables LIKE \'%-3505%\' AND variables LIKE \'%' .
    $barcode . '%\'ORDER BY wd.wid DESC', 0, 1);

    if ($my_result && !isset($form_state['values']['polaris_auto_register_barcode1'])) { // barcode we submitted was already used somewhere else in the system; try again

      // keep incrementing the barcode until we get a fresh one
      $count = 0;
      do {
        $barcode = ltrim($barcode, '9999990');
        $barcode = intval($barcode) + 1;
        $barcode = '9999990' . strval($barcode);

        $result = PolarisAPI::patronRegistrationCreate($logon_branch_id, $logon_user_id, $logon_workstation_id, $patron_branch_id,
        $postal_code, $zip_plus_four, $city, $state, $county, $country_id, $street_one, $street_two, $name_first, $name_last, $name_middle,
        $user1, $user2, $user3, $user4, $user5, $gender, $dob, $_SESSION['phone_voice1'], $_SESSION['phone_voice2'], $email_address, $language_id,
        $delivery_option_id, $user_name, $password, $barcode);

        $count = $count + 1;

      } while (($result['status'] == FALSE) AND ($count < 100));

      if ($result['status'] == TRUE) {
        polaris_auto_register_write_barcode($result, $name_first, $name_last, $email_address, $password);
        $form_state['redirect'] = 'polaris-auto-register/successful-registration';
      }
      else { // no good barcode in 100 tries; sound the alarm
        form_set_error('submit', t('There was a problem with your registration. Please try again, <a href="/ask-us">ask a librarian</a>, or call ' . variable_get('polaris_auto_register_help_phone') . ' for help.'));
      }
    }
    else { // some other problem; set an error
      form_set_error('submit', t('Some of the information you entered appears to already be in use. Please double-check your email address and card number and try again, <a href="/ask-us">ask a librarian</a>, or call ' . variable_get('polaris_auto_register_help_phone') . ' for help.'));
    }
  }

} // end of function  polaris_auto_register_form_submit

function polaris_auto_register_simplfy_phone($phone) {
  // Eliminate every char except 0-9
  $just_nums = preg_replace("/[^0-9]/", '', $phone);
  // Eliminate leading 1 if it's there
  if (strlen($just_nums) == 11) {
    $just_nums = preg_replace("/^1/", '', $just_nums);
  }
  // Add hyphens back in.
  $first_phone = substr($just_nums, 0, 3) . '-' . substr($just_nums, 3);
  $final_phone = substr($first_phone, 0, 7) . '-' . substr($first_phone, 7);
  return $final_phone;
}

/*
 * Saves certain values to session for easy access.
 */
function polaris_auto_register_write_barcode($result, $name_first, $name_last, $email_address, $password, $barcode = NULL) {
  $entered_barcode = FALSE;
  if (empty($barcode)) {
    $barcode = $result['response']->Barcode;
  }
  else {
    $entered_barcode = TRUE;
  }
  $_SESSION['barcode_values'] = array(
    'barcode' => $barcode,
    'fname' => $name_first,
    'lname' => $name_last,
    'pin' => $password,
    'email' => $email_address,
  );

  // Success, so write the new barcode to the database (if not manually entered on the form).
  if ($entered_barcode == FALSE) {
    $table = 'polaris_barcodes';
    $date = new DateTime();
    $timestamp = $date->getTimestamp();
    $record = array(
      'barcode' => $_SESSION['barcode_values']['barcode'],
      'created' => $timestamp,
    );
    drupal_write_record($table, $record);
  }
}
