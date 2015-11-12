<?php

function tribune_local_stats($node) {
  drupal_add_css(drupal_get_path('module', 'tribune_local') . '/tribune-local-stats.css');
  drupal_add_js('//cdnjs.cloudflare.com/ajax/libs/d3/3.0.8/d3.min.js', 'external');
  drupal_add_js(drupal_get_path('module', 'tribune_local') . '/tribune-local-stats.js', 'file');

  drupal_add_js(array(
    'tribune_stats' => array(
      'activity' =>  tribune_local_stats_activity($node),
      'users' =>     tribune_local_stats_users($node),
      'relations' => tribune_local_stats_relations($node),
    ),
  ), 'setting');

  return drupal_get_form('tribune_local_stats_form', $node);
}

function tribune_local_stats_activity($node) {
  $query = db_select('tribune_local_post', 't');
  $query->addExpression('SUBSTRING(timestamp FROM 1 FOR 4)', 'year');
  $query->addExpression('SUBSTRING(timestamp FROM 5 FOR 2)', 'month');
  $query->addExpression('SUBSTRING(timestamp FROM 7 FOR 2)', 'day');
  $query->addExpression('SUBSTRING(timestamp FROM 1 FOR 8)', 'date');
  $query->addExpression('COUNT(*)', 'count');
  $query->addExpression('LENGTH(timestamp)', 'strlen');
  $query->groupBy('date');
  $query->condition('t.nid', $node->nid);
  $query->havingCondition('strlen', 14);
  $volume = $query->execute()->fetchAllAssoc('date');
  $counts = $query->execute()->fetchAllKeyed(3, 4);
  $days = array_keys($volume);
  $start = min($days);
  $stop = max($days);

  $start = array(
    'year' =>  (int)substr($start, 0, 4),
    'month' => (int)substr($start, 2, 2),
    'day' =>   (int)substr($start, 4, 2),
  );
  $stop = array(
    'year' =>  (int)substr($stop, 0, 4),
    'month' => (int)substr($stop, 2, 2),
    'day' =>   (int)substr($stop, 4, 2),
  );

  // This is ugly
  $query = db_select('tribune_local_post', 't');
  $query->addExpression('SUBSTRING(timestamp FROM 9 FOR 2)', 'hour');
  $query->addExpression("WEEKDAY(STR_TO_DATE(timestamp, '%Y%m%d'))", 'weekday');
  $query->addExpression("CONCAT(WEEKDAY(STR_TO_DATE(timestamp, '%Y%m%d')), ':', SUBSTRING(timestamp FROM 9 FOR 2))", 'division');
  $query->addExpression('COUNT(*)', 'count');
  $query->groupBy('division');
  $query->condition('t.nid', $node->nid);
  $hourly = $query->execute()->fetchAllAssoc('division');

  // Initialize empty divisions
  for ($day = 0 ; $day < 7 ; $day++) {
    for ($hour = 0 ; $hour < 24 ; $hour++) {
      $division = $day . ':' . str_pad($hour, 2, '0', STR_PAD_LEFT);
      if (!isset($hourly[$division])) {
        $hourly[$division] = array(
          'hour' => str_pad($hour, 2, '0', STR_PAD_LEFT),
          'weekday' => $day,
          'division' => $division,
          'count' => 0,
        );
      }
    }
  }

  return array(
    'calendar' => array_values($volume),
    'hourly' => array_values($hourly),
    'max' => max($counts),
    'min' => min($counts),
    'start' => $start,
    'stop' => $stop,
  );
}

function tribune_local_stats_users($node) {
  $query = db_select('tribune_local_post', 't');
  $query->join('users', 'u', 'u.uid = t.uid');
  $query->addExpression('COUNT(*)', 'count');
  $query->fields('u', array('name'));
  $query->groupBy('u.name');
  $query->condition('t.nid', $node->nid);
  $query->condition('u.uid', 0, '>');
  $volume_auth = $query->execute()->fetchAllAssoc('name');

  foreach ($volume_auth as $key => &$value) {
    $value->count = (int)$value->count;
  }

  $query = db_select('tribune_local_post', 't');
  $query->addExpression('SUBSTRING(t.info FROM 1 FOR 10)', 'name');
  $query->addExpression('COUNT(*)', 'count');
  $query->groupBy('name');
  $query->condition('t.nid', $node->nid);
  $query->condition('t.uid', 0, '=');
  $volume_anon = $query->execute()->fetchAllAssoc('name');

  foreach ($volume_anon as $key => &$value) {
    $value->count = (int)$value->count;
  }

  return array(
    t('Authentified') => array_values($volume_auth),
    t('Anonymous') => array_values($volume_anon),
  );
}

function tribune_local_stats_relations($node) {
  $query = db_select('tribune_local_post_references', 't');
  $query->addExpression('COUNT(*)', 'count');
  $query->fields('t', array('reference_uid', 'target_uid'));
  $query->groupBy('t.reference_uid, t.target_uid');
  $query->condition('t.target_uid', 0, '<>');
  $query->condition('t.reference_uid', 0, '<>');
  $query->condition('t.nid', $node->nid);
  $results = $query->execute();

  $query = db_select('tribune_local_post', 't');
  $query->join('users', 'u', 'u.uid = t.uid');
  $query->addExpression('COUNT(*)', 'count');
  $query->fields('t', array('uid'));
  $query->fields('u', array('name'));
  $query->groupBy('t.uid');
  $query->condition('t.nid', $node->nid);
  $query->condition('t.uid', 0, '<>');
  $users = $query->execute()->fetchAllAssoc('uid');

  $relations = array();
  while ($relation = $results->fetchAssoc()) {
    $relation = (object)$relation;

    $relations[] = array(
      'source_id' => (int)$relation->reference_uid,
      'target_id' => (int)$relation->target_uid,
      'count' => (int)$relation->count,
    );
  }

  return array(
    'relations' => $relations,
    'names' => $users,
  );
}

function tribune_local_stats_form($form, $form_state, $node) {
  return array(
    'node' => array(
      '#value' => $node,
    ),
    '#prefix' => '<noscript>' . t('JavaScript is necessary for displaying statistics.') . '</noscript>',
    'activity' => array(
      '#type' => 'fieldset',
      '#title' => t('Yearly activity'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      'content' => array(
        '#markup' => '<div id="tribune-stats-activity"></div>',
      )
    ),
    'daily-activity' => array(
      '#type' => 'fieldset',
      '#title' => t('Daily activity'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      'content' => array(
        '#markup' => '<div id="tribune-stats-daily-activity"></div>',
      )
    ),
    'users' => array(
      '#type' => 'fieldset',
      '#title' => t('Active users'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      'content' => array(
        '#markup' => '<div id="tribune-stats-users"></div>',
      )
    ),
    'relations' => array(
      '#type' => 'fieldset',
      '#title' => t('Relations between users'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      'content' => array(
        '#markup' => '<div id="tribune-stats-relations"></div>',
      )
    ),
  );
}
