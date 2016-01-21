<?php

/**
 * @file
 * Contains the field handler for render variable name.
 */

/**
 * Field handler to provide variable name.
 */
class variable_log_handler_field_name extends views_handler_field {

  /**
   * Init the handler with necessary data.
   */
  public function init(&$view, &$options) {
    parent::init($view, $options);

    if (!empty($this->options['link_to_log'])) {
      $this->additional_fields['id'] = array('table' => 'variable_log', 'field' => 'id');
    }
  }

  /**
   * Information about options.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['link_to_log'] = array('default' => 'none');
    return $options;
  }

  /**
   * Build the options form.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['link_to_log'] = array(
      '#type' => 'checkbox',
      '#title' => t('Link this field to the diffs of variable'),
      '#default_value' => !empty($this->options['link_to_log']),
      '#description' => t("Enable to override this field's links."),
    );

    parent::options_form($form, $form_state);
  }

  /**
   * Render the field.
   */
  public function render($values) {
    $value = $this->get_value($values);
    return $this->render_link($this->sanitize_value($value), $values);
  }

  /**
   * Render the field as link.
   */
  public function render_link($data, $values) {
    if (!empty($this->options['link_to_log']) && !empty($this->additional_fields['id'])) {
      $id = $this->get_value($values, 'id');
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = 'admin/reports/variable-log/' . $id;
    }

    return $data;
  }
}
