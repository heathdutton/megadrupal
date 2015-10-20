<?php
/**
 * @file
 * nm_blog.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function nm_blog_ctools_plugin_api() {
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
function nm_blog_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_image_default_styles().
 */
function nm_blog_image_default_styles() {
  $styles = array();

  // Exported image style: nodemaker_blog_teaser.
  $styles['nodemaker_blog_teaser'] = array(
    'name' => 'nodemaker_blog_teaser',
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
          'width' => '800',
          'height' => '600',
          'xoffset' => 'center',
          'yoffset' => 'center',
          'resizable' => 1,
          'downscaling' => 1,
          'aspect_ratio' => 'CROP',
          'disable_if_no_data' => 1,
        ),
        'weight' => '-9',
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
          'width' => '800',
          'height' => '600',
        ),
        'weight' => '4',
      ),
    ),
  );

  return $styles;
}

/**
 * Implements hook_node_info().
 */
function nm_blog_node_info() {
  $items = array(
    'nm_blog' => array(
      'name' => t('Blog'),
      'base' => 'node_content',
      'description' => t('Use this <em>blog</em> content type for your date-relevant content.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => t('Use this <em>blog</em> content type for your date-relevant content.'),
    ),
  );
  return $items;
}
