<?php
/**
 * @file
 * Contains theme preprocess functions
 */
 
 /**
  * Override or insert variables into the html template.
  */
function simpleclean_preprocess_html(&$vars) {
  // Add conditional CSS for IE6.
  drupal_add_css(path_to_theme() . '/ie6.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'lt IE 7', '!IE' => FALSE), 'preprocess' => FALSE));
}

/**
 * Format submitted by in articles
 */
function simpleclean_preprocess_node(&$vars) {
  $node = $vars['node'];
  $vars['date'] = format_date($node->created, 'custom', 'd M Y');

  if (variable_get('node_submitted_' . $node->type, TRUE)) {
    $vars['display_submitted'] = TRUE;
    $vars['submitted'] = t('By @username on !datetime', array('@username' => strip_tags(theme('username', array('account' => $node))), '!datetime' => $vars['date']));
    $vars['user_picture'] = theme_get_setting('toggle_node_user_picture') ? theme('user_picture', array('account' => $node)) : '';
    
    // Add a footer for post
    $account = user_load($vars['node']->uid);
    $vars['simpleclean_postfooter'] = '';
    if (!empty($account->signature)) {  
      $postfooter = "<div class='post-footer'>" . $vars['user_picture'] . "<h3>" . check_plain(format_username($account)) . "</h3>";
      $cleansignature = strip_tags($account->signature);
      $postfooter .= "<p>" . check_plain($cleansignature) . "</p>";
      $postfooter .= "</div>";
      $vars['simpleclean_postfooter'] = $postfooter;
    } 
  }
  else {
    $vars['display_submitted'] = FALSE;
    $vars['submitted'] = '';
    $vars['user_picture'] = '';
  }
  
  // Remove Add new comment from teasers on frontpage
  
  if ($vars['is_front']) {
    unset($vars['content']['links']['comment']['#links']['comment-add']);
    unset($vars['content']['links']['comment']['#links']['comment_forbidden']);
  }
  
}

/**
 * Format submitted by in comments
 */
function simpleclean_preprocess_comment(&$vars) {
  $comment = $vars['elements']['#comment'];
  $node = $vars['elements']['#node'];
  $vars['created']   = format_date($comment->created, 'custom', 'd M Y');
  $vars['changed']   = format_date($comment->changed, 'custom', 'd M Y');
  $vars['submitted'] = t('By @username on !datetime at about @time.', array('@username' => strip_tags(theme('username', array('account' => $comment))), '!datetime' => $vars['created'], '@time' => format_date($comment->created, 'custom', 'H:i')));
}

/**
 * Change button to Post instead of Save
 */
function simpleclean_form_comment_form_alter(&$form, &$form_state, &$form_id) {
 $form['actions']['submit']['#value'] = t('Post');
 $form['comment_body']['#after_build'][] = 'configure_comment_form'; 
}

function configure_comment_form(&$form) {
  $form['und'][0]['format']['#access'] = FALSE;
  return $form;
}