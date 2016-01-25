<?php

/**
 * @file
 * Extends Simplenews by providing a visitor log by user.
 *
 * Date: 10-ott-2013
 * File: simplenews_visitor.lists.inc
 * Author: stefano
 */

/**
 * Orders the statistics by parameters.
 *
 * SC 21/giu/2011 10.12.23 stefano
 *
 * @param object $node
 *   the node/newsletter
 * @param string $by
 *   order by
 *
 * @return string
 *   HTML for a table
 */
function simplenews_visitor_listby($node, $by = "by_email") {
  $rows = array();

  if ($by == "reverse_email") {
    $query = db_select('{simplenews_subscriber}', 's');
    $query->leftJoin('{simplenews_subscription}', 't', '(s.snid = t.snid)');
    $query->leftJoin('{simplenews_newsletter}', 'n', '(n.tid = t.tid)');
  }
  else {
    $query = db_select('{simplenews_visitor}', 's');
    $query->where('s.nid = :nid', array(':nid' => $node->nid));
  }

  switch ($by) {
    case "by_email":
      $query->fields('s', array('mail', 'url', 'date', 'link'));
      $header = array(
        'mail' => array(
          'data' => t('Subscribers'),
          'sort' => 'asc',
          'field' => 'mail',
        ),
        'url' => array(
          'data' => t('Url'),
        ),
        'date' => array(
          'data' => t('Date'),
        ),
      );
      break;

    case "by_date":
      $query->fields('s', array('mail', 'url', 'date', 'link'));
      $header = array(
        'mail' => array(
          'data' => t('Subscribers'),
        ),
        'url' => array(
          'data' => t('Url'),
        ),
        'date' => array(
          'data' => t('Date'),
          'sort' => 'asc',
          'field' => 'date',
        ),
      );
      break;

    case "by_url":
      $query->fields('s', array('mail', 'url', 'date', 'link'));
      $header = array(
        'mail' => array(
          'data' => t('Subscribers'),
        ),
        'url' => array(
          'data' => t('Url'),
          'sort' => 'asc',
          'field' => 'link',
        ),
        'date' => array(
          'data' => t('Date'),
        ),
      );
      break;

    case "most_email":
      $query->groupBy('s.mail');
      $query->fields('s', array('mail'));
      $query->addExpression('count(s.mail)', 'group_mail');
      $header = array(
        'mail' => array(
          'data' => t('Subscribers'),
          'sort' => 'asc',
          'field' => 'mail',
        ),
        'click' => array(
          'data' => t('Url visited (all clicks)'),
        ),
      );
      break;

    case "most_url":
      $query->groupBy('s.url');
      $query->fields('s', array('url', 'link'));
      $query->addExpression('count(s.url)', 'group_url');
      $header = array(
        'url' => array(
          'data' => t('Url'),
          'sort' => 'asc',
          'field' => 'url',
        ),
        'click' => array(
          'data' => t('from Subscribers (all clicks)'),
        ),
      );
      break;
  }

  /* Added the pager.
   *
   * SC Thu Oct 24 18:20:44 CEST 2013 stefano
   */
  $query = $query->extend('PagerDefault')->extend('TableSort')->orderByHeader($header);

  $result = $query->execute();

  while ($row = $result->fetchAssoc()) {

    switch ($by) {
      case "most_email":
        $rows[] = array(
          check_plain($row['mail']),
          $row['group_mail'],
        );
        break;

      case "most_url":
        $rows[] = array(
          l((!empty($row['link']) ? $row['link'] : $row['url']), $row['url']),
          $row['group_url'],
        );
        break;

      default:
        $rows[] = array(
          check_plain($row['mail']),
          l((!empty($row['link']) ? $row['link'] : $row['url']), $row['url']),
          $row['date'],
        );
        break;
    }
  }

  $table = array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array(),
    'caption' => '',
    'colgroups' => array(),
    'sticky' => TRUE,
    'empty' => t('nessun dato ancora'),
  );

  return theme("table", $table) . theme('pager');
}

/**
 * Implements hook_theme().
 *
 * Collects data.
 *
 * SC 21/giu/2011 10.15.03 stefano
 */
function simplenews_visitor_listby_no_visitor($form, &$form_state, $node) {
  global $user;

  $header = array(
    'mail' => array(
      'data' => t('Subscribers'),
      'field' => 's.mail',
      'sort' => 'ASC',
    ),
  );

  // Field to sort on is picked from $header;
  $query = db_select('{simplenews_subscriber}', 's')->extend('PagerDefault')->extend('TableSort')->orderByHeader($header);
  $query->leftJoin('{simplenews_subscription}', 't', '(s.snid = t.snid)');
  $query->leftJoin('{simplenews_newsletter}', 'n', '(n.tid = t.tid)');
  $query->fields('s', array('mail', 'snid'));
  $query->where('n.nid = :nid1
  AND s.mail NOT IN (SELECT v.mail FROM {simplenews_visitor} AS v WHERE v.nid = :nid2)
  AND (:uid1 = 1)
  AND s.activated = 1', array(
    ':nid1' => $node->nid,
    ':nid2' => $node->nid,
    ':uid1' => $user->uid,
  ));

  /* SC 24/giu/2011 16.35.26 stefano
   *
   * Only own newsletter.
   * Only if activated globaly.
   */
  $result = $query->execute();

  $form = array();
  $checkboxes = array();
  while ($item = $result->fetchObject()) {

    /*
     * Adds each user id to my checkboxes array. Only keys, no values
     */
    $checkboxes[$item->snid] = array('mail' => check_plain($item->mail));
  }

  $form['checkboxes'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $checkboxes,
    '#empty' => t('No content available.'),
    '#multiple' => TRUE,
    '#disabled' => (!user_access('newsletter visitor resend') ? TRUE : FALSE),
    '#weight' => -10,
  );
  $form['pager'] = array(
    '#markup' => theme('pager'),
    '#weight' => -9,
  );

  if (!user_access('newsletter visitor resend')) {
    drupal_set_message(t("Your account doesn't have permission to select any of the subscriber."), 'warning');
  }

  $form['node'] = array(
    '#type' => 'hidden',
    '#value' => $node->nid,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Resend'),
  );

  return $form;
}

/**
 * Collects the email and resends the newsletter.
 *
 * See elementName simplenews simplenews_send_node().
 *
 * SC 21/giu/2011 10.21.46 stefano
 *
 * @param array $form
 *   post result
 * @param array $form_state
 *   post result
 */
function simplenews_visitor_listby_no_visitor_submit($form, &$form_state) {
  module_load_include('inc', 'simplenews', 'includes/simplenews.mail');
  $node = node_load($form['node']['#value']);

  /* SC 29/giu/2011 15:38:50 stefano
   *
   * Check if have availability.
   */
  if (_simplenews_visitor_check_max($node, count($form['checkboxes']['#value']))) {
    foreach ($form['checkboxes']['#value'] as $value) {
      $subscriber = simplenews_subscriber_load($value);
      if (!$subscriber) {
        /* The source expects a subscriber object with mail and language set.
         *
         * @todo: Find a cleaner way to do this.
         *
         * @see simplenews.mail.inc simplenews_send_test() line 117
         *
         * SC 16/lug/2013 16:11:08
         */
        $subscriber = new stdClass();
        $subscriber->uid = 0;
        $subscriber->mail = variable_get('site_mail', 'user@example.com');
        $subscriber->language = $GLOBALS['language']->language;
      }

      $source = new SimplenewsSourceNode($node, $subscriber);
      $source->setKey('node');

      /* SC Fri Oct 11 18:31:57 CEST 2013 stefano
       *
       * Newsletter sends again.
       *
       * Reduced the amount of the disponibility.
       */
      if (simplenews_send_source($source)) {
        watchdog('simplenews_visitor', 'Simplenews "%name" has been sent again.', array(
          '%name' => $node->title), WATCHDOG_WARNING);
      }
    }
  }
  else {
    watchdog('simplenews_visitor', "Simplenews '%name' hasn't enough disponibility.", array(
      '%name' => $node->title), WATCHDOG_WARNING);
    drupal_set_message(t("You haven't enough disponibility."), 'warning');
  }
}

/**
 * Checks if there is availability to send the email.
 *
 * SC 29/giu/2011 18:58:37 stefano
 *
 * @param array $node
 *   the node
 * @param int $count_mail
 *   the quantity of email to send
 *
 * @return bool
 *   if user has quantity sufficient
 */
function _simplenews_visitor_check_max($node, $count_mail) {
  $query = db_select('{simplenews_visitor_max_sent}', 's');
  $query->fields('s', array('max', 'amount'));
  $query->condition('tid', $node->simplenews->tid);

  $result = $query->execute()->fetchObject();

  /* Not check if max == 0 (unlimited sent).
   *
   * SC Fri Nov 29 13:10:04 CET 2013 stefano
   */
  if ($result->max && ($result->max - $result->amount - $count_mail) < 0) {
    $record = array(
      's_status' => SIMPLENEWS_STATUS_SEND_NOT,
      'nid' => $node->nid,
    );
    $update = 'tid';

    drupal_write_record('simplenews_newsletters', $record, $update);

    drupal_set_message(t('No more or insufficient availability to newsletter %newsletter.', array(
      '%newsletter' => $node->simplenews->name)));
    drupal_set_message(t('Please add more availability to send the newsletter.'));

    return FALSE;
  }
  elseif ($result->max > 0) {
    $record = array(
      'amount' => $result->amount + $count_mail,
      'tid' => $node->simplenews->tid,
    );
    $update = 'tid';

    drupal_write_record('simplenews_visitor_max_sent', $record, $update);
  }

  return TRUE;
}
