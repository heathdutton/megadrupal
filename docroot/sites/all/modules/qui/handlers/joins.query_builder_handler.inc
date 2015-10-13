<?php
/**
 * @file
 * Contains the Join handler.
 */

/**
 * Class Qui_Joins_Handler
 */
class Qui_Joins_Handler extends Qui_Query_Builder_Handler {

  public $name = 'joins';

  /**
   * Enabled Joins if the select table is set.
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
    if (in_array($this->buttonAction(), array('addJoin', 'deleteJoin'))) {
      return FALSE;
    }
    if (isset($this->controller->form_state['triggering_element']['#name']) && strpos($this->controller->form_state['triggering_element']['#name'], 'joins[') === 0) {
      return FALSE;
    }
    return TRUE;
  }

  /**
   * Create the Join elements.
   */
  public function build() {

    $this->elements['query']['joins-wrapper'] = array(
      '#type' => 'fieldset',
      '#title' => t('Joins'),
      '#collapsible' => TRUE,
      '#collapsed' => $this->collapsedState(),
      '#attributes' => array('class' => array('joins-fieldset')),
    );

    $wrapper = &$this->elements['query']['joins-wrapper'];

    $wrapper['help'] = array(
      '#markup' => $this->helpLink('https://www.drupal.org/node/1848348', t('Learn about Joins.')),
    );

    $wrapper['joins']['#tree'] = TRUE;

    foreach ($this->defaults as $key => $join) {

      $wrapper['joins'][$key] = array(
        '#type' => 'fieldset',
        '#attributes' => array('class' => array('inline-items')),
        '#weight' => is_numeric($join['weight']) ? $join['weight'] : $this->nextHighestWeight(),
      );

      $wrapper['joins'][$key]['type'] = array(
        '#type' => 'select',
        '#title' => t('Type'),
        '#options' => $this->typeOptions(),
      );

      $wrapper['joins'][$key]['table'] = array(
        '#type' => 'select',
        '#title' => t('Table'),
        '#options' => $this->getTables(),
        '#empty_option' => '-- select a table --',
        '#attributes' => array('class' => array('qui-autocomplete')),
        '#ajax' => $this->ajaxSettings(),
        '#default_value' => $join['table'],
      );

      if (!empty($join['table'])) {
        $wrapper['joins'][$key]['left_col'] = array(
          '#type' => 'select',
          '#title' => t('Left Column'),
          '#options' => $this->getFormattedActiveColumns(),
          '#empty_option' => '-- choose column --',
          '#attributes' => array('class' => array('qui-autocomplete')),
          '#default_value' => $join['left_col'],
        );
        $wrapper['joins'][$key]['right_col'] = array(
          '#type' => 'select',
          '#title' => t('Right Column'),
          '#options' => $this->getFormattedActiveColumns(),
          '#empty_option' => '-- choose column --',
          '#attributes' => array('class' => array('qui-autocomplete')),
          '#default_value' => $join['right_col'],
        );
      }

      $wrapper['joins'][$key]['weight'] = array(
        '#type' => 'weight',
        '#title' => t('Weight'),
        '#attributes' => array('class' => array('joins-weight')),
        '#default_value' => is_numeric($join['weight']) ? $join['weight'] : $this->nextHighestWeight(),
        '#delta' => self::WEIGHT_DELTA,
      );

      $wrapper['joins'][$key]['delete_join'] = array(
        '#type' => 'button',
        '#value' => t('Delete'),
        '#name' => 'delete_join_' . $key,
        '#button_action' => 'deleteJoin',
        '#delete_join' => $key,
        '#ajax' => $this->ajaxSettings(),
        '#attributes' => array('class' => array('delete-button')),
        '#validate' => array(),
      );
    }

    $wrapper['add_join'] = array(
      '#type' => 'button',
      '#value' => t('Add Join'),
      '#name' => 'add_join',
      '#button_action' => 'addJoin',
      '#ajax' => $this->ajaxSettings(),
    );

  }


  /**
   * Set default Join values.
   */
  public function setDefaults() {
    $this->defaults = $this->getDefaultValues($this->name);
  }

  /**
   * Get join types.
   */
  private function typeOptions() {
    return array(
      'left' => t('Left'),
      'inner' => t('Inner'),
      'right' => t('Right'),
    );
  }

  /**
   * Delete a Join.
   */
  public function deleteJoin() {
    $clicked = $this->clickedButton();
    unset($this->defaults[$clicked['#delete_join']]);
    unset($this->controller->form_state['values'][$this->name][$clicked['#delete_join']]);
  }

  /**
   * Add a Join.
   */
  public function addJoin() {
    $clicked = $this->clickedButton();
    $this->defaults[] = array(
      'type' => $clicked['#join_type'],
    );
  }

  /**
   * Validate the Join values.
   */
  public function validate() {
    foreach ($this->defaults as $key => $join) {
      if (empty($join['table'])) {
        form_set_error("joins][$key][table", t('Join @key table cannot be empty.', array('@key' => $key)));
      }
      if (empty($join['left_col'])) {
        form_set_error("joins][$key][left_col", t('Join @key left column cannot be empty.', array('@key' => $key)));
      }
      if (empty($join['right_col'])) {
        form_set_error("joins][$key][right_col", t('Join @key right column cannot be empty.', array('@key' => $key)));
      }
    }
  }

  /**
   * Save the Join values.
   */
  public function submit() {
    foreach ($this->defaults as $key => &$join) {
      unset($join['delete_join']);
    }
    $this->report->query->joins = array_values($this->sortByWeight());
  }

}
