<?php
/**
 * @file
 * API documentation for hooks exposed by the checkout multilane module.
 */

/**
 * Provide information about available checkout lanes.
 */
function hook_commerce_checkout_multilane_lane_info() {
  return array(
    'express' => array(
      'title' => 'Express checkout',
    ),
  );
}

/**
 * Alters information about available checkout lanes.
 *
 * @see hook_commerce_checkout_multilane_lane_info()
 *
 * @param array $lanes
 *   An associative array of lane records keyed by lane id.
 */
function hook_commerce_checkout_multilane_lane_info_alter(&$lanes) {
  // No example.
}

/**
 * Alters the lane id active on the current checkout page.
 *
 * @param string|NULL $lane_id
 *   The lane id active on the current checkout page.
 */
function hook_commerce_checkout_multilane_active_lane_alter(&$lane_id) {
  // No example.
}

/**
 * Alter checkout panes on a given lane.
 *
 * @param array $checkout_panes
 *   The array of checkout panes.
 * @param string|NULL $lane_id
 *   The lane id active on the current checkout page.
 */
function hook_commerce_checkout_multilane_pane_info_alter(&$checkout_panes, $lane_id) {
  if ($lane_id === 'express') {
    $panes['checkout_review']['page'] = 'disabled';
    $panes['commerce_payment']['page'] = 'checkout';
  }
}

/**
 * Alter checkout panes on a given lane.
 *
 * @see hook_commerce_checkout_multilane_pane_info_alter()
 *
 * @param array $checkout_panes
 *   The array of checkout pane.
 * @param string|NULL $lane_id
 *   The lane id active on the current checkout page.
 */
function hook_commerce_checkout_multilane_pane_info_LANE_ID_alter(&$checkout_panes, $lane_id) {
  // No example.
}
