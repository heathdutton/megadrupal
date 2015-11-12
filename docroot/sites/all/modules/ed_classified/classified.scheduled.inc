<?php
/**
 * @file
 * Scheduled operations for classified.module
 *
 * Can be invoked from the web UI or from Drush:
 * - purge
 * - expiration
 * - notifications
 *
 * @copyright (c) 2010-2011 Ouest Systemes Informatiques (OSInet)
 *
 * @license General Public License version 2 or later
 *
 * Original code, not derived from the ed_classified module.
 */

/**
 * Helper to return current time or a chosen time.
 *
 * @param int $time
 *   A UNIX timestamp.
 *
 * @return int
 *   A UNIX timestamp.
 */
function _classified_get_time($time = NULL) {
  return isset($time) ? $time : REQUEST_TIME;
}

/**
 * Unpublish nodes past their expiration date.
 *
 * The test on n.type is not required, since the inner join will only return
 * such nodes anyway, but allows the query to take advantage of the core index
 * on node.type
 *
 * Reset notify time to 0 because this field is only used for interim
 * notifications, not expire/purge.
 *
 * No addTag('node_access'): this query can be run at any time by anyone.
 *
 * We SELECT first and UPDATE later in order to get a notification list.
 *
 * @param int $time
 *   A UNIX timestamp. Normally not set: this was added for testing purposes.
 *
 * @return array
 *   A per-user array of per-nid expired nodes titles.
 */
function _classified_scheduled_build_expire($time = NULL) {

  // Obtain the list of ads to expire.
  $expires = _classified_get_time($time);

  $q = db_select('node', 'n')->comment(__FUNCTION__);
  $cn = $q->innerJoin('classified_node', 'cn', 'n.nid = cn.nid');
  $results = $q->fields('n', array('nid', 'title', 'uid'))
    ->condition('n.type', 'classified')
    ->condition('n.status', 1)
    ->condition("$cn.expires", $expires, '<')
    ->execute();

  $expired = array();
  $nids = array();
  foreach ($results as $result) {
    $expired[$result->uid][$result->nid] = $result->title;
    $nids[] = $result->nid;
  }
  unset($results);
  $count = count($nids);

  if ($count) {
    /*
     * Now perform the expiration. SQL*99 does not include join in updates, so
     * DBTNG does not have them either.
     *
     * http://api.drupal.org/api/drupal/includes--database--database.inc/function/db_update/7#comment-15464
     */
    $transaction = db_transaction();
    try {
      $q = db_update('node')
        ->comment(__FUNCTION__)
        ->fields(array('status' => 0))
        ->condition('nid', $nids, 'IN');
      $touched = $q->execute();
      db_update('node_revision')
        ->comment(__FUNCTION__)
        ->fields(array('status' => 0))
        ->condition('nid', $nids, 'IN')
        ->execute();
      db_update('classified_node')
        ->comment(__FUNCTION__)
        ->fields(array('notify' => 0))
        ->condition('nid', $nids, 'IN')
        ->execute();
    }
    catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception('classified', $e);
      throw $e;
    }
    // DBTNG controlled commit.
    unset($transaction);
    watchdog('classified', 'Expiration unpublished @count ads: @expired', array(
        '@count' => $touched,
        '@expired' => var_export($expired, TRUE),
      ), WATCHDOG_INFO);
  }
  else {
    watchdog('classified', 'Expiration check did not find any ad to expire.',
      NULL, WATCHDOG_INFO);
  }

  drupal_alter('classified_expire', $expired);
  return $expired;
}

/**
 * Build one of the various notification lists.
 *
 * All this work can be skipped if no module implements
 * hook_classified_notify(): in such a case, notifications do not happen, and
 * there is no reason to update the notify date since notifications are not
 * being sent.
 *
 * No addTag('node_access'): this is an administrative function, that needs full
 * access.
 *
 * @param string $kind
 *   The kind of notification to send, from the module-defined list of kinds.
 * @param int $time
 *   A UNIX timestamp. Normally not set: this was added for testing purposes.
 *
 * @return array
 *   A per-user array of per-nid node titles to be notified.
 */
function _classified_scheduled_build_notify($kind, $time = NULL) {
  $notified = array();

  $modules = module_implements('classified_notify_alter');
  if (empty($modules)) {
    return $notified;
  }

  $now = _classified_get_time($time);
  $q = db_select('node', 'n')->comment(__FUNCTION__);
  $cn = $q->innerJoin('classified_node', 'cn', 'n.nid = cn.nid');
  $q->fields('n', array('nid', 'uid', 'title'));

  switch ($kind) {
    case 'half-life':
      // cn.notify < half-life, now > half-life.
      $q->condition('n.type', 'classified')
        ->condition('n.status', 0, '!=')
        ->where("$cn.notify < (n.changed + $cn.expires) / 2")
        ->where("(n.changed + $cn.expires) / 2 < $now");
      break;

    case 'pre-expire':
      // cn.notify < expiration - 1 day, now > expiration - 1 day.
      $q->condition('n.type', 'classified')
        ->condition('n.status', 0, '!=')
        ->where("$cn.notify < $cn.expires - 86400")
        ->condition("$cn.expires", $now + 86400, '<');
      break;

    case 'pre-purge':
      // cn.notify < purge - 1 day, now > purge - 1 day
      // 'grace' is in days.
      $grace = (_classified_get('grace') - 1) * 24 * 60 * 60;
      $q->condition('n.type', 'classified')
        ->condition('n.status', 0)
        ->where("$cn.notify < $cn.expires + $grace - 86400")
        ->condition("$cn.expires", $now - $grace, '<');
      break;

    default:
      watchdog('classified', 'Invalid notify type requested: @kind',
        array('@kind' => $kind), WATCHDOG_WARNING);
      break;
  }

  $results = $q->execute();
  $notified = array();
  foreach ($results as $result) {
    $notified[$result->uid][$result->nid] = $result->title;
  }

  // Alter before updating: allow modules to modify the notification list.
  drupal_alter('classified_notify', $notified, $kind);

  // Avoid building an empty update query.
  if (empty($notified)) {
    return $notified;
  }

  $updated = array();
  foreach ($notified as $uid => $user_notified) {
    foreach (array_keys($user_notified) as $nid) {
      $updated[] = $nid;
    }
  }

  $q = db_update('classified_node')
    ->comment(__FUNCTION__)
    ->fields(array('notify' => $now))
    ->condition('nid', $updated, 'IN')
    ->execute();
  watchdog('classified', 'Updated notification timestamp on @count ads.', array(
    '@count' => count($updated), WATCHDOG_INFO));

  return $notified;
}

/**
 * Purge nodes past their expiration date + grace period
 *
 * Selected nodes: expires + grace < now => expires < now - grace
 *
 * The test on n.type is not required, since the inner join will only return
 * such nodes anyway, but allows the query to take advantage of the core index
 * on node.type
 *
 * There is no addTag('node_access') because this is an administrative
 * operation, that can be triggered by any user and needs to access all matching
 * nodes, not only those for which access is granted
 *
 * @param int $time
 *   A UNIX timestamp. Normally not set: this was added for testing purposes.
 *
 * @return array
 *   A per-user array of per-nid node titles to be deleted.
 */
function _classified_scheduled_build_purge($time = NULL) {
  $grace = _classified_get('grace');
  if ($grace == -1) {
    return array();
  }
  $limit = _classified_get_time($time) - $grace * 24 * 60 * 60;

  $q = db_select('node', 'n')->comment(__FUNCTION__);
  $cn = $q->innerJoin('classified_node', 'cn', 'n.nid = cn.nid');

  $results = $q->fields('n', array('nid', 'title', 'uid'))
    ->condition('n.type', 'classified')
    ->condition("$cn.expires", $limit, '<')
    ->execute()
    ->fetchAll();

  $count = count($results);

  if ($count) {
    $ads = array();
    foreach ($results as $result) {
      $ads[$result->uid][$result->nid] = $result->title;
      $deleted[] = $result->nid;
    }

    // Hide message information, since the page can be triggered by any user,
    // but needs to run as admin, and protect misc session content as well.
    //
    // About coder false positive:
    // http://drupal.org/node/224333#drupal_set_session
    $messages = isset($_SESSION['messages'])
      ? $_SESSION['messages']
      : array();

    global $user;
    $saved_account = $user;
    drupal_save_session(FALSE);
    $user = user_load(1);

    // May invoke drupal_set_message(), hence the hiding code.
    node_delete_multiple($deleted);

    $user = $saved_account;
    drupal_save_session(TRUE);
    // Coder: false positive.
    $_SESSION['messages'] = $messages;

    watchdog('classified', "Deleted @count nodes: @deleted", array(
      '@count' => $count,
      '@deleted' => var_export($deleted, TRUE),
    ), WATCHDOG_INFO);
  }
  else {
    watchdog('classified', 'Purge did not find any ad to delete.', NULL, WATCHDOG_INFO);
    $ads = array();
  }

  drupal_alter('classified_purge', $ads);
  return $ads;
}

/**
 * Page callback for expirations
 *
 * @return string
 *   A string instead of a render array, because there is not much to say.
 */
function _classified_scheduled_page_expire() {
  // Do not display results.
  _classified_scheduled_build_expire();
  drupal_set_breadcrumb(_classified_get_breadcrumb_by_term(NULL));

  return t('<p>Thanks for triggering our expiration process.</p>');
}

/**
 * Page callback for notifications
 *
 * @param string $kind
 *   A notification kind from _classified_get_notify_kinds().
 *
 * @return string
 *   A string instead of a render array, because there is not much to say.
 */
function _classified_scheduled_page_notify($kind) {
  _classified_scheduled_build_notify($kind);
  drupal_set_breadcrumb(_classified_get_breadcrumb_by_term(NULL));

  return t('<p>Thanks for triggering our @kind notification process.</p>', array(
    '@kind' => $kind,
  ));
}

/**
 * Page callback for purges.
 *
 * @return string
 *   A string instead of a render array, because there is not much to say.
 */
function _classified_scheduled_page_purge() {
  // Do not display results.
  _classified_scheduled_build_purge();
  drupal_set_breadcrumb(_classified_get_breadcrumb_by_term(NULL));

  return t('<p>Thanks for triggering our purge process.</p>');
}
