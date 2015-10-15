<?php

/**
 * @file
 * Definition of views_handler_filter_equality.
 */

/**
 * Simple filter to handle equal to / not equal to filters
 *
 * @ingroup views_filter_handlers
 */
class oz_postcodes_views_handler_filter_state extends views_handler_filter_many_to_one {
  
  function option_definition() {
    $options = parent::option_definition();
    $options['expose']['contains']['sort_states'] = array('default' => 0);
    $options['expose']['contains']['long_states'] = array('default' => FALSE);
    return $options;
  }
  
  /**
   * Provide default options for exposed filters.
   */
  function expose_options() {
    parent::expose_options();
    $this->options['expose']['sort_states'] = 0;
    $this->options['expose']['long_states'] = FALSE;
  }
  
  function get_value_options() {
    $states = _oz_postecodes_get_states();
    if ($this->options['expose']['long_states']) {
      $states = array(
        'ACT' => 'Australian Capital Territory',
        'NSW' => 'New South Whales',
        'VIC' => 'Victoria',
        'QLD' => 'Queensland',
        'SA' => 'South Australia',
        'WA' => 'Western Australia',
        'TAS' => 'Tasmania',
        'NT' => 'Northern Territory',
      );
    }
    if ($this->options['expose']['sort_states'] == 1) {
      asort($states);
    } elseif($this->options['expose']['sort_states'] == 2) {
      arsort($states);
    }
    $this->value_options = $states;
  }
  
  function expose_form(&$form, &$form_state) {
    $form['expose']['sort_states'] = array(
      '#title' => t('Sort states'),
      '#type' => 'select',
      '#options' => array(
        'none',
        'Ascending',
        'Descending'
      ),
      '#default_value' => $this->options['expose']['sort_states'],
    );
    $form['expose']['long_states'] = array(
      '#title' => t('Long state names'),
      '#type' => 'checkbox',
      '#description' => t('Show long state names. eg. Victoria instead of VIC.'),
      '#default_value' => $this->options['expose']['long_states'],
    );
    parent::expose_form($form, $form_state);
  }
}
