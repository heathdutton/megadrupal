<?php

function pmpapi_update_process_notification() {
  // An update/delete
  $body = file_get_contents("php://input");
  $secret = variable_get('pmpapi_update_secret');
  $hash = hash_hmac('sha1', $body, $secret);
  if ($_SERVER['REQUEST_METHOD'] == 'POST'
    && $_SERVER['CONTENT_TYPE'] == 'application/rss+xml'
    && $_SERVER['HTTP_X_HUB_SIGNATURE'] == 'sha1=' . $hash) {

    $xml = simplexml_load_string($body);
    if (!empty($xml->channel->item)) {
      $op = (string) $xml->channel->link;
      foreach($xml->channel->item as $item) {
        module_invoke_all('pmpapi_update_incoming_item', $item, $op);
        if (!empty($item->guid)) {
          $guid = (string) $item->guid;
          if (pmpapi_pull_doc_has_been_pulled($guid) && variable_get('pmpapi_update_updates_active')) {
            // UPDATE
            if ($op == pmpapi_update_get_topic_uri('updated')) {
              pmpapi_pull_pull_doc($guid);
              watchdog('pmpapi_update_update', t('Update request for GUID: %guid', array('%guid' => $guid)));
            }

            // DELETE
            if ($op == pmpapi_update_get_topic_uri('deleted')) {
              $query = db_query('SELECT entity_type, entity_id FROM {pmpapi_local_docs} WHERE guid=:guid', array(':guid' => $guid))->fetchAssoc();
              entity_delete($query['entity_type'], $query['entity_id']);
              watchdog('pmpapi_update_delete', t('Delete request for GUID: %guid', array('%guid' => $guid)));
            }
          }
        }
      }
      print 'OK';
      drupal_exit();
    }
  }

  // A subscribe verification
  if ($_SERVER['REQUEST_METHOD'] == 'GET') {
    $pending = variable_get('pmpapi_update_subscribe_pending');
    if (!empty($pending)) {
      $request = array();
      $request['verify_token'] = (isset($_GET['hub_verify_token'])) ? $_GET['hub_verify_token'] : NULL;
      $request['mode'] = (isset($_GET['hub_mode'])) ? $_GET['hub_mode'] : NULL;
      $request['topic_uri'] = (isset($_GET['hub_topic'])) ? $_GET['hub_topic'] : NULL;
      if ($pending == $request && isset($_GET['hub_challenge'])) {
        print $_GET['hub_challenge'];
        drupal_exit();
      }
    }
  }

  // If we haven't already printed and exited, return a 403. It's not a perfect
  // status code, but it works.
  drupal_access_denied();
  drupal_exit();
}