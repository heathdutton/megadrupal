<?php

/**
 * @file
 * Rules integration.
 */

/**
 * Implements hook_rules_action_info().
 */
function taxonomy_tools_publisher_rules_action_info() {
  $defaults = array(
    'parameter' => array(
      'taxonomy_term' => array(
        'label' => t('Taxonomy Term'),
        'type' => 'taxonomy_term',
        'save' => TRUE,
      ),
    ),
    'group' => t('Taxonomy'),
    'access callback' => 'taxonomy_tools_publisher_rules_access',
  );
  $actions['term_publish'] = $defaults + array(
    'label' => t('Publish taxonomy term'),
    'base' => 'taxonomy_tools_publisher_rules_term_publish',
  );
  $actions['term_unpublish'] = $defaults + array(
    'label' => t('Unpublish taxonomy term'),
    'base' => 'taxonomy_tools_publisher_rules_term_unpublish',
  );
  return $actions;
}

function taxonomy_tools_publisher_rules_term_publish($term) {
  if (isset($term->field_taxonomy_term_status)) {
    $term->field_taxonomy_term_status[LANGUAGE_NONE][0]['value'] = TERM_PUBLISHED;
  }
}

function taxonomy_tools_publisher_rules_term_unpublish($term) {
  if (isset($term->field_taxonomy_term_status)) {
    $term->field_taxonomy_term_status[LANGUAGE_NONE][0]['value'] = TERM_NOT_PUBLISHED;
  }
}

function taxonomy_tools_publisher_rules_access() {
  return user_access('administer term schedule');
}
