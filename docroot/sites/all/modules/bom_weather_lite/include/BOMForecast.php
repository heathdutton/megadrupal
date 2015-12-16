<?php
/**
 * Class that can parse BOM weather forecast data.
 *
 * @TODO: add defines for the parser strings.
 */

/**
 * Defines for parser string.
 */
define('BOM_WEATHER_LITE_IDENTIFIER',   'div[id=container] h1');
define('BOM_WEATHER_LITE_MAXIMUM',      'div[id=content] div.day div.forecast em.max');
define('BOM_WEATHER_LITE_SUMMARY',      'div[id=content] div.day div.forecast dd.summary');
define('BOM_WEATHER_LITE_FIRE_LIST',    'div[id=content] div.day dl.alert dd');
define('BOM_WEATHER_LITE_FIRE_SINGLE',  'div[id=content] div.day p.alert');
define('BOM_WEATHER_LITE_UVINDEX',      'div[id=content] div.day p.alert');
define('BOM_WEATHER_LITE_UVINDEX_PREG', '/(.*) ([0-9]+) \[([A-z ]+)\]$/');
define('BOM_WEATHER_LITE_DATE',         'div[id=content] p.date');
define('BOM_WEATHER_LITE_ICON',         'div[id=content] div.day div.forecast dd.image img');

class BOMForecast {
  private $url        = NULL;
  private $base       = NULL;
  private $identifier = NULL;
  private $maximum    = NULL;
  private $summary    = NULL;
  private $fire       = NULL;
  private $uv_index   = NULL;
  private $uv_text    = NULL;
  private $icon       = NULL;
  private $date       = NULL;

  public  $error      = NULL;

  function __construct($url = '') {
    if (!empty($url)) {
      $this->url = $url;
      $this->parse();
    }
  }

  function parse() {
    // Create a base URL for icons and internal links.
    $base = parse_url($this->url, PHP_URL_SCHEME) . '://' . parse_url($this->url, PHP_URL_HOST);

    $html = new simple_html_dom();

    // A timeout connecting to the BOM site generates a LOT of warning spam.
    // Hide the error output, but set an error condition so that the module
    // can detect if a problem happened and act appropriately.
    try {
      @$html->load_file($this->url);
    } catch (Exception $e) {
      $this->error = $e;
      return FALSE;
    }

    $identifier     = $html->find(BOM_WEATHER_LITE_IDENTIFIER, 0)->plaintext;
    $maximum        = $html->find(BOM_WEATHER_LITE_MAXIMUM, 0)->plaintext;
    $summary        = $html->find(BOM_WEATHER_LITE_SUMMARY, 0)->plaintext;
    $fire           = $html->find(BOM_WEATHER_LITE_FIRE_LIST, 0);

    // Fire danger can be a list *or* a single paragraph or be absent
    // altogether, in which case the paragraph contains the UV Alert.
    if (!empty($fire->plaintext)) {
      // Fire was a list, so UV index is the first alert paragraph.
      $fires     = preg_split('/:/', $fire->plaintext);
      $fire      = $fires[1];
      $uv_string = $html->find(BOM_WEATHER_LITE_UVINDEX, 0)->plaintext;
    }
    else {
      // The alerts were not a list. If there is no fire danger, the UV index
      // is the first alert. If not, fire danger is first and UV comes second.
      $uv_string = $html->find(BOM_WEATHER_LITE_FIRE_SINGLE, 0)->plaintext;
      if (strpos($uv_string, 'UV Alert') === FALSE) {
        list(,$fire) = preg_split('/ - /', $html->find(BOM_WEATHER_LITE_FIRE_SINGLE, 0)->plaintext);
        $uv_string = $html->find(BOM_WEATHER_LITE_UVINDEX, 1)->plaintext;
      }
    }

    // Ok, parsed the strings... now to extract the UV values.
    $ret = preg_match(BOM_WEATHER_LITE_UVINDEX_PREG, $uv_string, $matches);
    if (!empty($ret)) {
      $uv_index = $matches[2];
      $uv_text = $matches[3];
    }
    else {
      // Avoid a warning... but really this should not happen.
      $uv_index = $uv_text = '';
    }

    $date = $html->find(BOM_WEATHER_LITE_DATE, 0)->plaintext;
    $icon = $html->find(BOM_WEATHER_LITE_ICON, 0)->src;

    $this->identifier = trim($identifier);
    $this->maximum    = trim($maximum);
    $this->summary    = trim($summary);
    $this->fire       = trim($fire);
    $this->uv_index   = trim($uv_index);
    $this->uv_text    = trim($uv_text );
    $this->date       = trim($date);
    $this->icon       = $base . trim($icon);
    $this->base       = $base;

    // Everybody's freeeeee!
    $html->clear();
    unset($html);
  }

  /**
   * All the tedious OO setters.
   */
  function setUrl($url) {
    $this->url = $url;
  }

  /**
   * All the tedious OO getters.
   */
  function getIdentifier() {
    return $this->identifier;
  }

  function getMaximum() {
    return $this->maximum;
  }

  function getSummary() {
    return $this->summary;
  }

  function getFire() {
    return $this->fire;
  }

  function getUvIndex() {
    return $this->uv_index;
  }

  function getUvText() {
    return $this->uv_text;
  }

  function getDate() {
    return $this->date;
  }

  function getIcon() {
    return $this->icon;
  }

  function getBase() {
    return $this->base;
  }

  /**
   * Return an array that can go into a theme function.
   */
  function getForecast() {
    return array(
      'identifier' => $this->identifier,
      'maximum'    => $this->maximum,
      'summary'    => $this->summary,
      'fire'       => $this->fire,
      'uv_index'   => $this->uv_index,
      'uv_text'    => $this->uv_text,
      'date'       => $this->date,
      'icon'       => $this->icon,
      'url'        => $this->url,
      'base'       => $this->base,
    );
  }

  /**
   * Magic __toString() function returns the class as string.
   */
  function __toString() {
    $output  = '';
    $output .= sprintf("== %s ==\n", $this->identifier);
    $output .= sprintf("(%s)\n", $this->date);
    $output .= sprintf("Maximum:     %d\n", $this->maximum);
    $output .= sprintf("Summary:     %s\n", $this->summary);
    $output .= sprintf("UV levels:   %d (%s)\n", $this->uv_index, $this->uv_text);
    $output .= sprintf("Fire danger: %s\n", $this->fire);
    $output .= sprintf("Icon URL:    %s\n", $this->icon);
    return $output;
  }
}
