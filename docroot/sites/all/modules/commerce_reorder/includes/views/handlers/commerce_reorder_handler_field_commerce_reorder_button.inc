<?php
/**
 *
 */

class commerce_reorder_handler_field_commerce_reorder_button extends views_handler_field {

  function option_definition() {
    $options = parent::option_definition();
    $options['reorder_button_label'] = array('default' => 'Reorder', 'translatable' => TRUE);
    $options['redirect'] = array('default' => TRUE);
    $options['redirect_url'] = array('default' => 'cart');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['reorder_button_label'] = array(
      '#type' => 'textfield',
      '#title' => t('Reorder button label.'),
      '#default_value' => $this->options['reorder_button_label'],
    );
    $form['redirect'] = array(
      '#type' => 'checkbox',
      '#title' => t('Redirect after reorder.'),
      '#default_value' => $this->options['redirect'],

    );
    $form['redirect_url'] = array(
      '#type' => 'textfield',
      '#title' => t('Target url to redirect after reorder.'),
      '#default_value' => $this->options['redirect_url'],
      '#dependency' => array('edit-options-redirect' => array(TRUE)),
    );
  }

  function construct() {
    parent::construct();

    $this->additional_fields['order_id'] = 'order_id';

    $this->real_field = 'order_id';
  }

  function render($values) {
    return '<!--form-item-' . $this->options['id'] . '--' . $this->view->row_index . '-->';
  }

  /**
   * Returns the form which replaces the placeholder from render().
   */
  function views_form(&$form, &$form_state) {
    // The view is empty, abort.
    if (empty($this->view->result)) {
      return;
    }

    $form[$this->options['id']] = array(
      '#tree' => TRUE,
    );

    // At this point, the query has already been run, so we can access the results
    // in order to get the base key value (for example, nid for nodes).
    foreach ($this->view->result as $row_id => $row) {
      $order_id = $this->get_value($row, 'order_id');

      $form[$this->options['id']][$row_id] = array(
        '#type' => 'submit',
        '#value' => isset($this->options['reorder_button_label']) ? $this->options['reorder_button_label'] : t('Reorder'),
        '#name' => 'reorder-line-item-' . $row_id,
        '#order_id' => $order_id,
      );
    }
  }

  function views_form_submit($form, &$form_state) {
    $field_name = $this->options['id'];

    foreach (element_children($form[$field_name]) as $row_id) {
      if ($form_state['triggering_element']['#name'] == 'reorder-line-item-' . $row_id) {
        $order = commerce_order_load($form[$field_name][$row_id]['#order_id']);
        commerce_reorder_helper($order);
      }
    }

    if (strpos($form_state['triggering_element']['#name'], 'reorder-line-item-') === 0) {
      drupal_set_message(t('Order copied to your cart.'));

      // Check the redirect option set in the view and where to redirect.
      if ($this->options['redirect']) {
        $url = isset($this->options['redirect_url']) ? $this->options['redirect_url'] : 'cart';
        drupal_goto(check_url($url));
      }
    }
  }
}
