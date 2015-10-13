<?php

/**
 * @file
 * Definition of views_handler_field_membership_link_cancel.
 */

/**
 * Field handler to present a link to membership cancel.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_membership_link_cancel extends views_handler_field {

  function construct() {
    parent::construct();
    $this->additional_fields['machine_name'] = 'machine_name';
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
    $value = $this->get_value($values, 'machine_name');
    return $this->render_link($this->sanitize_value($value), $values);
  }

  function render_link($data, $values) {
    $mname = $values->{$this->aliases['machine_name']};

    if ($mname) {
      $pmts = paypal_roles_payments_load_conditional(array(
        'item_id' => $mname,
      ));

      $pmts_custom = paypal_roles_custom_payments_load_conditional(array(
        'item_id' => $mname,
      ));

      if (count($pmts) > 0 || count($pmts_custom) > 0) {
        return "";
      }

      $this->options['alter']['make_link'] = TRUE;

      $text = !empty($this->options['text']) ? $this->options['text'] : t('Cancel membership');

      $this->options['alter']['path'] = "admin/paypal-roles/memberships/manage/$mname/delete";
      $this->options['alter']['query'] = drupal_get_destination();

      return $text;
    }
  }

}
