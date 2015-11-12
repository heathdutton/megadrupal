<?php
/**
 * @file
 * Documentation and examples for Views Maintenance hooks.
 */

/**
 * Returns use cases of views displays in module.
 *
 * Should be implemented by modules which use view displays and want to report
 * about it on views usage page.
 *
 * If you want to put hook implementation into separate file (recommended)
 * your module should implement hook_views_api() with current Views API version.
 * If this condition is satisfied you can put your code to
 * MODULE.views_maintenance.inc file inside folder returned in "path" by
 * module's hook_views_api() implementation (by default it is module's folder).
 *
 * @param array $views
 *   Array of views objects for use case analysis, array keys are views names.
 *   Result will be filtered anyway (e.g. if your implementation returns use
 *   cases for disabled views), so this list can be used to reduce search area
 *   if it's possible.
 *
 * @return array
 *   Implementation should return nested array with use cases. Keys should be:
 *   - view name
 *   - display id
 *   - numerical keys for list of use cases.
 *   Each use case is an array, containing following elements:
 *   - "type": (required) translated type of use case, it will be escaped with
 *     check_plain().
 *   - "status": (required) can be 'broken', 'ok', 'maybe' or 'unused'.
 *     Implementation shouldn't return use case with 'unused' status unless it
 *     wants to provide info about disabled use case and links to enable it.
 *   - "description": (required) array of HTML items for description or single
 *     HTML string (will be printed as item list anyway).
 *   - "links": (optional) array of HTML links to print with this use case (for
 *     example, to edit or disable use case)
 */
function hook_views_maintenance_use_cases($views) {
  return array(
    'view_name' => array(
      'display_id' => array(
        array(
          'type' => t('My use case'),
          'status' => 'maybe',
          'description' => t('Not sure'),
        ),
        array(
          'type' => t('My use case'),
          'status' => 'unused',
          'description' => array(
            t('Package is enabled'),
            t('%name is disabled', array(
              '%name' => t('Some name'),
            )),
          ),
        ),
        array(
          'type' => t('My use case'),
          'status' => 'ok',
          'description' => t('%name is enabled', array(
            '%name' => t('Some name'),
          )),
          'links' => array(
            l(t('Edit my use case'), 'admin/build/my-use-case/edit'),
          ),
        ),
        array(
          'type' => t('My use case is broken'),
          'status' => 'broken',
          'description' => t('Missing required option'),
        ),
      ),
    ),
  );
}
