<?php

/**
 * @file
 * Provides a class to communicate to NetX.
 */

/**
 * Implements class NetXRepository.
 */
class NetXRepository {

  /**
   * Connection user.
   *
   * @var string
   */
  protected $user;

  /**
   * Connection password.
   *
   * @var string
   */
  protected $pass;

  /**
   * Server to connect to.
   *
   * @var string
   */
  protected $server;

  /**
   * Server cookie to re-use.
   *
   * @var string
   */
  protected $cookie;

  /**
   * Response object that used in last repository I/O.
   *
   * @var object
   */
  protected $response;

  /**
   * Server category to upload files when no category provided.
   *
   * @var string
   */
  protected $category;

  /**
   * The default array to send with every JSON-RPC request.
   *
   * @var array
   */
  protected $defaultRequestOptions = array(
    // @todo Is static ID enough?
    'id' => 1,
    'dataContext' => 'json',
    'jsonrpc' => '2.0',
  );


  /**
   * Constructs NetXRepository.
   *
   * @param string|null $server
   *   Custom server to use with authorization.
   */
  public function __construct($server = NULL) {
    if (empty($server)) {
      $this->user = variable_get('netx_user', '');
      $this->pass = variable_get('netx_pass', '');
      $this->server = variable_get('netx_server', '');
      $this->cookie = variable_get('netx_cookie', '');
      $this->category = variable_get('netx_upload_category', 'Drupal');
    }
    else {
      $this->server = $server;
    }
  }

  /**
   * Returns cookie from last response object.
   *
   * @return string
   *   The cookie string, empty string when no cookie is set.
   */
  public function getResponseCookies() {
    if (isset($this->response->headers['set-cookie'])) {
      return $this->response->headers['set-cookie'];
    }
    return '';
  }

  /**
   * Authorize user in DAM.
   *
   * @throws \Exception
   */
  protected function ensureAuthorized() {
    if (empty($this->cookie)) {
      // Try to authorize via JSON-RPC.
      $user = $this->authenticateAndGetUserBean($this->user, $this->pass);
      if ($user) {
        if ($cookies = $this->getResponseCookies()) {
          // Store invalidated cookies.
          variable_set('netx_cookie', $cookies);
          // Reuse cookie in the repository.
          $this->cookie = $cookies;
        }
      }
    }
    else {
      // @todo Check valid cookie expiration.
    }
  }

  /**
   * Queries DAM.
   *
   * @param string $url
   *   A string containing a fully qualified URI.
   * @param array $options
   *   An options array for drupal_http_request().
   * @param bool $decode_data
   *   (Optional) Boolean to decode response.
   *
   * @return object
   *   The request returned by drupal_http_request() with data property decoded.
   *
   * @throws \Exception
   *   When no data or request failed.
   */
  protected function doRequest($url, $options, $decode_data = TRUE) {
    timer_start('netx_request');
    $response = drupal_http_request($url, $options);
    $time = timer_stop('netx_request');

    if (variable_get('netx_log_requests', FALSE)) {
      watchdog('netx', 'Response takes %time for %url response %response', array(
        '%time' => sprintf('%.3fs', $time['time'] / 1000),
        '%url' => $url,
        '%response' => var_export((array) $response, TRUE),
      ), WATCHDOG_DEBUG);
    }

    if (isset($response->error)) {
      throw new Exception("Error Processing Request. (Error: {$response->code}, {$response->error})");
    }
    // Contents of file could be empty but data should exists.
    elseif (!isset($response->data)) {
      throw new \Exception("Error Processing Request. (Error: No data returned)");
    }

    if ($decode_data) {
      // Check the proper data format of response.
      $response->data = drupal_json_decode($response->data);
    }

    // Store decoded response.
    $this->response = $response;

    return $response;
  }

  /**
   * Makes request to JSON_RPC API.
   *
   * Builds command, sends to server, processes errors of the request.
   *
   * @param string $method
   *   The JSON-RPC method to execute.
   * @param array $params
   *   The method params prepend to default params.
   * @param bool $send_cookie
   *   Set auth-cookie for request.
   *
   * @return object
   *   The request returned by drupal_http_request() with data property decoded.
   *
   * @throws \Exception
   *   When no data or request failed.
   */
  protected function doRequestJsonRPC($method, $params, $send_cookie = TRUE) {
    $data = drupal_json_encode(array(
        'method' => $method,
        'params' => $params,
      ) + $this->defaultRequestOptions);
    $options = array(
      'method' => 'POST',
      'data' => $data,
      'headers' => array('Content-Type' => 'application/json'),
    );
    if ($send_cookie) {
      $options['headers']['Cookie'] = $this->cookie;
    }
    $url = 'https://' . $this->server . '/x7/json/';
    $response = $this->doRequest($url, $options);

    if (!isset($response->data['error'])) {
      return $response;
    }

    $error = $response->data['error'];
    if ($error['code'] == 6 && $send_cookie) {
      // Authorization failed, try start new session.
      unset($this->cookie);
      $this->ensureAuthorized();
      if (!empty($this->cookie)) {
        // Repeat request with new cookie.
        $options['headers']['Cookie'] = $this->cookie;
        $response = $this->doRequest($url, $options);
        if (!isset($response->data['error'])) {
          return $response;
        }
        $error = $response->data['error'];
      }
    }
    throw new Exception("Error Processing Request. (JSON-API error: {$error['code']}, {$error['message']})");
  }

  /**
   * Authorize user in DAM.
   *
   * @param string $user
   *   The user name to access DAM.
   * @param string $pass
   *   The password to access DAM.
   *
   * @return array
   *   The array of UserBean object values.
   *
   * @throws \Exception
   *   When no data or request failed.
   */
  public function authenticateAndGetUserBean($user, $pass) {
    $response = $this->doRequestJsonRPC('authenticateAndGetUserBean', array($user, $pass), FALSE);
    if ($response->data['result']) {
      // Update credentials to reuse repo.
      $this->user = $user;
      $this->pass = $pass;
    }
    return $response->data['result'];
  }

  /**
   * Retrieves a whole metadata about asset by ID.
   *
   * @param int $id
   *   The asset ID to retrieve.
   *
   * @return array
   *   The array of asset attributes.
   *
   * @throws \Exception
   */
  public function getAssetBean($id) {
    $this->ensureAuthorized();
    $response = $this->doRequestJsonRPC('getAssetBean', array($id));
    return $response->data['result'];
  }

  /**
   * Implements setAsset RPC method.
   *
   * @param array $asset
   *   Asset array from getAssetBean.
   *
   * @return mixed
   *   The array of setAsset method result.
   *
   * @throws \Exception
   */
  public function setAsset($asset) {
    $this->ensureAuthorized();
    // Set commitAttributes to update custom attributes.
    $asset['commitAttributes'] = TRUE;
    $response = $this->doRequestJsonRPC('setAsset', array($asset));
    return $response->data['result'];
  }

  /**
   * Retrieves a simple metadata about passed IDs.
   *
   * @param array $ids
   *   Array of asset IDs.
   *
   * @return array
   *   Array of simple metadata for each asset.
   *
   * @throws \Exception
   */
  public function getAssetListBeans($ids) {
    $this->ensureAuthorized();
    $response = $this->doRequestJsonRPC('getAssetObjects', array($ids));
    return $response->data['result'];
  }

  /**
   * Executes keyword search via JSON-RPC.
   *
   * @todo Allow pass an array of file extensions.
   *
   * @param string $keywords
   *   A string keyword to search for.
   * @param int $page
   *   Page number of search results starting from 0.
   * @param string $sort
   *   Name of field to sort results by.
   * @param int $sort_type
   *   Type of search. Either 0 (acending) or 1 (descending).
   *
   * @return array
   *   Array of AssetBean arrays.
   *
   * @throws \Exception
   */
  public function doSearchAssets($keywords, $page, $sort = '', $sort_type = 0) {
    $this->ensureAuthorized();
    $search = array();
    // @todo Pass as argument somehow.
    $search[] = '+fileType:(*)';
    if ($keywords) {
      $search[] = '+keywords:' . $keywords . '*';
    }
    $query = implode(' ', $search);
    $response = $this->doRequestJsonRPC('searchAssetBeanObjects', array(
        // The name of the field to sort the search results by.
        $sort,
        // Sort order for search results (0 - ascending, 1 - descending order).
        $sort_type,
        // Integer of the match type (0 - "AND", 1 - "OR", 2 - "NOT")
        0,
        // Type: 8 - Solr/Lucene search.
        array(8),
        array(0),
        array(0),
        // The search query.
        array($query),
        array(''),
        array(''),
        // Name of saved search, or NULL to not save it.
        NULL,
        // Whether you want to be notified immediately, or daily, etc.
        0,
        // If TRUE, record search results in stats.
        TRUE,
        // Starting index of results to send (to support paging of results).
        $page * 10,
        // Number of items to send.
        10,
      ));
    return $response->data['result'];
  }

  /**
   * Executes keyword search count via JSON-RPC.
   *
   * @todo Allow pass an array of file extensions.
   *
   * @param string $keywords
   *   A string keyword to search for.
   *
   * @return array
   *   Array of asset IDs.
   *
   * @throws \Exception
   */
  public function doSearchAssetListLength($keywords) {
    $this->ensureAuthorized();
    $search = array();
    // @todo Pass as argument somehow.
    $search[] = '+fileType:(*)';
    if ($keywords) {
      $search[] = '+keywords:' . $keywords . '*';
    }
    $query = implode(' ', $search);
    $response = $this->doRequestJsonRPC('searchAssetListLength', array(
        // The name of the field to sort the search results by.
        '',
        // Sort order for search results (0 - ascending, 1 - descending order).
        0,
        // Integer of the match type (0 - "AND", 1 - "OR", 2 - "NOT")
        0,
        // Type: 8 - Solr/Lucene search.
        array(8),
        array(0),
        array(0),
        // The search query.
        array($query),
        array(''),
        array(''),
        // Name of saved search, or NULL to not save it.
        NULL,
        // Whether you want to be notified immediately, or daily, etc.
        0,
        // If TRUE, record search results in stats.
        TRUE,
      ));
    return $response->data['result'];
  }

  /**
   * Gets asset metadata templates.
   *
   * @return array[]
   *   An array of AttributeBean arrays.
   *
   * @see netx_get_attribues()
   *
   * @throws \Exception
   */
  public function getAttributeTemplates() {
    $this->ensureAuthorized();
    $response = $this->doRequestJsonRPC('getAttributeTemplates', array());
    return $response->data['result'];
  }

  /**
   * Gets array of all pulldown objects in the DAM.
   *
   * @return array[]
   *   An array of PulldownBean arrays.
   *
   * @see netx_get_attribues()
   *
   * @throws \Exception
   */
  public function getPulldowns() {
    $this->ensureAuthorized();
    $response = $this->doRequestJsonRPC('getPulldowns', array());
    return $response->data['result'];
  }

  /**
   * Updates custom attributes for asset via JSON-RPC.
   *
   * @param int $asset_id
   *   The asset ID.
   * @param array $names
   *   An array of attribute names to update.
   * @param array $values
   *   The values to set for attributes.
   *
   * @return NULL
   *   No data returned.
   *
   * @throws \Exception
   *   When no connection to repository.
   */
  public function updateAttributes($asset_id, array $names, array $values) {
    $this->ensureAuthorized();
    $response = $this->doRequestJsonRPC('updateAttributes', array(
      $asset_id,
      $names,
      $values,
    ));
    return $response->data['result'];
  }

  /**
   * Download file content.
   *
   * @param string $path
   *   The remote path to file.
   *
   * @throws \Exception
   *   When no file received.
   *
   * @return mixed
   *   Binary stream.
   */
  public function getFileContent($path) {
    $url = 'https://' . $this->user . ':' . $this->pass . '@' . $this->server . $path;
    $response = $this->doRequest($url, array(), FALSE);
    return $response->data;
  }

  /**
   * Download file content (image) via REST API.
   *
   * @param int $asset_id
   *   The asset ID.
   * @param string $type
   *   The type ('thumb', 'preview', 'original').
   *
   * @throws \Exception
   *   When no file received.
   *
   * @return mixed
   *   Binary stream.
   */
  public function getImageFileContent($asset_id, $type = 'original') {
    // Use anonymous "viewer" setting.
    $url = 'https://' . $this->server . "/file/asset/$asset_id/$type";
    $request = $this->doRequest($url, array(), FALSE);

    if (empty($request->data)) {
      // Image should contain a data.
      throw new \Exception("Error Processing Request. (Error: No data returned)");
    }
    return $request->data;
  }

  /**
   * Returns web-accessible URL for given asset.
   *
   * @param int $asset_id
   *   The asset ID.
   * @param string $file_name
   *   The asset file name.
   * @param string $mime
   *   The stored mime-type.
   *
   * @return string
   *   The REST URL to access.
   */
  public function getRemoteOriginal($asset_id, $file_name, $mime) {
    $path = 'https://' . $this->server . '/file/asset/' . $asset_id;
    switch ($mime) {
      case 'video/netx':
        return $path . '/view/previewH264/' . $file_name;

      case 'audio/netx':
        return $path . '/view/previewMp3/' . $file_name;
    }
    return $path . '/original/' . $file_name;
  }

  /**
   * Uploads a file to DAM.
   *
   * @param \stdClass $file
   *   The file object to upload.
   *
   * @return string
   *   The server response content.
   *
   * @throws \Exception
   *   When upload failed.
   */
  public function uploadFile($file) {
    return $this->uploadFileData(file_get_contents($file->uri), $file->filename, $file->filemime);
  }

  /**
   * Upload file to NetX.
   *
   * @param string $file_data
   *   Content of the file.
   * @param string $file_name
   *   The name of the uploaded file.
   * @param string $mimetype
   *   The mime-type of the uploaded file.
   *
   * @return string
   *   The server response content.
   *
   * @throws \Exception
   *   When upload failed.
   */
  public function uploadFileData($file_data, $file_name, $mimetype) {
    $this->ensureAuthorized();
    $boundary = '--------------------------' . str_replace('.', '', microtime(TRUE));
    $url = 'https://' . $this->server . "/servlet/FileUploader";

    $data = "--$boundary\r\n";

    $data .= 'Content-Disposition: form-data; name="upfile"; filename="' . $file_name . "\"\r\n";
    $data .= "Content-Transfer-Encoding: binary\r\n";
    $data .= "Content-Type: $mimetype\r\n\r\n";
    $data .= $file_data . "\r\n";
    $data .= "--$boundary--";

    $options = array(
      'headers' => array(
        'Content-Type' => "multipart/form-data; boundary=$boundary",
        'Cookie' => $this->cookie,
      ),
      'method' => 'POST',
      'data' => $data,
      // @todo Configure timeout.
      'timeout' => 180,
    );
    $response = $this->doRequest($url, $options, FALSE);
    if (empty($response->data)) {
      throw new \Exception("Error Processing Request. (Error: No data returned)");
    }
    return $response->data;
  }

  /**
   * Imports uploaded file via JSON-RPC.
   *
   * @param string $file_name
   *   The file name to import.
   * @param int $category_id
   *   The category ID to import the file.
   *
   * @return array
   *   The asset object.
   *
   * @throws \Exception When import failed.
   */
  public function importAsset($file_name, $category_id) {
    $uploads = &drupal_static('netx_file_presave');
    $attributes_update = array();
    if (isset($uploads['netx://'])) {
      // There's initial values for attributes.
      $attributes_update = $uploads['netx://'];
    }
    // Prepare attributes array to send.
    $attributes = array();
    foreach ($attributes_update as $label => $value) {
      $attributes[] = array(
        'label' => $label,
        'value' => $value,
      );
    }

    $this->ensureAuthorized();
    $response = $this->doRequestJsonRPC('importAsset', array(
      $file_name,
      // Category ID.
      $category_id,
      // Storage ID.
      1,
      $attributes,
    ));

    if ($response->data['result']) {
      // Asset imported, no need in this data.
      unset($uploads['netx://']);
    }
    return $response->data['result'];
  }

  /**
   * Returns a status of asset import.
   *
   * @param int $asset_id
   *   The asset ID.
   *
   * @return int
   *   The asset import status:
   *   - Error (-1)
   *   - Import started (0)
   *   - Import complete (1)
   *   - Preview complete (2)
   *
   * @throws \Exception
   *   When I/O problem.
   */
  public function getImportStatus($asset_id) {
    $this->ensureAuthorized();
    $response = $this->doRequestJsonRPC('getImportStatus', array($asset_id));
    return $response->data['result'];
  }

  /**
   * Creates a category in DAM.
   *
   * @param string $name
   *   The name of new category.
   * @param int $parent
   *   The ID of the parent category, default to root.
   * @param bool $visible
   *   Is the category visible.
   *
   * @return int
   *   The ID othe new category
   *
   * @throws \Exception
   *   When no connection to repository.
   */
  public function createCategory($name, $parent = 1, $visible = TRUE) {
    $this->ensureAuthorized();
    $response = $this->doRequestJsonRPC('createCategory', array(
      $name,
      $parent,
      $visible
    ));
    return $response->data['result'];
  }

  /**
   * Retrieves a tree of categories.
   *
   * @param int $parent
   *   The parent category ID, default to root.
   *
   * @return array
   *   An array of categories.
   *
   * @throws \Exception
   *   When no connection to repository.
   */
  public function getCategoryTree($parent = 1) {
    $this->ensureAuthorized();
    $response = $this->doRequestJsonRPC('getCategoryTree', array(
      $parent
    ));
    return $response->data['result'];
  }

  /**
   * Returns default category to upload files.
   *
   * @return null|string
   *   The name of category.
   *
   * @see _netx_find_category()
   */
  public function getDefaultCategory() {
    return $this->category;
  }

  /**
   * Look up category id by category path.
   *
   * @param string $path
   *   The path.
   *
   * @return int
   *   ID of category, or 0 if the category is not found.
   */
  public function getCategoryByPath($path) {
    $this->ensureAuthorized();
    $response = $this->doRequestJsonRPC('getCategoryByPath', array(
      array($path),
    ));
    return $response->data['result'];
  }

  /**
   * Retrieves a list of assets for category.
   *
   * @param int $cid
   *   Category ID.
   *
   * @return array
   *   An array of asets.
   * @throws \Exception
   *   When no connection to repository.
   */
  public function getAssetsForCategory($cid) {
    $this->ensureAuthorized();
    $response = $this->doRequestJsonRPC('getAssetsForCategory', array(
      $cid, 'name', 0
    ));
    return $this->getAssetListBeans($response->data['result']);
  }

}
