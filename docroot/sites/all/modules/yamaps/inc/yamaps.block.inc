<?php

/**
 * @file
 * Yandex Maps blocks configuration.
 */

/**
 * Implements hook_block_info().
 */
function yamaps_block_info() {
  $blocks[YAMAPS_DEFAULT_BLOCK_DELTA] = array(
    'info' => t('Yandex Map #1'),
    'cache' => DRUPAL_NO_CACHE,
  );

  $blocks_amount = variable_get('yamaps_blocks_amount', YAMAPS_DEFAULT_BLOCK_AMOUNT);

  for ($block_number = 2; $block_number <= $blocks_amount; $block_number++) {
    $blocks[YAMAPS_DEFAULT_BLOCK_DELTA . '_' . $block_number] = array(
      'info' => t('Yandex Map #!block_number', array('!block_number' => $block_number)),
      'cache' => DRUPAL_NO_CACHE,
    );
  }

  return $blocks;
}

/**
 * Implements hook_block_configure().
 *
 * @see yamaps_field_process()
 */
function yamaps_block_configure($delta = YAMAPS_DEFAULT_BLOCK_DELTA) {
  // Add elements from default field edit form.
  $settings = array(
    '#delta' => 'block-' . $delta,
    '#value' => array(
      'coords' => variable_get($delta . '_block_coords', NULL),
      'type' => variable_get($delta . '_block_type', 'yandex#map'),
      'placemarks' => variable_get($delta . '_block_placemarks', NULL),
      'lines' => variable_get($delta . '_block_lines', NULL),
      'polygons' => variable_get($delta . '_block_polygons', NULL),
      'routes' => variable_get($delta . '_block_routes', NULL),
    ),
  );

  // Map information.
  $coords = isset($settings['#value']['coords']) ? $settings['#value']['coords'] : NULL;
  $coords_array = drupal_json_decode($coords);
  $type = isset($settings['#value']['type']) ? $settings['#value']['type'] : 'yandex#map';
  $placemarks = isset($settings['#value']['placemarks']) ? $settings['#value']['placemarks'] : NULL;
  $placemarks_array = drupal_json_decode($placemarks);
  $lines = isset($settings['#value']['lines']) ? $settings['#value']['lines'] : NULL;
  $lines_array = drupal_json_decode($lines);
  $polygons = isset($settings['#value']['polygons']) ? $settings['#value']['polygons'] : NULL;
  $polygons_array = drupal_json_decode($polygons);
  $routes = isset($settings['#value']['routes']) ? $settings['#value']['routes'] : NULL;
  $routes_array = drupal_json_decode($routes);

  // Unique map id.
  $id = drupal_html_id(implode('-', array(
    'ymap',
    $settings['#delta'],
    'edit',
  )));

  // Unique map button id.
  $open_button_id = drupal_html_id(implode('-', array($id, 'open_button')));
  $close_button_id = drupal_html_id(implode('-', array($id, 'close_button')));
  $block_form_display_options = variable_get('yamaps_block_edit_display_options', array());
  $behaviors = array('scrollZoom', 'dblClickZoom', 'drag');

  $form['global_conf'] = array(
    '#markup' => l(t('Global configuration for "Yandex Maps" blocks'), 'admin/config/services/yamaps'),
  );

  $form[$delta . '_block_display_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display options'),
    '#tree' => TRUE,
  );

  $block_display_options = variable_get($delta . '_block_display_options', array());

  $form[$delta . '_block_display_options']['map_type'] = array(
    '#type' => 'radios',
    '#title' => t('Map type for end user'),
    '#options' => array(
      'dynamic' => t('Dynamic Map'),
      'static' => t('Static Map'),
    ),
    '#default_value' => isset($block_display_options['map_type']) ? $block_display_options['map_type'] : 'dynamic',
    '#description' => t('Configure "Yandex Map" type for end user.'),
  );

  $form[$delta . '_block_display_options']['display_type'] = array(
    '#type' => 'radios',
    '#title' => t('Map display style for end user'),
    '#options' => array(
      'map' => t('Map'),
      'map_button' => t('Map opened by button click'),
    ),
    '#default_value' => isset($block_display_options['display_type']) ? $block_display_options['display_type'] : 'map',
    '#required' => FALSE,
    '#description' => t('Configure how to display "Yandex Map" block for end user.'),
  );

  $form[$delta . '_block_display_options']['open_button_text'] = array(
    '#type' => 'textfield',
    '#title' => t('"Open" button text'),
    '#default_value' => isset($block_display_options['open_button_text']) ? t($block_display_options['open_button_text']) : t(YAMAPS_DEFAULT_OPEN_MAP_TEXT),
    '#required' => FALSE,
    '#description' => t('Text of button that opens map for end user.'),
    '#states' => array(
      'visible' => array(
        ':input[name="' . $delta . '_block_display_options[display_type]"]' => array(
          'value' => 'map_button',
        ),
      ),
    ),
  );

  $form[$delta . '_block_display_options']['close_button_text'] = array(
    '#type' => 'textfield',
    '#title' => t('"Close" button text'),
    '#default_value' => isset($block_display_options['close_button_text']) ? t($block_display_options['close_button_text']) : t(YAMAPS_DEFAULT_CLOSE_MAP_TEXT),
    '#required' => FALSE,
    '#description' => t('Text of button that closes map for end user.'),
    '#states' => array(
      'visible' => array(
        ':input[name="' . $delta . '_block_display_options[display_type]"]' => array(
          'value' => 'map_button',
        ),
      ),
    ),
  );

  $form[$delta . '_block_controls'] = array(
    '#title' => t('Show controls'),
    '#type' => 'checkbox',
    '#default_value' => variable_get($delta . '_block_controls', TRUE),
    '#states' => array(
      'visible' => array(
        ':input[name="' . $delta . '_block_display_options[map_type]"]' => array(
          'value' => 'dynamic',
        ),
      ),
    ),
  );

  $form[$delta . '_block_traffic'] = array(
    '#title' => t('Show traffic'),
    '#type' => 'checkbox',
    '#default_value' => variable_get($delta . '_block_traffic', FALSE),
  );

  $form[$delta . '_block_behaviors'] = array(
    '#title' => t('Available mouse events'),
    '#type' => 'checkboxes',
    '#options' => yamaps_get_behaviors_list(),
    '#default_value' => variable_get($delta . '_block_behaviors', array()),
    '#states' => array(
      'visible' => array(
        ':input[name="' . $delta . '_block_display_options[map_type]"]' => array(
          'value' => 'dynamic',
        ),
      ),
    ),
  );

  $form[$delta . '_block_width'] = array(
    '#title' => t('Map width'),
    '#field_suffix' => ' ' . t('in pixels (px) or percentage (%) for dynamic map, in pixels (px) for static map.'),
    '#type' => 'textfield',
    '#default_value' => variable_get($delta . '_block_width', YAMAPS_DEFAULT_BLOCK_MAP_WIDTH),
    '#size' => 5,
    '#element_validate' => array('yamaps_field_validate_pixels_percentage'),
    '#required' => TRUE,
  );

  $form[$delta . '_block_height'] = array(
    '#title' => t('Map height'),
    '#field_suffix' => ' ' . t('in pixels (px) or percentage (%) for dynamic map, in pixels (px) for static map.'),
    '#type' => 'textfield',
    '#default_value' => variable_get($delta . '_block_height', YAMAPS_DEFAULT_BLOCK_MAP_HEIGHT),
    '#size' => 5,
    '#element_validate' => array('yamaps_field_validate_pixels_percentage'),
    '#required' => TRUE,
  );

  $open_map_button = FALSE;
  $close_map_button = FALSE;
  if (isset($block_form_display_options['display_type']) &&
    ($block_form_display_options['display_type'] == 'map_button')) {
    $form['open_map_button'] = array(
      '#type' => 'html_tag',
      '#tag' => 'div',
      '#value' => t($block_form_display_options['open_button_text']),
      '#attributes' => array(
        'id' => $open_button_id,
        'class' => array('open_yamap_button'),
        'mapId' => $id,
      ),
    );

    $form['close_map_button'] = array(
      '#type' => 'html_tag',
      '#tag' => 'div',
      '#value' => t($block_form_display_options['close_button_text']),
      '#attributes' => array(
        'id' => $close_button_id,
        'class' => array('close_yamap_button', 'element-invisible'),
        'mapId' => $id,
      ),
    );
    $open_map_button = TRUE;
    $close_map_button = TRUE;
  }

  $container_width = isset($block_form_display_options['width']) ? $block_form_display_options['width'] : YAMAPS_DEFAULT_ADMIN_UI_MAP_WIDTH;
  $container_height = isset($block_form_display_options['height']) ? $block_form_display_options['height'] : YAMAPS_DEFAULT_ADMIN_UI_MAP_HEIGHT;

  // Map container.
  $form['map'] = array(
    '#type' => 'html_tag',
    '#tag' => 'div',
    '#value' => '',
    '#attributes' => array(
      'id' => $id,
      'class' => $open_map_button && $close_map_button ? array('yamaps-map-container', 'element-invisible') : array('yamaps-map-container'),
      'style' => 'width: ' . $container_width . '; height: ' . $container_height . ';',
    ),
  );

  $form['coords'] = array(
    '#type' => 'textfield',
    '#title' => t('Coordinates'),
    '#default_value' => $coords,
    '#attributes' => array(
      'class' => array('field-yamaps-coords-' . $id),
      'style' => 'width: 100%;',
    ),
    '#description' => t('Search for object on the map to fill this field.'),
    '#required' => TRUE,
  );

  // Hidden elements to save map information.
  $form['type'] = array(
    '#type' => 'hidden',
    '#title' => t('Type'),
    '#default_value' => $type,
    '#attributes' => array('class' => array('field-yamaps-type-' . $id)),
  );

  $form['placemarks'] = array(
    '#type' => 'hidden',
    '#title' => t('Placemarks'),
    '#default_value' => $placemarks,
    '#attributes' => array('class' => array('field-yamaps-placemarks-' . $id)),
  );

  $form['lines'] = array(
    '#type' => 'hidden',
    '#title' => t('Lines'),
    '#default_value' => $lines,
    '#attributes' => array('class' => array('field-yamaps-lines-' . $id)),
  );

  $form['polygons'] = array(
    '#type' => 'hidden',
    '#title' => t('Polygons'),
    '#default_value' => $polygons,
    '#attributes' => array('class' => array('field-yamaps-polygons-' . $id)),
  );

  $form['routes'] = array(
    '#type' => 'hidden',
    '#title' => t('Routes'),
    '#default_value' => $routes,
    '#attributes' => array('class' => array('field-yamaps-routes-' . $id)),
  );

  // Map description.
  $form['#description'] = array(
    '#type' => 'html_tag',
    '#tag' => 'div',
    '#value' => l(
      t('Terms of service «API Yandex.Maps»'),
      YAMAPS_LEGAL_AGREEMENT_URL,
      array('attributes' => array('target' => '_blank'))
    ),
    '#attributes' => array(
      'class' => array('yamaps-terms'),
    ),
  );

  // Map initialization parameters.
  $map = array(
    'init' => array(
      'center' => $coords_array['center'],
      'zoom' => $coords_array['zoom'],
      'type' => $type,
      'behaviors' => $behaviors,
    ),
    'display_options' => array(
      'display_type' => isset($block_form_display_options['display_type']) ? $block_form_display_options['display_type'] : 'map',
    ),
    'controls' => 1,
    'traffic' => 0,
    'placemarks' => $placemarks_array,
    'lines' => $lines_array,
    'polygons' => $polygons_array,
    'routes' => $routes_array,
    'edit' => TRUE,
  );

  // Add information about this map to js.
  $form['#attached']['js'][] = array(
    'data' => array('yamaps' => array($id => $map)),
    'type' => 'setting',
  );
  // Load library.
  $form['#attached']['library'][] = array('yamaps', 'yamaps.full');

  return $form;
}

/**
 * Implements hook_block_save().
 */
function yamaps_block_save($delta = YAMAPS_DEFAULT_BLOCK_DELTA, $edit = array()) {
  variable_set($delta . '_block_controls', $edit[$delta . '_block_controls']);
  variable_set($delta . '_block_traffic', $edit[$delta . '_block_traffic']);
  variable_set($delta . '_block_behaviors', $edit[$delta . '_block_behaviors']);
  variable_set($delta . '_block_width', $edit[$delta . '_block_width']);
  variable_set($delta . '_block_height', $edit[$delta . '_block_height']);
  variable_set($delta . '_block_coords', $edit['coords']);
  variable_set($delta . '_block_type', $edit['type']);
  variable_set($delta . '_block_placemarks', $edit['placemarks']);
  variable_set($delta . '_block_lines', $edit['lines']);
  variable_set($delta . '_block_polygons', $edit['polygons']);
  variable_set($delta . '_block_routes', $edit['routes']);
  variable_set($delta . '_block_display_options', $edit[$delta . '_block_display_options']);
}

/**
 * Implements hook_block_view().
 */
function yamaps_block_view($delta = YAMAPS_DEFAULT_BLOCK_DELTA) {
  $block['subject'] = t('Yandex Map');
  $block['content'] = yamaps_block_content($delta);

  return $block;
}

/**
 * Returns block content.
 *
 * @see yamaps_block_view()
 */
function yamaps_block_content($delta) {
  $block_output = '';
  $display_options = variable_get($delta . '_block_display_options', array());

  // Map information.
  $coords = drupal_json_decode(variable_get($delta . '_block_coords', NULL));
  if (empty($coords)) {
    return $block_output;
  }

  $id = drupal_html_id(implode('-', array('ymap', 'block', $delta)));
  $open_button_id = drupal_html_id(implode('-', array($id, 'open_button')));
  $close_button_id = drupal_html_id(implode('-', array($id, 'close_button')));

  $width = variable_get($delta . '_block_width', YAMAPS_DEFAULT_BLOCK_MAP_WIDTH);
  $height = variable_get($delta . '_block_height', YAMAPS_DEFAULT_BLOCK_MAP_HEIGHT);

  $traffic = variable_get($delta . '_block_traffic', FALSE);
  $controls = variable_get($delta . '_block_controls', TRUE);

  $block_type = variable_get($delta . '_block_type', 'yandex#map');
  $map_type = isset($display_options['map_type']) ? $display_options['map_type'] : 'dynamic';
  $display_type = isset($display_options['display_type']) ? $display_options['display_type'] : 'map';

  $placemarks = drupal_json_decode(variable_get($delta . '_block_placemarks', NULL));
  $lines = drupal_json_decode(variable_get($delta . '_block_lines', NULL));
  $polygons = drupal_json_decode(variable_get($delta . '_block_polygons', NULL));
  $routes = drupal_json_decode(variable_get($delta . '_block_routes', NULL));

  // Set default 'Open map' text and 'Close map' text.
  if (!isset($display_options['open_button_text'])) {
    $display_options['open_button_text'] = YAMAPS_DEFAULT_OPEN_MAP_TEXT;
  }
  if (!isset($display_options['close_button_text'])) {
    $display_options['close_button_text'] = YAMAPS_DEFAULT_CLOSE_MAP_TEXT;
  }

  switch ($map_type) {
    case 'dynamic':
      // Map initialization parameters.
      $map = array(
        'init' => array(
          'center' => $coords['center'],
          'zoom' => $coords['zoom'],
          'type' => $block_type,
          'behaviors' => array_values(array_filter(variable_get($delta . '_block_behaviors', array()))),
        ),
        'display_options' => array(
          'display_type' => $display_type,
        ),
        'controls' => $controls,
        'traffic' => $traffic,
        'placemarks' => $placemarks,
        'lines' => $lines,
        'polygons' => $polygons,
        'routes' => $routes,
        'edit' => FALSE,
      );

      $open_map_button = FALSE;
      $close_map_button = FALSE;
      if (isset($display_options['display_type']) && $display_options['display_type'] == 'map_button') {
        $block_output['open_map_button'] = array(
          '#type' => 'html_tag',
          '#tag' => 'div',
          '#value' => t($display_options['open_button_text']),
          '#attributes' => array(
            'id' => $open_button_id,
            'class' => array('open_yamap_button'),
            'mapId' => $id,
          ),
        );

        $open_map_button = TRUE;
        $close_map_button = TRUE;
      }

      // Return map container div.
      $block_output['map_container'] = array(
        '#type' => 'html_tag',
        '#tag' => 'div',
        '#attributes' => array(
          'id' => $id,
          'style' => 'width:' . $width . '; height:' . $height . ';',
          'class' => $open_map_button && $close_map_button ? array('yamaps-map-container', 'element-invisible') : array('yamaps-map-container'),
        ),
        '#value' => '',
      );
      // Fix issue with block close button.
      if (isset($display_options['display_type']) && $display_options['display_type'] == 'map_button') {
        $block_output['close_map_button'] = array(
          '#type' => 'html_tag',
          '#tag' => 'div',
          '#value' => t($display_options['close_button_text']),
          '#attributes' => array(
            'id' => $close_button_id,
            'class' => array('close_yamap_button', 'element-invisible'),
            'mapId' => $id,
          ),
        );
      }

      // Adding map to js.
      $block_output['#attached']['js'][] = array(
        'data' => array('yamaps' => array($id => $map)),
        'type' => 'setting',
      );
      // Load library.
      $block_output['#attached']['library'][] = array('yamaps', 'yamaps.full');
      break;

    case 'static':
      $params = array();
      $params['ll'] = end($coords['center']) . ',' . reset($coords['center']);
      $params['z'] = $coords['zoom'];
      $params['size'] = intval($width) . ',' . intval($height);

      // 2.x to 1.x types.
      $map_types = array(
        'yandex#map' => 'map',
        'yandex#satellite' => 'sat',
        'yandex#hybrid' => 'sat,skl',
        'yandex#publicMap' => 'pmap',
        'yandex#publicMapHybrid' => 'sat,pskl',
      );
      $params['l'] = $map_types[$block_type];

      if (variable_get($delta . '_block_traffic', FALSE)) {
        $params['l'] .= ',trf';
      }

      // 2.x to 1.x colors.
      $colors21 = array(
        'blue' => 'bl',
        'lightblue' => 'lb',
        'night' => 'nt',
        'darkblue' => 'db',
        'green' => 'gn',
        'white' => 'wt',
        'red' => 'rd',
        'orange' => 'or',
        'darkorange' => 'do',
        'yellow' => 'yw',
        'violet' => 'vv',
        'pink' => 'pn',
      );

      // 2.x to hex colors.
      $colors = array(
        'blue' => '006cff',
        'lightblue' => '66c7ff',
        'night' => '004056',
        'darkblue' => '00339a',
        'green' => '33cc00',
        'white' => 'ffffff',
        'red' => 'ff0000',
        'orange' => 'ffb400',
        'darkorange' => 'ff6600',
        'yellow' => 'ffea00',
        'violet' => 'b832fd',
        'pink' => 'fd32fb',
      );

      // Placemarks.
      if ($placemarks) {
        $pt = array();

        foreach ($placemarks as $placemark) {
          $pm = end($placemark['coords']) . ',' . reset($placemark['coords']) . ',';
          $pm .= 'pm2';
          $pm .= $colors21[$placemark['params']['color']];
          $pm .= 'm';
          $pt[] = $pm;
        }

        $params['pt'] = implode('~', $pt);
      }

      // Lines and polygons.
      $pl = array();

      if ($lines) {
        foreach ($lines as $line) {
          $opts = $line['params'];
          $pm = 'c:' . $colors[$opts['strokeColor']] . dechex(255 * $opts['opacity']) . ',';
          $pm .= 'w:' . $opts['strokeWidth'] . ',';
          $c = array();

          foreach ($line['coords'] as $coords) {
            $c[] = end($coords);
            $c[] = reset($coords);
          }

          $pm .= implode(',', $c);
          $pl[] = $pm;
        }
      }
      if ($polygons) {
        foreach ($polygons as $polygon) {
          $opts = $polygon['params'];
          $opa = dechex(255 * $opts['opacity']);
          $pm = 'c:' . $colors[$opts['strokeColor']] . $opa . ',';
          $pm .= 'f:' . $colors[$opts['fillColor']] . $opa . ',';
          $pm .= 'w:' . $opts['strokeWidth'] . ',';
          $c = array();

          foreach ($polygon['coords'] as $coords_array) {
            foreach ($coords_array as $coords) {
              $c[] = end($coords);
              $c[] = reset($coords);
            }
          }

          $pm .= implode(',', $c);
          $pl[] = $pm;
        }
      }

      if (!empty($pl)) {
        $params['pl'] = implode('~', $pl);
      }

      $open_map_button = FALSE;
      $close_map_button = FALSE;
      if ($display_type == 'map_button') {
        $block_output['open_button_text'] = array(
          '#type' => 'html_tag',
          '#tag' => 'div',
          '#value' => t($display_options['open_button_text']),
          '#attributes' => array(
            'id' => $open_button_id,
            'class' => array('open_yamap_button'),
            'mapId' => $id,
          ),
        );

        $block_output['close_button_text'] = array(
          '#type' => 'html_tag',
          '#tag' => 'div',
          '#value' => t($display_options['close_button_text']),
          '#attributes' => array(
            'id' => $close_button_id,
            'class' => array('close_yamap_button', 'element-invisible'),
            'mapId' => $id,
          ),
        );
        $open_map_button = TRUE;
        $close_map_button = TRUE;
      }

      // Return map container div with image.
      $block_output['map_container'] = array(
        '#type' => 'html_tag',
        '#tag' => 'div',
        '#attributes' => array(
          'id' => $id,
          'style' => 'width:' . $width . 'px; height:' . $height . 'px;',
          'class' => $open_map_button && $close_map_button ? array('yamaps-map-container', 'element-invisible') : array('yamaps-map-container'),
        ),
        '#value' => theme('image', array(
          'path' => url(YAMAPS_STATIC_API_URL, array(
            'query' => $params,
            'external' => TRUE,
          )),
          'width' => $width,
          'height' => $height,
          'title' => t('Yandex Map'),
        )),
      );

      // Map initialization parameters.
      $maps[$id] = array(
        'display_options' => array(
          'display_type' => $display_type,
        ),
      );

      // Adding map to js and load library.
      $block_output['#attached']['js'][] = array(
        'data' => array('yamapsStatic' => $maps),
        'type' => 'setting',
      );
      $block_output['#attached']['library'][] = array('yamaps', 'yamaps.full');
      break;
  }

  return $block_output;
}
