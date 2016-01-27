<?php

/**
 * @file
 * Yandex Maps field formatter.
 */

/**
 * Implements hook_field_formatter_info().
 */
function yamaps_field_formatter_info() {
  return array(
    // Dynamic map formatter.
    YAMAPS_DYNAMIC_FORMATTER => array(
      'label' => t('Dynamic'),
      'field types' => array('field_yamaps'),
      'settings' => array(
        'controls' => TRUE,
        'traffic' => TRUE,
        'width' => YAMAPS_DEFAULT_DYNAMIC_WIDTH,
        'height' => YAMAPS_DEFAULT_DYNAMIC_HEIGHT,
        'behaviors' => array(
          'clickZoom',
          'dblClickZoom',
          'drag',
          'scrollZoom',
          'ruler',
          'rightMouseButtonMagnifier',
        ),
        'yamaps_display_options' => array(
          'display_type' => 'map',
          'open_button_text' => YAMAPS_DEFAULT_OPEN_MAP_TEXT,
          'close_button_text' => YAMAPS_DEFAULT_CLOSE_MAP_TEXT,
        ),
      ),
    ),
    // Static map formatter.
    YAMAPS_STATIC_FORMATTER => array(
      'label' => t('Static'),
      'field types' => array('field_yamaps'),
      'settings' => array(
        's_traffic' => FALSE,
        's_width' => YAMAPS_DEFAULT_STATIC_WIDTH,
        's_height' => YAMAPS_DEFAULT_STATIC_HEIGHT,
        'yamaps_display_options_static' => array(
          'display_type_static' => 'map',
          'open_button_text_static' => YAMAPS_DEFAULT_OPEN_MAP_TEXT,
          'close_button_text_static' => YAMAPS_DEFAULT_CLOSE_MAP_TEXT,
        ),
      ),
    ),
    // Simple text formatter.
    YAMAPS_TEXT_FORMATTER => array(
      'label' => t('Yandex map formatter from the one-line address'),
      'field types' => array('text'),
      'settings' => array(
        'width' => YAMAPS_DEFAULT_DYNAMIC_WIDTH,
        'height' => YAMAPS_DEFAULT_DYNAMIC_HEIGHT,
        'placemark' => array(
          'placemark_display' => TRUE,
          'placemark_text' => '[address]',
          'placemark_color' => 'blue',
        ),
        'behaviors' => array_keys(yamaps_get_behaviors_list()),
        'control' => TRUE,
        'traffic' => FALSE,
      ),
    ),
  );
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function yamaps_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $element = array();

  switch ($display['type']) {
    case YAMAPS_DYNAMIC_FORMATTER:
      $element['yamaps_display_options'] = array(
        '#type' => 'fieldset',
        '#title' => t('Display options'),
        '#tree' => TRUE,
      );

      $element['yamaps_display_options']['display_type'] = array(
        '#type' => 'radios',
        '#title' => t('Map display style for end user'),
        '#options' => array(
          'map' => t('Map'),
          'map_button' => t('Map opened by button click'),
        ),
        '#default_value' => $settings['yamaps_display_options']['display_type'],
        '#required' => FALSE,
        '#description' => t('Configure how to display "Yandex Map" field for end user.'),
      );

      $element['yamaps_display_options']['open_button_text'] = array(
        '#type' => 'textfield',
        '#title' => t('"Open" button text'),
        '#default_value' => $settings['yamaps_display_options']['open_button_text'],
        '#required' => FALSE,
        '#description' => t('Text of button that opens map for end user.'),
        '#states' => array(
          'visible' => array(
            ':input[name="fields[' . $field['field_name'] . '][settings_edit_form][settings][yamaps_display_options][display_type]"]' => array(
              'value' => 'map_button',
            ),
          ),
        ),
      );

      $element['yamaps_display_options']['close_button_text'] = array(
        '#type' => 'textfield',
        '#title' => t('"Close" button text'),
        '#default_value' => isset($settings['yamaps_display_options']['close_button_text']) ? $settings['yamaps_display_options']['close_button_text'] : t(YAMAPS_DEFAULT_CLOSE_MAP_TEXT),
        '#required' => FALSE,
        '#description' => t('Text of button that closes map for end user.'),
        '#states' => array(
          'visible' => array(
            ':input[name="fields[' . $field['field_name'] . '][settings_edit_form][settings][yamaps_display_options][display_type]"]' => array(
              'value' => 'map_button',
            ),
          ),
        ),
      );

      $element['controls'] = array(
        '#title' => t('Show controls'),
        '#type' => 'checkbox',
        '#default_value' => $settings['controls'],
      );

      $element['traffic'] = array(
        '#title' => t('Show traffic'),
        '#type' => 'checkbox',
        '#default_value' => $settings['traffic'],
      );

      $element['behaviors'] = array(
        '#title' => t('Available mouse events'),
        '#type' => 'checkboxes',
        '#options' => yamaps_get_behaviors_list(),
        '#default_value' => $settings['behaviors'],
      );

      $element['width'] = array(
        '#title' => t('Map width'),
        '#field_suffix' => ' ' . t('in pixels or in percentage'),
        '#type' => 'textfield',
        '#default_value' => $settings['width'],
        '#size' => 5,
        '#element_validate' => array('yamaps_field_validate_pixels_percentage'),
        '#required' => TRUE,
      );

      $element['height'] = array(
        '#title' => t('Map height'),
        '#field_suffix' => ' ' . t('in pixels or in percentage'),
        '#type' => 'textfield',
        '#default_value' => $settings['height'],
        '#size' => 5,
        '#element_validate' => array('yamaps_field_validate_pixels_percentage'),
        '#required' => TRUE,
      );

      break;

    case YAMAPS_TEXT_FORMATTER:
      $element['width'] = array(
        '#title' => t('Map width'),
        '#field_suffix' => ' ' . t('in pixels or in percentage'),
        '#type' => 'textfield',
        '#default_value' => $settings['width'],
        '#size' => 5,
        '#element_validate' => array('yamaps_field_validate_pixels_percentage'),
        '#required' => TRUE,
      );

      $element['behaviors'] = array(
        '#title' => t('Available mouse events'),
        '#type' => 'checkboxes',
        '#options' => yamaps_get_behaviors_list(),
        '#default_value' => $settings['behaviors'],
      );

      $element['height'] = array(
        '#title' => t('Map height'),
        '#field_suffix' => ' ' . t('in pixels or in percentage'),
        '#type' => 'textfield',
        '#default_value' => $settings['height'],
        '#size' => 5,
        '#element_validate' => array('yamaps_field_validate_pixels_percentage'),
        '#required' => TRUE,
      );
      // Placemark options.
      $element['placemark'] = array(
        '#title' => t('Placemark options'),
        '#type' => 'fieldset',
      );

      $element['placemark']['placemark_display'] = array(
        '#type' => 'checkbox',
        '#title' => t('Show placemark?'),
        '#default_value' => $settings['placemark']['placemark_display'],
      );

      $element['placemark']['placemark_text'] = array(
        '#type' => 'textfield',
        '#title' => t('Placemark text'),
        '#description' => t('Print <strong>[address]</strong> to display address from the field'),
        '#states' => array(
          'visible' => array(
            ':input[name="fields[' . $field['field_name'] . '][settings_edit_form][settings][placemark][placemark_display]"]' => array(
              'checked' => TRUE
            ),
          ),
        ),
        '#default_value' => $settings['placemark']['placemark_text'],
      );

      foreach (array_keys(yamaps_get_colors()) as $color) {
        $options[$color] = t($color);
      }

      $element['placemark']['placemark_color'] = array(
        '#title' => 'Placemark color',
        '#type' => 'radios',
        '#options' => $options,
        '#states' => array(
          'visible' => array(
            ':input[name="fields[' . $field['field_name'] . '][settings_edit_form][settings][placemark][placemark_display]"]' => array(
              'checked' => TRUE
            ),
          )
        ),
        '#default_value' => $settings['placemark']['placemark_color'],
      );

      $element['control'] = array(
        '#title' => t('Enable control'),
        '#type' => 'checkbox',
        '#default_value' => $settings['control'],
      );

      $element['traffic'] = array(
        '#type' => 'checkbox',
        '#title' => t('Show traffic?'),
        '#default_value' => $settings['traffic'],
      );

      break;

    case YAMAPS_STATIC_FORMATTER:
      $element['yamaps_display_options_static']['display_type_static'] = array(
        '#type' => 'radios',
        '#title' => t('Map display style for end user'),
        '#options' => array(
          'map' => t('Map'),
          'map_button' => t('Map opened by button click'),
        ),
        '#default_value' => $settings['yamaps_display_options_static']['display_type_static'],
        '#required' => FALSE,
        '#description' => t('Configure how to display "Yandex Map" field for end user.'),
      );

      $element['yamaps_display_options_static']['open_button_text_static'] = array(
        '#type' => 'textfield',
        '#title' => t('"Open" button text'),
        '#default_value' => $settings['yamaps_display_options_static']['open_button_text_static'],
        '#required' => FALSE,
        '#description' => t('Text of button that opens map for end user.'),
        '#states' => array(
          'visible' => array(
            ':input[name="fields[' . $field['field_name'] . '][settings_edit_form][settings][yamaps_display_options_static][display_type_static]"]' => array(
              'value' => 'map_button',
            ),
          ),
        ),
      );

      $element['yamaps_display_options_static']['close_button_text_static'] = array(
        '#type' => 'textfield',
        '#title' => t('"Close" button text'),
        '#default_value' => isset($settings['yamaps_display_options_static']['close_button_text_static']) ? $settings['yamaps_display_options_static']['close_button_text_static'] : t(YAMAPS_DEFAULT_CLOSE_MAP_TEXT),
        '#required' => FALSE,
        '#description' => t('Text of button that closes map for end user.'),
        '#states' => array(
          'visible' => array(
            ':input[name="fields[' . $field['field_name'] . '][settings_edit_form][settings][yamaps_display_options_static][display_type_static]"]' => array(
              'value' => 'map_button',
            ),
          ),
        ),
      );

      $element['s_traffic'] = array(
        '#title' => t('Show traffic'),
        '#type' => 'checkbox',
        '#default_value' => $settings['s_traffic'],
      );

      $element['s_width'] = array(
        '#title' => t('Map width'),
        '#field_suffix' => ' ' . t('pixels'),
        '#type' => 'textfield',
        '#default_value' => $settings['s_width'],
        '#size' => 5,
        '#element_validate' => array('element_validate_integer_positive'),
        '#required' => TRUE,
      );

      $element['s_height'] = array(
        '#title' => t('Map height'),
        '#field_suffix' => ' ' . t('pixels'),
        '#type' => 'textfield',
        '#default_value' => $settings['s_height'],
        '#size' => 5,
        '#element_validate' => array('element_validate_integer_positive'),
        '#required' => TRUE,
      );
      break;
  }
  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function yamaps_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $summary = array();

  switch ($display['type']) {
    case YAMAPS_DYNAMIC_FORMATTER:
      $summary[] = t('Show controls: @controls', array('@controls' => $settings['controls'] ? t('yes') : t('no')));
      $summary[] = t('Show traffic: @traffic', array('@traffic' => $settings['traffic'] ? t('yes') : t('no')));
      $summary[] = t('Map size: @width x @height', array(
        '@width' => $settings['width'],
        '@height' => $settings['height'],
      ));

      $available_events = array_filter($display['settings']['behaviors']);

      if (count($available_events) > 0) {
        $behaviors_list = yamaps_get_behaviors_list();
        $active_behaviors = array_values(array_filter($display['settings']['behaviors']));
        $prepared_behaviors_list = array();

        foreach ($active_behaviors as $val) {
          $prepared_behaviors_list[] = $behaviors_list[$val];
        }

        $summary[] = t('Enabled mouse event: !list', array('!list' => implode(', ', $prepared_behaviors_list)));
      }
      else {
        $summary[] = t('All mouse events are disabled.');
      }

      $summary[] = t('Display type: @type', array('@type' => $settings['yamaps_display_options']['display_type']));

      if ($settings['yamaps_display_options']['display_type'] == 'map_button') {
        $summary[] = t('"Open" button text: @text', array('@text' => $settings['yamaps_display_options']['open_button_text']));
        $summary[] = t('"Close" button text: @text', array('@text' => $settings['yamaps_display_options']['close_button_text']));
      }
      break;

    case YAMAPS_STATIC_FORMATTER:
      $summary[] = t('Show traffic: @traffic', array('@traffic' => $settings['s_traffic'] ? t('yes') : t('no')));
      $summary[] = t('Map size: @width x @height', array(
        '@width' => $settings['s_width'] . 'px',
        '@height' => $settings['s_height'] . 'px',
      ));

      $summary[] = t('Display type: @type', array('@type' => $settings['yamaps_display_options_static']['display_type_static']));

      if ($settings['yamaps_display_options_static']['display_type_static'] == 'map_button') {
        $summary[] = t('"Open" button text: @text', array('@text' => $settings['yamaps_display_options_static']['open_button_text_static']));
        $summary[] = t('"Close" button text: @text', array('@text' => $settings['yamaps_display_options_static']['close_button_text_static']));
      }
      break;

    case YAMAPS_TEXT_FORMATTER:
      $summary[] = t('Show controls: @controls', array('@controls' => $settings['control'] ? t('yes') : t('no')));
      $summary[] = t('Show traffic: @traffic', array('@traffic' => $settings['traffic'] ? t('yes') : t('no')));
      $summary[] = t('Map size: @width x @height', array(
        '@width' => $settings['width'],
        '@height' => $settings['height'],
      ));

      $summary[] = t('Display placemarks: @placemark', array('@placemark' => $settings['placemark']['placemark_display'] ? t('yes') : t('no')));
      if ($settings['placemark']['placemark_display']) {
        $summary[] = t('Text on placemark: @text', array('@text' => $settings['placemark']['placemark_text']));
        $summary[] = t('Placemark color: @color', array('@color' => t($settings['placemark']['placemark_color'])));
      }
      $behaviors = implode(', ', $settings['behaviors']);
      $summary[] = t('Available behaviors: @behaviors', array('@behaviors' => $behaviors));
      break;
  }

  return implode('<br />', $summary);
}

/**
 * Implements hook_field_formatter_view().
 */
function yamaps_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  if (empty($items)) {
    return array();
  }

  $element = array();
  $settings = $display['settings'];
  $entity_info = entity_get_info($entity_type);
  $entity_id = uniqid();

  if (isset($entity_info['entity keys']['id']) && isset($entity->{$entity_info['entity keys']['id']})) {
    $entity_id = $entity->{$entity_info['entity keys']['id']};
  }

  switch ($display['type']) {
    case YAMAPS_DYNAMIC_FORMATTER:
      $maps = array();
      foreach ($items as $delta => $item) {
        // Ensure that field with empty coordinates doesn't show up.
        if (empty($item['coords']) || $item['hide'] == 1) {
          continue;
        }

        // Map id.
        $id = drupal_html_id(implode('-', array(
          'ymap',
          $entity->type,
          $entity_id,
          $field['field_name'],
          $delta,
        )));

        // Unique map button id.
        $open_button_id = drupal_html_id(implode('-', array($id, 'open_button')));
        $close_button_id = drupal_html_id(implode('-', array($id, 'close_button')));

        // Coordinates of map center.
        $coords = drupal_json_decode($item['coords']);

        if (isset($settings['yamaps_display_options']['display_type']) && $settings['yamaps_display_options']['display_type'] == 'map_button') {
          $element[$delta]['open_button_text'] = array(
            '#type' => 'html_tag',
            '#tag' => 'div',
            '#value' => isset($settings['yamaps_display_options']['open_button_text']) ? t($settings['yamaps_display_options']['open_button_text']) : t(YAMAPS_DEFAULT_OPEN_MAP_TEXT),
            '#attributes' => array(
              'id' => $open_button_id,
              'class' => array('open_yamap_button'),
              'mapId' => $id,
            ),
          );

          $element[$delta]['close_button_text'] = array(
            '#type' => 'html_tag',
            '#tag' => 'div',
            '#value' => isset($settings['yamaps_display_options']['close_button_text']) ? t($settings['yamaps_display_options']['close_button_text']) : t(YAMAPS_DEFAULT_CLOSE_MAP_TEXT),
            '#attributes' => array(
              'id' => $close_button_id,
              'class' => array('close_yamap_button', 'element-invisible'),
              'mapId' => $id,
            ),
          );
        }

        $hide_map = (isset($settings['yamaps_display_options']['display_type']) &&
          $settings['yamaps_display_options']['display_type'] == 'map_button');
        $map_class = array('yamaps-map-container');
        $map_class[] = $hide_map ? 'element-invisible' : '';

        // Return map container div.
        $element[$delta]['map_container'] = array(
          '#type' => 'html_tag',
          '#tag' => 'div',
          '#attributes' => array(
            'id' => $id,
            'style' => 'width:' . $settings['width'] . '; height:' . $settings['height'] . ';',
            'class' => $map_class,
          ),
          '#value' => '',
        );

        // Map initialization parameters.
        $maps[$id] = array(
          'init' => array(
            'center' => $coords['center'],
            'zoom' => $coords['zoom'],
            'type' => $item['type'] ? $item['type'] : 'yandex#map',
            'behaviors' => array_values(array_filter($settings['behaviors'])),
          ),
          'display_options' => array(
            'display_type' => $settings['yamaps_display_options']['display_type'],
          ),
          'controls' => $settings['controls'],
          'traffic' => $settings['traffic'],
          'placemarks' => drupal_json_decode($item['placemarks']),
          'lines' => drupal_json_decode($item['lines']),
          'polygons' => drupal_json_decode($item['polygons']),
          'routes' => drupal_json_decode($item['routes']),
          'edit' => FALSE,
        );
      }

      // Adding map to js and load library.
      $element['#attached']['js'][] = array(
        'data' => array('yamaps' => $maps),
        'type' => 'setting',
      );
      $element['#attached']['library'][] = array('yamaps', 'yamaps.full');

      // Fixing formatter rendered by Views module (display and preview modes).
      $element[$delta]['#attached']['js'] = $element['#attached']['js'];
      $element[$delta]['#attached']['library'] = $element['#attached']['library'];

      break;

    case YAMAPS_STATIC_FORMATTER:
      foreach ($items as $delta => $item) {
        // Ensure that field with empty coordinates doesn't show up.
        if (empty($item['coords']) || $item['hide'] == 1) {
          continue;
        }

        $coords = drupal_json_decode($item['coords']);
        $params = array();
        $params['ll'] = end($coords['center']) . ',' . reset($coords['center']);
        $params['z'] = $coords['zoom'];
        $params['size'] = $settings['s_width'] . ',' . $settings['s_height'];

        // 2.x to 1.x types.
        $map_types = array(
          'yandex#map' => 'map',
          'yandex#satellite' => 'sat',
          'yandex#hybrid' => 'sat,skl',
          'yandex#publicMap' => 'pmap',
          'yandex#publicMapHybrid' => 'sat,pskl',
        );
        $params['l'] = $map_types[$item['type'] ? $item['type'] : 'yandex#map'];

        if ($settings['s_traffic']) {
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
        $colors = yamaps_get_colors();

        // Placemarks.
        if ($item['placemarks']) {
          $pt = array();

          foreach (drupal_json_decode($item['placemarks']) as $placemark) {
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

        if ($item['lines']) {
          foreach (drupal_json_decode($item['lines']) as $line) {
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
        if ($item['polygons']) {
          foreach (drupal_json_decode($item['polygons']) as $polygon) {
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

        // Map id and parameters.
        $id = drupal_html_id(implode('-', array(
          'ymap',
          $entity->type,
          $entity_id,
          $field['field_name'],
          $delta,
        )));

        $open_button_id = drupal_html_id(implode('-', array($id, 'open_button')));
        $close_button_id = drupal_html_id(implode('-', array($id, 'close_button')));

        $hide_map = (isset($settings['yamaps_display_options_static']['display_type_static']) &&
          $settings['yamaps_display_options_static']['display_type_static'] == 'map_button');
        $map_class = $hide_map ? 'element-invisible' : '';

        if ($hide_map) {
          $element[$delta]['open_button_text'] = array(
            '#type' => 'html_tag',
            '#tag' => 'div',
            '#value' => isset($settings['yamaps_display_options_static']['open_button_text_static']) ? t($settings['yamaps_display_options_static']['open_button_text_static']) : t(YAMAPS_DEFAULT_OPEN_MAP_TEXT),
            '#attributes' => array(
              'id' => $open_button_id,
              'class' => array('open_yamap_button'),
              'mapId' => $id,
            ),
          );

          $element[$delta]['close_button_text'] = array(
            '#type' => 'html_tag',
            '#tag' => 'div',
            '#value' => isset($settings['yamaps_display_options_static']['close_button_text_static']) ? t($settings['yamaps_display_options_static']['close_button_text_static']) : t(YAMAPS_DEFAULT_CLOSE_MAP_TEXT),
            '#attributes' => array(
              'id' => $close_button_id,
              'class' => array('close_yamap_button', 'element-invisible'),
              'mapId' => $id,
            ),
          );
        }

        // Return map container div with image.
        $element[$delta]['map_container'] = array(
          '#type' => 'html_tag',
          '#tag' => 'div',
          '#attributes' => array(
            'id' => $id,
            'style' => 'width:' . $settings['s_width'] . 'px; height:' . $settings['s_height'] . 'px;',
            'class' => array($map_class),
          ),
          '#value' => theme('image', array(
            'path' => url(YAMAPS_STATIC_API_URL, array('query' => $params, 'external' => TRUE)),
            'width' => $settings['s_width'],
            'height' => $settings['s_height'],
            'title' => t('Yandex Map'),
          )),
        );

        // Map initialization parameters.
        $maps[$id] = array(
          'display_options' => array(
            'display_type' => $settings['yamaps_display_options_static']['display_type_static'],
          ),
        );

        // Adding CSS for button.
        $element[$delta]['map_container']['#attached']['css'][] = drupal_get_path('module', 'yamaps') . '/misc/yamaps.css';

        // Adding map to js and load library.
        $element[$delta]['map_container']['#attached']['js'][] = array(
          'data' => array('yamapsStatic' => $maps),
          'type' => 'setting',
        );
        $element[$delta]['map_container']['#attached']['library'][] = array('yamaps', 'yamaps.full');
      }
      break;

    case YAMAPS_TEXT_FORMATTER:
      $display_options = variable_get('yamaps_block_edit_display_options', array());
      $active_behaviors = array_values(array_filter($settings['behaviors']));

      // This options must be defined to avoid problems on map rendering.
      $width = !empty($settings['width']) ? $settings['width'] : '100%';
      $height = !empty($settings['height']) ? $settings['height'] : '400px';

      foreach ($items as $delta => $item) {
        $id = drupal_html_id('yamap-text');
        $element[$delta]['container'] = array(
          '#type' => 'html_tag',
          '#tag' => 'div',
          '#attributes' => array(
            'id' => $id,
            'style' => "width: $width; height: $height;",
            'class' => array('yamaps-map-text-container'),
            'data-address' => $item['safe_value'],
          ),
          '#value' => '',
          '#attached' => array(
            'js' => array(
              drupal_get_path('module', 'yamaps') . '/misc/yamaps.text.js',
              array(
                'type' => 'setting',
                'data' => array(
                  'yamapsGeocoderUrl' => YAMAPS_GEOCODER_URL . '?format=json&results=1',
                  'yamaps-row' => array(
                    $id => array(
                      'init' => array(
                        // We will fill this one via JS.
                        'center' => array(),
                        'behaviors' => $active_behaviors,
                        'type' => 'yandex#map',
                      ),
                      'controls' => (bool) $settings['control'],
                      'traffic' => (bool) $settings['traffic'],
                      'display_options' => array(
                        'display_type' => isset($display_options['display_type'])
                          ? $display_options['display_type']
                          : 'map',
                      ),
                      // Placemarks will be filled later after settings checking.
                      // We need an empty array here because it could be not
                      // filled later to avoid one more "if" in JS.
                      'placemarks' => array(),
                      'edit' => FALSE,
                      'lines' => NULL,
                      'polygons' => NULL,
                      'routes' => NULL,
                    ),
                  ),
                  'yamaps' => array(),
                ),
              ),
            ),
          ),
        );

        $element[$delta]['container']['#attached']['library'][] = array('yamaps', 'yamaps.full');
        // Check if we should display placemark.
        if ($settings['placemark']['placemark_display']) {
          if (!empty($settings['placemark']['placemark_text'])) {
            $settings['placemark']['placemark_text'] = str_replace(
              '[address]',
              $item['safe_value'],
              $settings['placemark']['placemark_text']
            );
          }
          // Add placemark information.
          $element[$delta]['container']['#attached']['js'][1]['data']['yamaps-row'][$id]['placemarks'][] = array(
            // We will full this field via JS.
            'coords' => array(),
            'params' => array(
              // Placemark color
              'color' => $settings['placemark']['placemark_color'],
              // Placemark text.
              'iconContent' => $settings['placemark']['placemark_text'],
            ),
          );
        }
      }
      break;
  }

  return $element;
}
