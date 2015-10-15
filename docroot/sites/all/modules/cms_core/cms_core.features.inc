<?php
/**
 * @file
 * cms_core.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function cms_core_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "bean_admin_ui" && $api == "bean") {
    return array("version" => "5");
  }
  if ($module == "file_entity" && $api == "file_default_displays") {
    return array("version" => "1");
  }
  if ($module == "flexslider" && $api == "flexslider_default_preset") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_image_default_styles().
 */
function cms_core_image_default_styles() {
  $styles = array();

  // Exported image style: bootstrap3_col1.
  $styles['bootstrap3_col1'] = array(
    'label' => 'Bootstrap3 1/12',
    'effects' => array(
      12 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 68,
          'height' => '',
          'upscale' => 1,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: bootstrap3_col10.
  $styles['bootstrap3_col10'] = array(
    'label' => 'Bootstrap3 10/12',
    'effects' => array(
      33 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 945,
          'height' => '',
          'upscale' => 1,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: bootstrap3_col11.
  $styles['bootstrap3_col11'] = array(
    'label' => 'Bootstrap 11/12',
    'effects' => array(
      34 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 1043,
          'height' => '',
          'upscale' => 1,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: bootstrap3_col12.
  $styles['bootstrap3_col12'] = array(
    'label' => 'Bootstrap 12/12',
    'effects' => array(
      19 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 1140,
          'height' => '',
          'upscale' => 1,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: bootstrap3_col12_square.
  $styles['bootstrap3_col12_square'] = array(
    'label' => 'Bootstrap 12/12 Square',
    'effects' => array(
      4 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 1140,
          'height' => 1140,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: bootstrap3_col1_square.
  $styles['bootstrap3_col1_square'] = array(
    'label' => 'Bootstrap 1/12 Square',
    'effects' => array(
      21 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 68,
          'height' => 68,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: bootstrap3_col2.
  $styles['bootstrap3_col2'] = array(
    'label' => 'Bootstrap 1/6',
    'effects' => array(
      13 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 165,
          'height' => '',
          'upscale' => 1,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: bootstrap3_col2_square.
  $styles['bootstrap3_col2_square'] = array(
    'label' => 'Bootstrap 1/6 Square',
    'effects' => array(
      38 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 165,
          'height' => 165,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: bootstrap3_col3.
  $styles['bootstrap3_col3'] = array(
    'label' => 'Bootstrap 1/4',
    'effects' => array(
      14 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 263,
          'height' => '',
          'upscale' => 1,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: bootstrap3_col3_square.
  $styles['bootstrap3_col3_square'] = array(
    'label' => 'Bootstrap 1/4 Square',
    'effects' => array(
      23 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 263,
          'height' => 263,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: bootstrap3_col4.
  $styles['bootstrap3_col4'] = array(
    'label' => 'Bootstrap 1/3',
    'effects' => array(
      15 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 360,
          'height' => '',
          'upscale' => 1,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: bootstrap3_col4_square.
  $styles['bootstrap3_col4_square'] = array(
    'label' => 'Bootstrap 1/3 Square',
    'effects' => array(
      5 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 360,
          'height' => 360,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: bootstrap3_col5.
  $styles['bootstrap3_col5'] = array(
    'label' => 'Bootstrap 5/12',
    'effects' => array(
      31 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 458,
          'height' => '',
          'upscale' => 1,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: bootstrap3_col6.
  $styles['bootstrap3_col6'] = array(
    'label' => 'Bootstrap 1/2',
    'effects' => array(
      16 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 555,
          'height' => '',
          'upscale' => 1,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: bootstrap3_col6_square.
  $styles['bootstrap3_col6_square'] = array(
    'label' => 'Bootstrap 1/2 Square',
    'effects' => array(
      39 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 555,
          'height' => 555,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: bootstrap3_col7.
  $styles['bootstrap3_col7'] = array(
    'label' => 'Bootstrap 7/12',
    'effects' => array(
      32 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 653,
          'height' => '',
          'upscale' => 1,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: bootstrap3_col8.
  $styles['bootstrap3_col8'] = array(
    'label' => 'Bootstrap 2/3',
    'effects' => array(
      17 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 750,
          'height' => '',
          'upscale' => 1,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: bootstrap3_col8_banner.
  $styles['bootstrap3_col8_banner'] = array(
    'label' => 'Bootstrap 2/3 Banner',
    'effects' => array(
      22 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 750,
          'height' => 300,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: bootstrap3_col9.
  $styles['bootstrap3_col9'] = array(
    'label' => 'Bootstrap 3/4',
    'effects' => array(
      18 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 848,
          'height' => '',
          'upscale' => 1,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: bootstrap3_col9_banner.
  $styles['bootstrap3_col9_banner'] = array(
    'label' => 'Bootstrap 3/4 Banner',
    'effects' => array(
      20 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 848,
          'height' => 339,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: bootstrap3_col9_square.
  $styles['bootstrap3_col9_square'] = array(
    'label' => 'Bootstrap 3/4 Square',
    'effects' => array(
      6 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 848,
          'height' => 848,
        ),
        'weight' => 1,
      ),
    ),
  );

  return $styles;
}

/**
 * Implements hook_node_info().
 */
function cms_core_node_info() {
  $items = array(
    'page' => array(
      'name' => t('Basic page'),
      'base' => 'node_content',
      'description' => t('Use <em>basic pages</em> for your static content, such as an \'About us\' page.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
