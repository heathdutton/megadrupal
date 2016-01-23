<?php
/**
 * Modify page variables.
 */
function arthemia_preprocess_page(&$variables) {
  $variables['footer_message'] = empty($variables['footer_message'])? '' : $variables['footer_message'] . ' | ';
  $variables['footer_message'] .= '<a href="http://drupal.org/project/arthemia">Arthemia</a> is based on the original design by <a href="http://michaelhutagalung.com">Michael Hutagalung</a>.';
}

/**
 * Return a full tree of the expanded menu. Thank you multiflex-3 for this code!
 */ 
function arthemia_primary() {
  $output = '<div id="page-bar">';
  $output .= drupal_render(menu_tree(variable_get('menu_main_links_source','main-menu')));
  $output .= '</div>';
  return $output;
}

/**
 * Return a themed breadcrumb trail, but only if there is more than one link in it.
 */
function arthemia_breadcrumb($breadcrumb) {
  if (count($breadcrumb) > 1) {
    return '<div class="breadcrumb">'. implode(' &rsaquo; ', $breadcrumb) .'</div>';
  }
}

/**
 * Allow themable wrapping of all comments.
 */
function phptemplate_comment_wrapper($content, $node) {
  $comments_per_page = _comment_get_display_setting('comments_per_page', $node);
  $content = theme('pager', NULL, $comments_per_page, 0) . $content;
  if (!$content || $node->type == 'forum') {
    return '<div id="comments">'. $content .'</div>';
  }
  else {
    return '<div id="comments"><h2 class="comments">'. t('Comments') .'</h2>'. $content .'</div>';
  }
}

/**
 * Override or insert variables into the comment template.
 */
function arthemia_process_comment(&$variables) {
  $count = &drupal_static(__FUNCTION__);
  if (!isset($count)) {
    // If the comments are paginated, offset the count appropriately.
    $count = isset($_GET['page']) ? $_GET['page'] * variable_get('comment_default_per_page_' . $variables['node']->type, 1) : 0;
  }

  // Reset the permalink variable to include the current count.
  $variables['permalink'] = l('#' . ++$count, 'comment/' . $variables['comment']->cid, array('attributes' => array('class' => 'permalink'), 'fragment' => 'comment-' . $variables['comment']->cid));
}

/**
 * Modify the theme search box. Thank you http://agaric.com/note/theme-search-form-drupal-6 for instructions.
 */
function arthemia_preprocess_search_theme_form(&$vars, $hook) {
  // Remove the search box title.
  unset($vars['form']['search_theme_form']['#title']);
  
  // Replace the submit button with an image.
  $theme_path = drupal_get_path('theme', 'arthemia');
  $vars['form']['submit'] = array('#type' => 'image_button', '#value' => t('Search'),
                             '#src'  => $theme_path . '/images/magnify.gif');

  // Rebuild the rendered version (search form only, rest remains unchanged)
  unset($vars['form']['search_theme_form']['#printed']);
  $vars['search']['search_theme_form'] = drupal_render($vars['form']['search_theme_form']);

  // Rebuild the rendered version (submit button, rest remains unchanged)
  unset($vars['form']['submit']['#printed']);
  $vars['search']['submit'] = drupal_render($vars['form']['submit']);

  // Collect all form elements to make it easier to print the whole form.
  $vars['search_form'] = implode($vars['search']);
}