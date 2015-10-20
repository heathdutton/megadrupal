<?php
/**
 * @file
 * Functions for handling urls.
 */

class UrlTools {

  /**
   * Grabs legacy redirects for this node from D6 and adds $row->redirects.
   *
   * This function needs to be called in prepareRow() of your migration.
   *
   * @param object $row
   *   The object of this row.
   * @param string $db_reference_name
   *   The Drupal name/identifier of the legacy database.
   * @param object $source_connection
   *   Database source connection from migration.
   */
  public static function collectD6RedirectsToThisNode($row, $db_reference_name, $source_connection) {
    // Gather existing redirects from legacy.
    $row->redirects = Database::getConnection($db_reference_name, $source_connection)
      ->select('path_redirect', 'r')
      ->fields('r', array('source'))
      ->condition('redirect', "node/$row->nid")
      ->execute()
      ->fetchCol();
  }

  /**
   * Take a legacy uri, and map it to an alias.
   *
   * @param string $legacy_uri
   *   A legacy uri gets redirected to a node.
   *
   * @return string
   *   The alias matching the legacy uri, or an empty sting.
   */
  public static function convertLegacyToAlias($legacy_uri) {
    // Most common drupal paths have no ending / so start with that.
    $legacy_uri_no_end = rtrim($legacy_uri, '/');
    $redirect = redirect_load_by_source($legacy_uri_no_end);
    if (empty($redirect) && ($legacy_uri != $legacy_uri_no_end)) {
      // There is no redirect found, lets try looking for one with the path/.
      $redirect = redirect_load_by_source($legacy_uri);
    }
    if ($redirect) {
      $nid = str_replace("node/", "", $redirect->redirect);
      $node = node_load($nid);

      if ((!empty($node)) && (!empty($node->path)) && (!empty($node->path['alias']))) {
        return $node->path['alias'];
      }

      // Check for language other than und, because the aliases are
      // intentionally saved with language undefined, even for a spanish node.
      // A spanish node, when loaded does not find an alias.
      if (!empty($node->language) && ($node->language != LANGUAGE_NONE)) {
        // Some other language in play, so lookup the alias directly.
        $path = url($redirect->redirect);
        $path = ltrim($path, '/');
        return $path;
      }

      if ($node) {
        $uri = entity_uri("node", $node);
        if (!empty($uri['path'])) {
          return $uri['path'];
        }
      }
    }
    $message = "legacy uri @legacy_uri does not have a node associated with it";
    MigrationMessage::makeMessage($message, array('@legacy_uri' => $legacy_uri), WATCHDOG_NOTICE, 1);
    // Without legacy path yet migrated in, leave the link to source url
    // so that the redirects can handle it when that content is migrate/created.
    $base = variable_get('migration_tools_base_domain', '');
    if (!empty($base)) {
      return "{$base}/{$legacy_uri}";
    }
    else {
      throw new Exception("The base domain is needed, but has not been set. Visit /admin/config/migration_tools");
    }

  }

  /**
   * Generates a legacy file path based on a row's original path.
   *
   * @param object $row
   *   The row being imported.
   */
  public static function generateLegacyPath($row) {
    // $row->url_path can be used as an identifer, whereas $row->legacy_path
    // may have multiple values.

    // @TODO Need to alter connection to old path but it won't come from fileid.
    $row->url_path = substr($row->fileid, 1);
    $row->legacy_path = $row->url_path;
  }


  /**
   * Convert a relative url to absolute.
   *
   * @param string $rel
   *   Relative url.
   * @param string $base
   *   Base url.
   * @param string $subpath
   *   An optional sub-path to check for when translating relative URIs that are
   *   not root based.
   *
   * @return string
   *   The relative url transformed to absolute.
   */
  public static function convertRelativeToAbsoluteUrl($rel, $base, $subpath = '') {
    // Return if already absolute URL.
    if (parse_url($rel, PHP_URL_SCHEME) != '') {
      return $rel;
    }

    // Check for presence of subpath in $rel to see if a subpath is missing.
    if ((!empty($subpath)) && (!stristr($rel, $subpath))) {
      // The subpath is not present, so add it.
      $rel = $subpath . '/' . $rel;
    }

    // Queries and anchors.
    if ($rel[0] == '#' || $rel[0] == '?') {
      return $base . $rel;
    }

    // Parse base URL and convert to local variables:
    // $scheme, $host, $path.
    extract(parse_url($base));

    // Remove non-directory element from path.
    $path = preg_replace('#/[^/]*$#', '', $path);

    // Destroy path if relative url points to root.
    if ($rel[0] == '/') {
      $path = '';
    }

    // Dirty absolute URL.
    $abs = "$host$path/$rel";

    // Replace '//' or '/./' or '/foo/../' with '/'.
    $re = array('#(/\.?/)#', '#/(?!\.\.)[^/]+/\.\./#');
    for ($n = 1; $n > 0; $abs = preg_replace($re, '/', $abs, -1, $n)) {
    }

    // Absolute URL is ready.
    return $scheme . '://' . $abs;
  }

  /**
   * Creates a redirect from a legacy path if one does not exist.
   *
   * @param string $source_path
   *   The path or url of the legacy source. MUST be INTERNAL to this site.
   * @param string $destination
   *   The destination of the redirect examples:
   *     * path-a/path-b/the-node-title
   *     * http://www.somesite.com
   * @param object $destination_node
   *   (required if the redirect is a node) Node object of the destination node.
   * @param array $allowed_hosts
   *   If passed, this will limit redirect creation to only urls that have a
   *   domain present in the array. Others will be rejected.
   */
  public static function createRedirect($source_path, $destination, $destination_node = '', $allowed_hosts = array()) {
    $alias = $destination;

    // We can not create a redirect for a URL that is not part of the domain
    // or subdomain of this site.
    if (!self::isAllowedDomain($source_path, $allowed_hosts)) {
      $message = "A redirect was NOT built for @source_path because it is not an allowed host.";
      $variables = array(
        '@source_path' => $source_path,
      );
      MigrationMessage::makeMessage($message, $variables, FALSE, 2);
      return FALSE;
    }

    if (!empty($source_path)) {
      // Alter source path to remove any externals.
      $source_path = self::fixSchemelessInternalUrl($source_path);
      $source = parse_url($source_path);
      $source_path = (!empty($source['path'])) ? $source['path'] : '';
      // A path should not have a preceeding /.
      $source_path = ltrim($source['path'], '/');
      $source_options = array();
      // Check for fragments (after #hash ).
      if (!empty($source['fragment'])) {
        $source_options['fragment'] = $source['fragment'];
      }
      // Check for query parameters (after ?).
      if (!empty($source['query'])) {
        parse_str($source['query'], $query);
        $source_options['query'] = $query;
      }

      if (!empty($destination_node)) {
        if (!empty($destination_node->nid)) {
          $destination = 'node/' . $destination_node->nid;
        }
      }
      // Check to see if the source and destination or alias are the same.
      if (($source_path !== $destination) && ($source_path !== $alias)) {
        // The source and destination are different, so make the redirect.
        $redirect = redirect_load_by_source($source_path);
        if (!$redirect) {
          // The redirect does not exists so create it.
          $redirect = new stdClass();
          redirect_object_prepare($redirect);
          $redirect->source = $source_path;
          $redirect->source_options = $source_options;
          $redirect->redirect = $destination;

          redirect_save($redirect);
          $message = 'Redirect created: @source ---> @destination';
          $variables = array(
            '@source' => $source_path,
            '@destination' => $redirect->redirect,
          );
          MigrationMessage::makeMessage($message, $variables, FALSE, 2);
        }
        else {
          // The redirect already exists.
          $message = 'The redirect of @legacy already exists pointing to @alias. A new one was not created.';
          $variables = array(
            '@legacy' => $source_path,
            '@alias' => $redirect->redirect,
          );
          MigrationMessage::makeMessage($message, $variables, FALSE, 2);
        }
      }
      else {
        // The source and destination are the same. So no redirect needed.
        $message = 'The redirect of @source have idential source and destination. No redirect created.';
        $variables = array(
          '@source' => $source_path,
        );
        MigrationMessage::makeMessage($message, $variables, FALSE, 2);
      }
    }
    else {
      // The is no value for redirect.
      $message = 'The source path is missing. No redirect can be built.';
      $variables = array();
      MigrationMessage::makeMessage($message, $variables, FALSE, 2);
    }
  }

  /**
   * Adds multiple redirects to the same destination.
   *
   * This is typically called within the migration's complete().
   *
   * @param array $redirects
   *   An internal paths to build redirects FROM (ex: array('path/blah', 'foo'))
   * @param string $destination
   *   The destination of where the redirect should go TO (ex: 'node/123')
   */
  public static function createRedirectsMultiple(array $redirects, $destination) {
    foreach ($redirects as $redirect) {
      self::createRedirect($redirect, $destination);
    }
  }

  /**
   * Removes the host if the url is intarnal but malformed.
   *
   * A url of 'mysite.com/path1/path2' is malformed because parse_url() will
   * not recognise 'mysite.com' as the host without the scheme (http://) being
   * present.  This method will remove the host if it is for this site and make
   * the url a proper root relative path.
   *
   * @param string $url
   *   A url.
   *
   * @return string
   *   A url or path correctly modified for this site.
   */
  public static function fixSchemelessInternalUrl($url) {
    if (!empty($url)) {
      $parsed_url = parse_url($url);
      if (empty($parsed_url['scheme'])) {
        // It has no scheme, so check if it is a malformed internal url.
        $host = self::getSiteHost();
        $pos = stripos($url, $host);
        if ($pos === 0) {
          // The url is starting with a site's host.  Remove it.
          $url = substr_replace($url, '', $pos, strlen($host));
        }
      }
    }
    return $url;
  }

  /**
   * Returns the defined site host.
   *
   * @return string
   *   The site host for this site example: 'mysite.com'.
   *
   * @throws MigrateException
   *   If the base domain has not been defined in /admin/config/migration_tools.
   */
  public static function getSiteHost() {
    // Obtain the designated url of the site.
    $base_url = variable_get('migration_tools_base_domain', '');
    $site_host = parse_url($base_url, PHP_URL_HOST);
    if (!empty($site_host)) {
      return $site_host;
    }
    else {
      // There is no site host defined.
      $message = "The base domain is needed, but has not been set. Visit /admin/config/migration_tools \n";
      throw new MigrateException($message);
    }
  }

  /**
   * Examines an url to see if it is within a allowed list of domains.
   *
   * @param string $url
   *   A url.
   * @param array $allowed_hosts
   *   A flat array of allowed domains. ex:array('www.site.com', 'site.com').
   *
   * @return bool
   *   TRUE if the host is within the array of allowed.
   *   TRUE if the array of allowed is empty (nothing to compare against)
   *   FALSE if the domain is not with the array of allowed.
   */
  public static function isAllowedDomain($url, $allowed_hosts) {
    $url = self::fixSchemelessInternalUrl($url);
    $host = parse_url($url, PHP_URL_HOST);
    // Treat it as allowed until evaluated otherwise.
    $allowed = TRUE;
    if (!empty($allowed_hosts) && (is_array($allowed_hosts)) && (!empty($host))) {
      // See if the host is allowed (case insensitive).
      $allowed = in_array(strtolower($host), array_map('strtolower', $allowed_hosts));
    }
    return $allowed;
  }


  /**
   * Examines an url to see if it is internal to this site.
   *
   * @param string $url
   *   A url.
   *
   * @param array $allowed_hosts
   *   Optional:  A flat array of allowed domains. Uses base url admin setting.
   *   ex:array('www.site.com', 'site.com').
   *
   * @return bool
   *   TRUE if the host is within the site.
   *   TRUE if there is no host (relative link).
   *   FALSE if the domain is not with this site.
   */
  public static function isInternalUrl($url, $allowed_hosts = array()) {
    if (empty($allowed_hosts)) {
      // Use the defined site host.
      $site_host = self::getSiteHost();
      $allowed_hosts = array($site_host);
    }

    if (!empty($allowed_hosts)) {
      return self::isAllowedDomain($url, $allowed_hosts);
    }
    else {
      // There is insufficient information to determine whether host is allowed.
      $message = "Unable to determine if this is internal link as no allowed hosts are specified.\n";
      throw new MigrateException($message);
    }
  }

  /**
   * Normalize the path to make sure paths are consistent.
   *
   * @param string $uri
   *   A uri.
   *
   * @return string
   *   The cleaned uri. with path ending in / if not a file.
   */
  public static function normalizePathEnding($uri) {
    $uri = trim($uri);
    // If the uri is a path, not ending in a file, make sure it ends in a '/'.
    if (!empty($uri) && !pathinfo($uri, PATHINFO_EXTENSION)) {
      $uri = rtrim($uri, '/');
      $uri .= '/';
    }
    return $uri;
  }

  /**
   * Take parse_url formatted url and return the url/uri as a string.
   *
   * @param array $parsed_url
   *   An array in the format delivered by php php parse_url().
   * @param bool $return_url
   *   Toggles return of full url if TRUE, or uri if FALSE (defaults: TRUE)
   *
   * @return string
   *   URL or URI.
   */
  public static function reassembleURL($parsed_url, $return_url = TRUE) {
    $url = '';
    if ($return_url) {
      $default_base = variable_get('migration_tools_base_domain', '');
      $default_scheme = parse_url($default_base, PHP_URL_SCHEME);
      $default_host = parse_url($default_base, PHP_URL_HOST);
      $url .= (!empty($parsed_url['scheme'])) ? $parsed_url['scheme'] . '://' : $default_scheme . '://';

      if ((empty($default_base)) && (empty($parsed_url['host']))) {
        throw new Exception("The base domain is needed, but has not been set. Visit /admin/config/migration_tools");
      }
      else {
        // Append / after the host to account for it being removed from path.
        $url .= (!empty($parsed_url['host'])) ? $parsed_url['host'] . '/' : $default_host . '/';
      }

    }
    // Trim the initial '/' to be Drupal friendly in the event of no host.
    $url .= (!empty($parsed_url['path'])) ? ltrim($parsed_url['path'], '/') : '';
    $url .= (!empty($parsed_url['query'])) ? '?=' . $parsed_url['query'] : '';
    $url .= (!empty($parsed_url['fragment'])) ? '#' . $parsed_url['fragment'] : '';

    return $url;
  }
}
