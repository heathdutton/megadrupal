<?php

/**
 * @file
 * Stamps.com shared functions.
 *
 * These are functions that are called from more than
 * one place.  They're placed here to keep them out of the
 * .module file, which is parsed on every single page load.
 */

/**
 * Select the best container for a package.
 *
 * This considers the containers in a mode and their associated
 * capacities and priorities.  It selects the best container.
 * This is the one with the highest priority that has enough capacity
 * to hold the weight of the package.  If there is a tie, it
 * takes the one with the lowest capacity.
 *
 * If nothing is valid, take the default container for the mode.
 *
 * @param object $mode
 *   The shipping mode with the packages loaded.
 * @param object $package
 *   The package object.
 *
 * @return object
 *   The best package.
 */
function uc_stamps_best_container($mode, $package) {
  // Convert everything to ounces.
  $package_weight = $package->weight *
    uc_weight_conversion($package->weight_units, 'oz');
  $best = NULL;
  foreach ($mode->containers as $container) {
    $container_capacity = uc_weight_conversion($container->capacity_units, 'oz') *
      $container->capacity;
    if ($container_capacity >= $package_weight) {
      if (empty($best) || $container->priority > $best->priority) {
        $best = $container;
      }
      elseif ($container->priority == $best->priority &&
              $container->capacity < $best->capacity) {
        $best = $container;
      }
    }
  }

  // Nothing matched.  Take the default.
  if (empty($best)) {
    $best = uc_stamps_container_load($mode->cid);
  }

  return $best;
}

/**
 * Return shipments with any of the given statuses.
 *
 * This searches the list of packages and loads those containing
 * packages with the given statuses.
 * the given status.  These are likely to be 'queued' or 'exported',
 * but could be a list of possible error statuses.  The statuses
 * are stored in the package's tracking_number field, until
 * there is an actual tracking number.
 *
 * @param array $statuses
 *   An array of strings, the statuses to search for.
 *
 * @return array
 *   An array of shipments containing packages with the given
 *   statuses.
 */
function uc_stamps_load_shipments_by_status($statuses) {
  if (!is_array($statuses)) {
    $statuses = array($statuses);
  }
  // Load the matching packages.
  $packages_result  = db_select('uc_packages', 'p')
     ->fields('p')
     ->condition('p.tracking_number', $statuses, 'IN')
     ->execute();
  $packages = $packages_result->fetchAllAssoc('package_id');

  // Load shipments for the packages.
  $shipments = array();
  foreach ($packages as $package) {
    if (!isset($shipments[$package->sid])) {
      $shipments[$package->sid] = uc_shipping_shipment_load($package->sid);
    }
  }
  // Remove non-stamps shipments.
  foreach ($shipments as $sid => $shipment) {
    if ($shipment->shipping_method != 'stamps') {
      unset($shipments[$sid]);
    }
  }

  return $shipments;
}

/**
 * Determine if a shipment needs customs forms.
 *
 * Customs forms are required for non-US destinations, plus armed forces
 * (APO/FPO) destinations, American Samoa, Guam, Federated States of
 * Micronesia, Northern Mariana Islands, and Marshall Islands.
 *
 * @param object $shipment
 *   The shipment object to check.
 *
 * @return boolean
 *   TRUE if the destination address is a location for which the USPS requires
 *   customs forms.  FALSE if not.
 */
function uc_stamps_shipment_needs_customs($shipment) {
  $country_code = $shipment->d_country;
  if (!uc_stamps_is_domestic($country_code)) {
    return TRUE;
  }

  $zone_code = $shipment->d_zone;
  if (in_array($zone_code, array(3, 6, 7, 8, 9, 10, 11, 17, 20, 30, 46, 50))) {
    return TRUE;
  }

  return FALSE;
}
