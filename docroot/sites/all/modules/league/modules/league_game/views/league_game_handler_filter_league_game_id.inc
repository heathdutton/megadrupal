<?php
/**
* @file
* Provide node nid argument handler.
*/
/**
* Argument handler to accept a person id.
*/
class league_game_handler_filter_league_game_id extends views_handler_filter_equality {
  function value_form(&$form, &$form_state) {
    parent::value_form($form, $form_state);
    $form['value'] = array( 
      '#type' => 'select',
      '#title' => t('Games'),
      '#options' => league_game_get_titles(),
      '#default_value' => $this->value,
      '#required' => FALSE,
    );
  }
}
