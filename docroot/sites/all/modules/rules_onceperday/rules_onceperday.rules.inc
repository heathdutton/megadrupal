<?php

/**
 * Implementation of hook_rules_event_info().
 */
function rules_onceperday_rules_event_info() {
  return array(
    'once_per_day' => array(
      'group' => t('Once per day'),
      'label' => t('Once per day'),
      'help' => t('Event fired when cron runs, but a maximum of once per day.')));
}

/**
 * Implementation of hook_rules_condition_info().
 */
function rules_onceperday_rules_condition_info() {
  return array(
    'rules_onceperday_condition_dayofweek' => array(
      'label' => t('Current day of the week'),
      'parameter' => array(
        'weekday' => array(
          'label' => t('Day of the week is a'),
          'type' => 'integer',
          'restriction' => 'input',
          'options list' => 'rules_onceperday_weekday_options')),
      'group' => t('Once per day'),
      'callbacks' => array(
        'execute' => 'rules_onceperday_compare_weekday')),
    'rules_onceperday_condition_dayofmonth' => array(
      'label' => t('Current day of the month'),
      'parameter' => array(
        'comparison' => array(
          'label' => t('Comparison'),
          'description' => t('The comparison operator.'),
          'type' => 'text',
          'options list' => 'rules_onceperday_comparison_options',
          'restriction' => 'input',
          'default value' => '=='),
        'monthday' => array(
          'label' => t('Day of the month'),
          'description' => t('The day number in the month, between 1 and 31.'),
          'type' => 'integer',
          'restriction' => 'input')),
      'group' => t('Once per day'),
      'callbacks' => array(
        'execute' => 'rules_onceperday_compare_monthday')));
}

/**
 * Callback function for rules condition options.
 */
function rules_onceperday_weekday_options() {
  return array(
    0 => 'Sunday',
    1 => 'Monday',
    2 => 'Tuesday',
    3 => 'Wednesday',
    4 => 'Thursday',
    5 => 'Friday',
    6 => 'Saturday',
    7 => 'Working day (Monday to Friday)',
    8 => 'Weekend (Saturday or Sunday)');
}

function rules_onceperday_comparison_options() {
  return array(
    '==' => 'equals',
    '>' => 'is greater than',
    '<' => 'is less than',
    '>=' => 'is greater than, or equal to',
    '<=' => 'is less than, or equal to');
}

/**
 * Callback function for rules condition test.
 */
function rules_onceperday_compare_weekday($weekday_selected) {
  $dayofweek = date('w');
  if ($weekday_selected <= 6) {
    return $dayofweek == $weekday_selected;
  }
  elseif ($weekday_selected == 7) {
    return ($dayofweek >= 1 and $dayofweek <= 5);
  }
  elseif ($weekday_selected == 8) {
    return ($dayofweek == 0 or $dayofweek == 6);
  }
}

/**
 * Callback function for rules condition test.
 */
function rules_onceperday_compare_monthday($comparison, $monthday_entered) {
  watchdog('ajc', print_r(func_get_args(), TRUE));
  $dayofmonth = date('d');
  switch ($comparison) {
    case '==':
    default:
      return $dayofmonth == $monthday_entered;
    case '>':
      return $dayofmonth > $monthday_entered;
    case '<':
      return $dayofmonth < $monthday_entered;
    case '>=':
      return $dayofmonth >= $monthday_entered;
    case '<=':
      return $dayofmonth <= $monthday_entered;
  }
}

/**
 * Validation function for rules condition input form.
 */
function rules_onceperday_condition_dayofmonth_validate($condition) {
  $day = $condition->settings['monthday'];
  if ($day > 31) {
    form_set_error('monthday', 'Day of the month cannot be greater than 31.');
  }
  elseif ($day > 30) {
    drupal_set_message('The condition will not be satisfied in February, April, June, September, or November.', 'warning', FALSE);
  }
  elseif ($day > 28) {
    drupal_set_message('The condition will not be satisfied in February, except in leap years.', 'warning', FALSE);
  }
  elseif ($day < 1) {
    form_set_error('monthday', 'Day of the month cannot be less than 1.');
  }
}