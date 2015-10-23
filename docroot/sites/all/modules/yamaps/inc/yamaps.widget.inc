<?php

/**
 * @file
 * Yandex Maps field widget.
 */

/**
 * Implements hook_field_widget_info().
 */
function yamaps_field_widget_info() {
  return array(
    'yamaps_field' => array(
      'label' => t('Yandex map'),
      'field types' => array('field_yamaps'),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function yamaps_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  if ($field['type'] == 'field_yamaps') {
    // Unique map id.
    $id = drupal_html_id(implode('-', array(
      'ymap',
      $element['#field_name'],
      $element['#delta'],
      'edit',
    )));

    // Unique map button id.
    $open_button_id = drupal_html_id(implode('-', array($id, 'open_button')));
    $close_button_id = drupal_html_id(implode('-', array($id, 'close_button')));
    $value = isset($items[$delta]) ? $items[$delta] : array();

    $element += array(
      '#type' => $instance['widget']['type'],
      '#default_value' => $value,
    );

    // Set value after "Add more" button clicked.
    if (isset($form_state['values'][$field['field_name']][$langcode][$delta])) {
      $value = $form_state['values'][$field['field_name']][$langcode][$delta];
    }

    // Map information.
    yamaps_cleanup_values($value);
    $default_js = yamaps_format_values_to_js($value);

    // Issue #1712292 fix.
    // Display default position on edit form.
    if (!$element['#default_value']) {
      if (isset($element['#entity']) && $entity = $element['#entity']) {
        $default_value = field_get_default_value($element['#entity_type'], $entity, $field, $instance);
        $default_value = reset($default_value);

        if (empty($default_value) || !is_array($default_value)) {
          $default_value = array();
        }
        yamaps_cleanup_values($default_value);
        $default_js = yamaps_format_values_to_js($default_value);
      }
    }

    $container_width = isset($field['settings']['display_options']['width']) ? $field['settings']['display_options']['width'] : YAMAPS_DEFAULT_ADMIN_UI_MAP_WIDTH;
    $container_height = isset($field['settings']['display_options']['height']) ? $field['settings']['display_options']['height'] : YAMAPS_DEFAULT_ADMIN_UI_MAP_HEIGHT;
    $open_map_button = FALSE;
    $close_map_button = FALSE;

    if ($field['settings']['display_options']['display_type'] == 'map_button') {
      $element['open_map_button'] = array(
        '#type' => 'html_tag',
        '#tag' => 'div',
        '#value' => t($field['settings']['display_options']['open_button_text']),
        '#attributes' => array(
          'id' => $open_button_id,
          'class' => array('open_yamap_button'),
          'mapId' => $id,
        ),
      );

      $element['close_map_button'] = array(
        '#type' => 'html_tag',
        '#tag' => 'div',
        '#value' => isset($field['settings']['display_options']['close_button_text']) ? t($field['settings']['display_options']['close_button_text']) : t(YAMAPS_DEFAULT_CLOSE_MAP_TEXT),
        '#attributes' => array(
          'id' => $close_button_id,
          'class' => array('close_yamap_button', 'element-invisible'),
          'mapId' => $id,
        ),
      );
      $open_map_button = TRUE;
      $close_map_button = TRUE;
    }

    // Map container.
    $element['map_wrapper']['map'] = array(
      '#type' => 'html_tag',
      '#tag' => 'div',
      '#value' => '',
      '#attributes' => array(
        'id' => $id,
        'class' => $open_map_button && $close_map_button ? array('yamaps-map-container', 'element-invisible') : array('yamaps-map-container'),
        'style' => 'width: ' . $container_width . '; height: ' . $container_height . ';',
      ),
    );

    $coords_description = array(
      t('Search for object on the map to fill this field automatically or leave it blank (if field is not required).'),
      '<strong>' . t('Leave this field blank to remove field item.') . '</strong>',
      l(
        t('Terms of service «API Yandex.Maps»'),
        YAMAPS_LEGAL_AGREEMENT_URL,
        array('attributes' => array('target' => '_blank'))
      ),
    );

    $element['coords'] = array(
      '#type' => 'textfield',
      '#title' => t('Coordinates'),
      '#default_value' => $value['coords'],
      '#attributes' => array(
        'class' => array('field-yamaps-coords-' . $id),
        'style' => 'width: 100%;',
      ),
      '#description' => implode(' ', $coords_description),
    );

    // Hidden elements to saving map information.
    $element['type'] = array(
      '#type' => 'hidden',
      '#default_value' => $value['type'],
      '#attributes' => array('class' => array('field-yamaps-type-' . $id)),
    );

    $element['placemarks'] = array(
      '#type' => 'hidden',
      '#default_value' => $value['placemarks'],
      '#attributes' => array('class' => array('field-yamaps-placemarks-' . $id)),
    );

    $element['lines'] = array(
      '#type' => 'hidden',
      '#default_value' => $value['lines'],
      '#attributes' => array('class' => array('field-yamaps-lines-' . $id)),
    );

    $element['polygons'] = array(
      '#type' => 'hidden',
      '#default_value' => $value['polygons'],
      '#attributes' => array('class' => array('field-yamaps-polygons-' . $id)),
    );

    $element['routes'] = array(
      '#type' => 'hidden',
      '#default_value' => $value['routes'],
      '#attributes' => array('class' => array('field-yamaps-routes-' . $id)),
    );

    $element['hide'] = array(
      '#type' => 'checkbox',
      '#title' => t('Hide field'),
      '#default_value' => $value['hide'],
      '#description' => t("Hide field containing map from the end user.
    Can be useful when administrator uses field as optional and doesn't intend to display map."),
    );

    // Map initialization parameters.
    $map = array(
      'init' => array(
        'center' => (isset($default_js['coords']['center'])) ? $default_js['coords']['center'] : NULL,
        'zoom' => (isset($default_js['coords']['zoom'])) ? $default_js['coords']['zoom'] : NULL,
        'type' => $default_js['type'],
        'behaviors' => array('scrollZoom', 'dblClickZoom', 'drag'),
      ),
      'display_options' => array(
        'display_type' => $field['settings']['display_options']['display_type'],
      ),
      'controls' => 1,
      'traffic' => 0,
      'placemarks' => $default_js['placemarks'],
      'lines' => $default_js['lines'],
      'polygons' => $default_js['polygons'],
      'routes' => $default_js['routes'],
      'edit' => TRUE,
    );

    // Add information about this map to js.
    // Settings should be attached to each element, because element changes
    // its ID when reloaded via Ajax in case of multi value field.
    $element['#attached']['js'][] = array(
      'data' => array('yamaps' => array($id => $map)),
      'type' => 'setting',
    );
    $form['#attached']['library'][] = array('yamaps', 'yamaps.full');
  }

  return $element;
}
