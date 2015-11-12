<?php

/**
 * @file
 *
 * File with node discussion display methods.
 */

/**
 * Implements hook_node_view().
 */
function discussthis_node_view($node, $view_mode, $langcode) {
  global $user;
  
  if ($node) {
    $links = array();

    // Verify that the content type is eligible for discussion.
    $node_types = node_type_get_names();
    $discussthis_types_config = variable_get('discussthis_types_config', array());
    if (!isset($discussthis_types_config[$node->type][$node->type . '_enable']) || !$discussthis_types_config[$node->type][$node->type . '_enable']) {
      return $links;
    }

    // Lookup for topic nid, if it exists (otherwise we get 0).
    $topic_nid = _discussthis_get_topic($node->nid);
    
    // Only pages being displayed full page have comments shown.
    if ($view_mode == 'full' && $topic_nid) {
      $node->content['discussion'] = _discussthis_discussion_load($topic_nid);
    }

    // Verify that the type has a valid discussion forum selected.
    $forum_tid = _discussthis_get_forum($node->nid, $node->type);
    if (!$forum_tid) {
      return $links;
    }

    // Get the DiscusssThis! link text.
    $discussthis_link = variable_get('discussthis_link', '');
    if (!$discussthis_link) {
      $discussthis_link = t('Discuss This!');
    }
    // Get the participate in DiscusssThis! link text.
    $participate_link = variable_get('discussthis_participate', '');
    if (!$participate_link) {
      $participate_link = t('Participate in this discussion');
    }

    // If the user is connected and have permission
    if ($user->uid && user_access('access discuss this links')) {
      if ($topic_nid) {
        $all = _discussthis_comment_num_all($topic_nid);
        if (variable_get('discussthis_showcounts', 1)) {
          $new = comment_num_new($topic_nid);
          $counts = t(' (@new new/@all total)', array('@new' => $new, '@all' => $all));
        }
        $links = array(
            // use "Discuss This!" when no discussion started yet
            '#type' => 'link',
            '#title' => ($all == 0 ?  $discussthis_link : $participate_link . $counts),
            '#href' => 'node/' . $topic_nid,
            '#attributes' => array(
                '#class' => 'discussthis-link',
                '#title' => t('Participate to the discussion about this page'),
            ),
        );
      }  //Otherwise, no topic exists? We need to create it !
      elseif ($topic_nid == 0) {
        // Check whether the user has permission to initiate a new topics.
        if (user_access('initiate discuss this topics')) {
          $links = array(           
              '#type' => 'link',       
              '#title' => $discussthis_link,
              '#href' => 'discussthis/create/' . $node->nid,
              '#attributes' => array(
                  '#class' => 'discussthis-link',
                  '#title' => t('Start a discussion about this page'),
                  '#rel' => 'nofollow',
              ),
          );
        }
      }
    } else {  // This happens for anonymous users
      // If the user is not logged in, offer him to do so.
      // Otherwise, would have access to the link.
      if (variable_get('discussthis_login', 1) && user_access('access discuss this links')) {
        if ($topic_nid) {
          $all = _discussthis_comment_num_all($topic_nid);
          if (variable_get('discussthis_showcounts', 1)) {
            $new = comment_num_new($topic_nid);
            $counts = t(' (@new new/@all total)', array('@new' => $new, '@all' => $all));
          }
          $destination = 'destination=node/' . $topic_nid;
          $appeal = $all == 0 ? $discussthis_link : $participate_link . $counts;
        }
        else {
          // The topic does not exist.
          $destination = 'destination=discussthis/new/' . $node->nid;
          $appeal = $discussthis_link;
        }
        $attributes = array(
            'class' => 'discussthis-login',
            'title' => t('Log in or register and start a discussion about this page'),
            'rel' => 'nofollow',
        );
        if (variable_get('user_register', 1)) {
          if (variable_get('user_email_verification', 1)) {
            // Users can register themselves but have to go through e-mail verification process
            $href = t('!login or !register to @appeal',
                array(
                    '!login' => l(t('Log in'), 'user/login',
                        array('query' => drupal_get_destination(), 'attributes' => $attributes)),
                    '!register' => l(t('Register'), 'user/register'),
                    '@appeal' => $appeal,
                ));
          }
          else {
            // Users can register themselves without e-mail verification
            $href = t('!login or !register to @appeal',
                array(
                    '!login' => l(t('Log in'), 'user/login', array('query' => drupal_get_destination())),
                    '!register' => l(t('Register'), 'user/register', array('query' => drupal_get_destination(), 'attributes' => $attributes)),
                    '@appeal' => $appeal,
                ));
          }
        }
        else {
          // Only admins can add new users, no public registration.
          $href = t('!login to @appeal',
              array(
                  '!login' => l('Log in', 'user/login', array('query' => drupal_get_destination(), 'attributes' => $attributes)),
                  '@appeal' => $appeal,
              ));
        }
        $links = array(
            '#type' => 'link',
            '#title' => $href,
            '#href' => '',
            '#options' => array(
              'html' => TRUE,
            ),
        );
      }
    }
    $node->content['discussthis'] = $links;
  }
}


/**
 * Implements hook_node_validate().
 */
function discussthis_node_validate($node, $form) {
  if ($node && user_access('override discuss this forums') && isset($node->discussthis)) {
    if (!empty($node->discussthis['discussthis_topic'])) {
      $topic_nid = $node->discussthis['discussthis_topic'];
      // valid integer?
      // and if valid, is that this very node? if so, that's not good either
      if ($topic_nid == (int) $topic_nid && !is_null($node->nid) && $topic_nid != $node->nid && is_numeric($topic_nid)) {
        // make sure we can actually load that node and that's a forum's node
        $topic = node_load((int) $topic_nid);
        if (!$topic || $topic->type != 'forum') {
          $topic_nid = FALSE;
        }
      }
      else {
        $topic_nid = FALSE;
      }
      if (!$topic_nid) {
        // ugly error setting which works
        form_set_error('discussthis][discussthis_topic', t('The Discuss This! forum topic #@nid was not found (or you chose this very node.) Please, try again. Use the auto-complete feature or enter the exact node identifier. If you did not change the topic identifier, it could have been deleted while you were editing this node. Simply clear the topic entry in that case.', array('@nid' => $node->discussthis['discussthis_topic'])));
        unset($node->discussthis['discussthis_topic']);
      }
    }
    // the forum should never be wrong with a select unless we're working with a hacker
    if (!empty($node->discussthis['discussthis_forum'])) {
      // valid integer? representing a valid forum?
      $forum_tid = $node->discussthis['discussthis_forum'];
      if ((int) $forum_tid != -1) {
        if ($forum_tid == (int) $node->discussthis['discussthis_forum']) {
          $vid = variable_get('forum_nav_vocabulary', '');
          $term = taxonomy_term_load($forum_tid);
          if (!$term || $term->vid != $vid) {
            form_set_error('discussthis][discussthis_forum', t('The Discuss This! forum #@tid was not found.', array('@tid' => $forum_tid)));
            unset($node->discussthis['discussthis_forum']);
          }
        }
        else {
          unset($node->discussthis['discussthis_forum']);
        }
      }
    }
  }
}

/**
 * Implements hook_node_insert().
 */
function discussthis_node_insert($node) {
  if ($node) {
    if ($node->type != 'forum' && user_access('override discuss this forums')) {
      // the forum field is only available for nodes that are
      // not yet attached to a topic, so it may not be defined
      if (isset($node->discussthis['discussthis_forum'])) {
        $discussthis_forum['nid'] = $node->nid;
        $discussthis_forum['forum_tid'] = $node->discussthis['discussthis_forum'];
        _discussthis_set_forum($discussthis_forum);
      }
    }
  }
}

/**
 * Implements hook_node_update().
 */
function discussthis_node_update($node) {
  if ($node) {
    if ($node->type != 'forum') {
      if (user_access('override discuss this forums')) {
        // The forum field is only available for nodes that are
        // not yet attached to a topic, so it may not be defined
        if (isset($node->discussthis['discussthis_forum'])) {
          $discussthis_forum['nid'] = $node->nid;
          $discussthis_forum['forum_tid'] = $node->discussthis['discussthis_forum'];
          _discussthis_set_forum($discussthis_forum);
        }

        // topics are defined by nid
        $transaction = db_transaction();
        try {
          $sql = "DELETE FROM {discussthis} WHERE nid = %d";
          // TODO Please review the conversion of this statement to the D7 database API syntax.
          /* db_query($sql, $node->nid) */
          db_delete('discussthis')
          ->condition('nid', $node->nid)
          ->execute();
          if (!empty($node->discussthis['discussthis_topic'])) {
            $sql = "INSERT INTO {discussthis} (nid, topic_nid) VALUES (%d, %d)";
            // TODO Please review the conversion of this statement to the D7 database API syntax.
            /* db_query($sql, $node->nid, $node->discussthis['discussthis_topic']) */
            $id = db_insert('discussthis')
            ->fields(array(
                'nid' => $node->nid,
                'topic_nid' => $node->discussthis['discussthis_topic'],
            ))
            ->execute();
          }
        } catch (Exception $e) {
          $transaction->rollback();
          watchdog_exception('my_type', $e);
        }
      }
    }
  }
}

/**
 * Implements hook_node_delete().
 */
function discussthis_node_delete($node) {
  // drop any reference to that node
  //
  // PROBLEM: Note that if someone just initiated a "Discuss this"
  // form, then they will have a surprise when saving since the
  // page will be gone. I don't see any way around that though.
  //
  $sql = 'DELETE FROM {discussthis} WHERE nid = %d OR topic_nid = %d';
  // TODO Please review the conversion of this statement to the D7 database API syntax.
  /* db_query($sql, $node->nid, $node->nid) */
  db_delete('discussthis')
  ->condition('nid', $node->nid)
  ->execute();
}

// -------------------------------------------------------------------------.
// HELPER METHODS

/**
 * Read the node specific forum settings.
 *
 * Load the Discuss This! data attached to the specified node.
 * For new nodes, load the defaults as defined in the global settings.
 *
 * @param $nid The node identifier, or 0
 * @param $nid $type The type of node
 *
 * @return array
 *          Array structure representing a discussthis_forums row.
 */
function _discussthis_get_forum($nid, $type) {
  
  $sql = 'SELECT forum_tid FROM {discussthis} WHERE nid = %d';
  $forum_fid = db_query('SELECT topic_nid FROM {discussthis} WHERE nid = :nid', array(':nid' => $nid))->fetchField();
  
  // create defaults if we cannot find data in the table
  if (!$forum_fid) {
    $discussthis_types_config = variable_get('discussthis_types_config', array());
    $forum_fid = isset($discussthis_types_config[$type][$type . '_forum']) ? $discussthis_types_config[$type][$type . '_forum'] : 0;
  }
  return $forum_fid;
}

/**
 * Find forum topics attached to node.
 *
 * Lookup the given nid in the discussthis db table, and return the
 * corresponding forum topic nid, otherwise return the default for
 * this node type
 *
 * @param $nid The node to be checked for a topic identifier.
 *
 * @return integer
 *          The topic nid or 0 if no topic is found
 */
function _discussthis_get_topic($nid) {
  $sql = 'SELECT topic_nid FROM {discussthis} WHERE nid = %d';
  $topic_nid = db_query('SELECT topic_nid FROM {discussthis} WHERE nid = :nid', array(':nid' => $nid))->fetchField();
  return $topic_nid ? $topic_nid : 0;
}

/**
 * Find the number of comments for a node.
 * 
 * @param $nid  the id of the node to retrieve comment count for.
 *
 * @return integer
 *          the comment count.
 */
function _discussthis_comment_num_all($nid) {
  static $cache;

  if (!isset($cache[$nid])) {
    $cache[$nid] = db_query('SELECT comment_count FROM {node_comment_statistics} WHERE nid = :nid', array(':nid' => $nid))->fetchField();
  }
  return $cache[$nid];
}

/**
 * Save a node to forum mapping.
 *
 * This function stores a mapping between the given node (nid) 
 * and a forum (tid). Helps to configure individually each node.
 *
 * @param $discussthis_forum The array to save in the discussthis_forums table
 */
function _discussthis_set_forum($discussthis_forum) {
  
  db_merge('discussthis')
  ->key(array('nid' => $discussthis_forum['nid']))
  ->fields(array(
  'nid' =>  $discussthis_forum['nid'],
  'forum_tid' => $discussthis_forum['forum_tid'],
  ))
  ->execute();
  
}

/**
 * Save a node to topic association
 *
 * This function stores a mapping between the given node (nid)
 * and a forum topic (tid). Helps to configure individually each node.
 *
 * @param $discussthis_topic The array to save in the discussthis_forums table
 */
function _discussthis_set_topic($discussthis_topic) {
  
  db_merge('discussthis')
  ->key(array('nid' => $discussthis_topic['nid']))
  ->fields(array(
  'nid' =>  $discussthis_topic['nid'],
  'topic_nid' => $discussthis_topic['topic_nid'],
  ))
  ->execute();
  
}


/**
 * Find discussion comments for a node
 */
function _discussthis_discussion_load($topic_nid) {
  $topic = node_load($topic_nid);
  $discussion = comment_node_page_additions($topic);
  unset($discussion['comment_form']);
  $discussion['#theme'] = 'discussion_render_wrapper';
  foreach ($discussion['comments'] as $cid => $comment) {
    if (is_int($cid)) {
     $discussion['comments'][$cid]['#theme'] ='discussion_render_item';
    }
  }
  return $discussion;
}