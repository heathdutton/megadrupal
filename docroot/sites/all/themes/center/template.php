<?php
/**
 * @file
 * Include template function files.
 *
 * Template functions are grouped into files loosely based on hooks, in the
 * hopes that functions addresssing similar concerns are easy to track. For
 * instance, template functions dealing with fields are likely grouped into
 * field.inc. The reason this isn't broken down further is because there isn't
 * yet an incentive to break functions out into even more files.
 */

/**
 * Creates a layout class based on the availability of regions in the given array.
 *  This function is useful if you need to adjust your layout CSS based on the
 *  presence or absence of certain regions.  Works well with pages, Panels
 *  and Display Suite layouts.
 *
 * @param array $regions
 *   An array of region machine names to check if they are empty or not.
 *
 * @return string
 *  A string in the form of l--0-1... with integers representing each region in
 *   the $regions argument.  Empty regions appear as 0. Populated regions appear
 *   as 1.
 */

function center_create_region_collection_classes($regions) {
  $class = 'l-';
  // Construct a class based on if regions are empty or not.
  foreach ($regions as $key => $region) {
    $class .= empty($region) ? '-0' : '-1';
  }
  return $class;
}

/**
 * Allows a set of preprocess functions to be declared.
 *
 * @param array $vars
 *   Template variables.
 * @param string $base
 *   The base theme hook implementation.
 * @param array $preprocesses OPTIONAL
 *   An of strings that define which preprocess functions to run.  If this is
 *    omitted, $base will be run as the function.
 */
function center_preprocessors(&$vars, $base, $preprocesses = NULL) {
  if ($preprocesses != NULL && !is_array($preprocesses)) {
    return;
  }

  // If $preprocesses is not set, try running $base as a function.
  if ($preprocesses === NULL && function_exists($base)) {
    $base($vars);
    return;
  }

  // Run each existing function.
  foreach ($preprocesses as $preprocess) {
    $function = $base . '__' . $preprocess;
    if (function_exists($function)) {
      $function($vars);
    }
  }
}

/**
 * Removes the given class attribute from a classes array.
 *
 * @param array $array
 *   The array from which the class should be removed.
 * @param string $class
 *   The class to remove.
 *
 * @return
 *  The filtered array.
 */
function center_remove_class($array, $class) {
  $index = array_search($class,$array);
  if ($index !== FALSE) { unset($array[$index]); }
  return $array;
}

/**
 * Remove multiple classes from a classes array.
 *
 * @param array $array
 *   The array from which the classes should be removed.
 * @param array $classes
 *   An array of class names to remove.
 *
 * @return
 *  The filtered array.
 */
function center_remove_classes($array, $classes) {
  foreach ($classes as $class) {
    $array = center_remove_class($array, $class);
  }
  return $array;
}

include 'inc/block.inc';
include 'inc/breadcrumb.inc';
include 'inc/field.inc';
include 'inc/form.inc';
include 'inc/html.inc';
include 'inc/link.inc';
include 'inc/menu.inc';
include 'inc/node.inc';
include 'inc/page.inc';
include 'inc/pager.inc';
include 'inc/taxonomy.inc';
