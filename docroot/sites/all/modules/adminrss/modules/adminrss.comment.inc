<?php
/**
 * @file
 * Comment feed related functions.
 *
 * @see adminrss_adminrss_feed_info()
 */

const ADMINRSS_VAR_LINK_TYPE = 'adminrss_link_type';

// Options for ADMINRSS_VAR_LINK_TYPE.
const ADMINRSS_LINK_QUEUE = 0;
const ADMINRSS_LINK_EDIT  = 1;

/**
 * Output a comment feed for comments under moderation.
 *
 * Items include a link to the comment on the node page and the node title,
 * to make it easier to check whether the comment is on topic.
 *
 * @return void
 *   Builder emits data and does not return any content.
 */
function adminrss_feed_comment() {
  global $base_url;

  $efq = new EntityFieldQuery();
  $entities = $efq->entityCondition('entity_type', 'comment')
    ->propertyOrderBy('changed', 'DESC')
    ->propertyCondition('status', 0)
    ->range(0, 15)
    ->execute();
  $ids = array_keys($entities['comment']);
  $comments = entity_load('comment', $ids);

  // Gather node loads, both to perform a single load and avoid duplicates.
  $nids = array();
  foreach ($comments as $comment) {
    $nids[$comment->nid] = $comment->nid;
  }
  $nodes = entity_load('node', $nids);

  $items = '';
  $link_type = variable_get(ADMINRSS_VAR_LINK_TYPE, ADMINRSS_LINK_QUEUE);
  foreach ($comments as $comment) {
    switch ($link_type) {
      case ADMINRSS_LINK_QUEUE:
        $link = url('admin/content/comment/approval', array('absolute' => TRUE));
        break;

      case ADMINRSS_LINK_EDIT:
        $link = url("comment/{$comment->cid}/edit", array(
          'query' => array('destination' => 'admin/content/comment/approval'),
          'absolute' => TRUE,
        ));
        break;

      // Site home: should not happen anyway.
      default:
        $link = base_path();
    }
    $nid = $comment->nid;
    $content = t('Comment on node @nid: <a href="!link">@title</a>', array(
      '@nid' => $nid,
      '@title' => $nodes[$nid]->title,
      '!link' => url("node/$nid", array('absolute' => TRUE)),
    ));
    if (!empty($comment->comment_body)) {
      $item = field_get_items('comment', $comment, 'comment_body');
      // Cardinality is always 1.
      $item = $item[0];
      $content .= " : \n" . check_markup($item['value'], $item['format']);
    }
    $items .= format_rss_item($comment->subject, $link, $content, array(
      array(
        'key' => 'pubDate',
        'value' => date('r', $comment->changed),
      ),
      array(
        'key'  => 'guid',
        'value'  => t('Comment @cid for node @nid at !site', array(
          '@cid' => $comment->cid,
          '@nid' => $comment->nid,
          // Safe by construction.
          '!site' => $base_url,
      )),
        'attributes' => array('isPermaLink' => 'false'),
      ),
      array(
        'key' => 'dc:creator',
        'value' => filter_xss($comment->name),
      ),
    ));
  }

  // Numeric index needed to quiet Coder Review/CodeSniffer.
  $channel = array(
    'description' => t('Unapproved Comments for Administration'),
    'title' => t('@site - AdminRSS Comments Feed', array('@site' => variable_get('site_name', 'drupal'))),
    0 => array(
      'key' => 'atom:link',
      'value' => NULL,
      'attributes' => array(
        'href' => url(adminrss_get_feed_path('comment'), array('absolute' => TRUE)),
        'rel' => 'self',
        'type' => 'application/rss+xml',
      ),
    ),
  );
  adminrss_format_feed($items, $channel);
}

/**
 * Implements hook_adminrss_feed_settings_TYPE().
 */
function adminrss_adminrss_feed_settings_comment() {
  $ret = array();
  $ret[ADMINRSS_VAR_LINK_TYPE] = array(
    '#type'          => 'select',
    '#title'         => t('Comment feed links back to'),
    '#options'       => array(
      ADMINRSS_LINK_QUEUE => t('Comment approval queue'),
      ADMINRSS_LINK_EDIT => t('Individual comment edit'),
    ),
    '#default_value' => variable_get(ADMINRSS_VAR_LINK_TYPE, ADMINRSS_LINK_EDIT),
  );

  return $ret;
}

/**
 * Implements hook_adminrss_feed_uninstall_TYPE().
 */
function adminrss_adminrss_feed_uninstall_comment() {
  variable_del(ADMINRSS_VAR_LINK_TYPE);
}
