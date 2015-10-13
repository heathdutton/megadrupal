<?php

/**
 * @file Worker functions for pathauto_regen.module.
 */

/**
 * Implements hook_node_update().
 *
 * Invokes a batch operation to update node path aliases.
 */
function pathauto_regen_node_update($node) {

  if (empty($node->path['pathauto_regen'])) {
    return;
  }

  if (empty($node->menu) || empty($node->menu['enabled']) || empty($node->menu['has_children'])) {
    return;
  }

  $child_mlids = pathauto_regen_get_menu_children($node->menu);

  if ($child_mlids) {
    $batch = array(
      'title' => t('Regenerating paths ...'),
      'operations' => array(),
      'init_message' => t('Loading menu children'),
      'progress_message' => t('Processed @current out of @total menu children.'),
      'error_message' => t('An error occurred during processing.'),
      'finished' => 'pathauto_regen_finished',
      'progressive' => FALSE,
      'file' => drupal_get_path('module', 'pathauto_regen') . '/pathauto_regen.inc',
    );

    foreach ($child_mlids as $child) {
      $batch['operations'][] = array('pathauto_regen_regenerate', array($child));
    }

    batch_set($batch);
  }
}

/**
 * Retrieves a list of child menu links given a mlid.
 */
function pathauto_regen_get_menu_children($menu) {
  $query = db_select('menu_links', 'ml')
    ->fields('ml', array('mlid'))
    ->condition('ml.router_path', 'node/%')
    ->condition('ml.menu_name', $menu['menu_name']);

  $or = db_or();

  $or->condition('ml.plid', $menu['mlid']);

  $i = 1;
  $p = 'ml.p1';
  while ($i <= MENU_MAX_DEPTH) {
    $or->condition($p, $menu['mlid']);
    $p = 'ml.p' . ++$i;
  }

  $query->condition($or);
  $query->condition('ml.mlid', $menu['mlid'], '!=');

  return $query->execute()->fetchCol();
}

/**
 * Batch callback that updates a node's path given a mlid.
 */
function pathauto_regen_regenerate($mlid, &$context) {
  $link_path = db_select('menu_links', 'ml')
    ->fields('ml', array('link_path'))
    ->condition('ml.mlid', $mlid)
    ->execute()
    ->fetchField();

  $nid = substr($link_path, 5);

  if ($node = node_load($nid)) {

    $node->path = array();
    if ($path = path_load('node/' . $node->nid)) {
      $node->path = $path;
    }

    if (variable_get('pathauto_regen_force_update')) {
      $node->path['pathauto'] = TRUE;
    }

    if (empty($context['results']['processed'])) {
      $context['results']['processed'] = 0;
    }

    if (!empty($node->path['pathauto'])) {
      pathauto_node_update_alias($node, 'update');

      $context['results']['processed']++;
      $context['message'] = t('Updating the path alias for %title.', array('%title' => $node->title));
    }
  }
}

/**
 * Callback for batch completion.
 *
 * @todo Handle error cases. I haven't run into any yet. :)
 */
function pathauto_regen_finished($success, $results, $operations, $time) {
  if ($success) {
    $message = format_plural($results['processed'],
      'Updated %count node in %time.',
      'Updated %count nodes in %time.',
      array('%count' => $results['processed'], '%time' => $time)
    );
    drupal_set_message($message);
  }
  else {
    // Handle error case.
  }
}
