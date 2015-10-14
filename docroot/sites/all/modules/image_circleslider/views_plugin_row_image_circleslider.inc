<?php
/**
 * @file
 * Contains the base row style plugin.
 */

/**
 * The basic 'fields' row plugin.
 *
 * This displays fields one after another, giving options for inline or not.
 *
 * @ingroup views_row_plugins.
 */
class views_plugin_row_image_circleslider extends views_plugin_row {
  /**
   * Implement option_definition().
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['image_field'] = array('default' => '');
    return $options;
  }

  /**
   * Implement option_form().
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // Pre-build all of our option lists for the dials and switches that follow.
    $fields = array('' => t('<none>'));
    foreach ($this->display->handler->get_handlers('field') as $field => $handler) {
      if ($label = $handler->label()) {
        $fields[$field] = $label() . ': ' . $handler->options['settings']['image_style'];
      }
      else {
        $fields[$field] = $handler->ui_name();
      }

    }
    $form['image_field'] = array(
      '#type' => 'select',
      '#title' => t('Image field'),
      '#options' => $fields,
      '#default_value' => $this->options['image_field'],
      '#description' => t('Select the field that will be used as the Image
        Slide. REQUIRED. '),
    );
  }

}
