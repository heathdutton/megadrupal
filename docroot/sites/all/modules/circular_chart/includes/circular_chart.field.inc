<?php
/**
 * @file
 * Provides all the hooks for exposing the new circular chart field.
 */

/**
 * Implements hook_field_info().
 */
function circular_chart_field_info() {
  return array(
    'field_circular_chart' => array(
      'label' => t('Circular Chart Widget'),
      'description' => t('Circular chart widget'),
      'settings' => array('allowed_values' => array(), 'allowed_values_function' => ''),
      'instance_settings' => array('text_processing' => 0),
      'default_widget' => 'options_select',
      'default_formatter' => 'circular_chart_raw_text',
    ),
  );
}

/**
 * Implements hook_field_widget_info().
 */
function circular_chart_field_widget_info() {
  $widgets = array();
  if (module_exists('elements')) {
    $widgets += array(
      'circular_chart_html5_number' => array(
        'label' => t('HTML5 Number field'),
        'field types' => array('field_circular_chart'),
      ),
    );
  }
  return $widgets;
}

/**
 * Implements hook_field_widget_info_alter().
 */
function circular_chart_field_widget_info_alter(&$info) {
  $widgets = array(
    'options_select' => array('field_circular_chart'),
  );

  foreach ($widgets as $widget => $field_types) {
    $info[$widget]['field types'] = array_merge($info[$widget]['field types'], $field_types);
  }
}

/**
 * Implements hook_options_list().
 */
function circular_chart_options_list($field, $instance, $entity_type, $entity) {
  return list_allowed_values($field, $instance, $entity_type, $entity);
}

/**
 * Implements hook_field_settings_form().
 */
function circular_chart_field_settings_form($field, $instance, $has_data) {
  $settings = $field['settings'];
  switch ($instance['widget']['type']) {
    case 'options_select':
      $form['allowed_values'] = array(
        '#type' => 'textarea',
        '#title' => t('Allowed values list'),
        '#default_value' => list_allowed_values_string($settings['allowed_values']),
        '#rows' => 10,
        '#element_validate' => array('list_allowed_values_setting_validate'),
        '#field_has_data' => $has_data,
        '#field' => $field,
        '#field_type' => $field['type'],
        '#access' => empty($settings['allowed_values_function']),
      );

      $description = '<p>' . t('The possible values this field can contain. Enter one value per line, in the format key|label.');
      $description .= '<br/>' . t('The key is the stored value, and must be numeric. The label will be used in displayed values and edit forms.');
      $description .= '<br/>' . t('The label is optional: if a line contains a single number, it will be used as key and label.');
      $description .= '<br/>' . t('Lists of labels are also accepted (one label per line), only if the field does not hold any values yet. Numeric keys will be automatically generated from the positions in the list.');
      $description .= '</p>';
      $form['allowed_values']['#description'] = $description;
      break;
  }

  $form['allowed_values_function'] = array(
    '#type' => 'value',
    '#value' => $settings['allowed_values_function'],
  );
  $form['allowed_values_function_display'] = array(
    '#type' => 'item',
    '#title' => t('Allowed values list'),
    '#markup' => t('The value of this field is being determined by the %function function and may not be changed.', array('%function' => $settings['allowed_values_function'])),
    '#access' => !empty($settings['allowed_values_function']),
  );

  return $form;
}

/**
 * Implements hook_field_widget_form().
 */
function circular_chart_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $value = isset($items[$delta]['circle_value']) ? $items[$delta]['circle_value'] : 0;

  $widget = $element;
  $widget['#delta'] = $delta;

  switch ($instance['widget']['type']) {

    case 'circular_chart_html5_number':
      if (module_exists('elements')) {
        $widget += array(
          '#type' => 'numberfield',
          '#title' => check_plain($element['#title']),
          '#default_value' => $value,
          '#min' => 0,
        );
      }
      else {
        $widget += array(
          '#type' => 'textfield',
          '#title' => check_plain($element['#title']),
          '#default_value' => $value,
        );
      }
      break;
  }

  $element['circle_value'] = $widget;
  return $element;
}

/**
 * Implements hook_field_widget_settings_form().
 */
function circular_chart_field_widget_settings_form($field, $instance) {
  switch ($instance['widget']['type']) {
    case 'circular_chart_html5_number':
      if (module_exists('elements')) {
        $circular_chart_number_max_value = array(
          '#type' => 'numberfield',
          '#title' => t('Max value'),
          '#description' => t('The maximum value of the field. This value will be used for later percentage calculation.'),
          '#default_value' => isset($instance['widget']['settings']['circular_chart_number_max_value']) ? $instance['widget']['settings']['circular_chart_number_max_value'] : '',
          '#required' => TRUE,
          '#min' => 0,
          '#element_validate' => array('element_validate_integer_positive'),
        );
      }
      else {
        $circular_chart_number_max_value = array(
          '#type' => 'textfield',
          '#title' => t('Max value'),
          '#description' => t('The maximum value of the field. This value will be used for later percentage calculation.'),
          '#default_value' => isset($instance['widget']['settings']['circular_chart_number_max_value']) ? $instance['widget']['settings']['circular_chart_number_max_value'] : '',
          '#required' => TRUE,
          '#element_validate' => array('element_validate_integer_positive'),
        );
      }
      $element = array(
        'circular_chart_number_max_value' => $circular_chart_number_max_value,
      );
      break;
  }
  return $element;
}

/**
 * Implements hook_field_is_empty().
 */
function circular_chart_field_is_empty($item, $field) {
  if ($item['circle_value'] == '') {
    return TRUE;
  }
  return FALSE;
}

/**
 * Implements hook_field_validate().
 */
function circular_chart_field_validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors) {
  switch ($instance['widget']['type']) {
    case 'circular_chart_html5_number':
      foreach ($items as $delta => $item) {
        if (!empty($item)) {
          // Check for the empty values and illeagal valus in the field.
          if ($item['circle_value'] !== '' && (!is_numeric($item['circle_value']) || intval($item['circle_value']) != $item['circle_value']) && $item['circle_value'] < 0) {
            $errors[$field['field_name']][$langcode][$delta][] = array(
              'error' => 'circular_chart_wrong_value',
              'message' => t('%name: the field should only contain positive integers.', array(
                '%name' => $instance['label'],
              )),
            );
          }
          // The given value can't be greater than the max value specified.
          if (isset($instance['widget']['settings']['circular_chart_number_max_value'])) {
            if ($item['circle_value'] !== '' && $item['circle_value'] > $instance['widget']['settings']['circular_chart_number_max_value']) {
              $errors[$field['field_name']][$langcode][$delta][] = array(
                'error' => 'circular_chart_exceeding_value',
                'message' => t('%name field should not contain larger value than the given max %max_value value.', array(
                  '%name' => $instance['label'],
                  '%max_value' => $instance['widget']['settings']['circular_chart_number_max_value'],
                )),
              );
            }
          }
        }
      }
      break;
  }
}

/**
 * Implements hook_field_formatter_info().
 */
function circular_chart_field_formatter_info() {
  return array(
    'circular_chart_raw_text' => array(
      'label' => t('Raw circular graph values'),
      'field types' => array('field_circular_chart'),
    ),
    'circular_chart_graph' => array(
      'label' => t('Circular graphs'),
      'field types' => array('field_circular_chart'),
      'settings' => array(
        'selected_chart_preset' => 'default_circular_chart',
      ),
    ),
  );
}

/**
 * Implements hook_field_formatter_view().
 */
function circular_chart_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $settings = $display['settings'];
  $element = array();
  switch ($display['type']) {
    case 'circular_chart_raw_text':
      foreach ($items as $key => $value) {
        $element[$key] = array(
          '#type' => 'markup',
          '#markup' => t('@val', array('@val' => $value['circle_value'])),
        );
      }
      break;

    case 'circular_chart_graph':
      // We will only need the stored settings if the graph display is selected.
      $selected_preset = $settings['selected_chart_preset'];
      // Load the selected preset and send it to the theme layer to generate the
      // circular graph.
      $preset_settings = circular_chart_load_chart(array($selected_preset));
      // First load the main library and then load the graph generation script.
      if (($library = libraries_load('circles')) && !empty($library['loaded'])) {
        drupal_add_js(drupal_get_path('module', 'circular_chart') . '/js/circular_chart_generate.js', array(
          'type' => 'file',
          'scope' => 'footer',
        ));
        // Generate the maximum value of the field.
        $max_value = NULL;
        switch ($instance['widget']['type']) {
          case 'circular_chart_html5_number':
            // In this field we do have a max field entry.
            $max_value = $instance['widget']['settings']['circular_chart_number_max_value'];
            break;

          case 'options_select':
            $allowed_values = list_allowed_values($field);
            $max_value = max(array_keys($allowed_values));
            break;
        }
        foreach ($items as $key => $value) {
          $chart = array(
            '#attributes' => array(
              'class' => 'circle-graph',
              'id' => drupal_html_id($field['field_name'] . '_' . $value['circle_value'] . '_' . $key),
              'circle_radius' => $preset_settings[$selected_preset]->data['circle_radius'],
              'circle_width' => $preset_settings[$selected_preset]->data['circle_width'],
              'circle_bg_color' => $preset_settings[$selected_preset]->data['circle_bg_color'],
              'circle_fg_color' => $preset_settings[$selected_preset]->data['circle_fg_color'],
              'circle_anim_duration' => $preset_settings[$selected_preset]->data['circle_anim_duration'],
              'value' => $value['circle_value'],
              'max_value' => $max_value,
              'wrapper-class' => drupal_html_class($preset_settings[$selected_preset]->data['circle_wrapper_class']),
              'inner-text' => NULL,
            ),
          );

          $element[$key] = array(
            '#type' => 'markup',
            '#markup' => theme('circle_chart', array('chart' => $chart)),
          );
        }
      }
      else {
        watchdog('circular_chart', '!error', array('!error' => $library['error message']), WATCHDOG_NOTICE, NULL);
      }
      break;
  }

  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function circular_chart_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $output = '';
  switch ($display['type']) {
    case 'circular_chart_graph':
      $chart_presets = array();
      $chart_options = array();
      $chart_presets = circular_chart_load_all_charts();
      if (!empty($chart_presets)) {
        foreach ($chart_presets as $chart_machine => $chart_details) {
          $chart_options[$chart_machine] = $chart_details->label;
        }
      }
      $output = t('Selected circular chart preset: %preset', array(
        '%preset' => $chart_options[$settings['selected_chart_preset']],
      ));
      break;
  }
  return $output;
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function circular_chart_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $element = array();
  // Only show the selection form if the display is selected as circular graph.
  if ($display['type'] == 'circular_chart_graph') {
    $chart_presets = array();
    $chart_options = array();
    $chart_presets = circular_chart_load_all_charts();
    if (!empty($chart_presets)) {
      foreach ($chart_presets as $chart_machine => $chart_details) {
        $chart_options[$chart_machine] = $chart_details->label;
      }
    }

    $element['selected_chart_preset'] = array(
      '#type' => 'select',
      '#title' => t('Circular Chart Presets'),
      '#description' => t("The preset's defined. If nothing is defined then define it from !link", array(
        '!link' => l(t('here'), 'admin/config/media/circular-charts/add'),
      )),
      '#options' => $chart_options,
      '#default_value' => $settings['selected_chart_preset'],
    );
  }

  return $element;
}
