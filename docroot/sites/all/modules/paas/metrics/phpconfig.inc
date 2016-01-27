<?php

function paas_metric_phpconfig_call($value) {
  switch ($value) {
    case 'ini':
      $name = $_GET['name'];
      $value = ini_get($name);
      if (is_null($value) || $value === FALSE) {
        echo '<null>';
        return;
      }
      drupal_add_http_header('Content-Type', 'text/plain');
      echo $value;
      break;
  }
}
