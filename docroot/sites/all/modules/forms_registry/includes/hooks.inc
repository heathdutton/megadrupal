<?php
/**
 * @file
 * Provides hook implementations for the forms module.
 */


class FormsHooks {

  /**
   * Provides an implementation for hook_menu_alter().
   */
  public static function menuAlter(&$items) {
    $items['system/ajax']['page callback'] = 'forms_ajax_form_callback';
  }

}