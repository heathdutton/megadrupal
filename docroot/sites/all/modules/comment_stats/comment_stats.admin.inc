<?php
/**
 * @file
 * Comment stats configuration form.
 */

/**
 * Comment stats configurations settings form.
 */
function comment_stats_admin_config_form() {
  // Set Title.
  drupal_set_title(t('Comment Stats'));
  $form  = array();
  $form['comment_stats_show_closed'] = array(
    '#type' => 'checkbox',
    '#title' => t('Do not show the closed comments'),
    '#description' => t('Hide the content for which comments are closed'),
    '#default_value' => variable_get('comment_stats_show_closed'),
  );
  $form['comment_stats_show_unpublished'] = array(
    '#type' => 'checkbox',
    '#title' => t('Do not show the unpublished comments'),
    '#description' => t('Hide the content which is unpublished'),
    '#default_value' => variable_get('comment_stats_show_unpublished'),
  );
  $form['comment_stats'] = array(
    '#type' => 'textfield',
    '#title' => t('Comment stats dashboard'),
    '#description' => t('Total number of records per page in comment stats dashboard'),
    '#maxlength' => 3,
    '#default_value' => variable_get('comment_stats'),
    '#size' => 20,
  );
  $form['comment_stats_users'] = array(
    '#type' => 'textfield',
    '#title' => t('Users in comment stats'),
    '#description' => t('Total number of users per page'),
    '#maxlength' => 3,
    '#default_value' => variable_get('comment_stats_users'),
    '#size' => 20,
  );
  $form['comment_stats_comments_per_user'] = array(
    '#type' => 'textfield',
    '#title' => t('Userwise comments/replies'),
    '#description' => t('Total number of comments/replies per page'),
    '#maxlength' => 3,
    '#default_value' => variable_get('comment_stats_comments_per_user'),
    '#size' => 20,
  );
  $form['comment_stats_posts'] = array(
    '#type' => 'textfield',
    '#title' => t('Comments'),
    '#description' => t('Total number of comments per page'),
    '#maxlength' => 3,
    '#default_value' => variable_get('comment_stats_posts'),
    '#size' => 20,
  );
  $form['comment_stats_replies'] = array(
    '#type' => 'textfield',
    '#title' => t('Replies'),
    '#description' => t('Total number of replies per page'),
    '#maxlength' => 3,
    '#default_value' => variable_get('comment_stats_replies'),
    '#size' => 20,
  );
  $form['comment_stats_unapproved'] = array(
    '#type' => 'textfield',
    '#title' => t('Unapproved comments/replies'),
    '#description' => t('Total number of unapproved comments/replies per page'),
    '#maxlength' => 3,
    '#default_value' => variable_get('comment_stats_unapproved'),
    '#size' => 20,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  return $form;
}

/**
 * Validating comment stats configurations form.
 */
function comment_stats_admin_config_form_validate($form, &$form_state) {

  $comment_stats = $form_state['values']['comment_stats'];
  $comment_stats_users = $form_state['values']['comment_stats_users'];
  $comment_stats_comments_per_user = $form_state['values']['comment_stats_comments_per_user'];
  $comment_stats_posts = $form_state['values']['comment_stats_posts'];
  $comment_stats_replies = $form_state['values']['comment_stats_replies'];
  $comment_stats_unapproved = $form_state['values']['comment_stats_unapproved'];

  if (!(is_numeric($comment_stats))) :
    form_set_error('comment_stats', t('Comment stats dashboard value should be numeric.'));
  else :
    if (strpos($comment_stats, ".") !== FALSE) :
      form_set_error('comment_stats', t('Comment stats dashboard value should be integer.'));
    elseif ($comment_stats < 10) :
      form_set_error('comment_stats', t('Comment stats dashboard minimum value should be 10.'));
    endif;
  endif;
  if (!(is_numeric($comment_stats_users))) :
    form_set_error('comment_stats_users', t('Users in comment stats value should be numeric.'));
  else :
    if (strpos($comment_stats_users, ".") !== FALSE) :
      form_set_error('comment_stats_users', t('Users in comment stats value should be integer.'));
    elseif ($comment_stats_users < 10) :
      form_set_error('comment_stats_users', t('Users in comment stats minimum value should be 10.'));
    endif;
  endif;
  if (!(is_numeric($comment_stats_comments_per_user))) :
    form_set_error('comment_stats_comments_per_user', t('Userwise comments/replies value should be numeric.'));
  else :
    if (strpos($comment_stats_comments_per_user, ".") !== FALSE) :
      form_set_error('comment_stats_comments_per_user', t('Userwise comments/replies value should be integer.'));
    elseif ($comment_stats_comments_per_user < 10) :
      form_set_error('comment_stats_comments_per_user', t('Userwise comments/replies minimum value should be 10.'));
    endif;
  endif;
  if (!(is_numeric($comment_stats_posts))) :
    form_set_error('comment_stats_posts', t('Comments value should be numeric.'));
  else :
    if (strpos($comment_stats_posts, ".") !== FALSE) :
      form_set_error('comment_stats_posts', t('Comments value should be integer.'));
    elseif ($comment_stats_posts < 10) :
      form_set_error('comment_stats_posts', t('Comments minimum value should be 10.'));
    endif;
  endif;
  if (!(is_numeric($comment_stats_replies))) :
    form_set_error('comment_stats_replies', t('Replies value should be numeric.'));
  else :
    if (strpos($comment_stats_replies, ".") !== FALSE) :
      form_set_error('comment_stats_replies', t('Replies value should be integer.'));
    elseif ($comment_stats_replies < 10) :
      form_set_error('comment_stats_replies', t('Replies minimum value should be 10.'));
    endif;
  endif;
  if (!(is_numeric($comment_stats_unapproved))) :
    form_set_error('comment_stats_unapproved', t('Unapproved comments/replies value should be numeric.'));
  else :
    if (strpos($comment_stats_unapproved, ".") !== FALSE) :
      form_set_error('comment_stats_unapproved', t('Unapproved comments/replies value should be integer.'));
    elseif ($comment_stats_unapproved < 10) :
      form_set_error('comment_stats_unapproved', t('Unapproved comments/replies minimum value should be 10.'));
    endif;
  endif;
}

/**
 * Submitting comment stats configurations form.
 */
function comment_stats_admin_config_form_submit($form, &$form_state) {
  $comment_stats_show_closed = $form_state['values']['comment_stats_show_closed'];
  $comment_stats_show_unpublished = $form_state['values']['comment_stats_show_unpublished'];
  $comment_stats = $form_state['values']['comment_stats'];
  $comment_stats_users = $form_state['values']['comment_stats_users'];
  $comment_stats_comments_per_user = $form_state['values']['comment_stats_comments_per_user'];
  $comment_stats_posts = $form_state['values']['comment_stats_posts'];
  $comment_stats_replies = $form_state['values']['comment_stats_replies'];
  $comment_stats_unapproved = $form_state['values']['comment_stats_unapproved'];
  variable_set('comment_stats_show_closed', $comment_stats_show_closed);
  variable_set('comment_stats_show_unpublished', $comment_stats_show_unpublished);
  variable_set('comment_stats', $comment_stats);
  variable_set('comment_stats_users', $comment_stats_users);
  variable_set('comment_stats_comments_per_user', $comment_stats_comments_per_user);
  variable_set('comment_stats_posts', $comment_stats_posts);
  variable_set('comment_stats_replies', $comment_stats_replies);
  variable_set('comment_stats_unapproved', $comment_stats_unapproved);
  drupal_set_message(t('Comment stats configuration saved successfully.'));
}
