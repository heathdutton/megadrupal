<?php
/**
 * @file
 * File module integration.
 */

/**
 * Implements hook_btsync_fields_field_widget_info() on behalf of file.module.
 */
function file_btsync_fields_field_widget_info() {
  return array(
    'btsync_fields_file_basic' => array(
      'label' => t('BitTorrent Sync (basic)'),
      'field types' => array('file'),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_CUSTOM,
      ),
    ),
  );
}

/**
 * 'Field widget form' callback for 'btsync_fields_file_basic' widget.
 */
function btsync_fields_btsync_fields_file_basic_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $field_settings = $instance['settings'];
  $widget_settings = $instance['widget']['settings'];

  $defaults = array(
    'fid' => 0,
    'display' => !empty($field['settings']['display_default']),
    'description' => '',
  );

  if ($field['cardinality'] == 1) {
    // Set the default value.
    $element['#default_value'] = !empty($items) ? $items[0] : $defaults;
    // If there's only one field, return it as delta 0.
    if (empty($element['#default_value']['fid'])) {
      $element['#description'] = theme('file_upload_help', array(
        'description' => $element['#description'],
      ));
    }
    $elements = array($element);
  }
  else {
    $elements = array($element);
    // If there are multiple values, add an element for each existing one.
    foreach ($items as $item) {
      $elements[$delta] = $element;
      $elements[$delta]['#default_value'] = $item;
      $elements[$delta]['#weight'] = $delta;
      ++$delta;
    }
  }

  $elements += array(
    '#type' => 'fieldset',
    '#title' => check_plain($element['#title']),
    '#description' => check_plain($instance['description']),
  );

  $elements[0]['btsync_field'] = array(
    '#type' => 'btsync_field',
    '#default_value' => '',
  );
  if (!empty($element['#entity'])) {
    $elements[0]['btsync_field']['#default_value'] = btsync_fields_get_secret($element['#entity'], $element['#entity_type'], $element['#field_name']);
  }

  return $elements;
}

/**
 * 'Presave' callback for 'btsync_fields_file_basic' widget.
 */
function btsync_fields_btsync_fields_file_basic_presave(&$field, $btsync_field) {
  extract($btsync_field);

  // Empty the items list so it can be regenerated from the BTSync files.
  $field = array('#secret' => $secret);

  $files = btsync_fields_get_files($secret);
  $folder = btsync_fields_get_folder($secret);

  $field_info = field_info_field($field_name);
  $destination = $field_info['settings']['uri_scheme'] . '://' . $instance['settings']['file_directory'];

  // Get field validators.
  $validators = file_field_widget_upload_validators($field_info, $instance);

  if (file_prepare_directory($destination, FILE_MODIFY_PERMISSIONS | FILE_CREATE_DIRECTORY)) {
    foreach ($files as $file) {
      $file = clone $file;
      if ($file->have_pieces == $file->total_pieces) {
        $source = $folder->dir . '/' . $file->name;
        $file->uri = $destination . '/' . $file->name;
        file_unmanaged_copy($source, $file->uri, FILE_EXISTS_REPLACE);

        $files = file_load_multiple(array(), array('uri' => $file->uri));
        if (!empty($files)) {
          $file = reset($files);
        }
        else {
          $file->uid = 1;
          $file->filename = $file->name;
          $file->filesize = $file->size;
          $file->filemime = file_get_mimetype($file->uri);
        }

        // Ensure file is valid based on Field settings.
        $errors = file_validate($file, $validators);
        if (empty($errors)) {
          file_save($file);
          $field[$langcode][] = (array) $file;
        }
      }
    }
  }
}
