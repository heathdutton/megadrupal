<?php

/**
 * @file
 * Definition of views_extra_pager_mini_all.
 */

/**
 * The plugin to handle full pager with link to all items.
 *
 * @ingroup views_pager_plugins
 */
class views_extra_pager_mini_all extends views_plugin_pager_mini {

  function showing_all() {
    return (!empty($_GET['items_per_page']) && $_GET['items_per_page'] == 'All');
  }

  function use_pager() {
    return ($this->showing_all()) ? FALSE : parent::use_pager();
  }

  function use_count_query() {
    return ($this->showing_all()) ? FALSE : parent::use_count_query();
  }

  function get_items_per_page() {
    return ($this->showing_all()) ? 0 : parent::get_items_per_page();
  }

  function query() {
    parent::query();
    if ($this->showing_all()) {
        $this->options['items_per_page'] = 0;
        $this->options['offset'] = 0;
        $this->view->query->set_limit($this->options['items_per_page']);
        $this->view->query->set_offset($this->options['offset']);
    }
  }

  function render($input) {
    if ($this->options['items_per_page'] == 0) {
      return NULL;
    }
    $pager = parent::render($input);

    $pager_theme = views_theme_functions('views_extra_pager_mini_all', $this->view, $this->display);

    return theme($pager_theme, array(
      'pager' => $pager,
      'element' => $this->get_pager_id(),
      'parameters' => $input,
      'all_label' => $this->options['expose']['items_per_page_options_all_label'],
    ));
  }

}
