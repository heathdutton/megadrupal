<?php

/**
 * @file
 * [polaris_online_registration_counts description]
 * display a simple table to show number of online registrations by month
 */
function polaris_online_registration_counts() {
$page = array();

$info = '<h3>This report displays Polaris online registration counts by month for' .
' the last 24 months in descending order.</h3>';

$page['info'] = array(
  '#markup' => $info,
);

$my_table = '<table style="width:300px"><tbody><tr><th>Year - Month</th>' .
  '<th>Number Registered</th></tr>';

// query the database
$results = get_registration_counts();

$count = 0;

$row_array = array(1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23);

foreach ($results as $row) {
  if (in_array($count, $row_array)) {
    $my_table .= '<tr class=" even"><td>' . $row->period . '</td><td>' . $row->registrations . '</td></tr>';
  }
  else {
    $my_table .= '<tr class=" odd"><td>' . $row->period . '</td><td>' . $row->registrations . '</td></tr>';
  }
  $count = $count + 1;
}


$my_table .= '</tbody></table><br />';

$page['report'] = array(
  '#markup' => $my_table,
);

return $page;
}

function get_registration_counts() {
  $result = db_query_range('SELECT DATE_FORMAT(from_unixtime(created),\'%Y - %m\') AS period, COUNT(barcode) AS registrations FROM {polaris_barcodes} GROUP BY Period ORDER BY 1 DESC', 0, 24);

  if ($result) {
    return $result;
  }
  else {
    drupal_set_message(t('There are no barcodes in the polaris_barcodes table'), 'error');
  }
}
