<?php
/**
 * @file
 * MenuGeneratorEngineHtmlCrawl classe.
 */

/**
 * Class MenuGeneratorEngineHtmlCrawl for crawling a menu(s) from a live site.
 *
 * NOTICE this class has not been tested since being refactored!
 */
class MenuGeneratorEngineHtmlCrawl extends MenuGeneratorEngine {
  protected $queryPath;
  protected $initialCssSelector;
  protected $menu;

  /**
   * Constructor.
   */
  public function __construct(MenuGenerationParametersHtmlCrawl $parameters) {
    $this->parameters = $parameters;
  }


  /**
   * {@inheritdoc}
   */
  public function generate() {
    $this->recurse();
    $this->getHtmlContent();

    return $this->menuFileContent;
  }

  /**
   * Setter.
   */
  public function setInitialCssSelector($initial_css_selector) {
    $this->initialCssSelector = $initial_css_selector;
  }

  /**
   * Get a qp() object.
   *
   * @param string $url
   *   Optional url to load as the new document.
   *
   * @return QueryPath
   *   The query path object.
   */
  protected function getQueryPath($url = NULL) {
    if (!empty($url)) {
      $this->parameters->setUriMenuLocation($url);
      $this->queryPath = NULL;

    }
    if (!$this->queryPath) {
      require_once DRUPAL_ROOT . '/sites/all/vendor/querypath/querypath/src/qp.php';
      $page_uri = $this->parameters->getUriMenuLocation();
      $ch = curl_init($page_uri);
      curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
      curl_setopt($ch, CURLOPT_USERAGENT, "Mozilla/4.0 (compatible; MSIE 5.01; Windows NT 5.0)");
      $html = curl_exec($ch);
      $response = curl_getinfo($ch, CURLINFO_HTTP_CODE);
      if ($response != 200) {
        drush_migration_tools_debug_output("The page at '$page_uri' returned a status of $response.  You might need to adjust '--menu-location-uri'.");
      }

      curl_close($ch);
      $this->queryPath = htmlqp($html);
      $this->cleanQueryPathHtml();
    }

    return $this->queryPath;
  }

  /**
   * Performs any cleaning on QueryPath object once, upon querypath creation.
   */
  protected function cleanQueryPathHtml() {
    // Intentionally empty, to be overidden.
  }

  /**
   * Returns the text content representing the menu import file.
   *
   * @return string
   *   The text content representing the menu import file.
   * @throws Exception
   *   If there is no menu to be processd.
   */
  protected function getHtmlContent() {
    if (empty($this->menuFileContent)) {
      if (empty($this->menu)) {
        throw new Exception("No menu was found.  You might be targetting the wrong page or selector.");
      }

      // Go get each level 1 page,  process its menu.
      $this->processNextLevelPages();

      // Apply prepended links.
      $this->menu = array_merge($this->getLinkstoPrepend(), $this->menu);

      // Apply appended links.
      $this->menu = array_merge($this->getLinkstoAppend(), $this->menu);
      $menu_elements = count($this->menu);
      $this->menuFileContent = '';
      $this->menuCounter = 0;
      $this->collapseMenu();
      $this->elementsToContent($this->menu);
    }
    drush_migration_tools_debug_output("Built a menu with {$this->menuCounter} items from $menu_elements elements.");

    return $this->menuFileContent;
  }

  /**
   * Processes an array of menu elements recursing into the children.
   *
   * Each menu element is written as a line in the menu.txt file to import.
   *
   * @param array $elements
   *   Any array of menu elements.
   */
  protected function elementsToContent(array $elements) {
    foreach ($elements as $element) {
      $this->menuFileContent .= $element['prefix'] . $element['title'] . "{\"url\":\"" . $element['url'] . "\"}\n";
      $this->menuCounter++;
      if (!empty($element['children'])) {
        $this->elementsToContent($element['children']);
      }
    }
  }

  /**
   * Crawls pages present the primary menu and adds second level menu items.
   */
  protected function processNextLevelPages() {
    if (is_array($this->menu) && $this->parameters->recurse) {
      $org_url = $this->parameters->getUriMenuLocationBasePath();
      $org_path = parse_url($org_url, PHP_URL_PATH);
      foreach ($this->menu as $uri => $item) {
        $in_group = (stripos($uri, $org_path) === FALSE) ? FALSE : TRUE;
        if (empty($item->prefix) && $in_group) {
          // This is a first tier page with a path in this group so process it.
          $query = $this->getQueryPath($uri);
          drush_migration_tools_debug_output("PROCESSING: " . $this->parameters->getUriMenuLocation());
          $this->recurse();
        }
      }
    }
  }

  /**
   * Moves any child elements into the children array of the parent.
   */
  protected function collapseMenu() {
    $prefix_level = $this->getPrefixMax();
    // Must process the deepest children first.
    while ($prefix_level >= 0) {
      foreach ($this->menu as $uri => &$item) {
        if ((strlen($item['prefix']) == $prefix_level) && !empty($item['parent_uri'])) {
          // Copy the item to its parent if the parent exists and
          // it is not already there.
          if ((!empty($this->menu[$item['parent_uri']])) && (empty($this->menu[$item['parent_uri']]['children'][$uri]))) {
            $this->menu[$item['parent_uri']]['children'][$uri] = $item;
            // Remove the original item.
            unset($this->menu[$uri]);
          }
          elseif (empty($this->menu[$item['parent_uri']])) {
            // The parent does not exist.
            // Copy it to the end of the menu as an orphan for hand adjustment.
            if (empty($this->menu['orphans'])) {
              $this->menu['orphans'] = array();
            }
            $this->menu['orphans'][$uri] = $item;
            // Remove the original item.
            unset($this->menu[$uri]);
          }

        }
      }
      // Break the reference.
      unset($item);
      $prefix_level--;
    }
    drush_migration_tools_debug_output('Menu object after collapsing:');
    drush_migration_tools_debug_output($this->menu);
    if (!empty($this->menu['orphans'])) {
      drush_migration_tools_debug_output('Menu orphans present:');
      drush_migration_tools_debug_output($this->menu['orphans']);
    }
    else {
      drush_migration_tools_debug_output('Menu collapsed without orphans.');
    }
  }

  /**
   * Gets the maximum number of prefixes found in the menu.
   *
   * @return int
   *   The maximum number of prefixes found in the menu.
   */
  private function getPrefixMax() {
    $max = 0;
    foreach ($this->menu as $items) {
      $count = strlen($items['prefix']);
      $max = ($count > $max) ? $count : $max;
    }
    return $max;
  }

  /**
   * Recursive function that processes a menu level.
   *
   * @param string $css_selector
   *   The css selector to get the ul we are to process.
   *
   * @param string $prefix
   *   The level of depth we are into represented by dashes. "" level 0, "-"
   *   level 1, and so on.
   * @param string $parent_uri
   *   The uri of the item's parent (optional).
   */
  protected function recurse($css_selector = NULL, $prefix = '', $parent_uri = '') {
    module_load_include("inc", "migration_tools", "includes/migration_tools");
    $last_uri = '';
    if (!isset($css_selector)) {
      // This is the first run through a subpage so get the menu selector.
      $css_selector = $this->initialCssSelector;
    }

    drush_migration_tools_debug_output("{$prefix}CSS SELECTOR: $css_selector child of $parent_uri");
    $query = $this->getQueryPath();
    $elements = $query->find($css_selector)->children();

    foreach ($elements as $elem) {
      $tag = $elem->tag();
      switch ($tag) {
        case 'li':
          drush_migration_tools_debug_output("$prefix I'm in $tag $css_selector child of $parent_uri");
          $children = $elem->children();
          foreach ($children as $child) {
            // Might be an anchor or might be a cluster of items.
            if ($child->tag() == 'a') {
              $this->addMenuElement($child->attr("href"), $child->text(), $prefix, $parent_uri);
              $last_uri = $child->attr("href");
            }
            else {
              $class_name = self::generaterandomstring();
              $elem->attr('class', $class_name);
              $this->recurse(".{$class_name}", $prefix, $last_uri);
            }
          }
          break;

        case 'ul':
        case 'div':
        default:
          $last_uri = (empty($last_uri)) ? $parent_uri : $last_uri;
          $new_prefix = ($tag == 'ul') ? $prefix . "-" : $prefix;
          $class_name = self::generaterandomstring();
          $elem->attr('class', $class_name);
          drush_migration_tools_debug_output("$new_prefix I'm in $tag $class_name child of $last_uri");
          $this->recurse(".{$class_name}", $new_prefix, $last_uri);
          break;
      }
    }
  }

  /**
   * Any menu items returned will be placed at the top of the menu import.
   *
   * @return array
   *   Menu elements to add to the top of the menu
   */
  public function getLinkstoPrepend() {
    return array();
  }

  /**
   * Any menu items returned will be placed at the bottom of the menu import.
   *
   * @return array
   *   Menu element array to add to the bottom of the menu.
   */
  public function getLinksToAppend() {
    return array();
  }

  /**
   * Adds a single menu item to $this->menu.
   *
   * @param string $link_path
   *   The url or uri of the link.
   * @param string $link_title
   *   The text to be used as the link.
   * @param string $prefix
   *   The prefix the title with that indicates the item's depth (optional).
   * @param string $parent_uri
   *   The uri of the menu items parent (optional).
   */
  public function addMenuElement($link_path, $link_title, $prefix = '', $parent_uri = '') {
    $link_path = $this->normalizeUri($link_path);

    if (empty($this->menu[$link_path])) {
      // This menu item does not exist yet so add it.
      $parent_uri = (empty($parent_uri)) ? '' : $this->normalizeUri($parent_uri);
      $this->menu[$link_path] = array(
        'prefix' => $prefix,
        'url' => $link_path,
        'parent_uri' => $parent_uri,
        'title' => $link_title,
        'children' => array(),
      );
      drush_migration_tools_debug_output("ADDED: $link_path to menu");
    }
    else {
      drush_migration_tools_debug_output("ALREADY HAVE: $link_path in menu");
    }
  }

  /**
   * Generate a random string.
   *
   * @return string
   *   Random string.
   */
  public static function generateRandomString() {
    $characters = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
    $randstring = '';
    for ($i = 0; $i < 10; $i++) {
      $randstring .= $characters[rand(0, strlen($characters))];
    }
    return $randstring;
  }

  /**
   * Menu getter for the menu array.
   *
   * @return array
   *   The menu array.
   */
  public function getMenu() {
    return $this->menu;
  }
}
