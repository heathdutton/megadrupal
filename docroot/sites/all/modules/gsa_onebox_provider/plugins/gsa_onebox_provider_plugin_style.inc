<?php
/**
 * @file
 * Generalized style plugin.
 */

/**
 *
 * @ingroup views_style_plugins
 */
class GSAOneboxProviderPluginStyle extends views_plugin_style {

  /**
   * Render the display in this style.
   */
  public function render() {
    if ($this->uses_row_plugin() && empty($this->row_plugin)) {
      vpr('views_plugin_style_default: Missing row plugin');
      return;
    }

    $output = '';
    $rows['header'] = $this->renderHeader();
    $rows['body'] = $this->renderBody();
    $rows['footer'] = $this->renderFooter();
    $title = '';
    $out_array = array(
      'view' => $this->view,
      'options' => $this->options,
      'rows' => $rows,
      'title' => $title,
    );
    $output .= theme($this->themeFunctions(), $out_array);
    return $output;
  }

  /**
   * Render the header.
   */
  public function renderHeader() {
    $rows = array();
    $title = '';
    $output = '';
    $out_array = array(
      'view' => $this->view,
      'options' => $this->options,
      'rows' => $rows,
      'title' => $title,
    );
    $output .= theme($this->themeFunctions($this->definition['additional themes base'] . '_header'), $out_array);
    return $output;
  }

  /**
   * Render the footer.
   */
  public function renderFooter() {
    $rows = array();
    $title = '';
    $output = '';
    $out_array = array(
      'view' => $this->view,
      'options' => $this->options,
      'rows' => $rows,
      'title' => $title,
    );
    $output .= theme($this->themeFunctions($this->definition['additional themes base'] . '_footer'), $out_array);

    return $output;
  }

  /**
   * Render the body.
   */
  public function renderBody() {
    if ($this->uses_row_plugin() && empty($this->row_plugin)) {
      vpr('views_plugin_style_default: Missing row plugin');
      return;
    }

    // Group the rows according to the grouping field, if specified.
    $sets = $this->render_grouping($this->view->result, $this->options['grouping']);

    // Render each group separately and concatenate.  Plugins may override this
    // method if they wish some other way of handling grouping.
    $output = '';
    foreach ($sets as $title => $records) {
      if ($this->uses_row_plugin()) {
        $rows = array();
        foreach ($records as $row_index => $row) {
          $this->view->row_index = $row_index;
          $rows[] = $this->row_plugin->render($row);
        }
      }
      else {
        $rows = $records;
      }

      $title = '';
      $out_array = array(
        'view' => $this->view,
        'options' => $this->options,
        'rows' => $rows,
        'title' => $title,
      );
      $output .= theme($this->themeFunctions($this->definition['additional themes base'] . '_body'), $out_array);
    }
    unset($this->view->row_index);
    return $output;

  }

  /**
   * Provide a full list of possible theme templates used by this style.
   */
  public function themeFunctions($hook = NULL) {
    if (is_null($hook)) {
      $hook = $this->definition['theme'];
    }
    return views_theme_functions($hook, $this->view, $this->display);
  }

  /**
   * Add any HTTP headers that this style plugin wants to.
   */
  public function add_http_headers() {
    $view = &$this->view;

    drupal_add_http_header('Cache-Control', 'max-age=60, must-revalidate');

    if (!empty($this->definition['headers'])) {
      foreach ($this->definition['headers'] as $name => $value) {
        drupal_add_http_header($name, $value);
      }
    }
  }
}
