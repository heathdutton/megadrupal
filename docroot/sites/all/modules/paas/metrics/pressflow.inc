<?php

function paas_metric_pressflow_call($value) {
  header('Content-Type: text/plain');
  if (is_file('modules/path_alias_cache/path_alias_cache.module')) {
    echo 'yes';
  }
  else {
    echo 'no';
  }
}
