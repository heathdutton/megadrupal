<?php
/**
 * @file
 * Defines the JSON Fields row style plugin, which lets users map view fields
 * to the components of the VEVENTs in the vcard feed.
 */

/**
 * A Views plugin which builds an JSON data from a views row with Fields.
 */
class widget_notification_plugin_row_json_fields extends views_plugin_row {

  /**
   * vcard field names and corresponding default values.
   */
  public function json_fields() {
    $json_fields = array(
      'path' => NULL,
      'fa_icon' => NULL,
      'title' => NULL,
      'subtitle' => NULL,
      'text_muted' => NULL,
      'badge_text' => NULL,
      'badge_type' => NULL,
      'progress_percent' => NULL,
      'progress_type' => NULL,
      'text' => NULL,
    );
    return $json_fields;
  }

  /**
   * Set up the options for the row plugin.
   */
  public function option_definition() {
    $options = parent::option_definition();

    $json_fields = $this->json_fields();

    foreach ($json_fields as $key => $value) {
      $options[$key] = array('default' => array());
    }
    return $options;
  }

  /**
   * Build the form for setting the row plugin's options.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $all_field_labels = $this->display->handler->get_field_labels();
    $text_field_label_options = array_merge(array('' => t('- None -')), $all_field_labels);

    $json_fields = $this->json_fields();

    $form['widget_notification_help'] = array(
      '#title' => t('Widget Notification Field Map'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $map_image_url = drupal_get_path('module', 'widget_notification') . '/includes/views_plugin_field_mapping.png';
    $form['widget_notification_help']['map'] = array(
      '#markup' => theme('image', array('path' => $map_image_url, 'width' => '100%')),
    );

    foreach ($json_fields as $key => $value) {
      $form[$key] =  array(
        '#type' => 'select',
        '#title' => $key,
        '#options' => $text_field_label_options,
        '#default_value' => $this->options[$key],
        '#required' => FALSE,
      );
    }


    $about_badge = l(t('Read more about badges'),'http://getbootstrap.com/components/#badges');
    $about_alert = l(t('Read more about alerts'),'http://getbootstrap.com/components/#alerts');
    $about_progress_type = l(t('Read more about progress and progress_type'),'http://getbootstrap.com/components/#progress');
    $about_fa = l(t('Read more about Font-Awesome'),'http://fortawesome.github.io/Font-Awesome/icons/');

    $form['fa_icon']['#description'] = t('Font-Awesome icon class name for the json feed (!link)', array('!link' => $about_fa));
    $form['badge_text']['#description'] = t('Text to be displayed inside a badge (!link)', array('!link' => $about_badge));
    $form['badge_type']['#description'] = t('Type of alert class for badges, possible values are "alert-danger", "alert-info", ... (!link)', array('!link' => $about_alert));
    $form['progress_percent']['#description'] = t('Percent progress (range 0 - 100)');
    $form['progress_type']['#description'] = t('Type of progress for progress bar, possible values are "progress-bar-danger", "progress-bar-warning", ... (!link)', array('!link' => $about_progress_type));
    $form['progress_type']['#description'] = t('Description/Body');
  }

  /**
   * Set up the environment for the render() function.
   */
  public function pre_render($result) {
    // Get the language for this view.
    $this->language = $this->display->handler->get_option('field_language');
    $substitutions = views_views_query_substitutions($this->view);
    if (array_key_exists($this->language, $substitutions)) {
      $this->language = $substitutions[$this->language];
    }
    $this->repeated_dates = array();
  }

  /**
   * Returns an Event array row in the query with index: $row->index.
   */
  public function render($row) {

    $data = array();

    $context = array(
      'row' => $row,
      'row_index' => $row->index,
      'language' => $this->language,
      'options' => $this->options,
    );

    $data = $this->create_json_data_array($row);

    // Allow other modules to alter the vcard before it gets passed to style plugin.
    drupal_alter('widget_notification_export_row', $data, $this->view, $context);

    return $data;
  }

  /**
   * Creates a JSON data array.
   */
  protected function create_json_data_array($row) {
    $data = $this->json_fields();

    foreach ($data as $key => $value) {
      if (!empty($this->options[$key])) {
        $data[$key] = strip_tags($this->get_field($row->index, $this->options[$key]));
      }
    }
    return $data;
  }

  /**
   * Retrieves a field value from the style plugin.
   *
   * @param int $index
   *   The index count of the row
   * @param string $field_id
   *   The ID assigned to the required field in the display.
   *
   * @see views_plugin_style::get_field()
   */
  protected function get_field($index, $field_id) {
    if (empty($this->view->style_plugin) || !is_object($this->view->style_plugin) || empty($field_id)) {
      return '';
    }
    return $this->view->style_plugin->get_field($index, $field_id);
  }
}
