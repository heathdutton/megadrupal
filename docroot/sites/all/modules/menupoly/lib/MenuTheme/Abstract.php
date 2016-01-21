<?php


/**
 * A quick theme that does not use the theme() layer for menu html.
 */
abstract class menupoly_MenuTheme_Abstract implements menupoly_MenuTheme_Interface {

  /**
   * Add first/last and odd/even classes.
   *
   * {@inheritdoc}
   */
  function processSubmenuItems(&$items) {
    $i = 0;
    $n = count($items);
    foreach ($items as $k => $item) {
      $class = ($i % 2) ? 'odd' : 'even';
      if ($i == 0) {
        $class .= ' first';
      }
      if ($i == $n - 1) {
        $class .= ' last';
      }
      $items[$k]['class'] = isset($item['class'])
        ? $item['class'] . ' ' . $class
        : $class;
      ++$i;
    }
  }
}
