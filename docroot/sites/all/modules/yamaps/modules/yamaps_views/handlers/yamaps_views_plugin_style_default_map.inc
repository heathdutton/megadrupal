<?php

/**
 * @file
 * Class yamaps_views_plugin_style_default_map.
 */

/**
 * Class defines Yandex Maps style plugin handler.
 */
class yamaps_views_plugin_style_default_map extends views_plugin_style {
  const PLACEMARK_TITLE = 'iconContent';
  const PLACEMARK_BALLON_HEADER = 'balloonContentHeader';
  const PLACEMARK_BALLON_BODY = 'balloonContentBody';
  const PLACEMARK_DEFAULT_FIELD = '<default>';
  const PLACEMARK_NONE_FIELD = '<none>';

  /**
   * Returns default settings for the map.
   *
   * @return array
   *   Default values.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['yandex_map_field'] = array('default' => '');
    $options['placemarks'] = array('default' => '');
    $options['lines'] = array('default' => '');
    $options['polygons'] = array('default' => '');
    $options['routes'] = array('default' => '');
    $options['yamaps_center_options'] = array(
      'default' => array(
        'map_center_type' => 'geolocation',
        'map_center_geolocation' => '',
        'zoom' => 6,
        'map_container' => array('coords' => ''),
      ),
    );
    $options['placemark_title'] = array('default' => self::PLACEMARK_DEFAULT_FIELD);
    $options['baloon_title'] = array('default' => self::PLACEMARK_DEFAULT_FIELD);
    $options['baloon_body'] = array('default' => self::PLACEMARK_DEFAULT_FIELD);
    $options['type'] = array('default' => 'yandex#map');
    $options['map_center'] = array('default' => '');
    $options['map_grouping_cat'] = array('default' => 'standart');

    return $options;
  }

  /**
   * Returns Yandex Maps views settings form.
   *
   * @param array $form
   *   Form structure array.
   * @param array $form_state
   *   Form state array.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $fields = $this->getFields();
    $yandex_fields = $this->getYandexMapsFields();

    $form['yandex_map_field'] = array(
      '#title' => t('Yandex Map Field'),
      '#description' => t('Choose Yandex Maps field. Add if views fields this field for the first.'),
      '#type' => 'select',
      '#options' => $yandex_fields,
      '#required' => TRUE,
      '#default_value' => $this->options['yandex_map_field'],
    );
    $form['yamaps_center_options'] = array(
      '#type' => 'fieldset',
      '#title' => t('Map center'),
      '#states' => array(
        'invisible' => array(
          ':input[name="style_options[yandex_map_field]"]' => array('value' => ''),
        ),
      ),
    );
    $form['yamaps_center_options']['map_center_type'] = array(
      '#type' => 'radios',
      '#title' => t('Choose map center type'),
      '#options' => array(
        'geolocation' => t('Geolocation.'),
        'mini_map' => t('Choose on map.'),
      ),
      '#default_value' => $this->options['yamaps_center_options']['map_center_type'],
      '#required' => FALSE,
      '#description' => t('Type of map displaying.'),
    );
    $form['yamaps_center_options']['map_center_geolocation'] = array(
      '#title' => t('Map center geolocation'),
      '#description' => t('Please enter place on whitch map will be centered.'),
      '#type' => 'textfield',
      '#default_value' => $this->options['yamaps_center_options']['map_center_geolocation'],
      '#size' => 40,
      '#states' => array(
        'visible' => array(
          ':input[name="style_options[yamaps_center_options][map_center_type]"]' => array('value' => 'geolocation'),
        ),
      ),
    );
    $form['yamaps_center_options']['zoom'] = array(
      '#title' => t('Zoom'),
      '#type' => 'select',
      '#description' => t('Zoom of map'),
      '#options' => range(1, 15),
      '#states' => array(
        'visible' => array(
          ':input[name="style_options[yamaps_center_options][map_center_type]"]' => array('value' => 'geolocation'),
        ),
      ),
      '#default_value' => $this->options['yamaps_center_options']['zoom'],
    );

    $this->options['coords'] = $this->getCoordinates();
    $prepared_params = yamaps_format_values_to_js($this->options);

    // Map initialization parameters.
    $map = array(
      'init' => array(
        'type' => $this->options['type'] ? $this->options['type'] : 'yandex#map',
        'behaviors' => array('scrollZoom', 'dblClickZoom', 'drag'),
      ),
      'display_options' => array(
        'display_type' => 'map',
      ),
      'controls' => 1,
      'traffic' => 0,
      'placemarks' => $prepared_params['placemarks'],
      'lines' => $prepared_params['lines'],
      'polygons' => $prepared_params['polygons'],
      'routes' => $prepared_params['routes'],
      'edit' => TRUE,
    );

    $coords = json_decode($this->options['coords']);

    if (isset($coords->center, $coords->zoom)) {
      $map['init']['center'] = $coords->center;
      $map['init']['zoom'] = $coords->zoom;
    }

    $id = drupal_html_id(implode('-', array(
      $this->plugin_name,
      $this->view->name,
      $this->view->current_display,
      'style_options_form',
    )));

    $yandexmap_field_settings = $this->view->display_handler->handlers['field'][$this->options['yandex_map_field']]->options['settings'];

    // Set width and height.
    if (isset($yandexmap_field_settings['width']) && isset($yandexmap_field_settings['height'])) {
      $width = $yandexmap_field_settings['width'];
      $height = $yandexmap_field_settings['height'];
    }
    elseif (isset($yandexmap_field_settings['s_width']) && isset($yandexmap_field_settings['s_height'])) {
      $width = $yandexmap_field_settings['s_width'];
      $height = $yandexmap_field_settings['s_height'];
    }
    else {
      $width = YAMAPS_DEFAULT_ADMIN_UI_MAP_WIDTH;
      $height = YAMAPS_DEFAULT_ADMIN_UI_MAP_HEIGHT;
    }

    $form['yamaps_center_options']['map_container'] = array(
      '#type' => 'fieldset',
      '#title' => t('Map center'),
      '#states' => array(
        'visible' => array(
          ':input[name="style_options[yamaps_center_options][map_center_type]"]' => array('value' => 'mini_map'),
        ),
      ),
    );
    // Map container.
    $form['yamaps_center_options']['map_container']['map'] = array(
      '#type' => 'html_tag',
      '#tag' => 'div',
      '#value' => '',
      '#description' => t('Map view will be used when "Choose on map." radio is active'),
      '#attributes' => array(
        'style' => ' width: ' . $width . '; height:' . $height . ';',
        'id' => $id,
        'class' => array(
          'yamaps-map-container',
        ),
      ),

    );
    $form['yamaps_center_options']['map_container']['coords'] = array(
      '#type' => 'textfield',
      '#title' => t('Coordinates'),
      '#default_value' => $this->getCoordinates(),
      '#attributes' => array(
        'class' => array('field-yamaps-coords-' . $id),
        'style' => 'width: 100%;',
      ),
      '#states' => array(
        'invisible' => array(
          ':input[name="style_options[yandex_map_field]"]' => array('value' => ''),
        ),
        'visible' => array(
          ':input[name="style_options[yamaps_center_options][map_center_type]"]' => array('value' => 'mini_map'),
        ),
      ),
      '#description' => t('Search for object on the map to fill this field or leave it blank (if field is not required).'),
    );
    // Hidden elements to save map information.
    $form['type'] = array(
      '#type' => 'hidden',
      '#title' => t('Type'),
      '#default_value' => $this->options['type'],
      '#attributes' => array('class' => array('field-yamaps-type-' . $id)),
    );
    // Hidden elements to saving map information.
    $form['placemarks'] = array(
      '#type' => 'hidden',
      '#default_value' => $this->options['placemarks'],
      '#attributes' => array('class' => array('field-yamaps-placemarks-' . $id)),
    );
    $form['lines'] = array(
      '#type' => 'hidden',
      '#default_value' => $this->options['lines'],
      '#attributes' => array('class' => array('field-yamaps-lines-' . $id)),
    );
    $form['polygons'] = array(
      '#type' => 'hidden',
      '#default_value' => $this->options['polygons'],
      '#attributes' => array('class' => array('field-yamaps-polygons-' . $id)),
    );
    $form['routes'] = array(
      '#type' => 'hidden',
      '#default_value' => $this->options['routes'],
      '#attributes' => array('class' => array('field-yamaps-routes-' . $id)),
    );
    $form['#attached']['js'][] = array(
      'data' => array('yamaps' => array($id => $map)),
      'type' => 'setting',
    );
    // Load library.
    $form['#attached']['library'][] = array('yamaps', 'yamaps.full');
    $form['placemark_title'] = array(
      '#title' => t('Placemark title'),
      '#type' => 'select',
      '#options' => $fields,
      '#default_value' => $this->options['placemark_title'],
      '#states' => array(
        'invisible' => array(
          ':input[name="style_options[yandex_map_field]"]' => array('value' => ''),
        ),
      ),
    );
    $form['baloon_title'] = array(
      '#title' => t('Baloon title'),
      '#type' => 'select',
      '#options' => $fields,
      '#default_value' => $this->options['baloon_title'],
      '#states' => array(
        'invisible' => array(
          ':input[name="style_options[yandex_map_field]"]' => array('value' => ''),
        ),
      ),
    );
    $form['baloon_body'] = array(
      '#title' => t('Balloon body Field'),
      '#type' => 'select',
      '#multiple' => TRUE,
      '#options' => $fields,
      '#default_value' => $this->options['baloon_body'],
      '#states' => array(
        'invisible' => array(
          ':input[name="style_options[yandex_map_field]"]' => array('value' => ''),
        ),
      ),
    );
  }

  /**
   * Prepares map body.
   *
   * @param array $body_array
   *   Body array.
   *
   * @return string
   *   Prepared body.
   */
  public function prepareBody(array $body_array) {
    $output = '<div class="balloon-inner">';
    foreach ($body_array as $key => $val) {
      $output .= '<span class="' . $key . '">' . $val . '</span>';
    }
    $output .= '</div>';
    return $output;
  }

  /**
   * Checks if the style uses fields.
   *
   * @return bool
   *   Boolean value.
   */
  public function uses_fields() {
    $fields = parent::uses_fields();
    $yandexmap_field = '';
    if (isset($this->view->field)) {
      foreach ($this->view->field as $field_name => $field_handler) {
        if (isset($field_handler->field_info) && $field_handler->field_info['type'] == 'field_yamaps') {
          $yandexmap_field = $field_name;
          break;
        }
      }
    }
    // Excluded field.
    if (isset($this->view->field[$yandexmap_field])) {
      $this->view->field[$yandexmap_field]->options['exclude'] = TRUE;
    }
    return $fields;
  }

  /**
   * Validates views style form.
   *
   * @return array
   *   Errors list.
   */
  public function validate() {
    $errors = parent::validate();

    // Checking of existence of pager settings.
    if ($this->display->handler->use_pager()) {
      $showed = &drupal_static(__FUNCTION__, FALSE);
      if (!$showed) {
        $errors[] = t('The Yandex Maps style cannot be used with a pager. Disable the "Use pager" option for this display.');
        $showed = TRUE;
      }
    }

    return $errors;
  }

  /**
   * Renders the map.
   *
   * @return array
   *   Rendered string.
   */
  public function render() {
    $placemarks = array();
    $prepared_lines = array();
    $prepared_polygons = array();
    $prepared_route = array();
    $this->options['coords'] = $this->getCoordinates();
    $prepared_params = yamaps_format_values_to_js($this->options);

    if (empty($this->options['yandex_map_field'])) {
      return t('Please add "Yandex Maps" field and chose it in views settings form.');
    }

    $yandexmap_field_type = $this->view->field[$this->options['yandex_map_field']]->options['type'];

    if ($yandexmap_field_type == YAMAPS_STATIC_FORMATTER &&
      (!isset($prepared_params['coords']['center']) || (isset($prepared_params['coords']['center']) && !is_array($prepared_params['coords']['center'])))) {
      return t('The Static Yandex Maps style cannot be used without coordinates of center.');
    }

    $this->render_fields($this->view->result);

    $yandexmap_field_name = 'field_' . $this->options['yandex_map_field'];
    $yandexmap_field_settings = $this->view->field[$this->options['yandex_map_field']]->options['settings'];

    foreach ($this->view->result as $row_index => $row) {
      // Fix yandex cart.
      foreach ($row->$yandexmap_field_name as $yandexmap_field_coords) {
        if (isset($yandexmap_field_coords['raw']['placemarks'])) {
          // Preparing placemarks.
          $decoded_placemarks = drupal_json_decode($yandexmap_field_coords['raw']['placemarks']);

          if (is_array($decoded_placemarks)) {
            foreach ($decoded_placemarks as $placemark) {
              // Override placemark title.
              if (isset($this->options['placemark_title'], $this->view->field[$this->options['placemark_title']]) && $this->options['placemark_title'] !== self::PLACEMARK_DEFAULT_FIELD) {
                // Prepare placemark title.
                $marker_title = $this->preparePlacemarkTitle($row_index);
                $field_title_settings = $this->view->field[$this->options['placemark_title']];

                switch ($field_title_settings->field_info['type']) {
                  case 'image':
                    // Special logic for image fields.
                    // Placemark type.
                    $placemark['options']['iconLayout'] = 'default#image';
                    // Image href.
                    $placemark['options']['iconImageHref'] = image_style_url($row->{'field_' . $this->options['placemark_title']}[0]['rendered']['#image_style'], $row->{'field_' . $this->options['placemark_title']}[0]['raw']['uri']);
                    $image_dimensions = getimagesize($placemark['options']['iconImageHref']);
                    // Placemark image size.
                    $placemark['options']['iconImageSize'] = array(
                      $image_dimensions[0],
                      $image_dimensions[1],
                    );
                    // Icon image offset of upper left angle.
                    $placemark['options']['iconImageOffset'] = array(
                      -($image_dimensions[0] / 2),
                      $image_dimensions[1] * 0.1 - $image_dimensions[1],
                    );
                    break;

                  default:
                    $placemark['params'][self::PLACEMARK_TITLE] = $marker_title;
                    break;
                }

              }
              // Prepare Baloon title.
              if ($this->options['baloon_title'] && $this->options['baloon_title'] !== self::PLACEMARK_DEFAULT_FIELD) {
                $placemark['params'][self::PLACEMARK_BALLON_HEADER] = $this->rendered_fields[$row_index][$this->options['baloon_title']];
              }
              // Prepare Baloon body.
              if (isset($this->options['baloon_body']) && is_array($this->options['baloon_body'])) {
                $ballon_body = array();
                foreach ($this->options['baloon_body'] as $bval) {
                  $ballon_body[] = $this->get_field($row_index, $bval);
                }
                $placemark['params'][self::PLACEMARK_BALLON_BODY] = $this->prepareBody($ballon_body);
              }
              $this->view->row_index = $row_index;
              unset($ballon_body);
              $placemarks[] = $placemark;
            }
          }
        }
        // Preparing lines.
        if (isset($yandexmap_field_coords['raw']['lines'])) {
          $decoded_lines = drupal_json_decode($yandexmap_field_coords['raw']['lines']);
          if (is_array($decoded_lines)) {
            foreach ($decoded_lines as $lines) {
              $prepared_lines[] = $lines;
            }
          }
        }
        // Preparing polygons.
        if (isset($yandexmap_field_coords['raw']['polygons'])) {
          $decoded_polygons = drupal_json_decode($yandexmap_field_coords['raw']['polygons']);
          if (is_array($decoded_polygons)) {
            foreach ($decoded_polygons as $polygons) {
              $prepared_polygons[] = $polygons;
            }
          }
        }
        // Preparing routes.
        if (isset($yandexmap_field_coords['raw']['routes'])) {
          $decoded_routes = drupal_json_decode($yandexmap_field_coords['raw']['routes']);
          if (is_array($decoded_routes)) {
            foreach ($decoded_routes as $route) {
              $prepared_route[] = $route;
            }
          }
        }
      }
    }

    unset($this->view->row_index);

    $array_of_unique_params = array(
      $this->plugin_name,
      $this->view->name,
      $this->view->current_display,
    );

    if (isset($this->view->dom_id)) {
      $array_of_unique_params[] = $this->view->dom_id;
    }
    // Unique map id.
    $id = drupal_html_id(implode('-', $array_of_unique_params));
    $open_button_id = drupal_html_id(implode('-', array($id, 'open_button')));
    $close_button_id = drupal_html_id(implode('-', array($id, 'close_button')));

    switch ($this->options['yamaps_center_options']['map_center_type']) {
      case 'geolocation':
        $prepared_params['coords']['center'] = NULL;
        $parameters = yamaps_geocoding($this->options['yamaps_center_options']['map_center_geolocation']);

        if (isset($parameters) && $parameters !== FALSE) {
          $prepared_params['coords']['center'] = $parameters['map_center'];
        }
        $prepared_params['coords']['zoom'] = ++$this->options['yamaps_center_options']['zoom'];
        $prepared_params['type'] = 'yandex#map';
        break;

      case 'mini_map':
        // Merging placemark.
        if (is_array($prepared_params['placemarks'])) {
          foreach ($prepared_params['placemarks'] as $p) {
            $placemarks[] = $p;
          }
        }
        // Merging lines.
        if (is_array($prepared_params['lines'])) {
          foreach ($prepared_params['lines'] as $lines) {
            $prepared_lines[] = $lines;
          }
        }
        // Merging polygons.
        if (is_array($prepared_params['polygons'])) {
          foreach ($prepared_params['polygons'] as $polygon) {
            $prepared_polygons[] = $polygon;
          }
        }
        // Merging routes.
        if (is_array($prepared_params['routes'])) {
          foreach ($prepared_params['routes'] as $route) {
            $prepared_route[] = $route;
          }
        }
        break;
    }

    $views_output = array();

    switch ($yandexmap_field_type) {
      case YAMAPS_DYNAMIC_FORMATTER:
        // Map initialization parameters.
        $map = array(
          'id' => $id,
          'display_options' => array(
            'display_type' => $yandexmap_field_settings['yamaps_display_options']['display_type'],
          ),
          'controls' => $yandexmap_field_settings['controls'],
          'traffic' => $yandexmap_field_settings['traffic'],
          'placemarks' => empty($placemarks) ? NULL : $placemarks,
          'lines' => empty($prepared_lines) ? NULL : $prepared_lines,
          'polygons' => empty($prepared_polygons) ? NULL : $prepared_polygons,
          'routes' => empty($prepared_route) ? NULL : $prepared_route,
          'edit' => FALSE,
          'init' => array(
            'type' => $prepared_params['type'],
            'center' => empty($prepared_params['coords']['center']) ? NULL : $prepared_params['coords']['center'],
            'zoom' => empty($prepared_params['coords']['zoom']) ? NULL : $prepared_params['coords']['zoom'],
            'behaviors' => array_values(array_filter($yandexmap_field_settings['behaviors'])),
          ),
        );
        // Adding map to js.
        $views_output['#attached']['js'][] = array(
          'data' => array('yamaps' => array($id => $map)),
          'type' => 'setting',
        );
        // Load library.
        $views_output['#attached']['library'][] = array(
          'yamaps',
          'yamaps.full',
        );

        $hide_map = (isset($yandexmap_field_settings['yamaps_display_options']['display_type']) &&
          $yandexmap_field_settings['yamaps_display_options']['display_type'] == 'map_button');
        $map_class = array('yamaps-map-container');
        $map_class[] = $hide_map ? 'element-invisible' : '';

        if (isset($yandexmap_field_settings['yamaps_display_options']['display_type']) &&
          $yandexmap_field_settings['yamaps_display_options']['display_type'] == 'map_button') {
          $views_output['open_map_button'] = array(
            '#type' => 'html_tag',
            '#tag' => 'div',
            '#value' => isset($yandexmap_field_settings['yamaps_display_options']['open_button_text']) ?
              t($yandexmap_field_settings['yamaps_display_options']['open_button_text']) :
              t(YAMAPS_DEFAULT_OPEN_MAP_TEXT),
            '#attributes' => array(
              'id' => $open_button_id,
              'class' => array('open_yamap_button'),
              'mapId' => $id,
            ),
          );
          $views_output['close_map_button'] = array(
            '#type' => 'html_tag',
            '#tag' => 'div',
            '#value' => isset($yandexmap_field_settings['yamaps_display_options']['close_button_text']) ?
              t($yandexmap_field_settings['yamaps_display_options']['close_button_text']) :
              t(YAMAPS_DEFAULT_CLOSE_MAP_TEXT),
            '#attributes' => array(
              'id' => $close_button_id,
              'class' => array('close_yamap_button', 'element-invisible'),
              'mapId' => $id,
            ),
          );
        }

        $views_output[] = array(
          '#type' => 'html_tag',
          '#tag' => 'div',
          '#attributes' => array(
            'style' => ' width: ' . $yandexmap_field_settings['width'] . '; height:' . $yandexmap_field_settings['height'] . ';',
            'id' => $id,
            'class' => $map_class,
          ),
          '#value' => '',
        );
        break;

      case YAMAPS_STATIC_FORMATTER:
        $params = array(
          'll' => end($prepared_params['coords']['center']) . ',' . reset($prepared_params['coords']['center']),
          'z' => $prepared_params['coords']['zoom'],
          'size' => $yandexmap_field_settings['s_width'] . ',' . $yandexmap_field_settings['s_height'],
        );

        // 2.x to 1.x types.
        $map_types = array(
          'yandex#map' => 'map',
          'yandex#satellite' => 'sat',
          'yandex#hybrid' => 'sat,skl',
          'yandex#publicMap' => 'pmap',
          'yandex#publicMapHybrid' => 'sat,pskl',
        );

        $params['l'] = $map_types[$prepared_params['type']];

        if ($yandexmap_field_settings['s_traffic']) {
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
        if (!empty($placemarks)) {
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

        if ($prepared_lines) {
          foreach ($prepared_lines as $line) {
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

        if ($prepared_polygons) {
          foreach ($prepared_polygons as $polygon) {
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

        $open_button_id = drupal_html_id(implode('-', array(
          $id,
          'open_button',
        )));

        $hide_map = (isset($yandexmap_field_settings['yamaps_display_options_static']['display_type_static']) &&
          $yandexmap_field_settings['yamaps_display_options_static']['display_type_static'] == 'map_button');
        $map_display_style = $hide_map ? '; display: none' : '';

        // Returns map container div with image.
        $views_output['map_container'] = array(
          '#type' => 'html_tag',
          '#tag' => 'div',
          '#attributes' => array(
            'id' => $id,
            'style' => 'width:' . $yandexmap_field_settings['s_width'] . 'px; height:' .
              $yandexmap_field_settings['s_height'] . 'px' . $map_display_style . ';',
          ),
          '#value' => theme('image', array(
            'path' => url(YAMAPS_STATIC_API_URL, array(
              'query' => $params,
              'external' => TRUE,
            )),
            'width' => $yandexmap_field_settings['s_width'],
            'height' => $yandexmap_field_settings['s_height'],
            'title' => t('Yandex Map'),
          )),
        );

        if ($hide_map) {
          $views_output['open_button_text'] = array(
            '#type' => 'html_tag',
            '#tag' => 'div',
            '#value' => isset($yandexmap_field_settings['yamaps_display_options_static']['open_button_text_static']) ?
              t($yandexmap_field_settings['yamaps_display_options_static']['open_button_text_static']) :
              t(YAMAPS_DEFAULT_OPEN_MAP_TEXT),
            '#attributes' => array(
              'id' => $open_button_id,
              'class' => array('open_yamap_button_static'),
              'mapId' => $id,
            ),
          );
        }

        // Map initialization parameters.
        $maps[$id] = array(
          'display_options' => array(
            'display_type' => $yandexmap_field_settings['yamaps_display_options_static']['display_type_static'],
          ),
        );

        // Adding CSS for button.
        $views_output['#attached']['css'][] = drupal_get_path('module', 'yamaps') . '/misc/yamaps.css';

        // Adding map to js and load library.
        $views_output['#attached']['js'][] = array(
          'data' => array('yamapsStatic' => $maps),
          'type' => 'setting',
        );
        $views_output['#attached']['library'][] = array(
          'yamaps',
          'yamaps.full',
        );
        break;
    }

    return $views_output;
  }

  /**
   * Returns field names.
   *
   * @return array
   *   Fields list.
   */
  public function getFields() {
    $field_names = array(
      '' => t(self::PLACEMARK_NONE_FIELD),
      self::PLACEMARK_DEFAULT_FIELD => t('Default baloon value'),
    );
    $fields = $this->display->handler->get_handlers('field');
    foreach ($fields as $id => $handler) {
      if (isset($handler->human_name)) {
        $field_names[$id] = $handler->human_name;
      }
      else {
        $field_names[$id] = $handler->definition['title'];
      }
    }
    return $field_names;
  }

  /**
   * Prepares placemark for the map.
   *
   * @param int $row_index
   *   Row index value.
   *
   * @return string
   *   Prepared string.
   */
  public function preparePlacemarkTitle($row_index) {
    $title = $this->get_field($row_index, $this->options['placemark_title']);

    return check_plain(strip_tags($title));
  }

  /**
   * Returns yandex maps specific field names.
   *
   * @return array
   *   List of yandex maps fields.
   */
  public function getYandexMapsFields() {
    $field_names = array(
      '' => t(self::PLACEMARK_NONE_FIELD),
    );
    $fields = $this->display->handler->get_handlers('field');

    foreach ($fields as $id => $handler) {
      if (isset($handler->field_info) && $handler->field_info['module'] == 'yamaps') {
        $field_names[$id] = $handler->definition['title'];
      }
    }

    return $field_names;
  }

  /**
   * Returns map coordinates.
   *
   * Get coordinates with support of old coordinates place.
   *
   * @return string
   *   List options 'coords'.
   */
  public function getCoordinates() {
    if (isset($this->options['coords'])) {
      return $this->options['coords'];
    }
    elseif (isset($this->options['yamaps_center_options']['map_container']['coords'])) {
      return $this->options['yamaps_center_options']['map_container']['coords'];
    }
    else {
      return '';
    }
  }

}
