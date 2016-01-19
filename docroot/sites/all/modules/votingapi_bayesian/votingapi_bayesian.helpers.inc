<?php

function _votingapi_bayesian_configuration() {
  $configuration = variable_get('votingapi_bayesian_config');

  if (!isset($configuration['config'])) {
    $configuration['config'] = array();
  }

  foreach($configuration['config'] as $tag => $data) {
    if (!isset($data['function_c']) || !isset($data['function_m'])) {
      unset($configuration['config'][$tag]);
      continue;
    }
    if (!function_exists($data['function_c']) || !function_exists($data['function_m'])) {
      unset($configuration['config'][$tag]);
    }
  }

  variable_set('votingapi_bayesian_config', $configuration);

  return $configuration;
}

function _votingapi_bayesian_sort_array_key_name($a, $b) {
  return strcmp($a['name'], $b['name']);
}

function _votingapi_bayesian_compute($entity_type, $entity_id, $config) {
  /**
   * Bayesian formula
   *
   * (C * m) + (ITEM_NUM_VOTE * ITEM_AVG_RATING)
   * ---------------------------------------------
   *         (C + ITEM_NUM_VOTE)
   *
   * See: http://goo.gl/EQ0D9M
   *
   */
  $item_num_vote   = _votingapi_bayesian_get_vote_count_by_item($entity_type, $config['tag'], $entity_id);
  $item_avg_rating = _votingapi_bayesian_get_avg_by_item($entity_type, $config['tag'], $entity_id);

  /*
   * C is assigned a value that is proportional to the typical data set size.
   * The value is larger when the expected variation between data sets (within the larger population) is small.
   * It is smaller, when the data sets are expected to vary substantially from one another.
   */
  $c = $config['function_c']($entity_type, $config['tag'], $entity_id);

  /*
   * In cases where the averages' relative values are the only result of importance, m can be replaced with zero.
   */
  $m = $config['function_m']($entity_type, $config['tag'], $entity_id);

  $result = 0;

  if ( 0 != ($c + $item_num_vote)) {
    $result = (($c*$m + ($item_num_vote * $item_avg_rating)) / ($c + $item_num_vote));
    //$msg = " ( ( " . $c . " * " . $m . " ) + ( " . $item_num_vote . " * " . $item_avg_rating . " ) ) / ( " . $c . " + " . $item_num_vote . " ) ) = " . $result;
    //echo $msg . "\n";
  }

  return $result;
}

function _votingapi_bayesian_regenerate($entity_type, $config) {
  $query = db_select('votingapi_cache', 'v')->distinct()
    ->fields('v', array('entity_id'))
    ->condition('tag', $config['tag'], '=')
    ->condition('entity_type', $entity_type, '=');
  $result = $query->execute();

  while($record = $result->fetchObject()) {
    db_update('votingapi_cache')
      ->fields(array(
        'value' => _votingapi_bayesian_compute($entity_type, $record->entity_id, $config)
      ))
      ->condition('function', 'bayesian', '=')
      ->condition('entity_type', $entity_type, '=')
      ->condition('tag', $config['tag'], '=')
      ->condition('entity_id', $record->entity_id, '=')
      ->execute();
  }
}
