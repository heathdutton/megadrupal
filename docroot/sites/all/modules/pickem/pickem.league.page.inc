<?php

/**
 * @file
 * Page callbacks for the league dashboard.
 */


/**
 * Menu callback for the league dashboard page.
 */
function league_page($league) {
  if ($league) {
    return theme('pickem_league', array(
      'content' => array(),
      'league' => $league,
    ));
  }
  else {
    return FALSE;
  }
}


/**
 * Theme function for the messages section of the league dashboard page.
 */
 function theme_league_messages($variables) {
  global $user;
  $league = $variables['league'];
  if (!module_exists("forum") || $league->forum_id == 0) {
    return '';
  }
  
  $output = '<div class="pickem-league-messages"><div class="pickem-league-messages-title">' . t('Messages');
  if (is_my_league($league)) {
    $output .= ' <div class="pickem-messages-addnew">' . l(t('Add new Message'), 'node/add/forum/' . $league->forum_id) . "</div>";
  }
  $output .= '</div>';

  $cur_w = get_current_week($league, TRUE);
  $output .= '<ul>';
  if ($cur_w != FALSE && $cur_w->first_gamestart != '' ) {
    if ( $cur_w->firstgame_dow <> "Sunday" ) {
      $output .= '<li class="warning">Warning: The first game is on <span style="text-decoration:underline;">' . strtoupper($cur_w->firstgame_dow) . '</span>.</li>';
    }
    if ($league->pick_lock == PICKEM_PICK_LOCK_WEEK) {
      $output .= '<li class="warning">Week ' . ($cur_w->wk_number) . ' picks due by ' . format_date($cur_w->first_gamestart, 'medium');
      if (module_exists("jstimer")) {
        $output .= ',<br/>' . get_countdown_timer($cur_w);
      }
      $output .= '</li>';
    }
  }

  if (module_exists("forum") && is_my_league($league) && $league->forum_id != 0) {
    $topics = forum_get_topics($league->forum_id, 1, 4);
    $i = 1;
    $content='';
    foreach ($topics as $topic) {
      if ( $i > 4 ) {
        break;
      }
      $i++;
      if (isset($topic->last_reply)) {
        $content .= '<li>' . l(t('Re:') . ' ' . $topic->title . ' - ' . $topic->last_reply->name, 'node/' . $topic->nid) . ' (' . format_date($topic->last_reply->created, 'short') . ')</li>';
      }
      else {
        $content .= '<li>' . l($topic->title . ' - ' . $topic->name, 'node/' . $topic->nid) . ' (' . format_date($topic->last_comment_timestamp, 'short') . ')</li>';
      }
    }
    $output .= $content;
  }

  if (strlen($output) > 4) {
    $content = "<li>None</li>";
  }
  $output .= '</ul></div>';

  return $output;
}
