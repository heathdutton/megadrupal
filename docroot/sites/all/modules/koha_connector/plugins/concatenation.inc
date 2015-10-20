<?php

/**
 * @file
 * Concatenation plugin.
 *
 * This plugin allows to choose a concatenation method for a given field.
 */

/**
 * Callback for Concatenation plugin.
 */
function koha_connector_plugin_concatenation($from, $conf) {

  // The array of strings we will return.
  $return = array();

  list($tag) = explode('$', $conf['value']);

  $values = array_key_exists($tag, $from['datafields']) ? $from['datafields'][$tag] : array();

  // If we have a summary to follow, we do it.
  if (array_key_exists('plugin_params', $conf)) {

    // Getting plugin params.
    $params = $conf['plugin_params'];

    // For each field.
    $i = 0;
    foreach ($values as $field) {

      // For each group of subfields.
      foreach ($field['subfields'] as $fkey => $subfields) {

        // For each subfield.
        foreach ($subfields as $key => $add) {

          // If we have parameters for this subfield...
          if (array_key_exists($key, $params)) {

            // We add the prefix (if it's not the first field).
            if (array_key_exists('before', $params[$key])) {
              $add = $params[$key]['before'] . $add;
            }

            // And we add the suffix.
            if (array_key_exists('after', $params[$key])) {
              $add = $add . $params[$key]['after'];
            }

            // We add the possibly prefixed and
            // suffixed value to the result.
            if (array_key_exists($i, $return)) {
              $return[$i] .= $add;
            }
            else {
              $return[$i] = $add;
            }
          }
        }
      }
      $i++;
    }
  }
  else {
    // Else, we use a default concatenation.
    $i = 0;
    foreach ($values as $field) {
      foreach ($field['subfields'] as $fkey => $subfields) {
        foreach ($subfields as $key => $add) {
          $return[$i] .= $add . "|";
        }
      }
      $i++;
    }
  }
  return $return;
}
