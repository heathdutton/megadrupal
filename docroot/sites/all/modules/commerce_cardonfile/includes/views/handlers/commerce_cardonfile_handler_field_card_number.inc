<?php

/**
 * Field handler to present the truncated card number with optional formatting.
 */
class commerce_cardonfile_handler_field_card_number extends views_handler_field_entity {

  function option_definition() {
    $options = parent::option_definition();
    $options['format'] = array('default' => TRUE);

    return $options;
  }

  /**
   * Provide the link to card option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['format'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display formatted'),
      '#description' => t('If the truncated card number is "1234", the output will be "XXXX-XXXX-XXXX-1234".'),
      '#default_value' => $this->options['format'],
    );
  }

  function get_value($values, $field = NULL) {
    if (!isset($this->entities[$this->view->row_index])) {
      return '';
    }

    $card = $this->entities[$this->view->row_index];
    if ($this->options['format']) {
      return commerce_cardonfile_format_credit_card_number($card->card_number, $card->card_type);
    }
    else {
      return $card->card_number;
    }
  }
}
