<?php

/**
 * Implements hook_dragdropfile_field_widget_types_alter().
 *
 * Extend or reduce the standard list of supported field widget types: file_generic, image_image.
 */
function hook_dragdropfile_field_widget_types_alter(&$supported) {
  $supported[] = 'my_custom_file_field_widget_type';
}
