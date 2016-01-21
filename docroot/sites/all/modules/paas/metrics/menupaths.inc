<?php

function paas_metric_menupaths_call($value) {
  header('Content-Type: text/plain');
  print 'path,access_callback';
  
  $items = module_invoke($value, 'menu');
  if (!is_array($items)) {
    return;
  }
  
  foreach ($items as $path => $item) {
    $item += array('access callback' => 'user_access');
    echo "\n$path,{$item['access callback']}";
  }
}
