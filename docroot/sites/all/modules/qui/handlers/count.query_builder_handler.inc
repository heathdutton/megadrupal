<?php
/**
 * @file
 * Contains Count handler.
 */

/**
 * Class Qui_Count_Handler
 */
class Qui_Count_Handler extends Qui_Query_Builder_Handler {

  public $name = 'row_count';

  /**
   * Enable if the select_table is set.
   */
  public function preBuild() {
    if (!empty($this->getHandler('qui_select_table')->defaults)) {
      $this->enabled = TRUE;
    }
  }

  /**
   * Create row_count checkbox.
   */
  public function build() {
    $this->elements['query']['extras'][$this->name] = array(
      '#type' => 'checkbox',
      '#title' => t('Row Count') . ' ' . l(t('[?]'), 'https://www.drupal.org/node/1848376', array('attributes' => array('target' => '_blank'))),
      '#default_value' => $this->defaults,
    );
  }

  /**
   * Set row count defaults.
   */
  public function setDefaults() {
    $this->defaults = $this->getDefaultValues($this->name);
  }

  /**
   * Save row count data.
   */
  public function submit() {
    $this->report->query->row_count = $this->defaults;
  }

}
