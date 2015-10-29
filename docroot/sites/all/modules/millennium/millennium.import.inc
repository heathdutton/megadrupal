<?php

/**
 * Recieves a URL that (presumably) has some WebPAC records in it, and returns
 * an array of record numbers to later harvest via other functions like
 * millennium_mass_fetch()
 *
 * @param $url
 *   URL in which to scan for records
 * @param $limit
 *   Maximum number of records to return.
 * @return
 *   An array of record numbers
 */
function millennium_result_list_scrape($url, $limit = 100) {
  $records = array();
  $base_url = millennium_get_real_baseurl($url);

  // This will loop while more pages of results are found
  while (TRUE) {

    // Launch
    $result = millennium_http_request($url);
    if ($result->code != 200) {
      drupal_set_message(t('WebOPAC returned "Not found" on <a href="@url">@url</a>.', array("@url" => $url)));
      break;
    }

    // Look for <input type="checkbox" name="save" value="b1890629" >
    $ok = preg_match_all(
      '/ value="(b[0-9]+)"[^>]*?>/si',
      $result->data,
      $matches
    );
    if (!$ok) {
      // Try second regexp: cart button
      $ok = preg_match_all(
        '/\?save=(b[0-9]+)/si',
        $result->data,
        $matches
      );
    }
    if (!$ok) {
      #drupal_set_message(t("No records found on URL !link", array("!link" => l($url, $url))));
      break;
    }
    $records = array_merge($records, array_unique($matches[1]));
    if (sizeof($records) >= $limit) {
      break;
    }

    // Look for "next" pager
    $ok = preg_match('/<td .*?class=.browsePager.*?<strong>[0-9]+<\/strong>[^<]*<a href="(\/search[^"]+)">([0-9]+)/si', $result->data, $pager_link_matches);
    if (!$ok) {
      break;
    }
    // Set path for next call from "Next" pager
    $url = $base_url . $pager_link_matches[1];
  }
  return array_splice($records, 0, $limit);
}

/**
 * Recieves a query argument, and returns scraped record numbers to later harvest
 * via other functions like millennium_mass_fetch().
 *
 * @param $query
 *   a query argument, e.g. keywords if the $searchtype is 'X', an isbn for 'i'.
 * @param $base_url
 *   The base_url of the WebOPAC.
 * @param $limit
 *   Maximum number of records to return.
 * @param $searchtype
 *   a character (X, i, etc.) for the type of search to launch.
 */
function millennium_query_scrape($query, $base_url, $limit = 100, $searchtype = 'X') {
  // Build query URL
  $url = "{$base_url}/search/{$searchtype}?" . drupal_encode_path($query);
  // Get records
  return millennium_result_list_scrape($url, $limit);
}

/**
 * Gets a sequential number of records obeying PHP's max_execution_time setting.
 * @param $recnums
 *   Array of item or bib numbers to fetch.
 * @param $base_url
 *   The base url of the WebOPAC to process.
 */
function millennium_mass_fetch($recnums, $base_url) {
  // This seems to be the limit for WebOPACs, do not change =)
  $max_records_for_bookcart = 40;

  // Ping the server to ensure we have obtained a cookie.
  $ok = millennium_ping($base_url);
  if (!$ok) {
    return array('found' => array(), 'not_found' => $recnums);
  }

  // Don't devote more than X% of max_execution_time
  $max_time = ini_get('max_execution_time') * 0.85;

  // Chunk array into groups of $max_records_for_bookcart
  $chunks = array_chunk($recnums, $max_records_for_bookcart);
  // Initialize results
  $results = array('found' => array(), 'not_found' => array(), 'elapsed' => 0);
  // Process chunks
  foreach ($chunks as $chunk) {
    $result = millennium_fetch_records_via_bookcart($chunk, FALSE, $base_url);
    $results['found'] = array_merge($results['found'], $result['found']);
    $results['not_found'] = array_merge($results['not_found'], $result['not_found']);
    $results['elapsed'] += $result['elapsed'];

    // Break if running time has exceeded $max_time
    if ($max_time > 0 && $results['elapsed'] > $max_time) {
      break;
    }
  }
  return $results;
}

/**
 * Gets item information (bib number & MARC record) using the III's book cart.
 * Recieves an array of item numbers and returns an array of found item data
 *   including bib number and MARC keyed by item number, and an unkeyed array
 *   of not found items.
 *
 * @param $recnums
 *   An unkeyed array of item or bib numbers = array('i123456',
 *   'b123456', ...)
 * @param $complete_holdings
 *   A flag that specifies if an extra request should be done
 *   if the holdings table is incomplete.
 * @param $base_url
 *   The base url of the WebOPAC to process.
 *
 * @return
 *   Returns an array of results in the following format:
 *   $results = array(
 *    'found' => array(
 *      'b100001' => array(
 *        'bib_recnum' => 'b100001',
 *        'marc' => 'LEADER 00000cam 2200000 a 4500 001 tec042...
 *        'holdings' => '...'
 *      ),
 *      'i222222' => array(
 *        'item_recnum' => 'i222222',
 *        'bib_recnum' => 'b426763',
 *        'marc' => 'LEADER 00000cam 2200000 a 4500 001 tec042...
 *        'holdings' => array(
 *          0 => array(
 *            'location' => 'MTY 4to. Piso',
 *            'classnumber' => 'QH541.15.B56 E95 2009',
 *            'classvolume' => '',
 *            'status' => 'CATALOGACION',
 *          ),
 *        ),
 *      ),
 *    ),
 *    'not_found' => array('i100000', 'i100005', [...])
 *  );
 */
function millennium_fetch_records_via_bookcart($recnums, $complete_holdings = FALSE, $base_url) {
  // Set the following to TRUE for lots and lots of debugging info =)
  $debug = FALSE;

  // Start timer to measure average performance
  timer_start("millennium_fetch_records_via_bookcart");

  $found_items = array();
  $not_found_items = array();

  // Clear the cart
  $path = "/search?//1,-1,-1,B/browse?clear_saves=1";
  $result = millennium_http_request("{$base_url}{$path}");
  if ($result->code != 200) {
    $timer = timer_stop("millennium_fetch_records_via_bookcart");
    return array('found' => array(), 'not_found' => $recnums, 'elapsed' => $timer['time'] / 1000);
  }

  // Issue N requests to add items to the bookcart, 25 records at a time is apparent maximum
  $chunks = array_chunk($recnums, 20);
  foreach ($chunks as $chunk) {
    // Add items from $recnums array to the cart
    $path = '/search*eng/X//1,1,1/?save=' . implode("&save=", $chunk);
    $result = millennium_http_request("{$base_url}{$path}");
    if ($debug) {
      drupal_set_message("Added items to bookcart response: path = " . check_plain($base_url . $path));
      dpm($result);
    }
  }

  // Get cart contents: only item and bib numbers
  // From matches in list we can determine if some item numbers do not actually exist in the database.
  $path = "/search*eng/X/++export/1,-1,-1,B/export";
  $bookcart_matches = array();
  while (TRUE) {
    // Repeat until no "next" pager link found
    $result = millennium_http_request("{$base_url}{$path}");
    if ($debug) {
      drupal_set_message("Cart contents and matches:");
      dpm($result);
    }
    // Match the bib record, the given record and the title (for false positives, see $prev_title below).
    // Sample:
    //    <input type="checkbox" name="save" value="b1100000" /> &nbsp;</td> <td class="browseEntryData"> <a href="/record=b1100000~S63">The city trilogy : five jade disks, defenders of the dragon </a> Chang Hsi-kuo &#59; translated from the Chinese by John Balcom. : Chang, S. K.; <strong> CCM GENERAL:DISPONIBLE, CEM GENERAL:DISPONIBLE, CSF GENERAL:DISPONIBLE, SIN GENERAL:DISPONIBLE</strong> &nbsp; </td>
    $ok = preg_match_all(
      '/name="save" value="(b[0-9]+)"[^>]+>.*?"browseEntryData">.*?record=([bi][0-9]+)[^>]+>([^<]*)<\/a>/si',
      $result->data,
      $tmp_matches,
      PREG_SET_ORDER
    );
    if ($debug) {
      drupal_set_message("One page of item<->bib matches from bookcart:");
      dpm($tmp_matches);
    }
    if ($ok) {
      $bookcart_matches = array_merge($bookcart_matches, $tmp_matches);
    }
    // Look for "next" pager: Found code like this:
    // <a href="/search/?/++export/13%2C-1%2C-1%2CB/export/">Next</a>
    // <a href="/search~S1/?/++export/13%2C-1%2C-1%2CB/export/">Next</a>
    // <a href="/search~S24/X/++export/25%2C-1%2C-1%2CB/export/&SORT=D">Next</a>
    $ok = preg_match('/<a href="(\/search[^"]+export\/[^"]+\/export\/[^"]*)">Next/si', $result->data, $pager_link_matches);
    if (!$ok) {
      break;
    }
    $path = $pager_link_matches[1];
  }
  if ($debug) {
    drupal_set_message("Finished paging thru bookcart. These are all the item<->bib matches from bookcart:");
    dpm($bookcart_matches);
  }

  // Start off assuming no items have been found
  foreach ($recnums as $num) {
    $not_found_items[$num] = $num;
  }

  // Order match data into an array
  // NOTE: At this point some matches are false, since the bookcart export shows
  //  items for record numbers that don't actually exist. These will be dealt
  //  with below, by checking MARC against the title shown and stored here.
  foreach ($bookcart_matches as $match) {
    $found_recnum = $match[2];
    $title = trim(decode_entities($match[3]));
    if ($title && $found_recnum) {
      // Remove found items off $not_found_items list
      unset($not_found_items[$found_recnum]);
      // Store title and recnum
      $found_items[$found_recnum] = array(
        'bib_recnum' => $match[1],
        'title' => $title,
        'base_url' => $base_url,
      );
      // If supplied recnum is an item record, store item <=> bib relationship
      if (substr($found_recnum, 0, 1) == "i") {
        $found_items[$found_recnum]['item_recnum'] = $found_recnum;
      }
    }
  }
  #file_put_contents('c:\temp\found_items.txt', var_export($found_items, TRUE)); // TODO debug remove

  // Get MARC for all!
  $path = "/search*eng/?/++export/1,-1,-1/export/";
  $post = "email_addx=&email_subj=&ex_device=43&ex_format=50";
  $result = millennium_http_request("{$base_url}{$path}", array(), 'POST', $post);
  // Split records
  $ok = preg_match_all(
      '/'
    . '<div class="exportHeading">.*?<pre>\s+(.*?)\s+<\/pre>.*?'
    . '(<table[^>]+class=.bibItems.*?<.table>|<.table>)' // Holdings table
    . '(?:.<center>.<form .*? action="(.*?)".*?<\/form>|)' // Form to show more holdings : example item: Newsweek : http://millenium.itesm.mx/record=b278353
    . '/si',
    $result->data,
    $marc_matches,
    PREG_SET_ORDER
  );
  if ($debug) {
    drupal_set_message("MARC request result:");
    dpm($result);
    drupal_set_message("MARC matches:");
    dpm($marc_matches);
  }
  #file_put_contents('c:\temp\marc_matches.txt', var_export($marc_matches, TRUE)); // TODO debug remove

  // Assign marc to the record data in $found_items
  // As mentioned above, $found_items contains some false positives, so check
  // the title stored above against the marc data before storing. False
  // positives get sent to the not_found_items array.
  $found_items_indexes = array_keys($found_items);
  $found_items_index = 0; // start at first item in $found_items
  foreach ($marc_matches as $count => $match) {
    $marc_text = decode_entities($match[1]);
    if (!isset($found_items_indexes[$found_items_index])) {
      break;
    }
    $current_recnum = $found_items_indexes[$found_items_index];
    $current_found_item = $found_items[$current_recnum];
    $compare_marc = _millennium_only_letters($marc_text);
    if ($debug) {
      dpm("MARC #{$count} : {$compare_marc}");
    }
    // Check if current found_item title coincides with title in this MARC
    while ($current_recnum && $found_items_index < sizeof($found_items_indexes)) {
      // TODO: The next line is too simplistic; maybe it needs to parse the marc
      //   and only use the 240|a and 245|a portion to compare to the title in
      //   $current_found_item["title"]
      $compare_title = $current_found_item["title"];
      $compare_title = _millennium_only_letters($compare_title);
      if (@mb_strpos($compare_marc, $compare_title) === FALSE) {
        // No; then remove this found_item and skip to the next one, check again
        if ($debug) {
          drupal_set_message("NOT IN SYNC ={$current_recnum}============");
          drupal_set_message("Current found_item:");
          dpm($current_found_item);
          drupal_set_message("Found_item: " . $compare_title);
        }
        $not_found_items[$current_recnum] = $current_recnum;
        unset($found_items[$current_recnum]);
        $found_items_index++;
        $current_recnum = isset($found_items_indexes[$found_items_index]) ? $found_items_indexes[$found_items_index] : FALSE;
        if ($current_recnum) {
          $current_found_item = $found_items[$current_recnum];
        }
      }
      else {
        if ($debug) {
          drupal_set_message("In sync ===={$current_recnum}=========");
          drupal_set_message("Found_item: " . $compare_title);
        }
        $found_items[$current_recnum]['marc'] = $marc_text;
        break;
      }
    }

    if ($current_recnum) {
      // Add holdings information; determine if an extra request is needed
      $this_bib_recnum = $found_items[$current_recnum]['bib_recnum'];
      // Does listing show a "View additional copies or search for a specific
      // volume/copy" button? Do we want to check for complete holdings?
      if (isset($match[3]) && $match[3] && $complete_holdings == TRUE) {
        // Yes. Issue additional request to OPAC to get holdings information for item.
        $found_items[$current_recnum]['holdings'] = millennium_get_holdings_info($this_bib_recnum, $base_url);
      }
      else {
        // No. Convert whatever holdings info (might be incomplete) we got along
        // with the MARC export and add it to the item array.
        $found_items[$current_recnum]['holdings'] = millennium_get_holdings_info($this_bib_recnum, $base_url, $match[2]);
      }
      $found_items_index++;
    }
  }

  // If any candidate found items didn't match to any MARC, clear them off the
  // $found_items array
  $current_recnum = isset($found_items_indexes[$found_items_index]) ? $found_items_indexes[$found_items_index] : FALSE;
  while ($current_recnum) {
    $not_found_items[$current_recnum] = $current_recnum;
    unset($found_items[$current_recnum]);
    $found_items_index++;
    $current_recnum = isset($found_items_indexes[$found_items_index]) ? $found_items_indexes[$found_items_index] : FALSE;
  }

  // Grab XRECORD for item
  #foreach ($found_items as $item_recnum => $dummy) {
  #  $result = millennium_fetch_recordpage($item_recnum, $base_url, 'xml');
  #  if ($result->data) {
  #    $found_items[$item_recnum]['item_xml'] = $result->data;
  #  }
  #}
  #dpm($found_items);

  // Debug, confirm found/not found items
  if ($debug) {
    _millennium_confirm_found_not_found($base_url, $found_items, $not_found_items);
  }

  // Clear the cart
  $path = "/search?//1,-1,-1,B/browse?clear_saves=1";
  $dummy = millennium_http_request("{$base_url}{$path}");

  // Return results
  $timer = timer_stop("millennium_fetch_records_via_bookcart");
  $results = array('found' => $found_items, 'not_found' => $not_found_items, 'elapsed' => $timer['time'] / 1000);
  if ($debug) {
    drupal_set_message("Final results:");
    dpm($results);
  }

  return $results;
}

function _millennium_only_letters($string) {
  // Remove MARC indicators
  $new = mb_ereg_replace("\|[a-z]", "", $string);
  // Remove things like "[videorecording]" in titles
  $new = mb_ereg_replace("\[[^\]]+\]*", "", $new);
  // Remove entities like "{232}" and "{u02B1}" in titles
  $new = mb_ereg_replace("{u*[0-9A-F]*}*", "", $new);
  // Leave only letters and numbers
  $new = drupal_strtolower(mb_ereg_replace("\W", "", $new));
  $new = str_replace(array(' ', "\n", "\r", '"'), '', $new);
  return $new;
}

/**
 * Debug function that confirms if found/not found records indeed exist or not
 * in OPAC. CAUTION: SLOW as it requests records one by one.
 *
 * @param $found_items
 *   Array of found records.
 * @param $not_found_items
 *   Array of non-found record numbers.
 */
function _millennium_confirm_found_not_found($base_url, $found_items, $not_found_items) {
  $false_positives = array();
  foreach ($found_items as $recnum => $found_item) {
    $tmp = millennium_fetch_recordpage($recnum, $base_url, 'plain');
    if (!strpos($tmp->data, '<a id="recordnum"')) {
      $false_positives[] = $recnum;
    }
  }
  if ($false_positives) {
    drupal_set_message("FALSE POSITIVES FOR {$base_url}:");
    dpm($false_positives);
    #file_put_contents("c:\\temp\\false_positives.txt", var_export($false_positives, TRUE));
  }
  else {
    drupal_set_message("NO FALSE POSITIVES FOR {$base_url}!");
  }

  $false_negatives = array();
  foreach ($not_found_items as $recnum => $not_found_item) {
    $tmp = millennium_fetch_recordpage($recnum, $base_url, 'plain');
    if (strpos($tmp->data, '<a id="recordnum"')) {
      $false_negatives[] = $recnum;
    }
  }
  if ($false_negatives) {
    drupal_set_message("FALSE NEGATIVES FOR {$base_url}:");
    dpm($false_negatives);
    #file_put_contents("c:\\temp\\false_positives.txt", var_export($false_positives, TRUE));
  }
  else {
    drupal_set_message("NO FALSE NEGATIVES FOR {$base_url}!");
  }
}
