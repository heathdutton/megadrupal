<?php
/**
 * @file
 * Define the "grid" views plugin.
 */

/**
 * Style plugin.
 */
class ViewsIsotopeViewsPluginStyleIsotopeGrid extends views_plugin_style_list {

  /**
   * Set default options.
   */
  public function option_definition() {
    $options = parent::option_definition();

    $options['instance_id'] = array('default' => '');
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

    $form['config'] = array(
      '#type' => 'select',
      '#options' => views_isotope_available_configs(),
      '#title' => t('Choose Isotope Configuration'),
      '#default_value' => !empty($this->options['config']) ? $this->options['config'] : 'isotope_default_config',
      '#required' => TRUE,
      '#description' => l(t('Manage Configurations'), 'admin/config/user-interface/isotope'),
    );

    $field_names = $this->display->handler->get_field_labels();
    $form['data_fields'] = array(
      '#type' => 'checkboxes',
      '#options' => $field_names,
      '#title' => t('Data Fields'),
      '#default_value' => $this->options['data_fields'],
      '#description' => t('Select which fields contain data to be used for filtering / sorting.'),
    );

    $form['instance_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Enter an Instance ID'),
      '#default_value' => $this->options['instance_id'],
      '#description' => t('(Optional) If you have multiple grids on a page and you want filters that target specific ones.'),
    );
  }

}
