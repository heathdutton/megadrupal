<?php

function tropo_rules_rules_action_info() {
  return array(
    'tropo_rules_sms' => array(
      'label' => t('Send an SMS message via Tropo'),
      'parameter' => array(
        'node' => array('type' => 'node', 'label' => t('Node')),
      ),
      'group' => t('Tropo API'),
      'access callback' => TRUE,
    ),
  );
}

/**
 * Send the SMS message
 */
function tropo_rules_sms($object, $title, $settings) {
  $data = array(
    'action' => 'create',
    'token' => variable_get('tropo_rules_api_messaging_token', ''),
    'number' => '',
    'message' => 'This is a test.',
  );

  // Cycle through each recipient and send the message.
  foreach ($recipients as $uid => $rec) {
    // Set the recipient
    $data['number'] = $rec;

    // Set the message.
    $body = field_get_items('node', $node, 'body');

    $summary = $body[0]['summary'];
    if (!$summary) {
      $summary =  substr(strip_tags($body[0]['value']), 0, 120);
    }

    $data['message'] = urlencode($summary . ' ' . t("Reply 'help' for assistance."));

    //dsm($data);

    // Build the query string
    $string = array();
    foreach ($data as $key => $item) {
      $string[] = "$key=$item";
    }
    $url = 'http://api.tropo.com/1.0/sessions?' .implode('&', $string);

    // Send the request to Tropo
    $res = drupal_http_request($url);

    //dsm($res);

    // Parse the result data.
    $xml = simplexml_load_string($res->data);
    $json = json_encode($xml);
    $status = json_decode($json,TRUE);

    // @todo Check for success vs failure
    if ($status['success'] == 'true') {
      watchdog('tropo_rules', 'SMS message sent to %number via Tropo', array('%number' => $rec), WATCHDOG_NOTICE);
    }
    else {
      //$links = l('User', user_load($uid));
      watchdog('tropo_rules', 'Failed to send SMS message sent to %number via Tropo. Error %error', array('%number' => $rec), WATCHDOG_ERROR);
    }
  }
}
