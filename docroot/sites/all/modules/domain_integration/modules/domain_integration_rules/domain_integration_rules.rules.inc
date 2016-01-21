<?php
/**
 * @file
 * Domain Integration Rules.
 */

/**
 * Implements hook_rules_condition_info().
 */
function domain_integration_rules_rules_condition_info() {
  $rules_conditions = array();

  $rules_conditions['list_intersect'] = array(
    'label' => t('List contains item in other list (array_intersect)'),
    'parameter' => array(
      'list_1' => array(
        'type' => 'list',
        'label' => t('List', array(), array('context' => 'data_types')),
        'restriction' => 'selector',
      ),
      'list_2' => array(
        'type' => 'list',
        'label' => t('List'),
        'description' => t('List that should contain at lease one item to check for.'),
      ),
    ),
    'group' => t('Domain Integration'),
    'base' => 'domain_integration_rules_rules_condition_list_intersect',
  );

  return $rules_conditions;
}

/**
 * Implements hook_rules_action_info().
 */
function domain_integration_rules_rules_action_info() {
  $rules_actions = array();

  $rules_actions['domain_integration_rules_rules_action_domain_node'] = array(
    'label' => t('Change node domain settings'),
    'parameter' => array(
      'node' => array(
        'type' => 'node',
        'label' => t('Node'),
        'restriction' => 'selector',
      ),
      'domain_node_settings' => array(
        'type' => 'list<text>',
        'label' => t('Domains'),
        'description' => 'New domain settings',
        'options list' => 'domain_integration_rules_rules_action_domains_options_list',
        'restriction' => 'input',
      ),
      'action' => array(
        'type' => 'text',
        'label' => t('Action'),
        'description' => t('Action to perform.'),
        'options list' => 'domain_integration_rules_rules_action_action_options_list',
        'restriction' => 'input',
      ),
    ),
    'group' => t('Domain Integration'),
    'base' => 'domain_integration_rules_rules_action_domain_node',
  );

  $rules_actions['domain_integration_rules_rules_action_domain_user'] = array(
    'label' => t('Change user domain settings'),
    'parameter' => array(
      'user' => array(
        'type' => 'user',
        'label' => t('User'),
        'restriction' => 'selector',
      ),
      'domain_user_settings' => array(
        'type' => 'list<text>',
        'label' => t('Domains'),
        'description' => 'New domain settings',
        'options list' => 'domain_integration_rules_rules_action_domains_options_list',
        'restriction' => 'input',
      ),
      'action' => array(
        'type' => 'text',
        'label' => t('Action'),
        'description' => t('Action to perform.'),
        'options list' => 'domain_integration_rules_rules_action_action_options_list',
        'restriction' => 'input',
      ),
    ),
    'group' => t('Domain Integration'),
    'base' => 'domain_integration_rules_rules_action_domain_user',
  );

  return $rules_actions;
}

/**
 * Rules callback: condition list_intersect.
 *
 * @return boolean
 *   TRUE if both lists have an intersection (array_intersect).
 *
 * @param array $list_1
 * @param array $list_2
 */
function domain_integration_rules_rules_condition_list_intersect($list_1, $list_2, $settings, $state) {
  // 2 arrays are passed in. If they have any intersection return TRUE.
  $intersect = array_intersect($list_1, $list_2);

  if (!empty($intersect)) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Options list with actions (add, remove, replace)
 *
 * @return array $options_list
 */
function domain_integration_rules_rules_action_action_options_list() {
  $options_list = array(
    'add' => t('Add'),
    'remove' => t('Remove'),
    'replace' => t('Replace'),
  );
  return $options_list;
}

/**
 * Options list with domain targets (current domain, active domains + sendtoall for nodes)
 *
 * @return array $options_list
 */
function domain_integration_rules_rules_action_domains_options_list($conf, $param) {

  $options_list = array();

  // SendToAll if we are acting on nodes.
  if ($param == 'domain_node_settings') {
    $options_list['domain_site'] = t('Send to all');
  }

  // Current domain. It's calculated upon execution.
  $options_list['current'] = t('Current');

  // Add active domains.
  foreach (domain_domains() as $domain) {
    $options_list[$domain['machine_name']] = $domain['sitename'];
  }

  return $options_list;
}

/**
 * Rules Action Callback that changes domain settings for nodes.
 *
 * Note: domain assignment is required for nodes.
 */
function domain_integration_rules_rules_action_domain_node($node, $domain_node_settings_new, $action = 'replace') {

  // Extract 'SendToAll' setting.
  if (isset($domain_node_settings_new['domain_site'])) {
    $domain_site = TRUE;
    $new_domains = $domain_node_settings_new;
    unset($new_domains['domain_site']);
  }
  else {
    $domain_site = FALSE;
    $new_domains = $domain_node_settings_new;
  }

  // Normalize passed in domain settings (merges 'current').
  $new_domains = _domain_integration_rules_rules_settings_normalize_domains($new_domains);


  // Change domain settings for this node entity.
  switch ($action) {
    case 'replace':
      $node->domains = $new_domains;
      $node->domain_site = $domain_site;


      // Set error and stop processing this node if $node->domains is empty.
      if (empty($node->domains)) {
        $vars['@nid'] = $node->nid;
        drupal_set_message(t('Replacement of domain-settings for node with nid @nid failed. You cannot replace domain settings with "Send to all" only. You must at least choose one domain.', $vars), 'error');
        watchdog('domain_integration_rules', 'Replacement of domain-settings for node with nid @nid failed. You cannot replace domain settings with "Send to all" only. You must at least choose one domain.', $vars, WATCHDOG_ERROR);
        return;
      }
      break;

    case 'add':
      // Get current node's domains and merge with submitted.
      $current_domains = $node->domains;

      // Just add the arrays with domains together.
      $combined_domains = $current_domains + $new_domains;
      $node->domains = array_unique($combined_domains);

      // 'send to all affiliates'.
      $node->domain_site = $domain_site;
      break;

    case 'remove':
      // Get current node's domains and remove submitted.
      $current_domains = $node->domains;

      // Remove submitted domains from the current domains array.
      $subtracted_domains = array_diff_assoc($current_domains, $new_domains);
      $node->domains = array_unique($subtracted_domains);

      // Remove 'send to all affiliates' if TRUE. If nothing is passed, do
      // change this value.
      if ($domain_site === TRUE) {
        $node->domain_site = FALSE;
      }


      // Set error and stop processing this node if $node->domains is empty.
      if (empty($node->domains)) {
        $vars['@nid'] = $node->nid;
        drupal_set_message(t('Removal of domain-settings for node with nid @nid failed. You cannot have a node without assigned domains.', $vars), 'error');
        watchdog('domain_integration_rules', 'Removal of domain-settings for node with nid @nid failed. You cannot have a node without assigned domains.', $vars, WATCHDOG_ERROR);
        return;
      }
      break;
  }

  // Save new settings.
  domain_node_access_records($node);
  // Explicit save node to force all node access hooks to be called.
  node_save($node);
}

/**
 * Rules Action Callback that changes domain settings for users.
 *
 * Note: domain assignment is optional for users.
 */
function domain_integration_rules_rules_action_domain_user($user, $domain_user_settings_new, $action = 'replace') {

  // Prevent action on user 0 (anonymous) user if it's passed in.
  if (!$user->uid) {
    return;
  }

  // Normalize passed in domain settings (merges 'current').
  $new_domains = _domain_integration_rules_rules_settings_normalize_domains($domain_user_settings_new);


  // Change domain settings for this user entity.
  switch ($action) {
    case 'replace':
      $edit['domain_user'] = $new_domains;
      break;

    case 'add':
      // Get current user's domains and merge with submitted.
      $current_domains = is_array($user->domain_user) ? $user->domain_user : array();

      // Just add the arrays with domains together.
      $combined_domains = $current_domains + $new_domains;
      $edit['domain_user'] = array_unique($combined_domains);
      break;

    case 'remove':
      // Get current node's domains and remove submitted.
      $current_domains = $user->domain_user;

      // Remove submitted domains from the current domains array.
      $subtracted_domains = array_diff_assoc($current_domains, $new_domains);
      $edit['domain_user'] = array_unique($subtracted_domains);
      break;

    default:
      $edit['domain_user'] = is_array($user->domain_user) ? $user->domain_user : array();
  }

  // Save new settings.
  // Function domain_user_save expects a third argument ($catalog), but it is
  // not used. Set it to a zero value here to prevent notices.
  $user->domain_user = $edit['domain_user'];
  domain_user_save($edit, $user, 0);
}

/**
 * Returns a normalized list of domain_id's
 *
 * @param array $rules_domain_settings
 *   - An array with machine_names of domains.
 *   - Also accepts machine_name 'current' that will be replaced by the current
 *   domain.
 *
 * @return array
 *   - An array with domain_id's as keys and values that can be directly passed
 *   to domain functions 'domain_user_save' and 'domain_node_access_records'.
 *   - If the key '
 */
function _domain_integration_rules_rules_settings_normalize_domains($rules_domain_settings = array()) {

  // Load Domain-ID's and merge 'current' if passed in.
  foreach ($rules_domain_settings as $domain_machinename => &$value) {
    if ($domain_machinename == 'current') {
      $current_domain = domain_get_domain();
      $value = $current_domain['domain_id'];
    }
    else {
      $value = domain_load_domain_id($domain_machinename);
    }
  }
  // Move values to keys and drop duplicates.
  $rules_domain_settings = array_combine($rules_domain_settings, $rules_domain_settings);

  return $rules_domain_settings;
}
