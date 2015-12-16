<?php
/**
 * @file
 * Validation functions.
 */

/**
 * Element validate callback for the file destination field.
 *
 * Remove slashes from the beginning and end of the destination value and ensure
 * that the file directory path is not included at the beginning of the value.
 */
function _cmp_file_directory_validate($element, &$form_state) {
  // Strip slashes from the beginning and end of $widget['file_directory'].
  $value = trim($element['#value'], '\\/');
  form_set_value($element, $value, $form_state);
}

/**
 * Element validate callback. Validates a URL.
 */
function _cmp_url_validate($element, &$form_state) {
  if (!empty($element['#value'])) {
    if (filter_var($element['#value'], FILTER_VALIDATE_URL)) {
      form_set_value($element, $element['#value'], $form_state);
    }
    else {
      form_error($element, t('Page URL is not a valid URL.'));
    }
  }
  else {
    form_set_value($element, '', $form_state);
  }
}

/**
 * Element validate callback. Validates a URL.
 */
function _cmp_percent_validate($element, &$form_state) {
  if (!empty($element['#value'])) {
    if ((int) $element['#value'] <= 0 || (int) $element['#value'] > 100) {
      form_set_value($element, 75, $form_state);
    }
  }
  else {
    form_set_value($element, 75, $form_state);
  }
}
