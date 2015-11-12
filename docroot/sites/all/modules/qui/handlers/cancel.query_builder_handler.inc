<?php
/**
 * @file
 * Contains Cancel handler.
 */

/**
 * Class Qui_Cancel_Handler
 */
class Qui_Cancel_Handler extends Qui_Query_Builder_Handler {

  public $name = 'cancel';
  public $enabled = TRUE;

  /**
   * Create the cancel button.
   */
  public function build() {
    $this->elements['actions']['cancel'] = array(
      '#type' => 'button',
      '#weight' => 3,
      '#button_action' => 'cancelSubmit',
      '#value' => t('Cancel'),
      '#states' => array('visible' => array(':input[name=select_table]' => array('filled' => TRUE))),
      '#validate' => array(),
    );
  }

  /**
   * Cancel button pressed.
   */
  public function cancelSubmit() {
    $this->getHandler('qui_cache')->deleteCache();
    drupal_goto('admin/reports/qui');
  }

}
