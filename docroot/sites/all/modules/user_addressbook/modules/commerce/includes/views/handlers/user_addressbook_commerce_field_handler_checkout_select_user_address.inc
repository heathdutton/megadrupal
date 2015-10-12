<?php

/**
 * @file
 *
 * A Views' field handler for selecting the user address.
 *
 */
class user_addressbook_commerce_field_handler_checkout_select_user_address extends views_handler_field {

  function construct() {
    parent::construct();
    $this->additional_fields['address_id'] = 'address_id';
    $this->additional_fields['uid'] = 'uid';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  /**
   * Render the field contents.
   *
   * @param $values
   * @return string
   */
  function render($values) {
    // Render a Views form item placeholder.
    return '<!--form-item-' . $this->options['id'] . '--' . $this->view->row_index . '-->';
  }

  /**
   * Add to and alter the form.
   */
  function views_form(&$form, &$form_state) {
    // Create a container for our replacements
    $form[$this->options['id']] = array(
      '#type' => 'container',
      '#tree' => TRUE,
    );
    // Iterate over the result and add our replacement fields to the form.
    foreach($this->view->result as $row_index => $row) {
      $uid = $row->{$this->aliases['uid']};
      $user_default_address = user_addressbook_get_default_addresses($uid, $this->view->args[1]);
      $order = commerce_order_load($this->view->args[3]);
      $form[$this->options['id']][$row_index] = array(
        '#type' => 'container',
        '#tree' => TRUE,
      );
      $form[$this->options['id']][$row_index]['value'] = array(
        '#type' => 'value',
        '#value' => $row->{$this->aliases['address_id']},
      );
      // Add submit button to the form.  This array convention
      // corresponds to the placeholder HTML comment syntax.
      $form[$this->options['id']][$row_index]['select_button'] = array(
        '#type' => 'button',
        '#return_value' => $row->{$this->aliases['address_id']},
        '#disabled' => ($user_default_address == $row->{$this->aliases['address_id']}),
        '#value' => t('Select'),
        '#executes_submit_callback' => TRUE,
        '#name' => $row->{$this->aliases['address_id']},
        '#submit' => array('user_addressbook_commerce_field_handler_checkout_select_user_address_submit'),
        '#ajax' => array(
          'callback' => 'user_addressbook_ajax_form_refresh',
          'wrapper' => 'user-addressbook-ajax-wrapper',
        ),
      );
    }
    $this->options['label'] = '';

    // Submit to the current page if not a page display.
    if ($this->view->display_handler->plugin_name != 'page') {
      $form['#action'] = current_path();
    }
  }

}
