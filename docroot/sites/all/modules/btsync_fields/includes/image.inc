<?php
/**
 * @file
 * Image module integration.
 */

/**
 * Implements hook_btsync_fields_field_widget_info_alter() on behalf of
 * image.module.
 */
function image_btsync_fields_field_widget_info_alter(&$items) {
  $items['btsync_fields_file_basic']['field types'][] = 'image';
}
