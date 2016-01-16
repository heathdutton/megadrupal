<?php
/**
 * @file
 * Function called by Ajax methods in search pages
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * Bookmark one or more items
 *
 * @return (String)
 *   A JSON string containing the result code:
 *   - -1: the bookmark does already exist
 *   - 0: the operation failed
 *   - 1: new bookmark were created
 */
function xc_search_ajax_bookmark_item() {
  global $user;

  list($nid, $sid) = explode('|', $_GET['id']);
  $result_code = xc_account_bookmark_item($user->uid, $nid, $sid);

  drupal_json_output(array('result' => (int) $result_code));
}

/**
 * Remove bookmark items
 */
function xc_search_ajax_remove_bookmark_item() {
  global $user;

  list($nid, $sid) = explode('|', $_GET['id']);
  $result_code = xc_account_remove_bookmark_item($user->uid, $nid, $sid);

  drupal_json_output(array('result' => (int) $result_code));
}

/**
 * Gets an image URL based on ISBN/UPC/OCLC identifiers
 *
 * @return JSON
 *   The URL of the image
 */
function xc_search_ajax_image_info() {
  $isbn = $_GET['ISBN'];
  $upc  = $_GET['UPC'];
  $oclc = $_GET['OCLC'];
  $size = (isset($_GET['large']) && $_GET['large'] == 1) ? SYNDETICS_LARGE : SYNDETICS_SMALL;
  $url = xc_search_get_syndetics_image($isbn, $upc, $oclc, $size);

  if (isset($_GET['debug'])) {
    return 'URL is ' . $url;
  }

  drupal_json_output(array('image_url' => $url));
}

/**
 * Gets the WorldCat Identity URL, and redirects
 */
function xc_search_ajax_open_worldcat_identity_url() {
  // alternative way, using OpenURL:
  // http://worldcat.org/identities/find
  // ?url_ver=Z39.88-2004
  // &rft_val_fmt=info:ofi/fmt:kev:mtx:identity
  // &rft.namelast=Hickey
  // &rft.namefirst=Thomas
  // &rft_id=info:oclcnum/9746645
  $worldcat_url = 'http://worldcat.org/identities/find?fullName=' . urlencode($_GET['sURL']);
  $content = file_get_contents($worldcat_url);
  $xml = new SimpleXMLElement($content);
  $types = array('ExactMatches', 'FuzzyFirstName', 'FuzzyFamilyName');
  $uri = '';
  foreach ($types as $type) {
    $results = $xml->xpath('//nameAuthorities/match[@type="' . $type . '"]/uri');
    if (!empty($results)) {
      while(list( , $node) = each($results)) {
        $uri = $node;
        break;
      }

      // an example URI:
      // name with LCCN: '/identities/lccn-n79-6533'
      // name without LCCN: '/identities/np-levan, ralph r'
      break;
    }
  }
  $url = strlen($uri) > 0 ? 'http://worldcat.org' . $uri : $_GET['sURL'];
  drupal_add_http_header('Location', $url);
  return;
}

/**
 * Get status info from NCIP
 *
 * The path to this function is xc_search/ajax/ncip_info
 * The JS variable of that URL is Drupal.settings.xc_search.ncip_url
 * The JS function to call this function is XCSearch.getAvailability
 */
function xc_search_ajax_ncip_info() {
  $ncip_id  = $_GET['ncip_id'];
  $bib_id   = $_GET['bib_id'];
  $identifier_int = $_GET['identifier_int'];
  $org_code = $_GET['org_code'];

  $availability = '';
  $call_number = '';
  $response = xc_ils_get_availability($ncip_id, $bib_id, $org_code);

  $no_response = FALSE;
  if (empty($response) || is_null($response)) {
    xc_log_error('ncip', 'No response for %bib at ncip #%ncip', array('%bib' => $bib_id, '%ncip' => $ncip_id));
    $no_response = TRUE;
  }

  if ($no_response || isset($response['error']) || !isset($response['results']['bib'][$bib_id])) {
    // $availability = ''; //<span class="not-available">' . t('Availability Unknown') . '</span>';
    if (isset($response['error'])) {
      if (is_array($response['error']) && isset($response['error']['ErrorMessage'])) {
        xc_log_error('ncip', $response['error']['ErrorMessage']);
      }
      else {
        xc_log_error('ncip', $response['error']);
      }
    }
    $result = xc_search_get_location_from_holdings($identifier_int);
    $availability = $result->availability;
    $call_number = $result->call_number;
    $available_item = array(
      'availability' => $availability,
      'call_number' => $call_number,
    );
    $available_items = $result->available_items;
    $use_holdings = TRUE;
  }
  else {
    // $bib = $status = $response['results']['bib'][$bib_id][0];
    $available_items = 0;
    $not_available_items = 0;
    $use_holdings = FALSE;
    foreach ($response['results']['bib'][$bib_id] as $key => $bib) {
      if (!isset($bib['CirculationStatus']) || empty($bib['CirculationStatus'])) {
        $result = xc_search_get_location_from_holdings($identifier_int);
        $availability = $result->availability;
        $call_number = $result->call_number;
        $available_item = array(
          'availability' => $availability,
          'call_number' => $call_number,
        );
        $available_items = $result->available_items;
        $use_holdings = TRUE;
      }
      else {
        $call_number = $bib['ItemDescription']['CallNumber'];
        xc_log_info('NCIPCirc', $bib_id . ': ' . $bib['CirculationStatus']);
        if ($bib['CirculationStatus'] == 'Not Charged') {
          $availability = $bib['Location'] . ' &mdash; <span class="available">' . t('Available') . '</span>';
          $available_items++;
          $available_item = array(
            'availability' => $availability,
            'call_number' => $call_number,
          );
        }
        else {
          $availability = $bib['Location'] . ' &mdash; <span class="not-available">' . t('Not Available') . '</span>';
          $not_available_items++;
        }
      }
    }

    if ($available_items == 0) {
      if ($not_available_items > 1) {
        $availability = t('Multiple') . ' &mdash; <span class="not-available">' . t('Not Available') . '</span>';
        $call_number = t('Multiple');
      }
      // $call_number = t('not available');
    }
    elseif ($available_items == 1) {
      $availability = $available_item['availability'];
      $call_number = $available_item['call_number'];
    }
    elseif ($available_items > 1) {
      if (!$use_holdings) {
        $availability = t('Multiple') . ' &mdash; <span class="available">' . t('Available') . '</span>';
      }
      $call_number = t('Multiple');
    }
  }

  if (is_null($availability)) {
    $availability = '';
  }

  if (is_null($call_number)) {
    $call_number = '';
  }

  xc_log_info('xc', $bib_id . ' => ' . $availability);

  if (isset($_GET['debug'])) {
    return sprintf('availabilitiy: %s, call number: %s, use_holdings: %s', $availability, $call_number, (int) $use_holdings);
  }

  drupal_json_output(array(
    'status' => 0,
    'availability' => $availability,
    'call_number' => $call_number,
    'use_holdings' => (int) $use_holdings,
  ));
}

/**
 * Gets location information from holdings records
 *
 * @param $identifier_int (int)
 *   The identifier_int field of the xc_metadata_properties table.
 *
 * @return (object)
 *   An object with the following keys:
 *     - availability: the location of the item
 *     - call_number: the call number
 *     - available_items: the number of items
 */
function xc_search_get_location_from_holdings($identifier_int) {
  $children = xc_entity_get_child_identifiers($identifier_int);

  if (!empty($children)) {
    if (count($children) > 1) {
      $call_number = t('Multiple');
      $availability = t('Multiple');
      $available_items = count($children);
    }
    else {
      $entity = xc_metadata_ids_by_identifier_int($children[0]);
      xc_sql_xc_retrieve($entity);

      if (isset($entity->metadata['xc:location'])) {
        $availability = xc_search_get_location($entity->metadata['xc:location'][0]['#value']);
      }
      else {
        $availability = '';
      }

      if (isset($entity->metadata['xc:callNumber'])) {
        $call_number = $entity->metadata['xc:callNumber'][0]['#value'];
      }
      else {
        $call_number = '';
      }
      $available_items = 1;
    }
  }

  return (object) array(
    'availability' => $availability,
    'call_number' => $call_number,
    'available_items' => $available_items,
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_get_location_from_holdings_full($bib_identifier_int) {
  $children = xc_entity_get_child_identifiers($bib_identifier_int);

  $rows = array();
  if (!empty($children)) {
    foreach ($children as $identifier_int) {
      $entity = xc_metadata_ids_by_identifier_int($identifier_int);
      xc_sql_xc_retrieve($entity);

      $location = '';
      if (isset($entity->metadata['xc:location'])) {
        $location = xc_search_get_location($entity->metadata['xc:location'][0]['#value']);
      }

      $call_number = '';
      if (isset($entity->metadata['xc:callNumber'])) {
        $call_number = $entity->metadata['xc:callNumber'][0]['#value'];
      }

      $holding_info = array();
      if (!empty($entity->metadata['xc:textualHoldings'])) {
        $has_holdings_info = TRUE;
        foreach ($entity->metadata['xc:textualHoldings'] as $_textual_holding) {
          $holding_info[] = $_textual_holding['#value'];
        }
      }
      $textual_holding = join('<br />', $holding_info);

      $rows[] = array('data' => array(
          array(
            'data' => $location,
            'class' => array('xc-circ-location'),
          ),
          array(
            'data' => $call_number,
            'class' => array('xc-circ-call-number'),
          ),
          array(
            'data' => $textual_holding,
            'class' => array('xc-circ-holdings'),
          ),
          array(
            'data' => '',
            'class' => array('xc-circ-status'),
          ),
        ));
    }
  }

  return (object) array(
    'rows' => $rows,
    'has_holdings_info' => $has_holdings_info,
  );
}

/**
 * Creates results for Full Record Display's NCIP request
 */
function xc_search_ajax_ncip_info_full() {
  $ncip_item_ids = array();
  $id_int = $_GET['id_int'];

  if ($_GET['bib_ids'] == '') {
    $locations = '';
  }
  else {
    $bib_ids = $_GET['bib_ids'];
    if (!is_array($bib_ids)) {
      $bib_ids = explode(',', $_GET['bib_ids']);
    }
    $token = $_GET['token'];
    xc_ajax_bootstrap($token);
    $ncip_item_ids[$id_int] = $bib_ids;
    $response = xc_search_get_full_ncip($ncip_item_ids);
    $locations = $response->locations;
    $count     = $response->count;
    $no_ncip_item_response = $response->no_ncip_item_response;
    // theme('manifestation_full_locations', $locations)
  }

  if (isset($_GET['debug'])) {
    return $locations;
  }

  drupal_json_output(array(
    'status' => 0,
    'content' => $locations,
    'count' => $count,
    'no_ncip_item_response' => $no_ncip_item_response,
  ));
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_get_full_ncip($ncip_ids) {
  $stack_directory_path = variable_get('xc_search_mapit_file', drupal_get_path('module', 'xc_search') . '/demo_stack_directory.pdf');

  // TODO: put this value into UI's properties
  $max_rows_to_display = 5;
  $no_ncip_item_response = array();
  $has_holdings_info = FALSE;
  $has_availability_info = FALSE;
  $rows = array();

  foreach ($ncip_ids as $identifier_int => $complex_ids) {

    if (!is_array($complex_ids)) {
      $complex_ids = array($complex_ids);
    }

    $bib_ids = array();
    $org_codes = array();
    foreach ($complex_ids as $complex_id) {
      list($bib_id, $ncip_provider_id, $org_code) = explode('|', $complex_id);
      $response = xc_ils_get_availability($ncip_provider_id, $bib_id, $org_code);

      if (isset($response['error'])) {
        $result = xc_search_get_location_from_holdings_full($identifier_int);
        $rows = array_merge($rows, $result->rows);
        if ($result->has_holdings_info) {
          $has_holdings_info = TRUE;
        }

        $msg = t('Error occured while requesting the circulation status of %item: %msg',
          array(
          '%item' => xc_util_conditional_join(', ', $bib_ids),
          '%msg' => $response['error']['ErrorMessage'],
        )
        );
        xc_log_error('ncip', $msg);
      }
      else {
        // No NCIP info for this bib id
        if (!isset($response['results']['bib'][$bib_id])) {
          $no_ncip_item_response[] = $bib_id;
          $result = xc_search_get_location_from_holdings_full($identifier_int);
          $rows = array_merge($rows, $result->rows);
          if ($result->has_holdings_info) {
            $has_holdings_info = TRUE;
          }
        }
        else {
          // TODO: admin interface for xc_search_mapit_file, maybe for ILS settings?
          $link_location = FALSE;
          if ($link_location) {
            $mapit_url = $GLOBALS['base_url'] . '/' . $stack_directory_path;
          }

          // process each item level record
          foreach ($response['results']['bib'][$bib_id] as $item) {
            if (!empty($item['Location'])) {
              $location = $item['Location'];
            }
            else {
              $location = xc_ncip_provider_get_name($ncip_provider_id);
            }

            // TODO: link location
            if ($link_location) {
              $location = l($location, $mapit_url, array('attributes' => array('target' => '_blank')));
            }

            $call_number = $item['ItemDescription']['CallNumber'];
            $holdings_info = $item['ItemDescription']['HoldingsInformation'];
            if ($has_holdings_info == FALSE && !empty($holdings_info)) {
              $has_holdings_info = TRUE;
            }

            if (!empty($item['CirculationStatus'])) {
              if ($item['CirculationStatus'] == 'Not Charged') {
                // $status = t('Available');
                $status = $item['CirculationStatus'];
                $status_class = 'xc-circ-status-available';
              }
              else {
                $status = $item['CirculationStatus'];
                $status_class = 'xc-circ-status-other';
              }
              $has_availability_info = TRUE;
              $data = array(
                array(
                  'data' => $location,
                  'class' => array('xc-circ-location'),
                ),
                array(
                  'data' => $call_number,
                  'class' => array('xc-circ-call-number'),
                ),
                array(
                  'data' => $holdings_info,
                  'class' => array('xc-circ-holdings'),
                ),
                array(
                  'data' => $status,
                  'class' => array('xc-circ-status', $status_class),
                ),
              );
              $rows[$item['ItemId']] = array('data' => $data);
            }
            // no CirculationStatus
            else {
              $no_ncip_item_response[] = $bib_id;
              $status = t('Availability Unknown');
              $status_class = 'xc-circ-status-unknown';
              $result = xc_search_get_location_from_holdings_full($identifier_int);
              $rows = array_merge($rows, $result->rows);
              if ($result->has_holdings_info) {
                $has_holdings_info = TRUE;
              }
            }
          } // each items
        } // has this bib id
      } // no errors in response
    } // foreach complex ids
  }
  // order by ItemId
  krsort($rows);
  $rows = array_values($rows);

  // add classes and other information and modify rows if there may be missing
  // last cell, or the number of rows are greater than maximum displayable

  $count = count($rows);
  $hide_rows = $count > $max_rows_to_display;

  for ($i = 0; $i < $count; $i++) {
    $rows[$i]['class'] = array();

    if ($i == 0) {
      $rows[$i]['class'][] = 'first';
    }
    elseif ($i == (count($rows) - 1)) {
      $rows[$i]['class'][] = 'last';
    }

    if ($i >= $max_rows_to_display) {
      $rows[$i]['class'][] = 'hidden';
    }

    if ($has_holdings_info && count($rows[$i]['data']) == 3) {
      $rows[$i]['data'][] = '';
    }
  }

  // Add a row, which contains a 'More' link to show all availability info
  if ($hide_rows) {
    $top_link = '<div id="xc-circ-link-on-top"><a href="#">' . t('Show less') . '</a></div>';
    $data = array(array(
        'data' => '<a href="#" id="xc-circ-link">'
              . t('Show more (!count)', array('!count' => $count)) . '</a>',
        'align' => 'right',
        'colspan' => ($has_holdings_info ? 4 : 3),
      ));
    $rows[] = array(
      'data' => $data,
      'id' => 'trigger-1',
    );
  }

  $header = array(t('Location'), t('Call number'), '', '');
  if ($has_holdings_info) {
    $header[2] = t('Volume information');
  }
  if ($has_availability_info) {
    $header[3] = array(
      'data' => t('Availability'),
      'class' => array('xc-circ-status'),
    );
  }

  return (object) array(
    'locations' => $top_link . theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('xc-circ')))),
    'count' => $count,
    'no_ncip_item_response' => $no_ncip_item_response,
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_ajax_more_facet() { // $facet, $query
  // xc_search/more_facets
  $query = $_GET['query'];
  $facet = $_GET['facet'];
  if (!preg_match('/^[a-z]\w+_fc$/', $facet)) {
    drupal_json_output(array(
      'status' => 0,
      'error_msg' => t('Sorry, %this is not a valid facet name.', array('%this' => $facet)),
    ));
  }
  $index_facet = xc_index_facet_load_by_name(preg_replace('/_fc/', '', $facet));

  $filters = !empty($_GET['filter']) ? (array) $_GET['filter'] : array();
  $offset = 5;
  if ($facet == 'format_fc') {
    $offset = 7; // because we hide two values ('Not microform', 'Not online') by default
  }

  require_once drupal_get_path('module', 'xc_search') . '/searching/xc_search_block.inc';
  $checkbox_options = xc_search_block_checkbox_options();
  if ($checkbox_options['online_only'] == 1) {
    $filters[] = 'format_fc:"Online"';
  }
  if ($checkbox_options['exclude_microform'] == 1) {
    $filters[] = 'format_fc:"Not microform"';
  }

  $params = array(
    'facet' => 'on',
    'facet.field' => $facet,
    'facet.mincount' => 1,
    'facet.offset' => $offset,
    'facet.limit' => variable_get('xc_search_more_facet_inline', 15) + 1,
    'facet.missing' => 'on',
    'fq' => array_unique($filters),
  );

  $url_params = xc_search_get_params();
  unset($url_params['facet']);
  unset($url_params['query']);
  unset($url_params['page']);
  unset($url_params['offset']);

  $response = $GLOBALS['_xc_search_server']->search($query, 0, 0, $params, FALSE);
  $terms = array();
  $has_more = FALSE;
  if (isset($response->facet_counts->facet_fields)
       && isset($response->facet_counts->facet_fields->$facet)) {
    foreach ($response->facet_counts->facet_fields->$facet as $term => $count) {
      if ($count != 0 && $term != '_empty_') {
        $url_queries = array_merge_recursive($url_params, array('filter' => $facet . ':"' . $term . '"'));
        $items[] = '<span class="xc-search-facet-link">'
          . l('<span class="facet-term">' . $term . '</span>',
              'xc/search/' . $query, array('query' => $url_queries, 'html' => TRUE)
            )
          . ' <span class="facet-term-count">(' . $count . ')</span>'
          . '</span>';
      }
    }
    if (count($items) >= $params['facet.limit']) {
      $has_more = TRUE;
      // remove one element from the list. This was just a check element whether it has more
      array_pop($items);
    }
    $output .= theme('item_list', array('items' => $items));
  }
  else {
    drupal_json_output(array(
      'status' => 0,
      'error_msg' => t('No terms in that facet'),
    ));
  }

  drupal_json_output(array(
    'status' => 1,
    'content' => $output,
    'hasMore' => $has_more,
  ));
}

/**
 * Autocomplete function
 *
 * @param $query (string)
 *   The string to get suggestions for
 */
function xc_search_autocomplete($query = '') {
  $suggestions = array();
  if ($query) {
    $suggestions = xc_search_suggestions($query, TRUE);
  }
  return drupal_json_output(drupal_map_assoc($suggestions));
}

/**
 * Ajax synonyms
 *
 * @param $query (String)
 *   The actual query
 */
function xc_search_ajax_synonyms($query = '') {
  $synonyms = array();
  if ($query) {
    $synonyms = module_invoke_all('xc_synonyms', 'ajax', $query);
  }
  return drupal_json_output(array('synonyms' => join('', $synonyms)));
}

/**
 * Ajax suggestions
 *
 * @param $query (string)
 *   The string to get suggestions for
 */
function xc_search_ajax_suggestions($query = '') {
  $themed_suggestions = '';
  if ($query) {
    xc_search_current_query($query);
    $suggestions = xc_search_suggestions($query, FALSE);
    $themed_suggestions = theme('xc_search_suggestions_serp', array('terms' => $suggestions));
  }
  return drupal_json_output(array('suggestions' => $themed_suggestions));
}
