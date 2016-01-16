<?php

/**
 * @file
 * Search by Page module API.
 *
 * The Search by Page module allows you to define sub-modules that
 * tell Search by Page what pages to add to the search index. You
 * supply a Drupal path to the page you want to index. The Search by
 * Page module builds the page during Search indexing, and adds page
 * content to the core Drupal Search index.
 *
 * There are several hooks a sub-module can implement:
 * - hook_sbp_paths() (required)
 * - hook_sbp_details() (required)
 * - hook_sbp_query_modify() (required)
 * - hook_sbp_settings() (optional)
 * - hook_sbp_delete_environment() (optional)
 *
 * There are also some helper functions defined in search_by_page.module that
 * may be useful for defining your sub-module. These are:
 * - search_by_page_force_reindex()
 * - search_by_page_force_remove()
 * - search_by_page_rebuild_paths()
 * - search_by_page_path_parts()
 * - search_by_page_excerpt()
 * - search_by_page_setting_get()
 * - search_by_page_setting_set()
 * - search_by_page_path_field_prefix()
 * - search_by_page_strip_tags()
 *
 * There is also a hook for a stemming or search preprocess module to
 * implement for better search excerpts:
 * - hook_sbp_excerpt_match().
 *
 * @ingroup search_by_page
 */

/**
 * Return a list of pages to be indexed (required sub-module hook).
 *
 * This hook is invoked whenever Search is building/adding to its
 * index (usually during a cron run).
 *
 * @param $environment
 *   ID of environment currently being indexed.
 *
 * @return
 *   Associative array of path items. The array keys are the paths to be
 *   indexed. Each value is an array, with the following components:
 *   - 'id': ID number (unsigned integer) for module to reference this
 *     path. This is passed into the module's hook_sbp_details()
 *     implementation when Search by Pages needs information about the page.
 *   - 'languages': Language codes to use to index this path. The
 *     page will be rendered in each language during the indexing process, and
 *     indexed separately for each language. Do not include the language prefix
 *     in the path. Must include at least one language.
 *   - 'role': ID of role to use when indexing this path. The page will be
 *     rendered as if being viewed by a user with this role's permissions (or
 *     not indexed at all if that role cannot view the page), and all visible
 *     text will be indexed and available for searching. Permissions will still
 *     be checked when displaying search results, however.
 *   - 'min_time' (default 1): Minimum time to wait to reindex this item,
 *     in seconds. Search by Page cycles through and reindexes items, because
 *     they are rendered through the theme, and it is assumed that rendering may
 *     depend on other content that can change. Each cron run, it first indexes
 *     any items that are new or have been marked as needing reindexing. Then
 *     it reindexes the oldest other items in its index, up to the cron throttle
 *     limit. However, no item will be reindexed before this minimum index time
 *     has been exceeded. Set to 0 for items that should only be indexed once
 *     and never reindexed unless search_by_page_force_reindex() is called.
 *   - 'max_time' (default 0): Maximum time to wait to reindex this item,
 *     in seconds. This effectively marks the item as needing immediate
 *     reindexing after this much time has passed. Set to 0 for items that do
 *     not need this to happen. See 'min_time' entry above for more details on
 *     reindexing.
 *
 * @see hook_sbp_details()
 */
function hook_sbp_paths($environment) {
  return array(
    'my/drupal/path' => array(
      'id' => 1234,
      'languages' => array('en', 'es'),
      'role' => DRUPAL_ANONYMOUS_RID,
    ),
  );
}

/**
 * Return details about an indexed path (required sub-module hook).
 *
 * This hook is invoked after a search, to get full information about
 * what to display.
 *
 * This hook is also invoked during search indexing to get the page title
 * which is added to the indexed text. In this case, $keys will be null, and
 * the module can also return rendered content to be indexed, if desired.
 *
 * @param $id
 *    The ID corresponding to the path. This is the ID number you
 *    returned in hook_sbp_paths(), for this path.
 * @param $environment
 *   ID of environment currently being indexed or searched.
 * @param $keys
 *    The keywords being searched for (useful in extracting a snippet). NULL
 *    indicates this call is for search indexing.
 *
 * @return
 *    - If for some reason this path should not be displayed or indexed,
 *      return NULL or zero.
 *    - If $keys is null, return an associative array for search
 *      indexing, with component 'title' (the page title) and optional
 *      component 'content' (an override of the page content, to avoid
 *      standard Drupal rendering).
 *    - If keywords are not null, return an associative array of fields
 *      suitable for display on search results screen for this path. See
 *      the Drupal documentation for hook_search() for a list of what
 *      the fields are. The 'title' component must be given.  The 'link'
 *      component should be omitted (it is handled by the main
 *      search_by_page module).  The search_by_page_excerpt() function may be
 *      useful in extracting a 'snippet'.
 *
 * @see hook_sbp_paths()
 */
function hook_sbp_details($id, $environment, $keys = NULL) {
  $node = my_module_get_node($id);
  $type = node_type_get_type($node);
  return array(
    'type' => check_plain($type->name),
    'title' => search_by_page_strip_tags($node->title, $environment),
    'user' => theme('username', array('account' => $node)),
    'date' => $node->changed,
    'extra' => $node->extra,
    'snippet' => search_by_page_excerpt($keys, search_by_page_strip_tags($node->body[LANGUAGE_NONE][0]['value'], $environment)),
  );
}

/**
 * Add conditions to the search query (required sub-module hook).
 *
 * This hook allows a module to add conditions and joins (usually related to
 * access permissions) to the query that finds the search
 * results.
 *
 * @param $environment
 *    Environment ID being queried.
 * @param $query
 *    Query object being constructed. You can join to it using the "sp.modid"
 *    column; this will contain the ID your module returned in hook_sbp_paths().
 *    All joins should be LEFT JOIN. You can also add tags and meta-data as
 *    necessary.
 *
 * @return
 *    db_and() condition object to add to the query. Do not add your conditions
 *    directly, as they need to be combined with other modules' conditions.
 */
function hook_sbp_query_modify($environment, $query) {
  // This example gives access to every path defined by this module.
  return $db_and()->condition(1);
}

/**
 * Add settings to the Search by Page settings screen (sub-module hook).
 *
 * The returned form array is displayed as a component of the page at URL
 * admin/config/search/search_by_page/%env, and the settings are automatically saved
 * using the form element names and the search_by_page_setting_set() function.
 *
 * If your module has no settings, you need not implement this
 * hook. If your module has many settings, you can also define your own
 * settings page with path 'admin/config/search/search_by_page/whatever' of
 * type MENU_LOCAL_TASK, to add a tab to the settings page. You can use
 * the function search_by_page_setting_get() to retrieve the setting.
 *
 * Note that the database of paths is updated after the settings are
 * submitted, so you do not need to do anything special to make this happen
 * if you use this hook. If you use your own settings page, you may want
 * to make use of these three functions:
 *   search_by_page_force_reindex()
 *   search_by_page_force_remove()
 *   search_by_page_rebuild_paths()
 *
 * @param $environment
 *   ID of environment for settings.
 *
 * @return
 *   Form API array that defines one or more fieldsets to add to the
 *   Search by Page settings screen at admin/config/search/search_by_page/%env.
 *   If you want to use weights for your fieldsets, choose numbers between
 *   -98 and -1.
 */
function hook_sbp_settings($environment) {
  $form = array();
  // add your settings form components here
  return $form;
}

/**
 * Respond to search environment deletion.
 *
 * Implement this hook if your sub-module needs to respond to the deletion
 * of a Search by Page search environment. You do not need to clear settings
 * saved with search_by_page_setting_set() or defined in hook_sbp_settings(),
 * as the main module will take care of this.
 *
 * @param $environment
 *   ID of environment being deleted.
 */
function hook_sbp_delete_environment($environment) {
  // Delete entries in my module's table, etc.
}

/**
 * Find derived forms of keywords for search excerpt (preprocess module hook).
 *
 * This hook is invoked by search_by_page_excerpt() to allow stemming
 * and other search preprocessing modules to find derived forms of
 * keywords to highlight, when creating a "snippet" for search results
 * display, if the given keyword is not found directly in the text.
 *
 * @param $key
 *    The keyword to find.
 * @param $text
 *    The text to search for the keyword.
 * @param $offset
 *    Offset position in $text to start searching at.
 * @param $boundary
 *    Text to include in a regular expression that will match a word boundary.
 * @return
 *    FALSE if no match is found. If a match is found, return an associative
 *    array with element 'where' giving the position of the match, and
 *    element 'keyword' giving the actual word found in the text at that
 *    position.
 */
function hook_sbp_excerpt_match($key, $text, $offset, $boundary) {
  // Find the root form of the keyword -- in this simple example,
  // all but the last 3 characters.
  $key = drupal_substr($key, 0, -3);
  if (drupal_strlen($key) < 3) {
    return FALSE;
  }

  // Look for this modified key at the start of a word.

  $match = array();
  if (!preg_match('/' . $boundary . $key . '/iu', $text, $match, PREG_OFFSET_CAPTURE, $offset)) {
    // didn't match our modified key.
    return FALSE;
  }

  // If we get here, we have a match. Find the end of the word we
  // actually matched, so it can be highlighted.
  $pos = $match[0][1];
  if (preg_match('/' . $boundary . '/iu', $text, $match, PREG_OFFSET_CAPTURE,
      $pos + drupal_strlen($key))) {
    $keyfound = drupal_substr($text, $pos, $match[0][1] - $pos);
  }

  return array('where' => $pos, 'keyword' => $keyfound);
}
