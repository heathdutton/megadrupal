<?php
/**
 * @file
 * Administration page callbacks for the Election Post module.
 */

/**
 * Page callback for managing election posts.
 */
function election_post_manage_page(stdClass $election) {
  election_set_breadcrumb($election);
  $view = views_get_view('election_posts_admin');
  $view->override_path = current_path();
  return $view->preview('embed', array($election->election_id));
}

/**
 * Page callback for editing an election post.
 *
 * Path: election-post/%election_post/edit.
 */
function election_post_page_edit($post) {

  $title = election_post_page_title($post);
  drupal_set_title(
    t(
      'Edit: %title',
      array('%title' => truncate_utf8($title, 20, TRUE, TRUE))
    ),
    PASS_THROUGH
  );

  $election = $post->election;

  election_post_set_breadcrumb($election, $post);

  module_load_include('inc', 'election_post', 'election_post.forms');

  return drupal_get_form('election_post_form', $post, $election);
}

/**
 * Page callback for adding an election post.
 */
function election_post_page_add($election) {

  election_set_breadcrumb($election);

  $values = array();
  $values['election_id'] = $election->election_id;

  $type = _election_type_get_info($election->type);
  if (!$type) {
    return t(
      'Cannot add post: the election type %type could not be found.',
      array('%type' => $election->type)
    );
  }

  $values['type'] = $type['post machine name'];

  $post = entity_get_controller('election_post')->create($values);

  $post_name = 'post';
  if (!empty($type['post name'])) {
    $post_name = $type['post name'];
  }

  drupal_set_title(
    t('Add new @post to %election', array('%election' => $election->title, '@post' => $post_name)),
    PASS_THROUGH
  );

  module_load_include('inc', 'election_post', 'election_post.forms');

  return drupal_get_form('election_post_form', $post, $election);

}


/**
 * Page callback for cloning an election post.
 *
 * Path: election-post/%election_post/clone.
 */
function election_post_page_clone($post) {

  $old_post = $post;
  $post = clone $post;
  $election = $post->election;

  election_set_breadcrumb($post->election);

  $post->post_id = NULL;
  $post->is_new = TRUE;
  $post->is_clone = TRUE;
  $post->title = t('Clone of !title', array('!title' => $post->title));

  $post_name = election_get_post_name($post->election->type);

  drupal_set_title(
    t('Clone: %title', array('%title' => $old_post->title, '@post' => $post_name)),
    PASS_THROUGH
  );

  module_load_include('inc', 'election_post', 'election_post.forms');

  return drupal_get_form('election_post_form', $post, $election);
}
