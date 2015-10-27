<?php

/**
 * @file
 * prettyPhoto formatter hooks and callbacks.
 */

/**
 * Theme function for displaying the prettyPhoto trigger image in an imagefield.
 *
 * @param array $variables
 *   An array of image attributes, e.g. class name.
 *
 * @return string
 *   HTML output for displaying the image.
 */
function theme_prettyphoto_formatters_image($variables) {
  $path = $variables['path'];
  $item = $variables['item'];
  $image_style = $variables['image_style'];
  $field_name = $variables['field_name'];

  if (!$variables["pfi_style"]) {
    $path['path'] = file_create_url($item['uri']);
  }
  else {
    $path['path'] = image_style_url($variables["pfi_style"], $item['uri']);
  }

  $path['options']['attributes']['rel'] = 'prettyPhoto[' . $field_name . ']';
  $path['options']['attributes']['title'] = $item['title'];

  $output = theme('image_formatter', array(
    'item' => $item,
    'path' => $path,
    'image_style' => $image_style,
  ));

  return '<div class="prettyphoto-formatter-image">' . $output . '</div>';
}
