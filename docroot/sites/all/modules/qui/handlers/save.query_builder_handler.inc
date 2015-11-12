<?php
/**
 * @file
 * Contains the Save handler.
 */

/**
 * Class Qui_Save_Handler
 */
class Qui_Save_Handler extends Qui_Query_Builder_Handler {

  public $enabled = TRUE;
  public $name = 'save';

  /**
   * Create the Save button.
   */
  public function build() {
    $this->elements['actions'] = array('#tree' => TRUE);
    $this->elements['actions'][$this->name] = array(
      '#type' => 'submit',
      '#value' => t(ucfirst($this->name)),
      '#states' => $this->getHandler('qui_settings')->report_name_state,
    );
  }

}
