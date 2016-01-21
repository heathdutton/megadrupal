<?php


/**
 * A quick theme that does not have classes for zebra striping or first/last.
 */
class menupoly_MenuTheme_Minimal extends menupoly_MenuTheme_Static {

  /**
   * Add first/last and odd/even classes.
   *
   * {@inheritdoc}
   */
  function processSubmenuItems(&$items) {
    // No zebra striping or first/last.
  }
}
