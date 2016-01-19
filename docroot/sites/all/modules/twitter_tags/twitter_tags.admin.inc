<?php

/**
 * @file
 * Twitter Tags administrative page callbacks.
 */

/**
 * Form constructor for the Twitter Tags administration form.
 *
 * @see twitter_tags_admin_reset_submit()
 *
 * @ingroup forms
 */
function twitter_tags_admin_form() {
  $form = array();

  $form['reset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Reset tags'),
    '#description' => t('Reset tags on all of the Tweets loaded on the site.'),
  );
  $form['reset']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#submit' => array('twitter_tags_admin_reset_submit'),
  );

  return $form;
}

/**
 * Form submission handler for twitter_tags_admin_form().
 */
function twitter_tags_admin_reset_submit() {
  $batch = array(
    'operations' => array(
      array('twitter_tags_batch_assign', array()),
    ),
    'title' => t('Setting Twitter hashtag data.'),
    'file' => drupal_get_path('module', 'twitter_tags') . '/twitter_tags.admin.inc',
  );
  batch_set($batch);
}

/**
 * Batch callback for assigning tags to Tweets.
 */
function twitter_tags_batch_assign(&$context) {

  // Set up our batch on the first run through.
  if (!isset($context['sandbox']['tweets'])) {
    $context['sandbox']['tweets'] = db_query("SELECT twitter_id FROM {twitter}")->fetchCol();
    $context['sandbox']['count'] = count($context['sandbox']['tweets']);
  }

  // Grab the next Tweet ID, try to load the Tweet.
  module_load_include('inc', 'twitter', 'twitter');
  $status = twitter_status_load(array_shift($context['sandbox']['tweets']));
  if ($status) {
    twitter_tags_set_tags($status);
  }

  // Report our progress.
  if ($context['sandbox']['count']) {
    $context['finished'] = 1 - count($context['sandbox']['tweets']) / $context['sandbox']['count'];
  }

}
