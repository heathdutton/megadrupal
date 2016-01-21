<?php
/**
 * @file
 * Custom rule for expiring URLs by drupal path or the alias
 */

/**
 * Show information about the rule in admin settings form
 */
function boost_custom_expire_rules_rule_path_info() {
  return array(
    'name' => 'path',
    'title' => 'Drupal path / URL based cache expire rule',
    'description' => 'Set custom expiry of boost cache based on drupal path. The rule supports setting different expire times for different patterns. The patters are the same as used in block configuration pages.',
    'examples' => array(
      1 => array(
        'rule' => '{"name": "path", "expire": 600, "path": "&lt;front&gt;"}',
        'description' => 'Set an expiry of 600 seconds for the home page',
      ),
      2 => array(
        'rule' => '{"name": "path", "expire": 3600, "path": "taxonomy/term/*"}',
        'description' => 'Set an expiry of 3600 seconds for the taxonomy term pages',
      ),
      3 => array(
        'rule' => '{"name": "path", "expire": 1200, "path": "categories/news"}',
        'description' => 'Set an expiry of 1200 seconds for the taxonomy term page with alias categories/news',
      ),
    ),
  );
}

/**
 * Get the expire value for the given rule based on the configuration set for the rule
 */
function boost_custom_expire_rules_rule_path_get_expire_value($rule, $path) {
  // $path is the aliased path
  // Apply the rule only when the path or the alias matches
  if (drupal_match_path($path, $rule->path) || drupal_match_path($_GET['q'], $rule->path)) {
    return ($rule->expire);
  }
}

/**
 * Validate the configuration set for the rule
 */
function boost_custom_expire_rules_rule_path_validate_configuration($rule) {
  $message = NULL;
  if (!isset($rule->expire)) {
    $message = 'The "expire" property is not set for the rule. ';
  }
  else {
    if (!is_int($rule->expire)) {
      $message .= 'The property "expire" must be set as an integer for the rule.';
    }
  }
  if (!isset($rule->path)) {
    $message .= 'The "path" property is not set for the rule.';
  }
  if (isset($message)) {
    return $message;
  }
}

