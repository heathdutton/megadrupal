<?php
/**
 * @file
 * makefile.file_type.inc
 */

/**
 * Implements hook_file_default_types().
 */
function makefile_file_default_types() {
  $export = array();

  $file_type = new stdClass();
  $file_type->disabled = FALSE; /* Edit this to true to make a default file_type disabled initially */
  $file_type->api_version = 1;
  $file_type->type = 'drush_make';
  $file_type->label = 'Drush Make';
  $file_type->description = 'Drush Make File';
  $file_type->mimetypes = array(
    0 => 'text/plain',
    1 => 'application/octet-stream',
  );
  $export['drush_make'] = $file_type;

  return $export;
}
