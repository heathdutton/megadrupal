<?php
// $Id$
/*
 * Drupal Most Popular - Showcase the most popular content across your Drupal website and engage your audience.
 * Copyright © 2009-2012 New Signature
 * 
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * You can contact New Signature by electronic mail at labs@newsignature.com -or- by U.S. Postal Service at 1100 H St. NW, Suite 940, Washington, DC 20005.
 */

/**
 * @file
 * Provides a connector to the AddThis.com API.
 *
 * See http://www.addthis.com/help/analytics-api
 */
class MostPopularAddThis {
  const data_scheme = 'https://';
  const data_url = 'api.addthis.com/analytics/1.0/pub/shares';

  protected $username;
  protected $password;
  protected $pubid;
  protected $domain;
  protected $result;

  /**
   * Constructs a new AddThis class.
   */
  public function MostPopularAddThis($username, $password, $pubid, $domain) {
    $this->username = $username;
    $this->password = $password;
    $this->pubid = $pubid;
    $this->domain = $domain;
  }

  /**
   * Makes a generic call to the AddThis.com server.
   *
   * See http://www.addthis.com/help/analytics-api
   *
   * @param string $dimension
   *   The dimension we're interested in.  It must be null or one of:
   *   - url
   *   - continent
   *   - country
   *   - domain
   *   - service
   * @param string $format
   *   The format to return results, either 'csv' or 'json'.
   * @param string $params
   *   Additional parameters to send.  The following are allowed:
   *   - period: 'day', 'week' or 'month'
   *   - domain: any URL domain
   *   - service: 'email'
   */
  public function fetch($dimension = '', $format = 'csv', $params = array()) {
    if (empty($this->username) || empty($this->password) || empty($this->pubid)) {
      drupal_set_message(t('You must configure the AddThis.com username, password, and publisher profile id.'), 'error');
      return FALSE;
    }

    // Create the URL
    $params += array(
      'userid' => $this->username,
      'password' => $this->password,
      'pubid' => $this->pubid,
      'domain' => $this->domain,
    );

    $url = self::data_url;
    if (!empty($dimension)) {
      $url .= '/' . $dimension;
    }
    $url .= '.' . $format;

    // Clean up the URL and add query parameters
    $request_url = url(self::data_scheme . $url,
      array( 'query' => $params ));

    $this->result = drupal_http_request($request_url);
    
    if (variable_get('mostpopular_debug', FALSE)) {
      watchdog('mostpopular_addthis', 'Request to !url<pre>!response</pre>', array(
        '!url' => $request_url,
        '!response' => print_r($this->result, TRUE),
      ), WATCHDOG_DEBUG);
    }

    if (isset($this->result->error)) {
      $msg = '';
      // Parse the error to figure out what happened.
      if (preg_match("/<b>message<\/b> <u>(.+?)<\/u>/", $this->result->data, $matches)) {
        $msg = $matches[1];
      }
      
      drupal_set_message(t(
"Error connecting to AddThis.com service.<br/>
!url<br/>
@code @error %msg", array(
          '!url' => url(self::data_scheme . $url, array( 'query' => $params )),
          '@code' => $this->result->code,
          '@error' => $this->result->error,
          '%msg' => $msg,
        )), 'error');
      return FALSE;
    }
    return $this->result->data;
  }

  /**
   * Makes a call to the AddThis.com server and parses the JSON response.
   *
   * @param string $dimension
   *   The dimension we're interested in.  It must be null or one of:
   *   - url
   *   - continent
   *   - country
   *   - domain
   *   - service
   * @param string $params
   *   Additional parameters to send.  The following are allowed:
   *   - period: 'day', 'week' or 'month'
   *   - domain: any URL domain
   *   - service: 'email'
   *
   * @return array
   *   An array of associative arrays, each containing:
   *     - shares: The number of times the page was shared.
   *     - url: The URL of the page.
   */
  public function fetchJson($dimension = '', $params = array()) {
    $data = $this->fetch($dimension, 'json', $params);
    if ($data !== FALSE) {
      return drupal_json_decode($data);
    }
    return FALSE;
  }
}