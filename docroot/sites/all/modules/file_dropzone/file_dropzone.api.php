<?php

/**
 * Implements hook_file_dropzone_process_form_alter().
 *
 * This hooks allows changing the input before it is passed to
 * drupal_process_form().
 *
 * This is useful for example when supporting non-numeric fids.
 */
function hook_file_dropzone_process_form_alter(&$input, &$form, $form_state, $form_parents) {
  $current_element = $form;
  foreach ($form_parents as $parent) {
    $current_element = $current_element[$parent];
  }

  $field_name = $current_element['#field_name'];
  $langcode = $current_element['#language'];

  $items = &$input[$field_name][$langcode];
  foreach ($items as $delta => $item) {
    if (!empty($item['fid'])) {
      list($target_type, $target_id) = explode(':', $item['fid'], 2);
      $items[$delta]['target_type'] = $target_type;
      $items[$delta]['target_id'] = $target_id;
    }

    // Ensure attached media is correctly pushed to target_type / target_id.
    $upload_name = implode('_', $form_parents) . '_' . $delta;
    if (!empty($input['media'][$upload_name])) {
      $item['fid'] = $input['media'][$upload_name];

      list($target_type, $target_id) = explode(':', $item['fid'], 2);
      $items[$delta]['fid'] = $fid;
      $items[$delta]['target_type'] = $target_type;
      $items[$delta]['target_id'] = $target_id;
    }
  }
}

