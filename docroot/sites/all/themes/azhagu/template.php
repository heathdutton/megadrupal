<?php
/**
 * @file
 * Contains the theme's functions to manipulate Drupal's default markup.
 */

/**
 * Implements template_preprocess_page().
 */
function azhagu_preprocess_page(&$variables) {
  $main_menu_tree = menu_tree_all_data('main-menu');
  $variables['main_menu_expanded'] = menu_tree_output($main_menu_tree);
  $variables['social_block_title'] = check_plain(theme_get_setting('social_block_title', 'azhagu'));
  $variables['default_title'] = 'Follow Us';
  $variables['facebook_url'] = check_url(theme_get_setting('facebook_url', 'azhagu'));
  $variables['twitter_url'] = check_url(theme_get_setting('twitter_url', 'azhagu'));
  $variables['gplus_url'] = check_url(theme_get_setting('gplus_url', 'azhagu'));
  $variables['pinterest_url'] = check_url(theme_get_setting('pinterest_url', 'azhagu'));
  $variables['path'] = base_path() . path_to_theme('theme', 'azhagu');
}

/**
 * Implements template_preprocess_node().
 */
function azhagu_preprocess_node(&$variables) {
  $variables['title_attributes_array']['class'][] = 'title';
  $node = $variables['node'];
  if (isset($node->created)) {
    $variables['date_day'] = format_date($node->created, 'custom', 'j');
    $variables['date_month'] = format_date($node->created, 'custom', 'M');
    $variables['date_year'] = format_date($node->created, 'custom', 'Y');
  }
  $variables['content']['field_tags']['#theme'] = 'links';
  unset($variables['content']['links']['comment']);

  // Change the read more html.
  unset($variables['content']['links']['node']['#links']['node-readmore']);
  if (isset($node->nid)) {
    $variables['newreadmore'] = l(t('Read More'), 'node/' . $node->nid);
  }
}

/**
 * Implements theme_breadcrumb().
 */
function azhagu_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];
  $delimiter = check_plain(theme_get_setting('breadcrumb_delimiter'));

  if (!empty($breadcrumb)) {
    $output = '<h2 class="element-invisible">' . t('You are here') . '</h2>';
    $breadcrumb[] = drupal_get_title();
    $output .= '<nav class="breadcrumb">' . implode($delimiter, $breadcrumb) . '</nav>';
    return $output;
  }
}

/**
 * Implements template_preprocess_username().
 */
function azhagu_preprocess_username(&$variables) {
  $account = user_load($variables['account']->uid);

  if (isset($account->field_real_name[LANGUAGE_NONE][0]['safe_value'])) {
    $variables['name'] = $account->field_real_name[LANGUAGE_NONE][0]['safe_value'];
  }
}

/**
 * Implements template_preprocess_pager().
 */
function azhagu_preprocess_pager(&$variables, $hook) {
  // Removing the first & last from the pager.
  if ($variables['quantity'] > 5) {
    $variables['quantity'] = 5;
  }
  $variables['tags'][0] = FALSE;
  $variables['tags'][4] = FALSE;
}

/**
 * Implements template_preprocess_comment().
 */
function azhagu_preprocess_comment(&$variables) {
  $comment = $variables['elements']['#comment'];
  if (isset($comment->created)) {
    $variables['created'] = format_date($comment->created, 'custom', 'l, d/m/Y');
  }
  if (isset($comment->changed)) {
    $variables['changed'] = format_date($comment->changed, 'custom', 'l, d/m/Y');
  }
  if (isset($variables['author']) && isset($variables['created'])) {
    $variables['submitted'] = t('Submitted by !username on !datetime', array('!username' => $variables['author'], '!datetime' => $variables['created']));
  }
}

/**
 * Implements theme_menu_link().
 */
function azhagu_menu_link(array $variables) {
  $element = $variables['element'];
  $sub_menu = '';

  if ($element['#below']) {
    $sub_menu = drupal_render($element['#below']);
  }
  $output = l($element['#title'], $element['#href'], $element['#localized_options']);

  // If link class is active, make li class as active too.
  if (strpos($output, 'active') > 0) {
    $element['#attributes']['class'][] = 'active';
  }
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}

/**
 * Implements hook_page_alter().
 */
function azhagu_page_alter($page) {
  $viewport = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'viewport',
      'content' => 'width=device-width, initial-scale=1, maximum-scale=1',
    ),
  );
  drupal_add_html_head($viewport, 'viewport');
}
