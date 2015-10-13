<?php

/**
 * @file
 * implemention for special image effects
 * and presets of them
 */

/**
 * Implements hook_image_default_styles().
 *
 * hook_image_default_styles() declares to Drupal any image styles that are
 * provided by the module. An image style is a collection of image effects that
 * are performed in a specified order, manipulating the image and generating a
 * new derivative image.
 *
 * This hook can be used to declare image styles that your module depends on or
 * allow you to define image styles in code and gain the benefits of using
 * a version control system.
 */
function archibald_image_default_styles() {
  // This hook returns an array, each component of which describes an image
  // style. The array keys are the machine-readable image style names and
  // to avoid namespace conflicts should begin with the name of the
  // implementing module. e.g.) 'mymodule_stylename'. Styles names should
  // use only alpha-numeric characters, underscores (_), and hyphens (-).
  $styles = array();
  $styles['archibald_details'] = array();
  $styles['archibald_details']['effects'] = array(
    array(
      'name' => 'image_scale',
      'data' => array(
        'width' => 170,
        'height' => 170,
        'upscale' => 0,
      ),
      'weight' => 0,
    ),
  );

  $styles['archibald_preview'] = array();
  $styles['archibald_preview']['effects'] = array(
    array(
      'name' => 'image_scale',
      'data' => array(
        'width' => 90,
        'height' => 90,
        'upscale' => 1,
      ),
      'weight' => 0,
    ),
  );

  $styles['archibald_contributor'] = array();
  $styles['archibald_contributor']['effects'] = array(
    array(
      'name' => 'image_scale',
      'data' => array(
        'width' => NULL,
        'height' => 16,
        'upscale' => 1,
      ),
      'weight' => 0,
    ),
  );

  return $styles;
}

/**
 * Image effect callback; Colorize an image resource.
 *
 * @param $image
 *   An image object returned by image_load().
 * @param $data
 *   An array of attributes to use when performing the colorize effect with the
 *   following items:
 *   - "color": The web-style hex color to use when colorizing the image.
 *
 * @return
 *   TRUE on success. FALSE on failure to colorize image.
 */
function archibald_colorize_effect(&$image, $data) {

  ###############   ATTENTION   ########################
  // This does NOT work with DEBIAN LENNY or lower:
  // Version: ImageMagick 6.3.7 11/16/10 Q16
  ###############   ATTENTION   ########################

  /**
   * new version of image imagick from cha
   */
  $source_file  = '/tmp/client_colorize_effect_' . uniqid();
  $dest_file    = '/tmp/client_colorize_effect_' . uniqid();
  $imageick_bin = '/usr/bin/convert';
  //if (file_exists($imageick_bin) && is_executable($imageick_bin)) {
  //casue fo openbasedir issues, we can not test it

  imagepng($image->resource, $source_file);

  $cmd = "$imageick_bin -colorspace gray " . "-sigmoidal-contrast 1/10% -fill '#{$data['color']}' " . "-tint 100 \"" . $source_file . "\" $dest_file";
  @exec($cmd);

  if (is_file($dest_file)) {
    $image->resource = imagecreatefrompng($dest_file);
    return TRUE;
  }


  // Image manipulation should be done to the $image->resource, which will be
  // automatically saved as a new image once all effects have been applied.
  // If your effect makes changes to the $image->resource that relate to any
  // information stored in the $image->info array (width, height, etc.) you
  // should update that information as well. See modules/system/image.gd.inc
  // for examples of functions that perform image manipulations.
  // Not all GD installations are created equal. It is a good idea to check for
  // the existence of image manipulation functions before using them.
  // PHP installations using non-bundled GD do not have imagefilter(). More
  // information about image manipulation functions is available in the PHP
  // manual. http://www.php.net/manual/en/book.image.php
  if (!function_exists('imagefilter')) {
    watchdog(
      'Image',
      'The image %image could not be colourized because of image filter()
        function is not available in this PHP installation.',
      array(
        '%file' => $image->source,
      )
    );

    return FALSE;
  }

  // Verify that Drupal is using the PHP GD library for image manipulations
  // since this effect depends on functions in the GD library.
  if ($image->toolkit != 'gd') {
    watchdog(
      'Image',
      'Image colourize failed on %path. Using non GD toolkit',
      array(
        '%path' => $image->source,
      ),
      WATCHDOG_ERROR
    );

    return FALSE;
  }

  // Convert short #FFF syntax to full #FFFFFF syntax.
  if (drupal_strlen($data['color']) == 4) {
    $c = $data['color'];
    $data['color'] = $c[0] . $c[1] . $c[1] . $c[2] . $c[2] . $c[3] . $c[3];
  }

  // Convert #FFFFFF syntax to hexadecimal colors.
  $data['color'] = hexdec(str_replace('#', '0x', $data['color']));

  // Convert the hexadecimal color value to a color index value.
  $rgb = array();
  for ($i = 16; $i >= 0; $i -= 8) {
    $rgb[] = (($data['color'] >> $i) & 0xFF);
  }

  // First desaturate the image, and then apply the new color.
  imagefilter($image->resource, IMG_FILTER_GRAYSCALE);
  imagefilter($image->resource, IMG_FILTER_COLORIZE, $rgb[0], $rgb[1], $rgb[2]);

  return TRUE;
}

/**
 * Implements hook_image_effect_info().
 *
 * This hook allows your module to define additional image manipulation effects
 * that can be used with image styles.
 */
function archibald_image_effect_info() {
  $effects = array();

  // The array is keyed on the machine-readable effect name.
  $effects['archibald_colorize'] = array(
    // Human readable name of the effect.
    'label' => t('Colourize'),
    // (optional) Brief description of the effect that will be shown when
    // adding or configuring this image effect.
    'help' => t('Colourizing will first remove all colours from the source image and then tint the image using the chosen colour.'),
    // Name of function called to perform this effect.
    'effect callback' => 'archibald_colorize_effect',
    // (optional) Name of function that provides a $form array with options for
    // configuring the effect. Note that you only need to return the fields
    // specific to your module. Submit buttons will be added automatically, and
    // configuration options will be serailized and added to the 'data' element
    // of the effect. The function will recieve the $effect['data'] array as
    // its only parameter.
    'form callback' => 'archibald_colorize_form',
    // (optional) Name of a theme function that will output a summary of this
    // effects configuation. Used when displaying list of effects associated
    // with an image style.
    'summary theme' => 'archibald_colorize_summary',
  );

  return $effects;
}

/**
 * Formats a summary of an image colorize effect.
 *
 * @param $variables
 *   An associative array containing:
 *   - data: The current configuration for this colorize effect.
 *
 * @ingroup themeable
 */
function theme_archibald_colorize_summary($variables) {
  $data = $variables['data'];
  return t('As colour #@color.', array('@color' => $data['color']));
}

/**
 * Form Builder; Configuration settings for colorize effect.
 *
 * Create a $form array with the fields necessary for configuring the
 * archibald_colorize effect.
 *
 * Note that this is not a complete form, it only contains the portion of the
 * form for configuring the colorize options. Therefore it does not not need to
 * include metadata about the effect, nor a submit button.
 *
 * @param $data
 *   The current configuration for this colorize effect.
 */
function archibald_colorize_form($data) {
  $form = array();
  // You do not need to worry about handling saving/updating/deleting of the
  // data collected. The image module will automatically serialize and store
  // all data associated with an effect.
  $form['color'] = array(
    '#type' => 'textfield',
    '#title' => t('Colour'),
    '#description' => t('The colour to use when colourizing the image.
      Use web-style colours. e.g.) #FF6633.'
    ),
    '#default_value' => isset($data['color']) ? $data['color'] : '',
    '#size' => 7,
    '#max_length' => 7,
    '#required' => TRUE,
  );
  return $form;
}

