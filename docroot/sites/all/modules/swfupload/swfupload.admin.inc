<?php
// $Id: swfupload.admin.inc,v 1.2 2010/06/17 18:24:57 eugenmayer Exp $

/**
 * @file
 * Include file for all functions required while using the CCK widget (e.g.: in node-edit forms).
 */

/**
 * Menu callback for menu item 'swfupload'.
 * This function is executed when SFWUpload.swf requests a file upload
 */
function swfupload_js() {
  $p = (object) $_POST;
  $op = $p->op;
  $file = (isset($p->file) ? json_decode($p->file) : NULL);
  $instance = json_decode($p->instance);
  $instance_settings = json_decode($p->instance_settings);
  $file_path = $p->file_path;
  unset($p);

  switch ($op) {
    case 'init':
      // Add the default callback functions for the SWF Upload
      $instance->type = 'table';
      $instance->callbacks = array(
        'swfupload_loaded_handler' => 'ref.swfUploadLoaded',
        'file_queued_handler' => 'ref.fileQueued',
        'queue_complete_handler' => 'ref.queueComplete',
        'file_queue_error_handler' => 'ref.fileQueueError',
        'file_dialog_complete_handler' => 'ref.dialogComplete',
        'upload_success_handler' => 'ref.uploadSuccess',
        'upload_progress_handler' => 'ref.uploadProgress',
        'upload_error_handler' => 'ref.uploadError',
        'upload_complete_handler' => 'ref.uploadComplete',
        'init_complete_handler' => 'ref.initComplete',// This custom javascript callback function is used to place the markup inside the dom
      );
      $instance->elements = array(
        'drag' => array(
          'class' => 'drag first indentation',
          'type' => 'drag',
          'colspan' => 3,
          'title' => t('Filename'),
          'add_separator' => TRUE,
        ),
        'icon' => array(
          'type' => 'icon',
          'class' => 'icon',
        ),
        'filename' => array(
          'type' => 'markup',
          'value' => '[filename]',
          'class' => 'text title',
        ),
      );
      break;
    case 'move_uploaded_file':
      $max_filesize = parse_size(file_upload_max_size());
      if (!empty($instance_settings->max_filesize) && parse_size($instance_settings->max_filesize) < $max_filesize) {
        $max_filesize = parse_size($instance_settings->max_filesize);
      }

      $file->validators = array(
        'file_validate_extensions' => array($instance_settings->file_extensions),
//        'filefield_validate_image_resolution' => array($instance_settings->max_resolution, $instance_settings->min_resolution),
        'file_validate_size' => array($max_filesize),
      );
      $file->file_path = $file_path;

      // Allow other modules to change the file_path an validators
      foreach (module_implements('swfupload') as $module) {
        $function = $module . '_swfupload';
        $function($file, $op, $instance, $instance_settings);
      }

      $op = 'upload_complete';
      break;
  }

  // Allow other modules to change the returned data
  foreach (module_implements('swfupload') as $module) {
    $function = $module . '_swfupload';
    $function($file, $op, $instance, $instance_settings);

    // We want to make sure the last column of each tablerow contains the 'cancel' or 'delete' button.
    if ($op == 'init') {
      $instance->elements['cancel'] = array('class' => 'last', 'type' => 'cancel');
    }
  }

  // Replace keys named 'class' to 'classname'
  if (!empty($instance->elements)) {
    array_walk($instance->elements, '_class_to_classname');
  }

  $p->op = $op;
  $p->file = $file;
  $p->file_path = $file_path;
  $p->instance = $instance;
  $p->instance_settings = $instance_settings;
  $p->messages = drupal_get_messages();

  echo json_encode($p);
  exit(0);
}

/**
 * Helper function to replace instance elements class keys to classname keys
 * Safari hangs if you use 'class' in javascript
 */
function _class_to_classname(&$element) {
  $element['classname'] = $element['class'];
  unset($element['class']);
}

