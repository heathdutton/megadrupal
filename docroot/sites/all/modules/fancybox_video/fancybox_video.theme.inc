<?php

/**
 * @file
 * Theme and preprocess functions for fancybox_video module.
 */

/**
 * Formats and displays a fancyBox video field.
 *
 * @param array $variables
 *   The template variables array.
 */
function theme_fancybox_video_formatter($variables) {
  $item = $variables['item'];
  $entity = $variables['entity'];
  $entity_type = $variables['entity_type'];
  $field = $variables['field'];
  $settings = $variables['display_settings'];
  $uri = $item['uri'];

  // Add jquery.oembed.
  libraries_load('jquery.oembed');

  // Add custom script.
  drupal_add_js(drupal_get_path('module', 'fancybox_video') . '/fancybox_video.js');

  // Get path to local thumbnail.
  $wrapper = file_stream_wrapper_get_instance_by_uri($uri);
  if ($wrapper) {
    $thumbnail = $wrapper->getLocalThumbnailPath();
  }

  // Render thumbnail as image.
  $image = theme('image_style', array(
    'style_name' => $settings['fancybox_video_thumbnail_image_style'],
    'path' => $thumbnail,
  ));

  return theme('link', array(
    'text' => $image,
    'path' => file_create_url($item['uri']),
    'options' => array(
      'html' => TRUE,
      'attributes' => array(
        'class' => array('fancybox-video'),
        'data-width' => $settings['fancybox_video_width'],
        'data-height' => $settings['fancybox_video_height'],
      ),
    ),
  ));
}
