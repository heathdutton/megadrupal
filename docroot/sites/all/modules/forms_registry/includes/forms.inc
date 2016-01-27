<?php
/**
 * @file
 * Provides a helper class for loading forms.
 */

/**
 * A helper class for loading forms.
 */
class Forms {

  /**
   * Loads a form based on the form id.
   *
   * @param string $id
   *
   * @return array
   */
  public static function get($id) {
    self::includeFiles($id);
    return drupal_get_form($id);
  }

  /**
   * Includes the files required by the form.
   *
   * @param string $id
   */
  public static function includeFiles($id) {
    $form_data = FormsRegistry::get($id);
    if (!empty($form_data['files'])) {
      foreach ($form_data['files'] as $f) {
        module_load_include($f['ext'], $f['module'], $f['name']);
      }
    }
  }

}