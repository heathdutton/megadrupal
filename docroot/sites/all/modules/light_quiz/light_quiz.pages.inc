<?php

function light_quiz_list_page() {
  $tests = array_map(function($test) {
    // Total results.
    $query = db_select('light_quiz_results');
    $query->addExpression('COUNT(*)');
    $query->condition('id', $test['_id']);
    $total = $query->execute()->fetchField();

    $score = $percentile = $average = 0;

    if ($total) {
      // Your score.
      $score = db_select('light_quiz_results', 'r')
        ->fields('r', array('percentage'))
        ->condition('r.uid', $GLOBALS['user']->uid)
        ->condition('r.id', $test['_id'])
        ->execute()->fetchField();

      // Percentile.
      $query = db_select('light_quiz_results');
      $query->addExpression('COUNT(*)');
      $query->condition('id', $test['_id']);
      $query->condition('percentage', $score, '<=');
      $percentile = ($query->execute()->fetchField() / $total) * 100;

      // Average.
      $query = db_select('light_quiz_results');
      $query->addExpression('AVG(percentage)');
      $query->condition('id', $test['_id']);
      $average = $query->execute()->fetchField();
    }

    return array(
      'name' => $test['name'],
      'link' => 'quiz/' . $test['_id'],
      'access' => light_quiz_view_access($test),
      'score' => (int)$score,
      'percentile' => (int)$percentile,
      'average' => (int)$average,
    );
  }, _light_quiz_load_tests());

  return theme('light_quiz_stats', array('tests' => $tests));
}

function light_quiz_result_incoming() {
  $data = json_decode(file_get_contents("php://input"), TRUE);

  db_merge('light_quiz_results')
    ->key(array(
      'id' => $data['id'],
      'uid' => $GLOBALS['user']->uid,
    ))
    ->fields(array(
      'id' => $data['id'],
      'uid' => $GLOBALS['user']->uid,
      'percentage' => $data['percentage'],
      'timestamp' => time(),
    ))
    ->execute();

  print drupal_json_encode(array(
    'status' => 'success',
    'message' => t('Only light_quiz will ever see this page - humans go away!'),
  ));
}

function light_quiz_page_title($test) {
  return $test['name'];
}

function light_quiz_page($test) {
  return theme('light_quiz', array('test' => $test));
}
