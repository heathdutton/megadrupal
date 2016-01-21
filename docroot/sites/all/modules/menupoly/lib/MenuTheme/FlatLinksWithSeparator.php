<?php


/**
 * A menu theme that renders the links without the ul / li wrapping.
 * This only works for "flat" menus without submenus.
 */
class menupoly_MenuTheme_FlatLinksWithSeparator extends menupoly_MenuTheme_Abstract {

  /**
   * @var string
   *   Separator string to show between links.
   */
  protected $separator;

  /**
   * @param string $separator
   *   Separator string to show between links.
   */
  function __construct($separator = '') {
    $this->separator = $separator;
  }

  /**
   * {@inheritdoc}
   */
  function renderMenuItem($item, $options, $submenu_html) {
    // Assume $submenu_html to be empty.
    return l($item['title'], $item['href'], $options);
  }

  /**
   * {@inheritdoc}
   */
  function renderMenuItem__no_access($item, $options, $submenu_html) {
    // Do not display items that the user cannot access.
    return NULL;
  }

  /**
   * {@inheritdoc}
   */
  function renderMenuTree($items_html) {
    // Display the links with a separator.
    return implode($this->separator, $items_html);
  }
}
