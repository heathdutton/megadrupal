<?php

/**
 * Collects report information from the client and saves it to the database.
 */
function tracekit_collect() {
  global $user;

  $data = array(
    'uid' => $user->uid,
    'created' => time(),
    'message' => '-none-',
    'mode' => '-none-',
    'name' => '-none-',
    'url' => '-none-',
    'useragent' => '-none-',
    'stack' => '-none-'
  );

  if (isset($_POST['message'])) {
    $data['message'] = substr($_POST['message'], 0, 255);
  }

  if (isset($_POST['mode'])) {
    $data['mode'] = substr($_POST['mode'], 0, 255);
  }

  if (isset($_POST['name'])) {
    $data['name'] = substr($_POST['name'], 0, 255);
  }

  if (isset($_POST['url'])) {
    $data['url'] = substr($_POST['url'], 0, 255);
  }

  if (isset($_POST['useragent'])) {
    $data['useragent'] = substr($_POST['useragent'], 0, 255);
  }

  if (isset($_POST['stack'])) {
    $data['stack'] = $_POST['stack'];
  }

  $result = drupal_write_record('tracekit_report', $data);
  $tracekit_report_id = NULL;
  if (isset($data['tracekit_report_id'])) {
    $tracekit_report_id = $data['tracekit_report_id'];
  }

  $response = array(
    'success' => !!$result,
    'tracekit_report_id' => $tracekit_report_id
  );

  return drupal_json_output($response);
}
