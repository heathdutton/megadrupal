<?php

/**
 * @file
 * Image output for a Slick Slideshow slide.
*/

if ($item) {
  $image = array(
    'path' => $item['uri'],
    'width' => $item['width'],
    'height' => $item['height'],
    'alt' => $item['alt'],
    'title' => $item['title'],
    'attributes' => '',
  );

  // If there's a style applied to the image (there should be), theme it with the style.
  if (function_exists('theme_image_style') && isset($image_style) && !empty($image_style)) {
    $image['style_name'] = $image_style;
    $slide = theme_image_style($image);
  } else {
    $slide = theme_image($image);
  }

  // Add image title as captions if setting is enabled.
  if ($captions) {
    $slide .= '<span class="slick-slide-caption">' . $image['title'] . '</span>';
  }

  // Link Widget support
  if (isset($item['link_url']) && isset($item['link_attributes'])) {
    $link_url = $item['link_url'];
    $link_options = array(
      'attributes' => $item['link_attributes'],
      'html' => TRUE,
    );

    if (!empty($link_url) && !empty($link_options)) {
      // Print the image wrapped in a link.
      print l($slide, $link_url, $link_options);
    }
  } else {
    // Just print the default image
    print $slide;
  }
}
