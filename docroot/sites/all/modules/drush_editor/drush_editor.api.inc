<?php

/**
 * @file
 * Drush integration for WYSIWYG editors.
 */

/**
 * Register list of editors to Drush.
 *
 * Each item may have the following properties
 * - class: Class name of editor, this will help to execute all the commands.
 * - file: The file where defines the class for editor.
 * - preset: List of editor presets.
 *
 * @return array
 *   List of editors.
 */
function hook_drush_editor() {
  $path = dirname(__FILE__);

  $editors['ckeditor'] = array(
    'class'  => 'DrushEditorCKEditor',
    'file'   => $path  . '/lib/editor/ckeditor/DrushEditorCKEditor.php',
    'preset' => array('basic', 'standard', 'full'),
  );

  $editors['tinymce'] = array(
    'class'  => 'DrushEditorTinyMCE',
    'file'   => $path  . '/lib/editor/tinymce/DrushEditorTinyMCE.php',
    'preset' => array('jquery'),
  );

  return $editors;
}

/**
 * Change the list of registered editors.
 *
 * @param array $editors
 *   List of editors.
 */
function hook_drush_editor_alter(&$editors) {
  $editors['tinymce']['preset'][] = 'core';
}
