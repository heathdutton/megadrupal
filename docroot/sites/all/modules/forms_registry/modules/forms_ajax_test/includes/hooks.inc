<?php
/**
 * @file
 * Hooks implementations for the Ajax Forms Test module
 */

/**
 * Provides hooks implementations for the Ajax Forms Test module.
 */
class FormsAjaxTestHooks {
  /**
   * Provides an implementation for hook_menu().
   */
  public static function menu() {
    $items = array();
    $path = drupal_get_path('module', 'forms_ajax_test') . '/pages';

    $items[FormsAjaxTestConfig::TEST_PAGE_PATH] = array(
      'type' => MENU_CALLBACK,
      'access arguments' => array('access content'),
      'page callback' => 'forms_ajax_test_page',
      'file' => 'forms_ajax_test.inc',
      'file path' => $path,
    );

    return $items;
  }

  /**
   * Provides an implementation for hook_forms_registry().
   */
  public static function formsRegistry() {
    $items = array();

    $items[FormsAjaxTestConfig::AJAX_SELECT_EXAMPLE] = array(
      'files' => array(
        array(
          'ext' => 'inc',
          'module' => 'forms_ajax_test',
          'name' => 'forms/ajax_dependent_selects',
        ),
        array(
          'ext' => 'inc',
          'module' => 'forms_ajax_test',
          'name' => 'forms/continents',
        ),
        array(
          'ext' => 'inc',
          'module' => 'forms_ajax_test',
          'name' => 'forms/countries',
        ),
        array(
          'ext' => 'inc',
          'module' => 'forms_ajax_test',
          'name' => 'forms/cities',
        )
      ),
    );

    return $items;
  }
}