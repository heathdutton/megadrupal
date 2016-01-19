<?php

/**
 * @file
 * Administrative settings for comment scoring.
 */


/**
 * The administration screen for brightcove
 */
function comment_score_settings($form, &$form_state) {
  // Thresholds.
  $form['thresholds'] = array(
    '#type' => 'fieldset',
    '#title' => t('Thresholds'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['thresholds']['comment_score_pre_threshold'] = array(
    '#type' => 'textfield',
    '#title' => t('Pre-moderation threshold for users that are not trusted'),
    '#description' => t('At what score should a comment be placed into the pre-moderation queue (and not instant published).'),
    '#size' => 3,
    '#maxlength' => 3,
    '#element_validate' => array('element_validate_integer'),
    '#default_value' => variable_get('comment_score_pre_threshold', -10),
    '#required' => TRUE,
  );
  $roles = user_roles();
  unset($roles[DRUPAL_ANONYMOUS_RID]);
  unset($roles[DRUPAL_AUTHENTICATED_RID]);
  $form['thresholds']['comment_score_trusted_role'] = array(
    '#type' => 'select',
    '#title' => t('Trusted commenter role'),
    '#options' => $roles,
    '#default_value' => variable_get('comment_score_trusted_role', array_search('trusted commenter', $roles) ?: ''),
    '#description' => t('This role will be given to users that have had at least one comment approved. It will be taken away again when one of their comments is flagged offensive/spam.'),
  );
  $form['thresholds']['comment_score_pre_threshold_trusted'] = array(
    '#type' => 'textfield',
    '#title' => t('Pre-moderation threshold for users with the trusted role'),
    '#description' => t('At what score should a comment be placed into the pre-moderation queue (and not instant published). In general this should be higher than the threshold for non-trusted users.'),
    '#size' => 3,
    '#maxlength' => 3,
    '#element_validate' => array('element_validate_integer'),
    '#default_value' => variable_get('comment_score_pre_threshold_trusted', 25),
    '#required' => TRUE,
  );

  // Blacklisted words.
  $form['blacklist'] = array(
    '#type' => 'fieldset',
    '#title' => t('Blacklisted words'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['blacklist']['comment_score_blacklist_penalty'] = array(
    '#type' => 'textfield',
    '#title' => t('Blacklisted words penalty'),
    '#description' => t('This is the amount of penalty incurred for each blacklisted word matched in the comment text.'),
    '#size' => 3,
    '#maxlength' => 3,
    '#element_validate' => array('element_validate_integer_positive'),
    '#default_value' => variable_get('comment_score_blacklist_penalty', 25),
    '#required' => TRUE,
  );
  $default_words = trim(file_get_contents(drupal_get_path('module', 'comment_score') . '/blacklist.txt'));
  $form['blacklist']['comment_score_blacklist_words'] = array(
    '#type' => 'textarea',
    '#title' => t('Words to penalise'),
    '#rows' => 25,
    '#description' => t('Every word that appears in the comment from this list will penalise the comment rank. You can use standard regex here. Please enter one matched word per line.'),
    '#default_value' => variable_get('comment_score_blacklist_words', $default_words),
  );

  // Capital letters.
  $form['captials'] = array(
    '#type' => 'fieldset',
    '#title' => t('Capital letters'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['captials']['comment_score_capital_good_ratio'] = array(
    '#type' => 'textfield',
    '#title' => t('Good percent of capital letters'),
    '#description' => t('This will positively effect the modifier for the comment if they have less than this amount.'),
    '#field_suffix' => '%',
    '#size' => 3,
    '#maxlength' => 3,
    '#element_validate' => array('element_validate_integer_positive'),
    '#default_value' => variable_get('comment_score_capital_good_ratio', 5),
    '#required' => TRUE,
  );
  $form['captials']['comment_score_capital_bad_ratio'] = array(
    '#type' => 'textfield',
    '#title' => t('Bad percent of capital letters'),
    '#description' => t('This will negatively effect the modifier for the comment if they have more than this amount.'),
    '#field_suffix' => '%',
    '#size' => 3,
    '#maxlength' => 3,
    '#element_validate' => array('element_validate_integer_positive'),
    '#default_value' => variable_get('comment_score_capital_bad_ratio', 25),
    '#required' => TRUE,
  );

  // URLs
  $form['urls'] = array(
    '#type' => 'fieldset',
    '#title' => t('URLs'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['urls']['comment_score_urls_allowed'] = array(
    '#type' => 'textfield',
    '#title' => t('URLs allowed'),
    '#description' => t('The number of URLs in a comment that are allowed before negative impacts.'),
    '#size' => 3,
    '#maxlength' => 3,
    '#element_validate' => array('element_validate_integer_positive'),
    '#default_value' => variable_get('comment_score_urls_allowed', 1),
    '#required' => TRUE,
  );
  $form['urls']['comment_score_urls_modifier'] = array(
    '#type' => 'textfield',
    '#title' => t('Bad URL modifier'),
    '#description' => t('This will negatively effect the modifier for the comment if they have more than this amount.'),
    '#size' => 3,
    '#maxlength' => 3,
    '#element_validate' => array('element_validate_integer_positive'),
    '#default_value' => variable_get('comment_score_urls_modifier', 25),
    '#required' => TRUE,
  );

  // Length.
  $form['length'] = array(
    '#type' => 'fieldset',
    '#title' => t('Length'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['length']['comment_score_length_minimum'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum length of the comment'),
    '#description' => t('For every character shorter than this they will get a negative modifier.'),
    '#size' => 6,
    '#maxlength' => 6,
    '#element_validate' => array('element_validate_integer_positive'),
    '#default_value' => variable_get('comment_score_length_minimum', 50),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}


/**
 * Tests the current comment settings against the rules in place.
 */
function comment_score_test($form, &$form_state) {
  $form['comment'] = array(
    '#type' => 'fieldset',
    '#title' => t('Comment score test'),
  );
  $form['comment']['comment_score_test_comment'] = array(
    '#type' => 'textarea',
    '#title' => t('Text'),
    '#rows' => 10,
    '#description' => t('Enter some sample text here as if it was a comment, and the comment score will be given. Lower scores indicate better comments.'),
    '#default_value' => isset($form_state['values']['comment_score_test_comment']) ?: '',
  );
  $form['comment']['comment_score_test_role'] = array(
    '#type' => 'select',
    '#title' => t('Comment from a trusted commenter?'),
    '#options' => array(
      0 => t('Not trusted'),
      1 => t('Trusted'),
    ),
    '#default_value' => 0,
    '#description' => t('You can choose whether this comment will appear to come from a trusted user or not.'),
  );
  $form['comment']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Debug comment score'),
    '#submit' => array('comment_score_test_comment'),
  );

  return $form;
}

/**
 * Prints debug information to the screen for the score.
 */
function comment_score_test_comment($form, &$form_state) {
  $values = $form_state['values'];
  $text = $values['comment_score_test_comment'];
  $text = trim($text);
  $result = comment_score_text($text, NULL, TRUE);

  // Find the appropriate limit for good and bad comments.
  if ($values['comment_score_test_role'] == 0) {
    $limit = variable_get('comment_score_pre_threshold', -10);
  }
  else {
    $limit = variable_get('comment_score_pre_threshold_trusted', 25);
  }

  if ($result['score'] > $limit) {
    drupal_set_message(t("Bad comment - score: %score (limit: %limit), debug:\n\n<pre>@debug</pre>", array(
      '%score' => $result['score'],
      '%limit' => $limit,
      '@debug' => print_r($result['debug'], TRUE),
    )), 'error');
  }
  else {
    drupal_set_message(t("Good comment - score: %score (limit: %limit), debug:\n\n<pre>@debug</pre>", array(
      '%score' => $result['score'],
      '%limit' => $limit,
      '@debug' => print_r($result['debug'], TRUE),
    )), 'status');
  }

  $form_state['values']['comment_score_test_comment'] = $text;
  $form_state['rebuild'] = TRUE;
}
