<?php
/**
 * @file
 * Views style plugin for the widget_notification module.
 */

/**
 * Defines a Views style plugin that renders JSON feeds.
 */
class widget_notification_plugin_style_json_feed extends views_plugin_style {

  /**
   * Internal helper function.
   */
  protected function _get_option($option_name) {
    return isset($this->options[$option_name]) ? $this->options[$option_name] : '';
  }

  /**
   * Set up the options for the style plugin.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['badge_text'] = array('default' => array());
    $options['badge_type'] = array('default' => array());
    $options['detail_url_text'] = array('default' => array());
    $options['detail_url'] = array('default' => array());
    return $options;
  }

  /**
   * Build the form for setting the style plugin's options.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

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

    $about_badge = l(t('Read more about badges'),'http://getbootstrap.com/components/#badges');
    $about_alert = l(t('Read more about alerts'),'http://getbootstrap.com/components/#alerts');
    $about_fa = l(t('Read more about Font-Awesome'),'http://fortawesome.github.io/Font-Awesome/icons/');

    $variables = array(
      'items' => array(
        '@start -- the initial record number in the set',
        '@end -- the last record number in the set',
        '@total -- the total records in the set',
        '@name -- the human-readable name of the view',
        '@per_page -- the number of items per page',
        '@current_page -- the current page number',
        '@current_record_count -- the current page record count',
        '@page_count -- the total page count',
      ),
    );
    $list = theme('item_list', $variables);

    $form['badge_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Badge Text'),
      '#default_value' => $this->_get_option('badge_text'),
      '#description' => t('Text to be displayed inside a badge (!link). The following tokens are supported:', array('!link' => $about_badge)) . $list,

    );
    $form['badge_type'] = array(
      '#type' => 'textfield',
      '#title' => 'badge_type',
      '#default_value' => $this->_get_option('badge_type'),
      '#description' => t('Type of alert class for badges, possible values are "alert-danger", "alert-info", ... (!link)', array('!link' => $about_alert)),
    );
    $form['fa_icon'] = array(
      '#type' => 'textfield',
      '#title' => 'fa_icon',
      '#default_value' => $this->_get_option('fa_icon'),
      '#description' => t('.'),
      '#description' => t('Font-Awesome icon class name for the json feed (!link)', array('!link' => $about_fa)),
    );
    $form['detail_url_text'] = array(
      '#type' => 'textfield',
      '#title' => 'detail_url_text',
      '#default_value' => $this->_get_option('detail_url_text'),
      '#description' => t('Text to be displayed on the link to details page'),
    );
    $form['detail_url'] = array(
      '#type' => 'textfield',
      '#title' => 'detail_url',
      '#default_value' => $this->_get_option('detail_url'),
      '#description' => t('Path to details page'),
    );
  }

  /**
   * Render the event arrays returned by the row plugin into a VCALENDAR.
   */
  public function render() {
    if (empty($this->row_plugin) || !in_array($this->row_plugin->plugin_name, array('widget_notification_fields'))) {
      debug('widget_notification_fields: This style plugin supports only the "Widget Notification Fields" row plugins.', NULL, TRUE);
      return t('To enable Widget Notification output, the view Format must be configured to Show: Widget Notification Fields.');
    }
    $var = array();
    $var['badge_text'] = $this->render_tokenize($this->options['badge_text']);
    $var['badge_type'] = $this->options['badge_type'];
    $var['fa_icon'] = $this->options['fa_icon'];
    $var['detail_url'] = url($this->options['detail_url']);
    $var['detail_url_text'] = $this->options['detail_url_text'];
    foreach ($this->view->result as $row_index => $row) {
      $this->view->row_index = $row_index;
      $row->index = $row_index;
      try {
        $var['messages'][] = $this->row_plugin->render($row);
      }
      catch (Exception $e) {
        debug($e->getMessage(), NULL, TRUE);
        return $e->getMessage();
      }
    }
    unset($this->view->row_index);
    // These steps shouldn't be run when during Preview on the View page.
    if (empty($this->view->live_preview)) {
      // Prevent devel module from appending queries to ical export.
      $GLOBALS['devel_shutdown'] = FALSE;

      drupal_add_http_header('Cache-Control', 'no-cache, must-revalidate');
      drupal_add_http_header('Expires', 'Sat, 26 Jul 1997 05:00:00 GMT');

      drupal_json_output($var);
      drupal_exit();
    }
    return drupal_json_encode($var);
  }


  /**
   * Find out the information to render.
   */
  function render_tokenize($format) {
    // Must have options and does not work on summaries.
    $output = '';
    // Calculate the page totals.
    $current_page = (int) $this->view->get_current_page() + 1;
    $per_page = (int) $this->view->get_items_per_page();
    $count = count($this->view->result);
    // @TODO: Maybe use a possible is views empty functionality.
    // Not every view has total_rows set, use view->result instead.
    $total = isset($this->view->total_rows) ? $this->view->total_rows : count($this->view->result);
    $name = check_plain($this->view->human_name);
    if ($per_page === 0) {
      $page_count = 1;
      $start = 1;
      $end = $total;
    }
    else {
      $page_count = (int) ceil($total / $per_page);
      $total_count = $current_page * $per_page;
      if ($total_count > $total) {
        $total_count = $total;
      }
      $start = ($current_page - 1) * $per_page + 1;
      $end = $total_count;
    }
    $current_record_count = ($end - $start) + 1;
    // Get the search information.
    $items = array('start', 'end', 'total', 'name', 'per_page', 'current_page', 'current_record_count', 'page_count');
    $replacements = array();
    foreach ($items as $item) {
      $replacements["@$item"] = ${$item};
    }
    // Send the output.
    if (!empty($total)) {
      $output .= filter_xss_admin(str_replace(array_keys($replacements), array_values($replacements), $format));
    }
    return $output;
  }
}
