<?php
/**
 * @file
 * imageflow style plugin for the Views module.
 */

/**
 * Implements a style type plugin for the Views module.
 */
class views_plugin_style_imageflow extends views_plugin_style {

  /**
   * Set default options.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options += array(
      'imageflow_preset' => array('default' => 'default'),
    );

    return $options;
  }

  /**
   * Show a form to edit the style options.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['imageflow'] = array(
      '#type' => 'fieldset',
      '#title' => 'imageflow',
    );

    $presets = array();
    foreach (imageflow_presets() as $name => $preset) {
      $presets[$name] = check_plain($preset['title']);
    }
    $form['imageflow']['imageflow_preset'] = array(
      '#title' => t('preset'),
      '#type' => 'select',
      '#options' => $presets,
      '#default_value' => $this->options['imageflow_preset'],
    );
  }

  /**
   * Performs some cleanup tasks on the options array before saving it.
   */
  function options_submit(&$form, &$form_state) {
    $options = &$form_state['values']['style_options'];

    // Pull the fieldset values one level up.
    $options += $options['imageflow'];
    unset($options['imageflow']);
  }

  /**
   * Searches for the image field to use.
   */
  function find_image_field() {
    foreach ($this->view->display_handler->get_handlers('field') as $id => $handler) {
      if (($handler instanceof views_handler_field_field) && ($handler->field_info['type'] == 'image')) {
        return $id;
        break;
      }
    }
    return FALSE;
  }

  /**
   * Searches for the link field to use.
   */
  function find_link_field() {
    foreach ($this->view->display_handler->get_handlers('field') as $id => $handler) {
      if (($handler instanceof views_handler_field_field) && ($handler->field_info['type'] == 'link_field')) {
        return $id;
        break;
      }
    }
    return FALSE;
  }

  /**
   * Render the display in this style.
   */
  function render() {
    $image_field = $this->find_image_field();
    if ($image_field === FALSE) {
      drupal_set_message(t('Style @style requires an image field to be added.', array('@style' => $this->definition['title'])), 'error');
      return;
    }
    $link_field = $this->find_link_field();

    // Group the rows according to the grouping field, if specified.
    $sets = $this->render_grouping($this->view->result, $this->options['grouping']);

    // Render each group separately and concatenate.
    $output = '';
    foreach ($sets as $title => $rows) {
      $output .= theme($this->theme_functions(),
        array(
          'view' => $this->view,
          'options' => $this->options,
          'img_field_name' => $image_field,
          'link_field_name' => $link_field,
          'rows' => $rows,
          'title' => $title)
        );
    }
    return $output;
  }
}
