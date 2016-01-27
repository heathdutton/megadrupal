<?php

/**
 * @file
 * Non-admin page callbacks.
 */


/**
 * Callback function from hook_menu().
 * The URL will pass in parameters. The 1st should be the ISBN or ASIN. The 2nd should be whether it's an ISBN or ASIN.
 * The 3rd parameter is optional and will indicate if the details being asked for are from an eBooks list
 */
function booklists_detail($type, $code, $delta = NULL, $ebooks = NULL) {
  // Redirect to PAC details page if possible.
  if (module_exists('pac') && $type == 'ISBN') {
    $result = PolarisAPI::searchBibs('q=' . $code, 'keyword/ISBN'); // Get bib number for Polaris.
    $bib_no = $result->BibSearchRows[0]->ControlNumber;
    if (!empty($bib_no)) {
      drupal_goto('search/detail/' . $bib_no);
    }
  }
  else if (module_exists('pac') && $type == 'ASIN') {
    $upc = booklists_get_amazon_upc($code);
    $result = PolarisAPI::searchBibs('q=UPC=' . $upc, 'boolean'); // Try UPC first.
    $bib_no = $result->BibSearchRows[0]->ControlNumber;
    if (!empty($bib_no)) {
      drupal_goto('search/detail/' . $bib_no);
    }
    else {
      $ean = booklists_get_amazon_ean($code);
      $result = PolarisAPI::searchBibs('q=' . $ean, 'keyword/ISBN'); // Try EAN 2nd.
      $bib_no = $result->BibSearchRows[0]->ControlNumber;
      if (!empty($bib_no)) {
        drupal_goto('search/detail/' . $bib_no);
      }
      else {
        $isbn = booklists_get_amazon_isbn($code);
        $result = PolarisAPI::searchBibs('q=' . $isbn, 'keyword/ISBN'); // Try standard ISBN last.
        $bib_no = $result->BibSearchRows[0]->ControlNumber;
        if (!empty($bib_no)) {
          drupal_goto('search/detail/' . $bib_no);
        }
      }
    }
  }
  $item = array('type' => $type, 'code' => $code, 'delta' => $delta, 'ebooks' => $ebooks);
  $content = array();
  $content[] = array(
    '#type' => 'markup',
    '#markup' => theme('booklists_detail', $item),
  );
  return $content;
}

/**
 * Function to query Polaris via ODBC connection.
 *
 * @param str $list
 *   Which list is being displayed (coming soon, new arrivals, etc.)
 * @param bool $results_only
 *   Whether or not to show
 * @param int $results_per_page
 *   The number of results per page.
 * @param int $page
 *   The page number to retrieve results for.  Defaults to the first page.
 */
function booklists_query_polaris($list, $results_only = FALSE, $results_per_page = 20, $page = 1) {
  // Sets the TDS Version environment variable.
  putenv('TDSVER=7.0');
  if (!empty($conf['polaris_mssql_config'])) {
    $mssql_config = $conf['polaris_mssql_config'];
  }
  else {
    $booklists_polaris_odbc_host = variable_get('booklists_polaris_odbc_host');
    $booklists_polaris_odbc_dbname = variable_get('booklists_polaris_odbc_dbname');
    $booklists_polaris_odbc_username = variable_get('booklists_polaris_odbc_username');
    $booklists_polaris_odbc_password = variable_get('booklists_polaris_odbc_password');
    $mssql_config = array(
      'user' => $booklists_polaris_odbc_username,
      'password' => $booklists_polaris_odbc_password,
      'dsn' => 'dblib:host=' . $booklists_polaris_odbc_host . ';dbname=' . $booklists_polaris_odbc_dbname,
    );
  }
  $query_params = drupal_get_query_parameters();
  // We need to treat them as three groups in the queries... a) format b) audience and c) fiction/non-fiction. If more than one item is selected within one of the groups, those items should be OR'ed together (e.g. DVDs OR Large Print, Teen OR Children, Fiction OR Non-fiction). If items are selected from 2 or 3 groups, then they should be AND'ed together (e.g. (DVDs OR Audiobooks on CD) AND (Teen OR Children) AND (Fiction OR Non-fiction)).
  $limiters = '';
  $audience_limiters = $format_limiters = $fiction_limiters = array(); // Set up some arrays for use below.
  if (isset($query_params['list_type'])) {
    foreach ($query_params['list_type'] as $list_type) {
      // Modify the query based on specific list type selected by user.
      switch ($list_type) {
        case 'movies': // DVD & blu-ray
          //$limiters = 'AND cir.AssignedCollectionID in (5,12,15,24,53,74,104,106)';
          $format_limiters[] = 'br.PrimaryMARCTOMID in (17,18,33,34,40)';
          break;
        case 'dvds':
          //$limiters = 'AND cir.AssignedCollectionID in (15,24,53,74,104,106)';
          $format_limiters[] = 'br.PrimaryMARCTOMID in (33)';
          break;
        case 'blu_ray':
          //$limiters = 'AND cir.AssignedCollectionID in (5,12,104,106)';
          $format_limiters[] = 'br.PrimaryMARCTOMID in (40)';
          break;
        case 'books': // LP & Regular
          $format_limiters[] = 'br.PrimaryMARCTOMID in (1,27,42,43)';
          break;
        case 'large_print':
          //$limiters = 'AND cir.AssignedCollectionID in (31,32,33,34,35,36,94)';
          $format_limiters[] = 'br.PrimaryMARCTOMID in (27)';
          break;
        case 'regular_print':
          $format_limiters[] = 'br.PrimaryMARCTOMID in (1)'; // Book only. Not large print (27).
          break;
        case 'audiobooks':
          $format_limiters[] = 'br.PrimaryMARCTOMID in (22,37,41)';
          break;
        case 'audiobooks_cd':
          $format_limiters[] = 'br.PrimaryMARCTOMID in (22,37)';
          break;
        case 'audiobooks_downloadable':
          $format_limiters[] = 'br.PrimaryMARCTOMID in (41)';
          break;
        case 'audiobooks_playaway':
          $format_limiters[] = 'MARCMedium = \'PLAYAWAY\'';
          break;
        case 'ebooks':
          $format_limiters[] = 'br.PrimaryMARCTOMID in (36)';
          break;
        case 'music':
          $format_limiters[] = 'br.PrimaryMARCTOMID in (5,21,35)';
          break;
        case 'videogames':
          $format_limiters[] = 'MARCMedium = \'VIDEOGAME\'';
          break;
        case 'fiction':
          $fiction_limiters[] = 'br.LiteraryForm in (\'1\',\'n\',\'j\',\'p\',\'m\')';
          // 1 = fiction, n = novels, j = short stories, p = poetry, m = mixed forms
          break;
        case 'nonfiction':
          $fiction_limiters[] = 'br.LiteraryForm in (\'0\',\'d\',\'e\',\'h\',\'s\',\'i\')';
          // 0 = nonfiction, d = dramas, e = essays, h = humor satires etc., s = speeches, i = letters
          break;
        case 'childrens':
          $audience_limiters[] = 'cir.AssignedCollectionID in (3,6,11,12,13,14,15,16,17,18,19,20,21,22,23,37,38,52,53,54,67,69,70,71,72,73,98,105,106,107,108,109)';
          break;
        case 'teens':
          $audience_limiters[] = 'cir.AssignedCollectionID in (28,42,43,49,79,80,81,82,83,84,85,86,97,103)';
          break;
        case 'adults':
          $audience_limiters[] = 'cir.AssignedCollectionID NOT IN (3,6,11,12,13,14,15,16,17,18,19,20,21,22,23,37,38,52,53,54,67,69,70,71,72,73,105,106,28,42,43,49,79,80,81,82,83,84,85,86,97,98,103,105,106,107,108,109)';
          break;
        default:
          break;
      }
    }
  }
  // Concatenate all of the limiters together to where they make sense in the query and are logically grouped with
  // "AND"s and "OR"s as specified in the comments above.
  $limiters .= booklists_limiters($audience_limiters);
  $limiters .= booklists_limiters($format_limiters);
  $limiters .= booklists_limiters($fiction_limiters);
  
  $availability = '';
  // Modify the result set if the user wants to show only available items.
  if (isset($query_params['available_only'])) {
    $availability = 'AND cir.ItemStatusID in (1)';
  }
  try {
    switch ($list) {
      case 'coming_soon':
        // Based on stored procedure "DASH_OnOrderItems".    
        $sql = "SELECT DISTINCT TOP 100
        br.BibliographicRecordID,
        br.FirstAvailableDate,
        br.PrimaryMARCTOMID,
        br.BrowseAuthor,
        br.BrowseTitle,
        MAX(cir.ItemStatusDate) as ItemStatusDate,
        br.LifetimeCircCount
        FROM Polaris.Polaris.BibliographicRecords br WITH (NOLOCK) 
        INNER JOIN Polaris.Polaris.CircItemRecords cir WITH (NOLOCK)
        ON br.bibliographicrecordid = cir.AssociatedBibRecordID
        WHERE cir.ItemStatusID = 13
        AND br.DisplayInPAC = 1
        AND br.LifetimeCircCount = 0
        " . $limiters . "
        " . $availability . "
        GROUP BY br.bibliographicrecordid, br.browsetitle, br.browseauthor, br.FirstAvailableDate, br.PrimaryMARCTOMID, br.LifetimeCircCount
        ORDER BY ItemStatusDate DESC";
        /*
        OFFSET " . $offset . " ROWS
        FETCH NEXT " . $results_per_page . " ROWS ONLY*/
        break;
      case 'just_checked_in':
        // Custom. No dashboard exists for just checked in.
        $sql = "SELECT DISTINCT TOP 100
        cir.AssociatedBibRecordID AS BibliographicRecordID,
        MAX(cir.LastCircTransactionDate) AS LastCircTransactionDate,
        cir.MaterialTypeID
        FROM Polaris.Polaris.BibliographicRecords br WITH (NOLOCK) 
        INNER JOIN Polaris.Polaris.CircItemRecords cir WITH (NOLOCK)
        ON br.bibliographicrecordid = cir.AssociatedBibRecordID
        WHERE cir.ItemStatusID = 1
        AND cir.DisplayInPAC = 1
        " . $limiters . "
        " . $availability . "
        AND cir.LastCircTransactionDate > getdate() - 14
        GROUP BY cir.AssociatedBibRecordID, cir.MaterialTypeID
        ORDER BY LastCircTransactionDate DESC";
        break;
      case 'most_popular':
        // Based on stored procedure "DASH_MostCircedTitles".
        $sql = "SELECT DISTINCT TOP 100
        br.BibliographicRecordID,
        br.FirstAvailableDate,
        br.PrimaryMARCTOMID,
        br.BrowseAuthor,
        br.BrowseTitle,
        d.PeriodicCircCount
        FROM Polaris.Polaris.BibDashboardStatistics d WITH (NOLOCK)
        INNER JOIN Polaris.Polaris.BibliographicRecords br WITH (NOLOCK)
        ON br.BibliographicRecordID = d.BibliographicRecordID
        INNER JOIN Polaris.Polaris.CircItemRecords cir WITH (NOLOCK)
        ON br.bibliographicrecordid = cir.AssociatedBibRecordID
        WHERE br.DisplayInPAC = 1
        AND br.BrowseTitle NOT LIKE '%JUVENILE PAPERBACK%'
        " . $limiters . "
        " . $availability . "
        ORDER BY d.PeriodicCircCount DESC";
        break;
      case 'new_arrivals':
        // Based on stored procedure "DASH_NewReleases".
        $sql = "SELECT DISTINCT TOP 100
          br.BibliographicRecordID,
          br.FirstAvailableDate,
          br.PrimaryMARCTOMID,
          br.BrowseAuthor,
          br.BrowseTitle
        FROM
          Polaris.Polaris.BibliographicRecords br with (nolock),
          Polaris.Polaris.CircItemRecords cir WITH (NOLOCK)
        WHERE
          br.FirstAvailableDate > getdate() - 14
        AND cir.AssociatedBibRecordID = br.BibliographicRecordID
        AND cir.DisplayInPAC = 1
        AND cir.ItemStatusID in (1,2,4,5,6)
        " . $limiters . "
        " . $availability . "
        ORDER BY br.FirstAvailableDate DESC";
        break;
    }

    /**
     * Try to pull results from 24 hour cache first. If not available, create a PDO connection to SQL Server.
     */
    $cid = md5($sql);
    $cached = cache_get($cid, 'cache');
    if ($cached && time() < $cached->expire && empty($availability)) {
      $bibs = $cached->data;
    }
    else {
      // Not in cache. Let's run the query.
      $conn = new PDO($mssql_config['dsn'], $mssql_config['user'], $mssql_config['password']);
      $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
      // Prepare the SQL query and execute.
      try {
        $q = $conn->prepare($sql);
        $q->execute();
        // Grab the results.
        $bibs = $q->fetchAll(PDO::FETCH_ASSOC);
        if (empty($availability)) {
          cache_set($cid, $bibs, 'cache', time() + 86400); // Cache the query result for 24 hours as long as it doesn't contain availability info. Those need to be done dynamically.
        }
      }
      catch (Exception $e) {
        watchdog('Booklists', 'There was an issue executing SQL query (most likely a syntax error): @exception', array('@exception' => $sql), WATCHDOG_ERROR);
        return FALSE;
      }
      // Close the connection.
      $conn = NULL;
    }
    $output_rows = '';
    $query = '';
    $page = pager_find_page();
    $offset = $page * $results_per_page;
    $maximum = $offset + $results_per_page;
    // Build the polaris query with all of these bib ids in it.
    if (isset($bibs) && is_array($bibs) {
      foreach ($bibs as $key => $bib) {
        $bib_id = $bib['BibliographicRecordID'];
        if ($key == 0) {
          $query .= $bib_id;
        }
        else {
          $query .= ',' . $bib_id;
        }
      }
      $query = 'q=CN=%7Blist%7D' . $query . '%7B/list%7D&bibsperpage=100';
      // Load include required for bento_boxes_build_boolean_polaris_query().
      module_load_include('inc', 'bento_boxes', 'bento_boxes.pages');
      $query .= bento_boxes_build_boolean_polaris_query(100, 1);
      $query = str_replace('+NOT+TOM=ebk', '', $query);
      $query = str_replace('%7B/list%7D&bibsperpage=100', '%7B/list%7D', $query);
      if (empty($query_params['sort_by'])) {
        // Don't sort by relevance by default like bento boxes does. Let's not sort. That way the list will come back in the same order that we sent it in.
        $query = str_replace('+sortby+RELEVANCE', '', $query);
      }
      if (count($bibs) > 0) {
        $results = PolarisAPI::searchBibs($query, 'boolean');
        $total_items = count($results->BibSearchRows);
      }
      // Print out the details of each item.
      if ($results_only == FALSE) { // Page display
        // START GETTING RATINGS FROM AMAZON.
        if (isset($query_params['sort_by']) && $query_params['sort_by'] == 'reviews') {
          foreach ($results->BibSearchRows as $key => $row) {
            if (!empty($row->ISBN)) {
              $asin = $row->ISBN;
            }
            if (empty($item) && !empty($row->UPC)) {
              $asin = amazon_upc_lookup($row->UPC); // Technically it's not an ISBN. It's an ASIN we're retrieving here, but let's just go with it for now.
              if (empty($asin) && strlen($code) == 13) {
                // If the UPC doesn't pull anything back from Amazon, try it as an EAN to see if we get anything.
                $asin = amazon_ean_lookup($code);
              }
            }
            $params = array(
              'ItemId' => $asin,
              'IdType' => $type,
              'ResponseGroup' => 'Reviews'
            );
            $results_amazon = amazon_http_request('ItemLookup', $params);
            $item = (array) $results_amazon->Items->Item->CustomerReviews->IFrameURL;
            $review = 0.05; // Basically make it 0. But theme_fivestar_static won't display it if it's absolute zero.
            if (!empty($item)) {
              $iframe_url = $item[0];
              $iframe_content = file_get_contents($iframe_url);
              preg_match('/alt="([^\s]*) out of 5 stars"/', $iframe_content, $matches);
              $review = $matches[1];
              if (empty($review)) { $review = 0.05; }
            }
            $results->BibSearchRows[$key]->review = $review;
          }
          // Sort based on ratings.
          usort($results->BibSearchRows, 'booklists_sort_reviews');
        }
        // END RATINGS.

        // OUTPUT
        if (isset($results) && is_object($results) && count($results->BibSearchRows) > 0) {
          foreach ($results->BibSearchRows as $key => $row) {
            // Let's only show 20 on the currently-active page. Not ALL of them.
            $l = $key + 1;
            if ($l > $offset && $l <= $maximum) {
              $output_rows .= theme('bento_boxes_polaris_search_result', array('row' => $row));
            }
          }
        }
      } // End checking to see if isset $bibs & is_array $bibs.
      if (count($results) == 0 || empty($results)) {
        $output_rows .= '<div class="view-empty"><p>' . t('No results were found.') . '</p></div>';
      }
      else {
        // Add pagers.
        pager_default_initialize($total_items, $results_per_page);
        $pager = theme('pager');
        $pager = str_replace('>next', '>Next', $pager);
        $pager = str_replace('previous<', 'Previous<', $pager);
        $output_rows = '<div class="bb_search-results-sort-paging">
          <div class="bb_search-results-sort">
            <div class="bb_search-results-sort-link" style="width: 80%">' . bento_boxes_build_sort_links() . '</div>
            <div class="bb_search-results-paging">' . $pager . '</div>
          </div>
        </div>' . $output_rows;
        // Bottom pager
        $output_rows .= '<div class="bb_search-results-sort-paging"><div class="bb_search-results-sort"> <div class="bb_search-results-paging">' . $pager . '</div></div></div>';
      }
    }
  }
  catch (Exception $e) {
    throw $e;
  }
  if ($results_only == FALSE) {
    return $output_rows;
  }
  else {
    return $results;
  }
}

/**
 * Custom sort function to put shipped items at the top, cancelled items at the bottom.
 */
function booklists_sort_reviews($a, $b) {
  if ($a->review > $b->review) {
    return -1; // A is greater than B. Move B down in the list.
  }
  else if ($a->review < $b->review) {
    return 1; // B is greater than A. Move it up in the list.
  }
  else {
    return 0; // Otherwise don't move it.
  }
}

/**
 * Concatenate all of the limiters together to where they make sense in the query and are logically grouped with
 * "AND"s and "OR"s as specified in the comments above.
 * Returns string $limiters, which is appended to the existing query.
 */
function booklists_limiters($array_of_limiters) {
  $limiters = '';
  if (!empty($array_of_limiters)) {
    $limiters .= ' AND (';
    foreach($array_of_limiters as $key => $value) {
      if ($key > 0) {
        $limiters .= ' OR ' . $value;
      }
      else {
        $limiters .= $value;
      }
    }
    $limiters .= ')';
  }
  return $limiters;
}
