<?php

/**
 * @file
 * Miscellaneous functions for the views multi-column display module.
 */

/**
 * Save the page model chosen for views display.
 */
function ofed_vmcd_misc_view_display_save() {
  // Fetch the query string from the URL.
  $query_string = drupal_get_query_array($_SERVER['QUERY_STRING']);
  // Check if a parameter is missing.
  // If a parameter is missing.
  if (empty($query_string) || empty($query_string['view']) || empty($query_string['display']) || empty($query_string['path']) || empty($query_string['redirect']) || empty($query_string['number'])
  ) {
    // Set error message.
    drupal_set_message(t('Missing parameter(s) to set the display of this views.'), 'error');
    // If the redirect is present.
    if (!empty($query_string['redirect'])) {
      // Redirect to target.
      drupal_goto($query_string['redirect']);
    }
    else {
      // Redirect to front page (to avoid to have blank page).
      drupal_goto('<front>');
    }
  }
  // If all parameter are present.
  else {
    // Check if the number of column is correct.
    $colums = variable_get('ofed_vmcd_allowed_choices_columns', '1, 2, 3');
    $colums = explode(',', $colums);
    if (!is_array($colums)) {
      // Set error message.
      drupal_set_message(t('Allowed columns choices not correclty formatted'));
      // Redirect to target.
      drupal_goto($query_string['redirect']);
    }
    if (!in_array($query_string['number'], $colums)) {
      // Set error message.
      $message = t('Allowed columns choices not valid. Only "!values" are allowed.', array('!values' => implode(',', $colums)));
      drupal_set_message(check_plain($message), 'error');
      // Redirect to target.
      drupal_goto($query_string['redirect']);
    }

    // Set the variable to remember the display choice.
    // Set the name of the variable.
    $variable_name = 'ofed_vmcd_' . $query_string['view'] . '_' . $query_string['display'];
    // Get the latest value of the variable.
    $variable_content = variable_get($variable_name, array());
    // Set the new content of the variable.
    $variable_content[$query_string['path']] = $query_string['number'];
    variable_set($variable_name, $variable_content);
    // Redirect.
    drupal_goto($query_string['redirect']);
  }
}
