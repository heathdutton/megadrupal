<?php
/**
 * @file
 * Contains the Range handler.
 */

/**
 * Class Qui_Range_Handler
 */
class Qui_Range_Handler extends Qui_Query_Builder_Handler {

  public $name = 'range';

  /**
   * Enable Range if the select table is set.
   */
  public function preBuild() {
    if (!empty($this->getHandler('qui_select_table')->defaults)) {
      $this->enabled = TRUE;
    }
  }

  /**
   * Gets the Range form elements.
   */
  public function build() {

    $this->elements['query']['extras'] = array(
      '#type' => 'fieldset',
      '#title' => t('Extras'),
      '#collapsible' => TRUE,
      '#attributes' => array('class' => array('extras-fieldset')),
      '#collapsed' => TRUE,
    );

    $this->elements['query']['extras'][$this->name] = array(
      '#type' => 'fieldset',
      '#title' => t(ucfirst($this->name)),
      '#tree' => TRUE,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#attributes' => array('class' => array('inline-items')),
    );
    $this->elements['query']['extras'][$this->name]['help'] = array(
      '#markup' => $this->helpLink('https://www.drupal.org/node/1848368', t('Learn about Ranges.')),
    );

    $this->elements['query']['extras'][$this->name]['min'] = array(
      '#type' => 'textfield',
      '#title' => t('Min'),
      '#default_value' => $this->defaults['min'],
    );
    $this->elements['query']['extras'][$this->name]['max'] = array(
      '#type' => 'textfield',
      '#title' => t('Max'),
      '#default_value' => $this->defaults['max'],
    );

  }

  /**
   * Sets default values for Range.
   */
  public function setDefaults() {
    $this->defaults = (array) $this->getDefaultValues($this->name);
    $this->defaults['min'] = isset($this->defaults['min']) ? $this->defaults['min'] : 0;
    $this->defaults['max'] = isset($this->defaults['max']) ? $this->defaults['max'] : 0;
  }

  /**
   * Save range values.
   */
  public function submit() {
    $this->report->query->range = (array) $this->defaults;
  }

}
