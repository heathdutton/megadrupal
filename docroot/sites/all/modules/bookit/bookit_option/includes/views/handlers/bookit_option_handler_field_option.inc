<?php

/**
 * @file
 * Contains the basic option field handler.
 */

/**
 * Field handler to provide simple renderer that allows linking to a option.
 */
class bookit_option_handler_field_option extends views_handler_field {
  function init(&$view, &$options) {
    parent::init($view, $options);

    if (!empty($this->options['link_to_option'])) {
      $this->additional_fields['option_id'] = 'option_id';
    }
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['link_to_option'] = array('default' => FALSE);

    return $options;
  }

  /**
   * Provide the link to option option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['link_to_option'] = array(
      '#title' => t("Link this field to the option's administrative view page"),
      '#description' => t('This will override any other link you have set.'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_option']),
    );
  }

  /**
   * Render whatever the data is as a link to the option.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {
    if (!empty($this->options['link_to_option']) && $data !== NULL && $data !== '') {
      $option_id = $this->get_value($values, 'option_id');
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = 'admin/bookit/config/options/' . $option_id;
    }

    return $data;
  }

  function render($values) {
    $value = $this->get_value($values);
    return $this->render_link($this->sanitize_value($value), $values);
  }
}
