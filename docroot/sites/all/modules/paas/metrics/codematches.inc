<?php

function paas_metric_codematches_call($value) {
  header('Content-Type: text/plain');
  if (empty($_GET['pattern'])) {
    print 'Missing pattern';
    return;
  }
  $pattern = '/' . $_GET['pattern'] . '/i';
  $sql = 'SELECT filename, type FROM {system} WHERE type = \'module\' AND name = :value';
  $res = db_query($sql, array(':value' => $value));
  if ($item = $res->fetchObject()) {
    $contents = file_get_contents($item->filename);
    preg_match_all($pattern, $contents, $data);
    $matches = array();
    foreach ($data[0] as $match) {
      if (!in_array($match, $matches)) {
        $matches[] = $match;
      }
    }
    print count($data[0]) . ',' . count($matches);
  }
}
