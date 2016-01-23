<?php

function _votingapi_bayesian_get_vote_count_by_item($entity_type, $tag = 'vote', $entity_id = NULL) {
  $query = db_select('votingapi_vote', 'v')
    ->condition('tag', $tag, '=')
    ->condition('entity_type', $entity_type, '=');

  if (is_numeric($entity_id)) {
    $query->condition('entity_id', $entity_id, '=');
  }

  $query->addExpression('COUNT(vote_id)');

  return $query->execute()->fetchfield(0);
}

function _votingapi_bayesian_get_vote_count($entity_type, $tag = 'vote', $entity_id = NULL) {
  return _votingapi_bayesian_get_vote_count_by_item($entity_type, $tag, NULL);
}

function _votingapi_bayesian_get_total_sum($entity_type, $tag = 'vote') {
  $query = db_query('SELECT SUM(value) FROM votingapi_vote WHERE entity_type = :type AND tag = :tag', array(':type' => $entity_type, ':tag' => $tag));
  return $query->fetchfield(0);
}

function _votingapi_bayesian_get_avg_by_item($entity_type, $tag = 'vote', $entity_id = NULL) {
  $query = db_select('votingapi_vote', 'v')
    ->condition('tag', $tag, '=')
    ->condition('entity_type', $entity_type, '=');

  if (is_numeric($entity_id)) {
    $query->condition('entity_id', $entity_id, '=');
  }

  $query->addExpression('AVG(value)');

  return $query->execute()->fetchfield(0);
}

function _votingapi_bayesian_get_zero($entity_type, $tag = 'vote', $entity_id = NULL) {
  return 0;
}

function _votingapi_bayesian_get_avg($entity_type, $tag = 'vote', $entity_id = NULL) {
  return _votingapi_bayesian_get_avg_by_item($entity_type, $tag, NULL);
}

function _votingapi_bayesian_get_avg_vote_count($entity_type, $tag = 'vote', $empty = NULL) {
  $query = db_select('votingapi_vote', 'v')
    ->condition('tag', $tag, '=')
    ->condition('entity_type', $entity_type, '=');
  $query->addExpression('COUNT(entity_id)/COUNT(DISTINCT entity_id)');

  return $query->execute()->fetchfield(0);
}

function _votingapi_bayesian_get_num_distinct_item($entity_type, $tag = 'vote', $empty = NULL) {
  $query = db_select('votingapi_vote', 'v')
    ->condition('tag', $tag, '=')
    ->condition('entity_type', $entity_type, '=');
  $query->addExpression('COUNT(entity_id)');

  return $query->execute()->fetchfield(0);
}

function _votingapi_bayesian_get_min($entity_type, $tag = 'vote', $entity_id = NULL) {
  $query = db_select('votingapi_vote', 'v')
    ->condition('tag', $tag, '=')
    ->condition('entity_type', $entity_type, '=');

  if (is_numeric($entity_id)) {
    $query->condition('entity_id', $entity_id, '=');
  }

  $query->addExpression('MIN(value)');

  return $query->execute()->fetchfield(0);
}

function _votingapi_bayesian_get_max($entity_type, $tag = 'vote', $entity_id = NULL) {
  $query = db_select('votingapi_vote', 'v')
    ->condition('tag', $tag, '=')
    ->condition('entity_type', $entity_type, '=');

  if (is_numeric($entity_id)) {
    $query->condition('entity_id', $entity_id, '=');
  }

  $query->addExpression('MAX(value)');

  return $query->execute()->fetchfield(0);
}

function _votingapi_bayesian_get_item_median($entity_type, $tag = 'vote', $entity_id) {
  $query = db_query('SELECT x.value from votingapi_vote x, votingapi_vote y WHERE x.entity_type = :type AND x.tag = :tag AND x.entity_id = :id GROUP BY x.value HAVING SUM(SIGN(1-SIGN(y.value-x.value)))/COUNT(*) > .5 LIMIT 1;', array(':type' => $entity_type, ':tag' => $tag, 'id' => $entity_id));
  return $query->fetchfield(0);
}

function _votingapi_bayesian_get_items_median($entity_type, $tag = 'vote', $empty) {
  $query = db_query('SELECT x.value from votingapi_vote x, votingapi_vote y WHERE x.entity_type = :type AND x.tag = :tag GROUP BY x.value HAVING SUM(SIGN(1-SIGN(y.value-x.value)))/COUNT(*) > .5 LIMIT 1;', array(':type' => $entity_type, ':tag' => $tag));
  return $query->fetchfield(0);
}

function _votingapi_bayesian_get_std_by_item($entity_type, $tag = 'vote', $entity_id = NULL) {
  $query = db_select('votingapi_vote', 'v')
    ->condition('tag', $tag, '=')
    ->condition('entity_type', $entity_type, '=');

  if (is_numeric($entity_id)) {
    $query->condition('entity_id', $entity_id, '=');
  }

  $query->addExpression('STD(value)');

  return $query->execute()->fetchfield(0);
}

function _votingapi_bayesian_get_std($entity_type, $tag = 'vote', $entity_id = NULL) {
  return _votingapi_bayesian_get_std_by_item($entity_type, $tag, NULL);
}

function _votingapi_bayesian_get_variance_by_item($entity_type, $tag = 'vote', $entity_id = NULL) {
  $query = db_select('votingapi_vote', 'v')
    ->condition('tag', $tag, '=')
    ->condition('entity_type', $entity_type, '=');

  if (is_numeric($entity_id)) {
    $query->condition('entity_id', $entity_id, '=');
  }

  $query->addExpression('VARIANCE(value)');

  return $query->execute()->fetchfield(0);
}

function _votingapi_bayesian_get_variance($entity_type, $tag = 'vote', $entity_id = NULL) {
  return _votingapi_bayesian_get_variance_by_item($entity_type, $tag, NULL);
}
