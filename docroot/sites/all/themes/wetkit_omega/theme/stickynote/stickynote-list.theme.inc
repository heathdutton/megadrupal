<?php
/**
 * @file
 * stickynot-list.theme.inc
 */


/**
 * Get notes for referring_url/current url return as list.
 *
 * @param array $variables
 *   Variables including referring_url, notes, can_edit, can_delete for theming.
 */
function wetkit_omega_stickynote_list($variables) {
  $output = '';
  // In this case, $result is a list of nodes returned from a database query.
  $list = array();
  $list['type'] = 'ol';
  $list['title'] = '';
  $list['attributes'] = array();
  $list['items'] = array();
  if (isset($variables['notes'])) {
    foreach ($variables['notes'] as $key => $note) {
      $item = '<span class="note-' . $key . '">';
      $item .= $note['data']['created']['data'] . ': ';
      $item .= $note['data']['note']['data'];
      if ($variables['can_edit']) {
        $item .= $note['data']['edit']['data'];
      }
      if ($variables['can_delete']) {
        $item .= $note['data']['delete']['data'];
      }
      $item .= '</span>';
      $list['items'][] = array('data' => $item, 'id' => 'note-' . $key);
    }
  }
  $output = theme('item_list', $list);
  return $output;
}
