<?php
/**
 * @file
 * Provides theme output for notes tied to specific nodes.
 */

/**
 * Returns themed output off the notes attached to the current node.
 *
 * @param object $node
 *   The node object.
 */
function node_notes_view($node) {
  $path = drupal_get_path('module', 'node_notes');
  drupal_add_css($path . '/css/node_notes.css');

  $notes = db_select('node_notes', 'notes')
    ->condition('nid', $node->nid, '=')
    ->extend('PagerDefault')
    ->limit(10)
    ->fields('notes')
    ->orderBy('created', 'DESC')
    ->execute()
    ->fetchAll();

  $notes = array(
    '#theme' => 'node_notes_wrapper',
    '#notes' => $notes,
    '#node' => $node,
  );

  if (user_access('create ' . $node->type . ' notes') || user_access('administer notes')) {
    $build['node_notes_form'] = drupal_get_form('node_notes_form', $node);
  }

  $build['node_notes'] = $notes;

  return $build;
}

/**
 * Process variables for node_notes-wrapper.tpl.php.
 *
 * @see node_notes-wrapper.tpl.php
 */
function template_preprocess_node_notes_wrapper(&$variables) {
  global $user;
  $variables['notes_list'] = '';

  foreach ($variables['notes'] as $note) {
    $author = theme('username', array('account' => user_load($note->uid)));

    $links = '';

    $delete_own_notes = (user_access('delete own ' . $variables['node']->type . ' notes') && ($note->uid == $user->uid));
    $delete_any_notes = user_access('delete any ' . $variables['node']->type . ' notes');
    $administer_notes = user_access('administer notes');

    if ($delete_own_notes || $delete_any_notes || $administer_notes) {
      $links = l(t('Delete'), 'node/' . $variables['node']->nid . '/notes/' . $note->noteid . '/delete');
    }

    $variables['notes_list'] .= theme(
      'node_notes_note',
      array(
        'noteid' => $note->noteid,
        'nid' => $note->nid,
        'uid' => $note->uid,
        'author' => $author,
        'created' => format_date($note->created),
        'body' => check_markup($note->body, $note->format),
        'links' => $links,
      )
    );
  }

  $variables['pager'] = theme('pager', array('tags' => array()));
  $variables['theme_hook_suggestions'][] = 'node_notes-wrapper__' . $variables['node']->type;
}

/**
 * Process variables for node_notes-note.tpl.php.
 *
 * @see node_notes-note.tpl.php
 */
function template_preprocess_node_notes_note(&$variables) {
  $classes_array = array(
    'node-note',
  );
  $variables['classes_array'] = $classes_array;
  $variables['classes'] = implode(' ', $classes_array);
}
