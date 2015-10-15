<?php

class EntityListHandlerDraggableViews extends EntityListHandlerViews {

  protected function viewOptions() {
    return _draggableviews_get_views_options();
  }

  /**
   * @todo Set tags with arguments for better granularity.
   */
  public function cacheTags() {
    $tags = array();
    if ($view = $this->view()) {
      list($name, $display) = explode(':', $this->config['view']);
      $tags = array('views' => array($name, "$name:$display"));

      // Add tags for this view's order view as well.
      if ($order_view = _draggableviews_get_order_view_display($view)) {
        list($order_name, $order_display) = explode(':', $order_view);
        $tags['views'][] = $order_name;
        $tags['views'][] = "$order_name:$order_display";
      }
    }
    return $tags;
  }

  public function adminPath() {
    if ($view = $this->view()) {
      list($name, $display) = explode(':', $this->config['view']);
      $view->build($display);
      if ($path = _draggableviews_get_order_path($view, TRUE)) {
        return $path;
      }
    }
    return FALSE;
  }
}
