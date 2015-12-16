<?php
/**
 * Author: Vojtěch Kusý <vojtech.kusy@gmail.com> 
 * Date: 22.7.13
 * Time: 16:30
 */

class UserExporter extends Exporter {

  function __construct($id) {
    parent::__construct($id);
  }

  function getItemId($item) {
    return $item->uid;
  }

  function getCount() {
    return (bool) db_query_range("SELECT COUNT(uid) FROM {users}", 0, 1)->fetchField();
  }

  function getItemIds($offset, $per_page) {
    return db_query_range("SELECT uid FROM {users} ORDER BY uid ASC", $offset, $per_page)->fetchAllKeyed(0, 0);
  }

  function loadItem($uid) {
    return user_load($uid);
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
