<?php
/**
 * @file
 * Administrative page callbacks for election candidates and nominations
 * (Election module).
 */

/**
 * Page callback for managing election candidates.
 */
function election_candidate_manage_page(stdClass $election) {
  election_set_breadcrumb($election);
  $view = views_get_view('election_candidates_admin');
  // Explicitly set the path, so that Views Bulk Operations could work if
  // configured (VBO needs help setting the form's "action" attribute).
  $view->override_path = current_path();
  return $view->preview('embed', array($election->election_id));
}

/**
 * Page callback for editing an election candidate.
 */
function election_candidate_page_edit($candidate) {

  drupal_set_title(t('Edit candidate: %candidate', array(
    '%candidate' => election_candidate_get_name($candidate, FALSE),
  )), PASS_THROUGH);

  $post = election_post_load($candidate->post_id);
  if ($post) {
    $election = $post->election;
    election_post_set_breadcrumb($election, $post);
  }
  else {
    $election = election_load($candidate->election_id);
    election_set_breadcrumb($election);
  }

  module_load_include('inc', 'election_candidate', 'election_candidate.forms');

  return drupal_get_form('election_candidate_form', $candidate, $post);
}

/**
 * Page callback for adding an election candidate.
 */
function election_candidate_page_add($post) {

  if (!is_object($post)) {
    return MENU_NOT_FOUND;
  }

  $election = $post->election;
  election_post_set_breadcrumb($election, $post);

  $candidate_type = !empty($post->settings['candidate_type']) ? $post->settings['candidate_type'] : 'candidate';
  $candidate = entity_get_controller('election_candidate')->create(array(
    'type' => $candidate_type,
    'election_id' => $election->election_id,
    'post_id' => $post->post_id,
    'is_new' => TRUE,
  ));

  drupal_set_title(
    t('Add new candidate to %post', array('%post' => $post->title)),
    PASS_THROUGH
  );

  module_load_include('inc', 'election_candidate', 'election_candidate.forms');

  return drupal_get_form('election_candidate_form', $candidate, $post);

}
