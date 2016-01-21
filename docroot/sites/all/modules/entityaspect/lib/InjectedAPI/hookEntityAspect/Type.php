<?php


class entityaspect_InjectedAPI_hookEntityAspect_Type {

  protected $data;

  protected $entityType;
  protected $loader;
  protected $baseItem;

  function __construct($entity_type, &$data) {

    $this->data =& $data;

    $this->entityType = $entity_type;

    $this->baseItem = array(
      'title callback' => 'entityaspect_page_title',
      'page callback' => 'entityaspect_page',
      'access callback' => 'entityaspect_page_access',
      'type' => MENU_LOCAL_TASK,
    );

    // We want a canonical wildcard loader to avoid conflicts.
    if (function_exists($entity_type . '_load')) {
      $this->loader = $entity_type;
    }
    else {
      $this->loader ='_entityaspect_entity';
      $this->baseItem['load arguments'] = array($entity_type);
    }
  }

  /**
   * Declare a page/route to be handled with entityaspect.
   *
   * @param string $route
   *   The router path, such as node/%.
   */
  function page($route, $hijack = FALSE) {
    if (preg_match('#^(.*)/(%([^/]*))(/.*|)$#', $route, $m)) {
      list(, $prepend,, $wildcard, $append) = $m;

      $n = count(explode('/', $prepend));
      $route = $prepend . '/%' . $this->loader . $append;
      $shortroute = $prepend . '/%' . $append;
      $args = array($this->entityType, $n, $shortroute);

      $item = array(
        'title arguments' => $args,
        'page arguments' => $args,
        'access arguments' => $args,
      ) + $this->baseItem;

      $this->data[$hijack ? 'hook_menu_alter' : 'hook_menu'][$route] = $item;
    }
    else {
      throw new Exception("The specified router path does not work with entityaspect.");
    }
    return $this;
  }

  /**
   * Same as page, but will use hook_menu_alter() to force.
   *
   * @param string $route
   *   The router path, such as node/%.
   */
  function hijackPage($route) {
    return $this->page($route, TRUE);
  }

  /**
   * Declare a new view mode.
   *
   * @param string $view_mode
   *   Machine key for the view mode.
   * @param string $title
   *   Human title for the view mode.
   */
  function viewMode($view_mode, $title) {
    $this->data['view_modes'][$this->entityType][$view_mode] = array(
      'label' => t($title),
      'custom settings' => FALSE,
    );
    return $this;
  }
}
