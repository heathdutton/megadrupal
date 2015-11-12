<?php
/**
 * @file
 * MenuGenerationParametersHtmlCrawl class.
 */

/**
 * Class MenuGenerationParametersHtmlCrawl for a generating a menu_import file.
 */
class MenuGenerationParametersHtmlCrawl extends MenuGenerationParameters {
  public $initialMenuLocation;
  public $menuCounter;

  public $recurse;
  private $uriLocalBase;
  private $uriMenuLocation;
  private $uriMenuLocationBasePath;


  /**
   * Constructor.
   */
  public function __construct($subdirectory) {
    parent::__construct();
    $this->setSubDirectory($subdirectory);

    // Defaults.
    // Defaults.
    $this->uriMenuLocation = $this->domain . "/" . $this->subDirectory;
    $this->setUriMenuLocationBasePath();

    $this->uriLocalBase = $this->getSubDirectory();
  }

  /**
   * Setter.
   */
  public function setRecurse($recurse) {
    $this->recurse = ($recurse == 'TRUE' || $recurse === TRUE) ? TRUE : FALSE;
  }

  /**
   * Setter.
   */
  public function setUriMenuLocation($uri_menu_location) {
    // If it is not an absolute path, make it one.
    if (stripos($uri_menu_location, '://') === FALSE) {
      $this->uriMenuLocation = $this->domain . "/" . $uri_menu_location;
    }
    else {
      $this->uriMenuLocation = $uri_menu_location;
    }
    $this->setUriMenuLocationBasePath();
  }

  /**
   * Sets the location of the menu being processed minus the filename.
   */
  public function setUriMenuLocationBasePath($url = NULL) {
    $url = (empty($url)) ? $this->getUriMenuLocation() : $url;
    // Pick apart the url and remove the file name.
    $parsed_url = parse_url($url);
    if (!empty($parsed_url['path'])) {
      // It has a path, so check for an extension.
      $parsed_path = pathinfo($parsed_url['path']);
      if (!empty($parsed_path['extension'])) {
        // It has an extention so get rid of the filename.extension.
        $needle = $parsed_path['filename'] . '.' . $parsed_path['extension'];
        $url = str_replace($needle, '', $url);
      }
    }

    $this->uriMenuLocationBasePath = $url;
  }

  /**
   * Getter.
   */
  public function getUriMenuLocation() {
    return $this->uriMenuLocation;
  }
}
