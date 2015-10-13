<?php

/**
 * @file
 * OPA reader
 */

/**
 * An iterator over an OPA remote resource.
 *
 * @todo  support pagination of the remote source, currently not possible
 */
class OPAReader implements Iterator, Countable {

  // The current data / page
  protected $data = array();

  // The result set limit (no limit by default)
  protected $limit = NULL;

  // The paging limit, we take the same as the maximum allowed by OPA
  protected $pagination_limit = 50;

  // Current page we're at
  protected $page = 1;

  // Response data to include
  protected $include = null;

  protected $opa;
  protected $params;

  /**
   * Current element object when iterating.
   *
   * @var
   */
  protected $currentElement = NULL;

  /**
   * Value of the ID for the current element when iterating.
   *
   * @var string
   */
  protected $currentId = NULL;


  /**
   * The position in the data array. This is a private ID
   * and depends on the current page.
   * @var integer
   */
  protected $position = 0;


  /**
   * Name of the field within the JSON object holding the ID value.
   * @var string
   */
  protected $idField;

  /**
   * Resource to query from OPA
   * @var string
   */
  protected $resource;

  /**
   * Initialize the members.
   *
   * @param $json_url
   *  URL or filespec of the JSON file to be parsed.
   * @param $id_field
   *  Name of the field within each object containing its unique ID.
   */
  public function __construct($opa, $params, $idField, $resource, $subPath = '') {
    $this->opa = $opa;
    $this->params = $params;
    $this->idField = $idField;
    $this->resource = $resource;
    $this->subPath = $subPath;

    if (isset($params['limit'])) {
      // A limit was specified
      $this->limit = $params['limit'];
    }

    if (isset($params['include'])) {
      // Include linked data
      $this->include = $params['include'];
    }

    // Initalize everything
    $this->reset();
    $this->opa->isAuthorized();
    $this->next();
  }

  protected function fetchData() {
    if (!$this->data) {
      $this->data = $this->opa->apiCall($this->resourcePath(), $this->params);
    }
  }

  protected function resourcePath() {
    $path = $this->resource;
    if ($this->subPath) {
      $path .= $this->subPath;
    }
    return $path;
  }

  protected function reset() {
    $this->currentElement = NULL;
    $this->position = -1;
    $this->currentId = NULL;
    $this->page = 1;
    // Reset also data to have it fresh with correct pagination
    $this->data = array();
  }

  /**
   * Implementation of Iterator::rewind().
   *
   * @return void
   */
  public function rewind() {
    $this->reset();
    $this->opa->isAuthorized();
    $this->next();
  }

  /**
   * Implementation of Iterator::next().
   *
   * Populates currentElement (the object being retrieved) and currentId (that
   * object's unique identifier) from the specified JSON file. Sets both to
   * NULL at end-of-file. Handles properly-formed JSON, as well as some improper
   * coding (specifically that generated in Ning exports).
   *
   * @return void
   */
  public function next() {
    // Use pagination when no user limit was specified or when the specified limit is
    // bigger than the maximum allowed by OPA
    $pagination = is_null($this->limit) || $this->limit > $this->pagination_limit;

    // Open the file and position it if necessary
    if (empty($this->data)) {
      $options = $this->params;
      $options['page'] = $this->page;
      $options['limit'] = $pagination ? $this->pagination_limit : $this->limit;

      // API method callback with pagination and limits
      $this->data = $this->opa->apiCall($this->resourcePath(), $options);

      // @todo add try/catch
      if (!$this->data) {
        return FALSE;
      }
    }

    // Increment Id
    $this->position++;

    // If we're paginating with a user-specified limit, check if the iterator has reached the limit
    //
    // NB the last OPA page will contain the pagination limit, i.e. 50 items by default.
    // If however the user-specified limit is 60 for instance, we only want the remaining 10 items from
    // that last page.
    $last_page_delta = $this->limit - (($this->page - 1) * $this->pagination_limit);
    if (!is_null($this->limit) && $pagination && $this->position + 1 > $last_page_delta) {
      // We are done!
      $this->currentElement = NULL;
    }
    elseif (isset($this->data[$this->resource][$this->position])) {
      $this->currentElement = $this->getData();
      $this->currentId = $this->currentElement['id'];
    }
    else {
      // Check if we need to advance the pagination
      if (isset($this->data['meta'][$this->resource]['pages']) && $pagination && $this->page < $this->data['meta'][$this->resource]['pages']) {
        // Yes, and we have pages available still, so increment page and reset data to fetch the next page
        $this->page++;
        $this->position = -1;
        $this->data = array();
        $this->next();
      }
      else {
        // We are done!
        $this->currentElement = NULL;
      }
    }
  }

  /**
   * Implementation of Iterator::current().
   *
   * @return null|object
   */
  public function current() {
    return $this->currentElement;
  }

  /**
   * Implementation of Iterator::key().
   *
   * @return null|string
   */
  public function key() {
    return $this->currentId;
  }

  /**
   * Implementation of Iterator::valid().
   *
   * @return bool
   */
  public function valid() {
    return !empty($this->currentElement);
  }

  /**
   * Returns the number of items in the remote source.
   * Can be spread over multiple pages.
   *
   * @return [type] [description]
   */
  public function count() {
    $this->fetchData();

    if ($this->data && isset($this->data['meta'][$this->resource]['totalCount'])) {
      return $this->data['meta'][$this->resource]['totalCount'];
    }
    return 0;
  }

  /**
   * Gets data of the specified resource type at the current position while iterating
   * over the result set. Also includes linked data if specified upon constructing
   * the OPAReader object.
   *
   * @return Array The OPA response data.
   */
  protected function getData() {
    $data = $this->data[$this->resource][$this->position];

    if (isset($this->include) && isset($this->data['linked']) && isset($this->data['linked'][$this->include])) {
      // Include linked program data
      if ($this->include == 'programs') {
        $program_id = $data['programId'];
        foreach ($this->data['linked']['programs'] as $program) {
          if ($program['programId'] == $program_id) {
            $data['program'] = $program;
          }
        }
      }
    }

    return $data;
  }

  /**
   * Return a string representing this Reader
   */
  public function __toString() {
    return "OPA reader with parameters: " . serialize($this->params);
  }


}

class OPAVideosReader extends OPAReader {

  /**
   * Initialize the members.
   */
  public function __construct($opa, $params) {
    parent::__construct($opa, $params, 'id', 'videos');
  }

}
