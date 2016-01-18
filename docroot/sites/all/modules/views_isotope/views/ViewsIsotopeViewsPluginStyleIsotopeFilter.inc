<?php
/**
 * @file
 * Define the "filter" views plugin.
 */

/**
 * Style plugin.
 */
class ViewsIsotopeViewsPluginStyleIsotopeFilter extends views_plugin_style_list {

  /**
   * Set default options.
   */
  public function option_definition() {
    $options = parent::option_definition();

    $options['instance_id'] = array('default' => '');
    $options['filter_group'] = array('default' => '');
    $options['data_fields'] = array('default' => array());

    return $options;
  }

  /**
   * Render the given style.
   */
  public function options_form(&$form, &$form_state) {
    $handlers = $this->display->handler->get_handlers('field');
    if (empty($handlers)) {
      $form['error_markup'] = array(
        '#markup' => t('<div class="error messages">You need at least one field before you can configure your isotope settings</div>'),
      );
      return;
    }

    $field_names = $this->display->handler->get_field_labels();
    $form['data_fields'] = array(
      '#type' => 'radios',
      '#required' => TRUE,
      '#options' => $field_names,
      '#title' => t('Data Fields'),
      '#default_value' => $this->options['data_fields'],
      '#description' => t('Select which fields contain data to be used for filtering.'),
    );

    $form['instance_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Enter an Instance ID'),
      '#default_value' => $this->options['instance_id'],
      '#description' => t('(Optional) If you have multiple grids on a page and you want filters that target specific ones.'),
    );

    $form['filter_group'] = array(
      '#type' => 'textfield',
      '#title' => t('Name of Filter'),
      '#default_value' => $this->options['filter_group'],
      '#description' => t('Each filter group needs a unique name. E.g. "Color", "Size", etc.'),
    );
  }

}
