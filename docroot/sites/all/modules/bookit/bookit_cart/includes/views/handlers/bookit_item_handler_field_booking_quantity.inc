<?php

class bookit_item_handler_field_booking_quantity extends views_handler_field {

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
        '#type' => 'textfield',
        '#title' => t('Quantity'),
        '#title_display' => 'invisible',
        '#element_validate' => array('element_validate_integer_positive'),
        '#default_value' => 1,
        '#size' => 3,
        '#disabled' => !$status,
      );
    }
  }

  function views_form_validate($form, &$form_state) {
    // If bookit_availability is enabled then validate the quantity.
    if(module_exists('bookit_availability')) {
      // Retrieve the row_id.
      $row_id = $form_state['triggering_element']['#row_id'];

      // Retrieve bookit_item.
      $item_id = $form_state['triggering_element']['#item_id'];

      // Get the quantity.
      $quantity = $form_state['values'][$this->options['id']][$row_id];

      // Get date.
      $date = bookit_calendar_date_by_item_id($item_id);

      // Get the current availability.
      $query = db_select('bookit_availability', 'a')
        ->condition('a.item_id', $item_id)
        ->condition('a.date', $date['min'], '>=')
        ->condition('a.date', $date['max'], '<');
      $query->addExpression('MIN(a.availability)');
      $availability = $query->execute()->fetchField();

      if($quantity > $availability) {
        $bookit_item = bookit_item_load($item_id);
        form_set_error($form_state['triggering_element']['#name'], t('The quantity of @quantity is not available for @title. Try less items.', array('@title' => $bookit_item->title, '@quantity' => $quantity)));
      }
    }
  }
}
