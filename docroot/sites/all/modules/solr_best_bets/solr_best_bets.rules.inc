<?php

/**
 * @file
 * Rules hook implementations for Solr Best Bets.
 */

/**
 * Implements hook_rules_action_info().
 */
function solr_best_bets_rules_action_info() {
  return array(
    'solr_best_bets_add' => array(
      'label' => t('Add node as a best bet'),
      'parameter' => array(
        'node' => array(
          'type' => 'node',
          'label' => t('Content'),
        ),
        'query_text' => array(
          'type' => 'text',
          'label' => t('Search query text'),
          'description' => t('Search queries matching this text will show the content at the top of the search result set.'),
        ),
        'environment' => array(
          'type' => 'list<text>',
          'label' => t('Search environment'),
          'options list' => 'solr_best_bets_get_environment_options',
          'description' => t('The search environment the best bet will be applied to.'),
        ),
        'exclude' => array(
          'type' => 'boolean',
          'label' => t('Exclude from results'),
          'description' => t('Exclude this content from searches queries matching the entered text.'),
          'default value' => FALSE,
        ),
      ),
      'group' => t('Search'),
      'base' => 'solr_best_bets_add_action',
    ),
  );
}

/**
 * Action that flags content as a best bet for a search query.
 *
 * @see solr_best_bets_save()
 */
function solr_best_bets_add_action($node, $query_text, $environments, $exclude) {
  module_load_include('inc', 'solr_best_bets', 'solr_best_bets.crud');
  foreach ($environments as $environment_name) {
    $values = array(
      'content_id' => 'entity:node:' . $node->nid,
      'query_text' => $query_text,
      'environment' => $environment_name,
      'exclude' => $exclude,
    );
    solr_best_bets_save($values);
  }
}
