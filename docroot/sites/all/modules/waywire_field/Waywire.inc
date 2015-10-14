<?php

/**
 * @file
 * Class for dealing with the Waywire videos.
 */

/**
 * Class for dealing with Waywire videos.
 */
class Waywire {

  private $_apiKey;
  private $_channel;
  public $thumbnail;
  public $duration;
  public $published;
  public $updated;
  public $video_url;

  /**
   * Default Constructor.
   *
   * @param string $apiKey
   *   The waywire channel url.
   * @param string $channel
   *   The channel url.
   */
  public function __construct($apiKey = FALSE, $channel = FALSE) {
    if ($apiKey) {
      $this->_apiKey = $apiKey;
    }
    if ($channel) {
      $this->_channel = $channel;
    }
  }

  /**
   * Get the size options for a waywire video.
   *
   * @return array
   *   Returns a list of video sizes.
   */
  public function get_size_options() {
    return array(
      '416x290' => '416px by 290px',
      '516x346' => '516px by 346px',
      '616x402' => '616px by 402px',
      '816x515' => '816px by 515px',
      'responsive' => 'responsive (full-width of container)',
      'custom' => 'custom',
    );
  }

  /**
   * Get Layout Options.
   *
   * @return array
   *   The layout options.
   */
  public function get_layout_options() {
    return array(
      '' => '',
      'compact' => 'compact',
      'viral' => 'viral',
      'thumnails' => 'thumbnials',
    );
  }

  /**
   * Get the type of links.
   *
   * @return array
   *   Returns a list of link types.
   */
  public function get_link_types() {
    return array(
      'content' => t('Content'),
      'waywire' => t('Waywire'),
    );
  }

  /**
   * Get all the player types.
   *
   * @return array
   *   Returns a list of playter types.
   */
  public function get_player_options() {
    return array(
      'svp' => t('Single-View player'),
      'mvp' => t('Multi-View player'),
      'cvp' => t('Chromeless player'),
    );
  }

  /**
   * Splits height and width when given size.
   *
   * @param string $size
   *   The size seleced in the display settings.
   * @param int $width
   *   The width pixels.
   * @param int $height
   *   The height pixels.
   *
   * @return array $dimenstions
   *   an array containing height and width indexes.
   */
  public function get_dimensions($size = NULL, $width = NULL, $height = NULL) {
    $dimensions = array();
    if ($size == 'responsive') {
      $dimensions['width'] = '100%';
      $dimensions['height'] = '100%';
    }
    elseif ($size == 'custom') {
      $dimensions['width'] = (int) $width;
      $dimensions['height'] = (int) $height;
    }
    else {
      // Locate the 'x'.
      $strpos = strpos($size, 'x');
      // Width is the first dimension.
      $dimensions['width'] = substr($size, 0, $strpos);
      // Height is the second dimension.
      $dimensions['height'] = substr($size, $strpos + 1, strlen($size));
    }

    return $dimensions;
  }

  /**
   * Curl function to fetch data from Waywire.
   *
   * @param string $path
   *   The path to the api function Example: /api/content/show.
   * @param array $params
   *   Api parameters.
   *
   * @return SimpleXMLElement $simpleXml
   *   The ximpleXml object.
   */
  public function fetch($path = '', $params = array()) {
    $simpleXml = NULL;

    $url = $this->_channel . $path . '?' . http_build_query($params);

    $headers = array(
      "X-Magnify-Key: " . $this->_apiKey,
    );

    $session = curl_init($url);
    curl_setopt($session, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($session, CURLOPT_HTTPHEADER, $headers);
    $reply = curl_exec($session);
    curl_close($session);

    try {
      $simpleXml = @new SimpleXMLElement($reply);
    }
    catch (Exception $e) {
      drupal_set_message(t('Could not parse Waywire XML.'), 'error');
      return FALSE;
    }

    return $simpleXml;
  }

  /**
   * Show api function that sets public varibales.
   *
   * @param int $id
   *   The waywire id.
   *
   * @return bool
   *   Returns FALSE if any error occurs.
   */
  public function show($id) {

    // Call the waywire api.
    $params = array(
      'id' => $id,
      'format' => 'xml',
    );
    $simpleXml = $this->fetch('/api/content/show', $params);

    // Itterate through the xml to get values from xml.
    if ($simpleXml) {
      $nameSpaces = $simpleXml->getDocNamespaces();

      foreach ($simpleXml->entry as $entry) {
        // The video url is stored in the alternate link.
        foreach ($entry->link as $link) {
          $rel = (string) $link->attributes()->rel;
          if ($rel == 'alternate') {
            $this->video_url = (string) $link->attributes()->href;
          }
        }

        $this->updated = (string) $entry->updated;
        $this->published = (string) $entry->published;
        $this->title = (string) $entry->title;
        $media = $entry->children($nameSpaces['media']);
        $this->thumbnail = (string) $media->thumbnail->attributes()->url;
        $this->duration = (int) $media->content->attributes()->duration;
      }
    }
    else {
      drupal_set_message(t('Could not parse Waywire XML.'), 'error');
      return FALSE;
    }

    return TRUE;
  }

}
