<?php
/**
 * @file
 * Field hooks to implement our fields.
 */

/**
 * Implements hook_field_info().
 */
function studyroom_hours_field_info() {
  return array(
    'studyroom_hours' => array(
      'label' => t('Room Hours'),
      'description' => t('The room hours for a studyroom.'),
      'default_widget' => 'studyroom_hours',
      'default_formatter' => 'studyroom_hours',
    ),
  );
}

/**
 * Implements hook_field_is_empty().
 *
 * Always return false because we want to save all weekdays.
 */
function studyroom_hours_field_is_empty($item, $field) {
  return FALSE;
}

/**
 * Implements hook_field_widget_info().
 */
function studyroom_hours_field_widget_info() {
  return array(
    'studyroom_hours' => array(
      'label' => t('Pop-up calendar'),
      'field types' => array('studyroom_hours'),
      'settings' => array(
        'input_format' => 'h:i a',
      ),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_CUSTOM,
      ),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function studyroom_hours_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $is_default = FALSE;
  if (!empty($instance['widget']['is_new']) && $instance['widget']['is_new']) {
    $items = $instance['default_value'];
    $is_default = TRUE;
  }

  $element += array(
    '#title' => check_plain($instance['label']),
    '#type' => 'studyroom_hours',
    '#theme_wrappers' => array('studyroom_hours'),
    '#default_value' => isset($items) ? $items : '',
    '#date_is_default' => $is_default,
    '#element_type' => $instance['widget']['type'],
  );

  return $element;
}

/**
 * Implements hook_field_instance_settings_form().
 */
function studyroom_hours_field_instance_settings_form($field, $instance) {
  $settings = $instance['settings'];
  $form['set_hour_limits'] = array(
    '#type' => 'checkbox',
    '#title' => t('Set hour limits'),
    '#default_value' => isset($settings['set_hour_limits']) ? $settings['set_hour_limits'] : 0,
    '#description' => t('Checking this box to set limits on which hours are able to be scheduled. Leaving this box unchecked will default to global settings values.'),
    '#fieldset' => 'default_values',
  );

  // Build hours array.
  $hours = studyroom_date_hours();
  $form['limit_start'] = array(
    '#type' => 'select',
    '#title' => t('Limit day start time'),
    '#default_value' => isset($settings['limit_start']) && !empty($settings['limit_start']) ? $settings['limit_start'] : '',
    '#options' => array('' => '- None') + drupal_map_assoc($hours, 'studyroom_format_hours_ampm'),
    '#description' => t("Set the global day start time.  No time's in the module will be allowed to start before this time."),
    '#states' => array(
      'visible' => array(
        ':input[name*="set_hour_limits"]' => array('checked' => TRUE),
      ),
    ),
    '#fieldset' => 'default_values',
  );
  $form['limit_end'] = array(
    '#type' => 'select',
    '#title' => t('Limit day end time'),
    '#default_value' => isset($settings['limit_end']) && !empty($settings['limit_end']) ? $settings['limit_end'] : '',
    '#options' => array('' => '- None') + drupal_map_assoc($hours, 'studyroom_format_hours_ampm'),
    '#description' => t("Set the global day end time.  No time's in the module will be allowed to end after this time."),
    '#states' => array(
      'visible' => array(
        ':input[name*="set_hour_limits"]' => array('checked' => TRUE),
      ),
    ),
    '#fieldset' => 'default_values',
  );

  $context = array(
    'field' => $field,
    'instance' => $instance,
  );
  drupal_alter('studyroom_hours_field_instance_settings_form', $form, $context);

  $form['#element_validate'] = array('studyroom_hours_field_instance_settings_form_validate');
  return $form;
}

/**
 * Form validate handler for studyroom_hours_field_instance_settings_form().
 */
function studyroom_hours_field_instance_settings_form_validate($form, &$form_state) {
  $settings = $form_state['values']['instance']['settings'];

  // Hour Limits.
  if ($settings['set_hour_limits']) {
    if (empty($settings['limit_start'])) {
      form_set_error('instance][settings][limit_start', t('You must provide a start day time limit.'));
    }
    if (empty($settings['limit_end'])) {
      form_set_error('instance][settings][limit_end', t('You must provide a end day time limit.'));
    }

    // Check if end time greater that start time.
    if ($settings['limit_start'] > $settings['limit_end']) {
      form_set_error('instance][settings][limit_start', t('The day start time limit is less the day end time limit.'));
    }
  }
  else {
    form_set_value($form['limit_start'], NULL, $form_state);
    form_set_value($form['limit_end'], NULL, $form_state);
  }
}

/**
 * Implements hook_field_formatter_info().
 */
function studyroom_hours_field_formatter_info() {
  return array(
    'studyroom_hours' => array(
      'label' => t('Room hours'),
      'field types' => array('studyroom_hours'),
      'settings' => array(
        'format_type' => 'hours_ampm',
        'time_spacer' => 'to',
        'weekday_format' => 'full',
      ),
    ),
  );
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function studyroom_hours_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $element = array();

  $element['format_type'] = array(
    '#title' => t('Choose how users view times:'),
    '#type' => 'select',
    '#options' => date_format_type_options(),
    '#default_value' => $settings['format_type'],
    '#description' => t('To add or edit options, visit @date-time-page. <br />If you choose a format with a date, the date part will be ingorned.', array('@date-time-page' => l(t('Date and time settings'), 'admin/config/regional/date-time'))),
    '#weight' => 0,
  );

  $options = array(
    'to' => 'to',
    '-' => 'dash',
  );
  $element['time_spacer'] = array(
    '#title' => t('Choose spacer:'),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => $settings['time_spacer'],
    '#description' => t('Choose the space to use between the start and end time.'),
    '#required' => TRUE,
  );

  $options = array(
    'full' => 'Full',
    'abbr' => 'Abbr',
  );
  $element['weekday_format'] = array(
    '#title' => t('Weekday format'),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => $settings['weekday_format'],
    '#description' => t('Full weekday name or First 3 letter abbreviation.'),
  );

  $context = array(
    'field' => $field,
    'instance' => $instance,
    'view_mode' => $view_mode,
  );

  drupal_alter('studyroom_hours_field_formatter_settings_form', $element, $context);

  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function studyroom_hours_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $format_types = date_format_type_options();
  $summary = array();

  $format = FALSE;
  if (!empty($format_types[$settings['format_type']])) {
    $format = $format_types[$settings['format_type']];
  }
  if ($format) {
    $summary[] = t('Display times using the @format format.', array('@format' => $format));
  }
  else {
    $summary[] = t('Display times using the default format because the specified format (@format) is not defined.', array('@format' => $settings['format_type']));
  }

  $options = array(
    'to' => 'to',
    '-' => 'dash',
  );
  $summary[] = t('The %spacer will be the spacer between the start and end times.', array('%spacer' => $options[$settings['time_spacer']]));

  $options = array(
    'full' => t('Full textual representation of the day of the week. (ex. Sunday)'),
    'abbr' => t('First 3 letter abbreviation for the day of the week. (ex. Sun)'),
  );
  $summary[] = $options[$settings['weekday_format']];

  $context = array(
    'field' => $field,
    'instance' => $instance,
    'view_mode' => $view_mode,
  );
  drupal_alter('studyroom_hours_field_formatter_settings_summary', $summary, $context);

  return implode('<br />', $summary);
}

/**
 * Implements hook_field_formatter_view().
 */
function studyroom_hours_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();
  $settings = $display['settings'];
  $variables = array(
    'entity' => $entity,
    'entity_type' => $entity_type,
    'field' => $field,
    'instance' => $instance,
    'langcode' => $langcode,
    'items' => $items,
    'display' => $display,
    'attributes' => array(),
  );

  // Reorder weekdays to mach reginal settings.
  $items = date_week_days_ordered($items);

  // Give other modules a chance to prepare the entity before formatting it.
  drupal_alter('studyroom_hours_formatter_pre_view', $entity, $variables);

  switch ($display['type']) {
    case 'studyroom_hours':
      foreach ($items as $delta => $item) {
        if (empty($item['start_time'])) {
          continue;
        }
        $variables['weekday'] = studyroom_hours_field_formatter_process_weekdays($delta, $settings['weekday_format']);
        $variables['start_time'] = studyroom_hours_field_formatter_process_time($item['start_time'], $display['settings']['format_type']);;
        $variables['end_time'] = studyroom_hours_field_formatter_process_time($item['end_time'], $display['settings']['format_type']);;
        $output = theme('hours_display', $variables);
        if (!empty($output)) {
          $element[$delta] = array('#markup' => $output);
        }
      }
      break;
  }
  return $element;
}

/**
 * Function to create correctly formated time.
 */
function studyroom_hours_field_formatter_process_time($time, $format) {
  $date = new DateObject($time);
  return date_format_date($date, $format);
}

/**
 * Helper function to get days of the week in the correct format.
 */
function studyroom_hours_field_formatter_process_weekdays($day, $format) {
  if ($format == 'full') {
    $days = date_week_days_ordered(date_week_days_untranslated());
  }
  else {
    $days = date_week_days_ordered(date_week_days_abbr());
  }
  return $days[$day];
}

/**
 * Implements hook_field_validate().
 *
 * @see studyroom_hours_field_widget_error()
 */
function studyroom_hours_field_validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors) {
  $settings = $instance['settings'];
  $limit_start = studyroom_limit_start();
  $limit_end = studyroom_limit_end(TRUE);
  if (isset($settings['set_hour_limits']) && $settings['set_hour_limits']) {
    $local_limit_start = $settings['limit_start'] . ':00:00';
    $local_limit_end = $settings['limit_end'] . ':00:00';
    if ($local_limit_start > $limit_start) {
      $limit_start = $local_limit_start;
    }
    if ($local_limit_end < $limit_end) {
      $limit_end = $local_limit_end;
    }
  }

  $days = date_week_days_untranslated();
  foreach ($items as $delta => $item) {
    if (!empty($item['start_time'])) {
      if ($item['start_time'] < $limit_start) {
        $errors[$field['field_name']][$langcode][$delta]['start_time'] = array(
          'error' => 'studyroom_hours_limit_start',
          'delta' => $delta,
          'message' => t("The 'Start time' for %day (%time1) is before the allowed time of %time2.",
            array(
              '%day' => $days[$delta],
              '%time1' => studyroom_hours_mil_to_ampm($item['start_time']),
              '%time2' => studyroom_hours_mil_to_ampm($limit_start),
            )
          ),
        );
      }
      if ($item['end_time'] > $limit_end) {
        $errors[$field['field_name']][$langcode][$delta]['end_time'] = array(
          'error' => 'studyroom_hours_limit_end',
          'delta' => $delta,
          'message' => t("The 'End time' for %day (%time1) is after the allowed time of %time2.",
            array(
              '%day' => $days[$delta],
              '%time1' => studyroom_hours_mil_to_ampm($item['end_time']),
              '%time2' => studyroom_hours_mil_to_ampm($limit_end),
            )
          ),
        );
      }
    }
  }
}

/**
 * Implements hook_field_widget_error().
 *
 * Do not display default errors
 */
function studyroom_hours_field_widget_error($element, $error, $form, &$form_state) {
  switch ($error['error']) {
    case 'studyroom_hours_limit_start':
      form_error($element[$error['delta']]['start_time'], $error['message']);
      break;

    case 'studyroom_hours_limit_end':
      form_error($element[$error['delta']]['end_time'], $error['message']);
      break;

  }
}

/**
 * Implements hook_field_widget_properties_alter().
 *
 * Alters the widget properties of a field instance before it gets displayed.
 * Used to flag new entities so we can later tell if they need default values.
 */
function studyroom_hours_field_widget_properties_alter(&$widget, $context) {
  if ($widget['type'] == 'studyroom_hours') {
    $entity_type = $context['entity_type'];
    $entity = $context['entity'];
    $info = entity_get_info($entity_type);
    $id = $info['entity keys']['id'];
    $widget['is_new'] = FALSE;
    if (empty($entity->$id)) {
      $widget['is_new'] = TRUE;
    }
  }
}
