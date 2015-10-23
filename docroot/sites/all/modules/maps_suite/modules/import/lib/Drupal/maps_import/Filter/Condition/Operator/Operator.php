<?php

namespace Drupal\maps_import\Filter\Condition\Operator;

use Drupal\maps_import\Filter\Condition\Condition;

/**
 * Special conditions: logical operator.
 */
abstract class Operator extends Condition {

  /**
   * @inheritdoc
   */
  public function isContainer() {
    return TRUE;
  }

  /**
   * @inheritdoc
   */
  public function getLabel() {
    $name = t($this->getTitle(), array(), array('context' => 'conditional rules'));
    return t('Operator @name (ID: @id)', array('@name' => $name, '@id' => $this->getId()));
  }

  /**
   * Return the operator form.
   * 
   * @param $form
   * @param $form_state
   * 
   * @return array
   */
  public function form($form, &$form_state) {
    return $form;
  }

}

