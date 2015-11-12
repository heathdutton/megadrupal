<?php

/**
 * LastFM Request class.
 *
 * Use lastFmApiRequest::factory(my.methodname) to get a request object. All
 * public methods return $this and can be chained together.
 */
class lastFmApiRequest {

  // API response, populated by lastFmApiRequest->execute()
  public $response;
  
  // Flag to designate if the $response is from a cache.
  public $cache;

  // Default API url
  protected $url;

  // API parameter, use lastFmApiRequst->setFilter() to set.
  protected $params = array();

  // The cache table to use.
  protected $cache_table = 'cache'; 

  /**
   * Factory method!
   *
   * @param $method
   *   The Last.fm API method to build a request for.
   *
   * @return a new lastFmApiRequest object
   */
  static function factory($method, $base_table) {
    switch ($base_table) {
      case 'lastfm_artists':
        return new lastFmApiRequestArtists($method);
      case 'lastfm_events':
        return new lastFmApiRequestEvents($method);
    }
  }

  /**
   * Constructor, please use the factory. k,thx
   */
  function __construct($method) {
    $this->url = variable_get('lastfm_api_url', LASTFM_DEFAULT_API_URL);
    $this->params['method'] = $method;
    $this->params['api_key'] = variable_get('lastfm_api_key', '');
  }

  /**
   * Add a filter to the request. Classes that extend this one should declare
   * what $keys are accepted.
   *
   * @param $key
   *   The filter to set
   * @param $value
   *   The value to set for the $key.
   *
   * @return the lastFmApiRequest object
   */
  public function setFilter($key, $value) {
    $this->params[$key] = $value;
    return $this;
  }

  /**
   * Sign the API request as a particular user.
   *
   * Note: the actually signature isn't generated until
   * lastFmApiRequest::execute() is called. Here we simply add the user's
   * session key to the request.
   * 
   * @param $account
   *   The user to sign the request as.
   *
   * @return the lastFmApiRequest object
   */
  public function sign($account = null) {
    if (!$account) {
      global $user;
      $account = $user;
    }

    if (module_exists('lastfm_user') && isset($user->lastfm_user)) {
      $this->params['sk'] = $user->lastfm_user['key'];
    }
    return $this;
  }

  /**
   * Make the request! The results of the call will be set on the `response`
   * attribute.
   *
   * @return the lastFmApiRequest object
   */
  public function execute() {
    // If this is an authenticated API call add the signature. This isn't done
    // in lasFmApiRequest::sign() so that it doens't need to freeze the params.
    if (isset($this->params['sk'])) {
      lastfm_user_sign($this->params);
    }

    // We cannot include the format in the signature, so we add it last.
    // @see http://www.last.fm/group/Last.fm+Web+Services/forum/21604/_/500629/1#f8513949
    $this->params['format'] = 'json';

    // Put the API call URL together.
    // TODO determine if we really should use url() here...
    $request_url = url($this->url, array('query' => $this->params, 'absolute' => TRUE));

    // Check if we have a cache hit or not.
    if ($result = $this->cache_get($request_url)) {
      $this->response = $result->data;
      $this->cache = TRUE;
    }
    else {
      $this->response = $this->request($request_url);
      $this->cache_set($request_url, $this->response);
      $this->cache = FALSE;
    }
    return $this;
  }

  /**
   * Make the actual HTTP request and parse output
   */
  protected function request($request_url, $l = 1) {
    // Limit to ten levels of recursion.
    if ($l > 10) {
     watchdog('warning', "Recusion limit reached.");
     return array();
    }
    $l++;

    $response = drupal_http_request($request_url);
    if ($response->code == '200') {
      $data = json_decode($response->data);
      if (is_object($data)) {
        if (isset($data->error)) {
          watchdog('error', "LastFM error !code recieved: %message", array('!code' => $data->error, '%mesage' => $data->message));
        }
        else {
          return $data;
          //
          // TODO determine how to handle the pager here. It seems like every
          // request returns a stdClass with a single attribute. This attribute
          // doesn't have a standard name, but varies by method. This single
          // attribute is also a stdClass, it has a @attr attribute that contains
          // the paging info.
          //

          /*
          list($head, $values) = $data;
          // A full reponse may be spread over several pages, so if there are
          // additional pages we retrieve them.
          if (($head->page < $head->pages) && ($head->per_page < $head->total)) {
            $this->params['page'] = $head->page + 1;
            $url = $this->url .'?'. $this->query_string();
            if ($recurse = $this->_request($url, $l)) {
              $values = array_merge($values, $recurse);
            }
          }
          return $values;
          */
        }
      }
      else {
        watchdog('error', "Didn't receive valid API response (invalid JSON).");
      }
    }
    else {
      watchdog('error', 'HTTP error !code received', array('!code' => $response->code));
    }
    return FALSE;
  }

  /**
   * Populate the cache. Wrapper around Drupal's cache_get()
   *
   * TODO determine the correct lifetime / management strategy for the response cache.
   * 
   * The caching model implemented here is somewhat different than drupal's
   * normal model because the data in this cache cannot be regenerated locally.
   *
   * Additionally we wait to avoid making repeated failed requests to the API in
   * the case where it's either down, or a invalid query has been fomulated.
   *
   * @param $url
   *   The API url that would be used.
   * @param $reset
   *   Set to TRUE to force a retrieval from the database.
   */
  protected function cache_get($request_url, $reset = FALSE) {
    static $items = array();

    $cid = $this->cache_id($request_url);
    if (!isset($items[$cid]) || $reset) {
      $items[$cid] = cache_get($cid, $this->cache_table);
      // Don't return temporary items more that 5 minutes old.
      if ($items[$cid]->expire === CACHE_TEMPORARY && $items[$cid]->created > (time() + 300)) {
        return FALSE;
      }
    }
    return $items[$cid];
  }

  /**
   * Retrieve the cache. Wrapper around Drupal's cache_set()
   */
  protected function cache_set($url, $data) {
    if ($data === FALSE) {
      // If we don't get a response we set a temporary cache to prevent hitting
      // the API frequently for no reason.
      cache_set($this->cache_id($url), FALSE, $this->cache_table, CACHE_TEMPORARY);
    }
    else {
      $ttl = (int)variable_get('lastfm_cache_time', LASTFM_DEFAULT_CACHE_TIME);
      $expire = time() + ($ttl * 60);
      cache_set($this->cache_id($url), $data, $this->cache_table, $expire);
    }
  }

  /**
   * Helper function to generate a cache id based on the class name and
   * hash of the url
   */
  protected function cache_id($request_url) {
    return get_class($this) .':'. md5($request_url);
  }
  
  protected function get_related_data($joins, $data) {
    $result = array();
    
    // Joins aka relationships are set as separate requests whose data is pulled into
    // the existing result set. The method to be called is set in the 'method' key,
    // and the arg to be passed to the method is set in the 'arg' key. The assumption
    // is that the arg exists as a property in the result set. 
    //
    // There is also an assumption that related data only returns one record. This makes
    // sense if you think about it. 
    if (isset($joins)) {
      foreach ($joins as $type => $info) {
        $request = lastFmApiRequest::factory($info['method'], $type);
        $request->setFilter($info['arg'], $data->{$info['arg']});
        $request->execute();
        $result = $request->parse(array());
        
        // This is kind of ugly but it is what it is. 
        // NOTE: Can cause big problems in terms of naming collisions.
        foreach ($result as $key => $value) {
          $data->$key = $value;
        }        
      }
    }
  }

  /**
   * Custom parser for the type of data we're retrieving. 
   * Must be implemented by all subclasses. Must return an
   * indexed array of results, each result being an array keyed
   * by the field name. 
   */
  protected function parse($joins) {
  }
}

/**
 * Event-specific request handler
 */
class lastFmApiRequestEvents extends lastFmApiRequest {
  function parse($joins) {
    $data = $this->response->events;
    $results = array();
    
    // When the last.fm api returns one event, it returns $events->event and that
    // object has the event data. When it returns more than one event, $events->event
    // is a keyed array of event data. Because of this, we need to make sure we iterate
    // from the right place or problems occur.
    if (!empty($data->event->id)) {
      // This is a single event
      $events->event = array($events->event);
    }

    // Iterate through the events and set result data as appropriate.
    // Unfortunately a lot of the returned data has to be munged in one
    // form or another. This processor probably needs to be abstracted
    // into the factory and created based on type so we can add more
    // types of data down the road.
    foreach ($data->event as $key => $event) {
      // Venue information is embedded in more arrays and objects, so we need to
      // flatten it out for our fields to be able to find it.
      $event->venue_latitude = $event->venue->location->{'geo:point'}->{'geo:lat'};
      $event->venue_longitude = $event->venue->location->{'geo:point'}->{'geo:long'};
      unset($event->venue->location->{'geo:point'});

      // Venue location information
      foreach ($event->venue->location as $venue_location_key => $value) {
        $event->{'venue_' . $venue_location_key} = $value;
      }
      unset($event->venue->location);

      // Venue images
      foreach ($event->venue->image as $value) {
        $event->{'venue_image_' . $value->size} = $value->{'#text'};
      }
      unset($event->venue->image);

      // Generic venue information
      foreach ($event->venue as $venue_key => $value) {
        $event->{'venue_' . $venue_key} = $value;
      }
      unset($event->venue);

      // Event images
      foreach ($event->image as $value) {
        $event->{'event_image_' . $value->size} = $value->{'#text'};
      }
      unset($event->image);
      
      // Convert startDate field to a Unix timestamp
      $event->startDate = strtotime($event->startDate);

      // Save ticketing information. When there is none, this comes through
      // as a string that can contain just blank spaces. When there is ticketing
      // information, it is an array with the ticket provider and a url. Can
      // this be multiple providers? It seems so but I've never seen it.
      if (is_object($event->tickets)) {
        $event->ticket_supplier = $event->tickets->ticket->supplier;
        $event->ticket_url = $event->tickets->ticket->{'#text'};        
      }
      
      $event->artist = $event->artists->headliner;
      
      if (!empty($joins)) {
        $related_data = $this->get_related_data($joins, $event);
      }

      $results[] = $event;
    }
    
    return $results;
  }
}

/**
 * Artist-specific request handler
 */
class lastFmApiRequestArtists extends lastFmApiRequest {
  // I believe that all calls to artist.GetInfo, which is the only thing 
  // this handler will ever handle, return a single record.
  function parse($joins) {
    $artist = $this->response->artist;

    $artist->bio_published = $artist->bio->published;
    $artist->bio_summary = $artist->bio->summary;
    $artist->bio_content = $artist->bio->content;
    unset($artist->bio);
    
    $artist->stats_listeners = $artist->stats->listeners;
    $artist->stats_playcount = $artist->stats->playcount;
    unset($artist->stats);
    
    return $artist;
  }
}
