<?php
/**
 * @file
 * Functionality to implement opportunity for display 2GIS Maps in blocks.
 */

/**
 * Implements hook_block_info().
 */
function dgis_maps_block_info() {
  $count = variable_get('dgis_maps_blocks_amount', 1);

  for ($num = 1; $num <= $count; $num++) {
    $blocks[DGIS_DEFAULT_BLOCK_DELTA . '_' . $num] = array(
      'info' => t('2GIS Map #!block_number', array('!block_number' => $num)),
      'cache' => DRUPAL_NO_CACHE,
    );
  }

  return $blocks;
}

/**
 * Implements hook_block_configure().
 */
function dgis_maps_block_configure($delta = '') {
  $form = array();

  if (strpos($delta, DGIS_DEFAULT_BLOCK_DELTA) === 0) {
    $settings = json_decode(dgis_maps_get_map_data($delta));

    $form['dgis_maps'] = array(
      '#title' => t('Map settings'),
      '#type' => 'fieldset',
      '#tree' => TRUE,
    );

    $form['dgis_maps']['center'] = array(
      '#title' => t('Center'),
      '#attributes' => array(
        'class' => array('edit-dgis-map-center')
      ),
      '#type' => 'textfield',
      '#default_value' => $settings->center,
    );

    $form['dgis_maps']['zoom'] = array(
      '#title' => t('Zoom'),
      '#attributes' => array(
        'class' => array('edit-dgis-map-zoom')
      ),
      '#type' => 'textfield',
      '#default_value' => $settings->zoom,
    );

    $form['dgis_maps']['markers'] = array(
      '#type' => 'hidden',
      '#attributes' => array(
        'class' => array('edit-dgis-map-markers')
      ),
      '#default_value' => $settings->markers,
    );

    // Load library.
    $form['#attached']['library'][] = array('dgis_maps', '2gis_maps_block_admin');

    // We don't use drupal_html_id, to store true delta.
    $map_id = "map-$delta";
    $form['dgis_maps']['map'] = array(
      '#type' => 'html_tag',
      '#tag' => 'div',
      '#value' => ' ',
      '#attributes' => array(
        'id' => $map_id,
        'class' => array('dgis-maps-container-edit'),
        'style' => 'width:' . 500 . 'px; height:' . 400 . 'px; clear: both;',
      ),
      '#weight' => -20,
    );
  }

  return $form;
}

/**
 * Implements hook_block_save().
 */
function dgis_maps_block_save($delta = '', $edit = array()) {
  if (strpos($delta, DGIS_DEFAULT_BLOCK_DELTA) === 0) {
    variable_set($delta . '_settings', json_encode($edit['dgis_maps']));
  }
}

/**
 * Implements hook_block_view().
 */
function dgis_maps_block_view($delta) {
  $block['subject'] = t('2GIS Map');
  $block['content'] = dgis_maps_block_content($delta);

  return $block;
}

/**
 * Returns block content.
 */
function dgis_maps_block_content($delta) {

  $map_id = drupal_html_id('map');

  $map_data = array(
    '#type' => 'html_tag',
    '#tag' => 'span',
    '#attributes' => array(
      'id' => "$map_id-data",
      'class' => array('element-hidden'),
    ),
    '#value' => dgis_maps_get_map_data($delta),
  );

  $block_output['map'] = array(
    '#type' => 'html_tag',
    '#tag' => 'div',
    '#attributes' => array(
      'id' => $map_id,
      'class' => array('dgis-maps-container-view'),
      'style' => 'width:' . 500 . 'px; height:' . 400 . 'px;',
    ),
    '#value' => '',
    '#suffix' => render($map_data),
  );

  $block_output['#attached']['library'][] = array('dgis_maps', '2gis_maps');

  return $block_output;
}
