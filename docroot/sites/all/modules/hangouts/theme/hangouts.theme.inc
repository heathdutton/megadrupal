<?php

/**
 * @file
 * Theming function
 */

function theme_hangouts_button($vars) {  
  $image = hangouts_get_image($vars['size']);
  if (is_array($image)) {
    $image = array_shift($image);
  }
  if (!$vars['gid']) {
    return $image;
  }

  $url = 'https://plus.google.com/hangouts/_?gid=' . $vars['gid'];
  $attrs = array(
    'html' => TRUE,
    'attributes' => array(
      'style' => 'text-decoration:none;',
    ),
  );

  return l($image, $url, $attrs);
}
