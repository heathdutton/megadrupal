<?php

/**
 * Object that can render various parts of a menu.
 */
interface menupoly_MenuTheme_Interface {

  /**
   * @param array &$items
   *   Process the items of a submenu, before it is rendered.
   *
   * @todo Document optional $depth argument.
   *   (It is only optional for backwards compatibility)
   */
  function processSubmenuItems(&$items);

  /**
   * @param array $item
   * @param array $options
   * @param string $submenu_html
   *   Rendered HTML for the submenu.
   *
   * @return string
   *   Rendered menu item.
   */
  function renderMenuItem($item, $options, $submenu_html);

  /**
   * Render an item where the user has permission to view the menu item, but not
   * the destination url.
   *
   * @param array $item
   * @param array $options
   * @param string $submenu_html
   *   Rendered HTML for the submenu.
   *
   * @return string
   *   Rendered menu item.
   */
  function renderMenuItem__no_access($item, $options, $submenu_html);

  /**
   * Render an entire submenu.
   *
   * @param array $items_html
   *   HTML for each item.
   *
   * @return string
   *   Rendered submenu.
   */
  function renderMenuTree($items_html);
}
