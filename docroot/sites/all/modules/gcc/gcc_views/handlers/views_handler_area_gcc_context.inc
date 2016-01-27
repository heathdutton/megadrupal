<?php

/**
 * @file
 * Definition of views_handler_area_text.
 */

/**
 * Views area text handler.
 *
 * @ingroup views_area_handlers
 */
class views_handler_area_gcc_context extends views_handler_area {

  function option_definition() {

    $options = parent::option_definition();
    $options['entity_type'] = array('default' => NULL);
    $options['arg_position'] = array('default' => NULL);

    return $options;
  }

  function options_form(&$form, &$form_state) {

    parent::options_form($form, $form_state);

    unset($form['empty']);

    $form['entity_type'] = array(

      '#type' => 'select',
      '#title' => t('Entity Type'),
      '#default_value' => $this->options['entity_type'],
      '#options' => array(
        'node' => t('Node'),
      ),
    );

    $form['arg_position'] = array(

      '#type' => 'select',
      '#title' => t('Argument Position'),
      '#default_value' => $this->options['arg_position'],
      '#options' => drupal_map_assoc(array(0, 1, 2, 3, 4, 5, 6, 7, 8, 9)),
    );

  }

  function render($empty = FALSE) {

    return '';
  }
}
