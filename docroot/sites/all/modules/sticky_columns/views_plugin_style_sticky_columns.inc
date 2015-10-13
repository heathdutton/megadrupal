<?php

/**
 * @file
 * Contains the table style plugin.
 */

/**
 * Style plugin to render each item as a row in a table.
 *
 * @ingroup views_style_plugins
 */
class views_plugin_style_sticky_columns extends views_plugin_style_table {

  /**
   * Render the given style.
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
