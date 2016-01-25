<?php

/**
 * @file
 * Hooks provided by the MaPS Suite module.
 *
 * All hooks belongs to the group "maps_suite" and may indeed be
 * defined in a separate file mymodule.maps_suite.inc.
 */

/**
 * Defines the information displayed in the MaPS Suite overview.
 *
 * The MaPS Suite overview is a basic table with 2 columns, where
 * the first one is a kind of heading and the second one the related
 * content or value(s).
 *
 * The information are stored in an associative array, whose keys
 * are the first column content and values the second column content.
 */
function hook_maps_suite_admin_overview() {
  return array(
    t('Title of my piece of information') => t('Related value'),
  );
}
