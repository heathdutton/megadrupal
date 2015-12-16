<?php
/**
 * @file
 * SlideRoom hook definitions.
 */

/**
 * Define SlideRoom API application exports.
 *
 * Implementations of this hook return an associative array in which the keys
 * are a unique export name and the values are an array of application export
 * request parameters.
 *
 * Results are fetched from the API asynchronously. When the export is complete,
 * this module triggers hook_slideroom_application_export_complete() with the
 * result data.
 *
 * @return array
 *    An array of application exports.
 */
function hook_slideroom_application_export() {
  return array();
}

/**
 * Handle SlideRoom API application export data.
 *
 * This hook is triggered after the SlideRoom API has successfully finished
 * generating application export data. It is passed the original request data
 * that was defined by an implementation of hook_slideroom_application_export()
 * and the response data returned by the SlideRoom API.
 *
 * @param array $application_result
 *    A slideroom result record containing the data that was used to generate the export.
 * @param array $export_result
 *    A slideroom result record containing successfully exported data from the SlideRoom API.
 */
function hook_slideroom_application_export_complete($application_result, $export_result) {
  return;
}
