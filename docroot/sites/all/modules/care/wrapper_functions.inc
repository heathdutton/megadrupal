<?php

/**
 * @file
 * Various useful functions that wrap complex CARE calls where only one or two
 * variable arguments are needed.
 */

/**
 * Check whether given ContactNumber has an existing positive Gift Aid
 * declaration, for both membership and donations, or not.
 * If there is an existing declaration, return the CARE XML data object.
 */
function care_giftaid_existing($contact_number) {
  if ($contact_number) {
    $data = array(
      'ContactNumber' => $contact_number);
    $typedata = array(
      'pSelectDataType' => 'xcdtContactGiftAidDeclarations');
    $care_xml_data = care_call_method('SelectContactData', $data, $typedata);
    if (!isset($care_xml_data->ErrorMessage)) {
      foreach ($care_xml_data as $declaration_data) {
        $not_cancelled = (string) $declaration_data->CancelledOn == '';
        $no_enddate = (string) $declaration_data->EndDate == '';
        $future_enddate = (DateTime::createFromFormat('d/m/Y', (string) $declaration_data->EndDate) > new DateTime('today'));
        $current = ($no_enddate or $future_enddate);
        $donations = (string) $declaration_data->Donations == 'Y';
        $members = (string) $declaration_data->Members == 'Y';
        if ($not_cancelled and $current and $donations and $members) {
          return $declaration_data;
        }
      }
    }
  }
  return FALSE;
}

/**
 * Add new Gift Aid declaration record for the given ContactNumber, either a
 * positive declaration or a note that the declaration was declined.
 */
function care_giftaid_declare($contact_number, $declaration = TRUE) {
  // Calculate start date of current tax year.
  $aprilsixlast = new DateTime('6 Apr');
  $today = new DateTime('today');
  if ($aprilsixlast > $today) {
    $aprilsixlast->modify('-1 year');
  }
  $start_date = $aprilsixlast;
  // Check all previous declarations.
  $data = array(
    'ContactNumber' => $contact_number);
  $typedata = array(
    'pSelectDataType' => 'xcdtContactGiftAidDeclarations');
  $care_xml_data = care_call_method('SelectContactData', $data, $typedata);
  foreach ($care_xml_data as $declaration_data) {
    if ((string) $declaration_data->EndDate == '') {
      // Cancel existing declaration.
      $yesterday = new DateTime('yesterday');
      $data = array(
        'UserID' => $contact_number,
        'DeclarationNumber' => (string) $declaration_data->DeclarationNumber,
        'CancelledOn' => $yesterday->format('d/m/Y'),
        'CancellationReason' => 'F',
        'Source' => 'WEB',
        'DoCancel' => 'Y');
      $care_result_xml = care_call_method('CancelGiftAidDeclaration', $data);
      $start_date = new DateTime('today');
    }
    else {
      // Check cancelled declaration end date.
      $declaration_end = DateTime::createFromFormat('d/m/Y', (string) $declaration_data->EndDate);
      if ($declaration_end >= $aprilsixlast) {
        // If cancelled this tax year, set start date to today.
        $start_date = new DateTime('today');
      }
    }
  }
  if ($declaration) {
    // Add new declaration for both Donations and Membership fees.
    $data = array(
      'ContactNumber' => $contact_number,
      'StartDate' => $start_date->format('d/m/Y'),
      'ConfirmedOn' => date('d/m/Y'),
      'DeclarationMethod' => 'W',
      'Donations' => 'Y',
      'Members' => 'Y',
      'Source' => 'WEB');
    $resultxml = care_call_method('AddGiftAidDeclaration', $data);
  }
  else {
    // Add activity noting non-declaration.
    $data = array(
      'ContactNumber' => $contact_number,
      'ActivityDate' => date('d/m/Y'),
      'ValidFrom' => date('d/m/Y'),
      'ValidTo' => date('d/m/Y', time() + 60 * 60 * 24),
      'Activity' => 'F',
      'ActivityValue' => 'NGA',
      'Quantity' => 1,
      'Source' => 'WEB');
    $resultxml = care_call_method('AddActivity', $data);
  }
  return $resultxml;
}
