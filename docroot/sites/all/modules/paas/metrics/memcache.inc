<?php

function paas_metric_memcache_bin_mapping($bin = 'cache') {
  $bins = array_flip(variable_get('memcache_bins', array('cache' => 'default')));
  if (isset($bins[$bin])) {
    return $bins[$bin];
  }
  else {
    // The default bin is 'cache'.
    return _memcache_admin_default_bin($bin);
  }
}

function paas_metric_memcache_get_info() {
  $output = array(
    'cache_inc' => variable_get('cache_inc', ''),
    'bins' => array(),
  );
  if (function_exists('dmemcache_stats')) {
    $memcache_servers = variable_get('memcache_servers', array('127.0.0.1:11211' => 'default'));
    foreach ($memcache_servers as $server => $b) {
      $bin = paas_metric_memcache_bin_mapping($b);
      $stats = dmemcache_stats($bin, 'default', TRUE);
      if ($stats) {
        $output['bins'][$bin] = $stats[$bin]['total'];
      }
    }
  }
  return $output;
}

function paas_metric_memcache_call($value) {
  switch ($value) {
    case 'info':
      $info = paas_metric_memcache_get_info();
      $info = serialize($info);
      echo $info;
      exit;
  }
}
