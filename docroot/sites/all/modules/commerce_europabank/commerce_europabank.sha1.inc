<?php

/**
 * @file
 * SHA-1 security hash specific functionality.
 */

/**
 * Validate the feedback with the SHA-1 signature.
 */
function commerce_europabank_feedback_valid_sha1($feedback, $secret) {
  $sha1 = '';
  $sha1 = $feedback['Id'];
  $sha1 .= $feedback['Orderid'];
  $sha1 .= $secret;
  $hash = drupal_strtoupper(sha1($sha1));
  if ($hash === $feedback['Hash']) {
    return TRUE;
  }

  watchdog('europabank', 'hash comparison failed. secret: %secret feedback %feedback generated_hash %generated_hash', array(
      '%secret' => $secret,
      '%feedback' => print_r($feedback, TRUE),
      '%generated_hash' => $hash,
    ), WATCHDOG_ERROR, NULL);

  return FALSE;
}

/**
 * Get a list of valid SHA-1 out parameters.
 */
function commerce_europabank_sha1_out_parameters() {
  return array(
    'Uid',
    'Orderid',
    'Amount',
    'Description',
    'hash',
    'beneficiary',
    'Brand',
    'Recurringfrequency',
    'Recurringexpiry',
    'installmentpayment',
    'Chname',
    'Chlanguage',
    'Chcountry',
    'Chemail',
    'Chip',
    'Css',
    'Template',
    'Title',
    'merchantparam',
    'Merchantemail',
    'Redirecturl',
    'Redirecttype',
    'Feedbackurl',
    'Feedbacktype',
    'Feedbackemail',
  );
}
