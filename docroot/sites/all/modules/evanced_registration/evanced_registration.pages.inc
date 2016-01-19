<?php

/**
 * @file
 * Evanced Registration non-admin pages
 */



/**
 * Menu callback function to produce the main form.
 */
function evanced_registration_form($form, &$form_state, $evanced_id) {
  $event_title = evanced_registration_get_title($evanced_id);

  // Grab the form from Evanced. We're going to look for custom stipulations like a possible required library card number field.
  $url = EVANCED_HOST . 'eventsignup.asp?ID=' . check_plain($evanced_id);
  $result = drupal_http_request($url);
  $data = drupal_convert_to_utf8($result->data, 'Windows-1252');
  $data = trim(preg_replace('/\s+/', ' ', $data));

  // Get the custom stipulations/questions from the Evanced version of the registration form.
  $custom_stipulations = array();
  $custom_stipulation_labels = array();
  for ($x=1; $x < 6; $x++) {
    unset($matches, $matches2);
    preg_match('/(<input type=text name=CustomStip' . $x . ')/U', $data, $matches); // U makes it an ungreedy match.
    $custom_stipulations[$x] = $matches[1];
    preg_match('/<td class="entryformlabel">([^<]*)<\/td> <td class="entryformdata"><input type=text name=CustomStip' . $x . '/U', $data, $matches2); // U makes it an ungreedy match.
    $custom_stipulation_labels[$x] = str_replace(array(':', '*'), '', $matches2[1]);
  }
  // Get the description from Evanced.
  preg_match('/(<b>Event Type.*)<\/td>/U', $data, $matches3); // U makes it an ungreedy match.
  $event_info = strip_tags(str_replace('</div>', '<br>', str_replace('&nbsp;', '', $matches3[1])), '<br><a><b><strong><i><em>');
  
  // Check for full registration (Don't allow overbooking!)
  preg_match('/(This event is currently full.)/U', $data, $matches4); // U makes it an ungreedy match.
  $event_full = $matches4[1];

  // Check for age restrictions.
  preg_match('/(Attendee must be between the ages of .* and .* old.)/U', $data, $matches5); // U makes it an ungreedy match.
  $age_restriction = $matches5[1];
  if (!empty($age_restriction)) {
    $event_info .= '<b>* ' . $age_restriction . '</b>';
  }
  preg_match('/Attendee must be between the ages of (.*) and (.*) old./U', $age_restriction, $matches_age_restriction); // U makes it an ungreedy match.
  $age_restriction_start = $matches_age_restriction[1];
  $age_restriction_end = $matches_age_restriction[2];

  // Check for registration not started.
  preg_match('/(We&#039;re sorry. The first registration date for this event is scheduled for .*.<BR>Please come back and register at that time.)/U', $data, $matches_registration_date); // U makes it an ungreedy match.
  $registration_not_started = $matches_registration_date[1];
  if (!empty($registration_not_started)) {
    $event_info .= '<br><b>* ' . $registration_not_started . '</b>';
  }

  if (!empty($event_full) || !empty($registration_not_started)) {
    $form['Overview'] = array(
      '#markup' => $event_info,
    );
    // Do nothing further. The message should be in the above description about the event registration being full.
  }
  else if (!empty($event_title) && $event_title != FALSE) { // Don't show the form if we don't have a record of it in Drupal.
    $form['Overview'] = array(
      '#markup' => $event_info,
    );
    $form['EventID'] = array(
      '#type' => 'hidden',
      '#value' => $evanced_id,
    );
    $form['AgeCheck'] = array(
      '#type' => 'hidden',
      '#value' => '0',
    );
    $form['GradeCheck'] = array(
      '#type' => 'hidden',
      '#value' => '0',
    );
    $form['BirthDateCheck'] = array(
      '#type' => 'hidden',
      '#value' => '0',
    );
    $form['FormAction'] = array(
      '#type' => 'hidden',
      '#value' => 'ValidateEventSignUp',
    );
    $form['First'] = array(
      '#type' => 'textfield',
      '#title' => t('Attendee First Name'),
      '#required' => TRUE,
      '#size' => 50,
      '#default_value' => isset($default_value['First']) ? $default_value['First'] : '',
    );
    $form['Last'] = array(
      '#type' => 'textfield',
      '#title' => t('Attendee Last Name'),
      '#required' => TRUE,
      '#size' => 50,
      '#default_value' => isset($default_value['Last']) ? $default_value['Last'] : '',
    );
    $form['PhoneAC'] = array(
      '#type' => 'textfield',
      '#title' => t('Attendee Phone Number'),
      '#required' => TRUE,
      '#size' => 20,
      '#maxlength' => 21,
      '#default_value' => isset($default_value['PhoneAC']) ? $default_value['PhoneAC'] : '',
    );
    $form['PhoneExt'] = array(
      '#type' => 'hidden',
      '#title' => t('Ext.'),
      '#size' => 4,
      '#maxlength' => 5,
      '#default_value' => isset($default_value['PhoneExt']) ? $default_value['PhoneExt'] : '',
    );
    $form['AreaCode2'] = array(
      '#type' => 'hidden',
      '#title' => t('Alternate Phone Number'),
      '#size' => 20,
      '#maxlength' => 21,
      '#default_value' => isset($default_value['AreaCode2']) ? $default_value['AreaCode2'] : '',
    );
    $form['Phone2Ext'] = array(
      '#type' => 'hidden',
      '#title' => t('Ext.'),
      '#size' => 4,
      '#maxlength' => 5,
      '#default_value' => isset($default_value['Phone2Ext']) ? $default_value['Phone2Ext'] : '',
    );
    $form['Email'] = array(
      '#type' => 'textfield',
      '#title' => t('Email'),
      '#size' => 50,
      '#default_value' => isset($default_value['Email']) ? $default_value['Email'] : '',
      '#required' => TRUE,
    );
    if (!empty($age_restriction)) {
      $form['Age'] = array(
        '#type' => 'select',
        '#title' => t('Attendee Age'),
        '#options' => array(
          '' => t('Please select'),
          1 => '1 Month',
          2 => '2 Months',
          3 => '3 Months',
          4 => '4 Months',
          5 => '5 Months',
          6 => '6 Months',
          7 => '7 Months',
          8 => '8 Months',
          9 => '9 Months',
          10 => '10 Months',
          11 => '11 Months',
          12 => '12 Months',
          13 => '13 Months',
          14 => '14 Months',
          15 => '15 Months',
          16 => '16 Months',
          17 => '17 Months',
          18 => '18 Months',
          19 => '19 Months',
          20 => '20 Months',
          21 => '21 Months',
          22 => '22 Months',
          23 => '23 Months',
          24 => '2 Years',
          30 => '2 1/2 Years',
          36 => '3 Years',
          42 => '3 1/2 Years',
          48 => '4 Years',
          54 => '4 1/2 Years',
          60 => '5 Years',
          66 => '5 1/2 Years',
          72 => '6 Years',
          84 => '7 Years',
          96 => '8 Years',
          108 => '9 Years',
          120 => '10 Years',
          132 => '11 Years',
          144 => '12 Years',
          156 => '13 Years',
          168 => '14 Years',
          180 => '15 Years',
          192 => '16 Years',
          204 => '17 Years',
          216 => '18 Years',
          228 => '19 Years',
          240 => '20 Years',
          252 => '21 Years',
          264 => '22+ Years'
        ),
        '#default_value' => isset($default_value['Age']) ? $default_value['Age'] : '',
        '#required' => TRUE,
      );
      $form['age_restriction_start'] = array(
        '#type' => 'hidden',
        '#value' => $age_restriction_start,
      );
      $form['age_restriction_end'] = array(
        '#type' => 'hidden',
        '#value' => $age_restriction_end,
      );
    }
    // Add in the custom stipulation fields from Evanced if the source registration page had them.
    for ($x=1; $x < 6; $x++) {
      if (!empty($custom_stipulations[$x])) {
        $form['CustomStip' . $x] = array(
          '#type' => 'textfield',
          '#title' => !empty($custom_stipulation_labels[$x]) ? $custom_stipulation_labels[$x] : 'Custom Stipulation ' . $x,
          '#size' => 20,
          '#maxlength' => 60,
          '#default_value' => isset($default_value['CustomStip' . $x]) ? $default_value['CustomStip' . $x] : '',
          '#required' => TRUE,
        );
      }
    }
    $form['Group'] = array(
      '#type' => 'hidden',
      '#title' => t('Number of Registrants (Group)'),
      '#size' => 5,
      '#description' => 'Leave blank for single registration.',
      '#default_value' => isset($default_value['Group']) ? $default_value['Group'] : '',
    );
    $form['Notes'] = array(
      '#type' => 'textfield',
      '#title' => t('Notes'),
      '#maxlength' => 255,
      '#size' => 60,
      '#default_value' => isset($default_value['Notes']) ? $default_value['Notes'] : '',
    );
    if (strstr($event_info, 'Waiting List')) {
      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Add Me to the Waiting List',
      );
    }
    else {
      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Sign Up',
      );
    }
  }
  else {
    $form['overview'] = array(
      '#type' => 'markup',
      '#markup' => t('<p>We weren\'t able to find a record of an event with that ID in our system. Please return to the <a href="/events">Events page</a> and find one of our existing events prior to attempting to register.</p>'),
    );
  }

  return $form;
}

/**
 * Validate handler for form ID 'evanced_registration_form'.
 */
function evanced_registration_form_validate($form, &$form_state) {
  if (!empty($form_state['values']['PhoneAC'])) {
    $phone = evanced_registration_simplfy_phone($form_state['values']['PhoneAC']);
    // If we have 12 digits left, it's probably valid.
    if (strlen($phone) != 12) {
      form_set_error('PhoneAC', t('The telephone number appears to be invalid.'));
    }
  }
  if (!empty($form_state['values']['AreaCode2'])) {
    $phone = evanced_registration_simplfy_phone($form_state['values']['AreaCode2']);
    if (strlen($phone) != 12) {
      form_set_error('AreaCode2', t('The second telephone number appears to be invalid.'));
    }
  }
  if (!empty($form_state['values']['Email']) && !valid_email_address($form_state['values']['Email'])) {
    form_set_error('Email', t('The email address appears to be invalid.'));
  }
  if (!empty($form_state['values']['Group']) && !is_numeric($form_state['values']['Group'])) {
    form_set_error('Group', t('The number in your group appears to be invalid.'));
  }
  if (!empty($form_state['values']['Age'])) {
    // Let's check their age against what was allowed.
    $age = $form_state['values']['Age'];
    $age_restriction_start = strtolower($form_state['values']['age_restriction_start']);
    $age_restriction_end = strtolower($form_state['values']['age_restriction_end']);
    // Convert the age restrictions to a specific number of months, so that we can compare apples to apples.
    // They look like this: '2 years', '18 months', '1 month', '2 1/2 years'
    $age_restriction_start = str_replace(' 1/2', '.5', $age_restriction_start);
    $age_restriction_end = str_replace(' 1/2', '.5', $age_restriction_end);
    // They look like this: '2 years', '18 months', '1 month', '2.5 years'
    $age_restriction_start = str_replace(' month', '', str_replace(' months', '', $age_restriction_start));
    $age_restriction_end = str_replace(' month', '', str_replace(' months', '', $age_restriction_end));
    // They look like this: '2 years', '18', '1', '2.5 years'
    if (strstr($age_restriction_start, 'years')) {
      $age_restriction_start = str_replace('+', '', $age_restriction_start);
      $age_restriction_start = $age_restriction_start * 12;
    }
    if (strstr($age_restriction_end, 'years')) {
      $age_restriction_end = str_replace('+', '', $age_restriction_end);
      $age_restriction_end = $age_restriction_end * 12;
    }
    // They look like this: '24', '18', '1', '30'
    if ($age < $age_restriction_start || $age > $age_restriction_end) {
      form_set_error('Age', t('Your age does not meet the age requirement to register for this event.'));
    }    
  }
}

/**
 * Submit handler for form ID 'evanced_registration_form'.
 */
function evanced_registration_form_submit($form, &$form_state) {
  // Redirect to a thank-you page after verifying that the information was successfully posted.

  // Post to Evanced using CURL.

  // Set POST variables
  $url = EVANCED_HOST . 'eventsignup.asp';

  // PhoneAC should be broken up from xxx-xxx-xxxx into PhoneAC, Phone1, and Phone2. 3 separate post fields on the receiving end.
  $phone_parts = explode('-', evanced_registration_simplfy_phone($form_state['values']['PhoneAC']));
  // AreaCode2 too. Becomes AreaCode2, PhoneFirst2, and PhoneSecond2.
  $phone_parts_2 = explode('-', evanced_registration_simplfy_phone($form_state['values']['AreaCode2']));
  $fields = array(
    'EventID' => urlencode($form_state['values']['EventID']),
    'AgeCheck' => urlencode($form_state['values']['AgeCheck']),
    'GradeCheck' => urlencode($form_state['values']['GradeCheck']),
    'BirthDateCheck' => urlencode($form_state['values']['BirthDateCheck']),
    'FormAction' => urlencode($form_state['values']['FormAction']),
    'First' => urlencode($form_state['values']['First']),
    'Last' => urlencode($form_state['values']['Last']),
    'PhoneAC' => urlencode($phone_parts[0]),
    'Phone1' => urlencode($phone_parts[1]),
    'Phone2' => urlencode($phone_parts[2]),
    'PhoneExt' => urlencode($form_state['values']['PhoneExt']),
    'AreadCode2' => urlencode($phone_parts_2[0]),
    'PhoneFirst2' => urlencode($phone_parts_2[1]),
    'PhoneSecond2' => urlencode($phone_parts_2[2]),
    'Phone2Ext' => urlencode($form_state['values']['Phone2Ext']),
    'Email' => urlencode($form_state['values']['Email']),
    'Group' => urlencode($form_state['values']['Group']),
    'Notes' => urlencode($form_state['values']['Notes'])
  );
  // Add custom stipulation fields if necessary.
  for ($x=1; $x < 6; $x++) {
    if (!empty($form_state['values']['CustomStip' . $x])) {
      $fields['CustomStip' . $x] = urlencode($form_state['values']['CustomStip' . $x]);
    }
  }
  // Add age if necessary.
  if (!empty($form_state['values']['Age'])) {
    $fields['Age'] = urlencode($form_state['values']['Age']);
  }

  //url-ify the data for the POST
  foreach($fields as $key=>$value) { $fields_string .= $key.'='.$value.'&'; }
  $fields_string = rtrim($fields_string, '&');

  $options = array(
    'method' => 'POST',
    'data' => $fields_string,
    'timeout' => 30,
    'headers' => array('Content-Type' => 'application/x-www-form-urlencoded'),
    'max_redirects' => 5,
  );

  $result = drupal_http_request($url, $options);

  // Based on the result from the post, we need to show a confirmation page, etc.
  // The response page should contain a JS alert that says "A confirmation email has been sent to your email address."
  if ($result->code == '200' && strstr($result->data, "A confirmation email has been sent to your email address.") != FALSE) {
    // Redirect to a thank-you page.
    if ($form_state['values']['submit'] == 'Add Me to the Waiting List') {
      $form_state['redirect'] = array('evanced-registration/confirmation-waiting');
    }
    else {
      $form_state['redirect'] = array('evanced-registration/confirmation');
    }
  }
  else {
    if (EVANCED_ASSISTANCE_URL != '') {
      drupal_set_message('An error occurred with your registration. This type of error might occur if you\'re trying to re-register for an event that you\'re already registered for. If the problem continues to occur, please <a href="' . EVANCED_ASSISTANCE_URL . '">contact the staff at this library for assistance</a>.', 'error');
    }
    else {
      drupal_set_message('An error occurred with your registration. This type of error might occur if you\'re trying to re-register for an event that you\'re already registered for. If the problem continues to occur, please contact the staff at this library for assistance.', 'error');
    }
  }

}

/**
 * Page callback from hook_menu.
 */
function evanced_registration_confirmation() {
  return '<p>' . t('Your registration for the event has been confirmed. Thank you for your participation in @sitename events. Check your inbox for more details, or to change or cancel your reservation. Or you may call the location at which you\'re registering.', array('@sitename' => variable_get('site_name'))) . '</p>';
}

/**
 * Page callback from hook_menu.
 */
function evanced_registration_confirmation_waiting() {
  return '<p>' . t('Because the event is currently full, you\'ve been added to the waiting list for the event. If one of the attendees cancels, you\'ll automatically be moved into their spot to attend and you\'ll receive an email in your inbox. Thank you for your participation in @sitename events.', array('@sitename' => variable_get('site_name'))) . '</p>';
}

/**
 * Menu callback function to produce the main form.
 */
function evanced_cancellation_form($form, &$form_state, $confirmation_number) {
  // Need to pull in data from Evanced in order to get the $evanced_id. Or else we don't know what it is.
  $url = EVANCED_HOST . 'regcancel.asp?cn=' . check_plain($confirmation_number);
  $result = drupal_http_request($url);
  $data = trim(preg_replace('/\s+/', ' ', $result->data));
  preg_match('/name="EventID" type="hidden" value="([0-9]+)"/U', $data, $matches); // U makes it an ungreedy match.
  $evanced_id = $matches[1];
  preg_match('/name=ID value="([0-9]+)"/U', $data, $matches2); // U makes it an ungreedy match.
  $registration_id = $matches2[1];
  preg_match('/<form.*>(.*)<input/U', $data, $matches3); // U makes it an ungreedy match.
  $notice = trim(str_replace('<br>', '', $matches3[1]));

  if (empty($evanced_id) || empty($registration_id)) { // We couldn't pull the event id from Evanced.
    if (EVANCED_ASSISTANCE_URL != '') {
      drupal_set_message('An error occurred with canceling your registration. If the problem continues to occur, please <a href="' . EVANCED_ASSISTANCE_URL . '">contact the staff at this library for assistance</a>.', 'error');
    }
    else {
      drupal_set_message('An error occurred with canceling your registration. If the problem continues to occur, please contact the staff at this library for assistance.', 'error');
    }
  }
  else { // We have our info and are ready for them to cancel.
    $form['EventID'] = array(
      '#type' => 'hidden',
      '#value' => $evanced_id,
    );
    $form['ID'] = array(
      '#type' => 'hidden',
      '#value' => $registration_id,
    );
    $form['AttendanceLinkID'] = array(
      '#type' => 'hidden',
      '#value' => '0',
    );
    $form['LangType'] = array(
      '#type' => 'hidden',
      '#value' => '0',
    );
    $form['FormAction'] = array(
      '#type' => 'hidden',
      '#value' => 'Link',
    );
    if (!empty($notice)) {
      $form['notice'] = array(
        '#markup' => '<p>' . t($notice) . '</p>',
      );
    }
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Cancel registration'),
    );
  }

  return $form;
}

/**
 * Submit handler for form ID 'evanced_cancellation_form'.
 */
function evanced_cancellation_form_submit($form, &$form_state) {

  // Set POST variables
  $url = EVANCED_HOST . 'regcancel.asp';

  $fields = array(
    'EventID' => urlencode($form_state['values']['EventID']),
    'ID' => urlencode($form_state['values']['ID']),
    'AttendanceLinkID' => urlencode($form_state['values']['AttendanceLinkID']),
    'LangType' => urlencode($form_state['values']['LangType']),
    'FormAction' => urlencode($form_state['values']['FormAction']),
  );

  foreach($fields as $key=>$value) { $fields_string .= $key.'='.$value.'&'; }
  $fields_string = rtrim($fields_string, '&');

  $options = array(
    'method' => 'POST',
    'data' => $fields_string,
    'timeout' => 30,
    'headers' => array('Content-Type' => 'application/x-www-form-urlencoded'),
    'max_redirects' => 5,
  );

  $result = drupal_http_request($url, $options);

  // Based on the result from the post, we need to show a confirmation page, etc.
  // The response page should contain a JS alert that says "A confirmation email has been sent to your email address."
  if ($result->code == '200' && (strstr($result->data, "A confirmation email has been sent to your email address.") != FALSE || strstr($result->data, "document.forms[0].submit();") != FALSE)) {
    // Redirect to a thank-you page.
    $form_state['redirect'] = array('evanced-registration/cancellation');
  }
  else {
    if (EVANCED_ASSISTANCE_URL != '') {
      drupal_set_message('An error occurred with canceling your registration. If the problem continues to occur, please <a href="' . EVANCED_ASSISTANCE_URL . '">contact the staff at this library for assistance</a>.', 'error');
    }
    else {
      drupal_set_message('An error occurred with canceling your registration. If the problem continues to occur, please contact the staff at this library for assistance.', 'error');
    }
  }

}

/**
 * Page callback from hook_menu.
 */
function evanced_registration_cancellation() {
  return '<p>' . t('Your registration for the event has been successfully cancelled. Thank you for your participation in @sitename events.', array('@sitename' => variable_get('site_name'))) . '</p>';
}
