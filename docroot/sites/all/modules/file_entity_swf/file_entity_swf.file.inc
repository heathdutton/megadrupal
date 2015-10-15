<?php

/**
 * @file
 * File hook implementations for File Entity SWF
 */

/**
 * Implements hook_file_presave().
 */
function file_entity_swf_file_presave($file) {
  if (!in_array($file->type, array('flash'))) {
    return null;
  }

  // Get metadata on the .swf file.
  $data = @getimagesize(drupal_realpath($file->uri));
  if (isset($data) && is_array($data)) {
    $file->metadata['width'] = $data[0];
    $file->metadata['height'] = $data[1];
  }
}
