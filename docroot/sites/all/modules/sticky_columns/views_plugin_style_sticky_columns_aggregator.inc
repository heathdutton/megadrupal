<?php

/**
 * @file
 * Contains the aggregator table style plugin.
 */

/**
 * Style plugin to render each item as a column in a table.
 *
 * @ingroup views_style_plugins
 */
class views_plugin_style_sticky_columns_aggregator extends views_aggregator_plugin_style_table {

  /**
   * {@inheritdoc}
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['sticky_column'] = array('default' => FALSE);

    return $options;
  }

  /**
   * {@inheritdoc}
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    foreach (element_children($form['info']) as $field) {
      $form['info'][$field]['sticky_column'] = array(
        '#type' => 'checkbox',
        '#default_value' => isset($this->options['info'][$field]['sticky_column']) ? $this->options['info'][$field]['sticky_column'] : 0,
        '#dependency' => $form['info'][$field]['empty_column']['#dependency'],
      );
    }
  }

}
