<?php
/**
 * @file
 * This files renders comment stats dashboard and related links.
 */

/**
 * Displays the comment stats dashboard.
 */
function comment_stats_admin_overview() {
  drupal_set_title(t('Comment Stats'));
  $output = '';
  $header = array(
    array('data' => t('Id'), 'field' => 'nid'),
    array('data' => t('Type'), 'field' => 'type'),
    array('data' => t('Title'), 'field' => 'title'),
    array('data' => t('Comment Status'), 'field' => 'comment'),
    array('data' => t('Visibility'), 'field' => 'status'),
    array('data' => t('Comments')),
    array('data' => t('Replies')),
    array('data' => t('Unapproved')),
    array('data' => t('Total'), 'field' => 'comment_count'),
    array('data' => t('Operations')),
  );

  $query = db_select("node_comment_statistics", "s");
  $query->leftJoin('node', 'n', 's.nid = n.nid');
  $query->fields('s', array('nid', 'comment_count'))
        ->fields('n', array('type', 'title', 'comment', 'status'));

  if (variable_get('comment_stats_show_closed')) {
    $query->condition('n.comment', variable_get('comment_stats_show_closed'), '=');
  }
  if (variable_get('comment_stats_show_unpublished')) {
    $query->condition('n.status', 1, '=');
  }
  $query = $query->extend('TableSort')->extend('PagerDefault')->limit(variable_get('comment_stats'));
  $query->orderByHeader($header);
  $result = $query->execute();

  $rows = array();
  while ($data = $result->fetchObject()) {
    // Status text.
    if ($data->status == 1) {
      $data->status = t("Published");
    }
    else {
      $data->status = t("Not published");
    }
    // Comment visibilty text.
    if ($data->comment == 0) {
      $data->comment = t("Hidden");
    }
    elseif ($data->comment == 1) {
      $data->comment = t("Closed");
    }
    elseif ($data->comment == 2) {
      $data->comment = t("Open");
    }

    $link_html = '';
    $link_html .= "<ul class='links inline'>";
    $link_html .= "<li><a href='" . url("admin/content/commentstats/users/" . $data->nid) . "'>" . t("Users") . "</a></li>";
    $link_html .= "<li><a href='" . url("admin/content/commentstats/posts/" . $data->nid) . "'>" . t("Comments") . "</a></li>";
    $link_html .= "<li><a href='" . url("admin/content/commentstats/replies/" . $data->nid) . "'>" . t("Replies") . "</a></li>";
    $link_html .= "<li><a href='" . url("admin/content/commentstats/unapproved/" . $data->nid) . "'>" . t("Unapproved") . "</a></li>";
    $link_html .= "</ul>";

    $count_query = db_query("SELECT count(cid) as posts,
    (SELECT count(cid) FROM {comment} WHERE thread LIKE '%.%' AND nid = :nid AND status = :newstatus) as replies,
    (SELECT count(cid) FROM {comment} WHERE nid = :nid AND status = :status) as unapproved
    FROM {comment} WHERE thread NOT LIKE '%.%' AND nid = :nid AND status = :newstatus",
    array(':nid' => $data->nid, ':status' => 0, ':newstatus' => 1));

    $count_data = $count_query->fetch();
    $total_root_post = $count_data->posts;
    $total_reply = $count_data->replies;
    $unapproved_comments = $count_data->unapproved;
    $total_comments = $total_root_post + $total_reply + $unapproved_comments;
    $rows[] = array(
      $data->nid,
      $data->type,
      $data->title,
      $data->comment,
      $data->status,
      $total_root_post,
      $total_reply,
      $unapproved_comments,
      $total_comments,
      $link_html,
    );
  };

  $output .= theme('table',
    array(
      'header' => $header,
      'rows' => $rows,
      'attributes' => array(),
      'sticky' => TRUE,
      'caption' => '',
      'colgroups' => array(),
      'empty' => t("No Records Found."),
    )
  ) . theme('pager');
  return $output;

}

/**
 * Lists users contentwise.
 */
function comment_stats_admin_overview_users() {
  // Set Title.
  drupal_set_title(t('Users: comment stats'));
  // Set Breadcrumbs.
  comment_stats_build_breadcrumbs();

  $nid = arg(4);
  $output = '';
  $header = array(
    array('data' => t('User Id'), 'field' => 'uid'),
    array('data' => t('User Type')),
    array('data' => t('User Name'), 'field' => 'name'),
    array('data' => t('Total comments/replies')),
    array('data' => t('Comments/replies list')),
  );

  $query = db_select("comment", "c");
  $query->fields('c', array('uid', 'name'))
        ->condition('c.nid', $nid, '=');
  $query->distinct();
  $query = $query->extend('TableSort')->extend('PagerDefault')->limit(variable_get('comment_stats_users'));
  $query->orderByHeader($header);
  $result = $query->execute();

  $rows = array();
  while ($data = $result->fetchObject()) {

    $user_details = user_load($data->uid);
    $user_type = implode(',', $user_details->roles);
    $link_html = '';
    $link_html .= "<ul class='links inline'>";
    $link_html .= "<li><a href='" . url("admin/content/commentstats/comments/view/" . $nid . "/" . $data->uid) . "'>" . t("View") . "</a></li>";
    $link_html .= "</ul>";

    $count_query = db_query("SELECT count(cid) FROM {comment} WHERE uid = :uid AND nid= :nid", array(':nid' => $nid, ':uid' => $data->uid));
    $count_comment = $count_query->fetchColumn();
    $rows[] = array(
      $data->uid,
      $user_type,
      $data->name,
      $count_comment,
      $link_html,
    );
  };

  $output .= theme('table',
    array(
      'header' => $header,
      'rows' => $rows,
      'attributes' => array(),
      'sticky' => TRUE,
      'caption' => '',
      'colgroups' => array(),
      'empty' => t("No Records Found."),
    )
  ) . theme('pager');
  return $output;

}

/**
 * Comments view.
 */
function comment_stats_admin_overview_comments() {
  drupal_add_css(drupal_get_path('module', 'comment_stats') . '/css/comment_stats.css');
  // Set Title.
  drupal_set_title(t('Comments: comment stats'));
  // Set Breadcrumbs.
  comment_stats_build_breadcrumbs();

  $nid = arg(5);
  $user_id = arg(6);

  $query = db_select("comment", "c");
  $query->fields('c', array('cid'))
        ->condition('c.nid', $nid, '=')
        ->condition('c.uid', $user_id, '=');
  $query = $query->extend('PagerDefault')->limit(variable_get('comment_stats_comments_per_user'));
  $result = $query->execute();
  $cids = array();
  while ($data = $result->fetchObject()) {
    $cids[] = $data->cid;
  };
  $comments = comment_load_multiple($cids);
  return theme('comment_stats_admin_overview_comments', array('all_comments' => $comments)) . theme('pager');;
}

/**
 * Displays the posts for content.
 */
function comment_stats_admin_overview_posts() {
  drupal_add_css(drupal_get_path('module', 'comment_stats') . '/css/comment_stats.css');
  // Set Title.
  drupal_set_title(t('Comments: comment stats'));
  // Set Breadcrumbs.
  comment_stats_build_breadcrumbs();
  $nid = arg(4);
  $query = db_select("comment", "c");
  $query->fields('c', array('cid'))
        ->condition('c.status', 1, '=')
        ->condition('c.nid', $nid, '=')
        ->condition('thread', '%' . db_like('.') . '%', 'NOT LIKE');
  $query = $query->extend('PagerDefault')->limit(variable_get('comment_stats_posts'));
  $result = $query->execute();
  $cids = array();
  while ($data = $result->fetchObject()) {
    $cids[] = $data->cid;
  };
  $comments = comment_load_multiple($cids);
  return theme('comment_stats_admin_overview_comments', array('all_comments' => $comments)) . theme('pager');
}

/**
 * Displays the replies for content.
 */
function comment_stats_admin_overview_replies() {
  drupal_add_css(drupal_get_path('module', 'comment_stats') . '/css/comment_stats.css');
  // Set Title.
  drupal_set_title(t('Replies: comment stats'));
  // Set Breadcrumbs.
  comment_stats_build_breadcrumbs();
  $nid = arg(4);
  $query = db_select("comment", "c");
  $query->fields('c', array('cid'))
        ->condition('c.status', 1, '=')
        ->condition('c.nid', $nid, '=')
        ->condition('thread', '%' . db_like('.') . '%', 'LIKE');
  $query = $query->extend('PagerDefault')->limit(variable_get('comment_stats_replies'));
  $result = $query->execute();
  $cids = array();
  while ($data = $result->fetchObject()) {
    $cids[] = $data->cid;
  };
  $comments = comment_load_multiple($cids);
  return theme('comment_stats_admin_overview_comments', array('all_comments' => $comments)) . theme('pager');
}

/**
 * Displays the unapproved comments for content.
 */
function comment_stats_admin_overview_unapproved() {
  drupal_add_css(drupal_get_path('module', 'comment_stats') . '/css/comment_stats.css');
  // Set Title.
  drupal_set_title(t('Unapproved Comments: comment stats'));
  // Set Breadcrumbs.
  comment_stats_build_breadcrumbs();
  $nid = arg(4);
  $query = db_select("comment", "c");
  $query->fields('c', array('cid'))
        ->condition('c.nid', $nid, '=')
        ->condition('status', 0, '=');
  $query = $query->extend('PagerDefault')->limit(variable_get('comment_stats_unapproved'));
  $result = $query->execute();
  $cids = array();
  while ($data = $result->fetchObject()) {
    $cids[] = $data->cid;
  };
  $comments = comment_load_multiple($cids);
  return theme('comment_stats_admin_overview_comments', array('all_comments' => $comments)) . theme('pager');
}
