<?php

/**
 * @file
 * The plugin to handle horizontal pager with scroll.
 * @ingroup views_pager_plugins
 */

class views_scroll_pager_plugin_horizontal_scroll extends views_plugin_pager {
  function summary_title() {
    if (!empty($this->options['offset'])) {
      return format_plural($this->options['items_per_page'], '@count item, skip @skip', 'Paged, @count items, skip @skip', array('@count' => $this->options['items_per_page'], '@skip' => $this->options['offset']));
    }
    return format_plural($this->options['items_per_page'], '@count item', 'Paged, @count items', array('@count' => $this->options['items_per_page']));
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['items_per_page'] = array('default' => 10);
    $options['offset'] = array('default' => 0);
    $options['id'] = array('default' => 0);

    return $options;
  }

  /**
   * Provide the default form for setting options.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $pager_text = $this->display->handler->get_pager_text();
    $form['items_per_page'] = array(
      '#title' => $pager_text['items per page title'],
      '#type' => 'textfield',
      '#description' => $pager_text['items per page description'],
      '#default_value' => $this->options['items_per_page'],
    );

    $form['offset'] = array(
      '#type' => 'textfield',
      '#title' => t('Offset'),
      '#description' => t('The number of items to skip. For example, if this field is 3, the first 3 items will be skipped and not displayed.'),
      '#default_value' => $this->options['offset'],
    );

    $form['id'] = array(
      '#type' => 'textfield',
      '#title' => t('Pager ID'),
      '#description' => t("Unless you're experiencing problems with pagers related to this view, you should leave this at 0. If using multiple pagers on one page you may need to set this number to a higher value so as not to conflict within the ?page= array. Large values will add a lot of commas to your URLs, so avoid if possible."),
      '#default_value' => $this->options['id'],
    );
  }

  function query() {
    if ($this->items_per_page_exposed()) {
      if (!empty($_GET['items_per_page']) && $_GET['items_per_page'] > 0) {
        $this->options['items_per_page'] = $_GET['items_per_page'];
      }
      elseif (!empty($_GET['items_per_page']) && $_GET['items_per_page'] == 'All' && $this->options['expose']['items_per_page_options_all']) {
        $this->options['items_per_page'] = 0;
      }
    }
    if ($this->offset_exposed()) {
      if (isset($_GET['offset']) && $_GET['offset'] >= 0) {
        $this->options['offset'] = $_GET['offset'];
      }
    }

    $limit = $this->options['items_per_page'];
    $offset = $this->current_page * $this->options['items_per_page'] + $this->options['offset'];

    $this->view->query->set_limit($limit);
    $this->view->query->set_offset($offset);
  }

  function render($input) {
    $pager_theme = views_theme_functions('views_scroll_pager_horizontal_scroll', $this->view, $this->display);
    $output = theme($pager_theme, array(
      'element' => $this->options['id'],
      'parameters' => $input,
      'views_class' => drupal_html_class('views_' . $this->view->name . '_' . $this->view->current_display),
    ));
    return $output;
  }

  /**
   * Set the current page.
   *
   * @param $number
   *   If provided, the page number will be set to this. If NOT provided,
   *   the page number will be set from the global page array.
   */
  function set_current_page($number = NULL) {
    if (isset($number)) {
      $this->current_page = $number;
      return;
    }

    // If the current page number was not specified, extract it from the global
    // page array.
    global $pager_page_array;

    if (empty($pager_page_array)) {
      $pager_page_array = array();
    }

    // Fill in missing values in the global page array, in case the global page
    // array hasn't been initialized before.
    $page = isset($_GET['page']) ? explode(',', $_GET['page']) : array();

    for ($i = 0; $i <= $this->options['id'] || $i < count($pager_page_array); $i++) {
      $pager_page_array[$i] = empty($page[$i]) ? 0 : $page[$i];
    }

    $this->current_page = intval($pager_page_array[$this->options['id']]);

    if ($this->current_page < 0) {
      $this->current_page = 0;
    }
  }

  function get_pager_total() {
    if ($items_per_page = intval($this->get_items_per_page())) {
      return ceil($this->total_items / $items_per_page);
    }
    else {
      return 1;
    }
  }

  /**
   * Update global paging info.
   *
   * This is called after the count query has been run to set the total
   * items available and to update the current page if the requested
   * page is out of range.
   */
  function update_page_info() {
    if (!empty($this->options['total_pages'])) {
      if (($this->options['total_pages'] * $this->options['items_per_page']) < $this->total_items) {
        $this->total_items = $this->options['total_pages'] * $this->options['items_per_page'];
      }
    }

    // Don't set pager settings for items per page = 0.
    $items_per_page = $this->get_items_per_page();
    if (!empty($items_per_page)) {
      // Dump information about what we already know into the globals.
      global $pager_page_array, $pager_total, $pager_total_items, $pager_limits;
      // Set the limit.
      $pager_limits[$this->options['id']] = $this->options['items_per_page'];
      // Set the item count for the pager.
      $pager_total_items[$this->options['id']] = $this->total_items;
      // Calculate and set the count of available pages.
      $pager_total[$this->options['id']] = $this->get_pager_total();

      // See if the requested page was within range:
      if ($this->current_page < 0) {
        $this->current_page = 0;
      }
      else if ($this->current_page >= $pager_total[$this->options['id']]) {
        // Pages are numbered from 0 so if there are 10 pages, the last page is 9.
        $this->current_page = $pager_total[$this->options['id']] - 1;
      }

      // Put this number in to guarantee that we do not generate notices when the pager
      // goes to look for it later.
      $pager_page_array[$this->options['id']] = $this->current_page;
    }
  }
}
