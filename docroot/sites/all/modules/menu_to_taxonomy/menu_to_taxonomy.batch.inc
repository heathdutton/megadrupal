<?php

/**
 * @file
 * Batch API Code.
 */

/**
 * Processes the Batch.
 *
 * @param int $vid
 *   Vocabulary ID to use.
 */
function _menu_to_taxonomy_insert_terms_batch($vid) {
  $menu_name = variable_get(_menu_to_taxonomy_build_variable('vocab_menu', $vid), FALSE);
  $parent_mlid = variable_get(_menu_to_taxonomy_build_variable('vocab_parent', $vid), FALSE);
  if (!empty($parent_mlid)) {
    $parent_link = array(
      'mlid' => $parent_mlid,
      'menu_name' => $menu_name,
    );
    // Get all children of the menu parent item under which we start syncing
    // terms.
    $mlids = _menu_to_taxonomy_get_all_menu_children($parent_link);
  }
  else {
    $mlids = _menu_to_taxonomy_get_menu_items_all(menu_tree_all_data($menu_name));
  }
  $batch = array(
    'operations' => array(
      array(
        '_menu_to_taxonomy_insert_link_items_process',
        array(
          $mlids,
          $menu_name,
        ),
      ),
    ),
    'finished' => '_menu_to_taxonomy_insert_link_items_success',
    'title' => t('Rebuilding Taxonomy Terms from Menu'),
    'init_message' => t('The terms that are linked to menu items in this vocabulary have been deleted, and are about to be regenerated.'),
    'progress_message' => t('Import progress: Completed @current of @total stages.'),
    'redirect' => 'admin/structure/taxonomy',
    'error_message' => t('The Taxonomy Menu rebuild process encountered an error.'),
  );
  batch_set($batch);
  batch_process();
}

/**
 * Inserts 10 items.
 *
 * @param array $mlids
 *   The menu link IDs.
 * @param string $menu_name
 *   The machine name of the menu.
 * @param array $context
 *   The Batch context.
 */
function _menu_to_taxonomy_insert_link_items_process(array $mlids, $menu_name, array &$context) {
  $start = NULL;
  $end = NULL;
  _menu_to_taxonomy_batch_init_context($context, $start, $end, 10);

  // Loop through $terms to process each term.
  for ($i = $start; $i < count($mlids) && $i < $end; $i++) {
    $link = menu_link_load($mlids[$i]);
    _menu_to_taxonomy_term_save($link);
  }

  _menu_to_taxonomy_batch_update_context($context, $end, count($mlids), 'Creating Terms');
}

/**
 * Sets a message stating the menu has been updated.
 */
function _menu_to_taxonomy_insert_link_items_success() {
  drupal_set_message(t('The vocabulary has been synchronized with the menu.'));
}

/**
 * Initialises the batch context.
 *
 * @param array $context
 *   Batch context array.
 * @param int $start
 *   The item to start on in this pass.
 * @param int $end
 *   The end item of this pass.
 * @param int $items
 *   The number of items to process in this pass.
 */
function _menu_to_taxonomy_batch_init_context(array &$context, &$start, &$end, $items) {
  // Initialize sandbox the first time through.
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
  }

  $start = $context['sandbox']['progress'];
  $end = $start + $items;
}

/**
 * Update the batch context.
 *
 * @param array $context
 *   The Batch context array.
 * @param int $end
 *   The end point of the most recent pass.
 * @param int $total
 *   The total number of items to process in this batch.
 * @param string $msg
 *   Message for the progress bar.
 */
function _menu_to_taxonomy_batch_update_context(array &$context, $end, $total, $msg) {
  // Update context array.
  if ($end > $total) {
    $context['finished'] = 1;
    return;
  }
  $context['message'] = t("%msg: %end of %total", array(
    '%msg' => $msg,
    '%end' => $end,
    '%total' => $total,
  ));
  $context['sandbox']['progress'] = $end;
  $context['finished'] = $end / $total;
}
