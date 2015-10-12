<?php
/**
 * @file
 * Custom rule for expiring nodes by age
 */

/**
 * Show information about the rule in admin settings form
 */
function boost_custom_expire_rules_rule_nodeage_info() {
  return array(
    'name' => 'nodeage',
    'title' => 'Node age based cache expire rule',
    'description' => 'Set custom expiry of boost cache based on creation date of the node. The rule supports open ended intervals from or to a given time and also closed intervals between two points in time. Time references are taken as seconds till now(). For example a time reference of 86400 defines a point in time which is 86400 seconds from now in the past which would be exactly 1 day before now().',
    'examples' => array(
      1 => array(
        'rule' => '{"name": "nodeage", "expire": 3600, "from": 86400, "to": 7200}',
        'description' => 'The rule sets an expiry of 1 hour for nodes that are between 1 day and 2 hours old',
      ),
      2 => array(
        'rule' => '{"name": "nodeage", "expire": 86400, "to": 86400}',
        'description' => 'The rule sets an expiry of 1 day for nodes that are older than 1 day',
      ),
      3 => array(
        'rule' => '{"name": "nodeage", "expire": 1200, "from": 7200}',
        'description' => 'The rule sets an expiry of 20 minutes for nodes that are younger than 2 hours',
      ),
    ),
  );
}

/**
 * Get the expire value for the given rule based on the configuration set for the rule
 */
function boost_custom_expire_rules_rule_nodeage_get_expire_value($rule, $path) {
  // Apply the age based rule only for node views
  if (arg(0) == 'node' && is_numeric(arg(1)) && arg(2) == '') {
    // Node load is cached so OK to load here as this is a
    // node view page anyway
    $node = node_load(arg(1));
    $now = time();
    // If the rule has both from time and to time
    if (isset($rule->from) && isset($rule->to)) {
      if ($node->created > ($now - $rule->from) && $node->created < ($now - $rule->to)) {
        $expire = $rule->expire;
      }
    }
    // If the rule has only from time
    elseif (isset($rule->from) && $node->created > ($now - $rule->from)) {
      $expire = $rule->expire;
    }
    // If the rule has only to time
    elseif (isset($rule->to) && $node->created < ($now - $rule->to)) {
      $expire = $rule->expire;
    }
    // Return expire value only if the rule condition matched the node
    if (isset($expire)) {
      return $expire;
    }
  }
}

/**
 * Validate the configuration set for the rule
 */
function boost_custom_expire_rules_rule_nodeage_validate_configuration($rule) {
  $message = NULL;
  if (!isset($rule->expire)) {
    $message = 'The "expire" property is not set for the rule. ';
  }
  if (!isset($rule->from) && !isset($rule->to)) {
    $message .= 'At least one of "from" or "to" properties has to be set for the rule. ';
  }
  if (isset($rule->expire) && !is_int($rule->expire) || isset($rule->from) && !is_int($rule->from) || isset($rule->to) && !is_int($rule->to)) {
    $message .= 'All the properties "expire", "from" and "to" must be set as integers for the rule. ';
  }
  if (isset($message)) {
    return $message;
  }
}

