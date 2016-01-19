<?php
/**
 * @file
 * The plugin to handle full pager.
 *
 * @ingroup views_pager_plugins
 */

class Views_plugin_pager_infinite_table_scroll extends views_plugin_pager_full {
  /**
   * Pager init function.
   */
  public function init(&$view, &$display, $options = array()) {
    parent::init($view, $display, $options);
  }

  /**
   * Pager settings.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['infinite_table_scroll'] = array(
      '#type' => 'fieldset',
      '#title' => 'Infinite table scroll settings',
    );
    $form['infinite_table_scroll']['tbody_height'] = array(
      '#title' => 'tbody height',
      '#type' => 'textfield',
      '#default_value' => empty($this->options['infinite_table_scroll']['tbody_height']) ? 250 : $this->options['infinite_table_scroll']['tbody_height'],
    );
    $form['infinite_table_scroll']['thead_height'] = array(
      '#title' => 'thead height (in pixel)',
      '#type' => 'textfield',
      '#default_value' => empty($this->options['infinite_table_scroll']['thead_height']) ? 30 : $this->options['infinite_table_scroll']['thead_height'],
    );
    $form['infinite_table_scroll']['tfoot_height'] = array(
      '#prefix' => 'Leave blank if not showing tfoot.',
      '#title' => 'tfoot height (in pixel)',
      '#description' => 'thead will be cloned as tfoot if tfoot height is defined',
      '#type' => 'textfield',
      '#default_value' => empty($this->options['infinite_table_scroll']['tfoot_height']) ? : $this->options['infinite_table_scroll']['tfoot_height'],
    );
  }

  /**
   * Setting summary.
   */
  public function summary_title() {
    if (!empty($this->options['offset'])) {
      return format_plural($this->options['items_per_page'], 'Infinite table scroll pager, @count item, skip @skip', 'Infinite scroll pager, @count items, skip @skip', array('@count' => $this->options['items_per_page'], '@skip' => $this->options['offset']));
    }
    return format_plural($this->options['items_per_page'], 'Infinite table scroll pager, @count item', 'Infinite scroll pager, @count items', array('@count' => $this->options['items_per_page']));
  }

  /**
   * Pager render function.
   */
  public function render($input) {
    global $base_url;
    $content_selector = '';
    $style_options = $this->view->style_options;
    $items_selector = '';
    $view_args = '';
    if (!empty($this->view->args)) {
      foreach ($this->view->args as $args_key => $args_val) {
        $view_args_arr[] = check_plain($args_val);
      }
      $view_args = implode ('/', $view_args_arr);
    }
    switch ($this->view->plugin_name) {
      case 'table':
        $content_selector = 'div.view-content > table > tbody';
        $items_selector = 'tr';
        $pager_theme = views_theme_functions('views_infinite_table_scroll_pager', $this->view, $this->display);
        return theme($pager_theme,
          array('tags' => $input,
            'quantity' => $this->options['items_per_page'],
            'view_name' => $this->view->name,
            'current_display' => $this->view->current_display,
            'content_selector' => $content_selector,
            'items_selector' => $items_selector,
            'element' => $this->options['id'],
            'view_args' => $view_args,
            'tbody_height' => $this->options['infinite_table_scroll']['tbody_height'],
            'thead_height' => $this->options['infinite_table_scroll']['thead_height'],
            'tfoot_height' => $this->options['infinite_table_scroll']['tfoot_height'],
          )
        );
      break;
    }
  }
}
