<?php
/**
 * @file A entity reference selection handler class to allow us to make use of
 * advance party features such as hats.
 */

/**
 * Entity Selection Handler class for Party Entities
 */
class EntityReference_SelectionHandler_Generic_party extends EntityReference_SelectionHandler_Generic {

  /**
   * Add a "target hats" field to the Entity field query.
   */
  public static function settingsForm($field, $instance) {
    $form = parent::settingsForm($field, $instance);

    // If party_hat is enabled allow filtering by them.
    if (module_exists('party_hat')) {
      $hats = entity_load('party_hat');

      $options = array();
      foreach ($hats as $hat) {
        $options[$hat->name] = $hat->label;
      }

      $form['target_hats'] = array(
        '#type' => 'select',
        '#title' => t('Target hats'),
        '#options' => $options,
        '#default_value' => isset($field['settings']['handler_settings']['target_hats']) ? $field['settings']['handler_settings']['target_hats'] : array(),
        '#size' => 6,
        '#multiple' => TRUE,
        '#description' => t('The hats that can be referenced. Optional, leave empty for all hats.')
      );
    }
    else {
      $form['target_hats'] = array(
        '#type' => 'value',
        '#value' => array(),
      );
    }

    // If party_user is enabled allow filtering by parties thaat have a user
    if (module_exists('party_user')) {
      // @todo: Implement. EFQ doesn't let you do joins. So we may have to be clever.
    }

    return $form;
  }

  /**
   * Add a field condition to the EntityFieldQuery to limit by hats.
   */
  protected function buildEntityFieldQuery($match = NULL, $match_operator = 'CONTAINS') {
    $query = parent::buildEntityFieldQuery($match, $match_operator);

    // If party_hat is enabled filter by them
    if (module_exists('party_hat') && !empty($this->field['settings']['handler_settings']['target_hats'])) {
      $query->fieldCondition('party_hat', 'hat_name', $this->field['settings']['handler_settings']['target_hats'], 'IN');
    }

    return $query;
  }
}
