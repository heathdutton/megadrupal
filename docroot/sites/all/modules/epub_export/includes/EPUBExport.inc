<?php
/**
 * @file
 * Main integration class for working with FunnyMonkey\EPUB\EPUBPackage class.
 */


use FunnyMonkey\EPUB\EPUBPackage as FMEPub;

define('EPUB_EXPORT_NOLOAD', 1);
define('EPUB_EXPORT_NORENDER', 2);

class EPUBExport {
  /**
   * @var array of assets that are related to the primary piece of content
   */
  protected $assets = array();

  /**
   * @var  array of export items that will be added to the export
   */
  protected $exports = array();

  /**
   * @var string stores the entity type
   */
  protected $epub_export_type = '';

  /**
   * @var int entity id
   */
  protected $id = 0;

  /**
   * @var string htmlid concated entity_type and entity id
   */
  protected $htmlid = '-0';

  /**
   * @var DOMDocument stores the DOMDocument representation of the rendered
   * content for adjusting markup and content as necessary
   */
  protected $dom = NULL;

  /**
   * @var string representing the current asset path alias
   */
  protected $alias = '';

  /**
   * @var string representing the amount of redirection required to reference
   * other items.
   */
  protected $pathRedirection = '';

  /**
   * @var mixed stdClass this will generally be a loaded entity object from
   * the appropriate entity_load() function.
   */
  public $entity = NULL;

  /**
   * @var string holds the rendered entity object
   */
  public $rendered = '';

  /**
   * Constructor.
   *
   * @param string $entity_type
   *   String indicating the entity type
   * @param int $id
   *   Entity id to load for the given type
   */
  public function __construct($entity_type, $id = 0) {
    $this->epub_export_type = $entity_type;
    $this->id = $id;
    $this->htmlid = $this->sanitizeID($this->epub_export_type . '-' . $id);
    try {
      $this->load();
    }
    catch (Exception $e) {
      return FALSE;
    }

    return TRUE;
  }

  /**
   * Returns a context useful for calls to drupal_alter, etc.
   */
  public function context() {
    return array(
      'entity_type' => $this->epub_export_type,
      'id' => $this->id,
    );
  }

  /**
   * Attempts to load the requested entity into the object.
   *
   * After calling the load method the DOM will be loaded and rendered via the
   * appropriate drupal calls. This will also do any basic setup to remove
   * extraneous markup via css classes.
   *
   * @throws 'Unable to load type[...]' exception if the entity cannot be loaded
   * @throws 'Unable to render entity[...] if an appropriate rendering function
   * cannot be found.
   * @throws DOM_NO_MODIFICATION_ALLOWED_ERR via DOMNode::removeChild
   * @throws DOM_NOT_FOUND via DOMNode::removeChild
   * @see http://us2.php.net/manual/en/domnode.removechild.php
   * @todo  Better generalize the render chain, and path redirection.
   */
  public function load() {
    // @todo can we make this more generic (eg. explode('_', $entity_type))?
    // then pass to drupal_alter()
    switch ($this->epub_export_type) {
      case 'node':
        // We remove node aliases to prevent duplicates
        $this->alias = 'node/' . $this->id;
        $parts = explode('/', $this->alias);
        $this->pathRedirection = str_repeat('../', count($parts));
        break;
      case 'taxonomy_term':
        $this->alias = drupal_get_path_alias('taxonomy/term/' . $this->id);
        $parts = explode('/', $this->alias);
        $this->pathRedirection = str_repeat('../', count($parts));
        break;
      default:
        // Do nothing for now.
    }

    // Load the requested entity.
    $centity = reset(entity_load($this->epub_export_type, array($this->id), array(), TRUE));
    $entity = clone($centity);
    $this->entity = $entity;
    if (empty($this->entity)) {
      throw new Exception('Unable to load type: ' . $this->typeinfo(), EPUB_EXPORT_NOLOAD);
    }

    $uri = entity_uri($this->epub_export_type, $this->entity);
    if (!empty($uri['path'])) {
      // Add the originating entity to the asset list.
      $this->assets[$uri['path']] = array('nav' => TRUE);
    }

    // Attempt to render the entity using the appropriate render function.
    $data = entity_view($this->epub_export_type, array($this->entity), 'epub_export');

    $variables = array(
      'head_title' => check_plain(entity_label($this->epub_export_type, $this->entity)),
      'content' => drupal_render($data),
      'entity_type' => $this->epub_export_type,
      'entity' => $this->entity,
    );
    $variables['language'] = $GLOBALS['language'];
    $variables['language']->dir = $GLOBALS['language']->direction ? 'rtl' : 'ltr';

    $this->rendered = theme('epub_export', $variables);

    $this->dom = new DOMDocument('1.0', 'utf-8');
    $this->dom->preserveWhiteSpace = FALSE;
    $this->dom->validateOnParse = TRUE;
    $this->dom->strictErrorChecking = FALSE;

    // We use adjust libxml error reporting to work around limitations with
    // DOMDocument's limitations in handling HTML5
    libxml_use_internal_errors(TRUE);
    $this->dom->loadHTML($this->rendered);
    libxml_clear_errors();

    $xpath = new DOMXpath($this->dom);

    // Remove any DOM elements and children based on class.
    foreach ($this->stripClasses() as $class) {
      // Remove DOM elements containing the list of classes.
      $query = "//*[contains(@class, '" . $class . "')]";
      $entries = $xpath->query($query);
      if (!empty($entries)) {
        foreach ($entries as $entry) {
          try {
            $entry->parentNode->removeChild($entry);
          }
          catch (Exception $e) {
            throw $e;
          }
        }
      }
    }

    $this->rendered = $this->dom->saveHTML();
  }

  /**
   * Provide a rendered text string.
   */
  protected function typeinfo() {
    return 'type: ' . $this->epub_export_type . ' id: ' . $this->id;
  }

  /**
   * Dig through the rendered content and pull out any referenced assets.
   *
   * This strictly discovers what assets are referenced. This doesn't do any
   * additional steps around determining what to do with the assets.
   */
  public function findAssets($level = 0) {
    if ($level === 0) {
      $nav = TRUE;
    }
    else {
      $nav = FALSE;
    }

    if (empty($this->assets)) {
      // Add the primary doc to the list of resources first.
      $uri = entity_uri($this->epub_export_type, $this->entity);
      if (!empty($uri['path'])) {
        // Add this to the asset list.
        if ($alias = drupal_get_path_alias($uri['path'])) {
          $this->assets[$alias] = array('nav' => $nav);
        }
        else {
          $this->assets[$uri['path']] = array('nav' => $nav);
        }
      }
    }

    $discovered = $this->discoverResourceLinks($this->dom);
    if (!empty($discovered)) {
      foreach ($discovered as $asset) {
        $this->assets[$asset] = array('nav' => $nav);
      }
    }

    // Give other modules a chance to modify our exported assets.
    drupal_alter('epub_export_find_assets', $this->assets, $this->context());
  }

  /**
   * Return the discovered asset list.
   *
   * @return array
   *   Associative array of assets.
   */
  public function getAssets() {
    return $this->assets;
  }

  /**
   * Return the to be exported assets.
   *
   * @return  array
   *   Associative array of exports.
   */
  public function getExports() {
    return $this->exports;
  }

  /**
   * Iterate over found assets and determine how and if they should be included.
   *
   * @todo abstract out the path detection routine into a standalone function
   */
  public function parseAssets() {


    if (!empty($this->assets)) {
      foreach ($this->assets as $asseturi => $asset) {
        $this->parseAsset($asseturi);
        $this->addAssetToExport($asseturi);
      }
    }

    // Give other modules an opportunity to alter all of our assets.
    drupal_alter('epub_export_exports', $this->exports, $this->context());
  }

  private function parseAsset($asseturi) {
    $path = $this->cleanURI($asseturi);
    $filepath = drupal_realpath(file_default_scheme() . '://' . $path);
    $this->assets[$asseturi]['dest'] = $path;

    // Assume it's a regular file and then if not fallback to drupal object
    // loading.

    if (file_exists($filepath)) {
      $this->handleUploadedFile($asseturi);
    }
    elseif (file_exists(DRUPAL_ROOT . '/' . $path)) {
     $this->handleStaticFile($asseturi, $filepath);
    }
    else {
      $this->handleDrupalAsset($asseturi);
    }

    // Give other modules a chance to modify our exported assets.
    drupal_alter('epub_export_export', $asset, $this->context());

  }

  /**
   * Handle the given asset as though it is a user uploaded file.
   */
  private function handleUploadedFile($asset) {
      $path = $this->cleanURI($asset);
      $filepath = drupal_realpath(file_default_scheme() . '://' . $path);

      // File is in the filesdir.
      $this->assets[$asset]['file'] = $filepath;
      $this->assets[$asset]['dest'] = $path;
      $this->assets[$asset]['mime'] = $this->getMimeType($filepath);
      $this->assets[$asset]['title'] = basename($filepath);

      // Disover if this is a managed file and if so add to ref_entity_type
      // ref_entity_id
      $query = db_select('file_managed', 'fm')
        ->fields('fm')
        ->condition('uri', file_default_scheme() . '://' . $path);

      foreach ($query->execute() as $row) {
        $this->assets[$asset]['ref_entity_type'] = 'file';
        $this->assets[$asset]['ref_entity_id'] = $row->fid;
      }
  }

  /**
   * Handle the given asset as though it is a static file in the Drupal site
   * codebase.
   */
  private function handleStaticFile($asset) {
    $path = $this->cleanURI($asset);
    // File is an absolute link to some file on this install.
    $this->assets[$asset]['file'] = DRUPAL_ROOT . '/' . $path;
    $this->assets[$asset]['mime'] = $this->getMimeType(DRUPAL_ROOT . '/' . $path);
    $this->assets[$asset]['title'] = basename($this->assets[$asset]['file']);
  }

  /**
   * Handle the given asset as though it is a drupal entity via menu_get_item().
   */
  private function handleDrupalAsset($asset) {
    $path = $this->cleanURI($asset);
    if ($item = menu_get_item($path)) {
      $this->assets[$asset]['title'] = $item['title'];
      // @todo determine more robust method for determining the entity
      if (!empty($item['page_arguments'][0])) {
        $entity = $item['page_arguments'][0];
        if (!in_array($item['path'], $this->getPrunedPaths()) && !empty($entity)) {
          if (!empty($entity->epub_export_type) && in_array($entity->epub_export_type, epub_export_supported_types())) {
            list($id, $vid, $bundle) = entity_extract_ids($entity->epub_export_type, $entity);
            if (!empty($id)) {
              $this->assets[$asset]['entity_type'] = $entity->epub_export_type;
              $this->assets[$asset]['dest'] = $path . '/index.html';
              $this->assets[$asset]['mime'] = 'text/html';
              $this->assets[$asset]['ref_entity_type'] = $entity->epub_export_type;
              $this->assets[$asset]['ref_entity_id'] = $id;
              $this->assets[$asset][$entity->epub_export_type][$id] = $entity;
            }
          }
        }
      }
    }
  }

  /**
   * Optionally add the given asset to our exports list if it has a properly set
   * mimetype.
   */
  private function addAssetToExport($asset) {
    // Store the asset in our assets list, if we have a mimetype. otherwise
    // omit it from the export since it was not properly handled. This
    // likely indicates a content issue or a bug in the handler.
    if (!empty($this->assets[$asset]['mime'])) {
      // Save this item to our exported assets
      $dest = $this->assets[$asset]['dest'];
      if (!isset($this->exports[$dest])) {
        $this->exports[$dest] = $this->assets[$asset];
      }
      // Save this mapping from the item to the exported, needed for URL rewriting.
      $this->assets[$asset]['target'] = $this->assets[$asset]['dest'];
    }
    else {
      //If code reaches this point %uri has no proper data handling path.  Perhaps bad markup.  In testing these tend to be links with value(s):
      // * void(0)
      // * relative paths that don't exist on the given server:
      // * URLS that simply do not resolve
      // * mailto: links
      unset($this->assets[$asset]);
    }
  }

  /**
   *
   */
  public function cleanURI($uri) {
    global $base_url;
    $context = $this->context();
    $context['org_uri'] = $uri;

    if ($this->isUrlEncoded($uri)) {
      $uri = urldecode($uri);
    }

    // @todo documentation for drupal_parse_url() indicates to not use it on
    // non-system generated urls, but this seems okay in this aspect.
    $options = drupal_parse_url($uri);
    if (!empty($options['path'])) {
      $path = $options['path'];
    }
    else {
      $path = $uri;
    }

    $path = trim($path);

    // Replace base url if it is present.
    if (mb_strpos($path, $base_url) === 0) {
      $path = substr_replace($path, '', 0, strlen($base_url));
    }

    // Strip a leading / if available.
    if (mb_strpos($path, '/') === 0) {
      $path = substr_replace($path, '', 0, 1);
    }
    $path = drupal_get_normal_path($path);

    // Strip files dir if applicable.
    if (mb_strpos($path, variable_get('file_public_path', 'sites/default/files')) === 0) {
      $path = substr_replace($path, '', 0, strlen(variable_get('file_public_path', 'sites/default/files')));

      // Remove the leading / if present.
      if (mb_strpos($path, '/') === 0) {
        $path = substr_replace($path, '', 0, 1);
      }
    }
    drupal_alter('epub_export_clean_uri', $path, $this->context());
    return $path;
  }

  /**
   * Helper function to determine if a string is urlencoded.
   *
   * There is no reliable way to determine if a string is already urlencoded.
   *
   * Currently implementation works on the naive assumption that if a '%' is
   * present then the URL is already encoded. This functionality is isolated
   * as it will likely need adjustments.
   *
   * @param string $string
   * @return bool
   *  TRUE indicating string is urlencoded FALSE otherwise.
   */
  function isUrlEncoded($string) {
    if (mb_strpos($string, '%') !== FALSE) {
      return TRUE;
    }
    return FALSE;
  }


  /**
   * Returns a map of tags to respective attributes can link external content.
   *
   * @todo make this hookable and configurable
   * @return array
   *   Associative array keyed by tagname with the attribute key that may
   *   reference assets external to the current document or markup.
   */
  public function tagMap() {
    $map = array(
      'a' => 'href',
      'area' => 'href',
      'audio' => 'src',
      'embed' => 'src',
      'iframe' => 'src',
      'img' => 'src',
      'link' => 'href',
      'object' => 'data',
      'script' => 'src',
      'source' => 'src',
      'track' => 'src',
      'video' => 'src',
    );

    // Give other modules an opportunity to alter the tag map.
    drupal_alter('epub_export_tag_map', $map, $this->context());
    return $map;
  }

  /**
   * Returns the mimetype for a given file.
   */
  public function getMimeType($file) {
    if (file_exists($file)) {
      $mime = mime_content_type($file);

      if ($mime == 'text/plain') {
        $info = pathinfo($file);
        switch ($info['extension']) {
          case 'css':
            $mime = 'text/css';
            break;

          case 'js':
            $mime = 'text/javascript';
            break;

        }
      }
      return $mime;
    }
    return FALSE;
  }

  /**
   * Return a css classes corresponding to markup that should be removed.
   *
   * This is useful for targetting details that are no longer pertinent when the
   * content will/may be used outside of a context of a live site where content
   * can be edited etc. Things like edit links and login/logout links are a good
   * representation of things that should be stripped.
   *
   * @todo determine if replacing with custom theme functions could do away with
   * this.
   * @return array
   *   css classes representing markup that should not be present in the final
   *   exported content.
   */
  public function stripClasses() {
    $classes = array(
      'contextual-links-wrapper',
      'meta submitted',
      'field-type-epub-export-epub',
      'submitted',
      // "Log in or register to post comments"
      'comment_forbidden',
      'comment-wrapper'
    );

    // Allow other modules to alter the css classes that are removed.
    drupal_alter('epub_export_strip_classes', $classes, $this->context());
    return $classes;
  }

  /**
   * Default paths to skip when considering assets.
   *
   * @return array
   *   Paths as returned from menu_get_item() to skip.
   */
  public function prune_paths() {
    $paths = array(
      'node/%/delete',
      'node/%/edit',
      'user/%',
      'user/login',
      'filter/tips',
    );

    // Give other modules an opportunity to alter the paths we consider assets.
    drupal_alter('epub_export_prune_paths', $paths, $this->context());
    return $paths;
  }


  /**
   * Determines if a URI should be included in epub export and rewriting.
   *
   * @param string $uri
   *   The uri to consider for inclusion. May be a partial or complete URL.
   *
   * @return bool
   *   TRUE if the uri should be included and/or rewritten FALSE otherwise.
   */
  public function includeURI($uri) {
    global $base_url;

    $context = $this->context();
    $context['org_uri'] = $uri;

    $include = TRUE;
    if (mb_strpos($uri, 'http://') === 0 && mb_strpos($uri, $base_url) === FALSE) {
      // This is an external link
      $include = FALSE;
    }

    if (mb_strpos($uri, '#') === 0) {
      // This is a page internal anchor link
      $include = FALSE;
    }

    drupal_alter('epub_export_include_uri', $include, $context);
    return $include;
  }

  /**
   * Rewrite referenced entities.
   *
   * After calling this function internal links within supplied $dom content
   * will be rewritten to the corresponding rewrite destination. Since the
   * assets were discovered via the content we should cover all of the available
   * linksregardless of the path used.
   */
  public function rewriteResourceLinks($dom = NULL) {
    if ($dom === NULL) {
      $dom = $this->dom;
    }
    if (!empty($this->assets)) {
      $xpath = new DOMXpath($dom);
      foreach ($this->assets as $uri => $asset) {
        foreach ($this->tagMap() as $tag => $attribute) {
          $query = "//" . $tag . "[@" . $attribute . "='" . $uri . "']";
          $entries = $xpath->query($query);
          if (!empty($entries)) {
            foreach ($entries as $entry) {
              if (!empty($asset['target'])) {
                // Only rewrite links we have bundled in our epub.
                $entry->setAttribute($attribute, $this->pathRedirection . $asset['target']);
              }
            }
          }
        }
      }
      return $dom->saveHTML();
    }
    return '';
  }

  public function discoverResourceLinks($dom) {
    $discovered = array();
    foreach ($this->tagMap() as $tag => $attribute) {
      $elements = $dom->getElementsByTagName($tag);
      foreach ($elements as $element) {
        if ($dom_node = $element->attributes->getNamedItem($attribute)) {
          $asset = $dom_node->nodeValue;
          if (!$this->includeURI($asset)) {
            continue;
          }

          if (empty($this->assets[$asset])) {
            $discovered[] = $asset;
          }
        }
      }
    }
    return $discovered;
  }

  /**
   * Provide default fallback document.
   *
   * This document will be used as the EPUB default fallback document if one is
   * required.
   */
  protected function getDefaultFallbackDocument() {
    $content = t('This referenced resource type is not supported by your device.');

    $variables = array(
      'head_title' => t('Fallback Document'),
      'content' => $content,
      'entity_type' => $this->epub_export_type,
      'entity' => $this->entity,
    );
    $variables['language'] = $GLOBALS['language'];
    $variables['language']->dir = $GLOBALS['language']->direction ? 'rtl' : 'ltr';

    $doc = theme('epub_export', $variables);

    // rewrite any links (primarily css files)
    foreach ($this->assets as $uri => $asset) {
      $doc = str_replace($uri, $asset['target'], $doc);
    }

    return $doc;
  }

  /**
   * Provides a list of assets to skip when deciding what assets to include.
   *
   * @see EPUBExport::prune_paths()
   */
  protected function getPrunedPaths() {
    return variable_get('epub_export_prune_paths', $this->prune_paths());
  }

  public function sanitizeId( $string ) {
    $string = str_replace(array('/', '.'), '-', $string);
    if (is_numeric(substr($string, 0, 1))) {
      $string = 'x-' . $string;
    }
    return $string;
  }

  private function addItem($uri, $asset, &$epub,  &$gathered){
    // If this is not a core media type add a fallback.
    if (in_array($asset['mime'], $epub->coreMediaTypes())) {
      $fallback = NULL;
    }
    else {
      // Add a default fallback document
      if (empty($gathered['fallback.html'])) {
        try {
          $epub->manifestAddItem('def-fallback', 'fallback.html', 'application/xhtml+xml', 'inline', $this->getDefaultFallbackDocument());
        }
        catch (Exception $e) {
          watchdog('epub_export', 'Caught exception adding default fallback document: %message', array('%message' => $e->getMessage()), WATCHDOG_NOTICE);
        }
        $gathered['fallback.html'] = TRUE;
      }
      $fallback = 'def-fallback';
    }

    if (isset($asset['file']) && (is_file($asset['file']) || !empty($asset['file_contents']))) {
      $id = $this->sanitizeID($asset['title']);
      // This is a static file directly add it.
      try {
        if (!empty($asset['file_contents'])) {
          $epub->manifestAddItem($id, $asset['dest'], $asset['mime'], 'inline', $asset['file_contents'], $fallback);
        }
        else {
          $epub->manifestAddItem($id, $asset['dest'], $asset['mime'], 'file', $asset['file'], $fallback);
        }
      }
      catch (Exception $e) {
        watchdog('epub_export', 'Caught exception adding static file: %message', array('%message' => $e->getMessage()), WATCHDOG_NOTICE);
      }
    }
    elseif (isset($asset['entity_type'])) {
      // This is a drupal object rewrite ResourceLinks.
      foreach ($asset[$asset['entity_type']] as $eid => $entity) {
        $htmlid = $this->sanitizeID($entity->epub_export_type . '-' . $eid);

        if ($htmlid != $this->htmlid) {
          $entityexport = new EPUBExport($asset['entity_type'], $eid);
          $entityexport->findAssets($level + 1);
          $entityexport->parseAssets();
          $this->rendered = $entityexport->rewriteResourceLinks($this->dom);
          $entityexport->bundleEPUB($epub, $level + 1, $gathered);
          try {
            $epub->manifestAddItem($htmlid, $asset['dest'], $asset['mime'], 'inline', $entityexport->rendered, $fallback);
          }
          catch (Exception $e) {
            watchdog('epub_export', 'Caught exception adding drupal child entity: %message', array('%message' => $e->getMessage()), WATCHDOG_NOTICE);
          }
        }
        else {
          try {
            $epub->manifestAddItem($htmlid, $asset['dest'], $asset['mime'], 'inline', $this->rendered, $fallback);
          }
          catch (Exception $e) {
            watchdog('epub_export', 'Caught exception adding default drupal entity: %message', array('%message' => $e->getMessage()), WATCHDOG_NOTICE);
          }
        }

        // If this is a navigation item add it to the spine and the nav doc.
        if ($asset['nav'] === TRUE && !empty($asset['ref_entity_type']) && in_array($asset['ref_entity_type'], epub_export_navigation_types())&& !empty($asset['ref_entity_id']) && !isset($epub->nav[$uri])) {
          $htmlid = $asset['ref_entity_type'] . '-' . $asset['ref_entity_id'];
          $epub->spineAddItemRef($htmlid);
          // Used to track the links for backwards compatible NCX document.
          $epub->nav[$uri] = '<li><a href="' . $asset['dest'] . '">' . check_plain($asset['title']) . '</a></li>';
        }
      }
    }
  }

  /**
   * Bundles a entity export into an epub package.
   *
   * This function should always be called without parameters.
   *
   * @param array $epub
   *   Internal recursion data (DO NOT USE)
   * @param int $level
   *   Internal recursion tracking (DO NOT USE)
   *
   * @return string
   *   Zipname if EPUB bundled properly or empty string.
   *
   * @todo adjust to use try/catch blocks where necessary
   */
  public function bundleEPUB(&$epub = NULL, $level = 0, &$gathered = array()) {
    libraries_load('FunnyMonkey-EPUB-Package');
    if (empty($epub)) {
      $epub = new FMEPub();
      $epub->nav = array();
    }

    foreach ($this->getExports() as $uri => $asset) {
      if (empty($gathered[$uri])) {
        $gathered[$uri] = TRUE;
        $this->addItem($uri, $asset, $epub, $gathered);
      }
    }

    // Bundle our EPUB.
    if ($level == 0) {
      $epub->validate();
      $epub->buildNCX();
      if (!$author = user_load($this->entity->uid)) {
        $author = drupal_anonymous_user();
      }

      // @todo dc:identifier should probably be UUID, URI, or field value
      $epub->metaSetDCMESElement('dc:identifier', variable_get('site_name', 'Drupal') . $this->id);
      $epub->metaSetDCMESElement('dc:title', check_plain(entity_label($this->epub_export_type, $this->entity)));
      $epub->metaAddDCMESElement('dc:creator', check_plain(format_username($author)));

      // Add our navigation 'nav' element.
      $variables = array(
        'head_title' => check_plain(entity_label($this->epub_export_type, $this->entity)),
        'links_array' => $epub->nav,
        'entity_type' => $this->epub_export_type,
        'entity' => $this->entity,
      );
      $variables['language'] = $GLOBALS['language'];
      $variables['language']->dir = $GLOBALS['language']->direction ? 'rtl' : 'ltr';

      $toc = theme('epub_export_toc', $variables);

      // Add additional documents to the export that may be in TOC only.
      $dom = new DOMDocument('1.0', 'utf-8');
      $dom->preserveWhiteSpace = FALSE;
      $dom->validateOnParse = TRUE;
      $dom->strictErrorChecking = FALSE;

      // We use adjust libxml error reporting to work around limitations with
      // DOMDocument's limitations in handling HTML5
      libxml_use_internal_errors(TRUE);
      $dom->loadHTML($toc);
      libxml_clear_errors();

      // See if there are any omitted resources
      $discovered = $this->discoverResourceLinks($dom);
      if (!empty($discovered)) {
        foreach ($discovered as $asseturi) {
          // omit non-css items
          if (strpos($asseturi, '.css') !== FALSE) {
            $this->parseAsset($asseturi);
            $this->addAssetToExport($asseturi);
            $dest = $this->assets[$asseturi]['dest'];
            if (empty($gathered[$dest])) {
              $gathered[$dest] = TRUE;
              $this->addItem($dest, $this->exports[$dest], $epub, $gathered);
            }
          }
        }
      }

      // rewrite css links. We don't use rewriteResourceLinks() because we
      // manually built the content of this doc and only the css is out of band
      // via Drupal's theme layer.
      foreach ($this->assets as $uri => $asset) {
        if (strpos($asset['target'], '.css') !== FALSE) {
          $toc = str_replace($uri, $asset['target'], $toc);
        }
      }

      try {
        $epub->manifestAddItem('nav', 'fmnav.html', 'application/xhtml+xml', 'inline', $toc, NULL, array('nav'));
      }
      catch (Exception $e) {
        watchdog('epub_export', 'Caught exception adding navigation document: %message', array('%message' => $e->getMessage()), WATCHDOG_NOTICE);
      }
      $zipname = drupal_tempnam(file_directory_temp(), 'epub-build-') . '.epub';
      try {
        $epub->bundle($zipname, variable_get('epub_export_method', 'PHP'));
        return $zipname;
      }
      catch (Exception $e) {
        throw($e);
      }
    }
    return '';
  }
}
