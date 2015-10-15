<?php
/**
 * @file
 * Xero make include file.
 */


/**
 * Return tracking category and option.
 *
 * @param $none
 *   Default to 'all'. Unused.
 * @param $options
 *   An array of tracking categories keyed by the category name with the value
 *   as the Tracking option.
 * @return array
 *   A formatted array to apply to the Tracking element on a line item.
 */
function xero_make_tracking($none, $options) {
  $ret = array(
    'TrackingCategory' => array(),
  );

  if ($options) {
    foreach ($options as $category => $option) {
      $ret['TrackingCategory'][] = array(
        'Name' => $category,
        'Option' => $option
      );
    }
  }

  return $ret;
}
