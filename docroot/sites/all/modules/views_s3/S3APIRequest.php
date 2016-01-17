<?php

/**
 * @file
 *   Class that connects with S3 to query for the actual files
 */

if (!class_exists('AmazonS3')) { libraries_load('awssdk'); libraries_load('awssdk-s3'); };

/**
 * @class
 *   @TODO: documentation
 * Possible and recommended improvement: implementing a caching logic.
 * See Last.FM module for a smart example:
 *<p></p> http://drupalcode.org/project/lastfm.git/blob/refs/heads/7.x-2.x:/lastfm.api.inc
 */
class S3APIRequest {
  const VIEWS_S3_DEFAULT_DEBUG_MODE = FALSE;
  

  // Default Bucket Name for the query.
  public $bucket_name;

  // Default CDN distribution name for the query.
  public $distribution;

  // Default CDN distribution domain name for the query.
  public $distribution_hostname;

  // Default Lifetime for the signed URL.
  private $lifetime;

  // Flag to designate whether we are in debug mode.
  private $debug_mode;
  
  // API call arguments. Use S3APIRequest->addArgument() to set.
  private $arguments = array();
  
  // API sort criteria
  private $criteria = array();

  private $s3 = NULL;

  /**
   * Constructor.
   */
  public function __construct() {
    $this->debug_mode = variable_get('views_s3_debug_mode', S3APIRequest::VIEWS_S3_DEFAULT_DEBUG_MODE);
    $this->log(variable_get('aws_key', ''), 'AWS key');
    $this->log(t('Not shown'), 'AWS secret');
    
    $this->s3 = new AmazonS3();
    if (_is_cloudfront_active()) {
      $this->cloudfront = new AmazonCloudFront();
    }

    $this->log('S3 object has been created');
  }
  
  /**
   * Set bucket name
   */
  public function setParam($param_name, $value) {
    $this->{$param_name} = $value;
    $this->log($value, t('Set parameter %param_name', array('%param_name' => $param_name)));
  }
  
  /**
   * Gets the content of a Bucket
   * @param $filename
   *   If specified the request will only try to retrieve the given file
   */
  public function getBucketContents($filename = NULL) {
    // TODO: Get paginated content by using http://docs.amazonwebservices.com/AWSSDKforPHP/latest/#m=AmazonS3/list_objects
    if (empty($this->bucket_name)) {
      throw new Exception(t('You need to supply a bucket name before getting its contents. Please open <em>Advanced</em> settings and select your bucket in <em>Query settings</em> under <em>Other</em> section.'));
    }
    if ($filename) {
      return array($filename => $this->s3->get_object_metadata($this->bucket_name, $filename));
    }
    $response = $this->s3->list_objects($this->bucket_name);
    if (!$response->isOK()) {
      return NULL;
    }

    $contents = $response->body->Contents;
    
    $output = array();
    foreach ($contents as $object) {
      $object->to_array()->map(function($item, $key, &$data) {
        $data[$key] = $item;
      }, $data);
      $output[$data['Key']] = $data;
    }

    $this->log($output);    
    return $output;
  }

  /**
   * Sets an argument for the request.
   */
  public function addArgument($key, $value) {
    if ($key == 'Key') {
      $value = urldecode($value);
    }
    $this->arguments[$key] = $value;
    $this->log(t('Added argument @key => @value', array('@key' => $key, '@value' => $value)));
    return $this;
  }

  /**
   * Executes the request. Returns the response data.
   */
  public function execute() {
    // If there is an argument by Key, do not fetch all results, just fetch the relevant one
    if (!empty($this->arguments['Key'])) {
      // Fetching one specific result
      $contents = $this->filter($this->getBucketContents($this->arguments['Key']));
    }
    else {
      // Ask for the information in S3 and filter it according to current arguments
      $contents = $this->filter($this->getBucketContents($this->arguments['Key']));
    }
    if (!empty($this->criteria)) {
      $this->order($contents);
    }
    $this->log('Query has been executed.');
    $this->parse($contents);
    return $contents;
  }
  
  /**
   * Filters the results based on current arguments
   */
  private function filter($data) {
    foreach ($data as $prefix => $item) {
      $data[$prefix] = array_merge($item, pathinfo($item['Key']));
      // Get the URL
      $data[$prefix]['signed_url'] = $this->s3->get_object_url($this->bucket_name, $item['Key'], $this->lifetime);
      if (_is_cloudfront_active()) {
        if (empty($this->distribution_hostname)) {
          $response = $this->cloudfront->get_distribution_info($this->distribution);
          if ($response->isOK()) {
            $this->distribution_hostname = (string)$response->body->DomainName;
          }
        }
        $data[$prefix]['cdn_signed_url'] = $this->cloudfront->get_private_object_url($this->distribution_hostname, $item['Key'], $this->lifetime);
      }
      // Filter the output array
      // @FIXME: Does not work for empty filters
      foreach ($this->arguments as $key => $value) {
        if (isset($data[$prefix][$key]) && !empty($value)) {
          if ($key == 'extension' || $key == 'ETag' || $key == 'StorageClass') {
            if ($key == 'ETag') {
              $data[$prefix][$key] = substr($data[$prefix][$key], 1, -1);
            }
            if ($value != $data[$prefix][$key]) {
              unset($data[$prefix]);
            }
          }
        }
        else {
          unset($data[$prefix]);
        }
      }
    }
    $this->log('Query has been filtered.');
    return $data;
  }

  /**
   * Parses the response data.
   */
  private function parse(&$data) {
    $parsed_data = array();
    foreach ($data as $prefix => $item) {
      $item['ETag'] = substr($item['ETag'], 1, -1);
      $parsed_data[] = (object)$item;
    }
    $data = $parsed_data;
  }

  /**
  * Logs debug info
  */
  private function log($data, $label = '') {
    if ($this->debug_mode) {
      if (module_exists('devel')) {
        dpm($data, empty($label) ? 'S3APIRequest log' : $label);
      }
      if (!is_string($data)) {
        $data = serialize($data);
      }
      watchdog('views_s3_log', $data, array(), WATCHDOG_DEBUG);
    }
  }
  
  /**
   * Orders the results based on the order criteria
   * Note: this becomes much more complicated when paginated
   * @param $contents
   *   Keyed array to order
   */
  // @FIXME: This fails when multiple sort criteria
  public function order(&$contents) {
    // Start ordering by the last criterion
    foreach ($this->criteria as $criterion) {
      $sorter = $this->getSorter($criterion, $contents);
      $criterion['direction'] == 'ASC' ? asort($sorter) : arsort($sorter);
      $sorted_contents = array();
      foreach (array_keys($sorter) as $key) {
        $sorted_contents[$key] = $contents[$key];
      }
      $contents = $sorted_contents;
    }
  }

  /**
   * Add order criterion
   */
  public function addOrderCriterion($item) {
    array_unshift($this->criteria, $item);
  }

  /**
   * Clears all the arguments
   */
   public function clearAllArguments() {
     $this->arguments = array();
     $this->log('All arguments have been deleted.');
   }

  /**
   * Clears all the arguments
   */
   public function getArguments() {
     return $this->arguments;
   }

   /**
    * Clear one specific argument
    */
    public function clearArgument($name = NULL) {
      if (is_null($name)) {
        $this->clearAllArguments();
      }
      elseif (isset($this->arguments[$name])) {
        unset($this->arguments[$name]);
      }
    }

    /**
     * Get sorter reference
     */
    private function getSorter($criterion, $contents) {
      foreach ($contents as $key => $value) {
        $sorter_item = $value[$criterion['field']];
        if (!empty($criterion['sorting_function'])) {
          $function = $criterion['sorting_function'];
          $sorter_item = $function($sorter_item, $criterion);
        }
        $sorter[$key] = $sorter_item;
      }
      return $sorter;
    }

    /**
     * Get simplified list of buckets
     */
    protected function getBucketList() {
      if (variable_get('aws_key', '') == '' || variable_get('aws_secret', '') == '') {
        return array('' => t('- Enter your credentials first -'));
      }
      return drupal_map_assoc($this->s3->get_bucket_list());
    }
    
    /**
     * Get file metadata
     * @param $filter
     *   Filter array with key & vaule to match a file against
     * @return
     *   An array containing the file metadata
     */
    public function getObjectMetadata($filter) {
      if (empty($filter)) {
        throw new Exception(t('You need to supply a filter.'));
        return array();
      }
      if (!empty($filter['Key'])) {
        $contents = $this->getBucketContents($filter['Key']);
        return $contents[$filter['Key']];
      }
      $contents = $this->getBucketContents();
      foreach ($contents as $prefix => $item) {
        foreach ($filter as $key => $value) {
          if ($item[$key] == $value) {
            return $contents[$prefix];
          }
        }
      }
      return array();
    }

    /**
     * Delete a file from the bucket
     * @param $filter
     *   Filter array with key & vaule to match a file against or an string the prefix of the object.
     * @return
     *   A boolean indicating success
     */
    public function deleteObject($filter) {
      if (is_array($filter)) {
        $object = $this->getObjectMetadata($filter);
        $prefix = $object['Key'];
      }
      else {
        $prefix = $filter;
      }
      $response = $this->s3->delete_object($this->bucket_name, $prefix);
      return $response->isOK();
    }
    
    /**
     * Copy an object from a source to a destination. This is a mini wrapper for AmazonS3::copy_object
     */
    public function copyObject($source, $destination) {
      $response = $this->s3->copy_object($source, $destination);
      return $response->isOK();
    }
    
    /**
     * Change storage type for a file. This is a mini wrapper for AmazonS3::change_storage_redundancy
     */
    public function changeStorage($param) {
      $response = $this->s3->change_storage_redundancy($this->bucket_name, $param['filename'], $param['storage']);
      return $response->isOK();
    }
}
