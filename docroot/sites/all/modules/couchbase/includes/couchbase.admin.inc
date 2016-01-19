<?php

/**
 * @file
 * Implements Couchbase Admin REST Calls 
 */
class CouchbaseAdmin extends CouchbaseRest {
  
  /**
   * The Couchbase class the Admin REST calls
   * will use. This looks like it always "default".  
   * I cannot find how to create different pools.
   */
  private $pool = 'default';

  /**
   * Default Constructor
   * @param 
   * 	$endpoint Couchbase Endpoint, ex: localhost
   * @param 
   *   $port Couchbase port: defaults to 8091
   * @param 
   * 	$username Username used for authentication
   * @param 
   * 	$password Password used for authentication
   */
  public function __construct($endpoint, $port, $username, $password) {
    $this->port = $port;
    $this->endpoint = "http://${username}:$password@{$endpoint}:{$this->port}";
  }
  
  /**
   * Get Couchbase pool information
   */
  public function getPools() {
    return $this->doGet('pools')->decoded_data;
  }
  
  /**
   * Get data about a specific pool
   */
  public function getPool() {
    return $this->doGet("pools/{$this->pool}")->decoded_data;
  }

  /**
   * Set the cluster's memory quota
   * @param 
   *   $memoryQuota Memory Quota for the cluster
   */
  public function initClusterMemoryQuota($memoryQuota = 256) {
    $this->doPost("pools/{$this->pool}", "memoryQuota={$memoryQuota}");
  }
  
  /**
   * Set the cluster's username, password and port
   * @param 
   *   $username Username used for authentication 
   * @param 
   *   $password Password used for authentication
   * @param 
   *   $port Port the cluster will listen on
   */
  public function initCluster($username, $password, $port) {
    $params = array(
      "username={$username}",
      "password={$password}",
      "port={$port}",
    );
    $this->doPost("settings/web", implode("&", $params));
  }
  
  /**
   * Get the auto failover settings from couchbase cluster 
   * @throws 
   *   CouchbaseAdminException Exception thrown if an error was encountered.
   * @return
   *   array Decoded response from couchbase server
   */
  public function getAutoFailoverSettings() {
    return $this->doGet('settings/autoFailover')->decoded_data;
  }
  
  /**
   * Update Couchbase cluster with autofailover settings
   * @param 
	 *   $enabled TRUE or FALSE 
   * @param 
   *   $timeout int value larger than 30. This is the number 
   *   of seconds a node is down before auto failover starts
   */
  public function updateAutoFailoverSettings($enabled, $timeout = 30) {
    if ($timeout < 30) {
      throw new CouchbaseRestException('Timeout cannot be less than 30 seconds');
    }
    $val = ($enabled) ? 'true': 'false';
    $params = array(
      "enabled={$val}",
      "timeout=$timeout",
    );
    $this->doPost('settings/autoFailover', implode('&', $params));
  }
  
  /**
   * Add a node to a cluster. 
   * @param
   *  $remoteEndpoint The remote node to add. This is
   *  in the format of remote.localhost:8091
   * @return
   *   TRUE if node was added to the cluster
   * @throws 
   * 	 CouchbaseAdminException Exception thrown if an error was encountered
   */
  public function addNode($remoteEndpoint, $remoteUsername, $remotePassword) {
    $params = array(
      "hostname={$remoteEndpoint}",
      "user={$remoteUsername}",
      "password={$remotePassword}",
    );
    
    $this->doPost('controller/addNode', implode('&', $params));
  }
  
  /**
   * Rebalance a couchbase cluster
   * @param 
   *   $otpNodes Array of otpNodes
   * @param
   *   $ejectedNodes Array of ejectedNodes
   * @return
   *   TRUE if rebalance command was successful
   * @throws 
   * 	 CouchbaseAdminException Exception thrown if an error was encountered
   */
  public function rebalance($otpNodes, $ejectedNodes) {
    if (!is_array($otpNodes) || !is_array($ejectedNodes)) {
      throw new CouchbaseRestException('otpNodes and ejectedNodes need to be passed as arrays');
    }
    $params = array(
      'knownNodes=' . implode(',', $otpNodes),
      'ejectedNodes=' . implode(',', $ejectedNodes),
    );
    $this->doPost('controller/rebalance', implode('&', $params));
  }
  
  /**
   * Stop rebalancing the cluster
   * @return
   *   array Decoded response from couchbase server
   */
  public function stopRebalance() {
    return $this->doPost('controller/stopRebalance')->decoded_data;
  }
  
  /**
   * Get the rebalance status
   * @return
   *   array Decoded response from couchbase server
   */
  public function rebalanceProgress() {
    return $this->doGet("pools/{$this->pool}/rebalanceProgress")->decoded_data;
  }
  
  /**
   * Eject a node from the Couchbase cluster
   * @param 
   *   $otpNode String reprensentation of otpNode. This value
   *   can be obtained from the getPool() method.
   * @return
   *   TRUE if eject command was successful
   */
  public function ejectNode($otpNode) {
    if (!isset($otpNode) && empty($otpNode)) {
      throw new CouchbaseRestException('otpNode value must be provided for a node to be ejected');
    }
    $this->doPost('controller/ejectNode', "otpNode={$otpNode}");
  }
  
  /**
   *
   * Failover a node from the Couchbase cluster
   * @param
   *   $otpNode String reprensentation of otpNode. This value
   *   can be obtained from the getPool() method.
   * @return
   *   TRUE if failover command was successful
   */
  public function failOver($otpNode) {
    if (!isset($otpNode) && empty($otpNode)) {
      throw new CouchbaseRestException('otpNode value must be provided for a node to be ejected');
    }
    $this->doPost('controller/failOver', "otpNode={$otpNode}");
  }
  
  /**
   * Get information about the node
   */
  public function getNodeSelf() {
    return $this->doGet('nodes/self')->decoded_data;
  }
  
  /**
   * Get health status of the node 
   */
  public function getNodeStatus() {
    return $this->doGet('nodeStatuses')->decoded_data;
  }
  
  /**
   * Get all buckets available
   */
  public function getBuckets() {
    return $this->doGet("pools/{$this->pool}/buckets")->decoded_data;
  }
  
  /**
   * Create a bucket from the REST API.  This method
   * will throw an Exception if there are errors.
   *   @param 
   *     $bucket Bucket Name
   *   @param 
   *     $ramQuotaMB The amount of ram to allocate to this bucket
   *   @param 
   *     $authType Can be none or sasl
   *   @param 
   *     $saslPassword If $authType is sasl, specify a password
   *   @param 
   *     $proxyPort Proxy Port this bucket will attach to
   *   @param 
   *     $bucketType BucketType
   */
  public function createBucket($bucket, $ramQuotaMB = 100, $authType = 'none', $saslPassword = '', $replicaNumber = 1, $proxyPort = 11215, $bucketType = 'couchbase') {
    // @todo: perform better setup of the params, particularly for auth type and ports
    $params = array(
      "name=$bucket",
      "ramQuotaMB=$ramQuotaMB",
      "authType=$authType",
      "saslPassword=$saslPassword",
      "replicaNumber=$replicaNumber",
      "proxyPort=$proxyPort",
      "bucketType=$bucketType",
    );
    $this->doPost("pools/{$this->pool}/buckets", implode("&", $params));
  }
  
  /**
   * Get a bucket given a name
   * @param 
   *   $bucket Bucket to get
   * @return 
   *   associative array
   */
  public function getBucket($bucket) {
    if (!isset($bucket) || empty($bucket)) {
      throw new CouchbaseRestException('Bucket name needs to be supplied');
    }
    return $this->doGet("pools/{$this->pool}/buckets/{$bucket}")->decoded_data;
  }
  
  /**
   * Delete a particular bucket 
   * @param 
   *   $bucket Bucket to delete
   * @throws 
   * 	 CouchbaseRestException Exception thrown if bucket is empty
   */
  public function deleteBucket($bucket) {
    if (!isset($bucket) || empty($bucket)) {
      throw new CouchbaseRestException('Bucket name needs to be supplied');
    }
    return $this->doDelete("pools/{$this->pool}/buckets/{$bucket}");
  }

  /**
   * Flush a particular bucket
   * @param 
   * 	$bucket Bucket to flush
   * @throws CouchbaseRestException
   */
  public function flushBucket($bucket) {
    if (!isset($bucket) || empty($bucket)) {
      throw new CouchbaseRestException('Bucket name needs to be supplied');
    }
    return $this->doPost("pools/{$this->pool}/buckets/{$bucket}/controller/doFlush");
  }
  
  /**
   * Get bucket stats 
   * @param 
   *   $bucket
   * @throws 
   *   CouchbaseAdminException
   */
  public function getBucketStats($bucket) {
    if (!isset($bucket) || empty($bucket)) {
      throw new CouchbaseRestException('Bucket name needs to be supplied');
    }
    return $this->doGet("pools/{$this->pool}/buckets/{$bucket}/stats")->decoded_data;
  }
  
}
