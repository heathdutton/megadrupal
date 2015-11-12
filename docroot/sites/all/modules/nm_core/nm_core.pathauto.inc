<?php
/**
 * @file
 */


/**
 * Batch processing callback; Generate aliases for users' pages.
 */
function nm_core_pathauto_bulk_update_batch_process(&$context) {
  if (!isset($context['sandbox']['current'])) {
    $context['sandbox']['count'] = 0;
    $context['sandbox']['current'] = 0;
  }

  $query = db_select('users', 'u');
  $query->leftJoin('url_alias', 'ua', "CONCAT('user/', u.uid, '/edit') = ua.source");
  if (module_exists('contact')) {
    $query->leftJoin('url_alias', 'ua', "CONCAT('user/', u.uid, '/contact') = ua.source");
  }
  if (module_exists('nm_activity')) {
    $query->leftJoin('url_alias', 'ua', "CONCAT('user/', u.uid, '/activity') = ua.source");
  }
  if (module_exists('nm_blog')) {
    $query->leftJoin('url_alias', 'ua', "CONCAT('user/', u.uid, '/blog') = ua.source");
  }
  if (module_exists('nm_members')) {
    $query->leftJoin('url_alias', 'ua', "CONCAT('user/', u.uid, '/edit/nm_member_profile') = ua.source");
  }
  $query->addField('u', 'uid');
  $query->isNull('ua.source');
  $query->condition('u.uid', $context['sandbox']['current'], '>');
  $query->orderBy('u.uid');
  $query->addTag('pathauto_bulk_update');
  #$query->addMetaData('entity', 'user');

  // Get the total amount of items to process.
  if (!isset($context['sandbox']['total'])) {
    $context['sandbox']['total'] = $query->countQuery()->execute()->fetchField();

    // If there are no nodes to update, the stop immediately.
    if (!$context['sandbox']['total']) {
      $context['finished'] = 1;
      return;
    }
  }

  $query->range(0, 25);
  $uids = $query->execute()->fetchCol();

  $accounts = user_load_multiple($uids);
  foreach ($accounts as $account) {
    nm_core_update_alias($account, 'bulkupdate');
  }

  $context['sandbox']['count'] += count($uids);
  $context['sandbox']['current'] = max($uids);
  $context['message'] = t('Updated alias for user @uid edit.', array('@uid' => end($uids)));

  if ($context['sandbox']['count'] != $context['sandbox']['total']) {
    $context['finished'] = $context['sandbox']['count'] / $context['sandbox']['total'];
  }
}