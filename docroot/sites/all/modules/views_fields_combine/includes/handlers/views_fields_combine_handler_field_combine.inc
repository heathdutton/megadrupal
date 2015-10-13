<?php

/**
 * @file
 * A handler to combine the output of multiple fields into one.
 *
 * @ingroup views_field_handlers
 */
class views_fields_combine_handler_field_combine extends views_handler_field {
  function query() {
    // Do nothing -- to override the parent query.
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['combined_fields'] = array('default' => array());
    $options['separator'] = array('default' => '');
    $options['separator_allowed_tags'] = array('default' => array('a', 'em', 'strong', 'span'));
    $options['hide_empty'] = array('default' => FALSE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $options = $this->view->display_handler->get_field_labels();
    // Remove this field from options.
    unset($options[$this->options['id']]);

    $form['combined_fields'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Combined fields'),
      '#options' => $options,
      '#default_value' => $this->options['combined_fields'],
      '#prefix' => '<div id="edit-row-options-combined-wrapper"><div>',
      '#suffix' => '</div></div>',
    );

    $form['separator'] = array(
      '#title' => t('Separator'),
      '#type' => 'textfield',
      '#size' => 10,
      '#default_value' => isset($this->options['separator']) ? $this->options['separator'] : '',
      '#description' => t('The separator may be placed between combined fields to keep them from squishing up next to each other. You may use some basic HTML tags (@tags) in this field.', array('@tags' => implode(', ', $this->options['separator_allowed_tags']))),
    );

    $form['hide_empty'] = array(
      '#type' => 'checkbox',
      '#title' => t('Hide empty fields'),
      '#default_value' => $this->options['hide_empty'],
      '#description' => t('Do not display fields, labels or markup for fields that are empty.'),
    );
  }

  function render($values) {
    $output = array();
    $combined_fields = array_filter($this->options['combined_fields']);
    $fields = $this->view->display_handler->get_handlers('field');

    foreach (array_intersect_key($fields, $combined_fields) as $handler) {
      // Get rendered output from field.
      $rendered = $handler->last_render;
      if ($this->options['hide_empty'] && $this->is_value_empty($rendered, $handler->options['empty_zero'])) {
        continue;
      }
      $output[] = $rendered;
    }

    $separator = filter_xss($this->options['separator'], $this->options['separator_allowed_tags']);
    return implode($separator, $output);
  }
}
