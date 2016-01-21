<?php

/**
 * @file
 * Contains a Views field handler to display a Bookable unit booking form.
 */

class rooms_views_handler_booking_field extends views_handler_field {

  /**
   * Implements views_handler_field#construct().
   */
  function construct() {
    parent::construct();

    $this->additional_fields['unit_id'] = 'unit_id';
  }

  /**
   * Implements views_handler_field#option_definition().
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['text'] = array('default' => 'Book This', 'translatable' => TRUE);
    return $options;
  }

  /**
   * Implements views_handler_field#options_form().
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#description' => t('This is the text to display in the Booking button.'),
      '#default_value' => $this->options['text'],
    );
  }

  /**
   * Implements views_handler_field#query().
   */
  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  /**
   * Implements views_handler_field#render().
   */
  function render($values) {
    if (isset($this->view->filter['availability'])) {
      $start_date = $this->view->filter['availability']->value['rooms_start_date'];
      $end_date = $this->view->filter['availability']->value['rooms_end_date'];

      return drupal_get_form('rooms_views_booking_button_form', $start_date, $end_date, $values->{$this->aliases['unit_id']}, $this->options['text']);
    }
    else {
      return t('This field requires the Rooms Availability filter enabled');
    }
  }
}
