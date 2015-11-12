<?php

function education_preprocess_node(&$vars) {
  $vars['created_day'] = date('d', $vars['created']);
  $vars['created_month'] = date('M', $vars['created']);
  $vars['created_year'] = date('Y', $vars['created']);

  $vars['easy_social_widget'] = false;
  foreach($vars['content'] as $key => $value) {
    if(strpos($key, "easy_social") !== false) {
      $vars['easy_social_widget'] = $value;
      unset($vars['content'][$key]);
    }
  }
  $vars['comments_count'] = false;
  if(isset($vars['content']['links']['comment'])) {
    if(isset($vars['content']['links']['comment']['#links']['comment-comments'])) {
      $vars['comments_count'] = $vars['content']['links']['comment'];
      foreach($vars['comments_count']['#links'] as $key => $value) {
        if ($key != 'comment-comments') {
          unset($vars['comments_count']['#links'][$key]);
        }
      }
      $vars['comments_count']['#prefix'] = '<span class="comment-count-title"></span>';
      unset($vars['content']['links']['comment']['#links']['comment-comments']);
    }
  }
  $vars['page'] = ($vars['type'] == 'page') ? TRUE : FALSE;
}

function education_pager($vars) {
  if (drupal_is_front_page() && theme_get_setting('hide_pager_frontpage')) {
    return '';
  }
  return theme_pager($vars);
}