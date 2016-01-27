<?php
/**
 * @file
 * This file contains all the adminstration logic
 * for this module.
 * In effect, this file contains all the module logic.
 */

/**
 * if the API key is set, then we show the list page
 * otherwise, we show the "set api key" form.
 */
function google301_admin() {
  if (variable_get('google301_key')) {
    return google301_list();
  }
  drupal_goto('admin/config/search/redirect/google/set_key');
}

/**
 * The "set api key" form.
 */
function google301_key() {

  $form['key'] = array(
    'pretext' => array(
      '#markup' => t( '<img src="http://media.techworld.com/cmsdata/news/3222178/Google_Code_logo_thumb160.png" /><br/>'
                    . 'In order to search Google for inbound links, it requires a Custom Search API key. You can'
                    . ' get a key by going to <a href="@link" target="_blank">@link</a>'
                    . ', turning on "Custom Search API", and then copying the API key from the "API Access" page.<br/>&nbsp;'
                    , array('@link' => 'https://code.google.com/apis/console/', )
                  ),
    ),
    'api_key' => array(
      '#type' => 'textfield',
      '#title' => t('Google Custom Search API Key'),
      '#default_value' => variable_get('google301_key', ''),
      '#name' => 'api_key',
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => 'Save key',
    ),
  );
  return $form;
}
function google301_key_submit($form, $form_state) {
  if (!empty($form_state['values']['api_key'])) {
    variable_set('google301_key', $form_state['values']['api_key']);
  }
  drupal_goto('admin/config/search/redirect/google');
}
function google_301_key_validate($form, $form_state) {
  $key = '';
  if (!empty($form_state['values']['api_key'])) {
    $key = $form_state['values']['api_key'];
  }

  # easy length checking.
  # seems like API keys are 39 chars long, /iA-Z0-9/i
  # however, doing strict checking will likely cause errors one day
  if(strlen($key < 30)) {
    return form_set_error('api_key', 'Please enter an API key');
  }
}

/**
 * Creates an entry in the "redirect" table via the redirect_save method.
 * Redirects back to list page.
 */
function google301_create() {
  $path = isset($_GET['path']) ? $_GET['path'] : false;

  # if path is set and the URL is invalid
  if ($path && !(drupal_lookup_path('alias', $path) || drupal_lookup_path('source', $path) || google301_pathauto_lookup($path))) {
    $redir = array(
      'type' => 'redirect',
      'source' => $_GET['path'],
      'redirect' => '<front>',
      'language' => 'und',
      'status_code' => '301',
      'source_options' => array(),
      'redirect_options' => array()
    );
    redirect_save((object) $redir);

    # alter the cache to reflect this addition.
    $search_path = url('<front>', array('absolute' => TRUE, ));
    $cache_name = "google301:" . preg_replace('/[^a-z]/', '', $search_path);
    $cache = cache_get($cache_name, 'cache')->data;

    foreach ($cache as $k=>$row) {
      if ($row->path == $path) {
        $row->exists = true;
      }
    }

    cache_set($cache_name, $cache, 'cache', CACHE_TEMPORARY);

    # set a nice message
    drupal_set_message(t('Redirect created for @path with status code 301', array('@path' => $path, )));
  }
  # redir to main page
  drupal_goto('admin/config/search/redirect/google');
}

/**
 * Shows a list of results from google (cached for at least one hour)
 * and wether they actually exist on the site.
 */
function google301_list() {
  drupal_set_title('Google 301 Checker');

  $search_path = url('<front>', array('absolute' => TRUE, ));
  $cache_name = "google301:" . preg_replace('/[^a-z]/', '', $search_path);

  if ($results = cache_get($cache_name, 'cache')) {
    $results = $results->data;
  }
  else {
    $results = google301_rest($search_path);

    if (count($results) > 10) {
      cache_set($cache_name, $results, 'cache', CACHE_TEMPORARY);
    }
  }

  $header = array(
    'title' => array('data' => t('Title')),
    'path' => array('data' => t('Path')),
    'exists' => array('data' => t('Exists')),
    'operations' => array('data' => t('Operations')),
  );

  $rows = array();
  foreach ($results as $i => $item) {
    $row = array();
    $row['title'] = l($item->title, $item->href);
    $row['path'] = l($item->path, $item->href);
    $row['exists'] = '<span style="color: ' . ($item->exists ? 'green' : 'red') . '">' . ($item->exists ? 'yes' : 'no') . '</span>';
    $row['operations'] = $item->exists ?  '' : l(t('Create 301 redirect'), 'admin/config/search/redirect/google/create', array('query' => array('path' => $item->path), ));

    $rows[$i + 1] = $row;
  }

  $form['pretext'] = array(
    '#markup' => t('This page will show the first 100 results for this domain, and whether they have a corresponding URL on this site. If you want to change '
                 . 'your API key, click <a href="@link">here</a>.', array('@link' => url('admin/config/search/redirect/google/set_key')))
                 . '<br/><br/>',
  );

  $form['urls_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No data retrieved.'),
  );
  $form['urls_pager'] = array(
    '#theme' => 'pager'
  );
  return $form;
}

/**
 * Gets the top 100 results from Google in a big lump
 * based on the searched URL.
 *
 * @param $url
 *    The URL to search on.
 *
 * @param $page = 0
 *    The page offset from which to search.
 *
 * @param $key
 *    The API key to use, passed through recursion to reduce DB requests.
 */
function google301_rest($url, $page = 0, $key = FALSE) {
  // remove http(s), www, everything after the / (get the base domain, basically)
  preg_match('/(https?:\/\/)?(www\.)?([^\/]+).*/i', $url, $url);
  $url = $url[3];

  if (!$key) {
    $key = variable_get('google301_key');
  }

  $params = array(
    // search engine source
    'cx' => '013036536707430787589:_pqjad5hr1a',
    // api key
    'key' => $key,
    // query
    'q' => 'site:' . $url,
    // result offset
    'start' => ($page * 10) + 1,
    // fields - minimise data transfer and thus time
    'fields' => 'queries/nextPage,items(htmlTitle,link)',
  );

  // build the URL
  $gurl = 'https://www.googleapis.com/customsearch/v1?' . http_build_query($params);

  $result = drupal_http_request($gurl);

  # Request fail
  if (!isset($result->data)) {
    die('Request error: ' . $result->error);
  }

  # JSON fail
  if (!$json = json_decode($result->data)) {
    $errors = array(
      JSON_ERROR_DEPTH => 'Maximum stack depth exceeeded',
      JSON_ERROR_STATE_MISMATCH => 'Underflow or the modes mismatched',
      JSON_ERROR_CTRL_CHAR => 'Unexpected control character found',
      JSON_ERROR_SYNTAX => 'Syntax error, malformed JSON',
      JSON_ERROR_UTF8 => 'Malformed UTF-8 characters, possibly incorrectly encoded',
    );
    die('JSON error: ' . $errors[json_last_error()]);
  }

  # API fail
  if (isset($json->error)) {
    $o = new stdClass;
    $o->title = $json->error->message;
    $o->path  = $json->error->code;
    $o->href  = FALSE;
    $o->exists = FALSE;
    return array($o);
  }

  # Results fail
  $items = array();
  if (!isset($json->items)) {
    return $items;
  }

  # success!
  foreach ($json->items as $link) {
    // ignore any bad results (those which don't include a trailing part)
    $match = preg_match('/^(http:\/\/.+\.(com|uk|net|org))\/(.+)/', $link->link, $out);
    if (!$match) continue;

    $item = new stdClass;
    $item->title = $link->htmlTitle;
    $item->href = $link->link;
    $item->path = preg_replace('/(^[^a-z0-9]+|[^a-z0-9]+$)/', '', $out[3]); # trim all non a-z0-9
    $item->exists = (drupal_lookup_path('alias', $item->path) || drupal_lookup_path('source', $item->path) || google301_pathauto_lookup($item->path));

    $items[] = $item;
  }

  # if there are more results, get them and append them before returning
  if (isset($json->queries->nextPage)) {
    foreach (google301_rest($url, $page + 1, $key) as $k => $v) {
      $items[] = $v;
    }
  }

  return $items;
}

/**
 * Searches the url_alias table for a match on $path
 *
 * @param $path
 *    The internal path to check
 *
 * @return boolean
 *    TRUE if the path exists in url_alias or redirect,
 *    FALSE otherwise.
 */
function google301_pathauto_lookup($path) {
  if (db_select('url_alias', 'ua')
    ->fields('ua', array('pid'))
    ->condition('alias', $path)
    ->execute()->rowCount()) {
    return TRUE;
  }

  return (boolean) db_select('redirect', 'r')
    ->fields('r', array('rid'))
    ->condition('source', $path)
    ->execute()->rowCount();
}