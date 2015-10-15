<?php

class bookit_item_handler_field_booking_button extends views_handler_field {

  function option_definition() {
    $options = parent::option_definition();

    $options['text'] = array('default' => t('Book Now'), 'translatable' => TRUE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );

    parent::options_form($form, $form_state);
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
      $item_id = $this->get_value($row);

      // By default status is FALSE.
      $status = FALSE;

      // Get date by item id.
      if ($date = bookit_calendar_date_by_item_id($item_id)) {
        // Get item status by date.
        $status = bookit_calendar_item_status($item_id, $date['min'], $date['max']);
      }

      $form[$this->options['id']][$row_id] = array(
        '#type' => 'submit',
        '#value' => $this->options['text'],
        '#name' => 'op_' . $item_id,
        '#row_id' => $row_id,
        '#item_id' => $item_id,
        '#disabled' => !$status,
      );
    }
  }

  function views_form_validate($form, &$form_state) {
    // Retrieve the item_id.
    $item_id = $form_state['triggering_element']['#item_id'];

    $status = FALSE;

    // Get date by item id.
    if ($date = bookit_calendar_date_by_item_id($item_id)) {
      // Get item status by date.
      $status = bookit_calendar_item_status($item_id, $date['min'], $date['max']);
    }

    // If status is false dispay error.
    if(!$status) {
      $bookit_item = bookit_item_load($item_id);
      form_set_error($form_state['triggering_element']['#name'], t('@title is not available.', array('@title' => $bookit_item->title)));
    }
  }

  function views_form_submit($form, &$form_state) {
    global $user;

    // Only submit if triggering_element is this button.
    if ($form_state['triggering_element']['#parents'][0] != $this->options['id']) {
      return;
    }

    // Retrieve row_id.
    $row_id = $form_state['triggering_element']['#row_id'];

    // Default is 1.
    $quantity = 1;

    // If isset booking quantity element try to get its value.
    if(isset($form_state['values']['booking_quantity'][$row_id]) and is_numeric($form_state['values']['booking_quantity'][$row_id])) {
      $quantity = $form_state['values']['booking_quantity'][$row_id];
    }

    // Retrieve the item_id.
    $item_id = $form_state['triggering_element']['#item_id'];

    // Get current booking date.
    $date = bookit_calendar_date_by_item_id($item_id);

    // Create a new booking line item.
    $line_item = bookit_line_item_new($item_id, $date, $quantity);

    // Calculate the line item's sell price for first time.
    rules_invoke_event('commerce_product_calculate_sell_price', $line_item);

    // Add the product to cart.
    // TODO: Make cart combination optional.
    commerce_cart_product_add($user->uid, $line_item);
  }
}
