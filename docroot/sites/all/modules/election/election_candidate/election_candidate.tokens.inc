<?php
/**
 * @file
 * Token integration for the Election Candidate module.
 */

/**
 * Implements hook_token_info().
 */
function election_candidate_token_info() {
  $info = array();
  $info['types'] = array(
    'election_candidate' => array(
      'name' => t('Candidates'),
      'description' => t('Tokens related to election candidates.'),
      'needs-data' => 'election_candidate',
    ),
  );
  $info['tokens'] = array(
    'election_candidate' => array(
      'id' => array(
        'name' => t('Election candidate ID'),
        'description' => t('The unique ID of the candidate.'),
      ),
      'election' => array(
        'name' => t('Election'),
        'description' => t('The election in which the candidate is standing.'),
        'type' => 'election',
      ),
      'post' => array(
        'name' => t('Election post'),
        'description' => t('The post for which the candidate is standing.'),
        'type' => 'election_post',
      ),
      'name' => array(
        'name' => t('Name'),
        'description' => t('The name of the candidate'),
      ),
    ),
  );
  return $info;
}

/**
 * Implements hook_tokens().
 */
function election_candidate_tokens($type, array $tokens, array $data = array(), array $options = array()) {
  $sanitize = !empty($options['sanitize']);
  $replacements = array();
  if ($type == 'election_candidate' && !empty($data['election_candidate'])) {
    $candidate = $data['election_candidate'];
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'id':
          $replacements[$original] = $candidate->candidate_id;
          break;

        case 'election':
          $election = election_load($candidate->election_id);
          $replacements[$original] = $sanitize ? check_plain($election->title) : $election->title;
          break;

        case 'post':
          $post = election_post_load($candidate->post_id);
          $replacements[$original] = $sanitize ? check_plain($post->title) : $post->title;
          break;

        case 'name':
          $replacements[$original] = election_candidate_get_name($candidate, $sanitize);
          break;

      }
    }
    if ($election_tokens = token_find_with_prefix($tokens, 'election')) {
      $election = election_load($candidate->election_id);
      $replacements += token_generate('election', $election_tokens, array('election' => $election), $options);
    }
    if ($post_tokens = token_find_with_prefix($tokens, 'election_post')) {
      $post = election_post_load($candidate->post_id);
      $replacements += token_generate('election_post', $post_tokens, array('election_post' => $post), $options);
    }
  }
  return $replacements;
}
