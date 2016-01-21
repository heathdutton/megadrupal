<?php


class menupoly_MenuTheme_EntityTeaser extends menupoly_MenuTheme_Abstract {

  private $routes = array();

  static function createDefault() {
    $instance = new self();
    $instance->addEntityRoute('node/%', 'node', 1, 'teaser');
    return $instance;
  }

  /**
   * @param string $route
   *   E.g. 'node/%'.
   * @param string $type
   *   E.g. 'node'.
   * @param int|null $index
   *   Index of the url fragment with the entity id.
   * @param string $view_mode
   *   E.g. 'teaser'.
   *
   * @return $this
   *
   * @throws Exception
   */
  function addEntityRoute($route, $type, $index = NULL, $view_mode = 'teaser') {
    $fragments = explode('/', $route);
    if (isset($index)) {
      if ($index !== (int)$index) {
        throw new \Exception("Index must be numeric or NULL.");
      }
      if (!isset($fragments[$index]) || '%' !== $fragments[$index]) {
        throw new \Exception("Route fragment at index $index must be '%'.");
      }
    }
    else {
      $index = array_search('%', $fragments);
      if (FALSE === $index) {
        throw new \Exception("Route must contain '%' fragment.");
      }
    }
    $this->routes[$route] = array($type, $index, $view_mode);
    return $this;
  }

  /**
   * {@inheritdoc}
   */
  function renderMenuItem($item, $options, $submenu_html) {
    $router_item = menu_get_item($item['href']);
    if (!$router_item) {
      return NULL;
    }
    $map = $router_item['map'];
    $route = $router_item['path'];
    if (!isset($this->routes[$route])) {
      return NULL;
    }
    list($type, $index, $view_mode) = $this->routes[$route];
    $renderable = entity_view($type, array($map[$index]), $view_mode);
    if (empty($renderable)) {
      return NULL;
    }
    return render($renderable);
  }

  /**
   * {@inheritdoc}
   */
  function renderMenuItem__no_access($item, $options, $submenu_html) {
    return NULL;
  }

  /**
   * {@inheritdoc}
   */
  function renderMenuTree($items_html) {
    $items_html = implode('', $items_html);
    $attributes = htmltag_class_attribute('menu');
    return $attributes->DIV($items_html);
  }
}
