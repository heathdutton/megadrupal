<?php

/**
 * Field formatter - Image Preset
 */
function theme_image_preset_field_image_preset($vars) {
  $output = '';

  $file = $vars['file'];
  $url = file_create_url($file['uri']);
  
  if (!empty($file['uri'])) {
    $image_vars = array(
      'path' => $file['uri'],
      'alt' => '',
      'title' => $file['filename'],
      'style_name' => $vars['image_style'],
    );
    
    if (isset($file['not_stream']) && $file['not_stream']) {
      $output = '<img src="' . $file['uri'] . '" />';
    }
    else {
      $output .= theme('image_style', $image_vars);
    }
  }
  
  return $output;
}