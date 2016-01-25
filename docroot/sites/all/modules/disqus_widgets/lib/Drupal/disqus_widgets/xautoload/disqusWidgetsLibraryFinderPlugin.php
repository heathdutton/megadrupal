<?php
/**
 * @file
 * FinderPlugin for xautoload to handle the non-PSR-0 compliant Disqus class.
 */

namespace Drupal\disqus_widgets\xautoload;

/**
 * Implements \xautoload\FinderPlugin\Interface
 */
class disqusWidgetsLibraryFinderPlugin implements \xautoload_FinderPlugin_Interface {

  protected $libDir;

  /**
   * Plugin constructor.
   *
   * @param string $lib_dir
   *   Path to the library folder to search within.
   */
  public function __construct($lib_dir) {

    $this->libDir = $lib_dir;
  }


  /**
   * Provides a path to the phpColors library.
   *
   * @param xautoload_InjectedAPI_findFile $api
   *   An object with a suggestFile() method.
   *   We are supposed to suggest files until suggestFile() returns TRUE, or we
   *   have no more suggestions.
   *
   * @param string $path_fragment
   *   The key that this plugin was registered with.
   *   With trailing DIRECTORY_SEPARATOR.
   *
   * @param string $path_suffix
   *   Second part of the canonical path, ending with '.php'.
   *
   * @return bool
   *   TRUE on success, FALSE on failure.
   */
  public function findFile($api, $path_fragment, $path_suffix) {

    /*
      This winds up getting called more than once because the autoloader fires
      for each class located in disqusapi.php. This results in an error about
      redifining classes, so we need to do a check and exit early if the class
      is already defined.
     */
    if (class_exists('DisqusAPI')) {

      return FALSE;
    }

    if ('.' === dirname($path_suffix)) {

      $basename = strtolower(basename($path_suffix));
      $path = $this->libDir . '/disqusapi/' . $basename;

      if ($api->suggestFile($path)) {

        return TRUE;
      }
    }

    return FALSE;
  }
}
