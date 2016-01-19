<?php

/**
 * Field handler to present the card expiration date.
 */
class commerce_vantiv_handler_field_card_exp extends views_handler_field_entity {

  function render($values) {
    if (!isset($this->entities[$this->view->row_index])) {
      return '';
    }

    $card = $this->entities[$this->view->row_index];
    $wrapper = entity_metadata_wrapper('commerce_cardonfile', $card);
    $output = $wrapper->card_exp->value();
    if (!commerce_cardonfile_validate_card_expiration($card)) {
      // The card has expired, add visual cues.
      $output = '<div class="card-expired">' . $output . '</div>';
      $output .= '<div class="card-expired-description">' . t('Expired') . '</div>';
    }
    switch ($wrapper->card_type->value()) {
      case 'Checking':
        $output = '';
        break;
      case 'Savings':
        $output = '';
        break;
      case 'Corporate':
        $output = '';
        break;
      case 'Corp Savings':
        $output = '';
        break;
    }
    return $output;
  }
}
