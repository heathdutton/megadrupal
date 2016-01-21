<?php

/**
 * @file
 * Module API documentation.
 */

/**
 * @defgroup dsb_portal_active_filter_names_api dsb Portal Active Filter Names
 * @{
 * dsb Portal Active Filter Names changes the machine-readable values of enabled
 * filters on the dsb Portal search form to human-readable values. It uses
 * multiple strategies to accomplish this, and resulting human readable values
 * are always stored in cache for better performance. It is possible for other
 * modules to alter these human-readable names using
 * hook_dsb_portal_active_filter_names_alter().
 *
 * @}
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Implements hook_dsb_portal_active_filter_names_alter().
 *
 * It is possible to alter the computed human-readable value of an active
 * filter. The altered value will be cached, and alter hooks will not be called
 * for subsequent computing for the same filter name.
 *
 * @param string &$filter_name
 *    The computed filter name, or the original, machine-readable filter name
 *    if none could be found.
 * @param array $context
 *    An array with the following keys:
 *    - filter_group: The group the filter belongs to, like educaSchoolSubjects.
 *    - filter_value: The machine-readable name of the filter.
 *
 * @ingroup dsb_portal_active_filter_names_api
 */
function hook_dsb_portal_active_filter_names_alter(&$filter_name, $context) {
  $filter_name = "Jar Jar Binksin'";
}

/**
 * @} End of "addtogroup hooks".
 */
