<?php
/**
 * Author: Vojtěch Kusý <vojtech.kusy@gmail.com> 
 * Date: 18.7.13
 * Time: 17:13
 */


class NodeExporter extends Exporter {

  function __construct($id) {
    parent::__construct($id);
  }

  function getItemId($item) {
    return $item->nid;
  }

  function getCount() {
    return (int) db_query_range("SELECT COUNT(nid) FROM {node} WHERE status = 1", 0, 1)->fetchField();
  }

  function getItemIds($offset, $per_page) {
    return db_query_range("SELECT nid FROM {node} WHERE status = 1 ORDER BY nid ASC", $offset, $per_page)->fetchAllKeyed(0, 0);
  }

  function loadItem($nid) {
    return node_load($nid);
  }

  function renderHeader() {
    throw new BadMethodCallException(get_class($this) . '->' . __FUNCTION__ . ' method is not implemented.');
  }

  function renderItem($node) {
    throw new BadMethodCallException(get_class($this) . '->' . __FUNCTION__ . ' method is not implemented.');
  }

  function renderFooter() {
    throw new BadMethodCallException(get_class($this) . '->' . __FUNCTION__ . ' method is not implemented.');
  }

}
