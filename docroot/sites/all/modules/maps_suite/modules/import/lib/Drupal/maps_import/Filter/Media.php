<?php

namespace Drupal\maps_import\Filter;

/**
 * Class for MaPS Media Filter.
 */
class Media extends Filter {

  /**
   * Return available conditions.
   *
   * @return array
   */
  public function getAvailableConditions() {
    return array(
      'media_type' => array(
        'class' => 'Drupal\\maps_import\\Filter\\Condition\\Leaf\\Media\\Type',
        'title' => t('Media type'),
        'type' => 'criterium',
      ),
    );
  }

}
