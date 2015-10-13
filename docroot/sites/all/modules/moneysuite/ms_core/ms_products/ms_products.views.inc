<?php

/**
 * Implements hook_views_data_alter().
 */
function ms_products_views_data_alter(&$data) {
  $bundles = ms_products_get_bundles();
  foreach ($bundles as $bundle => $bundle_info) {
    $data['users'][$bundle . '_purchases'] = $data['users']['ms_products_purchases'];
    $data['users'][$bundle . '_purchases']['title'] = $bundle_info['purchase_name'];
    $data['users'][$bundle . '_purchases']['help'] = t("Associated @purchase_name via the user's uid.", array('@purchase_name' => $bundle_info['purchase_name']));
    $data['users'][$bundle . '_purchases']['relationship']['label'] = $bundle_info['purchase_name'];
    $data['users'][$bundle . '_purchases']['relationship']['extra'] = array(
      array('field' => 'bundle', 'value' => $bundle),
    );
  };
}
