<?php

/**
 * @file
 * Definition of views_handler_field_membership_link_edit.
 */

/**
 * Field handler to present a link to membership edit.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_membership_link_edit extends views_handler_field {

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
      $this->options['alter']['make_link'] = TRUE;

      $text = !empty($this->options['text']) ? $this->options['text'] : t('Edit membership');

      $this->options['alter']['path'] = "admin/paypal-roles/memberships/manage/$mname/edit";
      $this->options['alter']['query'] = drupal_get_destination();

      return $text;
    }
  }

}
