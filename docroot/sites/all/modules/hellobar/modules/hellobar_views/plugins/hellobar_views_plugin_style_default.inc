<?php
/**
 * @file
 * Definition of hellobar_views_plugin_style_default.
 */

/**
 * Class to define a style plugin handler.
 */
class HelloBarViewsPluginStyleDefault extends views_plugin_style {

  /**
   * Query.
   */
  function query() {
    $this->view->query->add_field('node', 'nid');
  }

  /**
   * Pre Render.
   *
   * @param array $result
   *   Result.
   */
  function pre_render($result) {
    $nids = array();
    hellobar_initialization();

    foreach ($result as $item) {
      $nids[] = $item->nid;
    }

    $nodes = node_load_multiple($nids);
    hellobar_content_message_push_multiple($nodes);
  }

  /**
   * Render.
   */
  function render() {}
}
