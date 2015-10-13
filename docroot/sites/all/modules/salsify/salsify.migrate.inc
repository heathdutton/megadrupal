<?php

/**
 * @file
 * salsify.migrate.inc
 */

/**
 * Implements hook_migrate_api().
 */
function salsify_migrate_api() {
  $api = array(
    'api' => 2,
    'wizard classes' => array('SalsifyMigrateWizard'),
    'field handlers' => array('SalsifyCommerceProductReferenceFieldHandler'),
  );
  return $api;
}
