<?php

/**
 * @file
 * Admin page functions.
 */

/**
 * Form constructor for the migrate cron jobs form.
 *
 * @see salsify_migrate_configure_cron_validate()
 */
function salsify_migrate_cron($form, &$form_state) {
  drupal_set_title(t('Migrate cron jobs'));
  $migrations = migrate_migrations();
  foreach (variable_get('salsify_cron', array()) as $level => $jobs) {
    switch ($level) {
      case 'group':
        $options = array();
        foreach (array_keys(array_intersect_key($jobs, MigrateGroup::groups())) as $job) {
          $options[$job] = MigrateGroup::getInstance($job)->getTitle();
        }
        if (!empty($options)) {
          $form['salsify_cron']['group'] = array(
            '#type' => 'checkboxes',
            '#title' => t('Groups'),
            '#description' => t('Check cron jobs to remove.'),
            '#options' => $options,
          );
        }
        break;

      case 'migration':
        $options = drupal_map_assoc(array_keys(array_intersect_key($jobs, $migrations)));
        if (!empty($options)) {
          $form['salsify_cron']['migration'] = array(
            '#type' => 'checkboxes',
            '#title' => t('Migrations'),
            '#description' => t('Check cron jobs to remove.'),
            '#options' => $options,
          );
        }
        break;
    }
  }
  if (isset($form['salsify_cron'])) {
    $form['salsify_cron']['#tree'] = TRUE;
  }
  else {
    drupal_set_message(t('No Migrate cron jobs found. Return to the <a href="!uri">dashboard</a> to add cron jobs.', array(
      '!uri' => url('admin/content/migrate'),
    )));
  }
  return system_settings_form($form);
}

/**
 * Form validation handler for salsify_migrate_cron().
 */
function salsify_migrate_cron_validate($form, &$form_state) {
  if (isset($form_state['values']['salsify_cron'])) {
    // Get settings from salsify_cron variable.
    $cron = variable_get('salsify_cron', array());
    foreach ($form_state['values']['salsify_cron'] as $level => &$jobs) {
      $jobs = array_diff_key($cron[$level], array_filter($jobs));
    }
    $form_state['values']['salsify_cron'] = array_intersect_key($cron, array_filter($form_state['values']['salsify_cron']));
  }
  else {
    $form_state['values']['salsify_cron'] = array();
  }
}

/**
 * Submit callback for the dashboard form.
 */
function salsify_migrate_ui_migrate_submit($form, &$form_state) {
  switch ($form_state['values']['operation']) {
    case 'import_immediate':
    case 'import_background':
      if (!empty($form_state['values']['cron'])) {
        $options = array(
          $GLOBALS['user']->uid,
          !empty($form_state['values']['force']),
          !empty($form_state['values']['update']),
          isset($form_state['values']['limit']) ? $form_state['values']['limit'] : '',
        );
        $cron = variable_get('salsify_cron', array());
        if (isset($form_state['values']['migrations'])) {
          foreach (array_filter($form_state['values']['migrations']) as $machine_name) {
            $cron['migration'][$machine_name] = $options;
          }
        }
        else {
          foreach (array_filter($form_state['values']['tasks']) as $group_name) {
            $cron['group'][$group_name] = $options;
          }
        }
        variable_set('salsify_cron', $cron);
      }
      break;

    case 'deregister':
      $cron = variable_get('salsify_cron', array());
      if (isset($form_state['values']['migrations'])) {
        foreach (array_filter($form_state['values']['migrations']) as $machine_name) {
          unset($cron['migration'][$machine_name]);
          db_drop_table('salsify_variations_' . strtolower($machine_name));
        }
      }
      else {
        $groups = array_filter($form_state['values']['tasks']);
        foreach (migrate_migrations() as $machine_name => $migration) {
          if (in_array($migration->getGroup()->getName(), $groups)) {
            unset($cron['migration'][$machine_name]);
            db_drop_table('salsify_variations_' . strtolower($machine_name));
          }
        }
        foreach ($groups as $group_name) {
          unset($cron['group'][$group_name]);
        }
      }
      variable_set('salsify_cron', array_filter($cron));
      break;
  }
}

/**
 * Prevent existing products migration from losing its product ID mapping.
 */
function salsify_form_migrate_ui_edit_mappings_validate($form, &$form_state) {
  $mapping = $form_state['salsify']['mappings']['product_id'];
  $form_state['values']['field_mappings']['product_id'] = array(
    'issue_group' => $mapping->getIssueGroup() == t('DNM'),
    'mapping' => $mapping->getSourceField() ? $mapping->getSourceField() : -1,
    'default_value' => $mapping->getDefaultValue(),
    'source_migration' => $mapping->getSourceMigration(),
  );
}
