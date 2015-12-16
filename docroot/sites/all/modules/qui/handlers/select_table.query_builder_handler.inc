<?php
/**
 * @file
 * Contains the Select Table handler.
 */

/**
 * Class Qui_Select_Table_Handler
 */
class Qui_Select_Table_Handler extends Qui_Query_Builder_Handler {

  public $enabled = TRUE;
  public $name = 'select_table';

  /**
   * Sets the select_table_state for other elements to use.
   */
  public function __construct($controller) {
    parent::__construct($controller);
    // Create state that other handlers can use.
    $this->select_table_state = array('visible' => array(':input[name=select_table]' => array('empty' => FALSE)));
  }

  /**
   * Creates the select table elements.
   */
  public function build() {
    $this->elements['query'] = array(
      '#type' => 'fieldset',
      '#title' => t('Query Builder'),
      '#description' => t('Build your dynamic query to generate a report. For more information, view the !link or the !drupal.', array(
        '!link' => l(t('QUI Docs'), 'https://www.drupal.org/node/2307683',
          array('attributes' => array('target' => '_blank'))),
        '!drupal' => l(t('Dynamic Queries Docs'), 'https://www.drupal.org/node/310075',
          array('attributes' => array('target' => '_blank'))),
      )),
      '#states' => $this->getHandler('qui_settings')->report_name_state,
    );
    $this->elements['query'][$this->name] = array(
      '#type' => 'select',
      '#title' => t('Select from table'),
      '#options' => $this->getTables(),
      '#required' => TRUE,
      '#empty_option' => '-- select a table --',
      '#attributes' => array('class' => array('qui-autocomplete')),
      '#suffix' => '<div id="query-options"></div>',
      '#default_value' => $this->defaults,
      '#ajax' => $this->ajaxSettings(),
    );
    return $this;
  }

  /**
   * Sets the default value for the select table.
   */
  public function setDefaults() {
    $this->defaults = $this->getDefaultValues($this->name);
  }

  /**
   * Save the select table.
   */
  public function submit() {
    $this->report->query->select_table = $this->defaults;
  }

}
