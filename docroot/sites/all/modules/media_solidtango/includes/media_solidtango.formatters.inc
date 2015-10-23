<?php

/**
 * @file
 * File formatters for Solidtango videos.
 *
 * @todo Cleanup of non-working stuff.
 */

/**
 * Implements hook_file_formatter_info().
 */
function media_solidtango_file_formatter_info() {
  $formatters['media_solidtango_video'] = array(
    'label' => t('Solidtango Video'),
    'file types' => array('video'),
    'default settings' => array(
      'width' => 640,
      'height' => 360,
      'preview_uri' => 'solidtango://host/example.solidtango.com/type/video/slug/xyz1a23b',
      'api' => 0,
      'autoplay' => 0,
      'byline' => 1,
      'color' => NULL,
      'loop' => 0,
      'portrait' => 1,
      'title' => 1,
      'protocol' => 'https://',
      'protocol_specify' => FALSE,
    ),
    'view callback' => 'media_solidtango_file_formatter_video_view',
    'settings callback' => 'media_solidtango_file_formatter_video_settings',
    'mime types' => array('video/solidtango'),
  );

  $formatters['media_solidtango_image'] = array(
    'label' => t('Solidtango Preview Image'),
    'file types' => array('video'),
    'default settings' => array(
      'image_style' => '',
    ),
    'view callback' => 'media_solidtango_file_formatter_image_view',
    'settings callback' => 'media_solidtango_file_formatter_image_settings',
    'mime types' => array('video/solidtango'),
  );

  return $formatters;
}

/**
 * Implements hook_file_formatter_FORMATTER_view().
 */
function media_solidtango_file_formatter_video_view($file, $display, $langcode) {
  $scheme = file_uri_scheme($file->uri);

  if ($scheme == 'solidtango') {
    $element = array(
      '#theme' => 'media_solidtango_video',
      '#uri' => $file->uri,
      '#options' => array(),
    );

    // Fake a default for attributes so the ternary doesn't choke.
    $display['settings']['attributes'] = array();

    $settings_array = array(
      'width',
      'height',
      'api',
      'autoplay',
      'byline',
      'color',
      'loop',
      'portrait',
      'title',
      'protocol',
      'protocol_specify',
      'attributes',
      'enablejsapi',
      'origin',
    );
    foreach ($settings_array as $setting) {
      if (isset($file->override[$setting])) {
        $element['#options'][$setting] = $file->override[$setting];
      }
      else {
        $element['#options'][$setting] = $display['settings'][$setting];
      }
    }

    return $element;
  }
}

/**
 * Implements hook_file_formatter_FORMATTER_settings().
 */
function media_solidtango_file_formatter_video_settings($form, &$form_state, $settings) {
  $element = array();

  $element['width'] = array(
    '#title' => t('Width'),
    '#type' => 'textfield',
    '#default_value' => $settings['width'],
    '#element_validate' => array('media_solidtango_validate_video_width_and_height'),
  );
  $element['height'] = array(
    '#title' => t('Height'),
    '#type' => 'textfield',
    '#default_value' => $settings['height'],
    '#element_validate' => array('media_solidtango_validate_video_width_and_height'),
  );

  // Multiple and string options.
  $element['color'] = array(
    '#title' => t('Custom video controls color'),
    '#type' => 'textfield',
    '#default_value' => $settings['color'],
    '#description' => t('A 3 or 6 character hex color code'),
    '#maxlength' => 7,
    '#size' => 7,
    '#element_validate' => array('media_solidtango_validate_video_color'),
  );
  $element['protocol_specify'] = array(
    '#title' => t('Specify an http protocol'),
    '#type' => 'checkbox',
    '#default_value' => $settings['protocol_specify'],
    '#description' => t('An explicit protocol may be necessary for videos embedded in RSS feeds and emails. If no protocol is specified, iframes will be protocol relative.'),
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
        ':input[name="displays[media_solidtango_video][settings][protocol_specify]"]' => array('checked' => FALSE),
      ),
    ),
  );

  // Single options.
  // Note: make sure the positive/negative language lines up with option
  // processing in media_solidtango.theme.inc.
  $element['autoplay'] = array(
    '#title' => t('Autoplay video on load'),
    '#type' => 'checkbox',
    '#default_value' => $settings['autoplay'],
  );
  $element['loop'] = array(
    '#title' => t('Loop video'),
    '#type' => 'checkbox',
    '#default_value' => $settings['loop'],
  );
  $element['title'] = array(
    '#title' => t('Show the video title'),
    '#type' => 'checkbox',
    '#default_value' => $settings['title'],
  );
  $element['byline'] = array(
    '#title' => t('Show the video byline'),
    '#type' => 'checkbox',
    '#default_value' => $settings['byline'],
  );
  $element['portrait'] = array(
    '#title' => t("Show the user's portrait"),
    '#type' => 'checkbox',
    '#default_value' => $settings['portrait'],
  );
  
  // JS api.
  $element['enablejsapi'] = array(
    '#title' => t('Enable the Embed API'),
    '#type' => 'checkbox',
    '#default_value' => $settings['enablejsapi'],
    '#description' => 'An id in the format <code>media-solidtango-{video_id}</code> will be added to each iframe.',
  );
  $element['origin'] = array(
    '#title' => t('Origin'),
    '#type' => 'textfield',
    '#description' => t('If the Javascript API is enabled, enter your site\'s domain for added security. Please insert a valid domain in the format https://yoursubdomain.solidtango.com.'),
    '#default_value' => $settings['origin'],
    '#states' => array(
      'invisible' => array(
        ':input[name="displays[media_solidtango_video][settings][enablejsapi]"]' => array('checked' => FALSE),
      ),
    ),
    '#element_validate' => array('_solidtango_validate_jsapi_domain'),
  );
  return $element;
}

/**
 * Validation for width and height.
 */
function media_solidtango_validate_video_width_and_height($element, &$form_state, $form) {
  // Check if the value is a number with an optional decimal, % or "auto".
  if (!empty($element['#value']) && !preg_match('/^(auto|([0-9]*(\.[0-9]+)?%?))$/', $element['#value'])) {

    $error_msg = t('The value entered for @dimension is invalid. Please insert a unitless integer for pixels, a percent, or "auto". Note that percent and auto may not function correctly depending on the browser and doctype.', array('@dimension' => $element['#title']));
    form_error($element, $error_msg);
  }
}

/**
 * Validation for color.
 */
function media_solidtango_validate_video_color($element, &$form_state, $form) {
  // If the value is set, check if it's a hex color with or without '#'.
  if (!empty($element['#value']) && !preg_match('/^#?([0-9a-fA-F]{3}|[0-9a-fA-F]{6})$/', $element['#value'])) {
    form_error($element, t("The color value is invalid. Please use a 3 or 6 character hex color code."));
  }
}

/**
 * Implements hook_file_formatter_FORMATTER_view().
 */
function media_solidtango_file_formatter_image_view($file, $display, $langcode) {
  $scheme = file_uri_scheme($file->uri);

  if ($scheme == 'solidtango') {
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
function media_solidtango_file_formatter_image_settings($form, &$form_state, $settings) {
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
 * Validation for Js API Origin.
 */
function _solidtango_validate_jsapi_domain ($element, &$form_state, $form) {

  // Check if the value is a url with http/s and no trailing directories.
  if (!empty($element['#value']) && !preg_match('/^https?\:\/\/[a-zA-Z0-9\-\.]+\.([a-zA-Z]{2,4}){1,2}$/', $element['#value'])) {
    form_error($element, t('Please insert a valid domain in the format https://yoursubdomain.solidtango.com'));
  }
}
