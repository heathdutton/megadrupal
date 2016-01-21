<?php

/**
 * @file
 * Global API classes and functions.
 */

/**
 * Class for Yandex.XML service.
 */
class YandexXml {

  /**
   * Response xml.
   *
   * @see http://api.yandex.com/xml/doc/dg/concepts/response.xml
   */
  public $response;

  /**
   * Results.
   */
  protected $results = array();

  /**
   * Number of results per page.
   */
  protected $limit = FALSE;

  /**
   * Search url.
   */
  protected $searchUrl;

  /**
   * Search query.
   */
  protected $query;

  /**
   * Search area.
   */
  protected $site;

  /**
   * Current page.
   */
  protected $page = 0;

  /**
   * Search options.
   */
  protected $options = array(
    'maxpassages'           => 5 ,
    'max-title-length'      => 160 ,
    'max-headline-length'   => 160 ,
    'max-passage-length'    => 160 ,
    'max-text-length'       => 640 ,
  );

  /**
   * Error code.
   */
  public $error = NULL;

  /**
   * Constructor.
   */
  public function __construct($url) {
    if (empty($url)) {
      throw new Exception('Yandex XML: search url is requried');
    }
    $this->searchUrl = $url;
  }

  /**
   * Query setter/getter.
   */
  public function query($query = FALSE) {
    if ($query) {
      $this->query = $query;
    }
    return $this->query;
  }

  /**
   * Page setter/getter.
   */
  public function page($page = FALSE) {
    if ($page) {
      $this->page = $page;
    }
    return $this->page;
  }

  /**
   * Site setter/getter.
   */
  public function site($site = FALSE) {
    if ($site) {
      $this->site = $site;
    }
    return $this->site;
  }

  /**
   * Limit setter.
   */
  public function limit($limit = FALSE) {
    if ($limit) {
      $this->limit = $limit;
    }
    return $this->limit;
  }

  /**
   * Send request.
   *
   * @see http://api.yandex.com/xml/doc/dg/concepts/post-request.xml
   */
  public function send($log_file = FALSE) {

    $request = new SimpleXMLElement("<?xml version='1.0' encoding='utf-8'?><request></request>");

    foreach ($this->options as $name => $value) {
      $request->addChild($name, $value);
    }
    $request->addChild('query', $this->query);
    $request->addChild('page', $this->page);

    if ($this->limit) {
      $groupby = $request->addChild('groupings')->addChild('groupby');
      $groupby->addAttribute('groups-on-page', $this->limit);
    }
    $url = $this->searchUrl . '&site=' . $this->site;
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_HTTPHEADER, array('Accept: application/xml'));
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($ch, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
    curl_setopt($ch, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);
    curl_setopt($ch, CURLOPT_POSTFIELDS, $request->asXML());
    curl_setopt($ch, CURLOPT_POST, TRUE);
    $data = curl_exec($ch);

    if (curl_errno($ch)) {
      throw new Exception('CURL error: ' . curl_error($ch));
    }

    if ($log_file) {
      file_put_contents($log_file, $data);
    }

    libxml_use_internal_errors(TRUE);
    $response = new SimpleXMLElement($data);

    $this->response = $response->response;

    if ($this->response->error) {
      $this->error = (int) $this->response->error->attributes()->code[0];
    }

    return $this;
  }

  /**
   * Get total number of results.
   */
  public function total() {
    $res = $this->response->xpath('found[attribute::priority="all"]');
    return (int) $res[0];
  }

  /**
   * Get total number of pages.
   */
  public function totalPages() {
    return ceil($this->total() / $this->limit);
  }

  /**
   * Get results.
   */
  public function results() {
    if (empty($this->results) && empty($this->error) && $this->response) {
      foreach ($this->response->results->grouping->group as $group) {
        $result = new stdClass();
        $result->url       = $group->doc->url;
        $result->domain    = $group->doc->domain;
        $result->title     = isset($group->doc->title) ? $group->doc->title : NULL;
        $result->headline  = isset($group->doc->headline) ? $group->doc->headline : NULL;
        $result->passages  = isset($group->doc->passages->passage) ? $group->doc->passages->passage : NULL;
        $result->sitelinks = isset($group->doc->snippets->sitelinks->link) ? $group->doc->snippets->sitelinks->link : NULL;
        array_push($this->results, $result);
      }
    }
    return $this->results;
  }

  /**
   * Highlight snippet.
   */
  static public function highlight($snippet) {
    $snippet = str_replace('<hlword>', '<strong>', $snippet);
    $snippet = str_replace('</hlword>', '</strong>', $snippet);
    $snippet = strip_tags($snippet, '<strong>');
    return $snippet;
  }

  /**
   * Get logo markup.
   */
  static public function getLogo($lang_code) {
    $logo['en'] = '<a class="yandex-logo" href="http://yandex.com" target="_blank"><span class="yandex-first-letter">Y</span>andex</a>';
    $logo['ru'] = '<a class="yandex-logo" href="http://yandex.ru" target="_blank"><span class="yandex-first-letter">Я</span>ндекс</a>';
    $logo['tr'] = '<a class="yandex-logo" href="http://www.yandex.com.tr/" target="_blank"><span class="yandex-first-letter">Y</span>andex</a>';
    // Use en as fallback.
    return isset($logo[$lang_code]) ? $logo[$lang_code] : $logo['en'];
  }

  /**
   * Get logo markup.
   */
  static public function getRegistrationHelpUrl($lang_code) {
    $logo['en'] = 'http://api.yandex.com/xml/doc/dg/task/registration.xml';
    $logo['ru'] = 'http://api.yandex.ru/xml/doc/dg/task/registration.xml';
    // Use 'en' as fallback.
    return isset($logo[$lang_code]) ? $logo[$lang_code] : $logo['en'];
  }

}
