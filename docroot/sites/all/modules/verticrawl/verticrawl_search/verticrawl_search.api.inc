<?php
/**
 * @file
 * Verticrawl API Class managing requests.
 */

define("VERTICRAWL_SEARCH_PHP_API_URL", "http://admin.verticrawl.com/search/search_json.php");
define("VERTICRAWL_SEARCH_PHP_API_PORT", "80");
define("VERTICRAWL_SEARCH_PHP_API_METHOD", "GET");

define("VERTICRAWL_SEARCH_PHP_SORT_BY_RELEVANCE", "r");
define("VERTICRAWL_SEARCH_PHP_SORT_ASC", "d");
define("VERTICRAWL_SEARCH_PHP_SORT_DESC", "D");

define("VERTICRAWL_SEARCH_PHP_KEYWORD_MATCH_MODE_EXACT_WORD", "wrd");
define("VERTICRAWL_SEARCH_PHP_KEYWORD_MATCH_MODE_BEGIN_WITH", "beg");
define("VERTICRAWL_SEARCH_PHP_KEYWORD_MATCH_MODE_END_WITH", "end");
define("VERTICRAWL_SEARCH_PHP_KEYWORD_MATCH_MODE_CONTAIN_TEXT", "sub");

/**
 * Verticrawl Search API Class.
 *
 * @category Class
 */
class VerticrawlSearchApi {
  private $apiKey = -1;
  private $searchResults = array();

  private $filterDate = array();
  private $filters = array();
  private $headerLanguage = 'en';
  private $itemsPerPage = 10;
  private $keywordWrapper = 'strong';
  private $keywords = array();
  private $matchMode = VERTICRAWL_SEARCH_PHP_KEYWORD_MATCH_MODE_EXACT_WORD;
  private $pageNum = 0;
  private $parameters = array();
  private $sortBy = VERTICRAWL_SEARCH_PHP_SORT_BY_RELEVANCE;

  /**
   * Setter: filterDate.
   *
   * @param array $filter_date
   *   The date filter. Used to filter the results by content date.
   */
  public function setFilterDate(array $filter_date) {
    if (!empty($filter_date) && is_array($filter_date)) {
      $valid_filter_date_keys = array(
        'dx',
        'dd',
        'dm',
        'dy',
      );

      $final_filter_date = array();
      $all_filter_date_defined = TRUE;

      foreach ($valid_filter_date_keys as $valid_filter_date_key) {
        if (isset($filter_date[$valid_filter_date_key])) {
          $final_filter_date[$valid_filter_date_key] = $filter_date[$valid_filter_date_key];
        }
        else {
          $all_filter_date_defined = FALSE;
        }
      }

      if ($all_filter_date_defined) {
        $this->filterDate = $final_filter_date;
      }
    }
    return $this;
  }

  /**
   * Setter: filters.
   *
   * @param array $filters
   *   The advanced filters. Used to filter the results with advanced
   *   parameters.
   */
  public function setFilters(array $filters) {
    if (!empty($filters) && is_array($filters)) {
      $valid_filter_keys = array(
        'g',
        'groupbysite',
        'type',
        'site',
        'ul',
      );

      $final_filters = array();
      foreach ($valid_filter_keys as $valid_filter_key) {
        if (isset($filters[$valid_filter_key])) {
          $final_filters[$valid_filter_key] = $filters[$valid_filter_key];
        }
      }
      $this->filters = $final_filters;
    }
    return $this;
  }

  /**
   * Setter: itemsPerPage.
   *
   * @param int $items_per_page
   *   The number of items displayed per page.
   */
  public function setItemsPerPage($items_per_page) {
    if (is_int($items_per_page) && $items_per_page > 0) {
      $this->itemsPerPage = $items_per_page;
    }
    return $this;
  }

  /**
   * Setter: keywordWrapper.
   *
   * @param string $keyword_wrapper
   *   The HTML tag used to wrap keywords in results.
   *   E.g.: 'strong' or 'em' (without "<" and ">").
   */
  public function setKeywordWrapper($keyword_wrapper) {
    if (!empty($keyword_wrapper) && is_string($keyword_wrapper)) {
      $this->keywordWrapper = $keyword_wrapper;
    }
    return $this;
  }

  /**
   * Setter: keywords.
   *
   * @param array $keywords
   *   The keywords used to define the search.
   */
  public function setKeywords(array $keywords) {
    if (is_array($keywords)) {
      $final_keywords = array();

      foreach ($keywords as $keyword) {
        $final_keywords[] = utf8_decode($keyword);
      }

      $this->keywords = $final_keywords;
    }
    return $this;
  }

  /**
   * Setter: matchMode.
   *
   * @param string $match_mode
   *   Define the way for matching the results with keywords.
   */
  public function setMatchMode($match_mode) {
    if (!empty($match_mode) && is_string($match_mode)) {
      switch ($match_mode) {
        case VERTICRAWL_SEARCH_PHP_KEYWORD_MATCH_MODE_EXACT_WORD:
        case VERTICRAWL_SEARCH_PHP_KEYWORD_MATCH_MODE_BEGIN_WITH:
        case VERTICRAWL_SEARCH_PHP_KEYWORD_MATCH_MODE_END_WITH:
        case VERTICRAWL_SEARCH_PHP_KEYWORD_MATCH_MODE_CONTAIN_TEXT:
          $this->matchMode = $match_mode;
          break;
      }
    }
    return $this;
  }

  /**
   * Setter: headerLanguage.
   *
   * @param string $header_language
   *   Define the language of the results.
   *   E.g.: 'en', 'es' or 'fr'.
   */
  public function setHeaderLanguage($header_language) {
    if (!empty($header_language) && is_string($header_language)) {
      $this->headerLanguage = $header_language;
    }
    return $this;
  }

  /**
   * Setter: pageNum.
   *
   * @param int $page_num
   *   The number of the page to display the results.
   *   In relationship with the $items_per_page parameter.
   */
  public function setPageNum($page_num) {
    if (is_int($page_num) && $page_num > 0) {
      $this->pageNum = $page_num - 1;
    }
    return $this;
  }

  /**
   * Setter: parameters.
   *
   * @param array $parameters
   *   An associative array used to rewrite array keys given
   *   by Verticrawl's response.
   */
  public function setParameters(array $parameters) {
    if (is_array($parameters)) {
      $this->parameters = $parameters;
    }
    return $this;
  }

  /**
   * Setter: sortBy.
   *
   * @param string $sort_by
   *   Define the method to sort the results.
   */
  public function setSortBy($sort_by) {
    if (!empty($sort_by) && is_string($sort_by)) {
      switch ($sort_by) {
        case VERTICRAWL_SEARCH_PHP_SORT_BY_RELEVANCE:
        case VERTICRAWL_SEARCH_PHP_SORT_ASC:
        case VERTICRAWL_SEARCH_PHP_SORT_DESC:
          $this->sortBy = $sort_by;
          break;
      }
    }
    return $this;
  }

  /**
   * PRIVATE Setter: searchResults.
   *
   * @param array $search_results
   *   The results of the search given by Verticrawl.
   */
  private function setSearchResults(array $search_results) {
    if (is_array($search_results) && !empty($search_results)) {
      $this->searchResults = $search_results;
    }
  }

  /**
   * CONSTRUCTOR.
   */
  public function __construct($api_key) {
    $this->apiKey = $api_key;
    return $this;
  }

  /**
   * Static Method : Return short and well-formed URL.
   *
   * This method transform a long URL to a shorter one.
   * It should be used in order to display more friendly links.
   *
   * @param string $url
   *   The url used to return a shorter format.
   */
  public static function getDisplayUrl($url) {
    $parsed_url = parse_url($url);

    if (empty($url) || $parsed_url === FALSE) {
      return $url;
    }

    $path_array = explode("/", substr($parsed_url['path'], 1));
    $nb_paths = count($path_array);

    $display_url = '';

    if ($nb_paths > 2) {
      $display_url = $parsed_url['host'] . '/.../' . $path_array[$nb_paths - 1];
    }
    else {
      $display_url = $parsed_url['host'] . $parsed_url['path'];
    }

    if (!empty($parsed_url['query'])) {
      $display_url .= '?' . $parsed_url['query'];
    }

    if (strlen($display_url) > 60) {
      $display_url = substr($display_url, 0, 60) . '...';
    }

    return $display_url;
  }

  /**
   * Public Method : Fecth All results.
   *
   * This method is used to fecth results from Verticrawl API
   * You have to used the SETTERS before fecthing results
   */
  public function fetchResults() {
    // HTTP/1.1 404 Not Found.
    // HTTP/1.1 200 OK.
    $final_content = array();

    // Default return if no kewyords defined.
    if (empty($this->keywords)) {
      foreach ($this->parameters as $key => $parameter) {
        if (!isset($$key)) {
          $$key = NULL;
        }

        $parameter_key = is_string($parameter) ? $parameter : $key;
        $final_content[$parameter_key] = NULL;
      }

      return $final_content;
    }

    // If keywords not empty.
    $query = $this->prepareQuery();

    $url = parse_url(VERTICRAWL_SEARCH_PHP_API_URL . $query);

    $server = $url['host'];
    $file = $url['path'] . '?' . $url['query'];
    $cont = "";

    $ip = gethostbyname($server);

    if (empty($ip)) {
      return array('error' => 'IP address is empty ! Please check if ' . $server . 'is accessible from your application');
    }

    $fp = fsockopen($server, VERTICRAWL_SEARCH_PHP_API_PORT, $errno, $errstr, 5);

    if (!$fp) {
      return array('error' => "$errno: $errstr");
    }
    else {
      $com = VERTICRAWL_SEARCH_PHP_API_METHOD . " " . $file . " ";
      $com .= "HTTP/1.0\r\n";
      $com .= "Accept: */*\r\n";
      $com .= "Accept-Language: " . $_SERVER["HTTP_ACCEPT_LANGUAGE"] . "\r\n";
      $com .= "User-Agent: " . $_SERVER["HTTP_USER_AGENT"] . "\r\n";
      $com .= "Referer: " . $_SERVER["HTTP_REFERER"] . "\r\n";
      $com .= "Host: $server:" . VERTICRAWL_SEARCH_PHP_API_PORT . "\r\n";
      // Do not remove this line.
      $com .= "Connection: close\r\n\r\n";

      fwrite($fp, $com);

      while (!feof($fp)) {
        $cont .= fread($fp, 256);
      }

      fclose($fp);

      list($header, $content) = explode("\r\n\r\n", $cont);
      if (is_array($content)) {
        unset($header);
        list($content, $trash) = explode("\r\n0", $content);
      }

      if (isset($trash)) {
        unset($trash);
      }

      $content = json_decode(utf8_encode($content), TRUE);

      if (array_key_exists('searchresult', $content)) {
        $this->setSearchResults($content['searchresult']);
      }

      foreach ($this->parameters as $key => $parameter) {
        if (!isset($content[$key])) {
          $content[$key] = NULL;
        }

        $parameter_key = is_string($parameter) ? $parameter : $key;

        if ($key == 'URL' && is_array($content[$key])) {
          $final_content[$parameter_key] = $this->wrapKeywords($content[$key]);
        }
        else {
          $final_content[$parameter_key] = $content[$key];
        }
      }

      return $final_content;
    }
  }

  /**
   * Public Method: Return an array for managing pagination.
   *
   * @param array $get_keys
   *   (optional) The $get_keys parameter will define the value for the query
   *   string ($_GET)in the query_string in page links 'keyword' and 'page'
   *   are default valuerendered it can be refine by an associative array.
   *   Example : $get_key = array('keywords', 'k')
   *   - The code above will change the key 'keywords' from $_GET parameter to
   *   'k' in page links.
   *
   *   You can define additonnal values in the query string
   *   Example : $get_key = array('title','Some title as a value')
   *   - The code above will add "&title=Some title as a value" in page links.
   */
  public function getPager($get_keys = array()) {
    $default_get_keys = array(
      'keywords' => 'keywords',
      'page' => 'page',
    );

    if (!is_array($get_keys) || empty($get_keys)) {
      $get_keys = $default_get_keys;
    }
    else {
      foreach ($default_get_keys as $key => $value) {
        if (!isset($get_keys[$key]) || empty($get_keys[$key])) {
          $get_keys[$key] = $value;
        }
      }
    }

    $pager = array();
    $items_per_page = $this->itemsPerPage;
    $search_results = $this->searchResults;

    if (is_array($search_results) && isset($search_results['query'])) {

      $query_string = '?' . $get_keys['keywords'] . '=' . str_replace(' ', '+', $search_results['query']);
      foreach ($get_keys as $key => $value) {
        if ($key != 'keywords' && $key != 'page') {
          $query_string .= "&" . $key . "=" . $value;
        }
      }

      $weights = array();

      $nb_pages = intval($search_results['total'] / $items_per_page);
      if ($search_results['total'] % $items_per_page != 0) {
        $nb_pages += 1;
      }

      $current_page = intval($search_results['last'] / $items_per_page);
      if ($search_results['last'] % $items_per_page != 0) {
        $current_page += 1;
      }

      if ($nb_pages > 1) {

        if ($current_page > 1) {
          $pager['« first'] = array(
            'query_string' => $query_string . '&' . $get_keys['page'] . '=1',
            'classes' => array(
              'pager-first',
              'first',
            ),
          );
          $weights['« first'] = -1;

          $pager['‹ previous'] = array(
            'query_string' => $query_string . '&' . $get_keys['page'] . '=' . ($current_page - 1),
            'classes' => array(
              'pager-previous',
            ),
          );
          $weights['‹ previous'] = 0;
        }

        if ($nb_pages > 10) {
          // If it's the 5 last pages.
          if ($current_page > $nb_pages - 5) {
            $page_begin = $nb_pages - 9;
            $page_end = $nb_pages;
          }
          elseif ($current_page >= 6) {
            $page_begin = $current_page - 5;
            $page_end = $current_page + 4;
          }
          elseif ($current_page < 6) {
            $page_begin = 1;
            $page_end = 10;
          }
        }
        else {
          $page_begin = 1;
          $page_end = $nb_pages;
        }

        for ($page = $page_begin; $page <= $page_end; $page++) {
          $classes = ($current_page == $page) ? array('pager-current') : array('pager-item');

          $pager[" $page "] = array(
            'query_string' => $query_string . '&' . $get_keys['page'] . '=' . $page,
            'classes' => $classes,
          );
          $weights[" $page "] = $page;
        }

        if ($current_page < $nb_pages) {
          $pager['next ›'] = array(
            'query_string' => $query_string . '&' . $get_keys['page'] . '=' . ($current_page + 1),
            'classes' => array(
              'pager-next',
            ),
          );

          $weights['next ›'] = $nb_pages + 1;

          $pager['last »'] = array(
            'query_string' => $query_string . '&' . $get_keys['page'] . '=' . $nb_pages,
            'classes' => array(
              'pager-last',
              'last',
            ),
          );

          $weights['last »'] = $nb_pages + 2;
        }
      }
      array_multisort($weights, SORT_ASC, $pager);
    }

    return $pager;
  }

  /**
   * Private Method: Prepare the query before fetching results.
   */
  private function prepareQuery() {
    $query = "?DATABASE=" . urlencode($this->apiKey);

    $query .= "&hl=" . urlencode($this->headerLanguage);

    // The ip_address() function is for Drupal ONLY to prevent BOT validation
    // errors!
    // Replace ip_address() to $_SERVER["REMOTE_ADDR"] if your
    // are NOT using a Drupal instance.
    $query .= "&ip=" . urlencode(ip_address());

    $query .= "&q=" . urlencode(implode(" ", $this->keywords));

    foreach ($this->filters as $key => $filter) {
      $query .= '&' . $key . '=' . urlencode($filter);
    }

    foreach ($this->filterDate as $key => $filter_date) {
      $query .= '&' . $key . '=' . urlencode($filter_date);
    }

    if (!empty($this->pageNum)) {
      $query .= "&np=" . urlencode($this->pageNum);
    }

    $query .= "&ps=" . urlencode($this->itemsPerPage);

    if ($this->matchMode != VERTICRAWL_SEARCH_PHP_KEYWORD_MATCH_MODE_EXACT_WORD) {
      $query .= "&wm=" . urlencode($this->matchMode);
    }

    if ($this->sortBy != VERTICRAWL_SEARCH_PHP_SORT_BY_RELEVANCE) {
      $query .= "&s=" . urlencode($this->sortBy);
    }

    return $query;
  }

  /**
   * Private Method: Wrap the keywords in results with an HTML Element.
   *
   * @param array $urls
   *   The content of the results in the 'URL' key given by Verticrawl.
   */
  private function wrapKeywords(array $urls) {
    if (is_array($urls) && !empty($urls)) {
      $urls_var_wrappable = array(
        'body',
        'url-title',
        'meta-description',
      );

      foreach ($urls as $key => $url) {
        foreach ($urls_var_wrappable as $urls_var) {
          if (isset($url[$urls_var])) {
            $url[$urls_var] = str_replace("<div@>", '<' . $this->keywordWrapper . '>', $url[$urls_var]);
            $url[$urls_var] = str_replace("</div@>", '</' . $this->keywordWrapper . '>', $url[$urls_var]);
            $urls[$key][$urls_var] = $url[$urls_var];
          }
        }
      }
    }
    return $urls;
  }

}
