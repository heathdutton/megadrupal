<?php

/**
 * @file
 * Field handler to present a link to a socolissimo flexibility rate.
 */

class commerce_socolissimo_handler_field_rate_link extends views_handler_field {
  /**
   * Class contructor.
   */
  function construct() {
    parent::construct();

    $this->additional_fields['rate_id'] = 'rate_id';
    $this->additional_fields['name'] = 'name';
  }

  /**
   * Handler option definition.
   *
   * @return array
   *   Returns default options for this handler.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['text'] = array('default' => '', 'translatable' => TRUE);

    return $options;
  }

  /**
   * Exposes a form for the handler options.
   *
   * @param array $form
   *   The form array.
   * @param array $form_state
   *   The form state array.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );
  }

  /**
   * Query method.
   */
  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  /**
   * Render a link.
   *
   * @param array $values
   *   View's values.
   *
   * @return string
   *   Return the link.
   */
  function render($values) {
    $text = !empty($this->options['text']) ? $this->options['text'] : t('view');
    $rate_id = $this->get_value($values, 'rate_id');
    $name = $this->get_value($values, 'name');

    $service_name_url = 'socolissimo-flexibility' . '-' . strtr($name, '_', '-');

    return l($text, 'admin/commerce/config/shipping/services/' . $service_name_url . '/rate/' . $rate_id, array('query' => drupal_get_destination()));
  }
}
