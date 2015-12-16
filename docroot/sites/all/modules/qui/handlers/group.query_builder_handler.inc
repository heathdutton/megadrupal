<?php
/**
 * @file
 * Contains the Group handler.
 */

/**
 * Class Qui_Group_Handler
 */
class Qui_Group_Handler extends Qui_Query_Builder_Handler {

  public $name = 'group';

  /**
   * Enable Groups if the select table is set.
   */
  public function preBuild() {
    if (!empty($this->getHandler('qui_select_table')->defaults)) {
      $this->enabled = TRUE;
    }
  }

  /**
   * Create the group elements.
   */
  public function build() {

    $this->elements['query']['extras']['group-wrapper'] = array(
      '#type' => 'fieldset',
      '#title' => t('Group By'),
      '#collapsible' => TRUE,
      '#collapsed' => $this->collapsedState(),
      '#attributes' => array('class' => array('group-fieldset')),
    );

    if (!$this->collapsedState()) {
      $this->getHandler('qui_range')->elements['query']['extras']['#collapsed'] = FALSE;
    }

    $wrapper = &$this->elements['query']['extras']['group-wrapper'];

    $wrapper['help'] = array(
      '#markup' => $this->helpLink('https://www.drupal.org/node/1848364', t('Learn about Grouping.')),
    );

    $wrapper[$this->name] = array('#tree' => TRUE);

    foreach ($this->defaults as $key => $group) {
      $wrapper[$this->name][$key] = array(
        '#type' => 'fieldset',
        '#attributes' => array('class' => array('inline-items')),
      );
      $wrapper[$this->name][$key]['column'] = array(
        '#type' => 'select',
        '#options' => $this->getFormattedActiveColumns(),
        '#empty_option' => '-- select a table --',
        '#attributes' => array('class' => array('qui-autocomplete')),
        '#default_value' => $group['column'],
      );
      $wrapper[$this->name][$key]['delete_group'] = array(
        '#type' => 'button',
        '#value' => t('Delete'),
        '#name' => 'delete_group_' . $key,
        '#button_action' => 'deleteGroup',
        '#delete_group' => $key,
        '#ajax' => $this->ajaxSettings(),
        '#attributes' => array('class' => array('delete-button')),
        '#validate' => array(),
      );
    }

    $wrapper['add_group'] = array(
      '#type' => 'button',
      '#value' => t('Add group'),
      '#name' => 'add_group',
      '#button_action' => 'addGroup',
      '#ajax' => $this->ajaxSettings(),
    );

  }

  /**
   * Get the collapsed state for the fieldset.
   */
  private function collapsedState() {
    if (in_array($this->buttonAction(), array('addGroup', 'deleteGroup'))) {
      return FALSE;
    }
    return TRUE;
  }

  /**
   * Set default values for groups.
   */
  public function setDefaults() {
    $this->defaults = $this->getDefaultValues($this->name);
  }

  /**
   * Delete a group.
   */
  public function deleteGroup() {
    $clicked = $this->clickedButton();
    unset($this->defaults[$clicked['#delete_group']]);
    unset($this->controller->form_state['values'][$this->name][$clicked['#delete_group']]);
  }

  /**
   * Add a group.
   */
  public function addGroup() {
    $this->defaults[] = array();
  }

  /**
   * Validate group values.
   */
  public function validate() {
    foreach ($this->defaults as $key => $group) {
      if (empty($group['column'])) {
        form_set_error("group][$key][column", t('Group @key column is required.', array('@key' => $key)));
      }
    }
  }

  /**
   * Save group values.
   */
  public function submit() {
    foreach ($this->defaults as $key => &$group) {
      unset($group['delete_group']);
    }
    $this->report->query->group = array_values($this->defaults);
  }

}
