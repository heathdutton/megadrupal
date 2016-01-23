<?php

function contextly_overlay_editor_page($type) {
  print ContextlyDrupalKit::getInstance()
    ->newOverlayDialog($type)
    ->render();
}

function contextly_ajax_editor_request_callback($request, $node) {
  try {
    // Build parameters list.
    $params = $_POST;
    $params['custom_id'] = $node->nid;

    // Handle the request.
    $result = ContextlyDrupalKit::getInstance()
      ->newWidgetsEditor()
      ->handleRequest($request, $params);

    // Deliver results to the client.
    drupal_json_output($result);
  }
  catch (ContextlyKitException $e) {
    _contextly_watchdog_exception($e);
    _contextly_return_internal_error();
  }
}

function _contextly_return_internal_error() {
  drupal_add_http_header('Status', '500 Internal Server Error');
  drupal_exit();
}
