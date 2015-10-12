<?php

/**
 * @file
 * Hooks provided by the "Better Form Errors" module.
 */

/**
 * Override the default error message replacement rules.
 *
 * Allows to set custom error messages in the language of the active interface.
 *
 * @param string $message
 *   The original Drupal form validation error message.
 * @param string $lang
 *   The language code representing the active interface language.
 *
 * @return string
 *   The new error message to replace the original one,
 *   or NULL if the module's built-in replacement rules should be used.
 */
function hook_better_form_errors_catch_message($message, $lang) {
  switch ($lang) {

    case 'de':
      // Replacement rules for German.
      // Translation source (msgid):
      // "!title is not allowed to have the same answer for more than one \
      // question."
      if (preg_match('/^(In )(.+)( darf dieselbe Antwort nur einmal vergeben werden\.)$/', $message, $matches)) {
        $intro = 'Im Feld ';
        $field = $matches[2];
        $explanation = $matches[3];
        $message = better_form_errors_compose_message($intro, $field, $explanation, $lang);
      }
      break;

    case 'fr':
      // Replacement rules for French...
      break;

    default:
      // Use the "Better Form Errors" module's built-in replacement rules for
      // all other languages:
      $message = NULL;
  }

  return $message;
}
