<?php
// $Id: syndication_parser.inc,v 1.9 2008/07/21 11:51:27 aronnovak Exp $

/**
 * @file
 *   Various helper functions for Syndication Parser module
 */

/**
 * Downloads the feed.
 * 
 * @param $url
 *   Downloads the HTTP or HTTPS url.
 */
function syndication_parser_download($url) {
  $prev = cache_get($url);
  $prev = isset($prev->data) ? $prev->data : NULL;
  $headers = array();
  if (is_array($prev)) {
    if (!empty($prev['etag'])) {
      $headers['If-None-Match'] = $prev['etag'];
    }
    if (!empty($prev['modified'])) {
      $headers['If-Modified-Since'] = gmdate('D, d M Y H:i:s', $prev['modified']) .' GMT';
    }
  }
  $result = drupal_http_request($url, $headers);
  if (isset($result->error)) {
    return array('data' => FALSE, 'url' => $url, 'error' => $result->error);
  }
  if ($result->code == 304) {
    return array('data' => $prev['data'], 'url' => $url);
  }
  $feed = _syndication_parser_detect_feed_in_html($result->data, $url);
  if (is_string($feed)) {
    $result = drupal_http_request($feed, $headers);
    $url = $feed;
  }
  $etag = empty($result->headers['ETag']) ? '' : $result->headers['ETag'];
  $modified = empty($result->headers['Last-Modified']) ? 0 : strtotime($result->headers['Last-Modified']);
  cache_set($url, array('data' => $result->data, 'url' => $url, 'etag' => $etag, 'modified' => $modified));
  return array('data' => $result->data, 'url' => $url);
}

/**
 * Detects a feed's format.
 */
function _syndication_parser_format_detect($data) {
  if (is_object($data)) {
    $attr = $data->attributes();
    $type = strtolower($data->getName());
    if (isset($data->entry) || $type == "feed") {
      return "atom";
    }
    if ($type == "rdf" && isset($data->channel)) {
      return "rdf";
    }
    if ($type == "rss" && in_array($attr["version"], array('0.91', "0.92", "2.0"))) {
      return "rss";
    }
  }
  return FALSE;
}

/**
 * Parses RSS 2.0, 0.91, 0.92 feeds.
 */
function _syndication_parser_rss(SimpleXMLElement $data) {
  $feed = new stdClass();
  $dc = $data->channel->children('http://purl.org/dc/elements/1.1/');
  $feed->title = _syndication_parser_choose("{$data->channel->title}", "{$dc->title}");
  $feed->description = _syndication_parser_choose("{$data->channel->description}", "{$dc->subject}");
  $feed->link = isset($data->channel->link) ? "{$data->channel->link}" : "";
  $feed->image = isset($data->channel->image->url) ? "{$data->channel->image->url}" : '';
  $feed->items = array();
  $category_splitter = '.';
  foreach ($data->xpath('//item') as $news) {
    // Get important namespaces.
    $content = $news->children('http://purl.org/rss/1.0/modules/content/');
		$dc = $news->children('http://purl.org/dc/elements/1.1/');
		$item = new stdClass();
		$item->guid = isset($news->guid) ? "{$news->guid}" : NULL;
		$item->title = _syndication_parser_choose("{$news->title}", "{$dc->title}");
		$item->description = _syndication_parser_choose("{$news->description}", "{$news->encoded}", "{$content->encoded}", "{$dc->description}");
		$item->link = _syndication_parser_choose("{$news->link}");
		$item->timestamp = _syndication_parser_parse_date("{$news->pubDate}");
		$item->categories = array();
		if (isset($news->category)) {
			foreach ($news->category as $cat) {
				if (is_object($cat)) {
					$item->categories[] = trim(strip_tags("$cat"));
				}
				else {
					foreach (explode($category_splitter, $cat) as $tag) {
						$item->categories[] = $tag;
					}
				}
			}
		}
		$item->categories = array_unique($item->categories);
		$item->namespaces = _syndication_parser_extract_namespaces($news, $data->getNamespaces(TRUE));
		$item->enclosures = _syndication_parser_extract_enclosures($news);
		$feed->items[] = $item;
  }
  return $feed;
}

/**
 * Parses Atom 1.0 feeds.
 */
function _syndication_parser_atom(SimpleXMLElement $data) {
  $feed = new stdClass();
  $feed->title = isset($data->title) ? "{$data->title}" : "";
  $feed->description = isset($data->subtitle) ? "{$data->subtitle}" : "";
  $feed->link = '';
  if (count($data->link) > 0) {
    $link = $data->link;
    $link = $link->attributes();
    $feed->link = isset($link["href"]) ? "{$link["href"]}" : "";
  }
  $feed->items = array();
  foreach ($data->entry as $news) {
    $item = new stdClass();
    $item->guid = !empty($news->id) ? "{$news->id}" : NULL;
    
    $link_element = "{$news->link}";
    $link_guid = valid_url($item->guid) ? $item->guid : '';
    $item->link = _syndication_parser_choose($link_element, $link_guid);
    $item->title = "{$news->title}";
    $body = '';
    if (!empty($news->content)) {
      foreach ($news->content->children() as $child)  {
        $body .= $child->asXML();
      }
      $body .= "{$news->content}";
    }
    else if (!empty($news->summary)) {
      foreach ($news->summary->children() as $child)  {
        $body .= $child->asXML();
      }
      $body .= "{$news->summary}";
    }
    $item->description = $body;
    $item->timestamp = _syndication_parser_parse_date("{$news->published}");
    $item->categories = array();
    if (isset($news->category)) {
			foreach ($news->category as $category)
				$item->categories[] = trim(strip_tags("{$category['term']}"));
		}
		$item->categories = array_unique($item->categories);
		$item->namespaces = _syndication_parser_extract_namespaces($news, $data->getNamespaces(TRUE));
		$item->enclosures = _syndication_parser_extract_enclosures($news);
    $feed->items[] = $item;
  }
  return $feed;
}

/**
 * Parses RDF feeds.
 */
function _syndication_parser_rdf(SimpleXMLElement $data) {
  $feed = new stdClass();
  $feed->title = isset($data->channel->title) ? "{$data->channel->title}" : "";
  $feed->description = isset($data->channel->description) ? "{$data->channel->description}" : "";
  $feed->link = isset($data->channel->link) ? "{$data->channel->link}" : "";
  $namespaces = $data->getNamespaces(TRUE);
  // Set category splitter (space is for del.icio.us feed).
  $category_splitter = ' ';
  $feed->items = array();
  foreach ($data->item as $news) {
    // Initialization.
    $id = $original_url = NULL;
    $title = $body = '';
    $categories = array();
    foreach ($namespaces as $ns_link) {
      // Get about attribute as guid.
      foreach ($news->attributes($ns_link) as $name => $value) {
        if ($name == 'about') {
          $id = "{$value}";
        }
      }

      // Get children for current namespace.
      if (version_compare(phpversion(), '5.1.2', '<')) {
        $ns = (array) $news;
      }
      else {
        $ns = (array) $news->children($ns_link);
      }

      // Title
      if (!empty($ns['title'])) {
        $title = "{$ns['title']}";
      }

      // Description or dc:description
      if (!empty($ns['description']) && $body == '') {
        $body = "{$ns['description']}";
      }

      // Link
      if (!empty($ns['link'])) {
        $link = "{$ns['link']}";
      }

      // content:encoded
      if (!empty($ns['encoded'])) {
        $body = "{$ns['encoded']}";
      }
      
      $time_in = (empty($ns['pubDate']) ? (empty($ns['date']) ? '' : "{$ns['date']}")  : "{$ns['pubDate']}");
      $timestamp = _syndication_parser_parse_date($time_in);

      // dc:subject
      if (!empty($ns['subject'])) {
        // there can be multiple category tags
        if (is_array($ns['subject'])) {
          foreach ($ns['subject'] as $cat) {
            if (is_object($cat)) {
              $categories[] = trim(strip_tags($cat->asXML()));
            }
            else {
              $categories[] = $cat;
            }
          }
        }
        else { //or single tag
          $categories = explode($category_splitter, "{$ns['subject']}");
        }
      }
    }
    if (empty($original_url) && !empty($id)) {
      $original_url = $id;
    }
    $item = new stdClass();
    $item->title = $title;
    $item->description = $body;
    $item->timestamp = $timestamp;
    $item->link = isset($link) ? $link : '';
    $item->guid = $id;
    $item->categories = $categories;
    $item->namespaces = _syndication_parser_extract_namespaces($news, $data->getNamespaces(TRUE));
    $item->enclosures = _syndication_parser_extract_enclosures($news);
    $feed->items[] = $item;
  }
  return $feed;
}

/**
 * Extracts all the namespace-contained information to ->namespaces structure.
 */
function _syndication_parser_extract_namespaces(SimpleXMLElement $item, $namespaces) {
  $result = array();
  foreach ($namespaces as $prefix => $url) {
    $ns = (array) $item->children($url);
    if (!(empty($ns) || empty($prefix))) {
      $result[$prefix] = $ns;
    }
  }
  return $result;
}

/**
 * Extracts all enclosures inside an item.
 */
function _syndication_parser_extract_enclosures(SimpleXMLElement $item) {
  $result = array();
  @$item = simplexml_load_string($item->asXML());
  $possible_enclosures = $item->xpath("//enclosure") + $item->xpath("//link[@rel='enclosure']");
  foreach ($possible_enclosures as $enc) {
    $add_enc = array();
    foreach ($enc->attributes() as $k => $v) {
      $add_enc[$k] = "{$v}";
    }
    $result[] = $add_enc;
  }
  return $result;
}

/**
 * Chooses the first argument which is not empty and return with it.
 */
function _syndication_parser_choose() {
  $args = func_get_args();
  foreach ($args as $arg) {
    if (strlen($arg) > 1) {
      return $arg;
    }
  }
  return '';
}

/**
 * Parses a date comes from a feed.
 *
 * @param $date_string
 *   The date string in various formats.
 * @return
 *   The timestamp of the string or the current time if can't be parsed
 */
function _syndication_parser_parse_date($date_str) {
  $parsed_date = strtotime($date_str);
  if ($parsed_date === FALSE || $parsed_date == -1) {
    $parsed_date = _syndication_parser_parse_w3cdtf($date_str);
  }
  return $parsed_date === FALSE ? time() : $parsed_date;
}

/**
 * Parses the W3C date/time format, a subset of ISO 8601.
 *
 * PHP date parsing functions do not handle this format.
 * See http://www.w3.org/TR/NOTE-datetime for more information.
 * Originally from MagpieRSS (http://magpierss.sourceforge.net/).
 *
 * @param $date_str
 *   A string with a potentially W3C DTF date.
 * @return
 *   A timestamp if parsed successfully or FALSE if not.
 */
function _syndication_parser_parse_w3cdtf($date_str) {
  if (preg_match('/(\d{4})-(\d{2})-(\d{2})T(\d{2}):(\d{2})(:(\d{2}))?(?:([-+])(\d{2}):?(\d{2})|(Z))?/', $date_str, $match)) {
    list($year, $month, $day, $hours, $minutes, $seconds) = array($match[1], $match[2], $match[3], $match[4], $match[5], $match[6]);
    // Calculate the epoch for current date assuming GMT.
    $epoch = gmmktime($hours, $minutes, $seconds, $month, $day, $year);
    if ($match[10] != 'Z') { // Z is zulu time, aka GMT
      list($tz_mod, $tz_hour, $tz_min) = array($match[8], $match[9], $match[10]);
      // Zero out the variables.
      if (!$tz_hour) {
        $tz_hour = 0;
      }
      if (!$tz_min) {
        $tz_min = 0;
      }
      $offset_secs = (($tz_hour * 60) + $tz_min) * 60;
      // Is timezone ahead of GMT?  If yes, subtract offset.
      if ($tz_mod == '+') {
        $offset_secs *= -1;
      }
      $epoch += $offset_secs;
    }
    return $epoch;
  }
  else {
    return FALSE;
  }
}

/**
 * Detects available feeds in a HTML document.
 * 
 * @param $site
 *   The HTML site.
 * @return
 *   The detected feed URL.
 */
function _syndication_parser_detect_feed_in_html($site, $site_url = FALSE) {
  @$html = DOMDocument::loadHTML($site);
  if ($html) {
    $site_parsed = simplexml_import_dom($html);
    $allowed_mime = array("text/xml", "application/rss+xml", "application/atom+xml", "application/rdf+xml", "application/xml");
    foreach ($site_parsed->xpath("//link") as $element) {
      $rel_words = preg_split("/[\s,]+/", $element['rel']);
      $alternate = FALSE;
      foreach ($rel_words as $word) {
        if (strtolower(trim($word)) == 'alternate') {
          $alternate = TRUE;
          break;
        }
      }
      if ($alternate && in_array(strtolower(trim($element['type'])), $allowed_mime)) {
        $detected_url = trim("{$element['href']}");
        break;
      }
    }
  }
  if (!empty($detected_url)) {
    // If the detected URL is relative, make it absolute.
    $parsed_url = parse_url($detected_url);
    if (!isset($parsed_url['scheme']) || !isset($parsed_url['host'])) {
      $base_element = array_pop($site_parsed->xpath("///head/base"));
      if (strlen("{$base_element['href']}") > 0) {
        $detected_url = "{$base_element['href']}" . $detected_url;
      }
      else {
        $original_url = parse_url($site_url);
        $detected_url = $original_url['scheme'] . '://' . $original_url['host'] . (isset($original_url['port']) ? ':' . $original_url['port'] : '') . $parsed_url['path'] . (isset($original_url['query']) ? '?' . $original_url['query'] : '') . (isset($original_url['fragment']) ? '#' . $original_url['fragment'] : '');
      }
    }
    return $detected_url;
  }
  return FALSE;
}