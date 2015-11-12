<?php

/**
 * @file
 * Utility Image Rebuild Batch functions.
 */

/**
 * Runs the batch process for one entity.
 */
function _utility_image_rebuild_batch_process_entity($table, $field_name, $id, &$context) {
  // Load existing file information.
  $file = file_load($id);
  // Check if the information is object.
  if (is_object($file)) {
    $info = image_get_info($file->uri);
    if (is_array($info)) {
      $file_object = $file;
      $file_object->filename = drupal_basename($file->uri);
      $file_object->filemime = file_get_mimetype($file->uri);
      // Update the filename and filemine information.
      file_save($file_object);
      // Update the file demensions.
      db_update($table)->fields(array(
        $field_name . '_width' => $info['width'],
        $field_name . '_height' => $info['height'],
      ))->condition($field_name . '_fid', $id)->execute();
    }
  }
  // Update batch progress information.
  if (!isset($context['results']['processed'])) {
    $context['results']['processed'] = 0;
  }
  $context['results']['processed']++;
  $context['message'] = t('We are processing the file with fid: @id', array(
    '@id' => $id,
  ));
}

/**
 * Batch process finished callback.
 */
function _utility_image_rebuild_batch_finished($success, $results, $operations) {
  if ($success) {
    $message = t('The images have been rebuilt by Utility Tool - Image Rebuild.');
    $severity = WATCHDOG_INFO;
  }
  else {
    $error_operation = reset($operations);
    $message = t('An error occurred while processing %error_operation with arguments: @arguments', array(
      '%error_operation' => $error_operation[0],
      '@arguments' => print_r($error_operation[1], TRUE),
    ));
    $severity = WATCHDOG_ERROR;
  }
  drupal_set_message($message);
  // Add watchdog to log the operation status.
  watchdog('Utility Image Rebuild', '%message', array('%message' => $message), $severity);
}
