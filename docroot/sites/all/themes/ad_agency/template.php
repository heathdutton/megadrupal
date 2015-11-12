<?php

/**
 * Override or insert variables into the page template.
 */
function ad_agency_preprocess_page(&$vars) {
  if (isset($vars['main_menu'])) {
    $vars['primary_nav'] = theme('links__system_main_menu', array(
      'links' => $vars['main_menu'],
      'attributes' => array(
        'class' => array('links', 'inline', 'main-menu'),
      ),
      'heading' => array(
        'text' => t('Main menu'),
        'level' => 'h2',
        'class' => array('element-invisible'),
      )
    ));
  }
  else {
    $vars['primary_nav'] = FALSE;
  }
  if (isset($vars['secondary_menu'])) {
    $vars['secondary_nav'] = theme('links__system_secondary_menu', array(
      'links' => $vars['secondary_menu'],
      'attributes' => array(
        'class' => array('links', 'inline', 'secondary-menu'),
      ),
      'heading' => array(
        'text' => t('Secondary menu'),
        'level' => 'h2',
        'class' => array('element-invisible'),
      )
    ));
  }
  else {
    $vars['secondary_nav'] = FALSE;
  }
}

/**
 * Override or insert variables into the node templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 */
function ad_agency_preprocess_node(&$vars) {
  global $user;

  // Special classes for nodes
  $classes = array('node');
  if ($vars['sticky']) {
    $classes[] = 'sticky';
  }
  if (!$vars['status']) {
    $classes[] = 'node-unpublished';
    $vars['unpublished'] = TRUE;
  }
  else {
    $vars['unpublished'] = FALSE;
  }
  if ($vars['uid'] && $vars['uid'] == $GLOBALS['user']->uid) {
    $classes[] = 'node-mine'; // Node is authored by current user.
  }
  if ($vars['teaser']) {
    $classes[] = 'node-teaser'; // Node is displayed as teaser.
  }
  // Class for node type: "node-type-page", "node-type-story", "node-type-my-custom-type", etc.
  $classes[] = ad_agency_id_safe('ntype-' . $vars['type']);
  $vars['classes'] = implode(' ', $classes); // Concatenate with spaces

  // set a new $is_admin variable
  // this is determined by looking at the currently logged in user and seeing if they are in the role 'admin'
  $vars['is_admin'] = in_array('admin', $user->roles);
}

/**
 * Override or insert variables into the comment templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("comment" in this case.)
 */
function ad_agency_preprocess_comment(&$vars, $hook) {
  // We load the node object that the current comment is attached to.
  $node = node_load($vars['comment']->nid);
  // If the author of this comment is equal to the author of the node, we set a
  // variable then in our theme we can theme this comment differently to stand
  // out.
  $vars['author_comment'] = $vars['comment']->uid == $node->uid ? TRUE : FALSE;
}

/**
 * Converts a string to a suitable html ID attribute.
 * - Preceeds initial numeric with 'n' character.
 * - Replaces space and underscore with dash.
 * - Converts entire string to lowercase.
 * - Works for classes too!
 *
 * @param $string
 *   The string
 * @return
 *   The converted string
 */
function ad_agency_id_safe($string) {
  if (is_numeric($string{0})) {
    // if the first character is numeric, add 'n' in front
    $string = 'n' . $string;
  }
  return strtolower(preg_replace('/[^a-zA-Z0-9-]+/', '-', $string));
}

/**
 * Allow themable wrapping of all comments.
 */
function phptemplate_comment_wrapper($content, $type = null) {
  static $node_type;
  if (isset($type)) $node_type = $type;

  if (!$content || $node_type == 'forum') {
    return '<div id="comments">' . $content . '</div>';
  }
  else {
    return '<h3 id="comments">' . t('Comments') . '</h3><ol class="commentlist">' . $content . '</ol>';
  }
}

