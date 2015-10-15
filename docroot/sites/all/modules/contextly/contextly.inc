<?php

/**
 * Drupal-specific kit implementation.
 *
 * @method ContextlyDrupalApiSessionShared newApiSession()
 * @method ContextlyDrupalApiTransport newApiTransport()
 * @method ContextlyDrupalAssetsList newAssetsList()
 *
 * @method ContextlyDrupalApiSessionShared newDrupalApiSessionShared()
 * @method ContextlyDrupalApiTransport newDrupalApiTransport()
 * @method ContextlyDrupalAssetsLibraryRenderer newDrupalAssetsLibraryRenderer()
 * @method ContextlyDrupalNodeEditor newDrupalNodeEditor()
 * @method ContextlyDrupalNodeData newDrupalNodeData()
 * @method ContextlyDrupalException newDrupalException()
 */
class ContextlyDrupalKit extends ContextlyKit {

  /**
   * @return ContextlyDrupalKit
   */
  public static function getInstance() {
    static $instance;

    if (!isset($instance)) {
      $config = self::getDefaultSettings();
      $instance = new self($config);
    }

    return $instance;
  }

  public static function getDefaultSettings() {
    $config = new ContextlyKitSettings();
    $config->cdn = (bool) variable_get('contextly_kit_cdn', TRUE);

    if (variable_get('contextly_mode', '') === 'dev') {
      $config->mode = 'dev';
    }

    $key = variable_get('contextly_api_key', array());
    foreach ($key as $property => $value) {
      $config->{$property} = $value;
    }

    return $config;
  }

  protected function getClassesMap() {
    $map = parent::getClassesMap();

    // Overrides.
    $map['ApiSession'] = 'ContextlyDrupalApiSessionShared';
    $map['ApiTransport'] = 'ContextlyDrupalApiTransport';
    $map['AssetsList'] = 'ContextlyDrupalAssetsList';

    // Drupal-specific classes.
    $map['DrupalApiSessionShared'] = 'ContextlyDrupalApiSessionShared';
    $map['DrupalApiTransport'] = 'ContextlyDrupalApiTransport';
    $map['DrupalAssetsLibraryRenderer'] = 'ContextlyDrupalAssetsLibraryRenderer';
    $map['DrupalNodeEditor'] = 'ContextlyDrupalNodeEditor';
    $map['DrupalNodeData'] = 'ContextlyDrupalNodeData';
    $map['DrupalException'] = 'ContextlyDrupalException';

    return $map;
  }

  function buildFileUrl($filepath) {
    return file_create_url(_contextly_kit_path() . '/' . $filepath);
  }

}

/**
 * Sends updated node to the Contextly service, builds JS settings.
 *
 * @property ContextlyDrupalKit $kit
 */
class ContextlyDrupalNodeEditor extends ContextlyKitBase {

  /**
   * @var ContextlyKitApi
   */
  protected $api;

  public function __construct($kit) {
    parent::__construct($kit);

    $this->api = $this->kit->newApi();
  }

  /**
   * Sends the node to Contextly.
   *
   * @param object $node
   */
  public function putNode($node) {
    $this->putNodeContent($node);
    if (module_exists('taxonomy')) {
      $this->putNodeTags($node);
    }
  }

  /**
   * Sends the node text content and meta-information to Contextly.
   *
   * @param object $node
   */
  protected function putNodeContent($node) {
    // Build absolute node URL.
    $uri = entity_uri('node', $node);
    if (!isset($uri['path'])) {
      throw $this->kit->newDrupalException(t('Unable to generate URL for the node #@nid.', array(
        '@nid' => $node->nid,
      )));
    }
    $uri['options']['absolute'] = TRUE;
    $node_url = url($uri['path'], $uri['options']);

    // Check if post has been saved to Contextly earlier.
    $contextly_post = $this->api
      ->method('posts', 'get')
      ->param('page_id', $node->nid)
      ->get();

    $user = user_load($node->uid);

    // TODO Take care about langcode.
    $content = node_view($node, 'contextly');
    $content = render($content);

    $post_data = array(
      'post_id' => $node->nid,
      'post_title' => $node->title,
      'post_date' => _contextly_format_date($node->created),
      'post_modified' => _contextly_format_date($node->changed),
      'post_status' => $node->status ? 'publish' : 'draft',
      'post_type' => $node->type,
      'post_content' => $content,
      'url' => $node_url,
      'author_id' => $node->uid,
      'post_author' => $user->name,
    );
    $this->api
      ->method('posts', 'put')
      ->extraParams($post_data);
    if (isset($contextly_post->entry)) {
      $this->api->param('id', $contextly_post->entry->id);
    }

    $this->api
      ->requireSuccess()
      ->get();
  }

  /**
   * Sends node tags to the Contextly.
   *
   * @param object $node
   */
  protected function putNodeTags($node) {
    // Remove existing tags first, if any.
    // TODO: Handle pagination of the request.
    $post_tags = $this->api
      ->method('poststags', 'list')
      ->searchParam('post_id', ContextlyKitApiRequest::SEARCH_TYPE_EQUAL, $node->nid)
      ->get();
    if (!empty($post_tags->list)) {
      foreach ($post_tags->list as $tag) {
        $this->api
          ->method('poststags', 'delete')
          ->param('id', $tag->id)
          ->requireSuccess();
      }
    }

    // Save new tags.
    // TODO WP Plugin sends only 3 first tags. Why?
    $tags = $this->kit
      ->newDrupalNodeData($node)
      ->getTags();
    foreach ($tags as $tag) {
      $this->api
        ->method('poststags', 'put')
        ->extraParams(array(
          'post_id' => $node->nid,
          'name' => $tag,
        ))
        ->requireSuccess();
    }

    // Make all requests at once.
    $this->api->get();
  }

}

class ContextlyDrupalApiTransport implements ContextlyKitApiTransportInterface {

  /**
   * Performs the HTTP request.
   *
   * @param string $method
   *   "GET" or "POST".
   * @param string $url
   *   Request URL.
   * @param array $query
   *   GET query parameters.
   * @param array $data
   *   POST data.
   * @param array $headers
   *   List of headers.
   *
   * @return ContextlyKitApiResponse
   */
  public function request($method, $url, $query = array(), $data = array(), $headers = array()) {
    // Add content type to the headers.
    $headers['Content-Type'] = 'application/x-www-form-urlencoded';

    // Add query to the URL.
    $url = url($url, array(
      'external' => TRUE,
      'query' => $query,
    ));

    $result = drupal_http_request($url, array(
      'method' => $method,
      'data' => http_build_query($data, NULL, '&'),
      'headers' => $headers,
    ));

    // Build response for the Kit.
    $response = new ContextlyKitApiResponse();
    $response->code = $result->code;
    $response->body = $result->data;
    if (isset($result->error)) {
      $response->error = $result->error;
    }

    return $response;
  }

}

class ContextlyDrupalApiSessionShared extends ContextlyKitBase implements ContextlyKitApiSessionInterface {

  const TOKEN_CACHE_ID = 'contextly:access-token';
  const TOKEN_CACHE_BIN = 'cache';

  /**
   * @var ContextlyKitApiTokenInterface
   */
  protected $token;

  public function __construct($kit) {
    parent::__construct($kit);

    $this->token = $this->loadSharedToken();
  }

  public function loadSharedToken() {
    $cache = cache_get(self::TOKEN_CACHE_ID, self::TOKEN_CACHE_BIN);

    if ($cache) {
      try {
        return $this->kit->newApiToken($cache->data);
      }
      catch (ContextlyKitException $e) {
        // Just suppress the exception on a broken saved value.
        watchdog_exception('contextly', $e);
      }
    }

    // Fallback to an empty token.
		return $this->kit->newApiTokenEmpty();
  }

  /**
   * @param ContextlyKitApiTokenInterface $token
   */
  public function saveSharedToken($token) {
    $expire = $token->getExpirationDate();
    cache_set(self::TOKEN_CACHE_ID, (string) $token, self::TOKEN_CACHE_BIN, $expire);
  }

  public function removeSharedToken() {
    cache_clear_all(self::TOKEN_CACHE_ID, self::TOKEN_CACHE_BIN);
  }

  public function cleanupToken() {
    $this->token = $this->kit->newApiTokenEmpty();
    $this->removeSharedToken();
  }

  public function setToken($token) {
    $this->token = $token;
    $this->saveSharedToken($token);
  }

  public function getToken() {
    return $this->token;
  }

}

class ContextlyDrupalAssetsList extends ContextlyKitAssetsList {

  function buildCssPaths() {
    $css = $this->getCss();
    if (empty($css)) {
      return array();
    }

    $paths = array();
    $basePath = _contextly_kit_path() . '/' . $this->kit->getFolderPath('client') . '/';
    foreach ($css as $path) {
      $paths[$path] = $basePath . $path . '.css';
    }

    return $paths;
  }

  function buildJsPaths() {
    $js = $this->getJs();
    if (empty($js)) {
      return array();
    }

    $paths = array();
    $basePath = _contextly_kit_path() . '/' . $this->kit->getFolderPath('client') . '/';
    foreach ($js as $path) {
      $paths[$path] = $basePath . $path . '.js';
    }

    return $paths;
  }

}

/**
 * Renders Contextly Kit assets in format suitable for hook_library() entry.
 *
 * @property ContextlyDrupalAssetsList $assets
 */
class ContextlyDrupalAssetsLibraryRenderer extends ContextlyKitAssetsRenderer {

  protected function renderAssets($assetsMethod, $external = FALSE) {
    $uris = array_values($this->assets->{$assetsMethod}());
    if (empty($uris)) {
      return array();
    }

    $options = array();
    if ($external) {
      $options += array(
        'type' => 'external',
      );
    }
    return array_combine($uris, array_fill(0, count($uris), $options));
  }

  public function renderCss() {
    if ($this->kit->isCdnEnabled()) {
      return $this->renderAssets('buildCssUrls', TRUE);
    }
    else {
      return $this->renderAssets('buildCssPaths');
    }
  }

  public function renderJs() {
    if ($this->kit->isCdnEnabled()) {
      return $this->renderAssets('buildJsUrls', TRUE);
    }
    else {
      return $this->renderAssets('buildJsPaths');
    }
  }

  public function renderTpl() {
    // We don't support templates rendering to the Drupal library yet.
  }

  public function renderAll() {
    return array(
      'css' => $this->renderCss(),
      'js' => $this->renderJs(),
    );
  }

}

/**
 * Helper to extract different data from the node.
 *
 * @property ContextlyDrupalKit $kit
 */
class ContextlyDrupalNodeData extends ContextlyKitBase {

  protected $node;

  public function __construct($kit, $node) {
    parent::__construct($kit);

    $this->node = $node;
  }

  public function getMetadata($language = NULL) {
    $metadata = array();

    // Basic data.
    $metadata['title'] = $this->node->title;
    $metadata['type'] = $this->node->type;
    $metadata['post_id'] = $this->node->nid;

    // Timestamps.
    $metadata['pub_date'] = _contextly_format_date($this->node->created);
    $metadata['mod_date'] = _contextly_format_date($this->node->changed);

    // Node URL.
    $metadata['url'] = $this->getUrl();

    // Author info.
    $author = $this->getAuthor();
    if ($author) {
      $metadata['author_id'] = $author->uid;
      $metadata['author_name'] = $author->name;
      $metadata['author_display_name'] = format_username($author);
    }
    else {
      $metadata['author_id'] = 0;
      $metadata['author_name'] = variable_get('anonymous', t('Anonymous'));
      $metadata['author_display_name'] = $metadata['author_name'];
    }

    // Tags and categories.
    // TODO Fill categories same way as tags, but from different fields.
    $metadata['tags'] = $this->getTags();
    $metadata['categories'] = array();

    // Featured image.
    $metadata['image'] = $this->getFeaturedImageUrl($language);

    return $metadata;
  }

  protected function getAuthor() {
    $author = NULL;
    if ($this->node->uid) {
      $author = user_load($this->node->uid);
    }
    return $author;
  }

  protected function getUrl() {
    $uri = entity_uri('node', $this->node);
    $uri['options']['absolute'] = TRUE;
    return url($uri['path'], $uri['options']);
  }

  protected function getFeaturedImageUrl($langcode) {
    $image_url = NULL;
    if (!module_exists('image')) {
      return $image_url;
    }

    $field = variable_get('contextly_featured_image__' . $this->node->type, '');
    if (empty($field)) {
      return $image_url;
    }

    $field_langcode = field_language('node', $this->node, $field, $langcode);
    if (empty($this->node->{$field}[$field_langcode][0]['uri'])) {
      return $image_url;
    }

    $uri = $this->node->{$field}[$field_langcode][0]['uri'];
    $image_url = file_create_url($uri);
    return $image_url;
  }

  /**
   * Extracts tags list attached to the node that should be sent to Contextly.
   *
   * @return array
   */
  public function getTags() {
    $tags = array();
    if (!module_exists('taxonomy')) {
      return $tags;
    }

    // Use either all available fields or selected fields only.
    if (variable_get('contextly_all_tags', TRUE)) {
      $fields = _contextly_get_node_type_fields($this->node->type, array('taxonomy_term_reference'));
    }
    else {
      $fields = array_values(variable_get('contextly_tags__' . $this->node->type, array()));
    }

    // Collect all term IDs first.
    $tids = array();
    foreach ($fields as $field_name) {
      if (!empty($this->node->{$field_name})) {
        // TODO: Handle multi-language fields properly.
        // For now just post all languages.
        foreach ($this->node->{$field_name} as $lang => $items) {
          foreach ($items as $item) {
            $tid = $item['tid'];
            $tids[$tid] = TRUE;
          }
        }
      }
    }

    if (!empty($tids)) {
      $terms = taxonomy_term_load_multiple(array_keys($tids));
      foreach ($terms as $term) {
        $tags[] = $term->name;
      }
    }

    return $tags;
  }

}

class ContextlyDrupalException extends ContextlyKitException {}
