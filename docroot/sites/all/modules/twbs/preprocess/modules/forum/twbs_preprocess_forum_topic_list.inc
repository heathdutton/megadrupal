<?php

/**
 * Preprocesses variables for forum-topic-list.tpl.php.
 *
 * @param $variables
 *   An array containing the following elements:
 *   - tid: Taxonomy term ID of the current forum.
 *   - topics: An array of all the topics in the current forum.
 *   - forum_per_page: The maximum number of topics to display per page.
 *
 * @see forum-topic-list.tpl.php
 * @see theme_forum_topic_list()
 */
function twbs_preprocess_forum_topic_list(&$variables) {
  global $forum_topic_list_header;

  // Create the tablesorting header.
  $ts = tablesort_init($forum_topic_list_header);
  $header = '';
  foreach ($forum_topic_list_header as $cell) {
    $cell = tablesort_header($cell, $forum_topic_list_header, $ts);
    $header .= _theme_table_cell($cell, TRUE);
  }
  $variables['header'] = $header;

  if (!empty($variables['topics'])) {
    $row = 0;
    foreach ($variables['topics'] as $id => $topic) {
      $variables['topics'][$id]->icon = theme('forum_icon', array('new_posts' => $topic->new, 'num_posts' => $topic->comment_count, 'comment_mode' => $topic->comment_mode, 'sticky' => $topic->sticky, 'first_new' => $topic->first_new));
      $variables['topics'][$id]->zebra = $row % 2 == 0 ? 'odd' : 'even';
      $row++;

      // We keep the actual tid in forum table, if it's different from the
      // current tid then it means the topic appears in two forums, one of
      // them is a shadow copy.
      if ($variables['tid'] != $topic->forum_tid) {
        $variables['topics'][$id]->moved = TRUE;
        $variables['topics'][$id]->title = check_plain($topic->title);
        $variables['topics'][$id]->message = l(t('This topic has been moved'), "forum/$topic->forum_tid");
      }
      else {
        $variables['topics'][$id]->moved = FALSE;
        $variables['topics'][$id]->title = l($topic->title, "node/$topic->nid");
        $variables['topics'][$id]->message = '';
      }
      $variables['topics'][$id]->created = theme('forum_submitted', array('topic' => $topic));
      $variables['topics'][$id]->last_reply = theme('forum_submitted', array('topic' => isset($topic->last_reply) ? $topic->last_reply : NULL));

      $variables['topics'][$id]->new_text = '';
      $variables['topics'][$id]->new_url = '';
      if ($topic->new_replies) {
        $variables['topics'][$id]->new_text = format_plural($topic->new_replies, '1 new', '@count new');
        $variables['topics'][$id]->new_url = url("node/$topic->nid", array('query' => comment_new_page_count($topic->comment_count, $topic->new_replies, $topic), 'fragment' => 'new'));
      }

    }
  }
  else {
    // Make this safe for the template.
    $variables['topics'] = array();
  }
  // Give meaning to $tid for themers. $tid actually stands for term id.
  $variables['topic_id'] = $variables['tid'];
  unset($variables['tid']);

  $variables['pager'] = theme('pager');
}
