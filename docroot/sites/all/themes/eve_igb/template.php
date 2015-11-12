<?php

function eve_igb_menu_local_task($link, $active) {
  if($active) {
    return ' ['. $link .'] ';
  }
  else {
    return ' '. $link .'';
  }
}

function eve_igb_menu_local_tasks() {
  $output = '';

  if ($primary = menu_primary_local_tasks()) {
    $output .= "<h4>". $primary ."</h4>";
  }
  if ($secondary = menu_secondary_local_tasks()) {
    $output .= "<h5>". $secondary ."</h5>";
  }

  return $output;
}

function eve_igb_menu_links($links){
  $o = '';
  foreach ($links as $k => $v) {
    $o .= l($v['title'], $v['href'], $v['attributes']). "\n";
  }
  return $o;
}

function isEveT() {
  // Template copy of isEve().
  return (!(strpos($_SERVER['HTTP_USER_AGENT'],"EVE-minibrowser") === FALSE));
}

/////////////////////////// Flatforum ///////////////////////////
/*
function _phptemplate_variables($hook, $vars) {
  static $is_forum;
  $variables = array();
  if (!isset($is_forum)) {
    if (arg(0) == 'node' && is_numeric(arg(1)) && arg(2) == '') {
      $nid = arg(1);
    }
    if (arg(0) == 'comment' && arg(1) == 'reply' && is_numeric(arg(2))) {
      $nid = arg(2);
    }
    if ($nid) {
      $node = node_load(array('nid' => $nid));
    }
    $is_forum = ($node && $node->type == 'forum');
    _is_forum($is_forum);
  }
  if ($is_forum) {
    switch ($hook) {
      case 'comment' :
        $variables['template_file'] = 'node-forum';
        $variables['row_class'] = _row_class();
        $variables['name'] = $vars['author'];
        $variables['userid'] = $vars['comment']->uid;
        $joined = module_invoke('flatforum', 'get_created', $vars['comment']->uid);
        $variables['joined'] = $joined ? format_date($joined, 'custom', 'Y-m-d') : '';
        $posts = module_invoke('flatforum', 'get', $vars['comment']->uid);
        $variables['posts'] = $posts ? $posts : 0;
        $variables['submitted'] = format_date($vars['comment']->timestamp);
        $subject = $vars['comment']->subject;
        $variables['title'] = empty($subject) ? '&nbsp' : $subject;
        $variables['content'] = $vars['comment']->comment;
        $variables['links'] = empty($vars['links']) ? '&nbsp' : $vars['links'];
        break;
      case 'node' :
        $variables['row_class'] = _row_class();
        $variables['userid']=$vars['node']->uid;
        $joined = module_invoke('flatforum', 'get_created', $vars['node']->uid);
        $variables['joined'] = $joined ? format_date($joined, 'custom', 'Y-m-d') : '';
        $posts = module_invoke('flatforum', 'get', $vars['node']->uid);
        $variables['posts'] = $posts ? $posts : 0;
        $variables['title'] = empty($vars['title']) ? '&nbsp' : $vars['title'];
        $variables['content'] = $vars['node']->body;
        $variables['links'] = empty($vars['links']) ? '&nbsp' : $vars['links'];
        break;
    }
  }
  return $variables;
}
function _row_class() {
  static $forum_row = TRUE;
  $forum_row = !$forum_row;
  return $forum_row ? 'odd' : 'even';
}
function _is_forum($arg = NULL) {
  static $is_forum = FALSE;
  if ($arg) {
    $is_forum = $arg;
  }
  return $is_forum;
}

*/
