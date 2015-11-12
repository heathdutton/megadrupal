<?php

/**
 * @file
 * Definition of views_handler_field_variable.
 */

/**
 * A handler to provide a field that display system variables.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_variable extends views_handler_field {

  /**
   * Called to add the field to a query.
   */
  function query() {
    // Do nothing -- to override the parent query.
  }

  /**
   * The function used to retrieve the value of the variable.
   *
   * @var mixed
   */
  public $function = NULL;

  /**
   * Constructs a new field handler.
   */
  function construct() {
    parent::construct();

    $this->function = module_exists('variable') ? 'variable_get_value' : 'variable_get';
  }

  /**
   * Information about options for all kinds of purposes will be held here.
   *
   * @return array
   *   Returns the options of this handler/plugin.
   *
   * @see views_object::export_option()
   * @see views_object::export_option_always()
   * @see views_object::unpack_translatable()
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['variable_name'] = array('default' => '');
    return $options;
  }

  /**
   * Build the options form.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['variable'] = array(
      '#type' => 'fieldset',
      '#collapsible' => FALSE,
      '#title' => t('System variable'),
    );

    $form['variable_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Variable name'),
      '#default_value' => $this->options['variable_name'],
      '#description' => t('Enter the machine name of the Drupal variable to display. You may enter data from this view as per the "Replacement patterns" below.'),
      '#fieldset' => 'variable',
    );

    $form['variable']['variable_help'] = $form['alter']['help'];
    $form['variable']['variable_help']['#weight'] = 100;
    unset($form['variable_help']['#dependency']);
  }

  /**
   * Render the field.
   *
   * @param stdClass $values
   *   The values retrieved from the database.
   */
  function render($values) {
    // Return the variable's value and continue with the rendering process.
    $variable = call_user_func($this->function, $this->tokenize_value($this->options['variable_name']));
    return (is_array($variable) || is_object($variable)) ? serialize($variable) : $variable;
  }
}
