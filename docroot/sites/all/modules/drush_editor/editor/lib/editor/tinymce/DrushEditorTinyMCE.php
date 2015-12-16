<?php

/**
 * @file
 * Drush integration for WYSIWYG editors.
 */

class DrushEditorTinyMCE extends DrushEditorBase {
  /**
   * {@inheritdoc}
   */
  protected $defaultVersion = 3;

  /**
   * {@inheritdoc}
   */
  protected $defaultPreset = NULL;

  /**
   * {@inheritdoc}
   */
  protected $minVersion = '3.5';

  /**
   * {@inheritdoc}
   */
  protected $name = 'tinymce';

  /**
   * Cache threshold, default is 7 days.
   */
  protected $threshold = 604800;

  /**
   * {@inheritdoc}
   */
  protected $urls = array(
    'discover' => 'http://www.tinymce.com/develop/changelog/index.php?type=tinymce',
    'download' => 'https://github.com/downloads/tinymce/tinymce',
  );

  /**
   * Get requested version.
   *
   * @return string
   *   Version.
   */
  public function getRequestedVersion() {
    $version = $this->getVersion();

    switch ($version) {
      case 'head':
        $major_version = $this->getMajorVersion();
        $versions      = $this->discover();
        $version       = reset($versions[$major_version]);

        break;

      case 'dev':
        $major_version = $this->getMajorVersion();
        $version       = $major_version . '.x';
        break;

      default:
        break;
    }

    return $version;
  }

  /**
   * {@inheritdoc}
   */
  public function getDownloadUrl() {
    if ($this->getVersion() == 'dev') {
      return 'http://www.tinymce.com/nightly/tinymce.zip';
    }

    $version = $this->getRequestedVersion();
    $url     = parent::getDownloadUrl();

    return str_replace('__VERSION__', $version, $url) . '/' . $this->getFileName();
  }

  /**
   * Get filename.
   *
   * @return string
   *   File name.
   */
  public function getFileName() {
    $name[] = 'tinymce';
    $name[] = $this->getRequestedVersion();

    $preset = $this->getPreset();

    if (!empty($preset)) {
      $name[] = $preset;
    }

    return implode('_', $name) . '.zip';
  }

  /**
   * {@inheritdoc}
   */
  public function validateRequest() {
    $major_version = $this->getMajorVersion();
    $version       = $this->getVersion();
    $preset        = $this->getPreset();

    // Validate version.
    if ($version == 'head' || $version == 'dev') {
      @list($min_major_version) = explode('.', $this->minVersion, 2);

      if ($major_version < $min_major_version) {
        return drush_set_error('DRUSH_EDITOR_VERSION_TOO_OLD', dt('The requested !editor is too old.', array('!editor' => $this->name)));
      }
    }
    elseif (version_compare($this->minVersion, $version) > 0) {
      return drush_set_error('DRUSH_EDITOR_VERSION_TOO_OLD', dt('!editor-!version is too old.', array('!editor' => $this->name, '!version' => $version)));
    }
    elseif (!empty($version)) {
      $versions = $this->discover();

      if (empty($versions[$major_version][$version])) {
        return drush_set_error('DRUSH_EDITOR_VERSION_NOT_FOUND', dt('!editor-!version is not found.', array('!editor' => $this->name, '!version' => $version)));
      }
    }

    // Validate preset.
    if (!empty($preset)) {
      $manager    = new DrushEditorManager();
      $definition = $manager->get($this->name);

      if (!isset($definition['preset'][$preset])) {
        return drush_set_error('DRUSH_EDITOR_VARIANT_NOT_FOUND',
          dt(
            "!editor !version.x doesn't have the preset !preset. Valid values: !presets",
            array(
              '!editor'  => $this->name,
              '!version' => $major_version,
              '!preset'  => $preset,
              '!presets' => implode(', ', array_keys($definition['preset'])),
            )
          )
        );
      }
    }

    return TRUE;
  }

  /**
   * Read cache from file.
   *
   * @return mixed
   *   Data.
   */
  protected function readCache($cid) {
    $cid   = 'drush_editor_tinymce_' . $cid;
    $cache = drush_trim_path(drush_find_tmp()) . '/' . $cid . '.tmp';

    if ($fh = @fopen($cache, 'rb')) {
      $content = array();

      while ($chunk = fread($fh, 8192)) {
        $content[] = $chunk;
      }

      fclose($fh);

      $content = implode('', $content);

      if ($content = @unserialize($content)) {
        // Cache is hot.
        if (!empty($content['timestamp']) &&
            ($content['timestamp'] + $this->threshold) > time()
        ) {
          return $content['data'];
        }
      }
    }

    return FALSE;
  }

  /**
   * Write data to file.
   *
   * @param string $cid
   *   Cache ID.
   *
   * @param mixed $data
   *   Data.
   *
   * @return bool
   *   Success or failed.
   */
  protected function writeCache($cid, $data) {
    $cid   = 'drush_editor_tinymce_' . $cid;
    $cache = drush_trim_path(drush_find_tmp()) . '/' . $cid . '.tmp';

    if ($fh = @fopen($cache, 'wb')) {
      $content = array(
        'data'      => $data,
        'timestamp' => time(),
      );

      fwrite($fh, serialize($content));
      fclose($fh);

      return TRUE;
    }

    return FALSE;
  }

  /**
   * Discover all versions from website.
   *
   * @return array
   *   List of versions.
   */
  protected function explore($page = 0) {
    $versions = array();
    $url      = $this->getDiscoverUrl();
    $response = drupal_http_request($url);

    if ($response->code == 200) {
      // Load HTML.
      $doc = new DomDocument('1.0', 'utf-8');
      $internal_error = libxml_use_internal_errors(TRUE);
      $doc->loadHTML('<?xml encoding="UTF-8">' . $response->data);
      libxml_clear_errors();
      libxml_use_internal_errors($internal_error);

      // Dirty fix.
      foreach ($doc->childNodes as $item) {
        if ($item->nodeType == XML_PI_NODE) {
          // Remove hack.
          $doc->removeChild($item);
        }
      }

      // Search for versions.
      $xpath = new DOMXpath($doc);
      $nodes = $xpath->query('//*[contains(concat(" ", @class, " "), " ver-release ")]/h2[1]/a[1]');

      // Parse version from HTML.
      for ($i = 0; $i < $nodes->length; ++$i) {
        $version = $nodes->item($i)->nodeValue;
        $version = preg_replace('/[^b0-9.]/', '', $version);

        if (!empty($version)) {
          $compare = version_compare($version, $this->minVersion);

          if ($compare >= 0) {
            @list($major_version) = explode('.', $version, 2);
            $key = str_replace('b', '-beta', $version);
            $versions[$major_version][$key] = $version;
          }
          else {
            break;
          }
        }
      }
    }

    return $versions;
  }

  /**
   * {@inheritdoc}
   */
  public function discover() {
    $cache = &drupal_static('drush:editors:tinymce:discover', array());

    if (empty($cache)) {
      $cache = $this->readCache('versions');

      if ($cache === FALSE) {
        $cache = $this->explore();

        // Order all versions.
        foreach ($cache as $major_version => $minor_versions) {
          uksort($cache[$major_version], 'version_compare');
          $cache[$major_version] = array_reverse($cache[$major_version]);
        }

        $this->writeCache('versions', $cache);
      }
    }

    return $cache;
  }

  /**
   * {@inheritdoc}
   */
  public function download() {
    if ($this->getMajorVersion() == 3 && $this->getVersion() == 'dev') {
      return drush_set_error("Sorry, TinyMCE 3 development release doesn't support direct download.");
    }

    $cache_duration = 86400 * 365;
    $url      = $this->getDownloadUrl();
    $filename = $this->getFileName();
    $tmpdir   = drush_trim_path(drush_find_tmp());
    $path     = drush_download_file($url, $tmpdir . DIRECTORY_SEPARATOR . $filename, $cache_duration);

    if ($path || drush_get_context('DRUSH_SIMULATE')) {
      drush_log("Downloading " . $filename . " was successful.");
    }
    else {
      return drush_set_error('DRUSH_PM_DOWNLOAD_FAILED', 'Unable to download ' . $filename . ' to ' . $tmpdir . ' from ' . $url);
    }

    return $path;
  }
}
