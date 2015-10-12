<?php

/**
 * @file
 * Administrative page for colorized gmap module.
 */

/**
 * Block settings configure form.
 */
function colorized_gmap_page_form($form, &$form_state) {
  module_load_include('inc', 'block', 'block.admin');
  $form = block_admin_configure($form, $form_state, 'colorized_gmap', NULL);
  $form_state['build_info']['base_form_id'] = 'block_add_block_form';
  $form['regions']['#weight'] = 6;
  $form['visibility_title']['#weight'] = 7;
  $form['visibility']['#weight'] = 8;
  $form['#validate'] = array();
  $form['#submit'] = array('colorized_gmap_form_submit');
  return $form;
}

/**
 * Submit callback.
 */
function colorized_gmap_form_submit($form, &$form_state) {
  colorized_gmap_block_save(NULL, $form_state['values']);
  drupal_set_message(t('The block has been created.'));
  cache_clear_all();
  $form_state['redirect'] = 'admin/structure/block';
}

/**
 * Menu callback: confirm deletion of menu blocks.
 */
function colorized_gmap_block_delete($form, &$form_state, $delta) {
  $form['delta'] = array('#type' => 'hidden', '#value' => $delta);
  return confirm_form($form, t('Are you sure you want to delete colorized gmap block?'), 'admin/structure/block', NULL, t('Delete'), t('Cancel'));
}

/**
 * Delete callback.
 */
function colorized_gmap_block_delete_submit($form, &$form_state) {
  // Remove the menu block configuration variables.
  entity_delete('colorized_gmap', $form_state['values']['delta']);
  drupal_set_message(t('The block has been removed.'));
  cache_clear_all();
  $form_state['redirect'] = 'admin/structure/block';
}

/**
 * Helper function. Create form elements for map styles.
 */
function _colorized_gmap_form_map_stylers(&$form, &$form_state, $entity = NULL) {
  $styles = NULL;
  if (isset($entity)) {
    // Get map styles and rebuild indexes.
    $i = 1;
    foreach ($entity->style as $item) {
      $styles[$i] = $item;
      $i++;
    }
  }
  // List of available map features.
  $feature_types = array(
    'water' => 'Water',
    'landscape' => 'Landscape',
    'landscape.man_made' => 'Landscape (man made)',
    'landscape.natural' => 'Landscape (natural)',
    'landscape.natural.landcover' => 'Landscape (natural landcover)',
    'landscape.natural.terrain' => 'Landscape (natural terrain)',
    'road' => 'Road',
    'road.highway' => 'Road (highway)',
    'road.highway.controlled_access' => 'Road highway (controlled access)',
    'road.arterial' => 'Road (Arterial)',
    'road.local' => 'Road (local)',
    'poi' => 'Poi',
    'poi.park' => 'Poi (park)',
    'poi.business' => 'Poi (business)',
    'poi.attraction' => 'Poi (attraction)',
    'poi.medical' => 'Poi (medical)',
    'poi.school' => 'Poi (school)',
    'poi.sports_complex' => 'Poi (sports complex)',
    'poi.government' => 'Poi (government)',
    'poi.place_of_worship' => 'Poi (place of worship)',
    'administrative' => 'Administrative',
    'administrative.country' => 'Administrative (country)',
    'administrative.land_parcel' => 'Administrative (land parcel)',
    'administrative.locality' => 'Administrative (locality)',
    'administrative.neighborhood' => 'Administrative (neighborhood)',
    'administrative.province' => 'Administrative (province)',
    'all' => 'All',
    'transit' => 'Transit',
    'transit.line' => 'Transit (line)',
    'transit.station' => 'Transit station',
    'transit.station.airport' => 'Transit station (airport)',
    'transit.station.bus' => 'Transit station (bus)',
    'transit.station.rail' => 'Transit station (rail)',
  );
  // List of available map elements.
  $elements = array(
    'all' => 'All',
    'geometry' => 'Geometry',
    'geometry.fill' => 'Geometry fill',
    'geometry.stroke' => 'Geometry stroke',
    'labels' => 'Labels',
    'labels.icon' => 'Labels icon',
    'labels.text' => 'Labels text',
    'labels.text.fill' => 'Labels text fill',
    'labels.text.stroke' => 'Labels text stroke',
  );
  $form['settings']['colorized_map_styles'] = array(
    '#theme' => 'colorized_gmap_table_form',
    '#type' => 'fieldset',
    '#weight' => 2,
    '#title' => t('Map styles'),
    '#prefix' => '<div id="gmap-ajax-wrapper">',
    '#suffix' => '</div>',
    '#header' => array(
      t('Feature type'),
      t('Element type'),
      t('Stylers'),
      t('Remove'),
    ),
    'settings' => array(
      '#tree' => TRUE,
    ),
  );
  // Map example div.
  $form['settings']['markup'] = array(
    '#type' => 'markup',
    '#markup' => '<div id="colorized-gmap-content"></div>',
  );

  if (!$form_state['rebuild'] && !empty($styles)) {
    $form_state['styles_count'] = count($styles);
    $form_state['styles_removed'] = array();
  }
  if (empty($form_state['styles_count'])) {
    $form_state['styles_count'] = 1;
    $form_state['styles_removed'] = array();
  }
  // Add more|remove row table functionality.
  for ($i = 1; $i < $form_state['styles_count'] + 1; $i++) {
    if (in_array($i, $form_state['styles_removed'])) {
      continue;
    }
    $form['settings']['colorized_map_styles']['settings'][$i]['featureType'] = array(
      '#type' => 'select',
      '#options' => $feature_types,
      '#default_value' => !empty($styles[$i]['featureType']) ? $styles[$i]['featureType'] : '',
      '#ajax' => array(
        'callback' => 'colorized_gmap_styles_update',
        'wrapper' => 'colorized-gmap-content',
        'progress' => array(
          'type' => 'none',
        ),
      ),
    );
    $form['settings']['colorized_map_styles']['settings'][$i]['elementType'] = array(
      '#type' => 'select',
      '#options' => $elements,
      '#default_value' => !empty($styles[$i]['elementType']) ? $styles[$i]['elementType'] : '',
      '#ajax' => array(
        'callback' => 'colorized_gmap_styles_update',
        'wrapper' => 'colorized-gmap-content',
        'progress' => array(
          'type' => 'none',
        ),
      ),
    );
    $form['settings']['colorized_map_styles']['settings'][$i]['stylers'][0] = array(
      '#tree' => TRUE,
      'color' => array(
        '#title' => t('Color'),
        '#type' => 'textfield',
        '#size' => 4,
        '#default_value' => !empty($styles[$i]['stylers'][0]['color']) ? $styles[$i]['stylers'][0]['color'] : NULL,
        '#attributes' => array(
          'class' => array('edit_color_input'),
        ),
        '#ajax' => array(
          'callback' => 'colorized_gmap_styles_update',
          'event' => 'textfield_change',
          'wrapper' => 'colorized-gmap-content',
          'progress' => array(
            'type' => 'none',
          ),
        ),
      ),
    );
    $form['settings']['colorized_map_styles']['settings'][$i]['stylers'][1] = array(
      'visibility' => array(
        '#type' => 'select',
        '#title' => t('Visibility'),
        '#default_value' => !empty($styles[$i]['stylers'][1]['visibility']) ? $styles[$i]['stylers'][1]['visibility'] : NULL,
        '#options' => array(
          'on' => 'On',
          'off' => 'Off',
          'simplified' => 'simplified',
        ),
        '#ajax' => array(
          'callback' => 'colorized_gmap_styles_update',
          'wrapper' => 'colorized-gmap-content',
          'progress' => array(
            'type' => 'none',
          ),
        ),
      ),
    );
    $form['settings']['colorized_map_styles']['settings'][$i]['stylers'][2] = array(
      'saturation' => array(
        '#type' => 'textfield',
        '#size' => 4,
        '#title' => t('Saturation'),
        '#default_value' => !empty($styles[$i]['stylers'][2]['saturation']) ? $styles[$i]['stylers'][2]['saturation'] : NULL,
        '#ajax' => array(
          'callback' => 'colorized_gmap_styles_update',
          'wrapper' => 'colorized-gmap-content',
          'progress' => array(
            'type' => 'none',
          ),
        ),
      ),
    );
    $form['settings']['colorized_map_styles']['settings'][$i]['stylers'][3] = array(
      'lightness' => array(
        '#type' => 'textfield',
        '#size' => 4,
        '#title' => t('Lightness'),
        '#default_value' => !empty($styles[$i]['stylers'][3]['lightness']) ? $styles[$i]['stylers'][3]['lightness'] : NULL,
        '#ajax' => array(
          'callback' => 'colorized_gmap_styles_update',
          'wrapper' => 'colorized-gmap-content',
          'progress' => array(
            'type' => 'none',
          ),
        ),
      ),
    );
    $form['settings']['colorized_map_styles']['settings'][$i]['stylers'][4] = array(
      'weight' => array(
        '#type' => 'textfield',
        '#size' => 1,
        '#title' => t('Weight'),
        '#default_value' => !empty($styles[$i]['stylers'][4]['weight']) ? $styles[$i]['stylers'][4]['weight'] : NULL,
        '#ajax' => array(
          'callback' => 'colorized_gmap_styles_update',
          'wrapper' => 'colorized-gmap-content',
          'progress' => array(
            'type' => 'none',
          ),
        ),
      ),
    );
    $form['settings']['colorized_map_styles']['settings'][$i]['remove'] = array(
      '#type' => 'checkbox',
    );
  }
  // Buttons.
  $form['settings']['ajax_buttons'] = array(
    '#type' => 'fieldset',
    '#weight' => 3,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['settings']['ajax_buttons']['add_more'] = array(
    '#type' => 'submit',
    '#value' => t('Add More'),
    '#submit' => array('colorized_gmap_form_add_row'),
    '#ajax' => array(
      'callback' => 'colorized_gmap_update',
      'wrapper' => 'gmap-ajax-wrapper',
    ),
  );
  $form['settings']['ajax_buttons']['remove_row'] = array(
    '#type' => 'submit',
    '#value' => t('Remove Row'),
    '#submit' => array('colorized_gmap_form_remove_row'),
    '#ajax' => array(
      'callback' => 'colorized_gmap_update',
      'wrapper' => 'gmap-ajax-wrapper',
    ),
  );
}

/**
 * Helper function. Create form elements for map controls settings.
 */
function _colorized_gmap_form_map_controls(&$form, &$form_state, $entity = NULL) {
  $settings = array();
  if (isset($entity)) {
    $settings = $entity->additional_settings['controls'];
  }
  $form['settings']['colorized_map_addtional_settings'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#title' => t('Additional gmap api settings'),
    '#weight' => 4,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['settings']['colorized_map_addtional_settings']['controls'] = array(
    '#type' => 'fieldset',
    '#title' => t('Controls'),
    '#weight' => 1,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['settings']['colorized_map_addtional_settings']['controls']['min_drag_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Min draggable screnn width'),
    '#size' => 5,
    '#description' => t('If your screen width is greater, the map will be draggable. Enter 0 to make map always draggable.'),
    '#default_value' => isset($settings['min_drag_width']) ? $settings['min_drag_width'] : 0,
    '#field_suffix' => 'px',
  );
  $form['settings']['colorized_map_addtional_settings']['controls']['streetViewControl'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable streetview control'),
    '#default_value' => isset($settings['streetViewControl']) ? $settings['streetViewControl'] : TRUE,
  );
  $form['settings']['colorized_map_addtional_settings']['controls']['panControl'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable pan control'),
    '#default_value' => isset($settings['panControl']) ? $settings['panControl'] : TRUE,
  );
  $form['settings']['colorized_map_addtional_settings']['controls']['mapTypeControl'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable map type control'),
    '#default_value' => isset($settings['mapTypeControl']) ? $settings['mapTypeControl'] : TRUE,
  );
}

/**
 * Helper function. Create form elements for map controls position settings.
 */
function _colorized_gmap_form_map_controls_position(&$form, &$form_state, $position, $entity = NULL) {
  $settings = array();
  if (isset($entity)) {
    $settings = $entity->additional_settings['controls_position'];
  }
  $form['settings']['colorized_map_addtional_settings']['controls_position'] = array(
    '#type' => 'fieldset',
    '#title' => t('Controls Position'),
    '#weight' => 2,
    '#attributes' => array(
      'class' => array('controls_position'),
    ),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['settings']['colorized_map_addtional_settings']['controls_position']['streetViewControl'] = array(
    '#type' => 'select',
    '#title' => t('Streetview control position'),
    '#options' => $position,
    '#default_value' => !empty($settings['streetViewControl']) ? $settings['streetViewControl'] : '1',
  );
  $form['settings']['colorized_map_addtional_settings']['controls_position']['panControl'] = array(
    '#type' => 'select',
    '#title' => t('Pan control position'),
    '#options' => $position,
    '#default_value' => !empty($settings['panControl']) ? $settings['panControl'] : '1',
  );
  $form['settings']['colorized_map_addtional_settings']['controls_position']['mapTypeControl'] = array(
    '#type' => 'select',
    '#title' => t('Map type control position'),
    '#options' => $position,
    '#default_value' => !empty($settings['mapTypeControl']) ? $settings['mapTypeControl'] : '3',
  );

}

/**
 * Helper function. Create form elements for map marker settings.
 */
function _colorized_gmap_form_map_marker(&$form, &$form_state, $entity) {
  $settings = array();
  if (isset($entity)) {
    $settings = $entity->additional_settings['marker_settings'];
    // If marker exists - load its info.
    if (!is_array($settings['marker']) && !empty($settings['marker'])) {
      $marker = _colorized_gmap_marker_info($settings['marker'], $entity->id);
      $settings['marker'] = $marker;
    }
  }

  $form['settings']['colorized_map_addtional_settings']['marker_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Marker'),
    '#attributes' => array(
      'class' => array('gmap_colorizer_input'),
    ),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['settings']['colorized_map_addtional_settings']['marker_settings']['marker'] = array(
    '#name' => 'Marker image',
    '#type' => 'managed_file',
    '#title' => t('Choose a file'),
    '#default_value' => !empty($settings['marker']['fid']) ? $settings['marker']['fid'] : '',
    '#theme_wrappers' => array(),
    '#theme' => 'colorized_gmap_image_marker',
    '#upload_location' => 'public://',
    '#upload_validators' => array(
      'file_validate_extensions' => array('gif png jpg jpeg'),
      'file_validate_size' => array(0.3 * 1024 * 1024),
      'file_validate_image_resolution' => array('100x100'),
    ),
  );
  $form['settings']['colorized_map_addtional_settings']['marker_settings']['markertitle'] = array(
    '#type' => 'textfield',
    '#size' => 30,
    '#title' => t('Title'),
    '#default_value' => !empty($settings['markertitle']) ? $settings['markertitle'] : 'Destination',
    '#description' => t('Title to display on the mouseover'),
  );
}

/**
 * Helper function. Create form elements for map zoom settings.
 */
function _colorized_gmap_form_zoom_settings(&$form, &$form_state, $position, $entity = NULL) {
  $settings = array();
  if (isset($entity)) {
    $settings = $entity->additional_settings['zoom_control_settings'];
  }
  $form['settings']['colorized_map_addtional_settings']['zoom_control_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Zoom control settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['settings']['colorized_map_addtional_settings']['zoom_control_settings']['zoom'] = array(
    '#type' => 'textfield',
    '#title' => t('Zoom'),
    '#size' => 10,
    '#default_value' => !empty($settings['zoom']) ? $settings['zoom'] : '15',
    '#description' => t('Enter zoom amount'),
  );
  $form['settings']['colorized_map_addtional_settings']['zoom_control_settings']['zoomControl'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable zoom control'),
    '#default_value' => isset($settings['zoomControl']) ? $settings['zoomControl'] : TRUE,
  );
  $form['settings']['colorized_map_addtional_settings']['zoom_control_settings']['scrollwheel'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable scrollwheel zoom'),
    '#default_value' => isset($settings['scrollwheel']) ? $settings['scrollwheel'] : TRUE,
  );
  $form['settings']['colorized_map_addtional_settings']['zoom_control_settings']['zoomControlSize'] = array(
    '#type' => 'select',
    '#title' => t('Zoom Control Size'),
    '#options' => array(
      '1' => 'Small',
      '2' => 'Large',
    ),
    '#default_value' => !empty($settings['zoomControlSize']) ? $settings['zoomControlSize'] : 2,
  );
  $form['settings']['colorized_map_addtional_settings']['zoom_control_settings']['zoomControlPosition'] = array(
    '#type' => 'select',
    '#title' => t('Zoom Control Position'),
    '#options' => $position,
    '#default_value' => !empty($settings['zoomControlPosition']) ? $settings['zoomControlPosition'] : 1,
  );
}

/**
 * Helper function. Create form elements for map zoom position settings.
 */
function _colorized_gmap_form_map_coordinates(&$form, &$form_state, $entity = NULL) {
  $coords = array();
  if (isset($entity)) {
    $coords = array(
      'latitude' => $entity->latitude,
      'longitude' => $entity->longitude,
    );
  }

  $form['settings']['coordinates'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#weight' => 1,
  );
  $form['settings']['coordinates']['latitude'] = array(
    '#type' => 'textfield',
    '#title' => t('Latitude'),
    '#size' => 10,
    '#weight' => 0,
    '#default_value' => !empty($coords['latitude']) ? $coords['latitude'] : '48.853358',
    '#ajax' => array(
      'callback' => 'colorized_gmap_styles_update',
      'wrapper' => 'colorized-gmap-content',
      'progress' => array(
        'type' => 'none',
      ),
    ),
  );
  $form['settings']['coordinates']['longitude'] = array(
    '#type' => 'textfield',
    '#title' => t('Longitude'),
    '#size' => 10,
    '#weight' => 2,
    '#default_value' => !empty($coords['longitude']) ? $coords['longitude'] : '2.348903',
    '#ajax' => array(
      'callback' => 'colorized_gmap_styles_update',
      'wrapper' => 'colorized-gmap-content',
      'progress' => array(
        'type' => 'none',
      ),
    ),
  );
}

/**
 * AJAX callback: Update form.
 */
function colorized_gmap_update($form, $form_state) {
  return $form['settings']['colorized_map_styles'];
}

/**
 * AJAX callback: updating styles.
 */
function colorized_gmap_styles_update($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  $form_state['styles_updated'] = TRUE;
}

/**
 * Pass map settings to js.
 *
 * @param int $map_settings
 *   Array of map settings.
 */
function colorized_gmap_admin_js_settings_export($map_settings) {
  $styles = array();
  if (isset($map_settings['settings'])) {
    foreach ($map_settings['settings'] as $key => $item) {
      if (is_numeric($key)) {
        unset($item['remove']);
        $styles[] = $item;
      }
    }
    $settings = array(
      'lat' => $map_settings['latitude'],
      'long' => $map_settings['longitude'],
      'style' => $styles,
      'additional_settings' => $map_settings['colorized_map_addtional_settings'],
    );
    drupal_add_js(array('colorized_gmap' => $settings), 'setting');
  }
}

/**
 * Helper function to check if map machine name exists.
 *
 * @param string $value
 *   Entered 'valid machine' name field value.
 */
function _colorized_gmap_block_name_exists($value) {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'colorized_gmap');
  $query->propertyCondition('name', $value);
  $result = $query->execute();
  if (!empty($result)) {
    return TRUE;
  }
  return FALSE;
}
