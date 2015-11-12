<?php

/**
 * @file
 * Contains all theming functions.
 */

/**
 * Theme the holdings widget.
 *
 * @param $variables
 *   An associative array containing:
 *   - nid: a node id.
 *   - holdings: a holdings array provided by millennium_get_holdings_info()
 *   - page: a boolean value indicating if the current view mode is 'full'.
 * 
 * @ingroup themeable
 * @see millennium_get_holdings_info()
 */
function theme_millennium_holdings($variables) {
  $nid = $variables['nid'];
  $holdings = $variables['holdings'];
  $full = $variables['page'];
  static $re;

  $biblio_data = millennium_get_biblio_data($nid);

  // Preferred locations
  if (empty($re)) {
    $tmp = variable_get('millennium_preferred_locations', '');
    $re = "/^(" . str_replace(",", "|", $tmp) . ")/i";
  }

  /*
  $holdings = (
    [0] => Array
          (
              [location] => MTY GENERAL
              [classnumber] =>
              [message] =>
              [status] => CATALOGACION
          )

      [1] => Array
          (
              [location] => MTY GENERAL
              [classnumber] => c.2
              [message] =>
              [status] => CATALOGACION
              [link] => array (
                [url] => http://.....
                [title] => Click here for table of contents...
                )
          )

  )
  */

  $shown = 0;
  $more = 0;
  $rows = array();
  if (is_array($holdings)) {
    // Sort items.
    usort($holdings, "millennium_holdings_sort");

    foreach ($holdings as $item) {
      // If requested from teaser view
      if (! $full) {
        if ($re != "") {
          if (! preg_match($re, $item["location"])) {
            $more++;
            continue;
          }
        }
      }

      // Style differently if items are "available" according to status and
      // millennium_availability_strings variable
      $class = "notavailable";
      $availability_strings = explode(',', variable_get('millennium_availability_strings', MILLENNIUM_ITEM_AVAILABLE_STRINGS));
      if (in_array($item['status'], $availability_strings)) {
        $class = "available";
      }
      $shown++;
      $classnumber_volume_copy = $item['classnumber'];
      if ($item['classvolume']) {
        $classnumber_volume_copy .= ($classnumber_volume_copy ? " " : "") . $item['classvolume'];
      }
      if ($item['copy']) {
        $classnumber_volume_copy .= ($classnumber_volume_copy ? " " : "") . $item['copy'];
      }
      // Location
      $location = $item['location'];
      if ($biblio_data && variable_get('millennium_holdings_opacname', 1)) {
        // Add opacname prefix to $location
        $sources = variable_get('millennium_sources', array());
        $source = $sources[$biblio_data["base_url"]];
        if ($source && $source['name']) {
          $location = $source['name'] . ": " . $location;
        }
      }
      $rows[] = array(
        'data' => array(
          array('data' => $item['status']),
          array('data' => $location),
          array('data' => $classnumber_volume_copy),
          //array('data' => $item['message']),
        ),
        'class' => array("holdings", $class),
      );
      if (isset($item['link']) && $item['link']) {
        $link = l($item['link']['title'], $item['link']['url']);
        $rows[] = array(
          'data' => array(
            array('data' => $link, 'colspan' => '3'),
          ),
          'class' => array('holdings', 'link'),
        );
      }
    }
  }

  // Extra messages
  $extra = '';
  if ($shown == 0) {
    if ($more == 0) {
      $extra = t('No items are currently available.');
    }
    else {
      $extra = t('Items available only through interlibrary loan. !url',
        array('!url' => l(t('View more items'), "node/{$nid}"))
      );
      // TODO: Links to more actions? Request/Buy another copy/etc.
    }
  }
  else {
    if ($more > 0) {
      $extra = l(t('View more items'), "node/{$nid}");
    }
  }
  if ($extra) {
    $rows[] = array(
      'data' => array(
        array('data' => $extra, 'colspan' => '3'),
      ),
      //'class' => 'holdings message'
    );
  }

  if ($rows) {
    $output = "<div class='millennium holdings'>";
    $table_attributes = array('class' => array("millennium holdings"));
    $table_headers = array(t('Availability'), t('Location'), t('Call Number'));
    $output .= theme('table', array('header' => $table_headers, 'rows' => $rows, 'attributes' => $table_attributes));
    $output .= '</div>';
  }
  else {
    $output = '';
  }
  return $output;
}

/**
 * Sort function for holdings widget
 *
 * @see theme_millennium_holdings()
 */
function millennium_holdings_sort($a, $b) {
  static $re;

  // Preferred locations
  if (empty($re)) {
    $tmp = variable_get('millennium_preferred_locations', '');
    $re = "/^(" . str_replace(",", "|", $tmp) . ")/i";
  }

  if ($re == "") {
    return strcmp($a["location"], $b["location"]);
  }

  if (preg_match($re, $a["location"])) {
    return -1;
  }
  else {
    return strcmp($a["location"], $b["location"]);
  }
}

/**
 * Theme a table with stored bibliographic information
 *
 * @param $variables
 *   An associative array containing:
 *   - biblio_data: The millennium_biblio_data array added in
 *       millennium_node_load().
 *   - mode: the view mode: can be 'full' or 'teaser'.
 *
 * @ingroup themeable
 * @see millennium_node_load()
 */
function theme_millennium_biblio_data($variables) {
  $biblio_data = $variables['biblio_data'];
  $mode = $variables['mode'];
  $rows = array();
  global $_millennium_field_labels;

  // Values to show depending on $mode argument
  // Available fields:
  //   title,type,alternate_title,short_title,translated_title,biblio_type,authors,corp_author,secondary_authors,edition,imprint,imprint_name,imprint_place,imprint_date,isbn,issn,lang,url,item_recnum,notes,item_description,links
  $map = array(
    'full' => 'title,type,alternate_title,translated_title,authors,edition,imprint,series,isbn,issn,other_number,volume,pages,lang,url,notes,item_description,links',
    'teaser' => 'series,translated_title,edition,imprint,lang',
  );

  $translateable_fields = array('type', 'lang');
  if (!isset($map[$mode])) {
    return '';
  }

  $content = "";
  foreach (explode(",", $map[$mode]) as $fieldname) {
    // Skip non-existing values.
    if (!isset($biblio_data[$fieldname])) {
      continue;
    }
    $display = "";
    if (is_string($biblio_data[$fieldname])) {
      $fieldvalue = trim($biblio_data[$fieldname]);
      if ($fieldvalue == "") {
        continue;
      }
    }
    else {
      $fieldvalue = $biblio_data[$fieldname];
    }

    if (in_array($fieldname, $translateable_fields)) {
      $fieldvalue = _millennium_human_string($biblio_data, $fieldname);
    }

    $callback = "millennium_display_field_{$fieldname}";
    if (function_exists($callback)) {
      $display = $callback($fieldvalue);
    }
    else {
      if (is_string($fieldvalue)) {
        $display = $fieldvalue;
      }
      elseif (is_array($fieldvalue) && sizeof($fieldvalue) > 0) {
        // Special case: array of one item
        if (sizeof($fieldvalue) == 1) {
          $display =  $fieldvalue[0];
        }
        else {
          $display = theme('item_list', array('items' => $fieldvalue));
        }
      }
    }

    if ($display == "") {
      continue;
    }
    if (empty($_millennium_field_labels[$fieldname])) {
      continue;
    }

    $rows[] = array(
      'data' => array(
        array('data' => $_millennium_field_labels[$fieldname], 'class' => array('fieldname')),
        array('data' => $display, 'class' => array('fieldvalue')),
      )
    );
  }
  $content = theme('table', array('header' => array(), 'rows' => $rows, 'attributes' => array('class' => array('millennium', 'biblio_data'))));
  return $content;
}

/**
 * Returns html for a cover image based on the isbn or issn from a biblio type node
 *
 * @param $variables
 *   An associative array containing:
 *   - biblio_data: The millennium_biblio_data array added in
 *       millennium_node_load().
 *   - nid: optional, a node ID to link to.
 *
 * @ingroup themeable
 * @see millennium_node_load()
 */
function theme_millennium_coverimage_widget($variables) {
  $biblio_data = $variables['biblio_data'];
  $nid = $variables['nid'];
  // TODO: Add CSS for coverimages in millennium_init()
  // TODO: Move wrapping-coverimages-in-div code from millennium_nodeapi to this function
  static $blank_url;
  if (empty($blank_url)) {
    $blank_url = url(drupal_get_path('module', 'millennium') . '/images/nocover.png');
  }
  $override_url = variable_get("millennium_coverimage_baseurl", FALSE);
  $cover_url = "";
  $title = str_replace('"', '\"', check_plain($biblio_data['title']));
  if ($override_url) {
    // Determine single isbn (or issn) to use for !id placeholder
    $first_isbn_issn = (!empty($biblio_data["isbn"][0])) ? $biblio_data["isbn"][0] : '';
    if ($first_isbn_issn == '') {
      $first_isbn_issn = (!empty($biblio_data["issn"][0])) ? $biblio_data["issn"][0] : '';
    }
    $cover_url = $override_url;
    // Replace placeholders with item's information from $biblio_data
    $cover_url = str_replace("!id", $first_isbn_issn, $cover_url);
    $cover_url = str_replace("!type", $biblio_data["type"], $cover_url);
    $cover_url = str_replace("!tit", urlencode($biblio_data['title']), $cover_url);
    if (isset($biblio_data["authors"])) {
      $cover_url = str_replace("!aut", urlencode(implode(';', $biblio_data["authors"])), $cover_url);
    }
    // If nothing was replaced, use blank cover's image url
    if ($cover_url == str_replace(array('!id', '!type', '!tit', '!aut'), '', $override_url)) {
      $cover_url = $blank_url;
    }
  }
  elseif (isset($biblio_data["coverimage_sources"])) {
    // No cover image override URL configured:
    // Try other sources in biblio information
    $cover_sources = $biblio_data["coverimage_sources"];
    if (is_array($cover_sources)) {
      $cover_info = array_pop($cover_sources);
      $cover_url = $cover_info[1];
    }
  }

  if ($cover_url) {
    $image = "<img src=\"{$cover_url}\" class=\"isbn-{$first_isbn_issn}\" />";
    // If $nid argument given, wrap image around <a> tag
    if ($nid) {
      $image = '<a href="' . url('node/' . $nid) . '" title="' . $title . '">' . $image . '</a>';
    }
    return "<div class='millennium coverimage'>" . $image . "</div>";
  }
  else {
    // No image url could be resolved; return empty string
    return "";
  }
}
