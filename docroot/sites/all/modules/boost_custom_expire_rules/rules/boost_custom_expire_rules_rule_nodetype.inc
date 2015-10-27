<?php
/**
 * @file
 * Custom rule for expiring nodes by node type
 */

/**
 * Show information about the rule in admin settings form
 */
function boost_custom_expire_rules_rule_nodetype_info() {
  return array(
    'name' => 'nodetype',
    'title' => 'Node type based cache expire rule',
    'description' => 'Set custom expiry of boost cache based on type of the node. The rule supports setting different expire times for different content types.',
    'examples' => array(
      1 => array(
        'rule' => '{"name": "nodetype", "expire": 3600, "type": "story"}',
        'description' => 'The rule sets an expiry of 1 hour for story nodes',
      ),
      2 => array(
        'rule' => '{"name": "nodetype", "expire": 86400, "type": "page"}',
        'description' => 'The rule sets an expiry of 86400 seconds (one day) for page nodes',
      ),
    ),
  );
}

/**
 * Get the expire value for the given rule based on the configuration set for the rule
 */
function boost_custom_expire_rules_rule_nodetype_get_expire_value($rule, $path) {
  // Apply the age based rule only for node views
  if (arg(0) == 'node' && is_numeric(arg(1)) && arg(2) == '') {
    // Node load is cached so OK to load here as this is a
    // node view page anyway
    $node = node_load(arg(1));
    // If the node is of the given type set in the rule
    if ($node->type == $rule->type) {
      return ($rule->expire);
    }
  }
}

/**
 * Validate the configuration set for the rule
 */
function boost_custom_expire_rules_rule_nodetype_validate_configuration($rule) {
  $message = NULL;
  if (!isset($rule->expire)) {
    $message = 'The "expire" property is not set for the rule. ';
  }
  else {
    if (!is_int($rule->expire)) {
      $message .= 'The property "expire" must be set as an integer for the rule.';
    }
  }
  if (!isset($rule->type)) {
    $message .= 'The "type" property is not set for the rule.';
  }
  else {
    if (!in_array($rule->type, array_keys(node_type_get_names()))) {
      $message .= 'The "type" property set is not a valid machine name for any of the known node types.';
    }
  }
  if (isset($message)) {
    return $message;
  }
}

