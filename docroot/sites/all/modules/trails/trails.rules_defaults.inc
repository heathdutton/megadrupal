<?php
/**
 * @file
 * Default Rules configuration to handle active trail.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function trails_default_rules_configuration() {
  $rules = array();

  // Create rule set for emulating default behavior setting breadcrumb.
  $rule_set = rules_rule_set(array());
  $rule_set->label = t('Set trail as breadcrumb');
  $rule_set->tags = array('trails');
  // Add rule 1: get trail.
  $rule = rule()
    ->action('trails_get_trail', array('trail:var' => 'trail', 'trail:label' => t('Current trail')));
  $rule->label = t('Get trail');
  $rule_set->rule($rule);
  // Add rule 2: set breadcrumb if trail not empty.
  $rule = rule()
    ->condition(rules_condition('data_is_empty', array('data:select' => 'trail'))->negate())
    // Prepend "Home" crumb.
    ->action('trails_create_crumb', array('title' => t('Home'), 'path' => '<front>', 'created_crumb:var' => 'home_crumb', 'created_crumb:label' => t('Home crumb')))
    ->action('list_add', array('list:select' => 'trail', 'item:select' => 'home_crumb', 'pos' => 'start'))
    // Set breadcrumb.
    ->action('trails_set_breadcrumb', array('trail:select' => 'trail'));
  $rule->label = t('Set breadcrumb if not empty');
  $rule_set->rule($rule);
  $rules['trails_set_breadcrumb'] = $rule_set;

  // Create inactive rule reacting on page render to set breadcrumb.
  $rule = rules_reaction_rule()
    ->event('trails_preprocess_page')
    // React on non-admin page.
    ->condition(rules_or()
      ->condition(rules_condition('trails_page_is_front'))
      ->condition(rules_condition('trails_page_is_admin'))
      ->negate())
    // Invoke component.
    ->action('component_trails_set_breadcrumb');
  $rule->label = t('Render trail as breadcrumb');
  $rule->tags = array('trails');
  $rule->active = FALSE;
  $rules['trails_set_breadcrumb_on_render'] = $rule;

  return $rules;
}
