<?php

class menupoly_Main {

  /**
   * @var menupoly_ServiceCache
   *   Object which provides lazy-created service objects.
   */
  protected $services;

  /**
   * @param menupoly_ServiceCache
   *   Object which provides lazy-created service objects.
   */
  function __construct($services) {
    $this->services = $services;
  }

  /**
   * @param array $settings
   *   Array of settings that define a menu tree.
   *
   * @return array
   *   Drupal-renderable array.
   */
  function settingsToRenderArray($settings) {
    return array(
      '#theme' => 'menupoly',
      '#menupoly' => $settings,
    );
  }

  /**
   * @param array $settings
   *   Array of settings that define a menu tree.
   *
   * @return string|null
   *   Rendered HTML, or NULL if empty.
   */
  function settingsToHtml($settings) {

    $tree = $this->settingsToMenuTree($settings);

    if (!empty($tree)) {
      // Render the tree.
      $menu_theme = $this->services->settingsProcessor->settingsResolveMenuTheme($settings);
      $html = $tree->render($menu_theme);
      return $html;
    }

    return NULL;
  }

  /**
   * @param array $settings
   *   Array of settings that define a menu tree.
   *
   * @throws Exception
   * @return menupoly_MenuTree|null
   *   Menu tree object, ready to render itself, or NULL if empty.
   */
  function settingsToMenuTree($settings) {

    // Normalize the settings array.
    $this->services->settingsProcessor->processSettings($settings);

    // Create a MenuTreeSource object based on 'menu_links'.
    $source = $this->services->menuTreeSource('menu_links');
    if (!is_object($source)) {
      throw new Exception("Source must be an object.");
    }

    list($root_mlid, $items) = $source->build($settings);
    if (empty($items)) {
      return NULL;
    }

    $this->services->accessChecker->itemsCheckAccess($items);

    // Build the MenuTree object.
    $tree = new menupoly_MenuTree($root_mlid);
    $tree->addItems($items);

    return $tree;
  }
}
