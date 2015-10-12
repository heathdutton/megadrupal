<?php

/**
 * @file
 * Contains the basic customer profile field handler.
 */

/**
 * Field handler to provide simple renderer that allows linking to a profile.
 */
class user_addressbook_handler_field_user_address extends views_handler_field {
  function init(&$view, &$options) {
    parent::init($view, $options);

    if (!empty($this->options['link_to_user_address'])) {
      $this->additional_fields['address_id'] = 'address_id';
      $this->additional_fields['uid'] = 'uid';
    }
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['link_to_user_address'] = array('default' => TRUE);

    return $options;
  }

  /**
   * Provide the link to profile option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['link_to_user_address'] = array(
      '#title' => t("Link this field to the profile's administrative view page"),
      '#description' => t('This will override any other link you have set.'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_user_address']),
    );
  }

  /**
   * Render whatever the data is as a link to the customer profile.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {
    if (!empty($this->options['link_to_user_address']) && $data !== NULL && $data !== '') {
      $user_address = user_address_load($this->get_value($values, 'address_id'));
      $user_address_uri = entity_uri('user_address', $user_address);
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = $user_address_uri['path'];
    }

    return $data;
  }

  function render($values) {
    $value = $this->get_value($values);
    return $this->render_link($this->sanitize_value($value), $values);
  }
}
