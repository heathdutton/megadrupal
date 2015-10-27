<?php

/**
 * @file
 * Integrate Couchbase with drush
 */

/**
* Implements hook_drush_help().
*/
function couchbase_drush_help($section) {
  switch ($section) {
    case 'drush:cb-server-list':
      return dt('List all servers in a Couchbase cluster');
    case 'drush:cb-server-info':
      return dt('Show details on one Couchbase server');
    case 'drush:cb-server-add':
      return dt('Add a new Couchbase server to an existing cluster');
    case 'drush:cb-rebalance':
      return dt('Rebalance the Couchbase cluster');
    case 'drush:cb-rebalance-stop':
      return dt('Stop rebalancing the Couchbase cluster');
    case 'drush:cb-rebalance-progress':
      return dt('Show rebalance progress details');
    case 'drush:cb-failover':
      return dt('Failover a particular server in a cluster');
    case 'drush:cb-memory-init':
      return dt('Set memory quota of the cluster');
    case 'drush:cb-cluster-init':
      return dt('Set username, password and port of the cluster');
    case 'drush:cb-bucket-list':
      return dt('List all buckets in a cluster');
    case 'drush:cb-bucket-create':
      return dt('Add a new bucket to the cluster');
    case 'drush:cb-bucket-delete':
      return dt('Delete an existing bucket');
    case 'drush:cb-bucket-flush':
      return dt('Flush a given bucket');
  }
}

/**
 * Implements hook_drush_command().
 */
function couchbase_drush_command() {
  $items = array();
  $items['cb-server-list'] = array(
    'callback' => 'couchbase_drush_server_list',
    'description' => dt('List all servers in a Couchbase cluster'),
    'drupal dependencies' => array('couchbase'),
    'arguments' => array(
      'cluster' => 'The cluster connection declared in settings.php. If no cluster passed the "default" server is used.',
    ),
    'aliases' => array('csl'),
  );
  $items['cb-server-info'] = array(
    'callback' => 'couchbase_drush_server_info',
    'description' => dt('Show details on one Couchbase server'),
  	'drupal dependencies' => array('couchbase'),
    'arguments' => array(
      'cluster' => 'The cluster connection declared in settings.php. If no cluster passed the "default" server is used.',
    ),
    'aliases' => array('csi'),
  );
  $items['cb-server-add'] = array(
    'callback' => 'couchbase_drush_server_add',
    'description' => dt('Add a server to the cluster'),
    'drupal_dependencies' => array('couchbase'),
    'arguments' => array(
      'cluster' => 'Couchbase cluster to add new server to',
      'hostname'=> 'New server hostname',
      'user' => 'New server username',
      'password' => 'New server password',
    ),
    'required-arguments' => TRUE,
   	'examples' => array(
    	'drush cb-server-add default 127.0.0.1:8091 username password' => 'Add the localhost server to the default cluster',
    ),
   	'aliases' => array('csa'),
  );
  $items['cb-rebalance'] = array(
    'callback' => 'couchbase_drush_rebalance',
    'description' => 'Start rebalancing the Couchbase cluster',
    'drupal_dependencies' => array('couchbase'),
    'arguments' => array(
  		'cluster' => 'The cluster connection declared in settings.php. If no cluster passed the "default" cluster is used.',
    ),
    'aliases' => array('cr'),
  );
  $items['cb-rebalance-stop'] = array(
    'callback' => 'couchbase_drush_stop_rebalance',
    'description' => 'Stop rebalancing the Couchbase cluster',
    'drupal_dependencies' => array('couchbase'),
    'arguments' => array(
			'cluster' => 'The cluster connection declared in settings.php. If no cluster passed the "default" cluster is used.',
    ),
    'aliases' => array('crs'),
  );
  $items['cb-rebalance-progress'] = array(
    'callback' => 'couchbase_drush_rebalance_progress',
    'description' => 'Show rebalance progress',
    'drupal_dependencies' => array('couchbase'),
    'arguments' => array(
			'cluster' => 'The cluster connection declared in settings.php. If no cluster passed the "default" cluster is used.',
    ),
    'aliases' => array('crp'),
  );
  $items['cb-failover'] = array( 
    'callback' => 'couchbase_drush_failover',
    'description' => 'Failover a particular server in a Couchbase cluster',
    'drupal_dependencies' => array('couchbase'),
  	'arguments' => array(
  		'cluster' => 'The cluster connection declared in settings.php. If no cluster passed the "default" cluster is used.',
    ),
    'aliases' => array('cf'),
  );
  $items['cb-memory-init'] = array(
  	'callback' => 'couchbase_drush_memory_init',
    'description' => 'Set memory quota of the cluster',
    'drupal_dependencies' => array('couchbase'),
    'arguments' => array(
      'cluster' => 'The cluster connection declared in settings.php.',
      'memoryQuota' => 'Memory Quota for the cluster. This value defaults 256'
    ),
    'required-arguments' => TRUE,
    'examples' => array(
     	'drush cb-memory-init default 512' => 'Set the cluster memory quota to 512 MB',
    ),
    'aliases' => array('cmi'),
  );
  $items['cb-cluster-init'] = array(
    'callback' => 'couchbase_drush_cluster_init',
    'description' => 'Set username, password and port of the cluster',
    'drupal_dependencies' => array('couchbase'),
    'arguments' => array(
      'cluster' => 'The cluster connection declared in settings.php.',
      'username' => 'The username used for the cluster',
      'password' => 'The password used for the cluster',
      'port' => 'The port the cluster will listen on',
    ),
  	'required-arguments' => TRUE,
    'examples' => array(
     	'drush cb-cluster-init default admin password 8091' => 'Set the cluster username to "admin", password to "password" and port to 8091',
    ),
    'aliases' => array('cci'),
  );
  $items['cb-bucket-list'] = array(
    'callback' => 'couchbase_drush_bucket_list',
    'description' => 'List all the buckets in the cluster',
  	'drupal_dependencies' => array('couchbase'),
  	'arguments' => array(
			'cluster' => 'The cluster connection declared in settings.php. If no cluster passed the "default" cluster is used.',
  	),
  	'aliases' => array('cbl'),
  );
  $items['cb-bucket-create'] = array(
    'callback' => 'couchbase_drush_bucket_create',
    'description' => 'Add a new bucket to the cluster',
  	'arguments' => array(
  		'cluster' => 'The cluster connection declared in settings.php. If no cluster passed the "default" cluster is used.',
  		'bucket' => 'The new bucket name',  		
    ),
    'required-arguments' => TRUE,
    'options' => array(
      'ram-quota' => array(
      	'description' => 'The amount of ram to allocate to this bucket.  This value defaults to 100MB',
      	'example-value' => '128 for 128MB',
      ),
      'auth-type' => array(
        'description' => 'The authentication type for this bucket. Can be none or sasl. Defaults to none',
      ),
      'sasl-password' => array(
        'description' => 'If auth_type is set to sasl, the sasl_password option must be set.',
      ),
      'replica-number' => array(
        'description' => 'Specify the number of replicas. Defaults to 1 if this option is not set.'
      ),
      'proxy-port' => array(
        'description' => 'Proxy port this bucket will attach to.  Defaults to 11215',
      ),
      'bucket-type' => array(
        'description' => 'The bucket type used by this bucket.',
        'example-value' => 'membase or couchbase',
      ),
    ),
  	'examples' => array(
    	'drush cb-bucket-create default foo' => 'Create the "foo" bucket with default options on the "default" cluster.',
    	'drush cb-bucket-create default foo --ram-quota=256 --proxy-port=11256' => 'Create the "foo" bucket with a ram quota of 256MB and listen on proxy port 11256',
    	'drush cb-bucket-create default foo --auth-type=sasl --sasl-password=password --bucket-type=membase' => 'Create the "foo" bucket using sasl authentication, set sasl-password to password and use bucket-type membase',
    ),
    'aliases' => array('cbc'),
  );
  $items['cb-bucket-delete'] = array(
  	'callback' => 'couchbase_drush_bucket_delete',
      'description' => 'Add a new bucket to the cluster',
    	'arguments' => array(
    		'cluster' => 'The cluster connection declared in settings.php. If no cluster passed the "default" cluster is used.',
    		'bucket' => 'Name of bucket to delete',  		
    ),
  	'required-arguments' => TRUE,
  	'aliases' => array('cbd'),
  );
  $items['cb-bucket-flush'] = array(
  	'callback' => 'couchbase_drush_bucket_flush',
      'description' => 'Flush a couchbase bucket',
    	'arguments' => array(
     		'cluster' => 'The cluster connection declared in settings.php. If no cluster passed the "default" cluster is used.',
     		'bucket' => 'Name of bucket to flush',  		
      ),
    	'required-arguments' => TRUE,
    	'aliases' => array('cbf'),
  );
  return $items;
}

/**
 * Drush command to get the list of servers
 */
function couchbase_drush_server_list($cluster = 'default') {
  try {
    $cb_admin = couchbase_admin($cluster);
    $pool = $cb_admin->getPool();
    foreach ($pool['nodes'] as $node) { 
      drush_print($node['otpNode'] . ' ' . $node['hostname']  . ' ' . $node['status'] . ' ' . $node['clusterMembership']);
    }
  }
  catch (CouchbaseRestException $re) {
    return drush_set_error($re->getMessage());
  }
}

/**
 * Drush command to show details on one Couchbase server 
 */
function couchbase_drush_server_info($cluster = 'default') {
  try {
    $cb_admin = couchbase_admin($cluster);
    $node = $cb_admin->getNodeSelf();
    drush_print_r($node);
  }
  catch (CouchbaseRestException $re) {
    return drush_set_error($re->getMessage());
  }
}

/**
 * Drush command to add a server to the cluster
 */
function couchbase_drush_server_add($cluster, $hostname, $user, $pass) {
  try {
    $cb_admin = couchbase_admin($cluster);
    $cb_admin->addNode($hostname, $user, $pass);
    drush_log(dt('Server successfully added to the cluster'), 'success');
  }
  catch (CouchbaseRestException $re) {
    return drush_set_error($re->getMessage());
  }
}

/**
 * Drush command to rebalance a Couchbase cluster
 */
function couchbase_drush_rebalance($cluster = 'default') {
  try {
    $cb_admin = couchbase_admin($cluster);
    $pool =  $cb_admin->getPool();
    $otpNodes = array();
    foreach ($pool['nodes'] as $node) {
      $otpNodes[] = $node['otpNode'];
    }
    $cb_admin->rebalance($otpNodes, array());
    drush_log(dt('Cluster rebalance started'), 'success');
  }
  catch (CouchbaseRestException $re) {
    return drush_set_error($re->getMessage());
  }
}

/**
 * Drush command to stop rebalancing 
 */
function couchbase_drush_stop_rebalance($cluster = 'default') {
  try {
    $cb_admin = couchbase_admin($cluster);
    $cb_admin->stopRebalance();
    drush_log(dt('Cluster rebalance stopped'), 'success');
  }
  catch (CouchbaseRestException $re) {
    return drush_set_error($re->getMessage());
  }
}

/**
 * Drush command to show rebalance progress 
 */
function couchbase_drush_rebalance_progress($cluster = 'default') {
  try {
    $cb_admin = couchbase_admin($cluster);
    $progress = $cb_admin->rebalanceProgress();
    if ($progress['status'] == 'none') {
      drush_print(dt('No rebalancing at this time'));
    }
    else {
      drush_print(dt('Rebalance is @rebalance', array('@rebalance' => $progress['status'])));
      foreach ($progress as $key => $value) {
        drush_print(dt('@server progress: @progress%', array('@server' => $key, '@progress' => $value['progress']*100)));
      }
    }
  }
  catch (CouchbaseRestException $re) {
    return drush_set_error($re->getMessage());
  }
}

/**
 * Drush command to failover a particular server in a cluster 
 */
function couchbase_drush_failover($cluster = 'default') {
  try {
    $cb_admin = couchbase_admin($cluster);
    // get the available servers
    $servers = $cb_admin->getNodeStatus();
    // if only one server no failover allowed
    if (count($servers) == 1) {
      drush_log(dt('Not enough servers to issue failover command'), 'error');
      return;
    }
    $choices = array();
    foreach ($servers as $key => $value) {
      $choices[$key] = dt($key);
    }
    if ($choice = drush_choice($choices, 'Choose the server to failover')) {
      if (isset($servers[$choice])) {
        $cb_admin->failover($servers[$choice]['otpNode']);
        drush_log(dt('Failover command issued.  Please rebalance the cluster.'), 'success');
      }
    }
  }
  catch (CouchbaseRestException $re) {
    return drush_set_error($re->getMessage());
  }
}

/**
 * Drush command to initialize memory quota for the cluster
 * @param $server
 * @param $memoryQuota
 */
function couchbase_drush_memory_init($cluster = 'default', $memoryQuota = 256) {
  try {
   $cb_admin = couchbase_admin($cluster);
   $cb_admin->initClusterMemoryQuota($memoryQuota); 
   drush_log(dt('Memory quota for the cluster is set'), 'success');
  }  
  catch (CouchbaseRestException $re) {
    return drush_set_error($re->getMessage());
  }
}

/**
 * Drush command to set the username, password and port of a cluster
 * @param $server
 * @param $username
 * @param $password
 * @param $port
 */
function couchbase_drush_cluster_init($cluster, $username, $password, $port) {
  try {
    $cb_admin = couchbase_admin($cluster);
    $cb_admin->initCluster($username, $password, $port);
    drush_log(dt('Cluster username, password and port are set'), 'success');
  }
  catch (CouchbaseRestException $re) {
    return drush_set_error($re->getMessage());
  }
}

/**
 * Drush command to list all buckets in a Couchbase cluster
 * @param $cluster
 */
function couchbase_drush_bucket_list($cluster = 'default') {
  try {
    $cb_admin = couchbase_admin($cluster);
    $buckets = $cb_admin->getBuckets();
    foreach ($buckets as $bucket) {
      $bucket_info = <<<EOF
{$bucket['name']}
  bucketType:     {$bucket['bucketType']}
  authType:       {$bucket['authType']}
  saslPassword:   {$bucket['saslPassword']}
  proxyPort:      {$bucket['proxyPort']}
  replicaIndex:   {$bucket['replicaIndex']}
  replicaNumber:  {$bucket['replicaNumber']}
  ramQuota:       {$bucket['quota']['ram']}
  ramUsed:        {$bucket['basicStats']['diskUsed']}
  opsPerSec:      {$bucket['basicStats']['opsPerSec']}
  diskFetches:    {$bucket['basicStats']['diskFetches']}
  itemCount:      {$bucket['basicStats']['itemCount']}
  diskUsed:       {$bucket['basicStats']['diskUsed']}
EOF;
      drush_print($bucket_info);
    }
  }
  catch (CouchbaseRestException $re) {
    return drush_set_error($re->getMessage());
  }
}

/**
 * Drush command to create a bucket 
 * @param $cluster
 * @param $bucket
 */
function couchbase_drush_bucket_create($cluster, $bucket) {
  try {
    $ramQuota = drush_get_option('ram-quota', 100);
    $authType = drush_get_option('auth-type', 'none');
    $saslPassword = drush_get_option('sasl-password', '');
    $replicaNumber = drush_get_option('replica-number', 1);
    $proxyPort = drush_get_option('proxy-port', 11215);
    $bucketType = drush_get_option('bucket-type', 'couchbase');
    $cb_admin = couchbase_admin($cluster);
    $cb_admin->createBucket($bucket, $ramQuota, $authType, $saslPassword, $replicaNumber, $proxyPort, $bucketType);
    drush_log(dt('@bucket bucket has been created', array('@bucket' => $bucket)), 'success');
  }
  catch (CouchbaseRestException $re) {
    return drush_set_error($re->getMessage());
  }
}

/**
 * Drush command to delete a bucket
 * @param $cluster
 * @param $bucket
 */
function couchbase_drush_bucket_delete($cluster, $bucket) {
  try {
    $cb_admin = couchbase_admin($cluster);
    $cb_admin->deleteBucket($bucket);
    drush_log(dt('@bucket bucket has been deleted', array('@bucket' => $bucket)), 'success');
  }
  catch (CouchbaseRestException $re) {
    return drush_set_error($re->getMessage());
  }
}

/**
 * Drush command to flush a bucket 
 * @param $cluster
 * @param $bucket
 */
function couchbase_drush_bucket_flush($cluster, $bucket) {
  try {
    $cb_admin = couchbase_admin($cluster);
    $cb_admin->flushBucket($bucket);
    drush_log(dt('@bucket bucket has been flushed', array('@bucket' => $bucket)), 'success');
  }
  catch (CouchbaseRestException $re) {
    return drush_set_error($re->getMessage());
  }
}