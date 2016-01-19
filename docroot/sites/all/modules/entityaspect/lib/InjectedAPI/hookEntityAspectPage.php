<?php


class entityaspect_InjectedAPI_hookEntityAspectPage {

  protected $info;

  /**
   * Constructor.
   *
   * @param array &$info
   *   An array that will be modified while the hook is rolling.
   */
  function __construct(array &$info) {
    $this->info =& $info;
  }

  /**
   * Set title and weight of a local task.
   * Combination of ->title() and ->weight().
   *
   * @param string $title
   *   The localized title to be displayed in a breadcrumb item or tab.
   * @param int $weight
   *   The weight of the tab.
   */
  function tab($title, $weight = NULL) {
    $this->info['title'] = $title;
    if (isset($weight)) {
      $this->info['weight'] = $weight;
    }
    return $this;
  }

  function tabDefault($title) {
    $this->tab($title, -30);
    $this->info['type'] = MENU_DEFAULT_LOCAL_TASK;
  }

  /**
   * Set the title that will be displayed on a breadcrumb item or tab.
   *
   * @param string $title
   *   The localized title
   */
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
    $this->info['page arguments'] = $args;
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

  /**
   * Grant access at this path, and
   * set the page callback to render the entity in the given view mode.
   *
   * @param string $view_mode
   *   View mode to render the entity at this path.
   */
  function viewMode($view_mode) {
    $this->info['view_mode'] = $view_mode;
    $this->info['access'] = TRUE;
    unset($this->info['page callback']);
    unset($this->info['redirect']);
    return $this;
  }

  /**
   * Grant access at this path, and
   * set the page callback to redirect to a different location
   *
   * @param string $path
   *   Path to redirect to.
   */
  function redirect($path) {
    $this->info['redirect'] = $path;
    $this->info['access'] = TRUE;
    unset($this->info['page callback']);
    unset($this->info['view_mode']);
  }

  /**
   * Set the page title that is shown if this page is visited.
   *
   * @param string $title
   *   The localized page title.
   */
  function pageTitle($title) {
    $this->info['page_title'] = $title;
    return $this;
  }

  /**
   * Alter the weight of the tab linking to this page.
   *
   * @param int $weight
   *   Weight for the tab.
   */
  function weight($weight) {
    $this->info['weight'] = $weight;
    return $this;
  }

  /**
   * Define the 'page' param for entity view.
   *
   * @param bool $bool
   *   If TRUE, the node will be rendered as "page".
   */
  function renderAsPage($bool) {
    $this->info['render as page'] = $bool;
    return $this;
  }

  /**
   * Make it so that no tab is shown for this path.
   */
  function noTab() {
    $this->info['tab'] = FALSE;
    return $this;
  }

  /**
   * Register a callback pageapi module.
   * Only has an effect if pageapi module is installed.
   *
   * @param string $callback
   *   Callback function
   * @param array $extra_args
   *   Additional arguments to pass into the callback function
   */
  function pageapiCallback($callback, array $extra_args = array()) {
    $this->info['pageapi_callback'] = $callback;
    $this->info['pageapi_arguments'] = $extra_args;
    return $this;
  }

  /**
   * TODO: What does this do exactly?
   */
  function override($weight) {
    $this->info['override'] = $weight;
    return $this;
  }
}
