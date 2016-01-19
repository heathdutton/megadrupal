<?php

/**
 * @file
 * Defines custom Entity Views field handler.
 */

/**
 * Custom Views field handler.
 */
class commerce_prodnodelink_handler_field_display_node_ids extends views_handler_field {
  /**
   * Nullify any query.
   */
  public function query() {
  }

  /**
   * Allow EntityFieldHandlerHelper take care of things.
   */
  public function get_value($values, $field = NULL) {
    return EntityFieldHandlerHelper::get_value($this, $values, $field);
  }

  /**
   * Allow EntityFieldHandlerHelper take care of things.
   */
  public function pre_render(&$values) {
    EntityFieldHandlerHelper::pre_render($this, $values);
  }

  /**
   * Format the list of node IDs as links.
   */
  public function render($values) {
    $items = array();
    $nids = $this->get_value($values, 'display_node_ids');

    if ($nids) {
      foreach ($nids as $nid) {
        $items[] = l(t('Node @nid', array('@nid' => $nid)), 'node/' . $nid);
      }
    }

    return array(
      '#theme' => 'item_list',
      '#items' => $items,
    );
  }
}
