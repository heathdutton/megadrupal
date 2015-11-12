<?php

/**
 * Implementation of hook_drush_command().
 */
function bounce_handler_drush_command() {
  return array(
    'bounce' => array(
      'description' => 'Process incoming bounce.',
      'arguments' => array(
        'sender' => 'Sender',
        'recipient' => 'Recipient',
      ),
    ),
  );
}

/**
 * Command callback.
 */
function drush_bounce_handler_bounce() {
  require_once "Mail/mimeDecode.php";
  require_once 'Mail/RFC822.php';
  require_once drupal_get_path('module', 'bounce_handler') .'/phpmailer-bmh_rules.php';

  $incoming_bounce = '';
  $stdin = fopen('php://stdin', 'r');
  while (!feof($stdin)) {
    $incoming_bounce.= fgets($stdin,4096);
  }
  fclose($stdin);

  $params['include_bodies'] = TRUE;
  $params['decode_bodies'] = TRUE;
  $params['decode_headers'] = TRUE;
  $params['input'] = $incoming_bounce;

  $structure = Mail_mimeDecode::decode($params);
  // If we can't parse the message we discard it.
  if (is_object($structure) && isset($structure->headers) && count($structure->headers) > 1) {
    // The To address is the envelop from address of the original mail.
    $to = Mail_RFC822::parseAddressList($structure->headers['to']);
    // de-verpify
    $destination = explode('+', $to[0]->mailbox, 2);
    $destination = str_replace('=', '@', $destination[1]);
    // There's no point in doing anything with invalid mail addresses.
    if (valid_email_address($destination)) {
      if ($structure->ctype_primary == 'multipart' && $structure->ctype_secondary == 'report') {
        $result = bmhDSNRules($structure->parts[0]->body, $structure->parts[1]->body);
      }
      else {
        $result = bmhBodyRules($structure->body, '', TRUE);
      }

      // Find the user
      $uid = (int) db_query("SELECT uid FROM {users} WHERE mail = :mail", array(':mail' => $destination))->fetchField();
      // We don't save bounces for non-existing users. You may want do set
      // the variable for debugging purposes or if you want to keep them anyway.
      if ($uid || variable_get('bounce_handler_keep_anonymous_mails', FALSE)) {
        // Now save the message in the DB, increase the counters
        db_merge('bounce_statistics')
          ->key(array('mail' => $destination))
          ->fields(
            array(
              'timestamp' => REQUEST_TIME,
            ))
          ->expression('count', 'count + :inc1', array(':inc1' => 1))
          ->expression('remove', 'remove + :inc2', array(':inc2' => $result['remove']))
          ->execute();
        $sid = db_query("SELECT sid FROM {bounce_statistics} WHERE mail = :mail", array(':mail' =>$destination))->fetchField();

        db_insert('bounces')
          ->fields(
            array(
              'sid' => $sid,
              'uid' => (isset($uid) ? $uid : 0),
              'timestamp' => REQUEST_TIME,
              'destination' => $destination,
              'bmh_no' => $result['rule_no'],
              'bmh_cat' => $result['rule_cat'],
              'bmh_type' => $result['bounce_type'],
              'severity' => $result['remove'],
              'message' => $incoming_bounce
            )
          )
          ->execute();
      }
    }
  }
}

/**
 * This is a slightly ugly hack in order to by able to use phpmailer-bmh_rules.php unmodified.
 */
if (!function_exists('imap_rfc822_parse_adrlist')) {
  function imap_rfc822_parse_adrlist($address, $default_host) {
    return Mail_RFC822::parseAddressList($address, $default_host);
  }
}
