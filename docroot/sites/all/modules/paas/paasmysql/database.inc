<?php

require_once('./includes/database/mysql/database.inc');

class DatabaseConnection_paasmysql extends DatabaseConnection_mysql {
  public function query($query, array $args = array(), $options = array()) {
    if (function_exists('paas_metric_profiler_query_start')) {
      paas_metric_profiler_query_start();
    }
    $output = parent::query($query, $args, $options);
    if (function_exists('paas_metric_profiler_query_stop')) {
      paas_metric_profiler_query_stop($query);
    }
    return $output;
  }
}
