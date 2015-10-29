<?php

/**
 * @file
 * This file contains callback functions from hook_menu()
 */

/**
 * Callback function from hook_menu() that imports a new node based on a given item or bib record number.
 */
function millennium_node_import() {
  // Check for valid token
  if (!drupal_valid_token($_GET['token'])) {
    echo t('Invalid token');
    exit;
  }
  // Check if URL is permalink
  $url = check_plain($_GET['url']);
  if (preg_match('/^http[s]*:\/\/[^\/]+\/record=([bi][0-9]+)/', $url, $matches)) {
    $recnum = $matches[1];
    $base_url = millennium_get_real_baseurl($url);
  }
  else {
    return t('Argument should be a valid record permalink URL with syntax: http://example.com/record=b123456');
  }

  if (!preg_match(MILLENNIUM_RECORD_PREG_NOCHECKDIGIT, $recnum)) {
    return t("@recnum is not a valid Millennium record number (b123456, i123456).", array('@recnum' => $recnum));
  }

  $data = array('base_url' => $base_url);
  switch (substr($recnum, 0, 1)) {
    case "i":
      $data['item_recnum'] = $recnum;
      $result = millennium_process_item_record($data, TRUE);
      break;
    case "b":
      $data['bib_recnum'] = $recnum;
      $result = millennium_process_bib_record($data, TRUE);
  }

  if ($result["success"] == FALSE) {
    return t("Failed to import @recnum from URL @url: @error", array('@recnum' => $recnum, '@url' => $base_url, "@error" => $result["error"]));
  }
  else {
    drupal_set_message(t("Record @recnum successfully imported from @url.", array("@recnum" => $recnum, '@url' => $base_url)));
    drupal_goto('node/' . $result['node']->nid);
  }
}

/**
 * Callback function from hook_menu to show how an millennium record would be imported
 */
function millennium_preview_import() {
  // Check if URL is permalink
  $url = check_plain($_GET['url']);
  if (preg_match('/^http[s]*:\/\/[^\/]+\/record=([bi][0-9]+)/', $url, $matches)) {
    $recnum = $matches[1];
    $base_url = millennium_get_real_baseurl($url);

    if (!preg_match(MILLENNIUM_RECORD_PREG_NOCHECKDIGIT, $recnum)) {
      return t("@recnum is not a valid Millennium record number (b123456, i123456).", array('@recnum' => $recnum));
    }

    $output = t("You can also !link", array('!link' => l(t("import this record now"), "millennium/import", array('query' => "url=$url&token=" . drupal_get_token()))));
    $output .= "<hr />";
    $output .= _millennium_show_conversion($recnum, $base_url);
  }
  else {
    $output = t('Argument should be a valid record permalink URL with syntax: http://example.com/record=b123456');
  }
  return $output;
}

/**
 * Callback function from hook_menu that shows import and conversion information for node $nid
 */
function millennium_node_tools($node) {
  $bib_recnum = millennium_bib_recnum_from_node($node->nid);
  if (! $bib_recnum) {
    drupal_set_message(t('This node (@nid) does not have any Millennium record number associated with it.', array('@nid' => $node->nid)));
    drupal_goto('node/' . $node->nid);
  }

  // Show record info
  $data = db_query("SELECT * from {millennium_node_bib} WHERE nid = :nid", array(':nid' => $node->nid))->fetchObject();
  $output = "<h2>" . t("Import history") . "</h2>";

  $items[] = t("WebOPAC: !link", array("!link" => l($data->base_url, $data->base_url)));
  $items[] = t("Bibliographic record #: !link", array("!link" => l($bib_recnum, millennium_permalink($bib_recnum, $data->base_url, 'plain'))));
  $recnums = millennium_item_recnums_from_node($node->nid);
  if ($recnums) {
    $items[] = t("Item records: @items", array("@items" => implode(", ", $recnums)));
  }
  $items[] = t("Node first import date: @date", array("@date" => $data->created));
  $items[] = t("Node last update date: @date", array("@date" => $data->updated));
  $items[] = l(t('Click here to reimport this node'), 'node/' . $node->nid . '/millennium/reimport', array('query' => array('token' => drupal_get_token())));
  $output .= theme('item_list', array('items' => $items));

  $output .= "<p></p>";
  $output .= "<h2>" . t('MARC record and conversion') . "</h2>";
  $output .= _millennium_show_conversion($bib_recnum, $data->base_url, $node->millennium_biblio_data["marc"]);
  return $output;
}

/**
 * Callback function from hook_menu to show marc record for this node
 */
function millennium_node_marc_tab($node) {
  $marc_text = $node->millennium_biblio_data["marc"];
  #drupal_set_title(t("MARC display for: @title", array("@title" => $node->title)));
  #$content = l(t("<< Back to normal display"), "node/$nid");
  $content = "<pre class='millennium marc'>$marc_text</pre>";
  return $content;
}

/**
 * Callback function from hook_menu that reimports a node using its previously stored bib data
 */
function millennium_node_reimport($node) {
  // Check for valid token
  if (!drupal_valid_token($_GET['token'])) {
    echo t('Invalid token');
    exit;
  }
  $bib_recnum = millennium_bib_recnum_from_node($node->nid);
  $base_url = db_query("SELECT base_url FROM {millennium_node_bib} WHERE nid = :nid", array(':nid' => $node->nid))->fetchField();
  if (! $bib_recnum) {
    drupal_set_message(t('This node does not have any Millennium record number associated with it.'));
    drupal_goto('node/' . $node->nid);
  }

  $result = millennium_process_bib_record(array('bib_recnum' => $bib_recnum, 'base_url' => $base_url), TRUE);

  if ($result["success"] !== FALSE) {
    drupal_set_message(t("Node reimported from Millennium item @recnum.", array("@recnum" => $bib_recnum)));
  }
  else {
    drupal_set_message(t("Failed to reimport from Millennium: @error", array("@error" => $result["error"])));
  }
  drupal_goto('node/' . $node->nid);
}

/**
 * Shows realtime holdings information, called via AJAX from a node or teaser view
 */
function millennium_ajax_handler() {
  $nid = intval($_GET['nid']);
  $page = (check_plain($_GET['page']) == "page");
  $data = db_query("SELECT bib_recnum, base_url FROM {millennium_node_bib} WHERE nid = :nid", array(':nid' => $nid))->fetchObject();
  $holdings = millennium_get_holdings_info($data->bib_recnum, $data->base_url);
  $output = theme('millennium_holdings', array('nid' => $nid, 'holdings' => $holdings, 'page' => $page));
  echo $output;
  exit();
}

/**
 * Callback function from hook_menu that shows status of imported items from Millennium
 */
function millennium_admin_status() {
  // Google charts
  $chart_prefix = "http://chart.apis.google.com/chart?cht=ls&chm=o,FF9900,0,-1,5.0&chs=200x100&chdlp=b&chma=10,10,10,10&chd=t:";
  #&chf=bg,s,EFEFEF

  $s[t("Total items imported")] = db_query("SELECT count(id) FROM {millennium_node_bib}")->fetchField();

  // Autocrawl
  $s[t("Crawl activated?")] = ( variable_get('millennium_crawl_flag', 0) ? t('yes') : t('no'));
  // Calculate performance
  $time_history = variable_get("millennium_time_history", array());

  $tot_records = 0;
  $tot_imported = 0;
  $tot_notfound = 0;
  $tot_time = 0;

  if (is_array($time_history) && $events = sizeof($time_history) > 0) {
    #dsm($time_history);
    foreach ($time_history as $event) {
      $date = format_date($event["timestamp"], 'custom', 'Y-m-d');
      $tot_records += $event["items"];
      $tot_imported += $event["imported"];
      $tot_notfound += $event["not_found"];
      $tot_time += $event["time"];
    }
    $s[t("Crawl average performance")] = t("@num records per second", array("@num" => sprintf("%2.2f", $tot_records / $tot_time)));
  }
  $s[t("Crawl latest successful item record")] = variable_get('millennium_webopac_latest_successful_itemrecord', 0);
  $s[t("Crawl latest attempted item record")] = variable_get('millennium_webopac_current_itemrecord', 0);
  $s[t("Crawl current estimated ending item record")] = variable_get('millennium_webopac_end_itemrecord', 0);

  $s[t('Queue: items pending')] = db_query("SELECT count(id) FROM {millennium_import_queue}")->fetchField();

  // Import activity per day
  $result = db_query_range("SELECT count(*) as n, date(created) as day FROM {millennium_node_bib} GROUP BY day ORDER BY day DESC", 0, 10);
  $import_history = "<table><tr><th>Date<th>Nodes created\n";
  $chd = array();
  $chd_min = 9999;
  $chd_max = 0;
  foreach ($result as $d) {
    $import_history .= "<tr><td>" . $d->day . "<td>" . $d->n . "\n";
    $chd_min = ($chd_min > $d->n) ? $d->n : $chd_min;
    $chd_max = ($chd_max < $d->n) ? $d->n : $chd_max;
    $chd[] = $d->n;
  }
  $import_history .= "</table>";
  $name = t('Nodes created in last 10 days');
  $s[$name] = $import_history;
  $chd_min = intval($chd_min);
  $chd_max = intval($chd_max);
  $chd = array_reverse($chd);
  $chart_url = $chart_prefix . implode(",", $chd) . "&chds=$chd_min,$chd_max&chdl=" . t('Nodes created');
  $chart[$name] = "<img src='$chart_url'>";

  // Performance history
  if ($events) {
    $performance_history = "<table><tr><th>Date<th>Records scanned<th>Not found<th>Imported or updated<th>Crawl time (secs)<th>Records/sec<th>Node handling time (secs)\n";
    unset($chd);
    $chd_min = 9999.0;
    $chd_max = 0.0;
    $max_time = 0;
    foreach ($time_history as $event) {
      if ($event["time"] > 0 && $event["items"] > 0) {
        $recs_per_sec = $event["items"] / $event["time"];
        $chd_min = ($chd_min > $recs_per_sec) ? $recs_per_sec : $chd_min;
        $chd_max = ($chd_max < $recs_per_sec) ? $recs_per_sec : $chd_max;
        $max_time = ($max_time < $event["time"]) ? $event["time"] : $max_time;
        $performance_history .= sprintf("<tr><td>%s<td>%d<td>%d<td>%d<td>%2.1f<td>%2.2f<td>%2.1f",
                                        strftime("%c", $event["timestamp"]),
                                        $event["items"],
                                        $event["not_found"],
                                        $event["imported"],
                                        $event["time"],
                                        $recs_per_sec,
                                        $event["time_node"]);
        $chd[] = intval($recs_per_sec * 100);
      }
    }
    $performance_history .= "</table>";
    $name = t('Performance during @num last harvests', array('@num' => MILLENNIUM_PERFORMANCE_HISTORY_SIZE));
    $s[$name] = $performance_history;
    $chd_min = intval($chd_min * 100);
    $chd_max = intval($chd_max * 100);

    $chart_url = $chart_prefix . implode(",", $chd) . "&chds=$chd_min,$chd_max&chdl=" . t('Rec/sec');
    $chart[$name] = "<img src='$chart_url'>";
    $max_execution_time = ini_get('max_execution_time');
    if ($max_time > $max_execution_time * 0.9) {
      $warning = t('<strong>Warning</strong>: the time spent processing records in one or more recent cron runs exceeds the maximum time allowed for PHP scripts.<p>To fix this, you can take either or both of these steps:<ul><li>increase <tt>max_execution_time</tt> in <tt>settings.php</tt> in your Drupal installation (current setting: @time seconds)<li>reduce the amount of records to process during cron runs in the <a href="@config-url">configuration page</a>.</ul>', array('@time' => $max_execution_time, '@config-url' => url(MILLENNIUM_SETTINGS_PATH . '/crawl')));
      drupal_set_message($warning, 'warning');
      #$chart[$name] .= "<div class=warning>";
      #$chart[$name] .=
      #$chart[$name] .= "</div>";
    }

  }

  $output = t('This is an overview of Millennium module performance. You can also go to the !configurationpage.', array('!configurationpage' => l(t('configuration page'), MILLENNIUM_SETTINGS_PATH)));

  foreach ($s as $head => $value) {
    if (isset($chart[$head])) {
      $head .= "<br /><br />" . $chart[$head];
    }

    // Table data
    $rows[] = array(
      "data" => array(
        array('data' => $head, 'header' => TRUE, 'style' => 'width:33%'),
        array('data' => $value),
      )
    );
  }
  $output .= theme('table', array('header' => array(), 'rows' => $rows));
  return $output;
}

/**
 * Diagnostics to show conversion of MARC into fields for node object
 */
function _millennium_show_conversion($recnum, $base_url, $marc_text = FALSE) {
  $empty_element = array();
  $empty_element['#children'] = NULL;
  $empty_element['#collapsible'] = TRUE;
  $empty_element['#collapsed'] = TRUE;

  $output = "";
  $base_url = millennium_get_real_baseurl($base_url);
  if ($base_url == FALSE) {
    return '<div class="msg error">' . t("Millennium data in node has errors and can not be displayed.") . '</div>';
  }

  $marc_text_msg = "";
  if ($marc_text === FALSE) {
    $marc_text = millennium_fetch_marc($recnum, $base_url);
    #$output .= t("Bib recnum = @bib", array("@bib" => $recnum));
    $marc_text_msg = t('This MARC record was just fetched from the WebOPAC.');
  }

  $tmp_node = millennium_record_to_nodeobject($recnum, $base_url, $marc_text);
  if (isset($tmp_node->success) && $tmp_node->success === FALSE) {
    return t("Millennium data in node has errors and can not be displayed.");
  }

  $element = $empty_element;
  $element['#title'] = t('MARC record');
  $element['#value'] = "<pre class='millennium marc conversion-preview'>$marc_text</pre>";
  $output .= theme('fieldset', array('element' => $element));

  $tmp = $tmp_node->millennium_biblio_data;
  ksort($tmp);
  $conv_output = "<pre class='millennium conversion-preview'>" . var_export($tmp, TRUE) . "</pre>";
  $element = $empty_element;
  $element['#title'] = t('Bibliographic data - raw');
  $element['#value'] = $conv_output;
  $output .= theme('fieldset', array('element' => $element));

  $image = theme('millennium_coverimage_widget', array('biblio_data' => $tmp_node->millennium_biblio_data));
  $conv_output = ($image ? $image : t("No image"));
  $conv_output .= $GLOBALS["_millennium_field_labels"]["type"] . ": " . $tmp_node->millennium_biblio_data["type"] . "<br />";
  $conv_output .= theme("millennium_biblio_data", array('biblio_data' => $tmp_node->millennium_biblio_data, 'mode' => 'full'));
  $element = $empty_element;
  $element['#title'] = t('Bibliographic data - themed');
  $element['#value'] = '<div>' . $conv_output . '</div>';
  $output .= theme('fieldset', array('element' => $element));

  // TODO: Show taxonomy mapping results. Maybe this should list any field_* inside the nodeobject.
  /*
  if (isset($tmp_node->taxonomy)) {
    $conv_output = t("You can change taxonomy mappings at the !configurationpage", array("!configurationpage" => l(t('mappings page'), MILLENNIUM_SETTINGS_PATH . '/map')));
    $conv_output .= "<pre class='millennium conversion-preview'>" . var_export($tmp_node->taxonomy, TRUE) . "</pre>";
    $element = $empty_element;
    $element['#title'] = t('Taxonomy mappings');
    $element['#value'] = '<div>' . $conv_output . '</div>';
    $output .= theme('fieldset', array('element' => $element));
  }
  */

  $holdings_info = millennium_get_holdings_info($recnum, $base_url);
  $conv_output = "<pre class='millennium conversion-preview'>" . var_export($holdings_info, TRUE) . "</pre>";
  $element = $empty_element;
  $element['#title'] = t('Holdings table - raw');
  $element['#value'] = '<div>' . $conv_output . '</div>';
  $output .= theme('fieldset', array('element' => $element));

  $element = $empty_element;
  $conv_output = t("You can change holdings table display options at the !configurationpage.", array("!configurationpage" => l(t('display page'), MILLENNIUM_SETTINGS_PATH . '/display')));
  $conv_output .= theme('millennium_holdings', array('nid' => NULL, 'holdings' => $holdings_info, 'page' => TRUE));
  $element['#title'] = t('Holdings table - themed');
  $element['#value'] = '<div>' . $conv_output . '</div>';
  $output .= theme('fieldset', array('element' => $element));

  return $output;
}
