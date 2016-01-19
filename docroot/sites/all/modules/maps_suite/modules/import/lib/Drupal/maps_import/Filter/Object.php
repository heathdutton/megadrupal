<?php

/**
 * @file
 * Handle filtering for MaPS System® objects.
 */

namespace Drupal\maps_import\Filter;

/**
 * Class for MaPS System® Object Filter.
 */
class Object extends Filter {

  /**
   * Return available conditions.
   *
   * @return array
   */
  public function getAvailableConditions() {
    return array(
      'object_nature' => array(
        'class' => 'Drupal\\maps_import\\Filter\\Condition\\Leaf\\Object\\Nature',
        'title' => t('Object nature'),
        'type' => 'criterium',
      ),
      'object_type' => array(
        'class' => 'Drupal\\maps_import\\Filter\\Condition\\Leaf\\Object\\Type',
        'title' => t('Object type'),
        'type' => 'criterium',
      ),
      'class' => array(
        'class' => 'Drupal\\maps_import\\Filter\\Condition\\Leaf\\Object\\ObjectClass',
        'title' => t('Object class'),
        'type' => 'criterium',
      ),
      'config_type' => array(
        'class' => 'Drupal\\maps_import\\Filter\\Condition\\Leaf\\Object\\ConfigurationType',
        'title' => t('Object related to configuration'),
        'type' => 'criterium',
      ),
      'object_parent_id' => array(
        'class' => 'Drupal\\maps_import\\Filter\\Condition\\Leaf\\Object\\ParentId',
        'title' => t('Object parent ID'),
        'type' => 'criterium',
      ),
    );
  }

}
