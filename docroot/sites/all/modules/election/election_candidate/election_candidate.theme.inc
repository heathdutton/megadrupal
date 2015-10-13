<?php
/**
 * @file
 * Theme and preprocess functions for the Election Candidate module.
 */

/**
 * Process variables for election-candidate.tpl.php.
 *
 * The $variables array contains the following arguments:
 * - $candidate
 * - $election
 * - $view_mode
 * - $page
 *
 * @see election-candidate.tpl.php
 */
function template_preprocess_election_candidate(&$variables) {

  $candidate = $variables['elements']['#candidate'];
  $election = $variables['elements']['#election'];
  $post = $variables['elements']['#post'];
  $view_mode = $variables['elements']['#view_mode'];

  // Detect whether the candidate is on its own page.
  $page_candidate = menu_get_object('election_candidate');
  $is_page = ($page_candidate && $page_candidate->candidate_id == $candidate->candidate_id);

  // Create some basic useful variables.
  $variables['view_mode'] = $view_mode;
  $variables['teaser'] = ($view_mode == 'teaser');
  $variables['page'] = $is_page;
  $variables['candidate'] = $candidate;
  $variables['election'] = $election;
  $variables['post'] = $post;

  $uri = election_candidate_uri($candidate);
  $variables['candidate_url'] = url($uri['path'], $uri['options']);

  $variables['name'] = election_candidate_get_name($candidate);

  $variables += array('content' => array());

  foreach (element_children($variables['elements']) as $key) {
    $variables['content'][$key] = $variables['elements'][$key];
  }

  // Make the field variables available with the appropriate language.
  field_attach_preprocess('election_candidate', $candidate, $variables['content'], $variables);

  $variables['classes_array'][] = drupal_html_class('election-type-' . $election->type . '-candidate');
  if ($view_mode == 'teaser') {
    $variables['classes_array'][] = 'election-candidate-teaser';
  }
  if (!$candidate->published) {
    $variables['classes_array'][] = 'election-candidate-unpublished';
  }

  $variables['theme_hook_suggestions'][] = 'election_candidate__' . $candidate->candidate_id;
  $variables['theme_hook_suggestions'][] = 'election__' . $election->type . '__candidate';
  $variables['theme_hook_suggestions'][] = 'election__' . $election->election_id . '__candidate';

}

/**
 * Process variables for theme_election_candidate_status().
 */
function template_preprocess_election_candidate_status(&$variables) {
  $candidate = $variables['candidate'];
  $variables['status'] = check_plain(_election_candidate_status_name($candidate));
  $variables['classes'] = array(
    'election-candidate-status',
    drupal_html_class('election-candidate-status-' . $variables['status']),
  );
}

/**
 * Theme function for displaying a candidate's status.
 */
function theme_election_candidate_status($variables) {
  $attributes = array();
  if ($variables['classes']) {
    $attributes['class'] = $variables['classes'];
  }
  return '<span' . drupal_attributes($attributes) . '>'
    . check_plain($variables['status'])
    . '</span>';
}

/**
 * Theme function for displaying a candidate as an item on a ballot form.
 */
function theme_election_candidate_ballot_item($variables) {
  $candidate = $variables['candidate'];
  // Get the unsanitized name of the candidate.
  $name = election_candidate_get_name($candidate, FALSE);
  // Link to the candidate's profile. The l() function will sanitize the name.
  $link = l($name, election_candidate_uri_path($candidate));
  return '<span class="election-candidate-ballot-item">' . $link . '</span>';
}

/**
 * Process variables for displaying a link to the nomination form on a post.
 */
function template_preprocess_election_candidate_nominate_link(&$variables) {
  $post = $variables['post'];
  $variables['logged_in'] = user_is_logged_in();
  $variables['nominate_path'] = election_post_uri_path($post) . '/nomination-form';
  $variables['nominate_open'] = _election_post_is_open($post, 'n');
  $variables['nominate_access'] = election_candidate_nominate_access($post);
}

/**
 * Theme function for displaying a link to the nomination form on a post.
 */
function theme_election_candidate_nominate_link($variables) {
  $nominate_access = $variables['nominate_access'];
  $nominate_open = $variables['nominate_open'];
  $nominate_path = $variables['nominate_path'];
  $logged_in = $variables['logged_in'];
  $options = array('query' => drupal_get_destination());
  if ($nominate_access) {
    $output = l(t('Submit a nomination'), $nominate_path, $options);
    return '<span class="election-nominate-link">' . $output . '</span>';
  }
  elseif (!$logged_in && $nominate_open) {
    $output = t('<a href="@login">Log in</a> to submit a nomination', array('@login' => url('user/login', $options)));
    return '<span class="election-nominate-link election-nominate-login">' . $output . '</span>';
  }
}
