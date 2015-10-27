<?php
/**
 * @file
 * Contains the Expressions handler.
 */

/**
 * Class Qui_Expressions_Handler
 */
class Qui_Expressions_Handler extends Qui_Query_Builder_Handler {

  public $name = 'expressions';

  /**
   * Enable if the select table is set.
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
    if (in_array($this->buttonAction(), array('addExpression', 'deleteExpression'))) {
      return FALSE;
    }
    if (isset($this->controller->form_state['triggering_element']['#name']) && strpos($this->controller->form_state['triggering_element']['#name'], 'expressions[') === 0) {
      return FALSE;
    }
    return TRUE;
  }

  /**
   * Create the expressions elements.
   */
  public function build() {

    $this->elements['query']['expressions-wrapper'] = array(
      '#type' => 'fieldset',
      '#title' => t(ucfirst($this->name)),
      '#collapsible' => TRUE,
      '#collapsed' => $this->collapsedState(),
      '#attributes' => array('class' => array('expressions-fieldset')),
    );

    $wrapper = &$this->elements['query']['expressions-wrapper'];

    $wrapper['help'] = array(
      '#markup' => $this->helpLink('https://www.drupal.org/node/1848358', t('Learn about Expressions.')),
    );

    $wrapper[$this->name] = array('#tree' => TRUE);

    foreach ($this->defaults as $key => $exp) {

      $wrapper[$this->name][$key] = array(
        '#type' => 'fieldset',
        '#attributes' => array('class' => array('inline-items')),
        '#weight' => is_numeric($exp['weight']) ? $exp['weight'] : $this->nextHighestWeight(),
        '#delta' => self::WEIGHT_DELTA,
      );

      $wrapper[$this->name][$key]['alias'] = array(
        '#type' => 'textfield',
        '#title' => t('Alias'),
        '#default_value' => $exp['alias'],
        '#size' => 20,
      );

      $wrapper[$this->name][$key]['value'] = array(
        '#type' => 'textfield',
        '#title' => t('Expression'),
        '#default_value' => $exp['value'],
      );

      $wrapper[$this->name][$key]['weight'] = array(
        '#type' => 'weight',
        '#title' => t('Weight'),
        '#attributes' => array('class' => array('expressions-weight')),
        '#default_value' => is_numeric($exp['weight']) ? $exp['weight'] : $this->nextHighestWeight(),
      );

      $wrapper[$this->name][$key]['delete_expression'] = array(
        '#type' => 'button',
        '#value' => t('Delete'),
        '#name' => 'delete_expression_' . $key,
        '#button_action' => 'deleteExpression',
        '#delete_expression' => $key,
        '#ajax' => $this->ajaxSettings(),
        '#attributes' => array('class' => array('delete-button')),
        '#validate' => array(),
      );

    }

    $wrapper['add_expression'] = array(
      '#type' => 'button',
      '#value' => t('Add Expression'),
      '#name' => 'add_expression',
      '#button_action' => 'addExpression',
      '#ajax' => $this->ajaxSettings(),
    );

  }

  /**
   * Set default expression values.
   */
  public function setDefaults() {
    $this->defaults = $this->getDefaultValues($this->name);
  }

  /**
   * Delete an expression.
   */
  public function deleteExpression() {
    $clicked = $this->clickedButton();
    unset($this->defaults[$clicked['#delete_expression']]);
    unset($this->controller->form_state['values'][$this->name][$clicked['#delete_expression']]);
  }

  /**
   * Add an expression.
   */
  public function addExpression() {
    $this->defaults[] = array();
  }

  /**
   * Validate that expressions were entered correctly.
   */
  public function validate() {
    foreach ($this->defaults as $key => $expression) {
      if (empty($expression['value'])) {
        form_set_error("expressions][$key][value", t('Expression @key value is required.', array('@key' => $key)));
      }
    }
  }

  /**
   * Save the expression values.
   */
  public function submit() {
    foreach ($this->defaults as $key => &$exp) {
      unset($exp['delete_expressions']);
    }
    $this->report->query->expressions = array_values($this->sortByWeight());
  }

}
