<?php

/**
 * @file
 * File formatters for Prezi presentations.
 */

/**
 * Implements hook_file_formatter_info().
 */
function media_prezi_file_formatter_info() {
  $formatters['media_prezi_presentation'] = array(
    'label' => t('Prezi Presentation'),
    'file types' => array('presentation'),
    'default settings' => array(
      'width' => 550,
      'height' => 400,
      'lock_to_path' => 0,
    ),
    'view callback' => 'media_prezi_file_formatter_presentation_view',
    'settings callback' => 'media_prezi_file_formatter_presentation_settings',
    'mime types' => array('presentation/prezi'),
  );
  $formatters['media_prezi_image'] = array(
    'label' => t('Prezi Preview Image'),
    'file types' => array('presentation'),
    'default settings' => array(
      'image_style' => '',
    ),
    'view callback' => 'media_prezi_file_formatter_image_view',
    'settings callback' => 'media_prezi_file_formatter_image_settings',
    'mime types' => array('presentation/prezi'),
  );
  return $formatters;
}

/**
 * Implements hook_file_formatter_FORMATTER_view().
 */
function media_prezi_file_formatter_presentation_view($file, $display, $langcode) {
  $scheme = file_uri_scheme($file->uri);

  if ($scheme == 'prezi') {
    $element = array(
      '#theme' => 'media_prezi_presentation',
      '#uri' => $file->uri,
      '#options' => array(),
    );

    // Fake a default for attributes so the ternary doesn't choke.
    $display['settings']['attributes'] = array();
    foreach (array(
               'width',
               'height',
               'lock_to_path',
               'attributes',
             ) as $setting) {
      $element['#options'][$setting] = isset($file->override[$setting]) ? $file->override[$setting] : $display['settings'][$setting];
    }
    return $element;
  }
}

/**
 * Implements hook_file_formatter_FORMATTER_settings().
 */
function media_prezi_file_formatter_presentation_settings($form, &$form_state, $settings) {
  $element = array();

  $element['width'] = array(
    '#title' => t('Width'),
    '#type' => 'textfield',
    '#default_value' => $settings['width'],
    '#element_validate' => array('media_prezi_validate_presentation_width_and_height'),
  );
  $element['height'] = array(
    '#title' => t('Height'),
    '#type' => 'textfield',
    '#default_value' => $settings['height'],
    '#element_validate' => array('media_prezi_validate_presentation_width_and_height'),
  );

  // @see http://developer.prezi.com/
  $element['lock_to_path'] = array(
    '#title' => t('lock to path'),
    '#type' => 'radios',
    '#default_value' => $settings['lock_to_path'],
    '#options' => array(
      '0' => t('Let viewers pan & zoom freely'),
      '1' => t('Constrain to simple back and forward steps'),
    ),
    '#states' => array(
      'invisible' => array(
        ':input[name="displays[media_prezi_presentation][settings][lock_to_path]"]' => array('checked' => FALSE),
      ),
    ),
  );
  return $element;
}

/**
 * Implements hook_file_formatter_FORMATTER_view().
 */
function media_prezi_file_formatter_image_view($file, $display, $langcode) {
  $scheme = file_uri_scheme($file->uri);

  if ($scheme == 'prezi') {
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
}

/**
 * Implements hook_file_formatter_FORMATTER_settings().
 */
function media_prezi_file_formatter_image_settings($form, &$form_state, $settings) {
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
 * Validation for width and height.
 */
function media_prezi_validate_presentation_width_and_height($element, &$form_state, $form) {
  // Check if the value is a number with an optional decimal or percentage sign.
  if (!empty($element['#value']) && !preg_match('/^(auto|([0-9]*(\.[0-9]+)?%?))$/', $element['#value'])) {
    form_error($element, t("The value entered for @dimension is invalid. Please insert a unitless integer for pixels, a percent, or 'auto'. Note that percent and auto may not function correctly depending on the browser and doctype.", array('@dimension' => $element['#title'])));
  }
}
