<?php

/**
 * @file
 * Pre-bootstrap multisite:host mapping tool.
 *
 * Common pattern is:
 * Live:
 *   arbitrary.name
 * Preprod:
 *   short.name.pp[.hirn.local]
 * Stage:
 *   short.name.stg[.hirn.local]
 * Local devbox:
 *   any.name[.anything.else]
 */

// The following line works around (IMO avoidable) drush issues:
if (!class_exists('DrupalSiteMatcher')) {

  class DrupalSiteMatcher {

    private $config;
    private $drush_alias;
    private $drush_uri;

    public $internals = array();
    public $settings = array();
    public $conf = array();

    /**
     * Try to load a settings include and set a fallback if failing.
     *
     * @param string $key
     *  The settings include prefix (e.g. 'sites' for 'sites.settings.inc').
     * @param array $fallback
     *  Arbitrary data to set if loading fails. Defaults to
     *  an empty array.
     *
     * @return bool
     *   Success status: TRUE, if the data could be loaded.
     *   FALSE, if not.
     */
    function loadConfig($key, $fallback = array()) {

      // Assure usable defaults.
      $data = $fallback = is_array($fallback) ? $fallback : array();

      $success = TRUE;

      $drupal_root = defined('DRUPAL_ROOT') ? DRUPAL_ROOT : dirname( __FILE__ ) . '/../../../..';

      try {
        include "${drupal_root}/sites/default/sitematcher/includes/${key}.settings.inc";
      } catch (Exception $e) {
        $success = FALSE;
      }

      $this->config[$key] = $data;

      return $success;
    }

    /**
     * Merge a data structure (environment, site, include) into $instance->$key.
     *
     * @param array $data
     *   A data structure array, containing 'settings', 'internals' and/or
     *   conf sub-arrays.
     */
    private function mapData($data) {

      foreach (array_keys($data) as $k) {
        if (isset($data[$k])) {
          $this->$k = array_merge_recursive((array) $this->$k, (array) $data[$k]);
        }
      }

    }

    /**
     * Run one ore more regex matches to find out the appropriate dataset to map.
     *
     * @param array $section
     *   A section array, containing multiple data structure arrays.
     *   Must contain an 'internals' array with a value for 'regex' set.
     *
     * @return string|bool
     *   The first matching key from $section or FALSE if nothing matched
     *   and no fallback section is defined.
     */
    private function matchData($section) {
      foreach ($section as $id => $data) {
        if (!empty($data['internals']['regex'])) {
          foreach ((array) $data['internals']['regex'] as $pattern) {
            if (preg_match($pattern, $this->internals['sitename'])) {
              return $id;
            }
          }
        }
      }
      return !empty($section['*']) && is_array($section['*']) ? '*' : FALSE;
    }

    /**
     * Loads, matches and maps a section.
     *
     * @param string $section
     *   Name of a section to be processed.
     *
     * @return string|bool result
     *   If the section could be loaded, the name of the section.
     *   FALSE, if the section failed to load.
     */
    private function processSection($section) {

      $this->loadConfig($section);
      if ($found = $this->matchData($this->config[$section])) {
        $this->mapData(@$this->config[$section][$found]);
      }

      // Always apply '+' section, if defined.
      if (!empty($this->config[$section]['+'])) {
        $this->mapData(@$this->config[$section]['+']);
        if ($found == FALSE) {
          $found = '+';
        }
      }

      return $found;

    }

    /**
     * Constructor.
     *
     * To keep usage of this class simple, the constructor fills instance
     * properties with mandatory and optional data from includes.
     * These properties are publicly accessible and can be used e.g.
     * in settings.php or with other bootstrap components.
     *
     * @param string $sitename
     *   (Optional) overrides $_SERVER['SERVER_NAME'], if set.
     */
    public function __construct($sitename = NULL) {

      $this->parseArgs();
      $this->guessUri();

      // 1. Determine site name with various fallbacks (finally 'default').
      $sitename = $sitename;

      if (!isset($sitename) && !empty($_SERVER['SERVER_NAME'])) {
        $sitename = $_SERVER['SERVER_NAME'];
      }

      if (!isset($sitename) && isset($_SERVER['HTTP_HOST'])) {
        $sitename = preg_replace('#:[0-9]+$#', '', (string) $_SERVER['HTTP_HOST']);
      }

      // Drush does not provide site related information at this stage.
      // @see self::parseArgs()
      if (!isset($sitename) && isset($this->drush_uri)) {
        $sitename = $this->drush_uri;
      }

      if (empty($sitename)) {
        $sitename = 'default';
      }

      $this->internals['sitename'] = $sitename;

      // 2. Map sites and fail if not set.
      if ($s = $this->processSection('sites')) {
        $this->internals['site'] = preg_match('#^(\*|\+)$#', $s) ? 'default' : $s;
      }
      else {
        throw new Exception('No sites file has been defined.');
      }

      // 3. Map further includes. Iterate until all includes are mapped.
      $this->internals['includes_processed'] = array();
      $includes = array('environments');

      do {
        $missing = array_diff($includes, array_keys($this->internals['includes_processed']));
        foreach ($missing as $include) {
          $success = $this->processSection($include) !== FALSE;
          // Add in any case, to avoid deadlocks on load errors.
          // Track errors via key->value.
          $this->internals['includes_processed'][$include] = $success;
        }
        // processSection may add new includes/dependencies and then stores them
        // internally. So we can merge-update our internal to-do-list from there
        // for the next iteration. It also contains all includes already mapped
        // when processing the sites include above, which will be executed
        // now as well.
        $includes = array_keys(array_fill_keys(array_merge($includes, (array) $this->internals['includes']), TRUE));
      } while (!empty($missing));
    }

    /**
     * Display a list of all site IDs found in the current sites.settings.inc.
     *
     * @return array
     *   A list of defined site IDs, e.g. for use with drush etc.
     */
    public function getSites() {
      $sites = array();
      foreach ($this->config['sites'] as $k => $v) {
        switch ($k) {
          case '+':
            break;
          case '*':
            $sites[] = $v['internals']['site'];
            break;
          default:
            $sites[] = $k;
        }
      }
      return $sites;
    }

    /**
     * Tries to determine the hostname from drush environment.
     */
    private function parseArgs() {
      if ($a = @$GLOBALS['argv']) {
        foreach ($a as $v) {
          if (preg_match('#^--uri=(?<uri>[^\s]+)$#', $v, $m)) {
            $this->drush_uri = $m['uri'];
          }
          elseif (preg_match('#^@(?<alias>[^\s]+)$#', $v, $m)) {
            $this->drush_alias = $m['alias'];
          }
        }
      }
    }

    /**
     * Tries to guess the hostname from a drush alias, if given.
     */
    private function guessUri() {
      if (!isset($this->drush_uri) && isset($this->drush_alias) && function_exists('_drush_sitealias_find_and_load_all_aliases')) {
        if ($sites = _drush_sitealias_find_and_load_all_aliases()) {
          if ($uri = @$sites[$this->drush_alias]['uri']) {
            $this->drush_uri = $uri;
          }
        }
      }
    }
  }
}
