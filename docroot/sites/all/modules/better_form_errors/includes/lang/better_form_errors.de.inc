<?php

/**
 * @file
 * Part of the "Better Form Errors" module that will be included for
 * catching and setting error messages in the language German (code "de").
 */

/**
 * Helper function to catch an existing Drupal form validation error message.
 *
 * @return string
 *   The error message to set.
 */
function _better_form_errors_catch_message($message, $lang) {
  // Strings from core's "includes/form.inc":
  // Translation source (msgid):
  // "!name field is required."
  // => Translation string (msgstr) -> "drupal-7.35.de.po":
  // "Das Feld „!name” ist erforderlich."
  if (preg_match('/^(Das Feld )(.+)( ist erforderlich\.)$/', $message, $matches)) {
    $intro = $matches[1];
    $field = $matches[2];
    $explanation = $matches[3];
    $message = better_form_errors_compose_message($intro, $field, $explanation, $lang);
  }
  // Translation source (msgid):
  // "!name cannot be longer than %max characters but is currently %length \
  // characters long."
  // => Translation string (msgstr) -> "drupal-7.35.de.po":
  // "!name darf nicht länger als %max Zeichen sein, ist aber momentan %length \
  // Zeichen lang."
  elseif (preg_match('/^(.+)( darf nicht länger als \<em class="placeholder"\>\d+\<\/em\> Zeichen sein, ist aber momentan \<em class="placeholder"\>\d+\<\/em\> Zeichen lang\.)$/', $message, $matches)) {
    $intro = 'Das Feld ';
    $field = $matches[1];
    $explanation = $matches[2];
    $message = better_form_errors_compose_message($intro, $field, $explanation, $lang);
  }
  // Webform Grid field having "Unique" enabled ("webform/components/grid.inc"):
  // Translation source (msgid):
  // "!title is not allowed to have the same answer for more than one question."
  // => Translation string (msgstr) -> "webform-7.x-4.8.de.po":
  // "In !title darf dieselbe Antwort nur einmal vergeben werden."
  elseif (preg_match('/^(In )(.+)( darf dieselbe Antwort nur einmal vergeben werden\.)$/', $message, $matches)) {
    $intro = $matches[1];
    $field = $matches[2];
    $explanation = $matches[3];
    $message = better_form_errors_compose_message($intro, $field, $explanation, $lang);
  }
  return $message;
}
