<?php

/**
 * @file
 * Api documentation.
 */

/**
 * Handle the file entity updating.
 *
 * With the passed in entity wrapper object and imported data from MPX
 * we can use this function override the default field update functions.
 *
 * @param entity_metadata_wrapper $file_wrapper
 *   The file entity video bundle.
 * @param array $fields
 *   Array of all fields that require remote MPX data sync.
 * @param array $video
 *   Array of data returned from remote MPX.
 */
function hook_media_thepaltform_mpx_field_update_video($file_wrapper, $fields, $video) {

  $modified = FALSE;
  foreach ($fields as $field => $value) {
    // Switch by the field widget type.
    switch ($value['widget type']) {
      case 'text_textfield':
        if (empty($video[$field])) {
          $existing_field = $file_wrapper->$field->value();
          if (isset($existing_field)) {
            $file_wrapper->$field = '';
            $modified = TRUE;
          }
        }
        else {
          $file_wrapper->$field = $video[$field];
          $modified = TRUE;
        }
        break;
    }
  }
  if ($modified) {
    $file_wrapper->save();
  }
  return TRUE;
}

/**
 * Handle the file entity updating on initial video insert.
 *
 * With the passed in entity wrapper object and imported data from MPX
 * we can use this function override the default field update functions.
 * we can switch with widget type or even with field machine name.
 *
 * @param entity_metadata_wrapper $file_wrapper
 *   The file entity video bundle.
 * @param array $fields
 *   Array of all fields that require remote MPX data sync.
 * @param array $video
 *   Array of data returned from remote MPX.
 */
function hook_media_thepaltform_mpx_field_insert_video($file_wrapper, $fields, $video) {

  $modified = FALSE;
  foreach ($fields as $field => $value) {
    switch ($value['widget type']) {
      case 'text_textfield':
        if (!empty($video[$field])) {
          $file_wrapper->$field = $video[$field];
          $modified = TRUE;
        }
        break;
    }
  }
  if ($modified) {
    $file_wrapper->save();
  }
  return TRUE;
}
