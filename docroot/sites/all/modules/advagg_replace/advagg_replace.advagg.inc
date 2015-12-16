<?php
/**
 * @file
 * Custom AdvAgg hook implementations.
 */

/**
 * Implements hook_advagg_get_css_aggregate_contents_alter().
 */
function advagg_replace_advagg_get_css_aggregate_contents_alter(&$data, $files, $aggregate_settings) {
  advagg_replace_search_replace('css', $data, $files, $aggregate_settings);
}

/**
 * Implements hook_advagg_get_js_aggregate_contents_alter().
 */
function advagg_replace_advagg_get_js_aggregate_contents_alter(&$data, $files, $aggregate_settings) {
  advagg_replace_search_replace('js', $data, $files, $aggregate_settings);
}

/**
 * Perform the search/replace action.
 *
 * @param string $type
 *   Either 'css' or 'js'.
 * @param string $data
 *   The code to be processed.
 * @param array $files
 *   The list of files being processed.
 * @param array $aggregate_settings
 *   All settings being used to compile this data.
 */
function advagg_replace_search_replace($type, &$data, $files, $aggregate_settings) {
  if ($type == 'css' || $type == 'js') {
    // Load the search strings.
    $settings = array();
    if (isset($aggregate_settings['variables']['advagg_replace_' . $type . '_search'])) {
      $settings = $aggregate_settings['variables']['advagg_replace_' . $type . '_search'];
    }

    // Load the replacements.
    $replacements = array();
    if (isset($aggregate_settings['variables']['advagg_replace_' . $type . '_replace'])) {
      $replacements = $aggregate_settings['variables']['advagg_replace_' . $type . '_replace'];
    }

    // Replace the string, if applicable.
    if (!empty($settings) && is_array($settings) && !empty($replacements) && is_array($replacements)) {
      foreach ($settings as $pos => $search) {
        if (isset($replacements[$search['string']])) {
          $data = str_replace($search['string'], $replacements[$search['string']], $data);
        }
      }
    }
  }
}
