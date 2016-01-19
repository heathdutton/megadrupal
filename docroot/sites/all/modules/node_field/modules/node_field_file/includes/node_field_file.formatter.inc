<?php

/**
 * File formatters.
 */
function node_field_file_formatter_info() {
  $items = array();

  $items['node_field_file_link'] = array(
    '#title' => t('Link'),
    '#formatter' => 'node_field_file_link_formatter',
  );
  $items['node_field_file_image'] = array(
    '#title' => t('Image'),
    '#formatter' => 'node_field_file_image_formatter',
  );
  $items['node_field_file_audio'] = array(
    '#title' => t('Audio'),
    '#formatter' => 'node_field_file_audio_formatter',
  );
  $items['node_field_file_video'] = array(
    '#title' => t('Video'),
    '#formatter' => 'node_field_file_video_formatter',
  );

  return $items;
}

/**
 * Link formatter.
 *
 * @param array $field
 */
function node_field_file_link_formatter($field) {
  $output = NULL;

  if (!empty($field['value'])) {
    $file = file_load($field['value']);
    if (!empty($file)) {
      $url = file_create_url($file->uri);
      $target = empty($field['settings']['link_target']) ? '_blank' : $field['settings']['link_target'];
      $options = array(
        'attributes' => array(
          'target' => $target,
          'class' => ($target == '_blank') ? array('new-window') : array(),
        )
      );
      $output = l($file->filename, $url, $options);
    }
  }

  return $output;
}

/**
 * Image formatter.
 *
 * @param array $field
 */
function node_field_file_image_formatter($field) {
  $output = NULL;

  if (!empty($field['value'])) {
    $file = file_load($field['value']);
    $url = file_create_url($file->uri);
    if (empty($field['settings']['image_style']) ||
      $field['settings']['image_style'] == 'original') {
      $output = theme('image', array('path' => $url));
    }
    else {
      $output = theme('image_style', array('style_name' => $field['settings']['image_style'], 'path' => $file->uri));
    }
  }

  return $output;
}

/**
 * Audio formatter.
 *
 * @param array $field
 */
function node_field_file_audio_formatter($field) {
  $output = '';

  if (!empty($field['value'])) {
    $fid = intval($field['value']);
    $file = file_load($fid);
    $url = file_create_url($file->uri);
    $width = NODE_FIELD_FILE_DEFAULT_WIDTH;
    if (!empty($field['settings']['size']['width'])) {
      $width = $field['settings']['size']['width'];
    }
    $height = NODE_FIELD_FILE_DEFAULT_HEIGHT;
    if (!empty($field['settings']['size']['height'])) {
      $height = $field['settings']['size']['height'];
    }

    $element = array(
      '#type' => 'html_tag',
      '#tag' => 'audio',
      '#attributes' => array(
        'width' => $width,
        'height' => $height,
        'id' => 'node_field_file_audio_' . $fid,
        'src' => $url,
      ),
    );

    $output = drupal_render($element);
  }

  return $output;
}

/**
 * Video formatter.
 *
 * @param array $field
 */
function node_field_file_video_formatter($field) {
  $output = '';

  if (!empty($field['value'])) {
    $fid = intval($field['value']);
    $file = file_load($fid);
    $url = file_create_url($file->uri);
    $width = NODE_FIELD_FILE_DEFAULT_WIDTH;
    if (!empty($field['settings']['size']['width'])) {
      $width = $field['settings']['size']['width'];
    }
    $height = NODE_FIELD_FILE_DEFAULT_HEIGHT;
    if (!empty($field['settings']['size']['height'])) {
      $height = $field['settings']['size']['height'];
    }

    $element = array(
      '#type' => 'html_tag',
      '#tag' => 'video',
      '#attributes' => array(
        'width' => $width,
        'height' => $height,
        'id' => 'node_field_file_video_' . $fid,
        'src' => $url,
      ),
    );

    $output = drupal_render($element);
  }

  return $output;
}