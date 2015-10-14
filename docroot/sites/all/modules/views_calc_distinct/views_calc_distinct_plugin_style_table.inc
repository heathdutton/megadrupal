<?php
/**
 * @file
 * Copied from the table style plugin.
 */

/**
 * Style plugin to render each item as a row in a table.
 *
 * @ingroup views_style_plugins
 */
class views_calc_distinct_plugin_style_table extends views_calc_table {

  /**
   * Option definition.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['views_calc_distinct'] = array('default' => 0);
    return $options;
  }

  /**
   * Render the given style.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['views_calc_distinct'] = array(
      '#title' => t('Enable Views Calc Distinct'),
      '#type' => 'checkbox',
      '#default_value' => $this->options['views_calc_distinct'],
      '#description' => t("Select 'Disticnt' option in 'Query settings' first."),
    );
  }
}
