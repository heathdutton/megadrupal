<?php


class nodeaspect_PageInfo {

  protected $info;

  function __construct(array $info) {
    $this->info = $info;
  }

  function localTaskWeight() {
    return $this->info['tab'] ? $this->info['weight'] : FALSE;
  }

  /**
   * Access callback.
   * This only returns an internal variable.
   */
  function access() {
    return !empty($this->info['access']);
  }

  /**
   * Title callback.
   * This only returns an internal variable.
   */
  function title() {
    return $this->info['title'];
  }

  /**
   * Page callback.
   */
  function page($node) {

    if (!empty($this->info['page_title'])) {
      drupal_set_title($this->info['page_title']);
    }

    if (isset($this->info['view_mode'])) {
      return $this->_nodeShow($node);
    }
    elseif (isset($this->info['page callback'])) {
      if (!empty($this->info['file'])) {
        require_once $this->info['file'];
      }
      return call_user_func_array($this->info['page callback'], $this->info['page arguments']);
    }
    elseif (isset($this->info['redirect'])) {
      // TODO: drupal_goto().
    }
  }

  function hook_pageapi($api, $node, $suffix) {
    if (!empty($this->info['pageapi_callback'])) {
      $f = $this->info['pageapi_callback'];
      $api->setModule($this->info['module']);
      if (empty($this->info['pageapi_arguments'])) {
        $f($api, $node, $suffix);
      }
      else {
        $args = array_merge(array($api, $node, $suffix), $this->info['pageapi_arguments']);
        call_user_func_array($f, $args);
      }
    }
  }

  function hook_preprocess_page(&$vars, $node, $suffix) {
    
  }

  /**
   * Adapted from function node_show()
   */
  protected function _nodeShow($node) {

    $info = $this->info;

    // For markup consistency with other pages, use node_view_multiple() rather than node_view().
    $render_array = node_view_multiple(array($node->nid => $node), $info['view_mode']);

    // Update the history table, stating that this user viewed this node.
    node_tag_new($node);

    return $render_array;
  }
}
