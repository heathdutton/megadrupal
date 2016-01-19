<?php
/**
 * @file
 * Provides a views plugin style.
 *
 * The projekktor views plugin style allows a means to combine elements accross
 * multiple entities into a single Projekktor playlist.
 */

/**
 * Implements views_plugin_style().
 */
class views_plugin_style_projekktor extends views_plugin_style {

  /**
   * Set default options.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options += array(
      'projekktor_preset' => array('default' => 'default'),
      'projekktor_theme' => array('default' => 'default'),
    );

    return $options;
  }

  /**
   * Show a form to edit the style options.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['projekktor'] = array(
      '#type' => 'fieldset',
      '#title' => 'Projekktor',
    );
    $form['projekktor'] += projekktor_settings_form($this->options);

    $form['projekktor']['projekktor_image_style'] = array(
      '#title' => t('Stage poster image style'),
      '#type' => 'select',
      '#default_value' => $this->options['projekktor_image_style'],
      '#empty_option' => t('None (original image)'),
      '#options' => image_style_options(FALSE),
    );

    // jCarousel checkbox option.
    $form['projekktor']['projekktor_jcarousel'] = array(
      '#title' => t('jCarousel thumbnails'),
      '#type' => 'checkbox',
      '#default_value' => $this->options['projekktor_jcarousel'],
    );

    // Adds the carousel form.
    $form['projekktor'] += projekktor_jcarousel_settings_form($this->options);
  }

  /**
   * Performs some cleanup tasks on the options array before saving it.
   */
  function options_submit(&$form, &$form_state) {
    $options = &$form_state['values']['style_options'];

    // Pull the fieldset values one level up.
    $options += $options['projekktor'];
    unset($options['projekktor']);
  }

  /**
   * Searches for the file field to use.
   */
  function find_file_field() {
    foreach ($this->view->display_handler->get_handlers('field') as $id => $handler) {
      if (($handler instanceof views_handler_field_field) && ($handler->field_info['type'] == 'file')) {
        return $id;
        break;
      }
    }
    return FALSE;
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
   * Render the display in this style.
   */
  function render() {
    $file_field = $this->find_file_field();
    if ($file_field === FALSE) {
      drupal_set_message(t('Style @style requires an file field to be added.', array('@style' => $this->definition['title'])), 'error');
      return;
    }
    $image_field = $this->find_image_field();
    if ($image_field === FALSE) {
      $image_field = NULL;
    }

    // Group the rows according to the grouping field, if specified.
    $sets = $this->render_grouping($this->view->result, $this->options['grouping']);

    // Render each group separately and concatenate.
    $output = '';
    foreach ($sets as $title => $rows) {
      $output .= theme($this->theme_functions(),
        array(
          'view' => $this->view,
          'options' => $this->options,
          'file_field_name' => $file_field,
          'image_field_name' => $image_field,
          'rows' => $rows,
          'title' => $title,
        )
      );
    }
    return $output;
  }
}
