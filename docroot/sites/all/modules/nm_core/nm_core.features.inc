<?php
/**
 * @file
 * nm_core.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function nm_core_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  list($module, $api) = func_get_args();
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function nm_core_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_image_default_styles().
 */
function nm_core_image_default_styles() {
  $styles = array();

  // Exported image style: nodemaker_headline_image.
  $styles['nodemaker_headline_image'] = array(
    'name' => 'nodemaker_headline_image',
    'effects' => array(
      1 => array(
        'label' => 'Javascript crop',
        'help' => 'Create a crop with a javascript toolbox.',
        'effect callback' => 'imagecrop_effect',
        'form callback' => 'imagecrop_effect_form',
        'summary theme' => 'imagecrop_effect_summary',
        'module' => 'imagecrop',
        'name' => 'imagecrop_javascript',
        'data' => array(
          'width' => '1200',
          'height' => '500',
          'xoffset' => 'center',
          'yoffset' => 'center',
          'resizable' => 1,
          'downscaling' => 1,
          'aspect_ratio' => 'CROP',
          'disable_if_no_data' => 1,
        ),
        'weight' => '-10',
      ),
      4 => array(
        'label' => 'Scale and crop',
        'help' => 'Scale and crop will maintain the aspect-ratio of the original image, then crop the larger dimension. This is most useful for creating perfectly square thumbnails without stretching the image.',
        'effect callback' => 'image_scale_and_crop_effect',
        'dimensions callback' => 'image_resize_dimensions',
        'form callback' => 'image_resize_form',
        'summary theme' => 'image_resize_summary',
        'module' => 'image',
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => '1200',
          'height' => '500',
        ),
        'weight' => '4',
      ),
    ),
  );

  // Exported image style: nodemaker_thumbnail.
  $styles['nodemaker_thumbnail'] = array(
    'name' => 'nodemaker_thumbnail',
    'effects' => array(
      1 => array(
        'label' => 'Javascript crop',
        'help' => 'Create a crop with a javascript toolbox.',
        'effect callback' => 'imagecrop_effect',
        'form callback' => 'imagecrop_effect_form',
        'summary theme' => 'imagecrop_effect_summary',
        'module' => 'imagecrop',
        'name' => 'imagecrop_javascript',
        'data' => array(
          'width' => '700',
          'height' => '700',
          'xoffset' => 'center',
          'yoffset' => 'center',
          'resizable' => 1,
          'downscaling' => 1,
          'aspect_ratio' => 'CROP',
          'disable_if_no_data' => 1,
        ),
        'weight' => '2',
      ),
      4 => array(
        'label' => 'Scale and crop',
        'help' => 'Scale and crop will maintain the aspect-ratio of the original image, then crop the larger dimension. This is most useful for creating perfectly square thumbnails without stretching the image.',
        'effect callback' => 'image_scale_and_crop_effect',
        'dimensions callback' => 'image_resize_dimensions',
        'form callback' => 'image_resize_form',
        'summary theme' => 'image_resize_summary',
        'module' => 'image',
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => '700',
          'height' => '700',
        ),
        'weight' => '4',
      ),
    ),
  );
  
  // Exported image style: nodemaker_thumbnail_small.
  $styles['nodemaker_thumbnail_small'] = array(
    'name' => 'nodemaker_thumbnail_small',
    'effects' => array(
      13 => array(
        'label' => 'Reuse a javascript crop selection',
        'help' => 'Reuse crop selection from another javascript crop preset.',
        'effect callback' => 'imagecrop_reuse_effect',
        'form callback' => 'imagecrop_reuse_form',
        'summary theme' => 'theme_imagecrop_reuse',
        'module' => 'imagecrop',
        'name' => 'imagecrop_reuse',
        'data' => array(
          'imagecrop_style' => 'nodemaker_thumbnail',
        ),
        'weight' => '-10',
      ),
      11 => array(
        'label' => 'Scale and crop',
        'help' => 'Scale and crop will maintain the aspect-ratio of the original image, then crop the larger dimension. This is most useful for creating perfectly square thumbnails without stretching the image.',
        'effect callback' => 'image_scale_and_crop_effect',
        'dimensions callback' => 'image_resize_dimensions',
        'form callback' => 'image_resize_form',
        'summary theme' => 'image_resize_summary',
        'module' => 'image',
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => '350',
          'height' => '350',
        ),
        'weight' => '-7',
      ),
    ),
  );

  return $styles;
}

/**
 * Implements hook_node_info().
 */
function nm_core_node_info() {
  $items = array(
    'nm_page' => array(
      'name' => t('Basic Page'),
      'base' => 'node_content',
      'description' => t('Use <em>basic pages</em> for your static content, such as an \'About us\' page.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
