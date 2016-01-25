<?php

/**
 * File formatters.
 */
function sky_field_file_formatter_info() {
  $items = array();

  $items['sky_field_file_link'] = array(
    '#title' => t('Link'),
    '#formatter' => 'sky_field_file_link_formatter',
  );
  $items['sky_field_file_image'] = array(
    '#title' => t('Image'),
    '#formatter' => 'sky_field_file_image_formatter',
  );
  $items['sky_field_file_audio'] = array(
    '#title' => t('Audio'),
    '#formatter' => 'sky_field_file_audio_formatter',
  );
  $items['sky_field_file_video'] = array(
    '#title' => t('Video'),
    '#formatter' => 'sky_field_file_video_formatter',
  );

  return $items;
}

/**
 * Link formatter.
 *
 * @param array $field
 */
function sky_field_file_link_formatter($field) {
  $output = NULL;

  if (!empty($field['value'])) {
    $file = file_load($field['value']);
    $url = file_create_url($file->uri);
    $output = l($file->filename, $url);
  }

  return $output;
}

/**
 * Image formatter.
 *
 * @param array $field
 */
function sky_field_file_image_formatter($field) {
  $output = NULL;

  if (!empty($field['value'])) {
    $file = file_load($field['value']);
    $url = file_create_url($file->uri);
    if (empty($field['settings']['image_style']) ||
      $field['settings']['image_style'] == 'original') {
      $output = theme('image', array('path' => $url));
    }
    else {
      $output = theme('image_style',
        array('style_name' => $field['settings']['image_style'], 'path' => $file->uri));
    }
  }

  return $output;
}

/**
 * Audio formatter.
 *
 * @param array $field
 */
function sky_field_file_audio_formatter($field) {
  $output = NULL;

  if (!empty($field['value'])) {
    $file = file_load($field['value']);
    $url = file_create_url($file->uri);
    $width = empty($field['settings']['size']['width']) ? SKY_FIELD_FILE_DEFAULT_WIDTH :
      $field['settings']['size']['width'];
    $height = empty($field['settings']['size']['height']) ? SKY_FIELD_FILEEFAULT_HEIGHT :
      $field['settings']['size']['height'];
    $output = '<audio width="' . $width . '" height="' . $height . '" id="sky_field_file_audio_' .
      $field['value'] . '" src="' . $url . '"></audio>';
  }

  return $output;
}

/**
 * Video formatter.
 *
 * @param array $field
 */
function sky_field_file_video_formatter($field) {
  $output = NULL;

  if (!empty($field['value'])) {
    $file = file_load($field['value']);
    $url = file_create_url($file->uri);
    $width = empty($field['settings']['size']['width']) ? SKY_FIELD_FILE_DEFAULT_WIDTH :
      $field['settings']['size']['width'];
    $height = empty($field['settings']['size']['height']) ? SKY_FIELD_FILEEFAULT_HEIGHT :
      $field['settings']['size']['height'];
    $output = '<video width="' . $width . '" height="' . $height . '" id="sky_field_file_video_' .
      $field['value'] . '" src="' . $url . '"></video>';
  }

  return $output;
}