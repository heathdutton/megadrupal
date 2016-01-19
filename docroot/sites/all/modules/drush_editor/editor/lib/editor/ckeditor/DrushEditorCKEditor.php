<?php

/**
 * @file
 * Drush integration for WYSIWYG editors.
 */

class DrushEditorCKEditor extends DrushEditorBase {
  /**
   * {@inheritdoc}
   */
  protected $defaultVersion = 3;

  /**
   * {@inheritdoc}
   */
  protected $defaultPreset = 'full';

  /**
   * {@inheritdoc}
   */
  protected $minVersion = '3.1';

  /**
   * {@inheritdoc}
   */
  protected $name = 'ckeditor';

  /**
   * Cache threshold, default is 7 days.
   */
  protected $threshold = 604800;

  /**
   * {@inheritdoc}
   */
  protected $urls = array(
    'build'    => 'http://ckeditor.com/online-builder/build.php',
    'discover' => 'http://ckeditor.com/whatsnew',
    'download' => 'http://download.cksource.com/CKEditor/CKEditor/CKEditor%20__VERSION__',
  );

  /**
   * {@inheritdoc}
   */
  public function setPreset($preset) {
    if ($this->getMajorVersion() > 3) {
      parent::setPreset($preset);
    }
    else {
      $this->preset = NULL;
    }

    return $this;
  }

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
      return 'https://github.com/ckeditor/ckeditor-releases/archive/latest/' . $this->getPreset() . '.zip';
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
    $name[] = 'ckeditor';
    $name[] = $this->getRequestedVersion();
    $preset = $this->getPreset();

    if (!empty($preset)) {
      $name[] = $preset;
    }

    return implode('_', $name) . '.tar.gz';
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
    if ($major_version > 3) {
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
    $cid   = 'drush_editor_ckeditor_' . $cid;
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
    $cid   = 'drush_editor_ckeditor_' . $cid;
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
    $response = drupal_http_request($url . '?page=' . $page);

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
      $nodes = $xpath->query('//*[@id="content"]/*[contains(concat(" ", @class, " "), " views-row ")]/div[1]/a[1]');
      $next  = FALSE;

      // Parse version from HTML.
      for ($i = 0; $i < $nodes->length; ++$i) {
        $link = $nodes->item($i)->getAttribute('href');

        preg_match('/ckeditor_([a-z0-9.]+)(?:_[a-z]+)?\.(?:zip|tar\.gz)$/i', $link, $matches);

        if (isset($matches[1])) {
          $version = $matches[1];
          $compare = version_compare($version, $this->minVersion);

          if ($compare >= 0) {
            @list($major_version) = explode('.', $version, 2);
            $key = preg_replace('/([a-z]+)$/', '-\1', $version);
            $versions[$major_version][$key] = $version;
            $next = $compare > 0;
          }
          else {
            break;
          }
        }
      }

      // Discover next page.
      if ($next) {
        $next = $page + 1;
        $pager_current = $xpath->query('//*[@id="content"]/*[contains(concat(" ", @class, " "), " pager ")]/a/text()[normalize-space(.)="' . $next . '"]');

        // If we have next page to process.
        if ($pager_current->length) {
          usleep(rand(100, 300));
          $items = $this->explore($next);

          // Merge results.
          foreach ($items as $major_version => $minor_versions) {
            if (isset($versions[$major_version])) {
              $versions[$major_version] = array_merge($versions[$major_version], $minor_versions);
            }
            else {
              $versions[$major_version] = $minor_versions;
            }
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
    $cache = &drupal_static('drush:editors:ckeditor:discover', array());

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
      return drush_set_error("Sorry, CKEditor 3 development release doesn't support direct download. Please use --package-handler=svn --no-source-control instead.");
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
