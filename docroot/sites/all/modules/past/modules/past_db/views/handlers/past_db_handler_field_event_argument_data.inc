<?php
/**
 * @file
 * Contains a views field handler.
 */

/**
 * Field handler which shows the event's argument key value.
 *
 * @ingroup views_field_handlers
 */
class past_db_handler_field_event_argument_data extends views_handler_field_entity {

  /**
   * {@inheritdoc}
   */
  function option_definition() {
    $definition = parent::option_definition();
    $definition['argument_name'] = array('default' => NULL);
    $definition['data_key'] = array('default' => NULL);
    return $definition;
  }

  /**
   * {@inheritdoc}
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['argument_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Argument nane'),
      '#default_value' => $this->options['argument_name'],
      '#required' => TRUE,
    );

    $form['data_key'] = array(
      '#type' => 'textfield',
      '#title' => t('Data key'),
      '#default_value' => $this->options['data_key'],
    );
    return $form;
  }

  /**
   * {@inheritdoc}
   */
  function render($values) {
    $event = $this->get_value(NULL);
    if (empty($this->options['argument_name'])) {
      return;
    }
    if (!empty($event)) {
      $argument = $event->getArgument($this->options['argument_name']);
      if (!empty($argument)) {
        $data = $argument->getData();
        if (!empty($this->options['data_key'])) {
          $data = isset($data[$this->options['data_key']]) ? $data[$this->options['data_key']] : NULL;
        }
        return is_array($data) ? print_r($data, TRUE) : $data;
      }
    }
    return;
  }

}
