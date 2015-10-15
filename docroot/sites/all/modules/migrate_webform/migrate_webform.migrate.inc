<?php

/**
 * @file
 * this file is automatically loaded when hook_migrate_api is invoked by the
 * Migrate module.
 */

/*
 * Implement hook_migrate_api().
 */
function migrate_webform_migrate_api() {
  $configured = variable_get('migrate_webform_source_database', FALSE);

  /**
   * Pre-set common arguments for creating classes.
   */
  $common_arguments = array(
    'source_connection' => variable_get('migrate_webform_source_database', 'default'),
    'source_version'=> substr(variable_get('migrate_webform_source_database_api_version', 'd6'), 1, 1),
    'group_name' => 'webform',
    'format_mappings' => array(
      '1' => 'filtered_html',
    ),
    'node_migrations' => explode(' ', variable_get('migrate_webform_node_migration_class', "WebformNode")),
  );

  /**
   * Node migration
   */
  if (variable_get('migrate_webform_node_map_ok', TRUE)) {
    $roles = migrate_webform_get_roles();
    $node_arguments = array(
      array(
        'class_name' => 'WebformNode',
        'description' => t('Migration of webform nodes from Drupal 6'),
        'machine_name' => 'WebformNode',
        'source_type' => 'webform',
        'destination_type' => 'webform',
      ),
    );
    $common_node_arguments = $common_arguments;
    if (variable_get('migrate_webform_user_map_ok', FALSE)) {
      $user_migration = variable_get('migrate_webform_user_migration_class', "");
      $common_node_arguments['user_migration'] = array($user_migration);
      $common_node_arguments['dependencies'] = array($user_migration);
    }
    $common_node_arguments['roles'] = $roles;
  }

  /**
   * Webforms
   */
  $settings_arguments = $common_arguments + array(
      'class_name' => 'WebformSettings',
      'machine_name' => 'WebformSettings',
      'description' => t('Migration of settings for webforms from Drupal 6'),
  );
  $components_arguments = $common_arguments + array(
      'class_name' => 'WebformComponents',
      'machine_name' => 'WebformComponents',
      'description' => t('Migration of components for webforms from Drupal 6'),
      'dependencies' => array('WebformSettings'),
  );
  $emails_arguments = $common_arguments + array(
      'class_name' => 'WebformEmails',
      'machine_name' => 'WebformEmails',
      'description' => t('Migration of settings for webforms from Drupal 6'),
      'dependencies' => array('WebformComponents'),
  );
  $submissions_arguments = $common_arguments + array(
      'class_name' => 'WebformSubmissions',
      'machine_name' => 'WebformSubmissions',
      'description' => t('Migration of submissions for webforms from Drupal 6'),
      'dependencies' => array('WebformEmails'),
  );
  $submitted_data_arguments = $common_arguments + array(
      'class_name' => 'WebformSubmittedData',
      'machine_name' => 'WebformSubmittedData',
      'description' => t('Migration of submitted data for webforms from Drupal 6'),
      'dependencies' => array('WebformSubmissions'),
  );
  $last_download_arguments = $common_arguments + array(
      'class_name' => 'WebformLastDownload',
      'machine_name' => 'WebformLastDownload',
      'description' => t('Migration of last download of submission logs by webform administrators from Drupal 6'),
      'dependencies' => array('WebformSubmittedData'),
  );

  /**
   * Support for webform_validate module
   */
  if (module_exists('webform_validation')) {
    $validation_rule_arguments = $common_arguments + array(
        'class_name' => 'WebformValidationRule',
        'machine_name' => 'WebformValidationRule',
        'description' => t('Migration of validation rules from Drupal 6'),
        'dependencies' => array('WebformLastDownload'),
    );
    $validation_rule_components_arguments = $common_arguments + array(
        'class_name' => 'WebformValidationRuleComponents',
        'machine_name' => 'WebformValidationRuleComponents',
        'description' => t('Migration of validation rules components from Drupal 6'),
        'dependencies' => array('WebformValidationRule'),
    );
  }

  /**
   * Class registration!
   */
  $api = array(
    'api' => 2,
  );

  if ($configured != FALSE) {
    $api['groups']['webform']['title'] = t('Webform Migrations');


    if (variable_get('migrate_webform_node_map_ok', TRUE)) {
      // Register all the node migrations
      foreach ($node_arguments as $arguments) {
        $arguments = array_merge_recursive($arguments, $common_node_arguments);
        $api['migrations'][$arguments['machine_name']] = $arguments;
      }
    }

    $api['migrations']['WebformSettings'] = $settings_arguments;
    $api['migrations']['WebformComponents'] = $components_arguments;
    $api['migrations']['WebformEmails'] = $emails_arguments;
    $api['migrations']['WebformSubmissions'] = $submissions_arguments;
    $api['migrations']['WebformSubmittedData'] = $submitted_data_arguments;
    $api['migrations']['WebformLastDownload'] = $last_download_arguments;
  
    if (module_exists('webform_validation')) {
      $api['migrations']['WebformValidationRule'] = $validation_rule_arguments;
      $api['migrations']['WebformValidationRuleComponents'] = $validation_rule_components_arguments;
    }
  }

  return $api;
}

