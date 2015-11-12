<?php

/**
 * The plugin that handles a panel_pane.
 */
class commerce_views_pane_plugin_display_checkout_pane extends views_plugin_display {
  /**
   * The default block handler doesn't support configurable items,
   * but extended block handlers might be able to do interesting
   * stuff with it.
   */
  function execute_hook_commerce_checkout_pane_info() {
    $info = array();

    if ($this->display->display_title == $this->definition['title']) {
      $info['name'] = t('View: !view', array('!view' => $this->view->get_human_name()));
    }
    else {
      $info['name'] = t('View: !view: !display', array('!view' => $this->view->get_human_name(), '!display' => $this->display->display_title));
    }

    return $info;
  }

  /**
   * The display block handler returns the structure necessary for a block.
   */
  function execute() {
    // Prior to this being called, the $view should already be set to this
    // display, and arguments should be set on the view.
    $info['content'] = $this->view->render();
    $info['title'] = filter_xss_admin($this->view->get_title());
    if (!empty($this->view->result) || $this->get_option('empty') || !empty($this->view->style_plugin->definition['even empty'])) {
      return $info;
    }
    return array();
  }

  function get_path() {
    if (empty($this->view->override_path)) {
      return parent::get_path();
    }
    return $this->view->override_path;
  }

  /**
   * Display use exposed widgets only if AJAX is set.
   */
  function uses_exposed() {
    if ($this->use_ajax()) {
      return parent::uses_exposed();
    }
    return FALSE;
  }
}
