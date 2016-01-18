<?php

/**
 * @file
 * Provides field markup handler for views integration for the quail api.
 */

/**
 * A handler for the quail_api_node problem "element" field.
 *
 * This "element" field is called otherwise called a problem snippet.
 */
class quail_api_handler_field_markup extends views_handler_field {
  function option_definition() {
    $options = parent::option_definition();

    $options['formatter'] = array(
      'default' => filter_fallback_format(),
    );

    return $options;
  }

  function options_form(&$form, &$form_state) {
    $filter_formats = filter_formats();
    $formatters = array();

    foreach ((array) $filter_formats as $filter_format => $filter_format_settings) {
      $formatters[$filter_format] = $filter_format_settings->name;
    }

    $form['formatter'] = array(
      '#type' => 'select',
      '#title' => t("Formatter"),
      '#options' => $formatters,
      '#default_value' => $this->options['formatter'],
    );

    parent::options_form($form, $form_state);
  }

  function render($values) {
    $format = $this->options['formatter'];

    $this->format = $format;

    if (empty($format)) {
      return $this->sanitize_value($values);
    }

    $value = $this->get_value($values);

    return check_markup($value, $format);
  }

  function element_type($none_supported = FALSE, $default_empty = FALSE, $inline = FALSE) {
    if ($inline) {
      return 'span';
    }

    if (isset($this->definition['element type'])) {
      return $this->definition['element type'];
    }

    return 'div';
  }
}
