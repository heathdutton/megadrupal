<?php
/**
 * @file
 * Contains COLOURlovers API class.
 */

class COLOURlovers {

  protected $apiUrl;

  /**
   * Class constructor function.
   */
  public function __construct($api_url) {
    $this->apiUrl = trim($api_url, '/');
  }

  /**
   * Returns information on a single color.
   *
   * @param string $hex
   *   The hex value of the color.
   * @param array $parameters
   *   Additional parameters as defined by the API.
   * @return array
   *   API return value or NULL.
   */
  public function getColor($hex, $parameters = array()) {
    return $this->call('color/' . $hex, $parameters);
  }

  /**
   * Returns information on multiple colors.
   *
   * @param int $num
   *   The number of colors to pull from the API.
   * @param array $parameters
   *   Additional parameters as defined by the API.
   * @return array
   *   API return value or NULL.
   */
  public function getColors($num = 20, $parameters = array()) {
    return $this->call('colors', array_merge(array(
      'numResults' => $num,
    ), $parameters));
  }

  /**
   * Returns information on new colors.
   *
   * @param int $num
   *   The number of colors to pull from the API.
   * @param array $parameters
   *   Additional parameters as defined by the API.
   * @return array
   *   API return value or NULL.
   */
  public function getColorsNew($num = 20, $parameters = array()) {
    return $this->call('colors/new', array_merge(array(
      'numResults' => $num,
    ), $parameters));
  }

  /**
   * Returns information on top colors.
   *
   * @param int $num
   *   The number of colors to pull from the API.
   * @param array $parameters
   *   Additional parameters as defined by the API.
   * @return array
   *   API return value or NULL.
   */
  public function getColorsTop($num = 20, $parameters = array()) {
    return $this->call('colors/top', array_merge(array(
      'numResults' => $num,
    ), $parameters));
  }

  /**
   * Returns information on a random color.
   *
   * @return array
   *   API return value or NULL.
   */
  public function getColorsRandom() {
    return $this->call('colors/random');
  }

  /**
   * Returns information on a single palette.
   *
   * @param int $id
   *   The id of the palette.
   * @param array $parameters
   *   Additional parameters as defined by the API.
   * @return array
   *   API return value or NULL.
   */
  public function getPalette($id, $parameters = array()) {
    return $this->call('palette/' . $id, $parameters);
  }

  /**
   * Returns information on multiple palettes.
   *
   * @param int $num
   *   The number of palettes to pull from the API.
   * @param array $parameters
   *   Additional parameters as defined by the API.
   * @return array
   *   API return value or NULL.
   */
  public function getPalettes($num = 20, $parameters = array()) {
    return $this->call('palettes', array_merge(array(
      'numResults' => $num,
    ), $parameters));
  }

  /**
   * Returns information on new palettes.
   *
   * @param int $num
   *   The number of palettes to pull from the API.
   * @param array $parameters
   *   Additional parameters as defined by the API.
   * @return array
   *   API return value or NULL.
   */
  public function getPalettesNew($num = 20, $parameters = array()) {
    return $this->call('palettes/new', array_merge(array(
      'numResults' => $num,
    ), $parameters));
  }

  /**
   * Returns information on top palettes.
   *
   * @param int $num
   *   The number of palettes to pull from the API.
   * @param array $parameters
   *   Additional parameters as defined by the API.
   * @return array
   *   API return value or NULL.
   */
  public function getPalettesTop($num = 20, $parameters = array()) {
    return $this->call('palettes/top', array_merge(array(
      'numResults' => $num,
    ), $parameters));
  }

  /**
   * Returns information on a random palette.
   *
   * @return array
   *   API return value or NULL.
   */
  public function getPalettesRandom() {
    return $this->call('palettes/random');
  }

  /**
   * Returns information on a single pattern.
   *
   * @param int $id
   *   The id of the pattern.
   * @param array $parameters
   *   Additional parameters as defined by the API.
   * @return array
   *   API return value or NULL.
   */
  public function getPattern($id, $parameters = array()) {
    return $this->call('pattern/' . $id, $parameters);
  }

  /**
   * Returns information on multiple patterns.
   *
   * @param int $num
   *   The number of patterns to pull from the API.
   * @param array $parameters
   *   Additional parameters as defined by the API.
   * @return array
   *   API return value or NULL.
   */
  public function getPatterns($num = 20, $parameters = array()) {
    return $this->call('patterns', array_merge(array(
      'numResults' => $num,
    ), $parameters));
  }

  /**
   * Returns information on new patterns.
   *
   * @param int $num
   *   The number of patterns to pull from the API.
   * @param array $parameters
   *   Additional parameters as defined by the API.
   * @return array
   *   API return value or NULL.
   */
  public function getPatternsNew($num = 20, $parameters = array()) {
    return $this->call('patterns/new', array_merge(array(
      'numResults' => $num,
    ), $parameters));
  }

  /**
   * Returns information on top patterns.
   *
   * @param int $num
   *   The number of patterns to pull from the API.
   * @param array $parameters
   *   Additional parameters as defined by the API.
   * @return array
   *   API return value or NULL.
   */
  public function getPatternsTop($num = 20, $parameters = array()) {
    return $this->call('patterns/top', array_merge(array(
      'numResults' => $num,
    ), $parameters));
  }

  /**
   * Returns information on a random pattern.
   *
   * @return array
   *   API return value or NULL.
   */
  public function getPatternsRandom() {
    return $this->call('patterns/random');
  }

  /**
   * Returns information on a single lover.
   *
   * @param int $id
   *   The id of the lover.
   * @param array $parameters
   *   Additional parameters as defined by the API.
   * @return array
   *   API return value or NULL.
   */
  public function getLover($id, $parameters = array()) {
    return $this->call('lover/' . $id, $parameters);
  }

  /**
   * Returns information on multiple lovers.
   *
   * @param int $num
   *   The number of lovers to pull from the API.
   * @param array $parameters
   *   Additional parameters as defined by the API.
   * @return array
   *   API return value or NULL.
   */
  public function getLovers($num = 20, $parameters = array()) {
    return $this->call('lovers', array_merge(array(
      'numResults' => $num,
    ), $parameters));
  }

  /**
   * Returns information on new lovers.
   *
   * @param int $num
   *   The number of lovers to pull from the API.
   * @param array $parameters
   *   Additional parameters as defined by the API.
   * @return array
   *   API return value or NULL.
   */
  public function getLoversNew($num = 20, $parameters = array()) {
    return $this->call('lovers/new', array_merge(array(
      'numResults' => $num,
    ), $parameters));
  }

  /**
   * Returns information on top lovers.
   *
   * @param int $num
   *   The number of lovers to pull from the API.
   * @param array $parameters
   *   Additional parameters as defined by the API.
   * @return array
   *   API return value or NULL.
   */
  public function getLoversTop($num = 20, $parameters = array()) {
    return $this->call('lovers/top', array_merge(array(
      'numResults' => $num,
    ), $parameters));
  }

  /**
   * Returns stats about colors.
   *
   * @return array
   *   API return value or NULL.
   */
  public function getStatsColors() {
    return $this->call('stats/colors');
  }

  /**
   * Returns stats about palettes.
   *
   * @return array
   *   API return value or NULL.
   */
  public function getStatsPalettes() {
    return $this->call('stats/palettes');
  }

  /**
   * Returns stats about patterns.
   *
   * @return array
   *   API return value or NULL.
   */
  public function getStatsPatterns() {
    return $this->call('stats/patterns');
  }

  /**
   * Returns stats about lovers.
   *
   * @return array
   *   API return value or NULL.
   */
  public function getStatsLovers() {
    return $this->call('stats/lovers');
  }

	/**
	 * Call the API
	 *
	 * @param string $method
	 *   The endpoint to call.
	 * @param array $values
	 *   The values to pass into the API
	 * @return array
	 *   API return value or NULL.
	 */
  private function call($method, $values = array()) {
    if ($response = drupal_http_request(url($this->apiUrl . '/' . $method, array(
      'absolute' => TRUE
    )), array(
      'headers' => array(
        'Content-Type' => 'application/x-www-form-urlencoded'
      ),
      'method' => 'POST',
      'data' => drupal_http_build_query(array_merge($values, array(
        'format' => 'json'
      ))),
    ))) {
      if ($response->code == 200) {
        return json_decode($response->data);
      }
    }
    return NULL;
  }

}
