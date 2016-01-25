<?php

/**
 * @file
 * The default options available with Views Slideshow: j360.
 */

/**
 * Implements hook_views_slideshow_slideshow_info().
 */
function views_slideshow_j360_views_slideshow_slideshow_info() {
  $options = array(
    'views_slideshow_j360' => array(
      'name' => t('j360'),
      'accepts' => array(
        'goToSlide',
        'nextSlide',
        'previousSlide',
      ),
      'calls' => array(
        'goToSlide',
        'nextSlide',
        'previousSlide',
      ),
    ),
  );
  return $options;
}

/**
 * Implements views_slideshow_slideshow_type_form().
 */
function views_slideshow_j360_views_slideshow_slideshow_type_form(&$form, &$form_state, &$view) {
  if (!views_slideshow_j360_spritespin_path()) {
    $link = l(t('Download Spritespin'),
        'http://www.mauromascia.com/files/spritespin-j360.tar.gz',
        array('attributes' => array('target' => '_blank'))
    );
    drupal_set_message(t('You must install the spritespin plugin. You can find it at @website', array('@website' => $link)), 'error');
  }

  // Create the form elements for our j360 view options.
  // For the complete list of available options see:
  // http://spritespin.ginie.eu/api.html
  $form['views_slideshow_j360'] = array(
    '#type' => 'fieldset',
    '#title' => t('j360 options'),
    '#description' => t("These options affect the slideshow when it's in the j360 mode."),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['views_slideshow_j360']['source_type'] = array(
    '#type' => 'radios',
    '#title' => t('Input image type'),
    '#description' => t('You can choose here which type of image you would like to use between single (make sure that the view "order by" is correct), inline or grid sprite, panorama.'),
    '#default_value' => $view->options['views_slideshow_j360']['source_type'],
    '#options' => array(
      'image-array' => t('Single images'),
      'inline' => t('Inline spritesheet'),
      'grid' => t('Grid spritesheet'),
      'panorama' => t('Panorama'),
    ),
  );

  $form['views_slideshow_j360']['animate'] = array(
    '#type' => 'checkbox',
    '#title' => t('Animate'),
    '#description' => t('If checked runs animation on startup'),
    '#default_value' => $view->options['views_slideshow_j360']['animate'],
  );

  $form['views_slideshow_j360']['frametime'] = array(
    '#type' => 'textfield',
    '#title' => t('Frame Time'),
    '#description' => t('Time between frame updates. Changing this will modify the animation speed (higher is slower).'),
    '#default_value' => $view->options['views_slideshow_j360']['frametime'],
  );

  $form['views_slideshow_j360']['loop'] = array(
    '#type' => 'checkbox',
    '#title' => t('Loop'),
    '#description' => t('If checked repeat animation in a loop'),
    '#default_value' => $view->options['views_slideshow_j360']['loop'],
  );

  // INLINE & GRID ONLY!
  $form['views_slideshow_j360']['frames'] = array(
    '#type' => 'textfield',
    '#title' => t('Frames'),
    '#description' => t('This will set the number of frames. Only available on <b>INLINE</b> and <b>GRID</b> image source.'),
    '#default_value' => $view->options['views_slideshow_j360']['frames'],
  );

  // GRID ONLY!
  $form['views_slideshow_j360']['framesx'] = array(
    '#type' => 'textfield',
    '#title' => t('Frames X'),
    '#description' => t('This will set the number of frames in a single row. Only available on <b>GRID</b> image source.'),
    '#default_value' => $view->options['views_slideshow_j360']['framesx'],
  );

  // PANORAMA ONLY!
  $form['views_slideshow_j360']['framesstepx'] = array(
    '#type' => 'textfield',
    '#title' => t('Frames Step X'),
    '#description' => t('This will set the distance in X direction to the next frame. Only available on <b>PANORAMA</b> image source.'),
    '#default_value' => $view->options['views_slideshow_j360']['framesstepx'],
  );

  $form['views_slideshow_j360']['maxwidth'] = array(
    '#type' => 'textfield',
    '#title' => t('Max Width'),
    '#description' => t('[Integer] This will set the max width (default "450" calculated in pixel). Only available on <b>PANORAMA</b> image source.'),
    '#default_value' => $view->options['views_slideshow_j360']['maxwidth'],
  );
}

/**
 * Implements hook_views_slideshow_option_definition().
 */
function views_slideshow_j360_views_slideshow_option_definition() {
  // Set default options.
  $options['views_slideshow_j360'] = array(
    'contains' => array(
      'source_type' => array('default' => 'image-array'),
      'animate' => array('default' => FALSE),
      'frametime' => array('default' => ''),
      'loop' => array('default' => FALSE),
      'frames' => array('default' => ''),
      'framesx' => array('default' => ''),
      'framesstepx' => array('default' => ''),
      'maxwidth' => array('default' => 450),
    ),
  );

  return $options;
}
