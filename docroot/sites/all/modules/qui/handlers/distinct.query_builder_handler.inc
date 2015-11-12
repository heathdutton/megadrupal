<?php
/**
 * @file
 * Contains the Distinct handler.
 */

/**
 * Class Qui_Distinct_Handler
 */
class Qui_Distinct_Handler extends Qui_Query_Builder_Handler {

  public $name = 'distinct';

  /**
   * Enable if the select table is set.
   */
  public function preBuild() {
    if (!empty($this->getHandler('qui_select_table')->defaults)) {
      $this->enabled = TRUE;
    }
  }

  /**
   * Create the distinct checkbox.
   */
  public function build() {
    $this->elements['query']['extras'][$this->name] = array(
      '#type' => 'checkbox',
      '#title' => t(ucfirst($this->name)) . ' ' . l(t('[?]'), 'https://www.drupal.org/node/1848354', array('attributes' => array('target' => '_blank'))),
      '#default_value' => $this->defaults,
    );
  }

  /**
   * Set the distinct default.
   */
  public function setDefaults() {
    $this->defaults = $this->getDefaultValues('distinct');
  }

  /**
   * Save the distinct value.
   */
  public function submit() {
    $this->report->query->distinct = $this->defaults;
  }

}
