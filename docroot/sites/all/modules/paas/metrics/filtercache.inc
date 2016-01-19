<?php

function paas_metric_filtercache_call($value) {
  switch ($value) {
    case 'uncacheable':
      $uncacheable = array();
      foreach (module_implements('filter') as $module) {
        $function = "{$module}_filter";
        foreach ($function('list') as $key => $name) {
          if ($function('no cache', $key)) {
            $uncacheable[] = $name;
          }
        }
      }
      drupal_set_header('Content-Type: text/plain; charset=utf-8');
      echo "name\n" . implode("\n", $uncacheable);
      exit;
  }
}
