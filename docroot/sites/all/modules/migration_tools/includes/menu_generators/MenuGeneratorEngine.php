<?php
/**
 * @file
 * MenuGeneratorEngine class.
 */

/**
 * Class MenuGeneratorEngine.
 *
 * The engine parses the source to build the menu import file.
 */
abstract class MenuGeneratorEngine {
  /**
   * Menu generation parameters.
   * @var MenuGenerationParameters
   */
  protected $parameters;
  /**
   * The content that will be written into the menu import file on save.
   * @var string
   */
  public $menuFileContents;


  /**
   * Take a uri and map them to the local uris from migrated pages or other.
   *
   * @param string $uri
   *   the legacy uri coming from the menu in the legacy site.
   *
   * @return string
   *   The local uri to which the legacy uri is being redirected or full url.
   */
  public function normalizeUri($uri) {
    $uri = UrlTools::normalizePathEnding($uri);

    $schema_and_domain = $this->parameters->getDomain();
    $local_base_path = $this->parameters->getSubDirectory();
    $url_destination_path = $schema_and_domain . $local_base_path;
    $parsed_url_destination = parse_url($url_destination_path);
    $url_menu_source_path = $this->parameters->getUriMenuLocationBasePath();
    $parsed_url_source = parse_url($url_menu_source_path);
    $parsed_url = parse_url($uri);

    // Determine type of path. (relative, root relative, absolute, external).
    $type = '';
    if ((!empty($parsed_url['host'])) && ($parsed_url['host'] != $parsed_url_destination['host'])) {
      $type = 'external';
    }
    elseif ((!empty($parsed_url['host'])) && ($parsed_url['host'] == $parsed_url_destination['host'])) {
      $type = 'absolute';
    }
    elseif (!empty($parsed_url['path'])) {
      if (strpos($parsed_url['path'], '/') === 0) {
        $type = 'root relative';
      }
      else {
        // Covers the case of either './somefile.htm' or 'somefile.htm'.
        $type = 'self-relative';
      }

      if (strpos($parsed_url['path'], '../') === 0) {
        $type = 'relative';
      }
    }

    switch ($type) {
      case 'external':
        // It is external, no further processing is needed.
        return $uri;

      case 'self-relative':
        // Make it relative by removing the inconsequential './'.
        if (strpos($parsed_url['path'], './') === 0) {
          $parsed_url['path'] = substr($parsed_url['path'], 0, 2);
        }
      case 'relative':
        // Make it root relative by prepending the base path of the source.
        $subpath = trim($parsed_url_source['path'], '/');
        $move_ups = substr_count($parsed_url['path'], '../');
        if ($move_ups) {
          $path_array = explode('/', $subpath);
          $items = count($path_array);
          // Remove one path element for each '../' present.
          for ($i = 1; $i <= $move_ups; $i++) {
            $index = $items - $i;
            unset($path_array[$index]);
          }
          if (!empty($path_array)) {
            $subpath = '/' . implode('/', $path_array) . '/';
          }
          else {
            $subpath = '/';
          }
          $parsed_url['path'] = str_replace('../', '', $parsed_url['path']);
        }
        $parsed_url['path'] = rtrim($subpath, '/') . '/' . ltrim($parsed_url['path'], '/');
        break;

      case 'absolute':
      case 'root relative':
      default:
        // These cases need no special handling.
        break;
    }

    // @codingStandardsIgnoreStart
    /*
     * Not needed for csv import.  Revisit for Html Crawl.
    // If the url is pointing to a directory and not a file, let's fix that.
    if (strcmp(substr($parsed_url['path'], -1), "/") == 0) {
      $parsed_url['path'] = "{$parsed_url['path']}index.html";
      // @TODO Seems brittle, hardcoding just the index.html by default.
      // It would be more reliable attempt to load the variants and use
      // whichever one exists, but slower, so not used for now.
    }
    */
    // @codingStandardsIgnoreEnd

    $group_path = rtrim($this->parameters->initialMenuLocation, '/');
    if (!empty($parsed_url['fragment'])) {
      // The uri contains a # which is not supported by the menu import, so
      // return a full path to ensure no drupal processing.
      $uri = UrlTools::reassembleURL($parsed_url, TRUE);
    }
    elseif (stripos($parsed_url['path'], "{$group_path}/index.html") !== FALSE ||
      stripos($parsed_url['path'], "{$group_path}/index.htm") !== FALSE) {
      // The index.(html|htm) pages are aliased to the group page, so adjust.
      $uri = $this->parameters->getSubDirectory();
    }
    else {
      // This is an internal uri so see what it redirects to.
      $redirect_to_lookup = UrlTools::reassembleURL($parsed_url, FALSE);
      $uri = UrlTools::convertLegacyToAlias($redirect_to_lookup);
    }

    return $uri;
  }


  /**
   * Generate triggers the build and returns $menuFileContent.
   *
   * @return string
   *   $menuFileContent
   */
  abstract public function generate();

}
