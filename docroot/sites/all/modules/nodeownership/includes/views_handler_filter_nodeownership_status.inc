<?php

/**
 * Simple filter to handle equal to / not equal to filters
 *
 * @ingroup views_filter_handlers
 */
class views_handler_filter_nodeownership_status extends views_handler_filter_equality {

  /**
   * Provide a select list for value selection.
   */
  function value_form(&$form, &$form_state) {
    parent::value_form($form, $form_state);

    $form['value'] = array(
      '#type' => 'select',
      '#title' => t('Status'),
      '#options' => array(
        0 => t('Pending'),
        1 => t('Approved'),
        2 => t('Declined'),
      ),
      '#default_value' => $this->value,
      '#required' => FALSE,
    );
  }
}

