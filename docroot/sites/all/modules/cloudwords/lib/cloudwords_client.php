<?php

/*
 * Copyright 2011, Cloudwords, Inc.
 *
 * Licensed under the API LICENSE AGREEMENT, Version 1.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.cloudwords.com/developers/license-1.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

require_once('cloudwords_api.php');

// check package dependencies
if( !function_exists('curl_init') ) {
  $params = array('error_message' => 'The Cloudwords API PHP SDK requires the CURL PHP extension.');
  throw new CloudwordsApiException(CloudwordsApiException::DEPENDENCY_EXCEPTION, $params);
}
if( !function_exists('json_decode') ) {
  $params = array('error_message' => 'The Cloudwords API PHP SDK requires the JSON PHP extension.');
  throw new CloudwordsApiException(CloudwordsApiException::DEPENDENCY_EXCEPTION, $params);
}

/**
 * Basic implementation of the Cloudwords API client.
 *
 * @author Douglas Kim <doug@cloudwords.com>
 * @since 1.0
 */
class CloudwordsClient implements CloudwordsAPI {

  /**
   * Constants
   */
  const AUTHORIZATION_HEADER = 'Authorization: ';
  const CONTENT_TYPE_HEADER = 'Content-Type: ';
  const CONTENT_TYPE_JSON = 'application/json';
  const CONTENT_TYPE_MULTIPART_FORM_DATA = 'multipart/form-data';
  const REQUEST_TYPE_GET = 'GET';
  const REQUEST_TYPE_POST = 'POST';
  const REQUEST_TYPE_PUT = 'PUT';

  /**
   * Member variables
   */

  // The domain + version for connecting to the API (e.g. https://api.cloudwords.com/v1)
  private $base_url_with_version;

  // The authorization token to validate identify when accessing the Cloudwords API
  private $auth_token;

  // The timeout in seconds until a connection is established. A timeout value of zero is
  // interpreted as an infinite timeout.
  private $connection_timeout = 30;

  // The socket timeout in seconds, which is the timeout for waiting for data or, put differently,
  // a maximum period inactivity between two consecutive data packets). A timeout value of zero is
  // interpreted as an infinite timeout.
  private $socket_timeout = 60;

  // The max concurrent connections the client can establish against the Cloudwords API
  private $max_total_connections = 3;

  /**
   * The CloudwordsProject class to instansiate.
   */
  protected $project_class = 'CloudwordsProject';

  /**
   * Convenience constructor that provides default configuration.
   *
   * @param string $base_api_url The base domain of the Cloudwords API
   * @param integer $version The version of the Cloudwords API to use
   * @param string $auth_token The authorization token to validate identify when accessing the Cloudwords API
   */
  public function __construct($base_api_url, $version, $auth_token) {
    $this->base_url_with_version = $base_api_url . '/' . $version;
    $this->auth_token = $auth_token;
  }

  /**
   * Private methods
   */
  private function init() {
    $conn = curl_init();
    curl_setopt($conn, CURLOPT_RETURNTRANSFER, true);
    curl_setopt($conn, CURLOPT_CONNECTTIMEOUT, $this->connection_timeout);
    curl_setopt($conn, CURLOPT_TIMEOUT, $this->socket_timeout);
    curl_setopt($conn, CURLOPT_MAXCONNECTS, $this->max_total_connections);
    curl_setopt($conn, CURLOPT_SAFE_UPLOAD, false);
    return $conn;
  }

  private function close($conn) {
    curl_close($conn);
  }

  private function get($url, $accept_content_type, $body_content_type) {
    $conn = $this->init();
    curl_setopt($conn, CURLOPT_URL, $url);
    curl_setopt($conn, CURLOPT_HTTPHEADER, $this->get_headers($body_content_type));
    $response = $data = $this->execute($conn, $url, self::REQUEST_TYPE_GET);
    $data = $this->get_request_data($response, $accept_content_type);
    $this->close($conn);
    return $data;
  }

  private function post($url, $params, $accept_content_type, $body_content_type) {
    $conn = $this->init();
    curl_setopt($conn, CURLOPT_URL, $url);
    curl_setopt($conn, CURLOPT_HTTPHEADER, $this->get_headers($body_content_type));
    curl_setopt($conn, CURLOPT_POST, count($params));
    curl_setopt($conn, CURLOPT_POSTFIELDS, $body_content_type == self::CONTENT_TYPE_JSON ? json_encode($params) : $params);
    $response = $this->execute($conn, $url, self::REQUEST_TYPE_POST);
    $data = $this->get_request_data($response, $accept_content_type);
    $this->close($conn);
    return $data;
  }

  private function put($url, $params, $accept_content_type, $body_content_type) {
    $conn = $this->init();
    curl_setopt($conn, CURLOPT_URL, $url);
    curl_setopt($conn, CURLOPT_HTTPHEADER, $this->get_headers($body_content_type));
    curl_setopt($conn, CURLOPT_CUSTOMREQUEST, self::REQUEST_TYPE_PUT);
    curl_setopt($conn, CURLOPT_POSTFIELDS, $body_content_type == self::CONTENT_TYPE_JSON ? json_encode($params) : $params);
    $response = $this->execute($conn, $url, self::REQUEST_TYPE_PUT);
    $data = $this->get_request_data($response, $accept_content_type);
    $this->close($conn);
    return $data;
  }

  private function get_headers($body_content_type) {
    $headers = array();
    $headers[] = self::AUTHORIZATION_HEADER . $this->auth_token;
    if( $body_content_type == self::CONTENT_TYPE_JSON ) {
      $headers[] = self::CONTENT_TYPE_HEADER . self::CONTENT_TYPE_JSON;
    } else if( $body_content_type == self::CONTENT_TYPE_MULTIPART_FORM_DATA ) {
      $headers[] = self::CONTENT_TYPE_HEADER . self::CONTENT_TYPE_MULTIPART_FORM_DATA;
    }
    return $headers;
  }

  private function get_request_data($data, $content_type) {
    if( $content_type == self::CONTENT_TYPE_JSON ) {
      return json_decode($data, true);
    } else if( $content_type == self::CONTENT_TYPE_MULTIPART_FORM_DATA ) {
      return $data;
    } else {
      $params = array('content_type' => $content_type);
      throw new CloudwordsApiException(CloudwordsApiException::UNSUPPORTED_CONTENT_TYPE_EXCEPTION, $params);
    }
  }

  private function execute($conn, $url, $request_type) {
    // Skip SSL certificate verification
    curl_setopt($conn, CURLOPT_SSL_VERIFYPEER, FALSE);
    $data = curl_exec($conn);
    if (curl_errno($conn)) {
      $params = array('error_message' => curl_error($conn));
      throw new CloudwordsApiException(CloudwordsApiException::REQUEST_EXCEPTION, $params);
    }
    $http_status_code = curl_getinfo($conn, CURLINFO_HTTP_CODE);
    if ($http_status_code === 200 || $http_status_code === 201) {
      return $data;
    } else {
      $params = array('http_status_code' => $http_status_code,
        'request_type' => $request_type,
        'request_url' => $url,
      );
      if ($http_status_code === 404) {
        $params['error_message'] = 'Not Found';
        $error_response = json_decode($data);
        if (isset($error_response->{'error'})) {
          $params['error_message'] = $error_response->{'error'};
        }
        throw new CloudwordsApiException(CloudwordsApiException::REQUEST_EXCEPTION, $params);
      } else {
        $error_response = json_decode($data);
        $params['error_message'] = $error_response->{'error'};
        throw new CloudwordsApiException(CloudwordsApiException::API_EXCEPTION, $params);
      }
    }
  }

  /**
   * Public methods
   */

  public function getBaseUrlWithVersion() {
    return $this->base_url_with_version;
  }

  public function getAuthToken() {
    return $this->auth_token;
  }

  public function getConnectionTimeout() {
    return $this->connection_timeout;
  }

  public function getSocketTimeout() {
    return $this->socket_timeout;
  }

  public function getMaxTotalConnections() {
    return $this->max_total_connections;
  }

  public function get_open_projects() {
    $open_projects = array();
    $projects_metadata = $this->get($this->base_url_with_version . '/project/open.json', self::CONTENT_TYPE_JSON, self::CONTENT_TYPE_JSON);
    foreach( $projects_metadata as $project_metadata ) {
      $open_projects[] = $this->project_factory($project_metadata);
    }
    return $open_projects;
  }

  public function get_closed_projects() {
    $closed_projects = array();
    $projects_metadata = $this->get($this->base_url_with_version . '/project/closed.json', self::CONTENT_TYPE_JSON, self::CONTENT_TYPE_JSON);
    foreach( $projects_metadata as $project_metadata ) {
      $closed_projects[] = $this->project_factory($project_metadata);
    }
    return $closed_projects;
  }

  public function set_project_class($class) {
    $this->project_class = $class;
  }

  protected function project_factory($data) {
    $project_class = $this->project_class;
    return new $project_class($data);
  }

  public function get_project($project_id) {
    $project_metadata = $this->get($this->base_url_with_version . '/project/' . $project_id . '.json', self::CONTENT_TYPE_JSON, self::CONTENT_TYPE_JSON);
    return $this->project_factory($project_metadata);
  }

  public function create_project($params) {
    $project_metadata = $this->post($this->base_url_with_version . '/project', $params, self::CONTENT_TYPE_JSON, self::CONTENT_TYPE_JSON);
    return $this->project_factory($project_metadata);
  }

  public function update_project($params) {
    $project_metadata = $this->put($this->base_url_with_version . '/project/' . $params['id'], $params, self::CONTENT_TYPE_JSON, self::CONTENT_TYPE_JSON);
    return $this->project_factory($project_metadata);
  }

  public function upload_project_source($project_id, $zip_file) {
    $params = array('file' => '@' . $zip_file);
    $source_metadata = $this->put($this->base_url_with_version . '/project/' . $project_id . '/file/source', $params, self::CONTENT_TYPE_JSON, self::CONTENT_TYPE_MULTIPART_FORM_DATA);
    return new CloudwordsFile($source_metadata);
  }

  public function get_project_source($project_id) {
    $source_metadata = $this->get($this->base_url_with_version . '/project/' . $project_id . '/file/source.json', self::CONTENT_TYPE_JSON, self::CONTENT_TYPE_JSON);
    return new CloudwordsFile($source_metadata);
  }

  public function download_source_file($project_id) {
    $source_metadata = $this->get_project_source($project_id);
    return $this->download_file_from_metadata($source_metadata);
  }

  public function upload_project_reference($project_id, $zip_file) {
    $params = array('file' => '@' . $zip_file);
    $reference_metadata = $this->post($this->base_url_with_version . '/project/' . $project_id . '/file/reference', $params, self::CONTENT_TYPE_JSON, self::CONTENT_TYPE_MULTIPART_FORM_DATA);
    return new CloudwordsFile($reference_metadata);
  }

  public function update_project_reference($project_id, $document_id, $zip_file) {
    $params = array('file' => '@' . $zip_file);
    $reference_metadata = $this->put($this->base_url_with_version . '/project/' . $project_id . '/file/reference/' . $document_id, $params, self::CONTENT_TYPE_JSON, self::CONTENT_TYPE_MULTIPART_FORM_DATA);
    return new CloudwordsFile($reference_metadata);
  }

  public function get_reviewer_instruction($project_id, $language_code) {
    return $this->get($this->base_url_with_version . '/project/' . $project_id . '/language/' . $language_code . '/reviewer-instructions.json', self::CONTENT_TYPE_JSON, self::CONTENT_TYPE_JSON);
  }

  public function create_reviewer_instruction($project_id, $language_code, $content) {
    return $this->post($this->base_url_with_version . '/project/' . $project_id . '/language/' . $language_code . '/reviewer-instructions', $content, self::CONTENT_TYPE_JSON, self::CONTENT_TYPE_JSON);
  }  

  /**
   * Gets the reference files for a given project.
   *
   * @param int $project_id
   *   A project id.
   *
   * @return CloudwordsFile[]
   *   A list of reference files.
   */
  public function get_project_references($project_id) {
    $project_references = array();
    $references_metadata = $this->get($this->base_url_with_version . '/project/' . $project_id . '/file/reference.json', self::CONTENT_TYPE_JSON, self::CONTENT_TYPE_JSON);
    foreach( $references_metadata as $reference_metadata ) {
      $project_references[] = new CloudwordsFile($reference_metadata);
    }
    return $project_references;
  }

  public function get_project_reference($project_id, $document_id) {
    $file_metadata = $this->get($this->base_url_with_version . '/project/' . $project_id . '/file/reference/' . $document_id . '.json', self::CONTENT_TYPE_JSON, self::CONTENT_TYPE_JSON);
    return new CloudwordsFile($file_metadata);
  }

  public function download_reference_file($project_id, $document_id) {
    $file_metadata = $this->get_project_reference($project_id, $document_id);
    return $this->download_file_from_metadata($file_metadata);
  }

  public function get_master_project_translated_file($project_id) {
    $file_metadata = $this->get($this->base_url_with_version . '/project/' . $project_id . '/file/translated.json', self::CONTENT_TYPE_JSON, self::CONTENT_TYPE_JSON);
    return new CloudwordsFile($file_metadata);
  }

  public function download_master_translated_file($project_id) {
    $file_metadata = $this->get_master_project_translated_file($project_id);
    return $this->download_file_from_metadata($file_metadata);
  }

  public function get_project_translated_files($project_id) {
    $project_translated_files = array();
    $files_metadata = $this->get($this->base_url_with_version . '/project/' . $project_id . '/file/translated/language.json', self::CONTENT_TYPE_JSON, self::CONTENT_TYPE_JSON);
    foreach( $files_metadata as $file_metadata ) {
      $project_translated_files[] = new CloudwordsFile($file_metadata);
    }
    return $project_translated_files;
  }

  public function get_project_translated_file($project_id, $language) {
    $file_metadata = $this->get($this->base_url_with_version . '/project/' . $project_id . '/file/translated/language/' . $language . '.json', self::CONTENT_TYPE_JSON, self::CONTENT_TYPE_JSON);
    return new CloudwordsFile($file_metadata);
  }

  public function approve_project_language($project_id, $language) {
  $params = array(
    'status' => array(
      'code' => 'approved',
    ),
  );
    $file_metadata = $this->put($this->base_url_with_version . '/project/' . $project_id . '/file/translated/language/' . $language, $params, self::CONTENT_TYPE_JSON, self::CONTENT_TYPE_JSON);
    return new CloudwordsFile($file_metadata);
  }

  public function download_translated_file($project_id, $language) {
    $file_metadata = $this->get_project_translated_file($project_id, $language);
    return $this->download_file_from_metadata($file_metadata);
  }

  public function download_file_from_metadata($metadata) {
    if( !is_null($metadata) && !is_null($metadata->getContentPath()) ) {
      return $this->get($metadata->getContentPath(), self::CONTENT_TYPE_MULTIPART_FORM_DATA, self::CONTENT_TYPE_JSON);
    }
    return NULL;
  }

  public function request_bids_for_project($project_id, $preferred_vendors, $do_let_cloudwords_choose) {
    $params = array('preferredVendors' => $preferred_vendors,
                    'doLetCloudwordsChoose' => $do_let_cloudwords_choose);
    $bid_request = $this->post($this->base_url_with_version . '/project/' . $project_id . '/bid-request', $params, self::CONTENT_TYPE_JSON, self::CONTENT_TYPE_JSON);
    return new CloudwordsBidRequest($bid_request);
  }

  public function get_current_bid_request_for_project($project_id) {
    $bid_request = $this->get($this->base_url_with_version . '/project/' . $project_id . '/bid-request/current.json', self::CONTENT_TYPE_JSON, self::CONTENT_TYPE_JSON);
    return new CloudwordsBidRequest($bid_request);
  }

  public function get_preferred_vendors() {
    $preferred_vendors = array();
    $vendors = $this->get($this->base_url_with_version . '/vendor/preferred.json', self::CONTENT_TYPE_JSON, self::CONTENT_TYPE_JSON);
    foreach( $vendors as $vendor ) {
      $preferred_vendors[] = new CloudwordsVendor($vendor);
    }
    return $preferred_vendors;
  }

  public function get_source_languages() {
    $source_languages = array();
    $languages = $this->get($this->base_url_with_version . '/org/settings/project/language/source.json', self::CONTENT_TYPE_JSON, self::CONTENT_TYPE_JSON);
    foreach( $languages as $language ) {
      $source_languages[] = new CloudwordsLanguage($language);
    }
    return $source_languages;
  }

  public function get_target_languages() {
    $target_languages = array();
    $languages = $this->get($this->base_url_with_version . '/org/settings/project/language/target.json', self::CONTENT_TYPE_JSON, self::CONTENT_TYPE_JSON);
    foreach( $languages as $language ) {
      $target_languages[] = new CloudwordsLanguage($language);
    }
    return $target_languages;
  }

  public function get_intended_uses() {
    $intended_uses = array();
    $uses = $this->get($this->base_url_with_version . '/org/settings/project/intended-use.json', self::CONTENT_TYPE_JSON, self::CONTENT_TYPE_JSON);
    foreach( $uses as $intended_use ) {
      $intended_uses[] = new CloudwordsIntendedUse($intended_use);
    }
    return $intended_uses;
  }

  public function get_vendor($vendor_id) {
    $vendor = $this->get($this->base_url_with_version . '/vendor/' . $vendor_id . '.json', self::CONTENT_TYPE_JSON, self::CONTENT_TYPE_JSON);
    return new CloudwordsVendor($vendor);
  }
 
  public function get_departments() {
    $departments = $this->get($this->base_url_with_version . '/department.json', self::CONTENT_TYPE_JSON, self::CONTENT_TYPE_JSON);
    return $departments;
  }    

  public function get_source_bundle($project_id) {
    return $this->get($this->base_url_with_version . '/project/' . $project_id . '/file/source/document.json', self::CONTENT_TYPE_JSON, self::CONTENT_TYPE_JSON);
  }

  public function get_translated_bundles($project_id, $language_code) {
    return $this->get($this->base_url_with_version . '/project/' . $project_id . '/file/translated/language/'.$language_code.'/document.json', self::CONTENT_TYPE_JSON, self::CONTENT_TYPE_JSON);
  }

  public function upload_source_preview_bundle($project_id, $document_id, $zip_file) {
    $params = array('file' => '@' . $zip_file);
    return $this->put($this->base_url_with_version . '/project/' . $project_id . '/file/source/document/'.$document_id.'/preview', $params, self::CONTENT_TYPE_JSON, self::CONTENT_TYPE_MULTIPART_FORM_DATA);
  }

  public function upload_translation_preview_bundle($project_id, $language_code, $document_id, $zip_file) {
    $params = array('file' => '@' . $zip_file);
    return $this->put($this->base_url_with_version . '/project/' . $project_id . '/file/translated/language/'.$language_code.'/document/'.$document_id.'/preview', $params, self::CONTENT_TYPE_JSON, self::CONTENT_TYPE_MULTIPART_FORM_DATA);
  }

  public function get_translated_document_by_id($project_id, $language_code, $document_id, $type = 'xliff') {
    return $this->get($this->base_url_with_version . '/project/' . $project_id . '/file/translated/language/'.$language_code.'/document/'.$document_id.'/'.$type, self::CONTENT_TYPE_MULTIPART_FORM_DATA, self::CONTENT_TYPE_JSON);
  }

}

