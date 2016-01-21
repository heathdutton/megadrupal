<?php

/**
 * @file
 * Batch process to update existing biblio nodes
 *
 * @see biblio_advanced_import.module
 * @see biblio.module
 *
 * @author Markus Kalkbrenner | Cocomore AG
 *   @see http://drupal.org/user/124705
 */

/**
 * Updates the hashes of all biblio nodes as batch process.
 *
 * @see biblio_advanced_import_settings_form_submit()
 *
 * @param $context
 *   drupal batch process context
 */
function biblio_advanced_import_update_hashes_batch(&$context) {
  if (empty($context['biblio_advanced_import_update_hashes_batch'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['current_nid'] = 0;
    $context['sandbox']['max_nid'] = db_query('SELECT MAX(nid) FROM {biblio}')->fetchField();
    $context['sandbox']['num_nids'] = db_query('SELECT COUNT(DISTINCT nid) FROM {biblio}')->fetchField();
    $context['sandbox']['limit'] = $context['sandbox']['num_nids'] > 20 ? 5 : 1;
  }

  if ($context['sandbox']['current_nid'] >= $context['sandbox']['max_nid']) {
    $context['sandbox']['progress'] = $context['sandbox']['num_nids'];
    $context['finished'] = 1;
  }
  else {
    $query = db_select('biblio', 'b');
    $alias = $query->innerJoin('node', 'n', 'b.nid = n.nid AND b.vid = n.vid');
    $query->fields('b', array('nid'))
      ->condition('b.nid', $context['sandbox']['current_nid'], '>')
      ->condition('b.nid', $context['sandbox']['max_nid'], '<=')
      ->orderBy('b.nid', 'ASC');
    $result = $query->execute();
    while ($row = $result->fetchObject()) {
      $node = node_load($row['nid'], NULL, TRUE);
      biblio_advanced_import_update_hash($node);

      $context['results'][] = $node->nid . ' : ' . $node->biblio_md5;
      $context['sandbox']['progress']++;
      $context['sandbox']['current_nid'] = $node->nid;
      $context['message'] = $node->nid . ' : ' . $node->title;
    }

    if ($context['sandbox']['progress'] != $context['sandbox']['num_nids']) {
      $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['num_nids'];
    }
  }
}


/**
 * Clean-up after hashes of all biblio nodes have been updated
 * in a batch process.
 *
 * @see biblio_advanced_import_settings_form_submit()
 */
function biblio_advanced_import_update_hashes_batch_finished($success, $results, $operations) {
  if ($success) {
    $message = format_plural(count($results), 'Updated one biblio hash.', 'Updates @count biblio hashes.');
  }
  else {
    $message = t('Finished with an error.');
  }
  drupal_set_message($message);

  // TODO re-enable import and creation of new biblio nodes
}
