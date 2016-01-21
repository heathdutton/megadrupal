<?php

/**
 * @file
 * Views hooks for the New Relic Insights Entity module.
 */


/**
 * Implements hook_views_data_alter().
 *
 * Updates core Insights property metadata for better UX in the Views UI.
 */
function new_relic_insights_views_data_alter(&$data) {
  // Unset versions of the Insight entity that we do not want to be queryable.
  unset($data['insight'], $data['entity_insight']);

  // Rename the EntityFieldQuery label.
  if (isset($data['efq_insight'])) {
    $data['efq_insight']['table']['base']['title'] = t('Insights');

    // Loop through custom properties provided by Better Stats and apply them.
    if (module_exists('better_statistics')) {
      $current = variable_get('better_statistics_fields', better_statistics_get_default_fields());
      foreach($current as $field => $definition) {
        if (isset($definition['views_field']['title'])) {
          $data['efq_insight'][$field]['title'] = $definition['views_field']['title'];
        }

        if (isset($definition['views_field']['help'])) {
          $data['efq_insight'][$field]['help'] = $definition['views_field']['help'];
        }
      }
    }

    // Loop through the default properties and provide better UI text.
    foreach(new_relic_insights_entity_default_properties('Transaction') as $prop => $definition) {
      $data['efq_insight'][$prop]['title'] = $definition['views_field']['title'];
      $data['efq_insight'][$prop]['help'] = $definition['views_field']['help'];
    }
  }
}
