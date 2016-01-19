<?php
/**
 * @file
 * bendy.features.inc
 */

/**
 * Implements hook_views_api().
 */
function bendy_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_image_default_styles().
 */
function bendy_image_default_styles() {
  $styles = array();

  // Exported image style: bendy_mobile
  $styles['bendy_mobile'] = array(
    'name' => 'bendy_mobile',
    'effects' => array(
      9 => array(
        'label' => 'Scale and crop',
        'help' => 'Scale and crop will maintain the aspect-ratio of the original image, then crop the larger dimension. This is most useful for creating perfectly square thumbnails without stretching the image.',
        'effect callback' => 'image_scale_and_crop_effect',
        'dimensions callback' => 'image_resize_dimensions',
        'form callback' => 'image_resize_form',
        'summary theme' => 'image_resize_summary',
        'module' => 'image',
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => '690',
          'height' => '265',
        ),
        'weight' => '-10',
      ),
    ),
  );

  // Exported image style: bendy_mobile_small
  $styles['bendy_mobile_small'] = array(
    'name' => 'bendy_mobile_small',
    'effects' => array(
      10 => array(
        'label' => 'Scale and crop',
        'help' => 'Scale and crop will maintain the aspect-ratio of the original image, then crop the larger dimension. This is most useful for creating perfectly square thumbnails without stretching the image.',
        'effect callback' => 'image_scale_and_crop_effect',
        'dimensions callback' => 'image_resize_dimensions',
        'form callback' => 'image_resize_form',
        'summary theme' => 'image_resize_summary',
        'module' => 'image',
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => '480',
          'height' => '185',
        ),
        'weight' => '-10',
      ),
    ),
  );

  // Exported image style: bendy_narrow
  $styles['bendy_narrow'] = array(
    'name' => 'bendy_narrow',
    'effects' => array(
      8 => array(
        'label' => 'Scale and crop',
        'help' => 'Scale and crop will maintain the aspect-ratio of the original image, then crop the larger dimension. This is most useful for creating perfectly square thumbnails without stretching the image.',
        'effect callback' => 'image_scale_and_crop_effect',
        'dimensions callback' => 'image_resize_dimensions',
        'form callback' => 'image_resize_form',
        'summary theme' => 'image_resize_summary',
        'module' => 'image',
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => '450',
          'height' => '173',
        ),
        'weight' => '-10',
      ),
    ),
  );

  // Exported image style: bendy_normal
  $styles['bendy_normal'] = array(
    'name' => 'bendy_normal',
    'effects' => array(
      7 => array(
        'label' => 'Scale and crop',
        'help' => 'Scale and crop will maintain the aspect-ratio of the original image, then crop the larger dimension. This is most useful for creating perfectly square thumbnails without stretching the image.',
        'effect callback' => 'image_scale_and_crop_effect',
        'dimensions callback' => 'image_resize_dimensions',
        'form callback' => 'image_resize_form',
        'summary theme' => 'image_resize_summary',
        'module' => 'image',
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => '610',
          'height' => '235',
        ),
        'weight' => '-10',
      ),
    ),
  );

  // Exported image style: bendy_wide
  $styles['bendy_wide'] = array(
    'name' => 'bendy_wide',
    'effects' => array(
      6 => array(
        'label' => 'Scale and crop',
        'help' => 'Scale and crop will maintain the aspect-ratio of the original image, then crop the larger dimension. This is most useful for creating perfectly square thumbnails without stretching the image.',
        'effect callback' => 'image_scale_and_crop_effect',
        'dimensions callback' => 'image_resize_dimensions',
        'form callback' => 'image_resize_form',
        'summary theme' => 'image_resize_summary',
        'module' => 'image',
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => '780',
          'height' => '300',
        ),
        'weight' => '-10',
      ),
    ),
  );

  return $styles;
}

/**
 * Implements hook_node_info().
 */
function bendy_node_info() {
  $items = array(
    'bendy_featured_content' => array(
      'name' => t('Featured content'),
      'base' => 'node_content',
      'description' => t('Use <em>Featured content</em> to add an item to the main rotating banner atop the home page.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
