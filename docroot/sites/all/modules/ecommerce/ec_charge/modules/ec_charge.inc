<?php
/**
 * @file
 * Provide standard methods for charges.
 */

/**
 * Implements hook_ec_charge_filter_info().
 */
function ec_charge_ec_charge_filter_info() {
  return array(
    'or' => array(
      'name' => t('Or'),
      'description' => t('Allow filters to be or\'ed together with other filters'),
      'module' => 'ec_charge_or',
      'file' => 'ec_charge.inc',
      'core' => TRUE,
      'no form' => TRUE,
      'parent' => TRUE,
      'weight' => -1,
    ),
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_charge_or_filter_process($type, $settings, $object, $charges, $chg) {
  _ec_charge_or_filter_children($settings['id'], TRUE);
  $child_filters = array_filter($chg->filters, '_ec_charge_or_filter_children');

  if (empty($child_filters)) {
    return TRUE;
  }
  else {
    foreach ($child_filters as $filter) {
      $ret = ec_charge_invoke_callback('filter', $filter['component'], 'process', $type, $filter, $object, $charges, $chg);

      if ($filter['reverse']) {
        $ret = !$ret;
      }

      if ($ret) {
        return TRUE;
      }
    }
    return FALSE;
  }
}

function _ec_charge_or_filter_children($a, $init = FALSE) {
  $parent = & drupal_static(__FUNCTION__ . '_parent', 0);

  if ($init) {
    $parent = $a;
    return;
  }

  return $a['parent'] == $parent;
}
