<?php

function paas_metric_cdn_call($value) {
  switch ($value) {
    case 'pickserver':
      if (function_exists('cdn_pick_server') || variable_get('CDN_PICK_SERVER_PHP_CODE_VARIABLE', '')) {
        die('yes');
      }
      else {
        die('no');
      }
      break;
  }
}
