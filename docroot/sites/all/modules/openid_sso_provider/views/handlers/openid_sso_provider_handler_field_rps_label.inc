<?php

/**
 * @file
 * Contains the basic relying party label field handler.
 */

/**
 * Field handler to provide simple renderer that allows linking to a relying party's realm.
 */
class openid_sso_provider_handler_field_rps_label extends views_handler_field {
  function init(&$view, &$options) {
    parent::init($view, $options);

    if (!empty($this->options['link_to_realm'])) {
      $this->additional_fields['realm'] = 'realm';
    }
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['link_to_realm'] = array('default' => FALSE);

    return $options;
  }

  /**
   * Provide the link to relying party link option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['link_to_realm'] = array(
      '#title' => t("Link this field to the OpenID relying party"),
      '#description' => t('This will override any other link you have set.'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_realm']),
    );
  }

  /**
   * Render whatever the data is as a link to the relying party.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {
    if (!empty($this->options['link_to_realm']) && $data !== NULL && $data !== '') {
      $realm = $this->get_value($values, 'realm');
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = $realm;
    }

    return $data;
  }

  function render($values) {
    $value = $this->get_value($values);
    return $this->render_link($this->sanitize_value($value), $values);
  }
}
