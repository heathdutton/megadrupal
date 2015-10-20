<?php

/**
 * @file
 * Provides hook implementations for the custom forms_test module.
 */

class FormsTestHooks {

  /**
   * Provides an implementation for hook_menu().
   */
  public static function menu() {
    $items = array();
    $path = drupal_get_path('module', 'forms_test') . '/pages';

    $items[FormsTestConfig::TEST_PAGE_PATH] = array(
      'title' => 'Forms Test Page',
      'description' => 'The main Forms Test page',
      'page callback' => 'forms_test_simple_test_page',
      'access arguments' => array('access content'),
      'file path' => $path,
      'file' => 'simple_test.inc',
    );

    return $items;
  }

  /**
   * Provides an implementation for hook_forms_registry().
   */
  public static function formsRegistry() {
    $items = array();

    $items[FormsTestConfig::SIMPLE_TEST_FORM_ID] = array(
      'files' => array(
        array(
          'ext' => 'inc',
          'module' => 'forms_test',
          'name' => 'forms/simple_test',
        ),
        array(
          'ext' => 'inc',
          'module' => 'forms_test',
          'name' => 'forms/countries',
        ),
      ),
    );

    return $items;
  }

}