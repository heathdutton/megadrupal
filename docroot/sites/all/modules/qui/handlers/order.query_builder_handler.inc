<?php
/**
 * @file
 * Contains the Order handler.
 */

/**
 * Class Qui_Order_Handler
 */
class Qui_Order_Handler extends Qui_Query_Builder_Handler {

  public $name = 'order';

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
    if (in_array($this->buttonAction(), array('addOrder', 'deleteOrder'))) {
      return FALSE;
    }
    if (isset($this->controller->form_state['triggering_element']['#name']) && strpos($this->controller->form_state['triggering_element']['#name'], 'order[') === 0) {
      return FALSE;
    }
    return TRUE;
  }

  /**
   * Create the Order elements.
   */
  public function build() {

    $this->elements['query']['extras']['order-wrapper'] = array(
      '#type' => 'fieldset',
      '#title' => t('Order By'),
      '#collapsible' => TRUE,
      '#collapsed' => $this->collapsedState(),
    );

    if (!$this->collapsedState()) {
      $this->getHandler('qui_range')->elements['query']['extras']['#collapsed'] = FALSE;
    }

    $wrapper = &$this->elements['query']['extras']['order-wrapper'];

    $wrapper['help'] = array(
      '#markup' => $this->helpLink('https://www.drupal.org/node/1848360', t('Learn about Ordering.')),
    );

    $wrapper[$this->name]['#tree'] = TRUE;

    foreach ($this->defaults as $key => $order) {

      $wrapper[$this->name][$key] = array(
        '#type' => 'fieldset',
        '#attributes' => array('class' => array('inline-items')),
        '#weight' => is_numeric($order['weight']) ? $order['weight'] : $this->nextHighestWeight(),
        '#title' => $order['type'],
      );

      $wrapper[$this->name][$key]['type'] = array(
        '#type' => 'hidden',
        '#value' => $order['type'],
        '#suffix' => ucfirst($order['type']),
      );

      if ($order['type'] == 'column') {
        $wrapper[$this->name][$key]['column'] = array(
          '#type' => 'select',
          '#options' => $this->getFormattedActiveColumns(),
          '#title' => t('Column'),
          '#empty_option' => '-- select a column --',
          '#attributes' => array('class' => array('qui-autocomplete')),
          '#default_value' => $order['column'],
        );
      }
      elseif ($order['type'] == 'alias') {
        $wrapper[$this->name][$key]['column'] = array(
          '#type' => 'textfield',
          '#title' => t('Alias'),
          '#attributes' => array('placeholder' => t('Column alias')),
          '#default_value' => $order['column'],
        );
      }

      $wrapper[$this->name][$key]['direction'] = array(
        '#type' => 'select',
        '#title' => t('Direction'),
        '#default_value' => $order['direction'],
        '#options' => array('DESC' => t('Desc'), 'ASC' => t('Asc')),
      );

      $wrapper[$this->name][$key]['weight'] = array(
        '#type' => 'weight',
        '#attributes' => array('class' => array('order-weight')),
        '#title' => t('Weight'),
        '#default_value' => is_numeric($order['weight']) ? $order['weight'] : $this->nextHighestWeight(),
        '#delta' => self::WEIGHT_DELTA,
      );

      $wrapper[$this->name][$key]['delete_order'] = array(
        '#type' => 'button',
        '#value' => t('Delete'),
        '#name' => 'delete_order_' . $key,
        '#button_action' => 'deleteOrder',
        '#delete_order' => $key,
        '#ajax' => $this->ajaxSettings(),
        '#attributes' => array('class' => array('delete-button')),
        '#validate' => array(),
      );
    }

    $wrapper['add_order_column'] = array(
      '#type' => 'button',
      '#value' => t('Order By Column'),
      '#name' => 'add_order_column',
      '#button_action' => 'addOrder',
      '#order_type' => 'column',
      '#ajax' => $this->ajaxSettings(),
    );
    $wrapper['add_order_alias'] = array(
      '#type' => 'button',
      '#value' => t('Order By Alias'),
      '#name' => 'add_order_alias',
      '#order_type' => 'alias',
      '#button_action' => 'addOrder',
      '#ajax' => $this->ajaxSettings(),
    );

  }

  /**
   * Get the default Order values.
   */
  public function setDefaults() {
    $this->defaults = $this->getDefaultValues($this->name);
  }

  /**
   * Delete Order by.
   */
  public function deleteOrder() {
    $clicked = $this->clickedButton();
    unset($this->defaults[$clicked['#delete_order']]);
    unset($this->controller->form_state['values'][$this->name][$clicked['#delete_order']]);
  }

  /**
   * Add an Order by.
   */
  public function addOrder() {
    $clicked = $this->clickedButton();
    $this->defaults[] = array(
      'type' => $clicked['#order_type'],
    );
  }

  /**
   * Validate the order values.
   */
  public function validate() {
    foreach ($this->defaults as $key => $order) {
      if (empty($order['column'])) {
        form_set_error("order][$key][column", t('Order @key column is required.', array('@key' => $key)));
      }
    }
  }

  /**
   * Save the order values.
   */
  public function submit() {
    foreach ($this->defaults as $key => &$order) {
      unset($order['delete_order']);
    }
    $this->report->query->order = array_values($this->sortByWeight());
  }

}
