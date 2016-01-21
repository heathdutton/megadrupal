<?php


/**
 * @file
 * Definition of a field handler for Moodle grades.
 */

/**
 * Field handler that provides different ways to display a numeric grade.
 *
 * Definition terms:
 * - display_format default: the default display format.
 *
 * @ingroup views_field_handlers
 */
class moodle_views_handler_field_grade extends views_handler_field_numeric {

  /**
   * Overrides init().
   */
  public function init(&$view, &$options) {
    parent::init($view, $options);
    if ($this->options['display_format'] != 'raw') {
      $this->additional_fields['grademin'] = array(
        'table' => 'grade_items',
        'field' => 'grademin');
      $this->additional_fields['grademax'] = array(
        'table' => 'grade_items',
        'field' => 'grademax');
    }
    if ($this->options['display_format'] == 'letter') {
      $this->additional_fields['letter'] = array(
        'table' => 'grade_letters',
        'field' => 'letter');
    }
  }

  /**
   * Defines the "display format" option.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['display_format'] = array(
      'default' => isset($this->definition['display_format default']) ? $this->definition['display_format default'] : 'raw');
    return $options;
  }

  /**
   * Provides the "display format" form widget.
   */
  public function options_form(&$form, &$form_state) {
    $form['display_format'] = array(
      '#title' => t('Grade display format'),
      '#description' => t("How to display the grade."),
      '#type' => 'radios',
      '#options' => array(
        'raw' => t('Raw number'),
        'percent' => t('Percentage')),
      '#default_value' => $this->options['display_format']);

    parent::options_form($form, $form_state);
  }

  /**
   * Renders the field.
   */
  public function render($values) {
    $value = $this->get_value($values);
    $min = $this->get_value($values, 'grademin');
    $max = $this->get_value($values, 'grademax');

    switch ($this->options['display_format']) {
      case 'percent':
        $value = ($value - $min) / ($max - $min);
        $value = number_format($value * 100, 0) . '%';
        break;
    }
    return $this->sanitize_value($value);
  }
}
