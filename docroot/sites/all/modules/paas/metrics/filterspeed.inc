<?php

function paas_metric_filterspeed_generate_text($seed) {
  $text = '';
  $words = array();
  for ($i = 1; $i <= 100; ++$i) {
    $words[] = substr(md5($i * $seed), 0, (($seed * $i) % 7) + 2);
  }
  for ($line = 1; $line <= 250; ++$line) {
    $text .= '<p>';
    for ($word = 1; $word <= 20; ++$word) {
      if ($word == 1) {
        $text .= ucfirst($words[(count($words) * $line) % $word]);
      }
      else {
        $text .= ' ' . $words[(count($words) * $line) % $word];
      }
      if ($word == 20) {
        $text .= '.';
      }
    }
    $text .= "</p>\n";
  }
  return $text;
}

function paas_metric_filterspeed_call($value) {
  switch ($value) {
    case 'test':
      $formats = array();
      $runs = 10;
      
      cache_clear_all(NULL, 'cache_filter');
      
      $sql = 'SELECT format FROM {filter_format}';
      $res = db_query($sql);
      foreach ($res as $format) {
        $timer = 0;
        for ($i = 1; $i <= $runs; ++$i) {
          $text = paas_metric_filterspeed_generate_text($i);
          $text .= time();
          timer_start('format');
          check_markup($text, $format->format, '', FALSE);
          $timer += timer_read('format');
        }
        $formats[] = $format->format . ',' . round($timer / $runs, 2);
      }
      
      cache_clear_all(NULL, 'cache_filter');
      
      drupal_add_http_header('Content-Type', 'text/plain; charset=utf-8');
      echo "format,timer\n" . implode("\n", $formats);
      exit;
  }
}
