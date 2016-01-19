<?php

/**
 * @file
 * Defines helper functions.
 */

/**
 * Helper function to get clculated age.
 *
 * @param object $date1
 *   The date object.
 * @param object $date2
 *   The date object.
 *
 * @return string
 *   The calculated age.
 */
function age_calculator_get_results($date1, $date2) {
  $output = "";
  // Fetch the configuration from variables table.
  $age_calculator_output = variable_get('age_calculator_output');
  // If the configrations are not saved by admin, we shall use the default one.
  if (empty($age_calculator_output)) {
    // Including helper functions inc file.
    module_load_include('inc', 'age_calculator', 'age_calculator.settings');
    // Fetching the defult configuration.
    $age_calculator_output = age_calculator_default_output_options();
  }
  // Fetching defined variables, based on which output will be displayed.
  $age_calculator_years_months_days = $age_calculator_output['age_calculator_years_months_days'];
  $age_calculator_months_days = $age_calculator_output['age_calculator_months_days'];
  $age_calculator_weeks_days = $age_calculator_output['age_calculator_weeks_days'];
  $age_calculator_days = $age_calculator_output['age_calculator_days'];
  $age_calculator_hours = $age_calculator_output['age_calculator_hours'];
  $age_calculator_minutes = $age_calculator_output['age_calculator_minutes'];
  $age_calculator_seconds = $age_calculator_output['age_calculator_seconds'];
  // Calculations in years, months & days.
  if (!empty($age_calculator_years_months_days)) {
    // Calling helper function to form output.
    $formatted_output = _age_calculator_results_formation($date1, $date2, 'age_calculator_years_months_days');
    if (strlen($output) > 0) {
      $result = "<span class='age_calculator_years_months_days'>";
      $result .= " " . t("Or !formatted_output", array('!formatted_output' => $formatted_output));
      $result .= "</span>";
      $output .= $result;
    }
    else {
      $result = "<span class='age_calculator_years_months_days'>";
      $result .= $formatted_output;
      $result .= "</span>";
      $output .= $result;
    }
  }
  // Calculations in months & days.
  if (!empty($age_calculator_months_days)) {
    // Calling helper function to form output.
    $formatted_output = _age_calculator_results_formation($date1, $date2, 'age_calculator_months_days');
    if (strlen($output) > 0) {
      $result = "<span class='age_calculator_months_days'>";
      $result .= " " . t("Or !formatted_output", array('!formatted_output' => $formatted_output));
      $result .= "</span>";
      $output .= $result;
    }
    else {
      $result = "<span class='age_calculator_months_days'>";
      $result .= $formatted_output;
      $result .= "</span>";
      $output .= $result;
    }
  }
  // Calculations in weeks & days.
  if (!empty($age_calculator_weeks_days)) {
    // Calling helper function to form output.
    $formatted_output = _age_calculator_results_formation($date1, $date2, 'age_calculator_weeks_days');
    if (strlen($output) > 0) {
      $result = "<span class='age_calculator_weeks_days'>";
      $result .= " " . t("Or !formatted_output", array('!formatted_output' => $formatted_output));
      $result .= "</span>";
      $output .= $result;
    }
    else {
      $result = "<span class='age_calculator_weeks_days'>";
      $result .= $formatted_output;
      $result .= "</span>";
      $output .= $result;
    }
  }
  // Calculations in days.
  if (!empty($age_calculator_days)) {
    // Calling helper function to form output.
    $formatted_output = _age_calculator_results_formation($date1, $date2, 'age_calculator_days');
    if (strlen($output) > 0) {
      $result = "<span class='age_calculator_days'>";
      $result .= " " . t("Or !formatted_output", array('!formatted_output' => $formatted_output));
      $result .= "</span>";
      $output .= $result;
    }
    else {
      $result = "<span class='age_calculator_days'>";
      $result .= $formatted_output;
      $result .= "</span>";
      $output .= $result;
    }
  }
  // Calculation in hours.
  if (!empty($age_calculator_hours)) {
    // Calling helper function to form output.
    $formatted_output = _age_calculator_results_formation($date1, $date2, 'age_calculator_hours');
    if (strlen($output) > 0) {
      $result = "<span class='age_calculator_hours'>";
      $result .= " " . t("Or !formatted_output", array('!formatted_output' => $formatted_output));
      $result .= "</span>";
      $output .= $result;
    }
    else {
      $result = "<span class='age_calculator_hours'>";
      $result .= $formatted_output;
      $result .= "</span>";
      $output .= $result;
    }
  }
  // Calculation in minutes.
  if (!empty($age_calculator_minutes)) {
    // Calling helper function to form output.
    $formatted_output = _age_calculator_results_formation($date1, $date2, 'age_calculator_minutes');
    if (strlen($output) > 0) {
      $result = "<span class='age_calculator_minutes'>";
      $result .= " " . t("Or !formatted_output", array('!formatted_output' => $formatted_output));
      $result .= "</span>";
      $output .= $result;
    }
    else {
      $result = "<span class='age_calculator_minutes'>";
      $result .= $formatted_output;
      $result .= "</span>";
      $output .= $result;
    }
  }
  // Calculation in seconds.
  if (!empty($age_calculator_seconds)) {
    // Calling helper function to form output.
    $formatted_output = _age_calculator_results_formation($date1, $date2, 'age_calculator_seconds');
    if (strlen($output) > 0) {
      $result = "<span class='age_calculator_seconds'>";
      $result .= " " . t("Or !formatted_output", array('!formatted_output' => $formatted_output));
      $result .= "</span>";
      $output .= $result;
    }
    else {
      $result = "<span class='age_calculator_seconds'>";
      $result .= $formatted_output;
      $result .= "</span>";
      $output .= $result;
    }
  }
  // Adding helper text.
  $helper_text = "<span class='age_calculator_helper_text'>" . t('Your age is') . " " . "</span>";
  // Returning calculated age.
  return $helper_text . $output;
}

/**
 * Helper function to format the age calculator output.
 *
 * @param object $date1
 *   The date object.
 * @param object $date2
 *   The date object.
 * @param string $output_format
 *   The output format configured by admin.
 *
 * @return string
 *   The computed age string.
 */
function _age_calculator_results_formation($date1, $date2, $output_format) {
  // Calculating difference in objects.
  $date_difference = $date1->diff($date2);
  switch ($output_format) {
    // Y Years M Months D Days.
    case 'age_calculator_years_months_days':
      // Formatting output.
      $years = $date_difference->y;
      $months = $date_difference->m;
      $days = $date_difference->d;
      $years_months_days = t('!y Years !m Months !d Days', array(
        '!y' => $years,
        '!m' => $months,
        '!d' => $days));
      // Returning output.
      return $years_months_days;

    // M Months D Days.
    case 'age_calculator_months_days':
      // Formatting output.
      $months = ($date_difference->y * 12) + $date_difference->m;
      $days = $date_difference->d;
      $months_days = t('!m Months !d Days', array('!m' => $months, '!d' => $days));
      // Returning output.
      return $months_days;

    // W Weeks D Days.
    case 'age_calculator_weeks_days':
      // Formatting output.
      $days = $date_difference->days;
      $weeks = (int) ($days / 7);
      $additional_days = $days % 7;
      $weeks_days = t('!w Weeks !d Days', array('!w' => $weeks, '!d' => $additional_days));
      // Returning output.
      return $weeks_days;

    // D Days.
    case 'age_calculator_days':
      // Formatting output.
      $days = $date_difference->days;
      $days = t('!d Days', array('!d' => $days));
      // Returning output.
      return $days;

    // H Hours.
    case 'age_calculator_hours':
      // Formatting output.
      $days_to_hours_output = $date_difference->days * 24;
      $hours = t('!h Hours', array('!h' => $days_to_hours_output));
      // Returning output.
      return $hours;

    // M Minutes.
    case 'age_calculator_minutes':
      // Formatting output.
      $days_to_minutes_output = $date_difference->days * 24 * 60;
      $minutes = t('!m Minutes', array('!m' => $days_to_minutes_output));
      // Returning output.
      return $minutes;

    // S Seconds.
    case 'age_calculator_seconds':
      // Formatting output.
      $days_to_seconds_output = $date_difference->days * 24 * 60 * 60;
      $seconds = t('!s Seconds', array('!s' => $days_to_seconds_output));
      // Returning output.
      return $seconds;
  }
}
