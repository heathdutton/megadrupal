<?php

/**
 * Migrates rules from the old format to the new.
 */
function _ms_membership_migrate_rules($skus) {
  // Set some variables.
  $rules_strings_map = array(
    'ms_membership_event_membership_cancel' => 'ms_products_event_purchase_cancel',
    'ms_membership_event_membership_signup' => 'ms_products_event_purchase_signup',
    'ms_membership_event_membership_modification' => 'ms_products_event_purchase_modification',
    'ms_membership_event_membership_renewal' => 'ms_products_event_purchase_renewal',
    'ms_membership_event_membership_payment' => 'ms_products_event_purchase_payment',
    'ms_membership_event_membership_expiring_soon' => 'ms_products_event_purchase_expiring_soon',
    'ms_membership_event_membership_expiring' => 'ms_products_event_purchase_expiring',
    'ms_membership_action_change_membership_status' => 'ms_products_action_change_purchase_status',
    'ms_membership_action_change_membership_expiration' => 'ms_products_action_change_purchase_expiration',
    'ms_membership_action_add_membership' => 'ms_products_action_add_purchase',
    'ms_membership_condition_membership_plan_is' => 'ms_products_condition_product_plan_is',
    'ms_membership_condition_user_has_membership' => 'ms_products_condition_user_has_purchase',
    'ms_membership_plan' => 'ms_products_plan',
    'm_plan' => 'ms_products_plan',
    'mpid' => 'sku',
    'membership' => 'purchase',
    'ms_membership' => 'ms_products_purchase',
  );

  $rules = rules_config_load_multiple();

  foreach ($rules as $rule) {
    $export_string = $rule->export();
    // Only do this for rules using ms_membership.
    if (strstr($export_string, 'ms_membership')) {
      // Replace the strings.
      foreach ($rules_strings_map as $old => $new) {
        $export_string = str_replace($old, $new, $export_string);
      }
      // Replace the tokens.
      $export_string = _ms_membership_migrate_tokens($export_string);
      // Replace the sku.
      foreach ($skus as $mpid => $sku) {
        $export_string = str_replace('"sku" : "' . $mpid . '"', '"sku" : "' . $sku . '"', $export_string);
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
      if (!empty($export_object[$rule->name])) {
        $rule->import($export_object[$rule->name]);
        $rule->save();
      } else {
        drupal_set_message(t("Unable to reimport rule @name. It may need to be manually recreated.", array('@name' => $rule->name)));
      }
    }
  }
}

/**
 * Migrates views from the old format to the new.
 */
function _ms_membership_migrate_views() {
  // First change the base tables.
  db_update('views_view')
    ->fields(array(
      'base_table' => 'ms_products_plans',
    ))
    ->condition('base_table', 'ms_membership_plans')
    ->execute();
  db_update('views_view')
    ->fields(array(
      'base_table' => 'ms_products_purchases',
    ))
    ->condition('base_table', 'ms_memberships')
    ->execute();

  // Then change the views themselves.
  $result = db_select('views_display', 'd')
    ->fields('d')
    ->execute();
  foreach ($result as $view) {
    $view->display_options = unserialize($view->display_options);

    $base_tables = array();

    // Migrate Views Fields.
    if (!empty($view->display_options['fields']) AND is_array($view->display_options['fields'])) {
      foreach ($view->display_options['fields'] as &$field) {
        if (in_array($field['table'], array('ms_membership_plans', 'ms_memberships'))) {
          $base_tables[$field['table']] = TRUE;
          _ms_membership_migrate_views_field($field);
        }
      }
    }

    // Migrate Views Filters.
    if (!empty($view->display_options['filters']) AND is_array($view->display_options['filters'])) {
      foreach ($view->display_options['filters'] as &$filter) {
        if (in_array($filter['table'], array('ms_membership_plans', 'ms_memberships'))) {
          $base_tables[$filter['table']] = TRUE;
          _ms_membership_migrate_views_field($filter);
        }
      }
    }

    // Migrate Views Sorts.
    if (!empty($view->display_options['sorts']) AND is_array($view->display_options['sorts'])) {
      foreach ($view->display_options['sorts'] as &$sort) {
        if (in_array($sort['table'], array('ms_membership_plans', 'ms_memberships'))) {
          $base_tables[$sort['table']] = TRUE;
          _ms_membership_migrate_views_field($sort);
        }
      }
    }

    // Add a bundle filter set to 'ms_membership'
    if (!empty($base_tables['ms_membership_plans'])) {
      $view->display_options['filters']['bundle'] = array(
        'id' => 'plan_bundle',
        'table' => 'ms_products_plans',
        'field' => 'bundle',
        'relationship' => 'none',
        'group_type' => 'group',
        'ui_name' => '',
        'operator' => 'in',
        'value' => array(
          'ms_membership' => 'ms_membership',
        ),
        'group' => 1,
        'exposed' => FALSE,
        'expose' => array(
          'operator_id' => '',
          'label' => '',
          'use_operator' => 0,
          'operator' => '',
          'identifier' => '',
          'required' => 0,
          'remember' => 0,
          'multiple' => 0,
          'reduce' => FALSE,
        ),
      );
    }
    if (!empty($base_tables['ms_memberships'])) {
      $view->display_options['filters']['bundle'] = array(
        'id' => 'purchase_bundle',
        'table' => 'ms_products_purchases',
        'field' => 'bundle',
        'relationship' => 'none',
        'group_type' => 'group',
        'ui_name' => '',
        'operator' => 'in',
        'value' => array(
          'ms_membership' => 'ms_membership',
        ),
        'group' => 1,
        'exposed' => FALSE,
        'expose' => array(
          'operator_id' => '',
          'label' => '',
          'use_operator' => 0,
          'operator' => '',
          'identifier' => '',
          'required' => 0,
          'remember' => 0,
          'multiple' => 0,
          'reduce' => FALSE,
        ),
      );
    }

    // Update the record.
    db_update('views_display')
      ->fields(array(
        'display_options' => serialize($view->display_options),
      ))
      ->condition('vid', $view->vid)
      ->condition('id', $view->id)
      ->execute();
  }
}

/**
 * Returns the new field name.
 */
function _ms_membership_migrate_views_field(&$field) {
  // Change the table name.
  switch ($field['table']) {
    case 'ms_membership_plans':
      $field['table'] = 'ms_products_plans';
      break;

    case 'ms_memberships':
      $field['table'] = 'ms_products_purchases';
      break;
  }

  // Change the field names when applicable.
  switch ($field['field']) {
    case 'mid':
      $field['field'] = 'id';
      break;

    case 'mpid':
      $field['field'] = 'sku';
      if (!empty($field['value']['value'])) {
        $field['value']['value'] = 'ms_membership-' . $field['value']['value'];
      }
      break;
  }
}

/**
 * Migrates a permission to the new ms_products ones.
 */
function _ms_membership_migrate_permission($old, $new) {
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
    ->condition('module', 'ms_membership')
    ->execute();
}

/**
 * Change tokens to use the new ms_products tokens.
 */
function _ms_membership_migrate_tokens($string) {
  $string = str_replace('[ms_membership:', '[ms_products_purchase:', $string);
  $string = str_replace('[ms_membership_plan:', '[ms_products_plan:', $string);
  $string = str_replace(':planName]', ':name]', $string);
  $string = str_replace(':planDescription]', ':description]', $string);
  $string = str_replace(':recurringSchedule]', ':payment-string]', $string);
  $string = str_replace('[ms_products_plan:changedRoles]', '', $string);
  $string = str_replace('[ms_products_purchase:currentPayments]', '[ms_products_purchase:current-payments]', $string);
  $string = str_replace('[ms_products_purchase:expirationDate]', '[ms_products_purchase:expiration:short]', $string);
  $string = str_replace('[ms_products_purchase:maxPayments]', '[ms_products_purchase:max-payments]', $string);
  $string = str_replace('[ms_products_purchase:purchaseStatus]', '[ms_products_purchase:status]', $string);
  $string = str_replace('[ms_products_purchase:startDate]', '[ms_products_purchase:start-date:short]', $string);
  $string = str_replace('[ms_products_purchase:renewLink]', '[ms_products_purchase:renew-uri]', $string);

  return $string;
}

/**
 * Inserts a permission.
 */
function _ms_membership_migrate_insert_permission($rid, $permission) {
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
 * Change the status to the new ms_products statuses.
 */
function _ms_membership_migrate_status($status) {
  switch ($status) {
    case 0:
      return 'expired';
    case 1:
      return 'active';
    case 2:
      return 'canceled';
    case 3:
      return 'completed';
    case 4:
      return 'expiring';
  }

  return 'expired';
}
