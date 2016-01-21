<?php

/**
 * @file
 * Definition of views_handler_field_custom_payment_link_cancel.
 */

/**
 * Field handler to present a link to cancel a custom payment.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_custom_payment_link_cancel extends views_handler_field {

  function construct() {
    parent::construct();
    $this->additional_fields['pid'] = 'pid';
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['text'] = array('default' => '', 'translatable' => TRUE);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );
    parent::options_form($form, $form_state);
  }

  // An example of field level access control.
  function access() {
    return user_access('administer paypal roles');
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $value = $this->get_value($values, 'pid');
    return $this->render_link($this->sanitize_value($value), $values);
  }

  function render_link($data, $values) {
    $pid = $values->{$this->aliases['pid']};

    if ($pid) {
      $this->options['alter']['make_link'] = TRUE;

      $text = !empty($this->options['text']) ? $this->options['text'] : t('Cancel custom payment');

      $this->options['alter']['path'] = "admin/paypal-roles/custom-payments/manage/$pid/delete";
      $this->options['alter']['query'] = drupal_get_destination();

      return $text;
    }
  }

}
