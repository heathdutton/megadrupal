<?php

/**
 * @file
 * The callback file for the "Sticky Sharrre Bar" module.
 */

/**
 * Emulation functional of php file from http://sharrre.com.
 */
function sticky_sharrre_bar_handler() {
  $url = $_GET['url'];
  $type = $_GET['type'];
  $output = array('url' => $url, 'count' => 0);

  drupal_page_is_cacheable(FALSE);

  if (filter_var($url, FILTER_VALIDATE_URL)) {
    if ($type == 'googlePlus') {
      $data = _sticky_sharrre_bar_parse('https://plusone.google.com/u/0/_/+1/fastbutton?url=' . $url . '&count=true');

      $dom = new DOMDocument();
      $dom->preserveWhiteSpace = FALSE;
      @$dom->loadHTML($data);
      $dom_xpath = new DOMXPath($dom);
      $new_dom = new DOMDocument();
      $new_dom->formatOutput = TRUE;

      $filtered = $dom_xpath->query('//div[@id="aggregateCount"]');
      if (isset($filtered->item(0)->nodeValue)) {
        $output['count'] = $filtered->item(0)->nodeValue;
      }
    }
    else {
      if ($type == 'stumbleupon') {
        $result = _sticky_sharrre_bar_parse('http://www.stumbleupon.com/services/1.01/badge.getinfo?url=' . $url);
        $result = json_decode($result);

        if (isset($result->result->views)) {
          $output['count'] = $result->result->views;
        }
      }
    }
  }

  return $output;
}

/**
 * Get necessary content use cURL.
 *
 * @param string $encoded_url
 *   Url of page.
 *
 * @return string
 *   Ready data
 */
function _sticky_sharrre_bar_parse($encoded_url) {
  $options = array(
    CURLOPT_RETURNTRANSFER => TRUE,
    CURLOPT_HEADER => FALSE,
    CURLOPT_FOLLOWLOCATION => TRUE,
    CURLOPT_ENCODING => '',
    CURLOPT_USERAGENT => 'sharrre',
    CURLOPT_AUTOREFERER => TRUE,
    CURLOPT_CONNECTTIMEOUT => 5,
    CURLOPT_TIMEOUT => 10,
    CURLOPT_MAXREDIRS => 3,
    CURLOPT_SSL_VERIFYHOST => 0,
    CURLOPT_SSL_VERIFYPEER => FALSE,
  );
  $curl = curl_init();

  $options[CURLOPT_URL] = $encoded_url;
  curl_setopt_array($curl, $options);
  $data = curl_exec($curl);

  curl_close($curl);

  return $data;
}
