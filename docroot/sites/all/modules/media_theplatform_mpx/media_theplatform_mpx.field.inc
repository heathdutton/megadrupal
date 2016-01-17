<?php

/**
 * @file
 * Field API integration for the media_theplatform_mpx module.
 */

/**
 * Implements hook_field_formatter_info().
 */
function media_theplatform_mpx_field_formatter_info() {
  $info['media_theplatform_mpx_player_iframe'] = array(
    'label' => t('mpx video player'),
    'field types' => array('file'),
    'settings' => array(
      'width' => '',
      'height' => '',
    ),
    'file formatter' => array(
      'mime types' => array('video/mpx'),
    ),
  );

  return $info;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function media_theplatform_mpx_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $summary = array();

  if ($display['type'] == 'media_theplatform_mpx_player_iframe') {
    $summary[] = t('Width: @value', array('@value' => $settings['width'] ? $settings['width'] : t('None')));
    $summary[] = t('Height: @value', array('@value' => $settings['height'] ? $settings['height'] : t('None')));
  }

  return implode('<br/>', $summary);
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function media_theplatform_mpx_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $element = array();

  if ($display['type'] == 'media_theplatform_mpx_player_iframe') {
    $element['width'] = array(
      '#title' => t('Width'),
      '#type' => 'textfield',
      '#size' => 20,
      '#default_value' => $settings['width'],
    );
    $element['height'] = array(
      '#title' => t('Height'),
      '#type' => 'textfield',
      '#size' => 20,
      '#default_value' => $settings['height'],
    );
  }

  return $element;
}

/**
 * Implements hook_field_formatter_view().
 */
function media_theplatform_mpx_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();
  $settings = $display['settings'];

  if ($display['type'] === 'media_theplatform_mpx_player_iframe') {
    foreach ($items as $delta => $item) {
      if ($item['filemime'] === 'video/mpx') {
        $element[$delta] = array(
          '#theme' => 'media_theplatform_mpx_player_iframe',
          '#file' => (object) $item,
          '#attributes' => array_filter($settings),
        );
      }
    }
  }

  return $element;
}

/**
 * Implements hook_file_formatter_info().
 */
function media_theplatform_mpx_file_formatter_info() {
  $formatters['media_theplatform_mpx_image'] = array(
    'label' => t('mpx Preview Image'),
    'file types' => array('video'),
    'default settings' => array(
      'image_style' => '',
    ),
    'view callback' => 'media_theplatform_mpx_file_formatter_image_view',
    'settings callback' => 'media_theplatform_mpx_file_formatter_image_settings',
    'mime types' => array('video/mpx', 'player/mpx'),
  );
  return $formatters;
}

/**
 * Implements hook_file_formatter_FORMATTER_settings().
 */
function media_theplatform_mpx_file_formatter_image_settings($form, &$form_state, $settings) {
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

/**
 * Implements hook_file_formatter_FORMATTER_view().
 */
function media_theplatform_mpx_file_formatter_image_view($file, $display, $langcode) {
  if ($file->filemime === 'video/mpx' || $file->filemime === 'player/mpx') {
    /** @var MediaThePlatformMpxStreamWrapper $wrapper */
    $wrapper = file_stream_wrapper_get_instance_by_uri($file->uri);

    $image_style = $display['settings']['image_style'];
    $valid_image_styles = image_style_options(FALSE);

    if (empty($image_style) || !isset($valid_image_styles[$image_style])) {
      if ($uri = $wrapper->getOriginalThumbnailPath()) {
        return array(
          '#theme' => 'image',
          '#path' => $uri,
        );
      }
    }
    else {
      if ($uri = $wrapper->getLocalThumbnailPath()) {
        return array(
          '#theme' => 'image_style',
          '#style_name' => $image_style,
          '#path' => $uri,
        );
      }
    }
  }
}
