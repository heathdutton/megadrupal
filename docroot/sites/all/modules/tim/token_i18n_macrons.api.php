<?php
/**
 * @file
 * Hooks provided by the Token i18n Macron Module
 */

/**
 * Provides the ability to modify the dictionary of words and settings.
 *
 * @param array $dictionary
 *   An array of unicode character to ascii equivilant
 * @param array $options
 *   An array of values including:
 *   'parameters' - which are given to the token upon request
 *   'settings' - used to configure how to handle 'other' unicode characters
 *     'ascii only' - if true, any non ASCII characters are converted to default
 *     'default' - a default value for other characters
 *     'pathauto' -> if true, any URL generated through pathauto is filtered
 */
function hook_token_i18n_macrons_dictionary_options_alter(&$dictionary, &$options) {
  // A list non-ASCII of Maori letters.
  $maori = array(
    // Key is character to find and replace.
    // Value is what to replace it with.
    'ā' => 'a',
    'Ā' => 'A',
    'ē' => 'e',
    'Ē' => 'E',
    'ī' => 'i',
    'Ī' => 'I',
    'ō' => 'o',
    'Ō' => 'O',
    'ū' => 'u',
    'Ū' => 'U',
  );

  // Add these to the list.
  $dictionary = array_merge($maori, $dictionary);

  // Change the default character to an underscore.
  $options['settings']['default'] = '_';

  // Could check if the token parameters has something specific.
  if (array_key_exists('TI18MSafe', $options) && $options['TI18MSafe'] === TRUE) {
    // This allows the translation to be altered or settings modified.
    // Allow non-ASCII characters to pass through.
    $options['settings']['ascii only'] = FALSE;
  }

  // Disable pathauto functionality.
  $options['settings']['pathauto'] = FALSE;
}
