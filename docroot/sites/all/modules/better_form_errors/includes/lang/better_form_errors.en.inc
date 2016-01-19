<?php

/**
 * @file
 * Part of the "Better Form Errors" module that will be included for
 * catching and setting error messages in Drupal's original language
 * = English (code "en"), as well as more specific English language
 * specifications such as "en-GB".
 */

/**
 * Helper function to catch an existing Drupal form validation error message.
 *
 * Conditionally called by the function "better_form_errors_form_validate".
 *
 * @return string
 *   The error message to set.
 */
function _better_form_errors_catch_message($message, $lang) {
  $intro = 'The field ';
  // Strings from core's "includes/form.inc":
  // Translation source (msgid):
  // "!name field is required."
  if (preg_match('/^(.+)( field)( is required\.)$/', $message, $matches)) {
    $field = $matches[1];
    $explanation = $matches[3];
    $message = better_form_errors_compose_message($intro, $field, $explanation, $lang);
  }
  // Translation source (msgid):
  // "!name cannot be longer than %max characters but is currently %length \
  // characters long."
  elseif (preg_match('/^(.+)( cannot be longer than \<em class="placeholder"\>\d+\<\/em\> characters but is currently \<em class="placeholder"\>\d+\<\/em\> characters long\.)$/', $message, $matches)) {
    $field = $matches[1];
    $explanation = $matches[2];
    $message = better_form_errors_compose_message($intro, $field, $explanation, $lang);
  }
  // Webform Grid field having "Unique" enabled ("webform/components/grid.inc"):
  // Translation source (msgid):
  // "!title is not allowed to have the same answer for more than one question."
  elseif (preg_match('/^(.+)( is not allowed to have the same answer for more than one question\.)$/', $message, $matches)) {
    $field = $matches[1];
    $explanation = $matches[2];
    $message = better_form_errors_compose_message($intro, $field, $explanation, $lang);
  }
  return $message;
}
