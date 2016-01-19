<?php


/**
 * @file polaris_auto_register_validate_email.inc
 * #1: See if all three of these items match: birthdate, last name, email
 * #2: See if all three of these items match: birthdate, first name, telephone number
 */
function check_email() {
  $email = check_plain($_POST['email']);
  $fname_original = check_plain($_POST['fname']);
  $fname = drupal_strtolower($fname_original);
  $mname = drupal_strtolower(check_plain($_POST['mname']));
  $lname = drupal_strtolower(check_plain($_POST['lname']));
  $phone = check_plain($_POST['phone']);
  // Empty out the value if no dob was actually passed.
  $dob_original = check_plain(str_replace('undefined-undefined-undefined', '', $_POST['dob']));
  $dob = $dob_original;

  $new_dob = explode('-', $dob);

  // make sure month and day are two digits
  $m = str_pad($new_dob[0], 2, '0', STR_PAD_LEFT);
  $d = str_pad($new_dob[1], 2, '0', STR_PAD_LEFT);
  $dob = $m . '-' . $d . '-' . $new_dob[2];

  // send email to Polaris and get the result
  $result = PolarisAPIAutoRegister::patronSearchEmail($email);

  $final = 1; // Default to telling JS that no matching record was found. Could change below...
  if ($result->TotalRecordsFound >= 1) {
    // the email was found, get the name from the results
    $name = $result->PatronSearchRows[0]->PatronFirstLastName;
    // get rid of the comma in the name
    $name = str_replace(',', '', $name);
    $names = explode(' ', $name);
    $lname2 = drupal_strtolower($names[0]);
    $polaris_name = $names[1] . ' ' . $names[0];

    // use barcode to get Patron data
    $barcode = $result->PatronSearchRows[0]->Barcode;

    // retrieve phone, birthdate
    $result2 = PolarisAPIAutoRegister::patronBasicDataGet($barcode);
    $dob2 = $result2->PatronBasicData->BirthDate;

    $dob3 = extract_polaris_timestamp($dob2);

    // #1 - we already know emails match; compare the last names and date of birth (or just the last name if the dob wasn't posted [from lost lib card version of form])
    if ($lname == $lname2 && ($dob == $dob3 || empty($dob_original))) {
      // it's the same person
      $final = array(
        'return' => 0,
        'barcode' => $barcode,
        'email' => $email,
        'polaris_name' => $polaris_name,
      );
    }
  }
  if ($result->TotalRecordsFound == 0 || $final == 1) { // #2 There was no match found in Polaris on the email address. Let's check their telephone number, first name, and date of birth
    $result = PolarisAPIAutoRegister::patronSearchBirthDateFirstNameAndPhone($new_dob[2], $fname_original, $phone);
    if ($result->TotalRecordsFound >= 1) {
      foreach ($result->PatronSearchRows as $patron_row) {
        if ($final == 1 || empty($final)) {
          // the email was found, get the name from the results
          $name = $patron_row->PatronFirstLastName;
          // get rid of the comma in the name
          $name = str_replace('  ', ' ', str_replace(',', '', $name));
          $names = explode(' ', $name);
          $lname2 = drupal_strtolower($names[0]);
          $polaris_name = $names[1] . ' ' . $names[0];

          // use barcode to get Patron data
          $barcode = $patron_row->Barcode;

          // retrieve phone, birthdate
          $result2 = PolarisAPIAutoRegister::patronBasicDataGet($barcode);
          $dob2 = $result2->PatronBasicData->BirthDate;

          $dob3 = extract_polaris_timestamp($dob2);
          $phone2 = $result2->PatronBasicData->PhoneNumber;
          $email2 = $result2->PatronBasicData->EmailAddress;

          if ($phone == $phone2 && $dob == $dob3) {
            // it's the same person
            $final = array(
              'return' => 0,
              'barcode' => $barcode,
              'email' => $email2,
              'polaris_name' => $polaris_name,
            );
          }
        }
      }
    }
  }

  // encode response
  drupal_json_output($final);

} // end of validate_email()

/**
 * Helper function to get human readable Y-m-d date from polaris timestamps
 * @param unknown $date
 */
function extract_polaris_timestamp($dob) {
  $match = preg_match('/\/Date\((-*)(\d+)(-\d+)\)\//', $dob, $date);

  $timestamp = $date[1] .$date[2]/1000;
  $timezone = $date[3]; // Not really necessary to know.
  $date = date('m-d-Y', $timestamp);
  // Prior to 1971. It's an API goof. Add on time to it.
  // See: http://developer.polarislibrary.com/forum/forum.aspx?g=posts&m=322&#post322
  if ($timestamp == '-68400') {
    $date = date('m-d-Y'); // Set it to current date.
  }
  else if ($timestamp < '31597189') {
    $future_date = strtotime('now') + $timestamp;
    $date = date('m-d-Y', $future_date);
  }

  return $date;
}

function send_email() {
  $email = check_plain($_POST['x_email']);
  $name = ucfirst(check_plain($_POST['f_name']));
  $barcode = check_plain($_POST['x_barcode']);

  $path_phpmailer = libraries_get_path('phpmailer');
  include_once($path_phpmailer . '/class.phpmailer.php');

  $mail = new PHPMailer();
  $mail->Subject = ' Your Richland Library Card Number';
  $mail->SetFrom(variable_get('polaris_auto_register_email'), variable_get('site_name'));

  $body = '<p>Dear ' . $name . ',</p>';
  $body .= '<p>This e-mail contains the library card number that you requested.'
           . ' If you did not request this, please contact us at '
           . '<a href="http://www.richlandlibrary.com/ask-us">Ask Us</a> and let us know.</p>';
  $body .= '<p>To complete the account recovery process please return to '
           . 'RichlandLibrary.com and log in.  Here is your login information:</p>';
  $body .= '<p>Card Number: ' . $barcode . '</p>';
  $body .= '<p><h2>Forgot Your PIN Too?</h2></p>';
  $body .= '<p>If you have forgotten the PIN number for logging into your account ';
  $body .= 'you can retrieve it <a href="http://catalog.richlandlibrary.com/polaris/logon.aspx\?forgotPassword=1">here</a>.</p>';


  $mail->AddAddress($email, $name);

  $mail->MsgHTML($body);

  if (!$mail->Send()) {
    $final = 0;
  }
  else {
    $final = 1;
  }


  // encode response
  drupal_json_output($final);
}

/**
 * AJAX callback function.
 */
function telephone_lookup() {
  $phone = check_plain($_POST['phone']);

  // Send telephone to Polaris and get the result
  $result = PolarisAPIAutoRegister::patronSearchPhone($phone);

  $final = 1; // Default to telling JS that no matching record was found. Could change below...
  if ($result->TotalRecordsFound >= 1) {
    // The phone number was found, get the name from the results.
    $name = $result->PatronSearchRows[0]->PatronFirstLastName;
    $barcode = $result->PatronSearchRows[0]->Barcode;

    // it's the same person
    $final = array(
      'return' => 0,
      'barcode' => $barcode,
      'name' => $name,
      'phone' => $phone,
    );
  }

  // encode response
  drupal_json_output($final);

} // end of telephone_lookup()
