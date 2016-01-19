<?php
// vim: set ts=2 sw=2 sts=2 et:

/**
 * @file
 * Preprocess functions.
 */

/**
 * Preprocess function for html.tpl.php
 *
 * @param array $vars
 *   Array of variables.
 */
function lc3_clean_preprocess_html(array &$vars) {
  // Add conditional stylesheets for IE
  drupal_add_css(
    drupal_get_path('theme', 'lc3_clean') . '/css/ie.css',
    array(
      'group'      => CSS_THEME,
      'browsers'   => array('IE' => 'lte IE 7', '!IE' => FALSE),
      'preprocess' => FALSE,
    )
  );

  // Add jquery.blockUI.js (should be in directory sites/all/libraries)
  if (file_exists(path_to_theme() . '/js/jquery.blockUI.js')) {
    // Added for backward compatibility
    drupal_add_js(path_to_theme() . '/js/jquery.blockUI.js');
  }
  elseif (function_exists('libraries_get_path')) {
    drupal_add_js(libraries_get_path('jquery.blockUI.js'));
  }

  // Add a special CSS class to the page when there no breadcrumb is going to be displayed
  if (2 > count(drupal_get_breadcrumb())) {
    $vars['classes_array'][] = 'no-breadcrumbs';
  }
}

/**
 * Add conditional JavaScript for IE.
 *
 * @param array $vars
 *   Array of variables.
 */
function lc3_clean_process_html(array &$vars) {
  $vars['scripts'] .= '<!--[if lte IE 7]><script type="text/javascript src="'
    . base_path() . path_to_theme() . '/js/ie.js"></script><![endif]-->' . "\n";
}

/**
 * Preprocess function for page.tpl.php
 *
 * @param array $vars
 *   Array of variables.
 */
function lc3_clean_preprocess_page(array &$vars) {
  // Render social links
  $twitter  = check_plain(theme_get_setting('theme_social_link_twitter'));
  $facebook = check_plain(theme_get_setting('theme_social_link_facebook'));
  $vars['twitter_link']  = $twitter  ? l(t('Follow us on Twitter'), 'http://www.twitter.com/#!/' . $twitter) : '';
  $vars['facebook_link'] = $facebook ? l(t('Find us on Facebook'), 'http://www.facebook.com/' . $facebook) : '';

  // Render the main menu
  if ($vars['main_menu'] && ($pid = variable_get('menu_main_links_source', 'main-menu'))) {
    $vars['main_menu']['header'] = array(
      '#markup' => '<h2 class="element-invisible">' . t('Main menu') . '</h2>',
    );
    $vars['main_menu']['tree'] = menu_tree($pid);
    $vars['main_menu']['tree']['#attributes'] = array(
      'id'    => 'main-menu',
      'class' => 'clearfix',
    );
  }

  // Add a hidden title for accessibility reasons
  if ($vars['page']['search']) {
    $vars['page']['search'] = array_merge(
      array(
        'header' => array('#markup' => '<h2 class="element-invisible">' . t('Search') . '</h2>'),
      ),
      $vars['page']['search']
    );
  }
}

/**
 * Preprocess function for node.tpl.php
 *
 * @param array $vars
 *   Array of variables.
 */
function lc3_clean_preprocess_node(array &$vars) {
  // Display only the first uploaded image in teasers
  if (!empty($vars['teaser']) && isset($vars['content']['field_image'])) {
    $vars['content']['field_image']['#items'] = array($vars['content']['field_image'][0]);
  }

   // Reorder and rename node links
  if (!empty($vars['teaser'])) {
    // Move "Read more" link to the end of the list
    if (isset($vars['content']['links']['node'])) {
      $node = $vars['content']['links']['node'];
      unset($vars['content']['links']['node']);
      $vars['content']['links']['node'] = $node;
    }
  }

  // Render the comment count as a link.
  // NOTE: do not change "!=" to the "!==" operator:
  // it's compared string and integer values here
  if (defined('COMMENT_NODE_CLOSED') && COMMENT_NODE_CLOSED != $vars['node']->comment) {
    $vars['linked_comment_count'] = (isset($vars['comment_count']))
      ? l(
        format_plural(
          $vars['comment_count'],
          '1<span> comment</span>',
          '@count<span> comments</span>'
        ),
        'node/' . $vars['node']->nid,
        array(
          'html'       => TRUE,
          'attributes' => array(
              'title' => format_plural($vars['comment_count'], '1 comment', '@count comments'),
              'class' => array('comment-count'),
          ),
        ),
        '#comments'
      )
      : '';
  }
  else {
    $vars['linked_comment_count'] = '';
  }

  $comment_location = variable_get('comment_form_location_' . $vars['node']->type, COMMENT_FORM_BELOW);

  // Link "Add new comment" to the form on the node page
  if (isset($vars['content']['links']['comment']['#links']['comment-add']) && COMMENT_FORM_SEPARATE_PAGE !== $comment_location) {

    if (!empty($vars['node']->teaser)) {
      $vars['content']['links']['comment']['#links']['comment-add']['href'] = drupal_lookup_path('alias', 'node/' . $vars['node']->nid);
    }
    elseif ('forum' !== $vars['node']->type) {
      // Remove the link if it is the node page
      unset($vars['content']['links']['comment']['#links']['comment-add']);
    }
  }

  // Forum nodes
  if ('forum' == $vars['node']->type) {

    $vars['classes_array'][] = 'forum-post';
    $vars['classes_array'][] = 'forum-top';
    $vars['classes_array'][] = 'comment';

    $vars['submitted'] = '<span>' . t('Posted:') . '</span> ' . format_date($vars['node']->created, 'custom', 'M j, Y, H:i');

    if (isset($vars['content']['links']['comment']['#links']['comment-add'])) {
      $vars['content']['links']['comment']['#links']['comment-add']['title'] = t('Reply');
    }
  }
  else {
    // Render the submission date in a custom format
    $vars['submitted'] = t(
      'Submitted by !username on !datetime',
      array(
        '!username' => $vars['name'],
        '!datetime' => format_date($vars['node']->created, 'custom', 'M j, Y &\n\d\a\s\h; H:i'),
      )
    );
  }
}

/**
 * Preprocess function for comment.tpl.php
 *
 * @param array $vars
 *   Array of variables.
 */
function lc3_clean_preprocess_comment(array &$vars) {
  $vars['title_attributes_array']['class'][] = 'subject';
  $vars['title'] = variable_get('comment_subject_field_' . $vars['node']->type, 1) ? $vars['comment']->subject : '';

  if (!$vars['title']) {
    $vars['classes_array'][] = 'comment-no-subject';
  }

  $uri = entity_uri('comment', $vars['comment']);
  $uri['options'] += array('attributes' => array('class' => 'permalink', 'rel' => 'bookmark'));
  $vars['permalink'] = l(t('#'), $uri['path'], $uri['options']);

  $vars['submitted'] = '<span>' . t('Posted:') . '</span> ' . format_date($vars['comment']->created, 'custom', 'M j, Y, H:i');
}

/**
 * Preprocess forum topic list.
 *
 * @param array $vars
 *   Array of variables.
 */
function lc3_clean_preprocess_forum_topic_list(array &$vars) {

  // Remove the first "icon" column header because now icons are inside the title column

  global $forum_topic_list_header;

  $forum_topic_list_header = array(
    array('data' => t('Topic'), 'field' => 'n.title'),
    array('data' => t('Replies'), 'field' => 'l.comment_count'),
    array('data' => t('Last reply'), 'field' => 'l.last_comment_timestamp'),
  );

  $ts = tablesort_init($forum_topic_list_header);
  $header = '';

  foreach ($forum_topic_list_header as $cell) {
    $cell = tablesort_header($cell, $forum_topic_list_header, $ts);
    $header .= _theme_table_cell($cell, TRUE);
  }

  $vars['header'] = $header;

  // Display topic creation dates differently
  if (!empty($vars['topics'])) {
    $row = 0;

    foreach ($vars['topics'] as $id => $topic) {
      $created = isset($topic->created) ? format_interval(REQUEST_TIME - $topic->created) : '';
      $user = user_load($vars['topics'][$id]->uid);

      $vars['topics'][$id]->created = t(
        'Created !date by !user',
        array(
          '!date' => '<span class="submitted-date">' . $created . '</span> ago',
          '!user' => l($user->name, drupal_lookup_path('alias', 'user' . $user->uid)),
        )
      );
    }
  }
}
