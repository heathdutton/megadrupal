<?php
/**
 * @file
 * This whole file is the implementation of a new search workflow instead of
 * Drupal's default search workflow. The reason to do that is, that those is
 * not fit to XC expectations, and now we can freely modify the original
 * structure without applying patches or other workaround.
 * The initial state of the process is the modified search module.
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */


function search_get_keys() {
  static $return;
  if (!isset($return)) {
    // Extract keys as remainder of path
    // Note: support old GET format of searches for existing links.
    $path = explode('/', $_GET['q'], 3);
    $keys = empty($_REQUEST['keys']) ? '' : $_REQUEST['keys'];
    $return = count($path) == 3 ? $path[2] : $keys;
  }
  return $return;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_view($type = 'xc_search') {
  if (module_exists('devel')) {
    $init = timer_read('page');
    $GLOBALS['timers']['before_main_content'] = array(
      'counter' => 1,
      'time' => $init,
    );
  }
  timer_start('xc_search_view');
  // Search form submits with POST but redirects to GET. This way we can keep
  // the search query URL clean as a whistle:
  // search/type/keyword+keyword

  $output = '';
  if (!isset($_POST['form_id'])) {
    if ($type == '') {
      // Note: search/node can not be a default tab because it would take on the
      // path of its parent (search). It would prevent remembering keywords when
      // switching tabs. This is why we drupal_goto to it from the parent instead.
      drupal_goto('search/node');
    }

    $keys = search_get_keys();
    // Only perform search if there is non-whitespace search term:
    $results = '';
    //if (trim($keys) == '') {
//  $keys = '*:*';
    //}
    $keys = str_replace('/undefined', '/', $keys);
    if (trim($keys)) {
      // Log the search keys:
      $meta_ = (object) array();
      $meta = &$meta_;
      xc_log_info('search', '%keys (@type).', array('%keys' => $keys, '@type' => $type), // module_invoke($type, 'search', 'name', $meta)),
        WATCHDOG_NOTICE,
        l(t('results'), 'search/' . $type . '/' . $keys)
      );

      // Collect the search results:
      // we use the Drupal search hook here, to keep as much compatibility as we can
      // it simply provides a wrapper for hook_search and hook_search_page
      timer_start('xc_search_view/xc_search_data');
      $results = xc_search_data($keys, $type, $meta);
      timer_stop('xc_search_view/xc_search_data');

      global $_xc_search_no_results;
      $_xc_search_no_results = TRUE;
      if ($results) {
        $_xc_search_no_results = FALSE;
        timer_start('xc_search_view/theme_box');
        // theme('box', NULL, $results);
        $results = '<div>' . $results . '</div>';
        timer_stop('xc_search_view/theme_box');
      }
      else {
        timer_start('xc_search_view/no_results');
        timer_start('xc_search_view/xc_search_suggestions');
        $suggestions = xc_search_suggestions($keys);
        timer_stop('xc_search_view/xc_search_suggestions');

        // Correction of search. If no results, we try the first suggestion came from the suggester
        if (!isset($_GET['no_corr']) || $_GET['no_corr'] != 1) {
          if (!empty($suggestions) && !empty($suggestions[0])) {
            $meta->correction = (object) array(
              'original' => $keys,
              'suggested' => array($suggestions[0], $suggestions[1], $suggestions[2]),
            );
            $results = xc_search_data($suggestions[0], $type, $meta);
            if ($results) {
              // $results = theme('box', NULL, $results);
              $_xc_search_no_results = FALSE;
            }
          }
        }

        if ($_xc_search_no_results) {
          $error = xc_search_error();
          if (!is_null($error)) {
            drupal_set_message(filter_xss($error), 'error');
          }
          $results = theme('xc_search_no_results', array('title' => t('Sorry, no results for: %keys', array('%keys' => $keys)), 'content' => ''
            // search_help('search#noresults', drupal_help_arg())
          ));
        }
        timer_stop('xc_search_view/no_results');
      }
    }

    drupal_add_js(drupal_get_path('module', 'xc_search') . '/js/xc_search.js');
    $output = $results;
  }
  else {
    if ($_POST['form_id'] != 'xc_search_form') {
      if ($_POST['form_id'] == 'xc_search_sortform') {
        drupal_get_form('xc_search_sortform', 'top');
      }
      else {
        // TODO  needs to have $form as its first parameter.
        drupal_get_form($_POST['form_id']);
      }
    }
  }

  timer_stop('xc_search_view');
  timer_start('after_main_content');

  return $output;
}

/**
 * Perform a standard search on the given keys, and return the formatted results.
 * This function is a modified version of search.module's search_data()
 *
 * @param $keys (String)
 *   Keyword query to search on.
 * @param $type (String)
 *   Search module to search.
 * @param $meta (Object)
 *   Reference object containing the following fields:
 *   - total (int): the total number or results
 *   - first (int): the first result to show
 *   - last (int): the last result to show
 *   - rows (int): the number of rows to display
 *   It will be fulfilled by the hook_search function, and used by the theme
 *
 * @return (String)
 *   The themed search results
 */
function xc_search_data($keys = NULL, $type = 'xc_search', &$meta = FALSE) {
  timer_start('xc_search_data');
  $output = '';
  if (isset($keys)) {
    $results = xc_search_search('search', $keys, $meta);
    if (isset($results) && is_array($results) && count($results)) {
      timer_start('xc_search_data/xc_search_search_page');
      xc_search_meta($meta);
      $output = theme('xc_search_search_results', array('results' => $results, 'meta' => $meta));
      timer_stop('xc_search_data/xc_search_search_page');
    }
  }
  timer_stop('xc_search_data');
  return $output;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_form($form, &$form_state, $action = '', $keys = '', $type = NULL, $prompt = NULL) {
  // isset($form_state['input']['online_only']) ? $form_state['input']['online_only']

  // add JS if we are on the <front>
  if (drupal_is_front_page()) {
    drupal_add_js(drupal_get_path('module', 'xc_search') . '/js/xc_search.js');
  }

  $online_only = isset($_GET['online_only']) ? $_GET['online_only'] : 0;

  $exclude_microform = isset($form_state['input']['exclude_microform'])
    ? $form_state['input']['exclude_microform']
    : (isset($_GET['exclude_microform'])
      ? $_GET['exclude_microform']
      : 1);
  $rows = xc_search_get_rows();
  $sort = isset($_GET['sort']) ? $_GET['sort'] : '';
  $is_query_link = (!empty($_GET['ql']) && $_GET['ql'] == 1) ? TRUE : FALSE;

  // set search term
  $original_state = isset($_GET['os']) ? $_GET['os'] : array();
  if (($keys == '*' || $keys == '') && empty($original_state) && isset($_GET['filter'])) {
    $key_default_value = '';
  }
  elseif (isset($original_state['keys'])) {
    $key_default_value = $original_state['keys'];
  }
  else {
    $key_default_value = $keys;
  }

  $hidden_filters = array();
  if (isset($form_state['input']['hidden_filters'])) {
    $hidden_filters = unserialize($form_state['input']['hidden_filters']);
  }

  if (isset($_GET['caller']) && $_GET['caller'] == 'xc-browse') {
    $browse_page = parse_url($_SERVER['HTTP_REFERER'], PHP_URL_QUERY);
    parse_str($browse_page, $browse_params);
    $hidden_filters = $browse_params['filter'];
    $key_default_value = '';
  }

  // set action
  if (!$action) {
    $action = url('xc/search');
  }

  $search_ui = xc_search_variable('search_ui');
  if (is_null($search_ui)) {
    $search_ui = xc_search_ui_get();
  }
  $method = isset($search_ui->use_post_method) && $search_ui->use_post_method == 0 ? 'GET' : 'POST';

  drupal_add_js(
    array(
      'xc_search' => array(
        'search_form_method' => $method,
        'clean_url' => variable_get('clean_url', FALSE),
        'clean_action' => 'xc/search',
      )
    ),
    array(
      'type' => 'setting',
      'scope' => JS_DEFAULT
    )
  );

  $form = array(
    '#action' => $action,
    '#attributes' => array('class' => 'search-form'),
    '#theme' => 'xc_search_form',
    '#method' => $method,
  );

  $form['module'] = array(
    '#type' => 'value',
    '#value' => $type,
  );

  $form['basic'] = array(
    '#type' => 'item',
    '#title' => $prompt,
  );

  $form['basic']['keys'] = array(
    '#type' => 'textfield',
    '#default_value' => $key_default_value,
    '#size' => FALSE,
    '#autocomplete_path' => 'xc_search/autocomplete',
    '#maxlength' => 255,
  );

  $form['basic']['clear'] = array(
    '#type' => 'item',
    '#markup' => l(t('X'), 'xc/search', array(
      'html' => TRUE,
      'attributes' => array(
        'title' => t('Clear the form'),
        'id' => 'clear-form-link',
      ),
    )),
  );

  $form['basic']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );

  $form['online_only'] = array(
    '#type' => 'hidden',
    '#value' => $online_only,
  );
  $form['exclude_microform'] = array(
    '#type' => 'hidden',
    '#value' => $exclude_microform,
  );

  $form['hidden_filters'] = array(
    '#type' => 'hidden',
    '#value' => serialize($hidden_filters),
  );

  // processed_keys is used to coordinate keyword passing between other forms
  // that hook into the basic search form.

  $form['basic']['processed_keys'] = array(
    '#type' => 'value',
    '#value' => array(),
  );
  $form['basic']['original_state'] = array(
    '#type' => 'value',
    '#value' => array(),
  );

  /*
   $form['basic']['is_query_link'] = array(
   '#type' => 'value',
   '#value' => $is_query_link
   );

   if ($is_query_link) {
   $form['basic']['keys']['#disabled'] = TRUE;
   $form['basic']['keys']['#attributes'] = array('style' => 'display: none;');
   $form['basic']['submit']['#value'] = t('Start new search');
   $form['basic']['hidden_keys'] = array(
   '#type' => 'hidden',
   '#value' => $keys,
   );
   }
   */

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_xc_search_form($variables) {
  $form = $variables['form'];
  drupal_add_css(drupal_get_path('module', 'xc_search') . '/xc_search.css');

  foreach (array('keys', 'clear', 'submit') as $key) { // 'sort', 'rows'
    $rendered[$key] = drupal_render($form['basic'][$key]);
  }

  return '<div id="xc-search-keys">' . $rendered['keys'] . '</div>'
       . '<div id="xc-search-clear">' . $rendered['clear'] . '</div>'
       . '<div id="xc-search-hits">' . $rendered['submit'] . '</div>'
//       . '<div id="xc-search-info">'
//       . '<div id="xc-search-sort">' . $rendered['sort'] . '</div>'
//       . '<div id="xc-search-rows">' . $rendered['rows'] . '</div>'
//       . '</div>'
       . drupal_render_children($form)
       . '<div style="clear:both"></div>';
}

