<?php

/**
 * Migrates rules from the old format to the new.
 */
function _ms_paypublish_migrate_rules($skus) {
  // Set some variables.
  $rules_strings_map = array(
    'ms_paypublish_condition_pp_node_status' => 'ms_products_condition_purchase_status',
    'ms_paypublish_action_change_pp_node_status' => 'ms_products_action_change_purchase_status',
    'ms_paypublish_condition_pp_plan_is' => 'ms_products_condition_product_plan_is',
    'ms_paypublish_action_change_pp_node_expiration' => 'ms_products_action_change_purchase_expiration',
    'pid' => 'sku',
    'pp_node' => 'purchase',
    'ms_paypublish_plan' => 'ms_products_plan',
    'ms_paypublish_node' => 'ms_products_purchase',
    'ms-paypublish-node:nid' => 'ms-paypublish-node:nid', // FIXME - Add support for updating rules with data selectors like this...
  );

  $rules = rules_config_load_multiple(FALSE);

  foreach ($rules as $rule) {
    $export_string = $rule->export();
    // Only do this for rules using ms_paypublish.
    if (strstr($export_string, 'ms_paypublish')) {
      // Replace the strings.
      foreach ($rules_strings_map as $old => $new) {
        $export_string = str_replace($old, $new, $export_string);
      }
      // Replace the tokens.
      $export_string = _ms_paypublish_migrate_tokens($export_string);
      // Replace the sku.
      foreach ($skus as $pid => $sku) {
        $export_string = str_replace('"sku" : "' . $pid . '"', '"sku" : "' . $sku . '"', $export_string);
      }

      // Delete the old actions and conditions.
      foreach ($rule->conditions() as $condition) {
        $condition->delete();
      }
      foreach ($rule->actions() as $action) {
        $action->delete();
      }

      // Reimport the rule.
      $export_object = drupal_json_decode($export_string);
      $rule->import($export_object[$rule->name]);
      $rule->save();
    }
  }
}

/**
 * Migrates a permission to the new ms_products ones.
 */
function _ms_paypublish_migrate_permission($old, $new) {
  // If the permission already exists, delete it.
  db_delete('role_permission')
    ->condition('permission', $new)
    ->condition('module', 'ms_products')
    ->execute();
  db_update('role_permission')
    ->fields(array(
      'permission' => $new,
      'module' => 'ms_products'
    ))
    ->condition('permission', $old)
    ->condition('module', 'ms_paypublish')
    ->execute();
}

/**
 * Inserts a permission.
 */
function _ms_paypublish_migrate_insert_permission($rid, $permission) {
  db_merge('role_permission')
    ->key(array(
      'rid' => $rid,
      'permission' => $permission,
    ))
    ->fields(array(
      'module' => 'ms_products',
    ))
    ->execute();
}

/**
 * Change tokens to use the new ms_products tokens.
 */
function _ms_paypublish_migrate_tokens($string) {
  $string = str_replace('[ms_paypublish_node:', '[ms_products_purchase:', $string);
  $string = str_replace('[ms_paypublish_plan:', '[ms_products_plan:', $string);
  $string = str_replace(':nodeStatus]', ':status]', $string);
  $string = str_replace(':expirationDate]', ':expiration:short]', $string);
  $string = str_replace(':planAmount]', ':payment-string]', $string);
  $string = str_replace(':publishUrl]', ':ms-paypublish-publish-uri]', $string);
  $string = str_replace(':denialNotes]', ':ms-paypublish-denial-notes]', $string);

  $string = str_replace(':planUnit]', ':main-unit]', $string);
  $string = str_replace(':planLength]', ':main-length]', $string);

  $string = str_replace(':planName]', ':name]', $string);

  $string = str_replace('[ms_products_purchase:startDate]', '[ms_products_purchase:start-date:short]', $string);
  $string = str_replace('[ms_products_purchase:renewLink]', '[ms_products_purchase:renew-uri]', $string);

  $string = str_replace('[node:', '[ms_products_purchase:ms-paypublish-nid:', $string);


  return $string;
}

/**
 * Change the status to the new ms_products statuses.
 */
function _ms_paypublish_migrate_status($status) {
  switch ($status) {
    case 0:
    case 3:
    case 8:
      return 'expired';
    case 1:
      return 'active';
    case 2:
    case 5:
      return 'approval_queue';
    case 4:
      return 'completed';
    case 7:
      return 'expiring';
    case 6:
      return 'cancelled';
    case 9:
      return 'approval_denied';
  }
}
