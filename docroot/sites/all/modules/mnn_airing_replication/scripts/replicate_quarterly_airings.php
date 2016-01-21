<?php
define('DRUPAL_ROOT', getcwd());

require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

$serialized_text =  variable_get(MNN_AIRING_REPLICATABLE_PROJECTS);
mnn_airing_replication_replicate_quarterly_airings();

return;
?>
