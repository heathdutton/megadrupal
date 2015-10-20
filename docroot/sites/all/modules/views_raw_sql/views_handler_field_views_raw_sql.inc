<?php

/**
 * A handler to provide a field that is completely custom SQL by the administrator.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_views_raw_sql extends views_handler_field {

  function query() {

    $this->field_alias = $this->query->add_field(NULL, $this->options['raw_sql'], 'raw_sql_field');

  } // query

  function option_definition() {

    $options = parent::option_definition();

    // Override the alter text option to never alter the text.
    $options['alter']['contains']['alter_text'] = array('default' => FALSE);
    $options['raw_sql'] = array('default' => '');

    return $options;

  } // option_definition

  function options_form(&$form, &$form_state) {

    parent::options_form($form, $form_state);

    unset($form['alter']);

    $form['raw_sql'] = array(
      '#type' => 'textarea',
      '#title' => 'SQL',
      '#description' => t('This SQL will be added to the query as a field expression.'),
      '#default_value' => $this->options['raw_sql'],
      '#required' => TRUE,
    );

    if (!user_access('edit views raw sql')) {
      $form['raw_sql']['#disabled'] = TRUE;
      $form['raw_sql']['#description'] = t('You lack %permission permission to edit this field.', array('%permission' => 'edit views raw sql'));
    } // if

  } // options_form

  /**
   * Simple submit handler
   */
  function options_submit(&$form, &$form_state) {

    $this->options['raw_sql'] = $form_state['values']['options']['raw_sql'];

  } // options_submit

  function render($values) {;

    return $values->{$this->field_alias};

  } // render

} // views_handler_field_views_raw_sql
