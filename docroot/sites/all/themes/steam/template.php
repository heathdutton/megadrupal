<?php
/**
 * @file
 * Theme override functions, and preprocess and process implementations for
 * the theme.
 */

/**
 * Implements hook_css_alter().
 */
function steam_css_alter(&$css) {
  $node = menu_get_object();
  if (!empty($node) || (arg(0) == 'footnotes' && !arg(1))) {
    // Exclude responsive styling on node pages and root footnote browser page.
    $exclude = array(
      drupal_get_path('theme', 'doune') . "/css/responsive.css" => FALSE,
      drupal_get_path('theme', 'steam') . "/css/responsive-layout.css" => FALSE,
    );
    $css = array_diff_key($css, $exclude);
  }
}

/**
 * Implements hook_doune_layouts_alter().
 */
function steam_doune_layouts_alter(&$layouts) {
  $layouts['fifty_fifty_left'] = array(
    'title_tabs_content' => array('eight', 'columns'),
    'sidebar_first' => array('eight', 'columns'),
  );
  $layouts['fifty_fifty_right'] = array(
    'title_tabs_content' => array('eight', 'columns', 'push-by-eight'),
    'sidebar_first' => array('eight', 'columns', 'pull-by-eight'),
  );
  $layouts['gallery'] = array(
    'title_tabs_content' => array('eleven', 'columns', 'push-by-five'),
    'sidebar_first' => array('five', 'columns', 'pull-by-eleven'),
  );
  $layouts['wide_open'] = array(
    'title_tabs_content' => array('sixteen', 'columns'),
  );
}

/**
 * Override or insert variables into the html templates.
 */
function steam_preprocess_html(&$vars) {
  // Need to manually include os_sharethis's javascript on this views page so
  // that the colorbox loading the visualizations can render the share widgets.
  if (context_isset('context', 'gallery_visualizations')) {
    os_sharethis_javascript();
  }
}

/**
 * Override or insert variables into the search results template.
 */
function steam_preprocess_search_results(&$vars) {
  if (isset($GLOBALS['pager_total_items']) && isset($GLOBALS['pager_total_items'][0])) {
    $vars['total_search_results'] = $GLOBALS['pager_total_items'][0];
    $vars['search_summary'] = t('Found !results containing: <span class="search-keywords">@terms</span>', array(
      '!results' => format_plural($vars['total_search_results'], '1 result', '@count results'),
      '@terms' => steam_search_terms(),
    ));
  }
}

/**
 * Retrieves or stores the current page's search terms (when on a search
 * results page).
 */
function steam_search_terms($terms = NULL) {
  static $search_terms = '';
  if (isset($terms)) {
    $search_terms = $terms;
  }
  return $search_terms;
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function steam_form_search_form_alter(&$form, $form_state) {
  // Remember what the search terms are so we can inject them into the
  // search block form.
  steam_search_terms($form['basic']['keys']['#default_value']);
  // Hide the search form.
  $form['advanced']['#access'] =
  $form['basic']['#access']    =
  $form['module']['#access']   = FALSE;
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function steam_form_search_block_form_alter(&$form, $form_state) {
  // Make the user's query appear in the search *block* form.
  $form['search_block_form']['#default_value'] = steam_search_terms();
}

/**
 * Override or insert variables into the individual search result template.
 */
function steam_preprocess_search_result(&$vars) {
  // Render the node in normal teaser view and substitute its body with the
  // snippet from the search engine.
  $elements = node_view($vars['result']['node'], 'teaser');
  $elements['body'][0]['#markup'] = $vars['snippet'];
  $vars['result'] = $elements;

  // Layout
  $vars['classes_array'][] = 'eight';
  $vars['classes_array'][] = 'columns';
  $vars['classes_array'][] = $vars['zebra'] === 'odd' ? 'alpha' : 'omega';
}

/**
 * Override or insert variables into the block templates.
 */
function steam_preprocess_block(&$vars) {
  $block = $vars['block'];
  if ($block->module === 'search' && $block->delta === 'form') {
    $vars['classes_array'][] = 'five columns omega';
  }
}
