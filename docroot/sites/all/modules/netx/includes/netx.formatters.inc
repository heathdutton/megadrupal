<?php

/**
 * @file
 * Formatters for Media: NetX.
 */

/**
 * Implements hook_file_formatter_info().
 */
function netx_file_formatter_info() {
  // Renders previews for all NetX files.
  $formatters['netx_preview'] = array(
    'label' => t('NetX Preview Image'),
    'default settings' => array(
      'image_style' => '',
    ),
    'view callback' => 'netx_file_formatter_netx_preview_view',
    'settings callback' => 'netx_file_formatter_netx_preview_settings',
  );
  // Renders images.
  $formatters['netx_image'] = array(
    'label' => t('NetX Image'),
    'file types' => array('image'),
    'default settings' => array(
      // Original image by default.
      'image_style' => '',
      // No link as image module does.
      'image_link' => '',
      // Set of attribute names to display.
      'attributes' => array(),
    ),
    'view callback' => 'netx_file_formatter_netx_image_view',
    'settings callback' => 'netx_file_formatter_netx_image_settings',
  );
  // Renders video files.
  $formatters['netx_video'] = array(
    'label' => t('NetX Video'),
    'file types' => array('video'),
    'default settings' => array(
      'display_type' => 'original',
    ),
    'view callback' => 'netx_file_formatter_netx_any_view',
    // @todo create callback when needed.
    // 'settings callback' => 'netx_file_formatter_netx_audio_settings',
  );
  $formatters['netx_audio'] = array(
    'label' => t('NetX Audio'),
    'file types' => array('audio'),
    'default settings' => array(
      'display_type' => 'original',
    ),
    'view callback' => 'netx_file_formatter_netx_any_view',
    // @todo create callback when needed.
    // 'settings callback' => 'netx_file_formatter_netx_video_settings',
  );
  $formatters['netx_document'] = array(
    'label' => t('NetX Document'),
    'file types' => array('document'),
    'default settings' => array(
      'display_type' => 'original',
    ),
    'view callback' => 'netx_file_formatter_netx_any_view',
    // @todo create callback when needed.
    // 'settings callback' => 'netx_file_formatter_netx_document_settings',
  );

  return $formatters;
}

/**
 * Implements hook_file_formatter_FORMATTER_view().
 */
function netx_file_formatter_netx_preview_view($file, $display, $langcode) {
  $scheme = file_uri_scheme($file->uri);
  if ($scheme !== 'netx') {
    // Allow other formatters to take care own files.
    return;
  }
  // @codingStandardsIgnoreStart
  /** @var \MediaNetXStreamWrapper $wrapper */
  // @codingStandardsIgnoreEnd
  $wrapper = file_stream_wrapper_get_instance_by_uri($file->uri);
  $path = $wrapper->getLocalPreviewPath($file->metadata['previewUrl']);

  $image_style = $display['settings']['image_style'];
  $valid_image_styles = image_style_options(FALSE);
  if (empty($image_style) || !isset($valid_image_styles[$image_style])) {
    $element = array(
      '#theme' => 'image',
      '#path' => $path,
    );
  }
  else {
    $element = array(
      '#theme' => 'image_style',
      '#style_name' => $image_style,
      '#path' => $path,
      '#alt' => isset($file->override['attributes']['alt']) ? $file->override['attributes']['alt'] : $file->filename,
    );
  }

  return $element;
}

/**
 * Implements hook_file_formatter_FORMATTER_settings().
 */
function netx_file_formatter_netx_preview_settings($form, &$form_state, $settings) {
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
function netx_file_formatter_netx_image_view($file, $display, $langcode) {
  $scheme = file_uri_scheme($file->uri);
  if ($scheme !== 'netx') {
    // Allow other formatters to take care own files.
    return;
  }
  // @codingStandardsIgnoreStart
  /** @var \MediaNetXStreamWrapper $wrapper */
  // @codingStandardsIgnoreEnd
  $wrapper = file_stream_wrapper_get_instance_by_uri($file->uri);
  $path = $wrapper->getLocalImagePath();

  // Check if the formatter involves a link.
  if ($display['settings']['image_link'] == 'file') {
    $link_file = TRUE;
  }

  if (isset($link_file)) {
    $uri = array(
      'path' => file_create_url($path),
      'options' => array(),
    );
  }
  // Image formatter works with local files only.
  $file->uri = $path;
  $element['image'] = array(
    // Media module defends on core image module.
    '#theme' => 'image_formatter',
    '#item' => (array) $file,
    '#image_style' => $display['settings']['image_style'],
    '#path' => isset($uri) ? $uri : '',
  );
  $attributes_settings = array_filter($display['settings']['attributes']);
  // Skip render if no attributes allowed.
  if ($attributes_settings) {
    $element['netx'] = array(
      '#theme' => 'netx_attributes',
      '#netx' => $file->metadata,
      '#settings' => $attributes_settings,
    );
  }

  return $element;
}

/**
 * Implements hook_file_formatter_FORMATTER_settings().
 */
function netx_file_formatter_netx_image_settings($form, &$form_state, $settings) {
  $element['image_style'] = array(
    '#title' => t('Image style'),
    '#type' => 'select',
    '#options' => image_style_options(FALSE),
    '#default_value' => $settings['image_style'],
    '#empty_option' => t('None (original image)'),
  );
  $element['image_link'] = array(
    '#title' => t('Link image to'),
    '#type' => 'select',
    '#default_value' => $settings['image_link'],
    '#empty_option' => t('Nothing'),
    '#options' => array(
      'file' => t('File'),
    ),
  );
  $options = array();
  foreach (netx_get_attribues() as $attribute) {
    // There's only "label" as string and "value" as int available.
    $options[$attribute['label']] = $attribute['label'];
  }
  $element['attributes'] = array(
    '#type' => 'checkboxes',
    '#default_value' => $settings['attributes'],
    '#options' => $options,
    '#title' => t('Display attributes'),
  );

  return $element;
}

/**
 * Implements hook_file_formatter_FORMATTER_view().
 */
function netx_file_formatter_netx_any_view($file, $display, $langcode) {
  $scheme = file_uri_scheme($file->uri);
  if ($scheme !== 'netx') {
    // Allow other formatters to take care own files.
    return;
  }

  // Override mime-type to allow default video/audio.
  // For URL file_entity uses $wrapper->getExternalUrl() call.
  $file->filemime = DrupalLocalStreamWrapper::getMimeType($file->uri);
}


/**
 * Implements hook_file_default_displays_alter().
 */
function netx_file_default_displays_alter(&$file_displays) {
  $mappings = array(
    'preview' => 'media_thumbnail',
    'teaser' => 'medium',
    'default' => '',
  );
  foreach ($mappings as $mode => $setting) {
    $name = 'image__' . $mode . '__netx_image';
    $file_display = new stdClass();
    $file_display->api_version = 1;
    $file_display->name = $name;
    $file_display->weight = 0;
    $file_display->status = TRUE;
    $file_display->settings = array(
      'image_style' => $setting,
    );
    $file_displays[$name] = $file_display;
  }
  // Configure preview display mode for default file types.
  $default_types = array('image', 'video', 'audio', 'document');
  foreach ($default_types as $type) {
    // Set previews to media defined.
    $name = $type . '__preview__netx_preview';
    $file_display = new stdClass();
    $file_display->api_version = 1;
    $file_display->name = $name;
    // Lower weight to fire first.
    $file_display->weight = -10;
    $file_display->status = TRUE;
    $file_display->settings = array(
      'image_style' => 'media_thumbnail',
    );
    $file_displays[$name] = $file_display;
  }
  // Set formatter for default to enable mime-type rewrite.
  $default_types = array('video', 'audio', 'document');
  foreach ($default_types as $type) {
    // Set teaser to media defined.
    $name = $type . '__teaser__netx_' . $type;
    $file_display = new stdClass();
    $file_display->api_version = 1;
    $file_display->name = $name;
    // Lower weight to fire first.
    $file_display->weight = -10;
    $file_display->status = TRUE;
    $file_display->settings = array();
    $file_displays[$name] = $file_display;
    // Set default to media defined.
    $name = $type . '__default__netx_' . $type;
    $file_display = new stdClass();
    $file_display->api_version = 1;
    $file_display->name = $name;
    // Lower weight to fire first.
    $file_display->weight = -10;
    $file_display->status = TRUE;
    $file_display->settings = array();
    $file_displays[$name] = $file_display;
  }
}
