<?php
/**
 * @file
 * Image style hooks and defaults.
 */

/**
 * Implements hook_image_default_styles().
 *
 */
function advanced_crop_image_default_styles() {
  $styles = array();

  $styles['advanced_crop_preview'] = array(
    'label' => 'Image Cropper Preview',
    'effects' => array(
      array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 250,
          'height' => NULL,
          'upscale' => 1,
        ),
        'weight' => 0,
      ),
    ),
  );

  $styles['image_ratio_16_9'] = array(
    'name' => 'image_ratio_16_9',
    'label' => 'Image Ratio 16:9',
    'effects' => array(
      array(
        'label' => 'Advanced Scale And Crop',
        'help' => 'Scale and crop based on data provided by <em>Advanced Image Cropper</em>.',
        'effect callback' => 'advanced_crop_scale_and_crop_effect',
        'dimensions callback' => 'image_resize_dimensions',
        'form callback' => 'image_resize_form',
        'summary theme' => 'image_resize_summary',
        'module' => 'advanced_crop',
        'name' => 'advanced_crop_scale_and_crop_effect',
        'data' => array(
          'width' => 900,
          'height' => 506,
        ),
        'weight' => 1,
      ),
    ),
  );

  $styles['image_ratio_1_1'] = array(
    'name' => 'image_ratio_1_1',
    'label' => 'Image Ratio 1:1',
    'effects' => array(
      array(
        'label' => 'Advanced Scale And Crop',
        'help' => 'Scale and crop based on data provided by <em>Advanced Image Cropper</em>.',
        'effect callback' => 'advanced_crop_scale_and_crop_effect',
        'dimensions callback' => 'image_resize_dimensions',
        'form callback' => 'image_resize_form',
        'summary theme' => 'image_resize_summary',
        'module' => 'advanced_crop',
        'name' => 'advanced_crop_scale_and_crop_effect',
        'data' => array(
          'width' => 900,
          'height' => 900,
        ),
        'weight' => 1,
      ),
    ),
  );

  $styles['image_ratio_4_3'] = array(
    'name' => 'image_ratio_4_3',
    'label' => 'Image Ratio 4:3',
    'effects' => array(
      array(
        'label' => 'Advanced Scale And Crop',
        'help' => 'Scale and crop based on data provided by <em>Advanced Image Cropper</em>.',
        'effect callback' => 'advanced_crop_scale_and_crop_effect',
        'dimensions callback' => 'image_resize_dimensions',
        'form callback' => 'image_resize_form',
        'summary theme' => 'image_resize_summary',
        'module' => 'advanced_crop',
        'name' => 'advanced_crop_scale_and_crop_effect',
        'data' => array(
          'width' => 900,
          'height' => 675,
        ),
        'weight' => 1,
      ),
    ),
  );

  $styles['image_ratio_4_6'] = array(
    'name' => 'image_ratio_4_6',
    'label' => 'Image Ratio 4:6',
    'effects' => array(
      array(
        'label' => 'Advanced Scale And Crop',
        'help' => 'Scale and crop based on data provided by <em>Advanced Image Cropper</em>.',
        'effect callback' => 'advanced_crop_scale_and_crop_effect',
        'dimensions callback' => 'image_resize_dimensions',
        'form callback' => 'image_resize_form',
        'summary theme' => 'image_resize_summary',
        'module' => 'advanced_crop',
        'name' => 'advanced_crop_scale_and_crop_effect',
        'data' => array(
          'width' => 900,
          'height' => 1350,
        ),
        'weight' => 1,
      ),
    ),
  );

  return $styles;
}
