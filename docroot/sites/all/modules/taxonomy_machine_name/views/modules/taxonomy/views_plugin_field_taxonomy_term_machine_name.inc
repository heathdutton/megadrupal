<?php

/**
 * Field handler to provide term machine_name token for using in url.
 */
class views_plugin_field_taxonomy_term_machine_name extends views_handler_field {
  /**
   * {@inheritdoc}
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['convert_underscores'] = array('default' => FALSE, 'bool' => TRUE);

    return $options;
  }

  /**
   * {@inheritdoc}
   */
  function options_form(&$form, &$form_state) {
    $form['convert_underscores'] = array(
      '#title'         => t('Convert underscores in term names to hyphens'),
      '#description'   => t('This allows links to work with Views taxonomy term arguments.'),
      '#type'          => 'checkbox',
      '#default_value' => !empty($this->options['convert_underscores']),
    );
    parent::options_form($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  function render($values) {
    $value = $this->get_value($values);
    $value = $this->sanitize_value($value);

    if (!empty($this->options['convert_underscores'])) {
      $value = str_replace('_', '-', $value);
    }

    return $value;
  }
}
