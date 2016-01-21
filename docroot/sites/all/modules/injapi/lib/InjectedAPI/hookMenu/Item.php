<?php


class injapi_InjectedAPI_hookMenu_Item extends injapi_InjectedAPI_Abstract_Item {

  //                                                          Loading and access
  // ---------------------------------------------------------------------------

  function loadArguments($args) {
    $this->setOrUnset('load arguments', $args);
  }

  function permission($permission) {
    unset($this->data['access callback']);
    $this->data['access arguments'] = array($permission);
    return $this;
  }

  function accessCallback($callback, $args = NULL) {
    $this->data['access callback'] = $callback;
    $this->setOrUnset('access arguments', $args);
    return $this;
  }

  function openToPublic() {
    $this->data['access callback'] = TRUE;
    unset($this->data['access arguments']);
  }

  function restrictToAdmin() {
    $this->permission('administer site configuration');
  }

  //                                                       Title and description
  // ---------------------------------------------------------------------------

  function title($title) {
    $this->data['title'] = $title;
    return $this;
  }

  function titleCallback($callback, $args = NULL) {
    $this->data['title callback'] = $callback;
    $this->setOrUnset('title arguments', $args);
    return $this;
  }

  function description($description) {
    $this->data['description'] = $description;
    return $this;
  }

  function linkOptions($options) {
    $this->data['options'] = $options;
  }

  function linkOption($key, $value) {
    $this->data['options'][$key] = $value;
  }

  //                                                       Menu and tab position
  // ---------------------------------------------------------------------------

  function weight($weight) {
    $this->data['weight'] = $weight;
    return $this;
  }

  function menuName($name) {
    $this->data['menu_name'] = $name;
  }

  function context($context) {
    $this->data['context'] = $context;
  }

  function tabParent($parent) {
    $this->data['tab_parent'] = $parent;
  }

  function tabRoot($root) {
    $this->data['tab_root'] = $root;
  }

  function position($position) {
    $this->data['position'] = $position;
  }

  //                                                    Page, delivery and theme
  // ---------------------------------------------------------------------------

  function pageCallback($callback, $args = NULL, $file = NULL) {
    $this->data['page callback'] = $callback;
    $this->setOrUnset('page arguments', $args);
    $this->setOrUnset('file', $file);
    return $this;
  }

  function form($form_id, $more_args = array(), $file = NULL) {
    $this->data['page callback'] = 'drupal_get_form';
    $this->data['page arguments'] = array_merge(array($form_id), $more_args);
    $this->setOrUnset('file', $file);
    return $this;
  }

  function deliveryCallback($callback) {
    $this->data['delivery callback'] = $callback;
  }

  function file($file, $file_path = NULL) {
    $this->data['file'] = $file;
    $this->setOrUnset('file path', $file_path);
  }

  function themeCallback($callback, $args = NULL) {
    $this->setOrUnset('theme callback', $callback);
    $this->setOrUnset('theme arguments', $args);
  }
}
