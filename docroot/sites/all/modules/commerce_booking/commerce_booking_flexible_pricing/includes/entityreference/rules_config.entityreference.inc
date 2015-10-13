<?php
/**
 * @file
 * A entity reference selection handler class for rules.
 */

/**
 * Entity Selection Handler class for Rules Configuration Entities
 */
class EntityReference_SelectionHandler_Generic_rules_config extends EntityReference_SelectionHandler_Generic {

  /**
   * Add a "target hats" field to the Entity field query.
   */
  public static function settingsForm($field, $instance) {
    $form = parent::settingsForm($field, $instance);

    // Optionally filter by tag.
    $options = db_select('rules_tags', 'rt')
      ->fields('rt', array('tag'))
      ->distinct()
      ->orderBy('tag')
      ->execute()->fetchCol();

    $form['target_tags'] = array(
      '#type' => 'select',
      '#title' => t('Target Tags'),
      '#options' => drupal_map_assoc($options),
      '#default_value' => isset($field['settings']['handler_settings']['target_tags']) ? $field['settings']['handler_settings']['target_tags'] : array(),
      '#size' => 6,
      '#multiple' => TRUE,
      '#description' => t('The tags that can be referenced. Optional, leave empty for all hats.'),
    );

    return $form;
  }

  /**
   * Add a field condition to the EntityFieldQuery to limit by hats.
   */
  protected function buildEntityFieldQuery($match = NULL, $match_operator = 'CONTAINS') {
    $query = parent::buildEntityFieldQuery($match, $match_operator);

    // If selected, filter by tags.
    if (!empty($this->field['settings']['handler_settings']['target_tags'])) {
      $ids = db_select('rules_tags', 'rt')
        ->fields('rt', array('id'))
        ->distinct()
        ->condition('rt.tag', $this->field['settings']['handler_settings']['target_tags'])
        ->orderBy('tag')
        ->execute()->fetchCol();
      $query->entityCondition('entity_id', $ids);
    }

    return $query;
  }
}
