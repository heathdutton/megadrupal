<?php

/**
 * @file
 * Drush integration for WYSIWYG editors.
 */

class DrushEditorManager {
  /**
   * A list of editors.
   */
  protected $editors = array();

  /**
   * Editor Manager.
   */
  public function __construct() {
    $editors = &drupal_static('drush:editors', array());

    if (empty($cache)) {
      $items = drush_command_invoke_all('drush_editor');

      if (!is_array($items)) {
        $items = array();
      }

      foreach ($items as $key => $item) {
        if (!isset($item['preset']) || !is_array($item['preset'])) {
          $items[$key]['preset'] = array();
        }
      }

      drush_command_invoke_all_ref('drush_editor_alter', $items);

      foreach ($items as $key => $item) {
        $preset = array();

        if (isset($item['preset']) && is_array($item['preset'])) {
          $preset = array_flip($item['preset']);
        }

        $items[$key]['preset'] = $preset;
      }

      $editors = $items;
    }

    $this->editors = $editors;
  }

  /**
   * Get all editors.
   *
   * @return array
   *   List of editor.
   *
   * @see hook_drush_editor()
   */
  public function getEditors() {
    return $this->editors;
  }

  /**
   * Get editor information.
   *
   * @return array|FALSE
   *   Editor information if exists or FALSE.
   */
  public function get($editor) {
    return isset($this->editors[$editor]) ? $this->editors[$editor] : FALSE;
  }

  /**
   * Load editor.
   *
   * @param string $editor
   *   Editor.
   *
   * @return bool
   *   Editor is loaded or not.
   */
  public function load($editor) {
    $editors = $this->getEditors();

    if (empty($editors[$editor]['file']) ||
        empty($editors[$editor]['class']) ||
        !file_exists($editors[$editor]['file'])
    ) {
      return FALSE;
    }

    include_once $editors[$editor]['file'];

    return class_exists($editors[$editor]['class']);
  }

  /**
   * Create new editor instance.
   */
  public function create($editor, $major_version = NULL, $version = NULL, $preset = NULL) {
    if (!$this->load($editor)) {
      return FALSE;
    }

    $editors = $this->getEditors();

    return new $editors[$editor]['class']($major_version, $version, $preset);
  }
}
