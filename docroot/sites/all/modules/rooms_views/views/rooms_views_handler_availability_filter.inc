<?php

/**
 * A handler to filter a view using Rooms Availability filters.
 *
 * @ingroup views_filter_handlers
 */
class rooms_views_handler_availability_filter extends views_php_handler_filter {

  /**
   * Implements views_handler#can_expose().
   */
  function can_expose() { return TRUE; }

  /**
   * Implements views_object#admin_summary().
   */
  function admin_summary() {
    return t('Availability filter');
  }

  /**
   * Implements views_object#option_definition().
   */
  function option_definition() {
    $options = parent::option_definition();
    $start_date = new Datetime();
    $end_date = new Datetime('+1 week');
    $options['value'] = array(
      'default' => array(
        'rooms_start_date' => $start_date,
        'rooms_end_date' => $end_date,
      )
    );
    $options['php_filter'] = array('default' => 1);
    return $options;
  }

  /**
   * Implements views_object#options_form().
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    unset($form['use_php_setup']);
    unset($form['php_setup']);
    unset($form['php_filter']);
    unset($form['expose']['multiple']);
    $form['expose']['required']['#value'] = 1;
    $form['expose']['required']['#disabled'] = 1;
  }

  /**
   * Implements views_object#value_form().
   */
  function value_form(&$form, &$form_state) {
    $form['value'] = array(
      '#type' => 'fieldset',
    );

    $form['value'] += rooms_date_range_fields();

    $form['value']['rooms_start_date']['#default_value'] = $this->value['rooms_start_date']->format('Y-m-d');
    $form['value']['rooms_end_date']['#default_value'] = $this->value['rooms_end_date']->format('Y-m-d');
  }

  /**
   * Implements views_object#value_validate().
   */
  function value_validate($form, &$form_state) {
    form_set_value($form['value']['rooms_start_date'], new DateTime($form_state['values']['options']['value']['rooms_start_date']), $form_state);
    form_set_value($form['value']['rooms_end_date'], new DateTime($form_state['values']['options']['value']['rooms_end_date']), $form_state);
  }

  /**
   * Implements views_object#accept_exposed_input().
   */
  function accept_exposed_input($input) {
    $start_date = new DateTime($input['rooms_start_date']);
    $end_date = new DateTime($input['rooms_end_date']);
    if ($end_date > $start_date) {
      $this->value['rooms_start_date'] = $start_date;
      $this->value['rooms_end_date'] = $end_date;
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Performs the filtering operation.
   *
   * @see views_php_views_post_execute()
   */
  function php_post_execute() {
    // Evaluate the Bookable units availability.
    if (!empty($this->options['php_filter'])) {
      $agent = new AvailabilityAgent($this->value['rooms_start_date'], $this->value['rooms_end_date']);

      $ids = array();
      $units = $agent->checkAvailability();

      if (!is_array($units)) {
        $this->view->result = array();
        return;
      }

      foreach ($units as $units_per_type) {
        foreach ($units_per_type as $units) {
          foreach ($units as $id => $unit) {
            $ids[] = $id;
          }
        }
      }
      foreach ($this->view->result as $i => $row) {
        if (!in_array($row->unit_id, $ids)) {
          unset($this->view->result[$i]);
        }
      }
    }
  }
}
