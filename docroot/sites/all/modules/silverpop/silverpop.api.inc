<?php

/**
 * @file
 * API file for the silverpop module.
 */

require_once dirname(__FILE__) . '/lib/Engage.php';

/**
 * Silverpop add recipient.
 *
 * @param integer $db_id
 *   Database ID.
 * @param integer|NULL $list_id
 *   Contact List ID. If NULL recipient will not be added to a contact list.
 * @param array $recipient
 *   Key, value pairs for database columns.
 * @throws Exception
 */
function silverpop_addrecipient($db_id, $list_id, $recipient) {

  $api_host = variable_get('silverpop_apihost', '');
  $username = variable_get('silverpop_username', '');
  $password = silverpop_get_password();

  // Check if Silverpop is properly configured.
  if (!$api_host OR !$username OR !$password) {
    watchdog('silverpop_webform', "Silverpop not configured properly, cannot add recipient.", array(), WATCHDOG_WARNING);
    return;
  }

  $visitor_key = '';

  if (isset($_COOKIE['com_silverpop_iMAWebCookie'])) {
    $visitor_key = $_COOKIE['com_silverpop_iMAWebCookie'];
    watchdog('silverpop_webform', "Visitor key Cookie value $visitor_key found.");
  }
  else {
    watchdog('silverpop_webform', "No visitor key cookie found.");
  }

  try {
    watchdog('silverpop_webform', "Logging into Engage API on {$api_host} as {$username}");

    $engage = new Engage($api_host);
    $engage->login($username, $password);

    $columns = '';

    // Build column XML
    foreach ($recipient as $key => $val) {
      $columns .= '<COLUMN><NAME>' . $key . '</NAME><VALUE>' . $val . '</VALUE></COLUMN>';
    }

    $request = "<AddRecipient> <LIST_ID>$db_id</LIST_ID>" .
      '<CREATED_FROM>1</CREATED_FROM><UPDATE_IF_FOUND>true</UPDATE_IF_FOUND>' .
      "<VISITOR_KEY>$visitor_key</VISITOR_KEY>";

    if (isset($list_id)) {
      $request .= "<CONTACT_LISTS><CONTACT_LIST_ID>$list_id</CONTACT_LIST_ID>" .
        '</CONTACT_LISTS>';
    }
    $request .= $columns .
      '</AddRecipient>';

    try {
      $response = $engage->execute($request);
    }
    catch (Exception $e) {
      throw new Exception('Login failed: ' . $e->getMessage());
    }

    if ($response->RESULT->SUCCESS == "TRUE") {
      $recipient = $response->RESULT->RecipientId;

      watchdog('silverpop_webform', "Added Recipient ID $recipient");
    }
    else {
      watchdog('silverpop_webform', "Failed to add recipient", array(), WATCHDOG_ERROR);
    }
  }
  catch (Exception $e) {
    watchdog('silverpop_webform', "AddRecipient exception " . $e->getMessage(), array(), WATCHDOG_ERROR);
  }
}
