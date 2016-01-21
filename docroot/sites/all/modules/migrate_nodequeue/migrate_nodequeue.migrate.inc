<?php

/**
 * @file
 * Migration code for Nodequeue.
 */

/*
 * Implementation of hook_migrate_api().
 */
function migrate_nodequeue_migrate_api() {
  $api = array(
    'api' => 2,
    'wizard classes' => array('MigrateNodequeueWizard'),
  );
  return $api;
}

