<?php
/**
 * @file
 * Token integration for the Election Post module.
 */

/**
 * Implements hook_token_info().
 */
function election_post_token_info() {
  $info = array();
  $info['types'] = array(
    'election_post' => array(
      'name' => t('Posts'),
      'description' => t('Tokens related to election posts.'),
      'needs-data' => 'election_post',
    ),
  );
  $info['tokens'] = array(
    'election_post' => array(
      'id' => array(
        'name' => t('Election post ID'),
        'description' => t('The unique ID of the post.'),
      ),
      'election' => array(
        'name' => t('Election'),
        'description' => t('The election of the post.'),
        'type' => 'election',
      ),
      'title' => array(
        'name' => t('Title'),
        'description' => t('The title of the post.'),
      ),
    ),
  );
  return $info;
}

/**
 * Implements hook_tokens().
 */
function election_post_tokens($type, array $tokens, array $data = array(), array $options = array()) {
  $sanitize = !empty($options['sanitize']);
  $replacements = array();
  if ($type == 'election_post' && !empty($data['election_post'])) {
    $post = $data['election_post'];
    $election = empty($post->election) ? election_load($post->election_id) : $post->election;
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'id':
          $replacements[$original] = $post->post_id;
          break;

        case 'title':
          $replacements[$original] = $post->title;
          break;

        case 'election':
          $replacements[$original] = $sanitize ? check_plain($election->title) : $election->title;
          break;

      }
    }
    if ($election_tokens = token_find_with_prefix($tokens, 'election')) {
      $replacements += token_generate('election', $election_tokens, array('election' => $election), $options);
    }
  }
  return $replacements;
}
