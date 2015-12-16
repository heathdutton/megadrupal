<?php
/**
 * @file
 * Contains Column handler.
 */

/**
 * Class Qui_Columns_Handler
 */
class Qui_Columns_Handler extends Qui_Query_Builder_Handler {

  public $name = 'columns';

  /**
   * Enable if the select_table is set.
   */
  public function preBuild() {
    if (!empty($this->getHandler('qui_select_table')->defaults)) {
      $this->enabled = TRUE;
    }
  }

  /**
   * Get the collapsed state for the fieldset.
   */
  private function collapsedState() {
    if (in_array($this->buttonAction(), array('addColumn', 'deleteColumn'))) {
      return FALSE;
    }
    if (isset($this->controller->form_state['triggering_element']['#name']) && strpos($this->controller->form_state['triggering_element']['#name'], 'columns[') === 0) {
      return FALSE;
    }
    return TRUE;
  }

  /**
   * Create the columns form elements.
   */
  public function build() {

    $this->elements['query']['columns-wrapper'] = array(
      '#type' => 'fieldset',
      '#title' => t(ucfirst($this->name)),
      '#collapsible' => TRUE,
      '#collapsed' => $this->collapsedState(),
      '#attributes' => array('class' => array('columns-fieldset')),
    );

    $wrapper = &$this->elements['query']['columns-wrapper'];

    $wrapper['help'] = array(
      '#markup' => $this->helpLink('https://www.drupal.org/node/1848358', t('Learn about Columns.')),
    );

    $wrapper[$this->name]['#tree'] = TRUE;

    foreach ($this->defaults as $key => $col) {

      $wrapper[$this->name][$key] = array(
        '#type' => 'fieldset',
        '#attributes' => array('class' => array('inline-items')),
        '#weight' => is_numeric($col['weight']) ? $col['weight'] : $this->nextHighestWeight(),
      );

      $wrapper[$this->name][$key]['alias'] = array(
        '#type' => 'textfield',
        '#title' => t('Alias'),
        '#default_value' => $col['alias'],
        '#size' => 20,
      );

      $wrapper[$this->name][$key]['column'] = array(
        '#type' => 'select',
        '#title' => t('Column'),
        '#options' => $this->getFormattedActiveColumns(),
        '#empty_option' => t('-- select a column --'),
        '#default_value' => $col['column'],
        '#attributes' => array('class' => array('qui-autocomplete')),
      );

      $wrapper[$this->name][$key]['weight'] = array(
        '#type' => 'weight',
        '#title' => t('Weight'),
        '#attributes' => array('class' => array('columns-weight')),
        '#default_value' => is_numeric($col['weight']) ? $col['weight'] : $this->nextHighestWeight(),
        '#delta' => self::WEIGHT_DELTA,
      );

      $wrapper[$this->name][$key]['delete_column'] = array(
        '#type' => 'button',
        '#value' => t('Delete'),
        '#name' => 'delete_columns_' . $key,
        '#button_action' => 'deleteColumn',
        '#delete_column' => $key,
        '#ajax' => $this->ajaxSettings(),
        '#attributes' => array('class' => array('delete-button')),
        '#validate' => array(),
      );

    }

    $wrapper['add_column'] = array(
      '#type' => 'button',
      '#value' => t('Add Column'),
      '#name' => 'add_column',
      '#button_action' => 'addColumn',
      '#ajax' => $this->ajaxSettings(),
    );

  }

  /**
   * Delete a column.
   */
  public function deleteColumn() {
    $clicked = $this->clickedButton();
    unset($this->defaults[$clicked['#delete_column']]);
    unset($this->controller->form_state['values'][$this->name][$clicked['#delete_column']]);
  }

  /**
   * Add a column.
   */
  public function addColumn() {
    $this->defaults[] = array();
  }

  /**
   * Set the default columns.
   */
  public function setDefaults() {
    $this->defaults = $this->getDefaultValues($this->name);
  }

  /**
   * Validates the Column values.
   */
  public function validate() {
    foreach ($this->defaults as $key => $col) {
      if (empty($col['column'])) {
        form_set_error("columns][$key][column", t('Column @key cannot be empty.', array('@key' => $key)));
      }
    }
  }

  /**
   * Save the columns.
   */
  public function submit() {
    foreach ($this->defaults as $key => &$col) {
      unset($col['delete_column']);
    }
    $this->report->query->columns = array_values($this->sortByWeight());
  }

}
