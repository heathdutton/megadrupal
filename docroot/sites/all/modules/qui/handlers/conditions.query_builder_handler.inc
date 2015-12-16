<?php
/**
 * @file
 * Contains the Conditions handler.
 */

/**
 * Class Qui_Conditions_Handler
 */
class Qui_Conditions_Handler extends Qui_Query_Builder_Handler {

  public $name = 'conditions';

  /**
   * Enabled conditions if the select_table is set.
   */
  public function preBuild() {
    if (!empty($this->getHandler('qui_select_table')->defaults)) {
      $this->enabled = TRUE;
    }
  }

  /**
   * Gets the collapsed state for conditions.
   */
  private function collapsedState() {
    if (in_array($this->buttonAction(), array('addCondition', 'deleteCondition'))) {
      return FALSE;
    }
    if (isset($this->controller->form_state['triggering_element']['#name']) && strpos($this->controller->form_state['triggering_element']['#name'], 'conditions[') === 0) {
      return FALSE;
    }
    return TRUE;
  }

  /**
   * Build the form elements for conditions.
   */
  public function build() {

    $this->elements['query']['conditions-wrapper'] = array(
      '#type' => 'fieldset',
      '#title' => t(ucfirst($this->name)),
      '#collapsible' => TRUE,
      '#collapsed' => $this->collapsedState(),
    );

    $wrapper = &$this->elements['query']['conditions-wrapper'];

    $wrapper['help'] = array(
      '#markup' => $this->helpLink('https://www.drupal.org/node/310086', t('Learn about Conditions.')),
    );

    $wrapper[$this->name] = array('#tree' => TRUE);

    foreach ($this->defaults as $key => $cond) {

      $wrapper[$this->name][$key] = array(
        '#type' => 'fieldset',
        '#attributes' => array('class' => array('inline-items')),
        '#title' => $cond['type'],
        '#weight' => is_numeric($cond['weight']) ? $cond['weight'] : $this->nextHighestWeight(),
      );

      $wrapper[$this->name][$key]['type'] = array(
        '#type' => 'hidden',
        '#default_value' => $cond['type'],
        '#suffix' => t(ucwords(str_replace('_', ' ', $cond['type']))),
      );

      if ($cond['type'] == 'where' || $cond['type'] == 'having') {
        $wrapper[$this->name][$key]['column'] = array(
          '#type' => 'select',
          '#options' => $this->getFormattedActiveColumns(),
          '#empty_option' => '-- select a column --',
          '#title' => t('Column'),
          '#attributes' => array('class' => array('qui-autocomplete')),
          '#default_value' => $cond['column'],
        );
        $wrapper[$this->name][$key]['operator'] = array(
          '#type' => 'select',
          '#title' => t('Operator'),
          '#empty_option' => '-- operator --',
          '#options' => $this->conditionOperators(),
          '#default_value' => $cond['operator'],
        );
        $wrapper[$this->name][$key]['value'] = array(
          '#type' => 'textfield',
          '#title' => t('Value'),
          '#default_value' => $cond['value'],
        );
      }
      elseif ($cond['type'] == 'where_snippet' || $cond['type'] == 'having_snippet') {
        $wrapper[$this->name][$key]['value'] = array(
          '#title' => t('Snippet'),
          '#type' => 'textfield',
          '#default_value' => $cond['value'],
        );
      }

      $wrapper[$this->name][$key]['weight'] = array(
        '#type' => 'weight',
        '#title' => t('Weight'),
        '#attributes' => array('class' => array('conditions-weight')),
        '#default_value' => is_numeric($cond['weight']) ? $cond['weight'] : $this->nextHighestWeight(),
        '#delta' => self::WEIGHT_DELTA,
      );

      $wrapper[$this->name][$key]['delete_condition'] = array(
        '#type' => 'button',
        '#value' => t('Delete'),
        '#name' => 'delete_condition_' . $key,
        '#button_action' => 'deleteCondition',
        '#delete_condition' => $key,
        '#ajax' => $this->ajaxSettings(),
        '#attributes' => array('class' => array('delete-button')),
        '#validate' => array(),
      );

    }

    $wrapper['add_where'] = array(
      '#type' => 'button',
      '#value' => t('Where'),
      '#name' => 'addWhere',
      '#button_action' => 'addCondition',
      '#condition_type' => 'where',
      '#ajax' => $this->ajaxSettings(),
    );
    $wrapper['add_where_snippet'] = array(
      '#type' => 'button',
      '#value' => t('Where Snippet'),
      '#name' => 'add_where_snippet',
      '#condition_type' => 'where_snippet',
      '#button_action' => 'addCondition',
      '#ajax' => $this->ajaxSettings(),
    );
    $wrapper['add_having'] = array(
      '#type' => 'button',
      '#value' => t('Having'),
      '#name' => 'add_having',
      '#button_action' => 'addCondition',
      '#condition_type' => 'having',
      '#ajax' => $this->ajaxSettings(),
    );
    $wrapper['add_having_snippet'] = array(
      '#type' => 'button',
      '#value' => t('Having Snippet'),
      '#name' => 'add_having_snippet',
      '#condition_type' => 'having_snippet',
      '#button_action' => 'addCondition',
      '#ajax' => $this->ajaxSettings(),
    );
  }

  /**
   * Get condition operators.
   */
  public function conditionOperators() {
    return _qui_get_conditions();
  }

  /**
   * Deletes a condition when the delete_condition button is pressed.
   */
  public function deleteCondition() {
    $clicked = $this->clickedButton();
    unset($this->defaults[$clicked['#delete_condition']]);
    unset($this->controller->form_state['values'][$this->name][$clicked['#delete_condition']]);
  }

  /**
   * Adds a condition when an add_condition button is pressed.
   */
  public function addCondition() {
    $clicked = $this->clickedButton();
    $this->defaults[] = array(
      'type' => $clicked['#condition_type'],
    );
  }

  /**
   * Sets default values for conditions.
   */
  public function setDefaults() {
    $this->defaults = $this->getDefaultValues($this->name);
  }

  /**
   * Ensure conditions are valid.
   */
  public function validate() {
    foreach ($this->defaults as $key => $cond) {
      if ($cond['type'] == 'where' || $cond['type'] == 'having') {
        if (empty($cond['column'])) {
          form_set_error("conditions][$key][column", t('Condition @key column cannot be empty.', array('@key' => $key)));
        }
        if (empty($cond['operator'])) {
          form_set_error("conditions][$key][operator", t('Condition @key operator cannot be empty.', array('@key' => $key)));
        }
      }
      else {
        if (empty($cond['value'])) {
          form_set_error("conditions][$key][value", t('Condition @key value cannot be empty.', array('@key' => $key)));
        }
      }
    }
  }

  /**
   * Save condition values.
   */
  public function submit() {
    foreach ($this->defaults as $key => &$cond) {
      unset($cond['delete_condition']);
    }
    $this->report->query->conditions = array_values($this->sortByWeight());
  }

}
