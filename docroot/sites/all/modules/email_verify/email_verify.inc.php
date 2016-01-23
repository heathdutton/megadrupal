<?php
/**
 * @file
 * Checks the email address for validity.
 */

/**
 * Checks the email address for validity.
 */
function _email_verify_check($mail) {
  if (!valid_email_address($mail)) {
    // The address is syntactically incorrect.
    // The problem will be caught by the 'user' module anyway, so we avoid
    // duplicating the error reporting here, just return.
    return;
  }

  module_load_include('inc', 'email_verify', 'windows_compat');

  $host = drupal_substr(strchr($mail, '@'), 1);

  // Let's see if we can find anything about this host in the DNS.
  if (!checkdnsrr($host . '.', 'ANY')) {
    return t('%host is not a valid email host. Please check the spelling and try again.', array('%host' => "$host"));
  }

  // If install found port 25 closed or fsockopen() disabled, we can't test
  // mailboxes.
  If (variable_get('email_verify_skip_mailbox', FALSE)) {
    return;
  }

  // What SMTP servers should we contact?
  $mx_hosts = array();
  if (!getmxrr($host, $mx_hosts)) {
    // When there is no MX record, the host itself should be used.
    $mx_hosts[] = $host;
  }

  // Try to connect to one SMTP server.
  foreach ($mx_hosts as $smtp) {

    $connect = @fsockopen($smtp, 25, $errno, $errstr, 15);

    if (!$connect) {
      continue;
    }

    if (preg_match("/^220/", $out = fgets($connect, 1024))) {
      // An SMTP connection was made.
      break;
    }
    else {
      // The SMTP server probably does not like us (dynamic/residential IP for
      // aol.com for instance).
      // Be on the safe side and accept the address, at least it has a valid
      // domain part.
      watchdog('email_verify', 'Could not verify email address at host @host: @out', array('@host' => $host, '@out' => $out));
      return;
    }
  }

  if (!$connect) {
    return t('%host is not a valid email host. Please check the spelling and try again or contact us for clarification.', array('%host' => "$host"));
  }

  $from = variable_get('site_mail', ini_get('sendmail_from'));

  // Extract the <...> part, if there is one.
  if (preg_match('/\<(.*)\>/', $from, $match) > 0) {
    $from = $match[1];
  }

  // Should be good enough for RFC compliant SMTP servers.
  $localhost = $_SERVER["HTTP_HOST"];
  if (!$localhost) {
    $localhost = 'localhost';
  }

  fputs($connect, "HELO $localhost\r\n");
  $out = fgets($connect, 1024);
  fputs($connect, "MAIL FROM: <$from>\r\n");
  $from = fgets($connect, 1024);
  fputs($connect, "RCPT TO: <{$mail}>\r\n");
  $to = fgets($connect, 1024);
  fputs($connect, "QUIT\r\n");
  fclose($connect);

  if (!preg_match("/^250/", $from)) {
    // Again, something went wrong before we could really test the address.
    // Be on the safe side and accept it.
    watchdog('email_verify', 'Could not verify email address at host @host: @from', array('@host' => $host, '@from' => $from));
    return;
  }

  if (
      // This server does not like us (noos.fr behaves like this for instance).
      preg_match("/(Client host|Helo command) rejected/", $to) ||
      // Any 4xx error also means we couldn't really check except 450, which is
      // explcitely a non-existing mailbox: 450 = "Requested mail action not
      // taken: mailbox unavailable".
      preg_match("/^4/", $to) && !preg_match("/^450/", $to)) {
    // In those cases, accept the email, but log a warning.
    watchdog('email_verify', 'Could not verify email address at host @host: @to', array('@host' => $host, '@to' => $to));
    return;
  }

  if (!preg_match("/^250/", $to)) {
    watchdog('email_verify', 'Rejected email address: @mail. Reason: @to', array('@mail' => $mail, '@to' => $to));
    return t('%mail is not a valid email address. Please check the spelling and try again or contact us for clarification.', array('%mail' => "$mail"));
  }

  // Everything is OK, so don't return anything.
  return;
}
