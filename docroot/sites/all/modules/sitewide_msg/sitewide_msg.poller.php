<?php
/**
 * @file
 * This file is the polling core file.
 * It should serve clients' poll request with minimal overhead.
 *
 * Also the file is responsible to turn off message if expiration timedate was set.
 *
 */
$root = preg_replace('/\/sites\/.*/', '', $_SERVER['SCRIPT_FILENAME']);
if ($root == $_SERVER['SCRIPT_FILENAME']) {
  echo "Installation error. Maybe module is not in correct location.";
  exit;
}
define('DRUPAL_ROOT', $root);

require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_DATABASE);

$query = db_select('{variable}', 'v')
  ->fields('v', array('value'));
$query->condition('name', 'sitewide_msg_alert');
$result = $query->execute();
$data = $result->fetchField();

if ($data) {
  $alert = unserialize($data);
}

// Polling interval in seconds.
$interval = is_object($alert) ? $alert->polling_interval : 30;
$output_no_messages = "<head><meta http-equiv='refresh' content='$interval' /> </head><body></body>";

if (!$data || ($alert->is_active == FALSE)) {
  echo $output_no_messages;
  exit;
}

if ($alert->is_expiration) {
  if ($alert->expiration_time <= time()) {
    $alert->is_active = FALSE;
    // Obsoleted, let us turn it off.
    variable_set('sitewide_msg_expiration_on', FALSE);
    variable_set('sitewide_msg_on', FALSE);
    variable_set('sitewide_msg_alert', $alert);
    echo $output_no_messages;
    exit;
  }
}

// Internal CSS
echo '<style type="text/css">

.alert { background: red; color: white; }
.warning { background: yellow; color: black; }
.status { background: lightgreen; color: black; }

body {
  font: 12px/18px "Lucida Grande", "Arial", sans-serif;
  font-weight:bold;
}

</style>';

$display .= "<div align='center' class='$alert->class_css'>$alert->message</div>";

$output = "<head><meta http-equiv='refresh' content='$interval' /> </head><body>$display</body>";
echo $output;
exit;
