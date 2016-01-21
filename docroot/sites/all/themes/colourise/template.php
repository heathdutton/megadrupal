<?php

/**
 * @file
 * Force refresh of theme registry.
 * DEVELOPMENT USE ONLY - COMMENT OUT FOR PRODUCTION
 */
// drupal_rebuild_theme_registry();

function colourise_preprocess(&$vars, $hook) {
  global $theme;
  // Set Page Class
  $vars['page_class'] = theme_get_setting('colourise_page_class');

  // Hide breadcrumb on all pages
  if (theme_get_setting('colourise_breadcrumb') == 0) {
    $vars['breadcrumb'] = '';
  }

  // Set Back to Top link toggle
  $vars['to_top'] = theme_get_setting('colourise_totop');
  if (theme_get_setting('colourise_totop') == 0) {
    $vars['to_top'] = '';
  }
  else {
    $vars['to_top'] = '<p id="to-top"><a href="#page">' . t('Back To Top') . '</a></p>';
  }

}

/**
 *  Create some custom classes for comments
 */
function comment_classes($comment) {
  $node = node_load($comment->nid);
  global $user;
  $output = '';
  $output .= ($comment->new) ? ' comment-new' : '';
  //$output .=  ' '. $status .' ';
  if ($node->name == $comment->name) {
    $output .= 'node-author';
  }
  if ($user->name == $comment->name) {
    $output .=  ' mine';
  }
  return $output;
}

/**
 *  Set Custom Stylesheet
 */
if (theme_get_setting('colourise_custom')) {
  drupal_add_css(drupal_get_path('theme', 'colourise') . '/css/' . 'custom.css', 'theme');
}

/**
 *  Add IE PNG Transparent fix
 */
if (theme_get_setting('colourise_iepngfix')) {
  drupal_add_js(drupal_get_path('theme', 'colourise') . '/js/jquery.pngFix.js', 'theme');
}


function colourise_preprocess_page(&$vars) {
  // Theme Main and secondary links.
  if (isset($vars['main_menu'])) {
    $vars['primary_nav'] = theme('links__system_main_menu', array(
        'links' => $vars['main_menu'],
        'attributes' => array(
          'class' => array('links', 'inline', 'main-menu'),
        ),
    ));
  }
  else {
    $vars['primary_nav'] = FALSE;
  }

  if (isset($vars['secondary_menu'])) {
    $vars['secondary_menus'] = theme('links', $vars['secondary_menu'], array('class' => 'links secondary-menu'));
  }

  // Set Accessibility nav bar
  if (isset($vars['main_menu'])) {
    $vars['nav_access'] = '
    <ul id="nav-access" class="hidden">
      <li><a href="#main-menu" accesskey="N" title="' . t('Skip to Main Menu') . '">' . t('Skip to Main Menu') . '</a></li>
      <li><a href="#main-content" accesskey="M" title="' . t('Skip to Main Content') . '">' . t('Skip to Main Content') . '</a></li>
    </ul>
  ';
  }
  else {
    $vars['nav_access'] = '
    <ul id="nav-access" class="hidden">
      <li><a href="#main-content" accesskey="M" title="' . t('Skip to Main Content') . '">' . t('Skip to Main Content') . '</a></li>
    </ul>
  ';
  }
}

/**
 * Return a themed breadcrumb trail.
 *
 * @param $breadcrumb
 *   An array containing the breadcrumb links.
 * @return a string containing the breadcrumb output.
 */

function colourise_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];
  if (!empty($breadcrumb)) {
    array_shift($breadcrumb);
    //$breadcrumb[0]= $first_link;
    return '<div class="breadcrumb">' . implode(' » ', $breadcrumb) . '</div>';
  }
}
