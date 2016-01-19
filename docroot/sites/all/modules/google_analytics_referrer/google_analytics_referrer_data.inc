<?php

/**
 * @file
 * Parsing and writing the fetched data.
 */

/**
 * Find how many distinct paths does Google Analytics have for this profile. This function is triggered by hook_cron().
 */
function google_analytics_referrer_update_path_counts() {

  // Record how long did this chunk take to process.
  $chunkprocessbegin = time();

  // Needing to stay under the Google Analytics API quota, let's count how many API retrievals were made in the last 24 hours.
  // @todo We should better take into consideration that the quota is reset at midnight PST (note: time() always returns UTC).
  $dayquota = variable_get('google_analytics_referrer_dayquota', array(0, 0));
  if (REQUEST_TIME - $dayquota[0] >= 86400) {
    // If last API request was more than a day ago, set monitoring time to now.
    $dayquota[0] = REQUEST_TIME;
    $dayquota[1] = 0;
    variable_set('google_analytics_referrer_dayquota', array($dayquota[0], $dayquota[1]));
  }
  // Are we over the GA API limit?
  $maxdailyrequests = variable_get('google_analytics_referrer_api_dayquota', 10000); // see http://code.google.com/apis/analytics/docs/gdata/gdataDeveloperGuide.html#quota
  if ($dayquota[1] > $maxdailyrequests) {
    watchdog(t('Google Analytics Referrer'), t('Google Analytics API quota of %maxdailyrequests requests has been reached. Will NOT fetch data from Google Analytics for the next %dayquota seconds. See <a href="/admin/config/system/google_analytics_referrer">the Google Analytics Referrer settings page</a> for more info.', array('%maxdailyrequests' => check_plain($maxdailyrequests), '%dayquota' => check_plain(($dayquota[0]+86400-REQUEST_TIME)))), NULL, WATCHDOG_ERROR);
    return;
  }

  // How many results to ask from GA in one request. Default on 1000 to fit most systems (for example those with no external cron).
  $chunk = variable_get('google_analytics_referrer_chunk_to_fetch', 1000);
  // In case there are more than $chunk path/counts to retrieve from GA, do just one chunk at a time and register that in $step.
  $step = variable_get('google_analytics_referrer_data_step', 0);
  // Which GA result to look for first. Must be between 1 - infinity.
  $pointer = $step*$chunk+1;

  // The earliest valid start-date for Google Analytics is 2005-01-01.
  $request = array(
    'dimensions' => array('ga:fullReferrer', 'ga:medium', 'ga:pagePath', 'ga:source'),
    'metrics' => array('ga:pageviews'),
    'start_date' => strtotime('2005-01-01'),
      'end_date' => strtotime('tomorrow'), // Using 'tomorrow' to offset any timezone shift between the hosting and Google servers.
      'start_index' => $pointer,
      'max_results' => $chunk,
      //'filters' => 'ga:pagePath==/node/3',
      //'sort' => array('-ga:pageviews'), // Get referrers sorted by how much they contribute to pageviews.
      );

  $resultcount = FALSE;
  $cachehere = array(
    'cid' => 'google_analytics_referrer_' . md5(serialize($request)),
    'expire' => google_analytics_referrer_cache_time(),
      'refresh' => FALSE,
      );
  $new_data = google_analytics_referrer_report_data($request, $cachehere);

  if (!$new_data->fromCache) { // Don't write anything to google_analytics_referrer if this GA data comes from cache (would be writing the same again).

    // This was a live request. Increase the GA request limit tracker.
    variable_set('google_analytics_referrer_dayquota', array($dayquota[0], ($dayquota[1]+1)));

    if (!empty($new_data->error)) { // If NULL then there is no error.
      watchdog(t('Google Analytics Referrer'), t('Problem fetching data from Google Analytics: %new_dataerror. Did you authenticate any Google Analytics profile? See <a href="/admin/config/system/google_analytics_referrer/authentication">here</a>.', array('%new_dataerror' => $new_data->error)), NULL, WATCHDOG_ERROR);
      // Nothing to do; return.
      //return;
    }
    else {
      $resultsretrieved = $new_data->results->rows;
      foreach ($resultsretrieved as $val) {

        // http://drupal.org/node/310085
        db_merge('google_analytics_referrer')
        ->key(array('hash' => md5($val['pagePath'].$val['fullReferrer'])))
        ->fields(array(
                'pagepath' => $val['pagePath'],
                'source' => $val['source'],
                'fullreferrer' => $val['fullReferrer'],
                'medium' => $val['medium'],
                'pageviews' => $val['pageviews'],
          ))
        ->execute();

      }
    }

  }

  // The total number of records for this profile.
  $resultcount = @$new_data->results->totalResults;
  // Store it in a variable.
  variable_set('google_analytics_referrer_totalpaths', $resultcount);

  // The total number of hits for all records for this profile.
  $totalhits = @$new_data->results->totalsForAllResults['pageviews'];
  variable_set('google_analytics_referrer_totalhits', $totalhits);

  // Set the pointer.
  $pointer += $chunk;

  watchdog(t('Google Analytics Referrer'), t('Retrieved %sizeof items from Google Analytics data for paths %first-%second.', array('%sizeof' => sizeof(@$new_data->results->rows), '%first' => ($pointer-$chunk), '%second' => ($pointer-$chunk-1+sizeof(@$new_data->results->rows)))), NULL, WATCHDOG_INFO);

  // OK now increase or zero $step
  if ($pointer <= $resultcount) {
    // If there are more results than what we've reached with this chunk, increase step to look further during the next run.
    $newstep = $step+1;
  }
  else {
    $newstep = 0;
  }
  variable_set('google_analytics_referrer_data_step', $newstep);

  // Record how long did this chunk take to process.
  variable_set('google_analytics_referrer_chunk_process_time', time()-$chunkprocessbegin);

}

/**
 * Calculate pageviews for one path (with any aliases).
 */
function google_analytics_referrer_get_sum_per_path($path, $removeprecise = '', $cacheon = TRUE) {

  // Depending on google_analytics_referrer_remove_paths we take either fullreferrer or source
  if ($removeprecise <> ''){
    $removepaths = $removeprecise;
  }
  else {
    $removepaths = variable_get('google_analytics_referrer_remove_paths', 'no');
  }
  if ($removepaths == 'yes'){
    $column = 'source';
  }
  else {
    $column = 'fullreferrer';
  }

  // Recognize special path 'all' to get the sum of all pageviews for the profile.
  if ($path == 'all') {

    // So in this case we simply need to select the most popular from all referrers.
    $cacheid = md5('all');
    if ($cache = cache_get('google_analytics_referrer_page_' . $cacheid) AND $cacheon) {
        $array_of_referrers = $cache->data;
    }
    else {
      // Get referrers contributing the most pageviews to these paths ... e.g. SELECT fullreferrer, SUM(pageviews) AS totalpageviews FROM google_analytics_referrer GROUP BY fullreferrer ORDER BY totalpageviews DESC

      $pathcounts = db_select('google_analytics_referrer', 'gar')
          ->fields('gar', array($column));
      $total = $pathcounts->addExpression('SUM(pageviews)', 'totalpageviews');
      //->condition('pagepath', array('/bg', '/node/3'), 'IN')
      $pathcounts = $pathcounts->groupBy($column)
          ->orderBy($total, 'DESC')
          ->execute();
      $array_of_referrers = array();
      foreach ($pathcounts as $pathcount) {
          $array_of_referrers[] = array($pathcount->$column, $pathcount->totalpageviews);
      }

      cache_set('google_analytics_referrer_page_' . $cacheid, $array_of_referrers, 'cache', CACHE_TEMPORARY);
    }

    return $array_of_referrers;
  }

  $path = check_plain($path); // Esp. in case function is called directly.

  // Remove initial slash, if any.
  if (substr($path, 0, 1) == '/') $path = substr($path, 1);

  // Get list of allowed languages to detect front pages such as http://mydomain.tld/en
  // Must come AFTER the possible initial slash is removed!
  $langs = language_list();
  $frontpages = array();
  foreach($langs as $lang=>$object){
    $frontpages[] = $lang;
  }
  $frontpages[] = '';
  $frontpages[] = '/';
  if (in_array($path, $frontpages)) {
    // This is the home page!
    $path = variable_get('site_frontpage', 'node');
  }

  //If it's a node we'll distinguish the language part of it, if any. Either format en/node/55 or node/55.
  $splitpath = explode('/', $path);
  $lang_prefix = '';
  if (
      (sizeof($splitpath) == 3 AND strlen($splitpath[0]) == 2 AND $splitpath[1] == 'node' AND is_numeric($splitpath[2]))
      OR
      (sizeof($splitpath) == 2 AND $splitpath[0] == 'node' AND is_numeric($splitpath[1]))
     ){
    if (sizeof($splitpath) == 3) {
      $nidhere = $splitpath[2];
    }
    else if (sizeof($splitpath) == 2) {
      $nidhere = $splitpath[1];
    }
    $dbresults = db_select('node', 'n')
      ->fields('n', array('nid','language'))
      ->condition('nid', $nidhere, '=')
      ->execute();
    foreach ($dbresults as $dbresult) {
      if ($dbresult->language <> 'und' AND $dbresult->language <> '') {
        $lang_prefix = $dbresult->language.'/';
        // If this is a language-prefixed node we need its path without the prefix for later.
        if (sizeof($splitpath) == 3){
          $path = $splitpath[1].'/'.$splitpath[2];
        }
      }
      break; // Is just 1 result anyway.
    }
  }

  //Now if it's a node but has a prefixed or unprefixed alias, e.g. en/my/path or my/path, we should also try to determine if it's a node and then count it's node/nid with it!
  if ($lang_prefix == ''){
    if (sizeof($splitpath) > 1 AND strlen($splitpath[0]) == 2 AND !is_numeric($splitpath[0])){ // E.g. en/view or nl/my/view or xx/view
      // Now we need to find which nid does it correspond (the language prefix + the alias)
      $withoutprefix = $splitpath;
      $lang = array_shift($withoutprefix);
      $withoutprefix = implode('/', $withoutprefix);
      $nodepath = drupal_lookup_path('source', $withoutprefix);
      if ($nodepath !== FALSE) {
        $path = $nodepath;
        $lang_prefix = $lang.'/';
      }
    }
  }

  //Now, it's also possible that it's a node alias but without prefix! E.g. my/path but in fact it's en/node/nid!
  if ($lang_prefix == ''){
    $nodepath = drupal_lookup_path('source', $path);
    if ($nodepath !== FALSE) {
      $path = $nodepath;
      $splitnodepath = explode('/', $nodepath);
      if (sizeof($splitnodepath) == 2 AND $splitnodepath[0] == 'node' AND is_numeric($splitnodepath[1])){
        $dbresults = db_select('node', 'n')
          ->fields('n', array('nid','language'))
          ->condition('nid', $splitnodepath[1], '=')
          ->execute();
        foreach ($dbresults as $dbresult) {
          if ($dbresult->language <> 'und' AND $dbresult->language <> '') {
            $lang_prefix = $dbresult->language.'/';
          }
          break; // Is just 1 result anyway.
        }
      }
    }
  }

  // All right, finally we can calculate the sum of pageviews. This process is cached.
  $cacheid = md5($lang_prefix.$path);
  if ($cache = cache_get('google_analytics_referrer_page_' . $cacheid) AND $cacheon) {
    $array_of_referrers = $cache->data;
  }
  else {
    // Get pageviews for this path and all its aliases.
    // NOTE: Here $path does NOT have an initial slash because it's coming from either check_plain($_GET['q']) (block) or from a tag like [gar|node/N].
    $unprefixedaliases = _google_analytics_referrer_path_aliases($path);
    $allpaths = array();
    foreach ($unprefixedaliases as $val) {
      // Google Analytics stores initial slash as well, so let's prefix them.
      $allpaths[] = '/'.$lang_prefix.$val; // With language prefix, if available, e.g. /en/node/55
      if ($lang_prefix <> ''){
        // Now, if we are counting NODE with language prefix, we also need to count the pageviews for that node without the prefix — it could be that before it had no language prefix but it still was the same node!
        // BUT this will not work for non-nodes, e.g. views. There we depend on the path e.g. /en/myview because it would be tricky to get a valid language prefix out of the path. E.g. /en/myview could be a path of a view where "en" does not mean the English language. In other words, while prefix before node/id does not change the page (it's the same node), with views or other custom pages the prefix may actually contain completely different content.
        $allpaths[] = '/'.$val;
        // @TODO ... obviously, here we should treat the possibility of the NODE/nid having a different language prefix. A niche case (how often do existing NODES change language?)
      }
    }

    // Find possible redirects for this path using redirect_load_multiple() from module Redirect http://drupal.org/project/redirect
    if (function_exists('redirect_load_multiple')){
      $redirectobjects = redirect_load_multiple(FALSE, array('redirect' => $path));
      foreach($redirectobjects as $redirectobject){
        $allpaths[] = '/'.$redirectobject->source;
        $allpaths[] = '/'.$redirectobject->language.'/'.$redirectobject->source;
      }
    }

    // Get referrers, for each of the path aliases, contributing the most pageviews to these paths ... e.g. SELECT fullreferrer, SUM(pageviews) AS totalpageviews FROM google_analytics_referrer WHERE pagepath IN ("/bg", "/node/3") GROUP BY fullreferrer ORDER BY totalpageviews DESC
    $pathcounts = db_select('google_analytics_referrer', 'gar')
    ->fields('gar', array($column));
    $total = $pathcounts->addExpression('SUM(pageviews)', 'totalpageviews');
    //->condition('pagepath', array('/bg', '/node/3'), 'IN')
    $pathcounts = $pathcounts->condition('pagepath', $allpaths, 'IN')
    ->groupBy($column)
    ->orderBy($total, 'DESC')
    ->execute();
    $array_of_referrers = array();
    foreach ($pathcounts as $pathcount) {
      $array_of_referrers[] = array($pathcount->$column, $pathcount->totalpageviews);
    }

    cache_set('google_analytics_referrer_page_' . $cacheid, $array_of_referrers, 'cache', CACHE_TEMPORARY);
  }

  return $array_of_referrers;
}

/**
 * Return a list of paths that are aliased with the given path (including the given path).
 */
function _google_analytics_referrer_path_aliases($node_path) {

  // Get the normal node path if it is a node.
  $node_path = drupal_get_normal_path($node_path);

  // Grab all aliases.
  $aliases = array($node_path);
  $result = db_query("SELECT * FROM {url_alias} WHERE source = :source", array(':source' => $node_path));
  foreach ($result as $row) {
    $aliases[] = $row->alias;
  }

  // If this is the front page, add the base path too, and index.php for good measure. There may be other ways that the user is accessing the front page but we can't account for them all.
  if ($node_path == variable_get('site_frontpage', 'node')) {
    $aliases[] = '';
    $aliases[] = '/';
    $aliases[] = 'index.php';
  }

  return $aliases;
}

/**
 * Request report data.
 *
 * @param $params
 *   An associative array containing:
 *   - profile_id: required [default=variable_get('google_analytics_referrer_profile_id')]
 *   - metrics: required.
 *   - dimensions: optional [default=none]
 *   - sort_metric: optional [default=none]
 *   - filters: optional [default=none]
 *   - segment: optional [default=none]
 *   - start_date: optional [default=GA release date]
 *   - end_date: optional [default=today]
 *   - start_index: optional [default=1]
 *   - max_results: optional [default=10,000]
 * @param $cache_options
 *   An optional associative array containing:
 *   - cid: optional [default=md5 hash]
 *   - expire: optional [default=CACHE_TEMPORARY]
 *   - refresh: optional [default=FALSE]
 */
function google_analytics_referrer_report_data($params = array(), $cache_options = array()) {

  $params_defaults = array(
    'profile_id' => 'ga:' . variable_get('google_analytics_referrer_profile_id', 0),
    );

  $params += $params_defaults;

  $GARFeed = google_analytics_referrer_new_gafeed();
  $GARFeed->queryReportFeed($params, $cache_options);

  return $GARFeed;
}
