<?php

/**
 * @file
 * Definition of views_handler_field_payment_link_status.
 */

/**
 * Field handler to present a link to change status of a payment.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_payment_link_status extends views_handler_field {

  function construct() {
    parent::construct();

    $this->additional_fields['txn_id'] = 'txn_id';
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['text_active'] = array('default' => '', 'translatable' => TRUE);
    $options['text_inactive'] = array('default' => '', 'translatable' => TRUE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    $form['text_active'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display (when status is active)'),
      '#default_value' => $this->options['text_active'],
    );

    $form['text_inactive'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display (when status is inactive)'),
      '#default_value' => $this->options['text_inactive'],
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
    $value = $this->get_value($values, 'txn_id');
    return $this->render_link($this->sanitize_value($value), $values);
  }

  function render_link($data, $values) {
    $txn_id = $values->{$this->aliases['txn_id']};

    if ($txn_id) {
      $pm = paypal_roles_payments_load($txn_id);

      $this->options['alter']['make_link'] = TRUE;

      if (isset($pm->cancelled) && (bool) $pm->cancelled) {
        $text = !empty($this->options['text_inactive']) ? $this->options['text_inactive'] : t('Uncancel');
        $this->options['alter']['path'] = "admin/paypal-roles/payments/manage/$txn_id/uncancel";
      }
      else {
        $text = !empty($this->options['text_active']) ? $this->options['text_active'] : t('Cancel');
        $this->options['alter']['path'] = "admin/paypal-roles/payments/manage/$txn_id/cancel";
      }

      $this->options['alter']['query'] = drupal_get_destination();

      return $text;
    }
  }

}
