<?php

/**
 * @file
 * File formatters for Vine videos.
 */

/**
 * Implements hook_file_formatter_info().
 */
function media_vine_file_formatter_info() {
  $formatters['media_vine_video'] = array(
    'label' => t('Vine Video'),
    'file types' => array('video'),
    'default settings' => array(
      'width' => 600,
      'height' => 600,
      'embed_format' => 'simple',
      'autoplay_audio' => 0,
    ),
    'view callback' => 'media_vine_file_formatter_video_view',
    'settings callback' => 'media_vine_file_formatter_video_settings',
    'mime types' => array('video/vine'),
  );

  $formatters['media_vine_image'] = array(
    'label' => t('Vine Preview Image'),
    'file types' => array('video'),
    'default settings' => array(
      'image_style' => '',
    ),
    'view callback' => 'media_vine_file_formatter_image_view',
    'settings callback' => 'media_vine_file_formatter_image_settings',
    'mime types' => array('video/vine'),
  );

  return $formatters;
}

/**
 * Implements hook_file_formatter_FORMATTER_view().
 */
function media_vine_file_formatter_video_view($file, $display) {
  $scheme = file_uri_scheme($file->uri);

  if ($scheme == 'vine') {
    $element = array(
      '#theme' => 'media_vine_video',
      '#uri' => $file->uri,
      '#options' => array(),
    );

    // Fake a default for attributes so the ternary doesn't choke.
    $display['settings']['attributes'] = array();

    $fields = array(
      'width',
      'height',
      'embed_format',
      'autoplay_audio',
      'attributes',
    );

    foreach ($fields as $setting) {
      $element['#options'][$setting] = isset($file->override[$setting]) ? $file->override[$setting] : $display['settings'][$setting];
    }

    return $element;
  }
}

/**
 * Implements hook_file_formatter_FORMATTER_settings().
 *
 * @see https://dev.twitter.com/web/vine/embed
 */
function media_vine_file_formatter_video_settings($form, &$form_state, $settings) {
  $element = array();

  $element['width'] = array(
    '#title' => t('Width'),
    '#type' => 'textfield',
    '#default_value' => $settings['width'],
  );
  $element['height'] = array(
    '#title' => t('Height'),
    '#type' => 'textfield',
    '#default_value' => $settings['height'],
  );

  $element['embed_format'] = array(
    '#title' => t('Specify embed format'),
    '#type' => 'select',
    '#options' => array(
      'simple' => t('Simple'),
      'postcard' => t('Postcard'),
    ),
    '#default_value' => $settings['embed_format'],
    '#description' => t('An embed format of Vine video.'),
  );

  $element['autoplay_audio'] = array(
    '#title' => t('Autoplay audio'),
    '#type' => 'checkbox',
    '#default_value' => $settings['autoplay_audio'],
  );

  return $element;
}

/**
 * Implements hook_file_formatter_FORMATTER_view().
 */
function media_vine_file_formatter_image_view($file, $display, $langcode) {
  $scheme = file_uri_scheme($file->uri);

  // Deal only with Vine video scheme.
  if ($scheme != 'vine') {
    return;
  }

  $wrapper = file_stream_wrapper_get_instance_by_uri($file->uri);
  $image_style = $display['settings']['image_style'];
  $valid_image_styles = image_style_options(FALSE);

  if (empty($image_style) || !isset($valid_image_styles[$image_style])) {
    $element = array(
      '#theme' => 'image',
      '#path' => $wrapper->getOriginalThumbnailPath(),
      '#alt' => isset($file->override['attributes']['alt']) ? $file->override['attributes']['alt'] : $file->filename,
    );
  }
  else {
    $element = array(
      '#theme' => 'image_style',
      '#style_name' => $image_style,
      '#path' => $wrapper->getLocalThumbnailPath(),
      '#alt' => isset($file->override['attributes']['alt']) ? $file->override['attributes']['alt'] : $file->filename,
    );
  }

  return $element;
}

/**
 * Implements hook_file_formatter_FORMATTER_settings().
 */
function media_vine_file_formatter_image_settings($form, &$form_state, $settings) {
  $element = array();

  $element['image_style'] = array(
    '#title' => t('Image style'),
    '#type' => 'select',
    '#options' => image_style_options(FALSE),
    '#default_value' => $settings['image_style'],
    '#empty_option' => t('None (original image)'),
  );

  return $element;
}
