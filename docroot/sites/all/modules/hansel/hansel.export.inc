<?php

/**
 * Export Hansel configuration.
 * 
 * @return array
 */
function hansel_export_config() {
  $config = hansel_get_config();
  $mapping = array();
  
  $export = array(
    'version' => $config['version'],
    'start_rid' => 0,
    'rules' => array(),
    'nodetypes' => $config['nodetypes'],
  );
  
  // Copy rules and renumber them.
  $id = 0;
  foreach ($config['rules'] as $rule) {
    ++$id;
    $mapping[$rule->rid] = $id;
    $rule->rid = $id;
    $export['rules'][$id] = clone $rule;
  }
  
  // Map start rule.
  $export['start_rid'] = $mapping[$config['start_rid']];
  
  // Map parents and destinations.
  foreach ($export['rules'] as $id => $rule) {
    if ($rule->pid) {
      $export['rules'][$id]->pid = $mapping[$rule->pid];
    }
    if ($rule->action == 'goto') {
      $export['rules'][$id]->destination = $mapping[$rule->destination];
    }
  }
  
  return $export;
}

/**
 * Import hansel configuration.
 * 
 * @param array $config
 */
function hansel_import_config($config) {
  // Delete old configuration.
  db_query("DELETE FROM {hansel_rule}");
  db_query("DELETE FROM {hansel_rule_action_goto}");
  db_query("DELETE FROM {hansel_rule_action_leave}");
  db_query("DELETE FROM {hansel_rule_action_switch}");

  // Mapping with old id's as keys and new id's as values.
  $mapping = array();

  // Store all rules without actions and parent id's first, we will update
  // those later cause we don't have a complete mapping here.
  foreach ($config['rules'] as $rule) {
    if (is_array($rule)) {
      // The input can be an array because we use features_var_export().
      $rule = (object) $rule;
    }
    $r = new stdClass();
    $r->name = $rule->name;
    $r->crumb_action = $rule->crumb_action;
    $r->crumb_action_arguments = serialize($rule->crumb_action_arguments);
    drupal_write_record('hansel_rule', $r);
    $mapping[$rule->rid] = $r->rid;
  }

  // Store all actions and update parent id's.
  foreach ($config['rules'] as $rule) {
    if (is_array($rule)) {
      // The input can be an array because we use features_var_export().
      $rule = (object) $rule;
    }
    if (isset($mapping[$rule->pid])) {
      db_query("UPDATE {hansel_rule} SET pid = :pid WHERE rid = :rid", array(':pid' => $mapping[$rule->pid], ':rid' => $mapping[$rule->rid]));
    }
    $action = new stdClass();
    $action->rid = $mapping[$rule->rid];
    switch ($rule->action) {
      case 'goto':
        if (isset($mapping[$rule->destination])) {
          $action->destination = $mapping[$rule->destination];
          drupal_write_record('hansel_rule_action_goto', $action);
        }
        break;
      case 'switch':
        $action->handler = $rule->handler;
        $action->arguments = serialize($rule->arguments);
        drupal_write_record('hansel_rule_action_switch', $action);
        break;
      case 'leave':
        $action->restore_original = $rule->restore ? 1 : 0;
        drupal_write_record('hansel_rule_action_leave', $action);
        break;
    }
  }
  
  $nodetypes = empty($config['nodetypes']) ? array() : $config['nodetypes'];
  variable_set('hansel_nodetypes', $nodetypes);

  cache_clear_all('hansel_config', 'cache');
}

/**
 * Export Hansel settings.
 * 
 * @return array
 */
function hansel_export_settings() {
  $settings = array(
    'variables' => array(),
  );
  $variables = db_select('variable', 'v')
    ->fields('v', array('name'))
    ->condition('v.name', 'hansel_%', 'LIKE')
    ->orderBy('v.name', 'ASC')
    ->execute()
    ->fetchCol();
  foreach ($variables as $name) {
    $settings['variables'][$name] = variable_get($name, NULL);
  }
  return $settings;
}

/**
 * Import Hansel settings.
 * 
 * @param array $settings
 */
function hansel_import_settings($settings) {
  foreach ($settings['variables'] as $name => $value) {
    variable_set($name, $value);
  }
}
