<?php

/**
 * @file
 * Drush integration for WYSIWYG editors.
 */

abstract class DrushEditorBase implements DrushEditorInterface {
  /**
   * Default version.
   */
  protected $defaultVersion = NULL;

  /**
   * Default preset.
   */
  protected $defaultPreset = NULL;

  /**
   * Major version.
   */
  protected $majorVersion = NULL;

  /**
   * Minimum accepted version.
   */
  protected $minVersion = NULL;

  /**
   * Editor name.
   */
  protected $name = NULL;

  /**
   * URLs to server.
   */
  protected $urls = array(
    'download' => '',
    'build'    => '',
    'discover' => '',
    'git'      => '',
    'svn'      => '',
  );

  /**
   * Preset.
   */
  protected $preset = NULL;

  /**
   * Version.
   */
  protected $version = NULL;

  /**
   * Base Constructor.
   */
  public function __construct($major_version = NULL, $version = NULL, $preset = NULL) {
    $this->setMajorVersion($major_version);
    $this->setVersion($version);
    $this->setPreset($preset);
  }

  /**
   * Get major version.
   *
   * @return string
   *   Version.
   */
  protected function getMajorVersion() {
    return $this->majorVersion;
  }

  /**
   * Set major version.
   *
   * @param string $version
   *   Major version.
   *
   * @return DrushEditorBase
   *   Editor.
   */
  protected function setMajorVersion($version) {
    $this->majorVersion = empty($version) ? $this->defaultVersion : $version;
    return $this;
  }

  /**
   * Get editor version.
   *
   * @return string
   *   Version.
   */
  public function getVersion() {
    return $this->version;
  }

  /**
   * Set version.
   *
   * @param string $version
   *   Version.
   *
   * @return DrushEditorBase
   *   Editor.
   */
  public function setVersion($version) {
    $this->version = empty($version) ? 'head' : $version;
    return $this;
  }

  /**
   * Get editor preset.
   *
   * @return string
   *   Preset.
   */
  public function getPreset() {
    return $this->preset;
  }

  /**
   * Set editor preset.
   *
   * @param string $preset
   *   Preset.
   *
   * @return DrushEditorBase
   *   Editor.
   */
  public function setPreset($preset) {
    $this->preset = empty($preset) ? $this->defaultPreset : $preset;;
    return $this;
  }

  /**
   * Get editor name.
   *
   * @return string
   *   Name.
   */
  public function getName() {
    return $this->name;
  }

  /**
   * Get editor name.
   *
   * @return string
   *   Name.
   */
  public function getLibraryName() {
    return $this->getName();
  }

  /**
   * Get URL.
   *
   * @return string
   *   URL.
   */
  public function getUrl($name) {
    return isset($this->urls[$name]) ? $this->urls[$name] : FALSE;
  }

  /**
   * Get URLs.
   *
   * @return array
   *   List of URLs.
   */
  public function getUrls() {
    return $this->urls;
  }

  /**
   * Get download url.
   *
   * @return string
   *   URL.
   */
  public function getDownloadUrl() {
    return $this->getUrl('download');
  }

  /**
   * Get build url.
   *
   * @return string
   *   URL.
   */
  public function getBuildUrl() {
    return $this->getUrl('build');
  }

  /**
   * Get discover url.
   *
   * @return string
   *   URL.
   */
  public function getDiscoverUrl() {
    return $this->getUrl('discover');
  }

  /**
   * Get git url.
   *
   * @return string
   *   URL.
   */
  public function getGitUrl() {
    return $this->getUrl('git');
  }

  /**
   * Get svn url.
   *
   * @return string
   *   URL.
   */
  public function getSvnUrl() {
    return $this->getUrl('svn');
  }

  /**
   * Validate drush request.
   *
   * @return bool
   *   Success or Fail.
   */
  public function validateRequest() {
    return TRUE;
  }

  /**
   * {@inheritdoc}
   */
  public function discover() {
    return array();
  }

  /**
   * {@inheritdoc}
   */
  public function select($version) {
    @list($major_version) = explode('.', $version, 2);
    $this->setMajorVersion($major_version);
    $this->setVersion($version);
    $this->setPreset($this->getPreset());
  }

  /**
   * {@inheritdoc}
   */
  public function download() {
    return FALSE;
  }

  /**
   * {@inheritdoc}
   */
  public function build() {
    return FALSE;
  }

  /**
   * {@inheritdoc}
   */
  public function checkoutGit() {
    return FALSE;
  }

  /**
   * {@inheritdoc}
   */
  public function checkoutSvn() {
    return FALSE;
  }

  /**
   * {@inheritdoc}
   */
  public function checkout($vcs = 'git') {
    if (empty($vcs)) {
      return FALSE;
    }

    $method = 'checkout' . drupal_ucfirst(drupal_strtolower($vcs));
    $method = array($this, $method);

    if (is_callable($method)) {
      call_user_func_array($method, array());
    }

    return FALSE;
  }
}
