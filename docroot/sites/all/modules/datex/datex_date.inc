<?php

/**
 * @file
 * Provides Jalali support for date module.
 */

// _____________________________________________________________________HELPERS.
/**
 * Checks to see if a field is supported.
 *
 * If month is present, Then it's supported. But if month is not present then 
 * none of smaller granularities should be present either, Otherwise it's 
 * imposible to calculate correct date, Since date module sets month to 1.
 */
function _datex_date_granularities_supported($g) {
  if (!in_array('month', $g) && count($g) > 1) {
    return FALSE;
  }
  return TRUE;
}

/**
 * Name of available schemas to display in form.
 */
function _datex_options() {
  return array(
    'disabled' => t('Force disable'),
    'default' => t('Use default schema'),
  ) + drupal_map_assoc(_datex_schema_list());
} 

/**
 * Find suitable calendar for field.
 */
function _datex_field_calendar($granularities, $schema = 'default', $tz = NULL) {
  if ($schema == 'disabled') {
    return FALSE;
  }

  $locale = _datex_language_calendar(NULL, NULL, $schema);
  if (!$locale) {
    return FALSE;
  }

  if(!_datex_date_granularities_supported($granularities)) {
    return FALSE;
  }

  return datex_factory($locale, NULL, NULL, $tz, _datex_cfg('useintl'));
}

/**
 * Helper functin copied from core to set input rather than value.
 */
function _datex_form_set_input($element, $input, &$form_state) {
  drupal_array_set_nested_value($form_state['input'], $element['#parents'], $input, TRUE);
}

// ___________________________________________________________________USERLAND.
/**
 * Implemetns hook_date_formatter_dates_alter().
 *
 * Converts gregorian date to Jalali date before display (such as node view).
 */
function datex_date_formatter_dates_alter(&$dates, $context) {
  if (isset($context['display']['settings']['datex_schema'])) {
    $schema = $context['display']['settings']['datex_schema'];
  }
  else {
    $schema = 'default';
  }
  $tz = $dates['value']['local']['timezone'];
  $g = array_values($context['field']['settings']['granularity']);
  foreach ($g as $index => $value) {
    if ($value === 0) {
      unset($g[$index]);
    }
  }
  if(!$calendar = _datex_field_calendar($g, $schema, $tz)) {
    return;
  }

  foreach ($dates as $name => &$date) {
    $calendar->setTimestamp($date['local']['object']->format('U'));
    $calendar->fixByGranularities($context['field']['settings']['granularity']);

    $calendar->setTime(
      (int) $date['local']['object']->format('G'), 
      (int) $date['local']['object']->format('i'),
      (int) $date['local']['object']->format('s')
    );

    $dates[$name]['formatted'] = $calendar->format($context['format']);
  }
}

function datex_date_combo_process_alter(&$element, &$form_state, $context) {
  //dargs();
}

/**
 * Handles date field.
 */
function datex_date_select_process_alter(&$element, &$form_state, $context) {
  if (isset($element['#instance']['widget']['settings']['datex_schema'])) {
    $schema = $element['#instance']['widget']['settings']['datex_schema'];
  }
  else {
    $schema = 'default';
  }
  
  $tz = $element['#date_timezone'];
  if (!$calendar = _datex_field_calendar($element['#granularity'], $schema, $tz)) {
    return;
  }

  $calendar->setTimestamp(strtotime($element['#default_value']));
  $year_now = $calendar->format('Y');

  static $convert = array('year' => 'Y', 'month' => 'n', 'day' => 'j');
  foreach ($convert as $name => $format) {
    $v = $calendar->format($format);
    if (isset($element[$name]) && !empty($element[$name]['#default_value'])) {
      $element[$name]['#default_value'] = $v;
    }
    if (isset($element[$name]) && !empty($element['#value'][$name])) {
      $element['#value'][$name] = $v;
    }
  }

  // Generating Year Option (if field has year granularity):
  if (in_array('year', $element['#granularity'])) {
    list($year_before, $year_after) = explode(':', $element['#date_year_range']);

    // If year field is not required, add an empty value as drupal does.
    if ($element['#required']) {
      $element['year']['#options'] = array();
    }
    else {
      $element['year']['#options'] = array('' => t('-Year'));
    }

    // Now add actual year options.
    for ($year = $year_now + $year_before; $year <= $year_now + $year_after; $year++) {
      $element['year']['#options'][$year] = $year;
    }
  }


  // Generating month Option (if field has month granularity).
  if (in_array('month', $element['#granularity'])) {
    $names = $calendar->getInfo('names');

    // If field is not required, add an empty value as drupal does.
    if ($element['#required']) {
    $element['month']['#options'] = array();
    }
    else {
      $element['month']['#options'] = array('' => t('-Month'));
    }

    // Now add month options.
    for ($i = 0; $i < 12; $i++) {
      $element['month']['#options'][$i + 1] = $names['months'][$i];
    }
  }
}

/**
 * Form element validation callback.
 *
 * Converts a Jalali date to Gregorian date just before actual validation of
 * date element happens.
 */
function _datex_element_validate_callback(&$element, &$form_state) {
  if(isset($element['#type']) && $element['#type'] == 'date_popup') {
    if (module_exists('datex_popup')) {
      return _datex_popup_element_validate_callback($element, $form_state);
    }
    else {
      return;
    }
  }

  if (isset($element['#instance']['widget']['settings']['datex_schema'])) {
    $schema = $element['#instance']['widget']['settings']['datex_schema'];
  }
  else {
    $schema = 'default';
  }

  $tz = $element['#date_timezone'];
  if (!$calendar = _datex_field_calendar($element['#granularity'], $schema, $tz)) {
    return;
  }

  $date = array();
  foreach ($element['#granularity'] as $granul) {
    $date[$granul] = $element[$granul]['#value'];
  }

  $calendar->setDatetime($date);
  // $calendar->fixGranularities($element['#granularity']);

  foreach (array('year' => 'Y', 'month' => 'n', 'day' => 'j') as $name => $format) {
    if (isset($element['#value'][$name])) {
      $value = $calendar->xformat($format);
      $element['#value'][$name] = $value;
      $element[$name]['#value'] = $value;
    }
  }

  form_set_value($element, $element['#value'], $form_state);
  _datex_form_set_input($element, $element['#value'], $form_state);
}

// ________________________________________________________________________INFO.
/**
 * Adds datex as a date form element validator.
 *
 * So we get a chance to convert Jalali to Greg.
 */
function datex_element_info_alter(&$elements) {
  foreach (array('select', 'popup') as $name) {
    $name = 'date_' . $name;
    if (isset($elements[$name])) {
      if (isset($elements[$name]['#element_validate'])) {
        $validator = $elements[$name]['#element_validate'];
      }
      else {
        $validator = array();
      }
  
      $validator = array_merge(array('_datex_element_validate_callback'), $validator);
      $elements[$name]['#element_validate'] = $validator;
    }
  }
}

/**
 * Add datex configuration to date field.
 *
 * Add an extra option to control datex for each field instance display. 
 */
function datex_field_formatter_info_alter(&$info) {
  foreach (array('date_default', 'date_plain') as $formatter) {
    if (isset($info[$formatter])) {
      $info[$formatter]['settings']['datex_schema'] = 'default';
    }
  }
}

/**      
 * Implements hook_field_info_alter().
 *
 * Add an extra option to control datex for each field instance. 
 */
function datex_field_info_alter(&$info) {
  foreach (array('datetime', 'date', 'datestamp') as $type) {
    if (isset($info[$type]['instance_settings'])) {
      $info[$type]['instance_settings']['datex_schema'] = 'default';
    }
  }
}

// __________________________________________________________________ADMIN AREA.
/**
 * Implements hook_date_field_instance_settings_form_alter().
 *
 * For per-field control.
 */
function datex_date_field_widget_settings_form_alter(&$form, $context) {
  $settings = $context['instance']['widget']['settings'];

  if (!_datex_date_granularities_supported($context['field']['settings']['granularity'])) {
    $options = array('disabled' => t('Force disabled (Field not supported)'));
    $default = 'disabled';
    $desc = t('Datex is disabled for fields without month granularity but with smaller granularity.');
  }
  else {
    $options = _datex_options();
    $default = isset($settings['datex_schema']) ? $settings['datex_schema'] : 'default';
    $desc = '';
  }

  $form['datex_schema'] = array(
    '#title' => t('Datex schema to use for this field.'),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => $default,
    '#description' => $desc,
  );
}

/**
 * implements hook_date_field_formatter_settings_form_alter().
 */
function datex_date_field_formatter_settings_form_alter(&$form, &$form_state, $context) {
  $viewmode = $context['view_mode'];
  $settings = $context['instance']['display'][$viewmode]['settings'];

  if (!_datex_date_granularities_supported($context['field']['settings']['granularity'])) {
    $options = array('disabled' => t('Force disabled (Field not supported)'));
    $default = 'disabled';
    $desc = t('Datex is disabled for fields without month granularity but with day granularity.');
  }
  else {
    $default = isset($settings['datex_schema']) ? $settings['datex_schema'] : 'default';
    $options = _datex_options();
    $desc = '';
  }

  $form['datex_schema'] = array(
    '#title' => t('Datex schema to use for this field.'),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => $default,
    '#description' => $desc,
  );
}

/**
* implements hook_date_field_formatter_settings_summary_alter().
 */
function datex_date_field_formatter_settings_summary_alter(&$summary, $context) {
  if(!_datex_date_granularities_supported($context['field']['settings']['granularity'])) {
    $summary[] = t('Datex: Force disabled (Field not supported)');
  }
  else {
    $viewmode = $context['view_mode'];
    $settings = $context['instance']['display'][$viewmode]['settings']; 
  
    if (isset($settings['datex_schema'])) {
      $settings = $settings['datex_schema'];
    }
    else {
      $settings = t('default');
    }

    $summary[] = t('Datex: @calendar', array('@calendar' => $settings));
  }
}

