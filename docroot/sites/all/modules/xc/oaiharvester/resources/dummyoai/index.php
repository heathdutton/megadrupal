<?php
header('Content-Type: text/xml;charset=UTF-8');

$SESSION_STORAGE = './storage/actual.txt';
if (!isset($_GET['verb'])) {
  if (isset($_GET['cmd'])) {
    $cmd = $_GET['cmd'];
    switch ($cmd) {
      case 'set':
        $value = $_GET['value'];
        file_put_contents($SESSION_STORAGE, $value);
        print '<success/>';
        break;

      case 'get':
        print file_get_contents($SESSION_STORAGE);
        break;

      default:
        print '<failure/>';
        break;
    }
  }
  else {
    print '<failure/>';
  }
  return;
}
$verb = $_GET['verb'];

switch($verb) {
  case 'Identify' :
    readfile('responses/standard/identify.xml');
    break;

  case 'ListSets' :
    readfile('responses/standard/list_sets.xml');
    break;

  case 'ListMetadataFormats' :
    readfile('responses/standard/list_metadata_formats.xml');
    break;

  case 'ListRecords' :
    if (isset($_GET['set'])) {
      $set = $_GET['set'];
    }
    elseif (file_exists($SESSION_STORAGE)) {
      $set = file_get_contents($SESSION_STORAGE);
    }
    else {
      $set = 'good';
    }

    if ($set == 'good') {
      readfile('responses/list_records_good.xml');
    }
    else if ($set == 'deleted') {
      readfile('responses/list_records_deleted.xml');
    }
    else if ($set == '10rec') {
      readfile('responses/list_records_10rec-step2.xml');
    }
    else if ($set == '10rec2') {
      readfile('responses/10-test-phase2-02.xml');
    }
    else if ($set == 'accents') {
      readfile('responses/accents.xml');
    }
    break;
}
