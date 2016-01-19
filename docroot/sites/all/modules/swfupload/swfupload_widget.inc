<?php
// $Id: swfupload_widget.inc,v 1.6 2010/12/02 16:02:53 skilip Exp $

/**
 * @file
 * SWFUpload widget hooks and callbacks.
 */

/**
 * Implements hook_field_widget_info().
 */
function swfupload_field_widget_info() {
  return array(
    'swfupload' => array(
      'label' => 'SWFUpload',
      'field types' => array('file', 'image'),
      'settings' => array(
        'progress_indicator' => 'throbber',
        'preview_image_style' => 'thumbnail',
      ),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_CUSTOM,
        'default value' => FIELD_BEHAVIOR_NONE,
      ),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function swfupload_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  // Collect the files already stored in this field indexed by their file ID.
  $files = array();
  foreach ($items as $delta => $file) {
    if ($file['fid'] != 0) {
      $files[$file['fid']] = $file;
    }
  }
  $element['#default_value'] = $files;

  $element_info = element_info('swfupload_widget');
  $element += array(
    '#type' => 'swfupload_widget',
    '#process' => $element_info['#process'],
    '#value_callback' => $element_info['#value_callback'],
    // Allows this field to return an array instead of a single value.
    '#extended' => TRUE,
  );
  // $element['#display_field'] = $field['settings']['display_field'];
  return $element;
}


/**
 * The #value_callback for the swfupload_widget type element.
 */
function swfupload_widget_value(&$element, $input = FALSE, $form_state = NULL) {

  if (is_string($input)) {
    $input = json_decode($input, TRUE);
  }

  if ($input === FALSE) {
    $default_value = array();

    if (!empty($element['#default_value'])) {
      foreach ($element['#default_value'] as $file) {
        if ($file) {

          // If we're dealing with an image, create a thumbpath
          if (image_get_info($file['uri'])) {
            $file['thumb'] = swfupload_thumb($file);
          }

          $default_value[$file['fid']] = $file;
        }
      }
    }
    return $default_value;
  }
  else {
    // Files need an integer value for the 'display' field.
    foreach ($input as $fid => $file) {
      if (empty($file['display'])) {
        $input[$fid]['display'] = 0;
      }
    }
    return $input;
  }
}

/**
 * Process the link type element before displaying the field.
 *
 * Build the form element. When creating a form using FAPI #process,
 * note that $element['#value'] is already set.
 *
 * The $fields array is in $form['#field_info'][$element['#field_name']].
 */
function swfupload_widget_process($element, $form_state, $form) {

  $field = field_info_field($element['#field_name']);
  $instance = field_info_instance($element['#entity_type'], $element['#field_name'], $element['#bundle']);

  // Make sure the element's name exists.
  if (!isset($element['#name'])) {
    $element['#name'] = array_shift($element['#parents']);
  }
  $element['#default_value'] = '[]';

  // Set a file upload limit for SWFUpload where 0 is unlimited.
  $limit = ($field['cardinality'] == -1 ? 0 : $field['cardinality']);

  // Set the button title which is used in the theme function.
  if ($field['type'] == 'image') {
    $element['#button_title'] = format_plural(($limit === 0 ? 2 : $limit), 'Upload new image', 'Upload new images');
  }
  else {
    $element['#button_title'] = format_plural(($limit === 0 ? 2 : $limit), 'Upload new file', 'Upload new files');
  }

  // Construct the JavaScript settings array.
  if ($library = libraries_load('swfupload')) {

    $settings['swfupload_settings'][$element['#id']] = array(
      'module_path' => drupal_get_path('module', 'swfupload'),
      'flash_url' => url($library['library path'] . '/Flash/swfupload.swf'),
      'upload_url' => url('swfupload'),  // Relative to the SWF file
      'upload_button_id' => $element['#id'],
      'file_post_name' => $element['#name'],
      'file_queue_limit' => $limit,
      'post_params' => array(
        'sid' => _swfupload_post_key(),
        'file_path' => $field['settings']['uri_scheme'] . '://' . $instance['settings']['file_directory'],
        'op' => 'move_uploaded_file',
        'instance' => json_encode(array('name' => $element['#field_name'])),
        'instance_settings' => json_encode($instance['settings'] + $field['settings']),
      ),
      'file_size_limit' => ($instance['settings']['max_filesize'] ? (parse_size($instance['settings']['max_filesize']) / 1048576) . 'MB' : 0),
      'file_types' => (empty($instance['settings']['file_extensions']) ? '' : '*.' . str_replace(" ", ";*.", $instance['settings']['file_extensions'])),
      'file_types_description' => ($element['#description'] ? $element['#description'] : ''),
      'file_upload_limit' => $limit,
      'custom_settings' => array(
        'upload_stack_value' => (!empty($element['#value'])) ? json_encode($element['#value']) : '[]',
        'max_queue_size' => ($instance['settings']['max_filesize'] ? $instance['settings']['max_filesize'] : 0),
      ),
    );
    drupal_add_js($settings, 'setting');
  }
  return $element;
}

/**
 * Theme function for the swfupload form element
 */
function theme_swfupload_widget($variables) {
  $element = $variables['element'];

  // Force the classes swfupload_button and disabled to be added to the button
  _form_set_class($element, array('swfupload_button', 'disabled'));
  $element['#attributes']['class'] = str_replace(' error', ' swfupload-error', $element['#attributes']['class']);

  $output[] = '<div id="' . $element['#id'] . '" ' . drupal_attributes($element['#attributes']) . '>';
  $output[] = '  <div class="swfupload-wrapper">';
  $output[] = '    <div id="' . $element['#name'] . '-swfwrapper">&nbsp;</div>';
  $output[] = '  </div>';
  $output[] = '  <div class="left">&nbsp;</div>';
  $output[] = '  <div class="center">' . $element['#button_title'] . '</div>';
  $output[] = '  <div class="right">&nbsp;</div><br />';
  $output[] = '</div>';

  if ($element['#description']) {
    $output[] = '  <div class="description">' . $element['#description'] . '</div>';
  }
  return join("\n", $output);
}
