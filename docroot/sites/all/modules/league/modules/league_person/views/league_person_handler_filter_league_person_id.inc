<?php
/**
* @file
* Provide node nid argument handler.
*/
/**
* Argument handler to accept a person id.
*/
class league_person_handler_filter_league_person_id extends views_handler_filter_equality {
  function value_form(&$form, &$form_state) {
    parent::value_form($form, $form_state);
    $form['value'] = array( 
// TODO: Change to autocomplete
      '#type' => 'select',
      '#title' => t('Player'),
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
