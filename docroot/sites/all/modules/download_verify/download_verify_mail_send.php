<?php
/**
 * @file
 * Mail send procedures.
 */

// Get the server root path.
$path = $_SERVER['DOCUMENT_ROOT'];
chdir($path);
define('DRUPAL_ROOT', getcwd());
require_once './includes/bootstrap.inc';
// Load up Drupal.
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
$download_verify_mail_token = $_GET['dvmailtoken'];
$seed = 'downloadverifymoduleseed';
// Check for valid token to avid CSRF attacks.
$token_is_valid = drupal_valid_token($download_verify_mail_token, $seed, TRUE);
if($token_is_valid && (strlen($download_verify_mail_token) == 43)) {
  // Get the POSTed variables.
  // Sanitize all user submitted variables in case of spoofed request.
  $download_verify_sendto = $_GET['dvsendto'];
  $download_verify_fname = check_plain($_GET['dvfname']);
  $download_verify_sname = check_plain($_GET['dvsname']);
  $download_verify_email = check_plain($_GET['dvemail']);

  $module = 'download_verify';
  $key = 'download_verify_submission';
  $language = language_default();
  // Pass the message details into an array.
  $params = array(
    'firstname' => $download_verify_fname,
    'surname' => $download_verify_sname,
    'email' => $download_verify_email,
  );
  $from = $download_verify_email;
  $send = TRUE;
  // Send it.
  $download_verify_mailsend = drupal_mail($module, $key, $download_verify_sendto, $language, $params, $from, $send);

  if ($download_verify_mailsend['result'] == TRUE) {
    $download_verify_mail_result = 'Mail sent';
    // drupal_set_message(t('Your message has been sent.'));
  }
  else {
    $download_verify_mail_result = 'Mail not sent';
    // drupal_set_message(t('There was a problem. Your message was not sent.'), 'error');
  }
  return $download_verify_mail_result;
}
else {
  // Invalid token.
  drupal_goto('<front>');
}
