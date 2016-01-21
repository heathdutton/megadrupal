<?php

/**
 * @file
 * Webform localization hooks.
 */

/**
 * Module specific instance of _webform_localization_csv_header().
 *
 * This allows each component to translate the analysis report CSV header.
 * These are found in under 'translated_strings' in the 'extra' array of the
 * component, which is build when the component is inserted / updated, or
 * when all webform strings are updated from
 * admin/config/regional/translate/i18n_string.
 *
 * @param $header
 *   The untranslated CSV header to be altered
 * @param $component
 *   The webform component
 *
 * @return
 *   The translated header array.
 */
function _webform_localization_csv_header_component($header, $component) {
  $header[1] = 'Custom string';
  return $header;
}

/**
 * Module specific instance of _webform_localization_csv_data().
 *
 * This allows each component to translate the analysis report CSV data.
 *
 * @param $data
 *   The untranslated CSV data to be altered
 * @param $component
 *   The webform component
 * @param $submission
 *   The webform submission
 *
 * @return
 *   The translated data array.
 */
function _webform_localization_csv_data_component($data, $component, $submission) {
  $data[1] = 'Custom string';
  return $data;
}

/**
 * Module specific instance of _webform_localization_analysis_data().
 *
 * This allows each component to translate the analysis report data.
 * These are found in under 'translated_strings' in the 'extra' array of the
 * component, which is build when the component is inserted / updated, or
 * when all webform strings are updated from
 * admin/config/regional/translate/i18n_string.
 *
 * @param $data
 *   The untranslated data to be altered
 * @param $node
 *   The webform node
 * @param $component
 *   The webform component
 *
 * @return
 *   The translated data array.
 */
function _webform_localization_analysis_data_component($data, $node, $component) {
  $data[1] = 'Custom string';
  return $data;
}
