<?php


/**
 * API object, to be injected in hook_nodeaspect().
 */
class nodeaspect_InjectedAPI_hookNodeaspect {

  protected $pages;
  protected $viewModes;

  function __construct(array &$pages, array &$view_modes) {
    $this->pages =& $pages;
    $this->viewModes =& $view_modes;
  }

  function page($node_subpath) {
    $this->pages[$node_subpath] = TRUE;
  }

  /**
   * View modes are called build modes in Drupal 6.
   * We support both, to reduce the diff between the D6 and D7 versions.
   */
  function buildmode($buildmode_suffix, $title) {
    $this->viewModes[$buildmode_suffix] = array(
      'title' => $title,
      'views style' => TRUE,
    );
  }

  function viewMode($view_mode, $title) {
    $this->viewModes[$view_mode] = array(
      'label' => t($title),
      'custom settings' => FALSE,
    );
  }
}
