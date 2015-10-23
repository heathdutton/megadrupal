<?php
/**
 * @file
 * These functions are adapted from the AvantLink WordPress add-on
 */

/**
 * Display product search results from AvantLink.(4)
 *
 * @param $search_term
 *   Keyword user entered into Product Search form.
 *
 * @see avantlinker_product_search_global_form_submit()
 *
 * @return
 *   String containing html list of product search results.
 */
function avantlinker_display_product_search_results($search_term = FALSE) {
  // output only if search_term variable is set
  if ($search_term !== FALSE && !empty($search_term)) {
    // get search page url
    $affiliate_id = variable_get('avantlinker_affiliate_id');
    $website_id = variable_get('avantlinker_website_id');
    $int_result_count = intval(variable_get('avantlinker_search_results_number', 10));
    if (get_magic_quotes_gpc()) {
      $search_term = stripslashes($search_term);
    }
    $int_result_count_safe = ($int_result_count > 0 ? $int_result_count : '');
    $str_search_term_safe = check_plain($search_term);

    if ($int_result_count > 0) {
      $search_results_count = $int_result_count;
    }
    elseif (variable_get('avantlinker_search_results_number', 10) != '') {
      $search_results_count = variable_get('avantlinker_search_results_number', 10);
    }
    else {
      $search_results_count = 10;
    }

    $arr_products = avantlinker_api_get_product_search( $affiliate_id, $website_id, $search_term,
      $search_results_count );

    $int_product_count = count($arr_products);
    $str_output = '';
    if ($int_product_count == 0) {
      $str_output .= "<p class=\"no_result\">No Results Found For Search Term: $search_term</p>";
      return $str_output;
    }
    else {
    $str_output = $arr_products;
    }

    return $str_output;
  }
  else {
    $str_output = '<p class="no_result">No search term specified.</p>';
    return $str_output;
  }
}

/**
 * Avantlink related products display.(5)
 *
 * @see avantlinker_product_search_global_form_submit()
 *
 * @return
 *   String containing html list of related product search results.
 */
function avantlinker_display_related_products() {
  $entity_id = arg(1);
  $output = '';
  // drupal variable_get
  $affiliate_id = variable_get('avantlinker_affiliate_id');
  $website_id = variable_get('avantlinker_website_id');
  $d_results = variable_get('avantlinker_products_number');
  $d_keyword = variable_get('avantlinker_global_keywords');
  $d_nkeyword = variable_get('avantlinker_global_negative_keywords');
  $all_posts = variable_get('avantlinker_apply_to_all', array('apply' => 'apply'));

  // get post specific settings
  $keyword = db_query('SELECT avantlinker_keywords_value FROM {field_data_avantlinker_keywords}
    WHERE entity_id = :entity_id', array(':entity_id' => $entity_id))->fetchField();
  if (!empty($keyword)) {
    $search_term = $keyword;
  }
  else {
    $search_term = $d_keyword;
  }
  $nkeyword = db_query('SELECT avantlinker_neg_words_value FROM {field_data_avantlinker_neg_words}
    WHERE entity_id = :entity_id', array(':entity_id' => $entity_id))->fetchField();
  if (!empty($nkeyword) ) {
    $nsearch_term = $nkeyword;
  }
  else {
    $nsearch_term = $d_nkeyword;
  }
  $results = variable_get('avantlinker_products_number', 5);
  $off_for_post = db_query('SELECT avantlinker_rp_off_value FROM {field_data_avantlinker_rp_off}
    WHERE entity_id = :entity_id', array(
    ':entity_id' => $entity_id))->fetchField();
  // logic for global vs post specific setting
  if ($results != '') {
    $num_results = $results;
  }
  else {
    $num_results = $d_results;
  }
  if ($all_posts == 0 && empty($keyword) || $off_for_post == 'off') {
    $disabled = TRUE;
  }
  else {
    $disabled = FALSE;
  }
  $query = '';

  // format negative keywords for request
  $nsearch_query = '';
  $nsearch_terms = explode(',', $nsearch_term);
  $int_search_term_count = count($nsearch_terms);
  for ($i = 0; $i < $int_search_term_count; $i++) {
    if (trim($nsearch_terms[$i]) != '') {
      $nsearch_term = str_replace(' ', ' -', trim($nsearch_terms[$i]));
      $nsearch_query .= '-' . $nsearch_term . ' ';
    }
  }
  $nsearch_query = trim($nsearch_query);

  // format keyword for request
  $search_terms = explode(',', $search_term);
  $int_search_term_count = count($search_terms);
  for ($i = 0; $i < $int_search_term_count; $i++) {
    if (trim($search_terms[$i]) != '') {
      $search_term = str_replace(' ', '+', $search_terms[$i]);
    if ($query == '') {
      $query = $search_term;
    }
    else {
      $query .= ' | ' . $search_term;
    }
    if ($nsearch_query != '') {
      $query .= ' ' . $nsearch_query;
    }
    }
  }

  if ($disabled == FALSE) {
    $arr_products = avantlinker_api_get_product_search( $affiliate_id, $website_id, $query, NULL, NULL, '1', 'nofollow' );
    // Move to template
  $int_product_count = count($arr_products);
  if ($int_product_count == 0) {
    $output = FALSE;
  }
  else {
        // max 10 results
        if ($num_results > 10) {
          $num_results = 10;
        }
        if (!is_numeric($num_results)) {
          $num_results = 5;
        }
        if ($int_product_count < $num_results) {
          $num_results = $int_product_count;
        }
        $arr_products = array_slice($arr_products, 0, $num_results);
        $output = $arr_products;
      }
  }

  return $output;
}

/**
 * Perform a product search for an affiliate.(3)
 *
 * @param $affiliate_id
 *   Unique ID give to each affiliate by AvantLink.
 * @param $website_id
 *   Associates your site with tracking in reports for
 *   affiliates at AvantLink.
 * @param $search_term
 *   From the per post keywords saved or if none given it
 *   reverts to the global keywords stored in a drupal variable.
 * @param $search_result_count
 *   Number of search results to display as set in Drupal configuration.
 * @param $search_result_count
 *   Not used at this time.
 * @param $adv_syntax
 *   Not used at this time.
 * @param $result_options
 *   Not used at this time.
 *
 * @see avantlinker_display_product_search_results()
 * @see avantlinker_display_related_products()
 *
 * @return
 *   Raw search result from avantlink.com.
 */
function avantlinker_api_get_product_search($affiliate_id, $website_id, $search_term, $search_result_count = NULL,
  $sort_order = NULL, $adv_syntax = NULL, $result_options = NULL) {
  $str_url = 'http://www.avantlink.com/api.php?module=ProductSearch&output=tab&affiliate_id=' . $affiliate_id .
    '&website_id=' . $website_id;
  $str_url .= '&search_term=' . urlencode($search_term);
  if (isset($search_result_count) && $search_result_count != NULL) {
    $str_url .= '&search_results_count=' . intval($search_result_count);
  }
  if (isset($sort_order) && $sort_order != NULL) {
    $str_url .= '&search_results_sort_order=' . urlencode($sort_order);
  }
  if (isset($adv_syntax) && $adv_syntax != NULL) {
    $str_url .= '&search_advanced_syntax=' . urlencode($adv_syntax);
  }
  if (isset($result_options) && $result_options != NULL) {
    $str_url .= '&search_results_options=' . urlencode($result_options);
  }
  $str_response = avantlinker_get_url_contents($str_url);
  if ($str_response == '' || strpos($str_response, 'Product Id') === FALSE || strpos($str_response,
    'Product Name') === FALSE) {
    return (array());
  }

  return (avantlinker_parse_tab_delim_response($str_response));
}

/**
 * Fetch contents from a URL.(1)
 *
 * Uses cURL if available, or file_get_contents, or fopen if nothing else.
 *
 * @param $str_request_url
 *   A string added to the AvantLink API URL with option prameters.
 *
 * @see avantlinker_api_get_product_search()
 *
 * @return
 *   Raw search result from avantlink.com.
 *
 */
function avantlinker_get_url_contents($str_request_url) {
  $str_response = '';

  if (function_exists('curl_init')) {
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, $str_request_url);
  if (defined('CURLOPT_ENCODING')) {
    curl_setopt($ch, CURLOPT_ENCODING, 'gzip,deflate');
  }
  curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
  curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, FALSE);
  curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 10);
  curl_setopt($ch, CURLOPT_TIMEOUT, 30);
  curl_setopt($ch, CURLOPT_FOLLOWLOCATION, TRUE);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
  $result = @curl_exec($ch);
  if ($result !== FALSE && curl_errno($ch) <= 0) {
    $str_response = $result;
  }
  @curl_close($ch);
  }
  else {
  if (function_exists('file_get_contents')) {
    $str_response = file_get_contents($str_request_url);
  }
  else {
    $fp = fopen($str_request_url, 'rb');
    if ($fp !== FALSE) {
      while (!(feof($fp))) {
        $str_response .= fread($fp, 4096);
      }
    fclose($fp);
    }
  }
  }

  return ($str_response);
}

/**
 * Parse a tab-delimited result set into an associative array.(2)
 *
 * Useful for processing API responses.
 *
 * @param $str_response
 *   Raw search result from avantlink.com.
 *
 * @see avantlinker_get_url_contents()
 *
 * @return
 *   Arrays ready to have html added.
 *
 */
function avantlinker_parse_tab_delim_response($str_response) {
  $arr_final = array();
  // Split the response into discrete lines (records)
  $str_response = str_replace("\r", '', $str_response);
  $arr_lines = explode("\n", $str_response);
  $int_line_count = count($arr_lines);
  if ($int_line_count < 2) {
    return ($arr_final);
  }
  // Determine field order by the header line
  $arr_fields = explode("\t", $arr_lines[0]);
  $int_field_count = count($arr_fields);
  if ($int_field_count < 1) {
    return ($arr_final);
  }
  for ($i = 0; $i < $int_field_count; $i++) {
    $arr_fields[$i] = str_replace(' ', '_', $arr_fields[$i]);
  }
  // Parse the remaining data into associative arrays
  for ($i = 1; $i < $int_line_count; $i++) {
    if (trim($arr_lines[$i]) != '') {
      $arr_record = explode("\t", $arr_lines[$i]);
      $arr_current = array();
      $int_field_pos = 0;
        foreach ($arr_fields as $str_field_name) {
          $arr_current[$str_field_name] = $arr_record[$int_field_pos];
          $int_field_pos++;
        }
      $arr_final[] = $arr_current;
    }
  }

  return ($arr_final);
}
