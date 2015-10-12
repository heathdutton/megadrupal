<?php
/**
 * @file
 * nm_galleries.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function nm_galleries_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "boxes" && $api == "box") {
    return array("version" => "1");
  }
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "flexslider" && $api == "flexslider_default_preset") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function nm_galleries_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_image_default_styles().
 */
function nm_galleries_image_default_styles() {
  $styles = array();

  // Exported image style: nodemaker_gallery_large.
  $styles['nodemaker_gallery_large'] = array(
    'name' => 'nodemaker_gallery_large',
    'effects' => array(
      3 => array(
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
        'weight' => '1',
      ),
    ),
  );

  // Exported image style: nodemaker_gallery_narrow.
  $styles['nodemaker_gallery_narrow'] = array(
    'name' => 'nodemaker_gallery_narrow',
    'effects' => array(
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
          'height' => '250',
        ),
        'weight' => '1',
      ),
    ),
  );

  // Exported image style: nodemaker_gallery_scaled.
  $styles['nodemaker_gallery_scaled'] = array(
    'name' => 'nodemaker_gallery_scaled',
    'effects' => array(
      2 => array(
        'label' => 'Scale',
        'help' => 'Scaling will maintain the aspect-ratio of the original image. If only a single dimension is specified, the other dimension will be calculated.',
        'effect callback' => 'image_scale_effect',
        'dimensions callback' => 'image_scale_dimensions',
        'form callback' => 'image_scale_form',
        'summary theme' => 'image_scale_summary',
        'module' => 'image',
        'name' => 'image_scale',
        'data' => array(
          'width' => '740',
          'height' => '',
          'upscale' => 0,
        ),
        'weight' => '2',
      ),
    ),
  );

  // Exported image style: nodemaker_gallery_square.
  $styles['nodemaker_gallery_square'] = array(
    'name' => 'nodemaker_gallery_square',
    'effects' => array(
      5 => array(
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
          'height' => '800',
        ),
        'weight' => '1',
      ),
    ),
  );

  return $styles;
}

/**
 * Implements hook_node_info().
 */
function nm_galleries_node_info() {
  $items = array(
    'nm_gallery' => array(
      'name' => t('Gallery'),
      'base' => 'node_content',
      'description' => t('The <em>Gallery</em> content type allows galleries to be created, and embedded on various content types.'),
      'has_title' => '1',
      'title_label' => t('Gallery Name'),
      'help' => '',
    ),
  );
  return $items;
}
