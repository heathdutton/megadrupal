<?php

/**
 * @file
 * AJAX commands.
 */

/**
 * Creates a Drupal Ajax 'insert' command that is capable of targeting elements
 * within iFrames.
 *
 * @param string $html
 *   The HTML content that will replace the matched element(s).
 *
 * @return
 *   An array suitable for use with the ajax_render() function.
 */
function entity_embed_command_insert($html) {
  return array(
    'command' => 'entityEmbedInsertEditor',
    'html' => $html,
  );
}

/**
 * Creates a Drupal Ajax 'editor dialog save' command.
 *
 * @param string $values
 *   The values that should be passed to the form constructor in Drupal.
 *
 * @return
 *   An array suitable for use with the ajax_render() function.
 */
function entity_embed_command_editor_dialog_save($values) {
  return array(
    'command' => 'editorDialogSave',
    'values' => $values,
  );
}
