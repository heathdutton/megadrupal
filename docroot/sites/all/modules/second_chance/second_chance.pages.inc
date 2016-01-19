<?php
/**
 * @file
 * Menu callbacks.
 */

/**
 * Menu callback for Listing deleted entities.
 *
 * TODO Views may be better here?
 *
 * @param string $entity_type
 *   Display deleted entities of this type.
 */
function second_chance_list($entity_type) {
  $info = entity_get_info($entity_type);

  $query = db_select('second_chance', 'sc');
  $query->fields('sc', array('scid', 'bundle', 'label', 'date', 'uid'));
  $query->condition('sc.entity_type', $entity_type);
  $query->orderBy('date', 'DESC');
  $items = $query->execute()->fetchAllAssoc('scid');

  if (!$items) {
    return array(
      '#markup' => '<p>' . t('The recycle bin is currently empty.') . '</p>',
    );
  }

  $table_rows = array();
  $prefetch_uids = array();
  foreach ($items as $item) {
    $prefetch_uids[] = $item->uid;
  }
  $users = user_load_multiple($prefetch_uids);

  foreach ($items as $item) {
    $table_rows[] = array(
      $item->label,
      $info['bundles'][$item->bundle]['label'],
      format_date($item->date),
      isset($users[$item->uid]) ? theme('username', array('account' => $users[$item->uid])) : t('Deleted user'),
      l(t('Restore'), 'admin/content/second-chance/restore/' . $item->scid),
    );
  }

  return array(
    '#theme' => 'table',
    '#rows' => $table_rows,
    '#header' => array('Title', 'Content type', 'Date deleted', 'User', ''),
  );
}

/**
 * Attempt to restore an entity.
 *
 * TODO Move to instance method of a SecondChanceItem class.
 * TODO Attempt to support things other than nodes.
 *
 * @param object $item
 *   A second chance item.
 *
 * @throws \SecondChanceRestoreException
 *   If any part of the restoration fails.
 */
function second_chance_restore_item($item) {

  if ($item->entity_type == 'node') {
    $node = $item->entity;
    $nid = $item->entity->nid;

    if (node_load($nid)) {
      throw new SecondChanceRestoreException("Target entity already exists before restore");
    }

    if (!$node->nid) {
      throw new SecondChanceRestoreException("Entity doesn't have an ID");
    }

    $node->is_new = TRUE;
    node_save($node);

    if (!$node->nid) {
      throw new SecondChanceRestoreException("Entity doesn't have an ID after restore");
    }

    if ($node->nid != $nid) {
      throw new SecondChanceRestoreException('Entity ID has changed after restore');
    }

    // Make sure that we have a new node from the DB.
    $node = node_load($nid, NULL, TRUE);

    if (!$node) {
      throw new SecondChanceRestoreException("Entity didn't save or cannot now be loaded.");
    }

    db_delete('second_chance')
      ->condition('scid', $item->scid)
      ->execute();

    drupal_set_message(t('%title has been restored.', array('%title' => $node->title)));
    drupal_goto('node/' . $nid);
  }
}
