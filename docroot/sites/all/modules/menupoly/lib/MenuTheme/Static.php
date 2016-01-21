<?php


/**
 * A quick theme that does not use the theme() layer for menu html.
 */
class menupoly_MenuTheme_Static extends menupoly_MenuTheme_Abstract {

  /**
   * {@inheritdoc}
   */
  function renderMenuItem($item, $options, $submenu_html) {
    $link_html = l($item['title'], $item['href'], $options);
    $attr = $this->_itemAttributes($item, $options, $submenu_html);
    return $attr->renderTag('li', $link_html . $submenu_html);
  }

  /**
   * {@inheritdoc}
   */
  function renderMenuItem__no_access($item, $options, $submenu_html) {
    $link_html = $options['html'] ? $item['title'] : check_plain($item['title']);
    $attr = $this->_itemAttributes($item, $options, $submenu_html);
    $attr->addClass('no-access');
    return $attr->renderTag('li', $link_html . $submenu_html);
  }

  /**
   * Builds the html tag attributes for an LI menu item.
   *
   * @param array $item
   * @param array $options
   * @param string $submenu_html
   *   Rendered HTML for the submenu.
   *
   * @return htmltag_TagAttributes
   *   Attributes for the LI element.
   */
  protected function _itemAttributes($item, $options, $submenu_html) {
    $attr = htmltag_tag_attributes();
    if (!empty($item['class'])) {
      $attr->addClass($item['class']);
    }
    $attr->addClass($submenu_html ? 'expanded' : ($item['has_children'] ? 'collapsed' : 'leaf'));
    $attr->addClassesIf(array(
      'active' => !empty($item['active']),
      'active-trail' => !empty($item['active-trail']),
    ));
    return $attr;
  }

  /**
   * {@inheritdoc}
   */
  function renderMenuTree($items_html) {
    return '<ul class="menu">' . implode('', $items_html) . '</ul>';
  }
}
