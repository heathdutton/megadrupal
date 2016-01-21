<?php

/**
 * @file
 * File formatters for SoundCloud audio.
 */

/**
 * Implements hook_file_formatter_info().
 */
function media_soundcloud_file_formatter_info() {
  $formatters['media_soundcloud_audio'] = array(
    'label' => t('SoundCloud Audio'),
    'file types' => array('audio'),
    'default settings' => array(
      'width' => '100%',
      'height' => 450,
      'api' => FALSE,
      'auto_play' => FALSE,
      'visual' => TRUE,
      'show_artwork' => TRUE,
      'show_comments' => TRUE,
      'color' => NULL,
      'protocol' => 'https://',
      'protocol_specify' => FALSE,
    ),
    'view callback' => 'media_soundcloud_file_formatter_audio_view',
    'settings callback' => 'media_soundcloud_file_formatter_audio_settings',
    'mime types' => array('audio/soundcloud'),
  );

  $formatters['media_soundcloud_image'] = array(
    'label' => t('SoundCloud Preview Image'),
    'file types' => array('audio'),
    'default settings' => array(
      'image_style' => '',
    ),
    'view callback' => 'media_soundcloud_file_formatter_image_view',
    'settings callback' => 'media_soundcloud_file_formatter_image_settings',
    'mime types' => array('audio/soundcloud'),
  );

  return $formatters;
}

/**
 * Implements hook_file_formatter_FORMATTER_view().
 */
function media_soundcloud_file_formatter_audio_view($file, $display, $langcode) {
  $scheme = file_uri_scheme($file->uri);

  if ($scheme == 'soundcloud') {
    $element = array(
      '#theme' => 'media_soundcloud_audio',
      '#uri' => $file->uri,
      '#options' => array(),
    );

    // Fake a default for attributes so the ternary doesn't choke.
    $display['settings']['attributes'] = array();

    foreach (array('width', 'height', 'api', 'auto_play', 'show_comments', 'show_artwork', 'color', 'visual', 'protocol', 'protocol_specify', 'attributes') as $setting) {
      $element['#options'][$setting] = isset($file->override[$setting]) ? $file->override[$setting] : $display['settings'][$setting];
    }

    return $element;
  }
}

/**
 * Implements hook_file_formatter_FORMATTER_settings().
 */
function media_soundcloud_file_formatter_audio_settings($form, &$form_state, $settings) {
  $element = array();

  $element['width'] = array(
    '#title' => t('Width'),
    '#type' => 'textfield',
    '#default_value' => $settings['width'],
    '#element_validate' => array('_soundcloud_validate_audio_width_and_height'),
  );
  $element['height'] = array(
    '#title' => t('Height'),
    '#type' => 'textfield',
    '#default_value' => $settings['height'],
    '#element_validate' => array('_soundcloud_validate_audio_width_and_height'),
  );

  $element['color'] = array(
    '#title' => t('Custom audio controls color'),
    '#type' => 'textfield',
    '#default_value' => $settings['color'],
    '#description' => t('A 3 or 6 character hex color code'),
    '#maxlength' => 7,
    '#size' => 7,
    '#element_validate' => array('_soundcloud_validate_audio_color'),
  );
  $element['protocol_specify'] = array(
    '#title' => t('Specify an http protocol'),
    '#type' => 'checkbox',
    '#default_value' => $settings['protocol_specify'],
    '#description' => t('An explicit protocol may be neccesary for videos embedded in RSS feeds and emails. If no protocol is specified, iframes will be protocol relative.'),
  );
  $element['protocol'] = array(
    '#title' => t('Iframe protocol'),
    '#type' => 'radios',
    '#default_value' => $settings['protocol'],
    '#options' => array(
      'http://' => 'HTTP',
      'https://' => 'HTTPS',
    ),
    '#states' => array(
      'invisible' => array(
        ':input[name="displays[media_soundcloud_audio][settings][protocol_specify]"]' => array('checked' => FALSE),
      ),
    ),
  );

  $element['auto_play'] = array(
    '#title' => t('Autoplay audio on load'),
    '#type' => 'checkbox',
    '#default_value' => $settings['auto_play'],
  );
  $element['visual'] = array(
    '#title' => t('Use the visual player'),
    '#type' => 'checkbox',
    '#default_value' => $settings['visual'],
  );
  $element['show_artwork'] = array(
    '#title' => t('Show artwork'),
    '#type' => 'checkbox',
    '#default_value' => $settings['show_artwork'],
  );
  $element['show_comments'] = array(
    '#title' => t('Show comments'),
    '#type' => 'checkbox',
    '#default_value' => $settings['show_comments'],
  );
  $element['api'] = array(
    '#title' => t('Enable the <a href="@url">Javascript API</a>', array('@url' => 'http://developers.soundcloud.com/docs/api/html5-widget')),
    '#type' => 'checkbox',
    '#default_value' => $settings['api'],
    '#description' => t('A required id in the format <code>media-soundcloud-{audio_id}</code> will be added to each iframe.'),
  );

  return $element;
}

/**
 * Validation for width and height.
 */
function _soundcloud_validate_audio_width_and_height($element, &$form_state, $form) {
  // Check if the value is a number with an optional decimal or percentage sign, or "auto".
  if (!empty($element['#value']) && !preg_match('/^(auto|([0-9]*(\.[0-9]+)?%?))$/', $element['#value'])) {
    form_error($element, t("The value entered for @dimension is invalid. Please insert a unitless integer for pixels, a percent, or \"auto\". Note that percent and auto may not function correctly depending on the browser and doctype.", array('@dimension' => $element['#title'])));
  }
}

/**
 * Validation for color.
 */
function _soundcloud_validate_audio_color($element, &$form_state, $form) {
  // If the value is set, check if it's a hex color with or without '#'.
  if (!empty($element['#value']) && !preg_match('/^#?([0-9a-fA-F]{3}|[0-9a-fA-F]{6})$/', $element['#value'])) {
    form_error($element, t("The color value is invalid. Please use a 3 or 6 character hex color code.", array('@dimension' => $element['#title'])));
  }
}

/**
 * Implements hook_file_formatter_FORMATTER_view().
 */
function media_soundcloud_file_formatter_image_view($file, $display, $langcode) {
  $scheme = file_uri_scheme($file->uri);

  if ($scheme == 'soundcloud') {
    $wrapper = file_stream_wrapper_get_instance_by_uri($file->uri);
    $image_style = $display['settings']['image_style'];
    $valid_image_styles = image_style_options(FALSE);

    if (empty($image_style) || !isset($valid_image_styles[$image_style])) {
      $element = array(
        '#theme' => 'image',
        '#path' => $wrapper->getLocalThumbnailPath(),
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
}

/**
 * Implements hook_file_formatter_FORMATTER_settings().
 */
function media_soundcloud_file_formatter_image_settings($form, &$form_state, $settings) {
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
