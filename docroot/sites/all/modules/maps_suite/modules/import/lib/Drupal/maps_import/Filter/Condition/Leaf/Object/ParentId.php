<?php

namespace Drupal\maps_import\Filter\Condition\Leaf\Object;

use Drupal\maps_import\Filter\Condition\Leaf\Leaf;

/**
 * Condition on object parent.
 */
class ParentId extends Leaf {

  /**
   * @inheritdoc
   */
  public function getTitle() {
    return 'Object parent ID';
  }

  /**
   * @inheritdoc
   */
  public function getType() {
    return 'object_parent_id';
  }

  /**
   * @inheritdoc
   */
  public function match(array $entity) {
    if (!$this->getCriteria()) {
      return !$this->negate;
    }
    $criteria = $this->getCriteria();

    return $this->checkNegate($entity['parent_id'] == reset($criteria));
  }

  /**
   * Build the filter condition form element.
   *
   * @param $form
   *   An associative array containing the structure of the form.
   * @param $form_state
   *   A keyed array containing the current state of the form.
   */
  public function form($form, &$form_state) {
    $form['criteria'] = array(
      '#type' => 'textfield',
      '#title' => $this->getTitle(),
      '#required' => TRUE,
      '#default_value' => $this->getCriteria(),
    );

    $form['negate'] = array(
      '#type' => 'checkbox',
      '#title' => t('Negate'),
      '#default_value' => $this->negate,
    );

    return $form;
  }

}
