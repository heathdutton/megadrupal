<?php


class nodeaspect_InjectedAPI_hookNodeaspectPage {

  protected $info;

  function __construct(array &$info) {
    $this->info =& $info;
  }

  /**
   * Set title and weight of a local task.
   * Combination of ->title() and ->weight().
   */
  function tab($title, $weight = NULL) {
    $this->info['title'] = $title;
    if (isset($weight)) {
      $this->info['weight'] = $weight;
    }
    return $this;
  }

  function title($title) {
    $this->info['title'] = $title;
    return $this;
  }

  /**
   * As an alternative to the view_mode, this method allows to set a page
   * callback, to be called when the page is to be rendered.
   */
  function pageCallback($callback, array $args) {
    $this->info['page callback'] = $callback;
    $this->info['page arguments'] = $callback;
    $this->info['access'] = TRUE;
    unset($this->info['view_mode']);
    unset($this->info['redirect']);
    return $this;
  }

  /**
   * File to include for the page callback.
   */
  function file($path, $module = NULL) {
    if (!isset($module)) {
      $module = $this->info['module'];
    }
    if ($module !== FALSE) {
      $path = drupal_get_path('module', $module) . '/' . $path;
    }
    $this->info['file'] = $path;
    return $this;
  }

  function viewMode($view_mode) {
    $this->info['view_mode'] = $view_mode;
    $this->info['access'] = ($view_mode === 'full') || user_access('nodeaspect view ' . $view_mode);
    unset($this->info['page callback']);
    unset($this->info['redirect']);
    return $this;
  }

  function redirect($path) {
    $this->info['redirect'] = $path;
    $this->info['access'] = TRUE;
    unset($this->info['page callback']);
    unset($this->info['view_mode']);
  }

  function pageTitle($title) {
    $this->info['page_title'] = $title;
    return $this;
  }

  /**
   * Weight of the tab linking to this page.
   */
  function weight($weight) {
    $this->info['weight'] = $weight;
    return $this;
  }

  function renderAsPage($bool) {
    $this->info['render as page'] = $bool;
    return $this;
  }

  function renderLinks($bool) {
    $this->info['render links'] = $bool;
    return $this;
  }

  function renderAsTeaser($bool) {
    $this->info['render as teaser'] = $bool;
    return $this;
  }

  function renderComments($bool) {
    $this->info['render comments'] = $bool;
    return $this;
  }

  function noTab() {
    $this->info['tab'] = FALSE;
    return $this;
  }

  function pageapiCallback($callback, array $extra_args = array()) {
    $this->info['pageapi_callback'] = $callback;
    $this->info['pageapi_arguments'] = $extra_args;
    return $this;
  }

  function override($weight) {
    $this->info['override'] = $weight;
    return $this;
  }
}
