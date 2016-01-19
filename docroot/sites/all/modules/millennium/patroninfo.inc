<?php

/**
 * @file patroninfo.inc
 *
 * Functions to authenticate against III Millennium OPACs and return information
 * like current checkouts, number of holds, total fines and more.
 *
 * TODOs:
 *  Check that regular expressions work on other systems.
 *  Renew functions not working.
 *  Add support for library messages: /patroninfo/XXXXX/msg
 *  Add support for showing full fines screen: /patroninfo/XXXXX/overdues
 *  Eliminate dependency on CURL library.
 */

/**
 * Regular expressions for screen-scraping
 */

// Error messages in patroninfo screen
// <SPAN ID="ERRORSPAN">Sorry, the information you submitted was invalid. Please try again.</SPAN>
define("PATRONINFO_ERRORMSG_RE", '/<SPAN ID="ERRORSPAN">([^<]+)<\/SPAN>/');

// User's name in patroninfo screen
// <strong>Garza Gonzalez, Alejandro</strong><br>
define("PATRONINFO_NAMES_RE", "/<strong>([A-Z].*?)(, [A-Z].*|)<\/strong><br \/>/i");

// Holds
// <a href="/patroninfo/304550/holds" onClick="return replace_or_redraw( '/patroninfo/304550/holds' )">2 requests (holds). 2 ready for pickup.</a>
define("PATRONINFO_GLOBAL_HOLDS_RE", "/<a href=.(.patroninfo.*?\\/[0-9]+.holds). [^>]*>([0-9]+) requests* \(holds*\)\. *(?:([0-9]+) ready for pickup|)/");

// Fines
//
define("PATRONINFO_FINES_RE", "/patFuncFinesTotalAmt\">.*?\\$([0-9]+\.[0-9]+).*?<\/td>/");


// Due date per item
// <td align="left" class="patFuncStatus"> DUE 02-03-09 +2 HOLDS <span  class="patFuncRenewCount">Renewed 13 times</span>
define("PATRONINFO_ITEM_DUEDATE_RE", "/.*DUE ([0-9]+)-([0-9]+)-([0-9]+).*/i");

// Number of renewals
// <td align="left" class="patFuncStatus"> DUE 02-03-09 +2 HOLDS <span  class="patFuncRenewCount">Renewed 13 times</span>
define("PATRONINFO_ITEM_HOLDS_RE", "/.*\+([0-9]+) HOLDS*.*/i");


/**
 * Wrapper for preg_match().
 */
function patroninfo_scrape($regexp, $html) {
  $match = preg_match($regexp, $html, $matches);
  #print "<PRE class=debug><B>" . htmlspecialchars($regexp) . "</B><br>"; print_r($matches); print "</PRE>";
  #if ($match>0)
    return $matches;
  #else
  #  return FALSE;
}

/**
 * Returns a filename for a temporary (session) cookiejar file
 */
function patroninfo_cookiefilename($session_id) {
  return file_directory_temp() . "/curl_cookiefileName_" . $session_id;
}

/**
 * Cleanup function.
 */
function patroninfo_end_session($session_id) {
  $curl_cookiefile = patroninfo_cookiefilename($session_id);
  @unlink($curl_cookiefile);
}

/**
 * Wrapper for curl_exec() calls, using a cookiejar file.
 */
function patroninfo_request($session_id, $url) {
  global $PI_post_data;

  $max_tries = 4;
  $post_data = $PI_post_data[$session_id];
  $curl_agent = "Mozilla/5.0 (Windows; U; Windows NT 5.0; en-US; rv:1.4) Gecko/20030624 Netscape/7.1 (ax)";
  $curl_cookiefile = patroninfo_cookiefilename($session_id);

  $tries = 0;
  while ( $tries < $max_tries) {
    $tries++;
    #drupal_set_message("Try #$tries: $url, $post_data");
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 1);
    // This disables strict SSL certificate checking.
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
    curl_setopt($ch, CURLOPT_USERAGENT, $curl_agent);
    curl_setopt($ch, CURLOPT_COOKIEJAR, $curl_cookiefile);
    curl_setopt($ch, CURLOPT_COOKIEFILE, $curl_cookiefile);
    curl_setopt($ch, CURLOPT_URL, $url);
    if ($post_data) {
      curl_setopt($ch, CURLOPT_POST, 1);
      curl_setopt($ch, CURLOPT_POSTFIELDS, $post_data);
    }
    ob_start(); // prevent any output
    curl_exec($ch); // execute the curl command
    $response = ob_get_contents();
    ob_end_clean(); // stop preventing output
    $errormsg = curl_error($ch);
    $errno = curl_errno($ch);
    curl_close($ch);

    if ($errno == 0) {
      break;
    }
    sleep(1);
  }

  if ($errno != 0) {
    return array(
      "success" => FALSE,
      "errormsg" => t("patroninfo_request() error: (@errno) @errormsg", array("@errno" => $errno, "@errormsg" => $errormsg))
    );
  }

  if (preg_match('/patActionsLinks|patStatusLinks|patnavRow/', $response)) {
    return array(
      "success" => TRUE,
      "response" => $response
    );
  }
  else {
    return array(
      "success" => FALSE,
      "errormsg" => t("Fatal error: could not find required string in response.")
    );
  }
}

/**
 * Begins an authenticated user session with Millennium through http.
 */
function patroninfo_start_session($millennium_baseurl, $name, $code, $pin, $date_format = 'MDY') {
  global $PI_post_data;

  // Generate a new sessionid (random number)
  $session_id = rand(100000, 999999);
  register_shutdown_function('patroninfo_end_session', $session_id);

  // Store name/code/pin for future callse
  $post_data = "name={$name}&code={$code}&pin={$pin}";
  $PI_post_data[$session_id] = $post_data;
  $login_page = $millennium_baseurl . "/patroninfo*eng";

  // Conect to login page.
  $r = patroninfo_request($session_id, $login_page);
  if ($r["success"] === FALSE) {
    return array(
      "success" => FALSE,
      "errormsg" => t("Connection error: @errormsg", array("@errormsg" => $r["errormsg"]))
    );
  }
  $fetched_html = $r["response"];

  // Check for any error messages inside page HTML.
  $matches = patroninfo_scrape(PATRONINFO_ERRORMSG_RE, $fetched_html);
  if (sizeof($matches) > 0) {
    return array(
      "success" => FALSE,
      "errormsg" => t("Authentication error: @error", array("@error" => trim($matches[1])))
    );
  }

  $patroninfo_data["session_id"] = $session_id;

  $ok = preg_match("/<a href=\"\/patroninfo.*?\/([0-9]+)\/(top|holds|items|overdues)\/\">/", $fetched_html, $matches);
  if (!$ok) {
    return array(
      "success" => FALSE,
      "errormsg" => t("Could not get URL for patron information")
    );
  }
  $id = $matches[1];
  $url_checkouts = "$login_page/$id/items";
  $url_holds = "$login_page/$id/sortedholds";
  $url_fines = "$login_page/$id/overdues";

  // Grab the checked out item list; the URL for that screen is in $url_checkouts
  $r = patroninfo_request($session_id, $url_checkouts);
  if ($r["success"] === FALSE) {
    return array(
      "success" => FALSE,
      "errormsg" => t("Connection error: @errormsg", array("@errormsg" => $r["errormsg"]))
    );
  }
  $fetched_html = $r["response"];
  // Scrape checked-out items screen.
  $items = patroninfo_scrape_items($fetched_html, $date_format);
  $patroninfo_data["checkouts"] = $items;
  // Scrape first, last names from current screen.
  $matches = patroninfo_scrape(PATRONINFO_NAMES_RE, $fetched_html);
  if (sizeof($matches) > 0) {
    $patroninfo_data["lastname"] = $matches[1];
    $patroninfo_data["firstname"] = str_replace(", ", "", $matches[2]);
  }


  // Get global holds
  $r = patroninfo_request($session_id, $url_holds);
  $fetched_html = $r["response"];
  $matches = patroninfo_scrape(PATRONINFO_GLOBAL_HOLDS_RE, $fetched_html);
  if (sizeof($matches) > 0) {
    $patroninfo_data["number_holds"] = intval($matches[2]);
    $patroninfo_data["number_holds_ready"] = intval($matches[3]);
    $url_holds = $millennium_baseurl . $matches[1];
  }

  // Get fines
  $r = patroninfo_request($session_id, $url_fines);
  $fetched_html = $r["response"];
  $matches = patroninfo_scrape(PATRONINFO_FINES_RE, $fetched_html);
  if (sizeof($matches) > 0) {
    $patroninfo_data["fines"] = $matches[1];
  }
  else {
    $patroninfo_data["fines"] = 0;
  }

  // Complete! Return the data
  $patroninfo_data["success"] = TRUE;
  return $patroninfo_data;
}

/**
 * Returns an array of item; each item is an array containing the item_recnum, title, barcode, status, message, fine, callno and duedate_timestamp
 */
function patroninfo_scrape_items($fetched_html, $date_format) {

  /* SAMPLE items screen:
  <table border="0" class="patFunc">
  <tr class="patFuncTitle">
  <td colspan="5" align="center" class="patFuncTitle">
  <strong>11 ITEMS CHECKED OUT</strong></td>
  </tr>

  <tr class="patFuncHeaders">
  <th class="patFuncHeaders"> RENEW </th><th class="patFuncHeaders"> TITLE </th><th  class="patFuncHeaders"> BARCODE </th><th class="patFuncHeaders"> STATUS </th><th  class="patFuncHeaders"> CALL NUMBER </th>
  <tr class="patFuncEntry"><td align="left" class="patFuncMark"><input type="checkbox" name="renew0" value="i3017950" /></td><td align="left" class="patFuncTitle"><a href="/patroninfo/120796/item&1105815"> PHP 5 recipes : a problem-solution approach / Lee  </a>
  <br />

  </td>
  <td align="left" class="patFuncBarcode"> 30002007031271 </td>
  <td align="left" class="patFuncStatus"> DUE 30-03-09 <br /><font color="red">FINE(up to now) $5.00</font> <span  class="patFuncRenewCount">Renewed 15 times</span>
  </td>
  <td align="left" class="patFuncCallNo"> QA76.73.P224 P455 2005  </td>
  </tr>
  <tr class="patFuncEntry"><td align="left" class="patFuncMark"><input type="checkbox" name="renew1" value="i3017951" /></td><td align="left" class="patFuncTitle"><a href="/patroninfo/120796/item&1103072"> Information architecture for the World Wide Web /  </a>

  <br />

  </td>
  <td align="left" class="patFuncBarcode"> 30002007031263 </td>
<td align="left" class="patFuncStatus"> DUE 30-03-09 <br /><font color="red">FINE(up to now) $5.00</font> <span  class="patFuncRenewCount">Renewed 15 times</span>
</td>
<td align="left" class="patFuncCallNo"> TK5105.888 .R67 2007  </td>
</tr>

<tr class="patFuncEntry"><td align="left" class="patFuncMark"><input type="checkbox" name="renew2" value="i2886489" /></td><td align="left" class="patFuncTitle"><a href="/patroninfo/120796/item&1112077"> Pro Drupal development / John K. VanDyk and Matt W </a>
<br />

</td>
<td align="left" class="patFuncBarcode"> 30002006932941 </td>
<td align="left" class="patFuncStatus"> DUE 30-03-09 <br /><font color="red">FINE(up to now) $5.00</font> <span  class="patFuncRenewCount">Renewed 7 times</span>
</td>
<td align="left" class="patFuncCallNo"> TK5105.888 .V35 2007  </td>

</tr>
<tr class="patFuncEntry"><td align="left" class="patFuncMark"><input type="checkbox" name="renew3" value="i3008568" /></td><td align="left" class="patFuncTitle"><a href="/patroninfo/120796/item&1134199"> Free prize inside! : how to make a purple cow / Se </a>
<br />

</td>
<td align="left" class="patFuncBarcode"> 30002007021397 </td>
<td align="left" class="patFuncStatus"> DUE 30-03-09 <br /><font color="red">FINE(up to now) $5.00</font> <span  class="patFuncRenewCount">Renewed 6 times</span>
</td>

<td align="left" class="patFuncCallNo"> HF5415 .G577 2007  </td>
</tr>
<tr class="patFuncEntry"><td align="left" class="patFuncMark"><input type="checkbox" name="renew4" value="i3163218" /></td><td align="left" class="patFuncTitle"><a href="/patroninfo/120796/item&1177514"> Estudio exploratorio acerca del nivel de satisfacc </a>
<br />

</td>
<td align="left" class="patFuncBarcode"> 30002007139439 </td>
<td align="left" class="patFuncStatus"> DUE 30-03-09 <br /><font color="red">FINE(up to now) $5.00</font> <span  class="patFuncRenewCount">Renewed 6 times</span>

</td>
<td align="left" class="patFuncCallNo"> LC5803.C65 G84 2008  </td>
</tr>
<tr class="patFuncEntry"><td align="left" class="patFuncMark"><input type="checkbox" name="renew5" value="i3163584" /></td><td align="left" class="patFuncTitle"><a href="/patroninfo/120796/item&1155562"> Handbook of online education / Shirley Bennett ; w </a>
<br />

</td>
<td align="left" class="patFuncBarcode"> 30002007082985 </td>
<td align="left" class="patFuncStatus"> DUE 30-03-09 <br /><font color="red">FINE(up to now) $5.00</font> <span  class="patFuncRenewCount">Renewed 1 time</span>

</td>
<td align="left" class="patFuncCallNo"> LB1044.87 .B44 2007  </td>
</tr>
<tr class="patFuncEntry"><td align="left" class="patFuncMark"><input type="checkbox" name="renew6" value="i3287656" /></td><td align="left" class="patFuncTitle"><a href="/patroninfo/120796/item&1187469"> Drupal for education and e-learning : teaching and </a>
<br />

</td>
<td align="left" class="patFuncBarcode"> 30002007191208 </td>
<td align="left" class="patFuncStatus"> DUE 14-04-09
</td>

<td align="left" class="patFuncCallNo"> TK5105.8885.D78 F58 2008  </td>
</tr>
<tr class="patFuncEntry"><td align="left" class="patFuncMark"><input type="checkbox" name="renew7" value="i3317628" /></td><td align="left" class="patFuncTitle"><a href="/patroninfo/120796/item&1201313"> Using Drupal / Angela Byron ... [et al.] </a>
<br />

</td>
<td align="left" class="patFuncBarcode"> 30002007199110 </td>
<td align="left" class="patFuncStatus"> DUE 14-04-09
</td>
<td align="left" class="patFuncCallNo"> TK5105.8885.D78 U84 2009  </td>

</tr>
<tr class="patFuncEntry"><td align="left" class="patFuncMark"><input type="checkbox" name="renew8" value="i539478" /></td><td align="left" class="patFuncTitle"><a href="/patroninfo/120796/item&353358"> Sony Music, 100 years [sound recording] : soundtra </a>
<br />

</td>
<td align="left" class="patFuncBarcode"> 60002000347438 </td>
<td align="left" class="patFuncStatus"> DUE 03-04-09
</td>
<td align="left" class="patFuncCallNo"> 68601 v.1 disco 1 </td>
</tr>

<tr class="patFuncEntry"><td align="left" class="patFuncMark"><input type="checkbox" name="renew9" value="i191651" /></td><td align="left" class="patFuncTitle"><a href="/patroninfo/120796/item&182750"> Best of George Benson </a>
<br />

</td>
<td align="left" class="patFuncBarcode"> 60002000208010 </td>
<td align="left" class="patFuncStatus"> DUE 03-04-09
</td>
<td align="left" class="patFuncCallNo"> 20801  </td>
</tr>
<tr class="patFuncEntry"><td align="left" class="patFuncMark"><input type="checkbox" name="renew10" value="i191620" /></td><td align="left" class="patFuncTitle"><a href="/patroninfo/120796/item&182723"> The best of Mike Olfield [grabaci�n sonora]: eleme </a>

<br />

</td>
<td align="left" class="patFuncBarcode"> 60002000190010 </td>
<td align="left" class="patFuncStatus"> DUE 03-04-09
</td>
<td align="left" class="patFuncCallNo"> 19001  </td>
</tr>
</table>

  */

  $items = array();

  /**
   * Sample HTML for one item:
   *
   * <tr class="patFuncEntry"><td align="left" class="patFuncMark"><input type="checkbox" name="renew4" value="i3163218" /></td><td align="left" class="patFuncTitle"><a href="/patroninfo/120796/item&1177514"> Estudio exploratorio acerca del nivel de satisfacc </a>
        <br />

        </td>
        <td align="left" class="patFuncBarcode"> 30002007139439 </td>
        <td align="left" class="patFuncStatus"> DUE 30-03-09 <br /><font color="red">FINE(up to now) $5.00</font> <span  class="patFuncRenewCount">Renewed 6 times</span>

        </td>
        <td align="left" class="patFuncCallNo"> LC5803.C65 G84 2008  </td>
        </tr>
   *
   * Fines are optionally shown like this:
   *
   *    <td align="left" class="patFuncStatus"> Prestado hasta 03-05-06 <br><font color="red">CARGO/MULTA(hasta ahora) $10.00</font>
   */
  $re = "/<tr class=.patFuncEntry.>";
  $re .= ".*? value=\"(?:cancel|)(i[0-9]{5,10}x*).*?\""; //1
  $re .= ".*?patFuncTitle[^>]+>(?:.*?record=(b[0-9]+)|).*?>([^<]+)<"; //2, 3
  $re .= "(?:.*?patFuncBarcode[^>]+>([^<]+)<|)"; //3
  $re .= ".*?patFuncStatus[^>]+>([^<]+)(?:.*?\\\$([0-9.]+)|)"; //4, 5
  $re .= "(?:.*?patFuncRenewCount[^>]+>.*?Renewed ([0-9]+) time.*?|)"; //6
  $re .= ".*?patFuncCallNo[^>]+>([^<]+)."; //7
  $re .= "*?<.tr>/s";

  $result = preg_match_all($re, $fetched_html, $matches, PREG_SET_ORDER);
  if ($matches) {
    foreach ($matches as $match) {
      $item = array();
      /*
        Array
        (
            [0] => [... lots of html...]
            [1] => i3163218
            [2] => b123456
            [3] =>  Estudio exploratorio acerca del nivel de satisfacc
            [4] =>  30002007139439
            [5] =>  DUE 30-03-09
            [6] => 5.00
            [7] => 6
            [8] =>  LC5803.C65 G84 2008
        )
      */
      #print_r($match);
      $item["item_recnum"] = $match[1];
      $item["bib_recnum"] = $match[2];
      $item["title"] = trim(utf8_encode($match[3]));
      $item["barcode"] = trim($match[4]);
      $item["status"] = trim(utf8_encode($match[5]));
      $item["fine"] = (float) $match[6];
      $item["renew_count"] = trim(utf8_encode($match[7]));
      $item["callno"] = trim($match[8]);
      $item["duedate_timestamp"] = patroninfo_itemstatus_due_to_timestamp($item["status"], $date_format);

      // Get holds per item
      // DUE 02-03-09 +2 HOLDS
      if (preg_match(PATRONINFO_ITEM_HOLDS_RE, $item["status"], $matches)) {
        $item["hold_count"] = intval($matches[1]);
      }
      $items[] = $item;
    }

    $items = patroninfo_items_duedate_sort($items);
  }

  return $items;
}

/**
 * Renew all items for the user specified in the $patroninfo array
 */
/*
function patroninfo_renewall($patroninfo) {
  $session_id=$patroninfo["session_id"];
  $url="http://millenium.itesm.mx/patroninfo*eng/120796/items?renewall";
  $r = patroninfo_request($session_id, $url);
  $fetched_html=$r["response"];

  $items = patroninfo_scrape_items($fetched_html);

  #$patroninfo_data["checkouts"]=$items;

  if ($r["success"]===FALSE) {
    #echo "<PRE STYLE=color:red>$errormsg</PRE>";
    return FALSE;
  }
  return $items;
}
*/

/**
 * Renew one item for the patron specified by the $patroninfo array and item number in $itemrecnum
 */
/*
function patroninfo_renewitem($patroninfo, $itemrecnum) {
  #http://millenium.itesm.mx/patroninfo*eng/120796/items?renewsome=TRUE&renew15=i2582571
  #/patroninfo*spi/120796/items?renewsome=TRUE&renew0=i2523762


  #OK en firefox
  #http://millenium.itesm.mx/patroninfo/120796/items?renewsome=TRUE&renew0=i2523762

  $session_id=$patroninfo["session_id"];
  $curl_agent="Mozilla/5.0 (Windows; U; Windows NT 5.0; en-US; rv:1.4) Gecko/20030624 Netscape/7.1 (ax)";
  $curl_cookiefile="/tmp/biblio/curl_cookiefileName_".$session_id;
  #echo $curl_cookiefile;
  $url="http://millenium.itesm.mx/patroninfo/120796/items?renewsome=TRUE&renew0=".$itemrecnum;
  #echo $url;
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 1);
  curl_setopt($ch, CURLOPT_USERAGENT, $curl_agent);
  curl_setopt($ch, CURLOPT_COOKIEJAR, $curl_cookiefile);
  curl_setopt($ch, CURLOPT_COOKIEFILE, $curl_cookiefile);
  curl_setopt($ch, CURLOPT_URL, $url);
  ob_start();      // prevent any output
  curl_exec($ch);  // execute the curl command
  $response = ob_get_contents();
  ob_end_clean();  // stop preventing output
  $errormsg=curl_error($ch);
  curl_close($ch);
  if ($errormsg) {
    #echo "<PRE STYLE=color:red>$errormsg</PRE>";
    return FALSE;
  }
  #echo "<PRE STYLE=overflow:auto;height:200px;>".htmlspecialchars($response)."</PRE>";
  if (strpos($response, "RENOVADO</b><br>")) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}
*/

/**
 * Helper function that converts the III due date to a unix timestamp
 */
function patroninfo_itemstatus_due_to_timestamp($itemstatus, $date_format = 'MDY') {
  $m = patroninfo_scrape(PATRONINFO_ITEM_DUEDATE_RE, $itemstatus);
  if ($m == FALSE) {
    return FALSE;
  }

  // Make timestamp
  // int mktime  ([ int $hour  [, int $minute  [, int $second  [, int $month  [, int $day  [, int $year  [, int $is_dst  ]]]]]]] )
  if ($date_format == 'MDY') {
    $ts = mktime(0, 0, 0, $m[1], $m[2], "20" . $m[3]);
  }
  else {
    $ts = mktime(0, 0, 0, $m[2], $m[1], "20" . $m[3]);
  }
  return $ts;
}

/**
 * Helper function to sort the items array returned by patroninfo_scrape_items() by duedate timestamp
 */
function patroninfo_items_duedate_sort($items) {
  static $firstrun = TRUE;
  if ($firstrun) {
    function _cmp($a, $b) {
      return ($a["duedate_timestamp"] < $b["duedate_timestamp"]) ? -1 : 1;
    }
    $firstrun = FALSE;
  }
  if (is_array($items)) {
    usort($items, "_cmp");
  }
  return $items;
}
