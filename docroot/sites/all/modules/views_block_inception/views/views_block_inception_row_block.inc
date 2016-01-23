<?php

class views_block_inception_row_block extends views_plugin_row {
  function option_definition() {
    $options = parent::option_definition();

    $options['blockfields']['subject'] = '';
    $options['blockfields']['content'] = '';
    $options['blockfields']['delta'] = '';
    $options['default_field_elements'] = array('default' => TRUE, 'bool' => TRUE);

    return $options;
  }

  /**
   * Provide a form for setting options.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $options = array(''=>'None') + $this->display->handler->get_field_labels();

    $form['blockfields']['delta'] = array(
      '#type' => 'select',
      '#title' => t('Delta field'),
      '#description' => t('Select the field that will be the delta (example: the ID of the entity).'),
      '#options' => $options,
      '#default_value' => $this->options['blockfields']['delta'],
    );

    $form['blockfields']['subject'] = array(
      '#type' => 'select',
      '#title' => t('Subject field'),
      '#description' => t('Select the field that will be the subject of the block.'),
      '#options' => $options,
      '#default_value' => $this->options['blockfields']['subject'],
    );

    $form['blockfields']['content'] = array(
      '#type' => 'select',
      '#title' => t('Content field'),
      '#description' => t('Select the field that will be the content of the block.'),
      '#options' => $options,
      '#default_value' => $this->options['blockfields']['content'],
    );
  }
}
