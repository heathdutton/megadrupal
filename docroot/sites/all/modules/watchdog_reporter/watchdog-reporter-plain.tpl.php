<?php

/**
 * @file
 * Watchdog reporter mail template
 *
 */  
?>

<?php

if ($variables['preset']['#values']['watchdog_reporter_group'] != WATCHDOG_REPORTER_GROUP_NONE) {
  $lines = array();
  $printf_tpl = "%7s %-50.500s";
  $lines[] = sprintf($printf_tpl, 'Count', 'Messages');
  foreach ($variables['result'] as $key => $row) {
    $lines[] = sprintf($printf_tpl, $row->count, $key);
  }
  echo implode("\n", $lines);
}
else {
  foreach ($variables['result'] as $key => $row) {
    echo $row->message . "\r\n";
  }
}
?>
