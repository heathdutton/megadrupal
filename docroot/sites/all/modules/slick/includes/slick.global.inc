<?php
/**
 * @file
 * Global functions across Slick field formatters and Views.
 */

/**
 * Defines global field formatter and Views settings.
 */
function slick_get_global_default_settings() {
  $settings = array(
    'asnavfor_main'       => '',
    'asnavfor_thumbnail'  => '',
    'asnavfor_auto'       => FALSE,
    'optionset'           => 'default',
    'optionset_thumbnail' => '',
    'skin'                => '',
    'skin_thumbnail'      => '',
    'skin_arrows'         => '',
    'skin_dots'           => '',
    'slide_caption'       => array(),
    'slide_image'         => '',
    'slide_layout'        => '',
    'slide_overlay'       => array(),
    'slide_link'          => '',
    'slide_title'         => '',
    'thumbnail_caption'   => '',
    'grid'                => '',
    'grid_medium'         => '',
    'grid_small'          => '',
    'visible_slides'      => '',
    'preserve_keys'       => FALSE,
    'override'            => FALSE,
    'overridables'        => array(),
    'cache'               => 0,
  );
  return $settings;
}

/**
 * Returns items as a grid display.
 */
function slick_build_grids(array $build, &$settings) {
  $grids = array();
  if (!empty($settings['unslick'])) {
    $settings['current_display'] = 'main';
    $settings['current_item'] = 'grid';

    // Displays all items if unslick is enforced such as plain grid to lightbox.
    $grids[0] = array(
      '#theme' => 'slick_grid',
      '#items' => $build,
      '#delta' => 0,
      '#settings' => array(
        'count' => 2,
        'skin' => $settings['skin'],
        'unslick' => TRUE,
      ),
    );
  }
  else {
    // Otherwise do chunks to have a grid carousel.
    if ($visible_slides = $settings['visible_slides']) {
      $grid_items = array_chunk($build, $visible_slides, $settings['preserve_keys']);
      foreach ($grid_items as $delta => $grid_item) {
        $grids[] = array(
          '#theme' => 'slick_grid',
          '#items' => $grid_item,
          '#delta' => $delta,
          '#settings' => array(
            'count' => count($grid_items),
            'skin' => $settings['skin'],
          ),
        );
      }
    }
  }

  return $grids;
}
