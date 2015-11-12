<?php

/**
 * Page for displaying couchbase statistics 
 */
function couchbase_statistics() {
  $connections = variable_get(COUCHBASE_CONF, array());
  $output = array();
  foreach ($connections as $key => $value) {
    $cb = couchbase($key);
    if ($cb != FALSE) {
      $stats = $cb->getStats();
      if ($stats !== FALSE) {
        foreach ($stats as $stat) {
          $header = array(t('Property'), t('Value'));
          $rows = array();
          $rows[] = array(
            'host',
            $value['host'],          
          );
          $rows[] = array(
            'user',
            $value['user'],
          );
          $rows[] = array(
            'bucket',
            $value['bucket'],
          );
          foreach ($stat as $k => $v) {
            $rows[] = array(
              $k,
              $v,
            );
          }
        }
        $output[$key . 'label'] = array(
          '#markup' => "<h3>Statistics for: ${value['bucket']} bucket</h3>",
        );
        $output[$key] = array(
          '#theme' => 'table', 
          '#header' => $header,
          '#rows' => $rows
        );
      }
    }
  }
  return $output;
}