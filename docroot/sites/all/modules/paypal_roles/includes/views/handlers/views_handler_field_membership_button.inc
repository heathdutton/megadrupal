<?php

/**
 * @file
 * Definition of views_handler_field_membership_button.
 */

/**
 * Render a field as a numeric value.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_membership_button extends views_handler_field {

  function option_definition() {
    $options = parent::option_definition();

    $options['button_text'] = array('default' => 'Buy', 'translatable' => TRUE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    $form['button_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Button Text'),
      '#default_value' => $this->options['button_text'],
      '#description' => t('Text to put on the checkout button'),
    );

    parent::options_form($form, $form_state);
  }

  function render($values) {
    if($machine_name = $values->machine_name){
      $formid = 'paypal_roles_block_form_' . $machine_name;
      $form = drupal_get_form($formid);

      unset($form['desctext']);
      unset($form['amounttext']);

      $form['actions']['submit']['#value'] = $this->sanitize_value($this->options['button_text'], 'xss');

      return render($form);
    } else {
      return "Please activate the Machine Name field and hide it from display to show the Paypal button.";
    }
  }
}
