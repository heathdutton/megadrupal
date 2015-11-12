<?php
  $rows = array();
  $header = array(
    array(
      'data' => t('Name'),
      'title' => t('Name and Type of Test.'),
    ),
    array(
      'data' => t('Your score'),
      'title' => t('The score attained last time you took the test, if applicable'),
    ),
    array(
      'data' => t('Percentile'),
      'title' => t('What percentage of people you have done better than with your test result.'),
    ),
    array(
      'data' => t('Average'),
      'title' => t('Average score for this test across all users.'),
    ),
  );

  foreach ($tests as $id => $test) {
    $rows[] = array(
      $test['access'] ? l($test['name'], $test['link']) : $test['name'],
      $test['score'] ? $test['score'] . '%' : '-', // Your score
      $test['percentile'] ? $test['percentile'] . '%' : '-', // Percentile
      $test['average'] ? $test['average'] . '%' : '-', // Average
    );
  }

  print theme('table', array('header' => $header, 'rows' => $rows))
?>
