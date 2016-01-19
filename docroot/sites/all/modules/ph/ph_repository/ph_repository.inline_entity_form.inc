<?php
/**
 * @file
 * Inline entity form integration.
 */

/**
 * Inline entity form controller.
 */
class PHRepositoryInlineEntityFormController extends EntityInlineEntityFormController {
  /**
   * Overrides labels.
   */
  public function labels() {
    $labels = array(
      'singular' => t('repository'),
      'plural' => t('repositories'),
    );
    return $labels;
  }

  /**
   * Declares fields to display in the entity table.
   */
  public function defaultFields($bundles) {
    $fields['type'] = array(
      'type' => 'property',
      'label' => t('Type'),
      'visible' => TRUE,
      'weight' => -1,
    );
    $fields['summary'] = array(
      'type' => 'property',
      'label' => t('Summary'),
      'visible' => TRUE,
    );

    return $fields;
  }
}
