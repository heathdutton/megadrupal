<?php

/**
 * Class to parse Facebook URL to array($type, $id)
 *
 * Used inside fbphoto_parser class.
 *
 * @see FBPhotoWebTestCase::testUrlParser()
 */
class fbphoto_url_parser
{
  private $url;

  public function __construct($url = NULL) {
    if (!empty($url)) {
      $this->url = parse_url($url);
    }
  }

  public function parse($url = NULL) {
    if (!empty($url)) {
      $this->url = parse_url($url);
    }

    if ($return = $this->parseOldStyle()) {
      return $return;
    }
    return $this->parseNewStyle();
  }

  private function parseOldStyle() {
    if (strpos($this->url['path'], 'photo.php') !== FALSE) {
      @parse_str($this->url['query'], $query);
      if (isset($query['fbid']) && is_numeric($query['fbid'])) {
        return array('photo', $query['fbid']);
      }
    }
  }

  private function parseNewStyle() {
    @parse_str($this->url['query'], $query);

    if (strpos($this->url['path'], '/media/set/') !== FALSE) {
      return $this->parseAlbum($this->url['path'], $query);
    }
    return $this->parsePhoto($this->url['path']);
  }

  /**
   * Example
   *   https://www.facebook.com/8427738891/photos/a.380743478891.164254.8427738891/380744538891/
   *   Pattern: …/[PHOTO-ID]/
   */
  private function parsePhoto($path) {
    $id = preg_replace('/.*\/(\d+).*/', '$1', $path);
    if (is_numeric($id)) {
      return array('type' => 'photo', 'id' => $id);
    }
  }

  /**
   * Example:
   *   https://www.facebook.com/media/set/?set=a.380743478891.164254.8427738891&type=3
   *   pattern: set=a.[ALBUM-ID].[RANDOM-NUMBER].[PROFILE-ID]
   */
  private function parseAlbum($path, $query) {
    if (isset($query['set']) && strpos($query['set'], 'a.') === 0) {
      $id = explode('.', $query['set']);
      if (!empty($id[1]) && is_numeric($id[1])) {
        return array('type' => 'album', 'id' => $id[1]);
      }
    }
  }
}

/**
 * Class to parse replace pairs for a long text.
 */
class fbphoto_parser
{
  private $text;
  private $url_parser;

  public function __construct($text = NULL) {
    $this->url_parser = new fbphoto_url_parser();

    if (!empty($text)) {
      $this->text = $text;
    }
  }

  public function parse() {
    $find = $replace = array();

    foreach (array(
                 '[fb:' => '/(\[fb:[^\]]*\])/i',
                 '[attach_fb' => '/(\[attach_fb:[^\]]*\])/i',
             ) as $keyword => $pattern) {
      if (FALSE !== stripos($this->text, $keyword)) {
        preg_match_all($pattern, $this->text, $matches);
        if (!empty($matches[1])) {
          foreach ($matches[1] as $match) {
            $find[] = $match;
            $replace[] = $this->buildRepaceItem(substr($match, 4, -1));
          }
        }
      }
    }

    return array($find, $replace);
  }

  public function buildRepaceItem($url) {
    if ($info = $this->url_parser->parse($url)) {
      try {
        switch ($info['type']) {
          case 'album':
            $album = fbphoto_get_album_photos($info['id']);
            $output = theme('fbphoto_album', array('album' => $album));
            return drupal_render($output);

          case 'photo':
            $photo = fbphoto_get_photo($info['id']);
            $output = theme('fbphoto_photo', array('photo' => $photo));
            return drupal_render($output);

          default:
            return '<!--- fbphoto: unsupported -->';
        }
      } catch (Exception $e) {
        watchdog(WATCHDOG_ERROR, $e->getMessage());
        return '<!--- fbphoto: Exception -->';
      }
    }
    return '';
  }
}
