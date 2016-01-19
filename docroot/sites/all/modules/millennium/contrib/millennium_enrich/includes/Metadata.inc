<?php

/*
interface MetadataInterface {
  public function calculateUrl($nid, $subject);
}
*/

class Metadata {

  // sourceId: A string for the source; will be used in database indexes
  protected $sourceId;

  // cacheLifetime: number of seconds after which data is considered stale.
  // -1 for "never", 0 = always
  protected $cacheLifetime = 0;

  // Number of maximum simultaneous connections for harvest
  protected $maxConnections = 10;

  // Sample structure of a subject
  //   Subjects could be $node objects or arrays, they just contain enough information
  //   for the Metadata class to determine what the object is and query the remote
  //   sources for information.
  /*
  public $subjectSample = array(
    'nid' => 0, // This is the id of the item the metadata describes.
    'biblio' => array(
      'authors' => array('tom', 'jerry'),
      'publisher' => 'McGraw Johnson and Thomas, Inc.',
      'year_published' => 1998
      ),
    'taxonomy' => array(
      1 => array(1,2,3),
      2 => array(3,4,5),
    ),
  );
  */

  // TODO: maybe include some RDFa "stuff" here?

  function __construct($sourceId) {
    $this->sourceId = $sourceId;
  }

  function getMaxConnections() {
    return $this->maxConnections;
  }

  /**
   * obtains metadata from a single source, for one subject (nid). Only
   * returns info from local cache. A simple wrapper for the getMulti() method.
   */
  public function get($nid) {
    if (!$nid) {
      return FALSE;
    }
    $results = $this->getMulti(array($nid));
    if (isset($results[$nid])) {
      return $results[$nid];
    }
    return FALSE;
  }
  /**
   * Obtains an arry of metadata objects from a single source, for one or more
   * subjects (array of nids). Only returns info from local cache.
   *
   * @param $nids
   *    an array of nids (e.g., array(1,2,3))
  */
  public function getMulti($nids) {
    #drupal_set_message("getMulti() for nids = ");
    #dpm($nids);
    $results = array();
    // Chunk request to avoid MySQL overflow
    $chunks = array_chunk($nids, 100);
    // Process chunk
    foreach ($chunks as $chunk) {
      #drupal_set_message("getMulti() for chunk = ");
      #dpm($chunk);
      // Build query
      $handle = db_query(
        "SELECT nid, data, found, updated_timestamp FROM {metadata_storage}
        WHERE source= :source
        AND nid IN (:nids)",
        array(':source' => $this->sourceId, ':nids' => $chunk)
      );
      foreach ($handle as $result) {
        #drupal_set_message("getMulti() for nid = ". $result->nid);
        #dpm($result);
        $results[$result->nid] = $result;
        // Unserialize data property
        $results[$result->nid]->data = unserialize($result->data);
        // Add a staleness property
        $results[$result->nid]->stale = $this->isStale($result->updated_timestamp);
      }
    }
    return $results;
  }

  /**
   * Main processing function: recieves array of records that need metadata,
   * calculates necessary data requests, runs fetch info from web, converts each
   * response into a metadata arrays which get stored in the database.
   *
   * @param $subjects
   *   a keyed array ($nid => $subjects) of subjects
   */
  public function harvest($subjects) {
    // First, build "flat" queue of requests vs callbacks
    $queue = $this->harvestBuildRequestQueue($subjects);

    // Split queue into bits, split by max number of wanted connections.
    $chunks = array_chunk($queue, $this->maxConnections);
    foreach ($chunks as $chunk) {
      // Run actual web fetch
      $results = $this->harvestFetchQueuedJobs($chunk);
      // Store fetched results
      $this->harvestProcessAndStoreFetched($chunk, $results);
    }
  }

  // Build "flat" queue of requests vs callbacks
  public function harvestBuildRequestQueue($subjects, $force_update = FALSE) {
    $queue = array();
    $harvestJobs = $this->harvestCalculateJobs($subjects, $force_update);
    foreach ($harvestJobs as $nid => $harvestJob) {
      foreach ($harvestJob as $job_part) {
        $queue[] = $job_part;
      }
    }
    return $queue;
  }

  public function harvestFetchQueuedJobs($queue) {
    $requests = array();
    if (!is_array($queue)) {
      return array();
    }
    foreach ($queue as $id => $job_part) {
      $requests[$id] = $job_part["request"];
    }
    $results = $this->multiRequest($requests);
    return $results;
  }

  public function harvestProcessAndStoreFetched($queue, $results) {
    // Process each result with a callback
    foreach ($results as $id => $content) {
      $callback = $queue[$id]['callback'];
      $metadata = $this->$callback($content);
      #echo "Metadata returned = " . var_export($metadata, TRUE) . "<br>";
      #dpm("Metadata returned:");
      #dpm($metadata);
      /*if ($id < 4){
        drupal_set_message("Processing result id = $id. Job ==");
        dpm($queue[$id]);
        dpm($content);
        dpm($metadata);
      }
       *
       */
      // Store metadata
      $this->storeSingle($queue[$id]['nid'], $metadata);
    }
  }

  /* Generates harvest jobs to run for requested subjects, depending on
   * available metadata and its staleness.
   *
   *  @param $subjects
   *    array of keyed node objects (nid => node object)
   *  @return array(
   *    0 => array(
   *      'nid' => id original subject
   *      'request' => array('url' => http....,
   *      'callback' => 'functionName'
   *
   */
  public function harvestCalculateJobs($subjects, $force_update = FALSE) {
    #drupal_set_message("harvestCalculateJobs(): Calculating for ". sizeof($subjects) . " subjects");
    $harvestJobs = array();
    foreach ($subjects as $nid => $subject) {
      // $subject needs to be an array
      if (is_array($subject)) {
        $subject = (object) $subject;
      }

      // Only queue if data is stale or non existent
      $existing = $this->querySingle($nid);
      #echo "existing: <p>" . var_export($existing, TRUE) . "</p><hr>";
      if (!is_object($existing) || $existing->stale === TRUE || $force_update) {
        $job = $this->calculateHarvestJobSingle($nid, $subject);
        if ($job) {
          $harvestJobs[$nid] = $job;
        }
      }
    }
    #drupal_set_message("harvestCalculateJobs(): Finished calculation: result = ". sizeof($harvestJobs) . " jobs");
    return $harvestJobs;
  }

  // Recieves a subject OBJECT and returns an array of (requests,callback) arrays to process
  /*
  public function calculateHarvestJobSingle($nid, $subject) {
    $harvestJob = array();
    // This example tries to see if taxonomy terms from $subject (a node object) and:
    //   * adds URLs to dbpedia pages using that term's name
    //   * adds a callback function (dbpediaTaxonomyTerm) that will process that URL's data
    if ($subject->taxonomy) {
      // Cycle through terms
      foreach ($subject->taxonomy as $term_id) {
        $term = taxonomy_get_term($term_id);
        $harvestJob[] = array(
          'nid' => $nid,
          'request' => array(
            'url' => "http://dbpedia.org/page/" . $term->name,
            ),
          'callback' => 'MetadataXYZ::responseExtractData'
        );
      }
    }
    return $harvestJob;
  }
  */

  // Recieves raw data from a server response, processes it (scrape, DOM parsing, etc)
  //   and returns a metadata record to store in the database. This is the last
  //   step before storage; later processing could be done when displaying the information.
  /*
  public function responseExtractMetadata($raw_response) {
    $metadata = array();
    // This example finds the <div id=content> tag that has an abstract of the term
    // THIS DOESN'T WORK but would be awesome if it would =)
    $xml = simplexml_load_string($data);
    $result = $xml->xpath('//*[@id="content"]');
    return $metadata;
  }
  */

  /**
   * Pipelined fetch from the Web. Requires PHP CURL library.
   *
   * @see millennium_enrich_requirements()
   * See: http://www.phpied.com/simultaneuos-http-requests-in-php-with-curl/
   */
  public function multiRequest($requests, $options = array()) {
    // array of curl handles
    $curl_handles = array();
    // data to be returned
    $result = array();
    // If no requests, return empty array
    if (!$requests) {
      return $result;
    }
    // multi handle
    $multi_handle = curl_multi_init();

    // loop through $data and create curl handles
    // then add them to the multi-handle
    foreach ($requests as $index => $request) {

      $curl_handles[$index] = curl_init();

      $url = (is_array($request) && !empty($request['url'])) ? $request['url'] : $request;
      curl_setopt($curl_handles[$index], CURLOPT_URL, $url);
      curl_setopt($curl_handles[$index], CURLOPT_HEADER, 0);
      curl_setopt($curl_handles[$index], CURLOPT_RETURNTRANSFER, 1);
      curl_setopt($curl_handles[$index], CURLOPT_FOLLOWLOCATION, 1);
      curl_setopt($curl_handles[$index], CURLOPT_MAXREDIRS, 3);

      // post?
      if (is_array($request)) {
        if (!empty($request['post'])) {
          curl_setopt($curl_handles[$index], CURLOPT_POST, 1);
          curl_setopt($curl_handles[$index], CURLOPT_POSTFIELDS, $request['post']);
        }
      }

      // extra options?
      if (!empty($options)) {
        curl_setopt_array($curl_handles[$index], $options);
      }

      curl_multi_add_handle($multi_handle, $curl_handles[$index]);
    }

    // execute the handles
    $running = NULL;
    do {
      curl_multi_exec($multi_handle, $running);
    } while ($running > 0);

    // get content and remove handles
    foreach ($curl_handles as $index => $c) {
      $result[$index] = curl_multi_getcontent($c);
      curl_multi_remove_handle($multi_handle, $c);
    }

    // all done
    curl_multi_close($multi_handle);

    return $result;
  }

  /**
   * Locally stores metadata (or non-found status) as:
   *    destination node id (integer)
   *    source id string (e.g. 'openlibrary')
   *    serialized metadata array (e.g. 'contents' => 'Chapter 1...', 'edition' => '2nd')
   *    found status (boolean)
   *    date updated (timestamp)
   *
   */
  public function storeSingle($nid, $metadata) {
    $existing = $this->querySingle($nid);
    if ($existing) {
      db_update('metadata_storage')
        ->fields(array(
          'data' => serialize($metadata),
          'found' => ($metadata !== FALSE) ? 1 : 0,
          'updated_timestamp' => REQUEST_TIME,
        ))
        ->condition('source', $this->sourceId)
        ->condition('nid', $nid)
        ->execute();
    }
    else {
      db_insert('metadata_storage')
        ->fields(array(
          'nid' => $nid,
          'source' => $this->sourceId,
          'data' => serialize($metadata),
          'found' => ($metadata !== FALSE) ? 1 : 0,
          'updated_timestamp' => REQUEST_TIME,
        ))
        ->execute();
    }
  }

  /**
   * Fetches a single metadata object from the local store, tacking on a
   * staleness property depending on the update timestamp and staleness
   * constant.
   */
  public function querySingle($nid) {
    $result = db_query(
        "SELECT * FROM {metadata_storage} WHERE nid= :nid AND source= :source",
        array(':nid' => $nid, ':source' => $this->sourceId)
      )->fetchObject();
    if ($result) {
      // Add on staleness information
      $result->stale = $this->isStale($result->updated_timestamp);
      return $result;
    }
    return FALSE;
  }

  /**
   * Returns TRUE/FALSE depending on given timestamp compared to current time
   * exceeds the staleness constant.
   */
  public function isStale($updated_timestamp) {
    // -1 means "never stale"
    if ($this->cacheLifetime == -1) {
      return FALSE;
    }
    // 0 means "always stale"
    elseif ($this->cacheLifetime == 0) {
      return TRUE;
    }
    else {
      // Calculate staleness, return TRUE if stale
      #echo "Calculated staleness: " . (($updated_timestamp < (time() - $this->cacheLifetime)) ? "stale" : "not stale") . "<hr>";
      return ($updated_timestamp < (time() - $this->cacheLifetime));
    }
  }
}
