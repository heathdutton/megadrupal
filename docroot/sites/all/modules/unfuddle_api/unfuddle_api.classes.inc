<?php

/**
 * @file Contains Unfuddle API classes for Unfuddle API Drupal module.
 */

/**
 * Primary Unfuddle API implementation class.
 */
class Unfuddle {

  protected $url;
  protected $user;
  protected $pass;
  protected $project;

  protected $format = 'json';

  public function __construct($url = NULL, $user = NULL, $pass = NULL, $project = NULL) {
    $this->url = is_null($url) ? variable_get('unfuddle_api_url', '') : $url;
    $this->user = is_null($user) ? variable_get('unfuddle_api_user', '') : $user;
    $this->pass = is_null($pass) ? variable_get('unfuddle_api_pass', '') : $pass;
    $this->project = is_null($project) ? variable_get('unfuddle_api_project', '') : $project;
  }

  public function setURL($url) {
    $this->url = $url;
  }

  public function setAuth($user, $pass) {
    $this->user = $user;
    $this->pass = $pass;
  }

  public function setProject($projectID) {
    $this->project = $projectID;
  }

  // Projects.
  public function getProjects() {
    try {
      $projects = $this->request($this->url . '/api/v1/projects');
      return $projects;
    }
    catch (Exception $e) {
      $message = $e->getMessage();
      watchdog('unfuddle_api', 'Unable to retrieve Unfuddle projects. Error message @message', array('@message' => $message), WATCHDOG_ERROR);
      return FALSE;
    }
  }

  public function getProject($projectID = NULL) {
    $projectID = is_null($projectID) ? $this->project : $projectID;
    try {
      $project = $this->request($this->url . '/api/v1/projects/' . $projectID);
      return $project;
    }
    catch (Exception $e) {
      $message = $e->getMessage();
      watchdog('unfuddle_api', 'Unable to retrieve Unfuddle project !id. Error message @message', array('!id' => $projectID, '@message' => $message), WATCHDOG_ERROR);
      return FALSE;
    }
  }

  // People.
  public function getPeople($projectID = NULL) {
    $projectID = is_null($projectID) ? $this->project : $projectID;
    try {
      $people = $this->request($this->url . '/api/v1/people');
      return $people;
    }
    catch (Exception $e) {
      $message = $e->getMessage();
      watchdog('unfuddle_api', 'Unable to retrieve Unfuddle people. Error message @message', array('@message' => $message), WATCHDOG_ERROR);
      return FALSE;
    }
  }

  public function getProjectPeople($projectID = NULL) {
    $projectID = is_null($projectID) ? $this->project : $projectID;
    try {
      $people = $this->request($this->url . '/api/v1/projects/' . $projectID . '/people');
      return $people;
    }
    catch (Exception $e) {
      $message = $e->getMessage();
      watchdog('unfuddle_api', 'Unable to retrieve Unfuddle people for project !id. Error message @message', array('!id' => $projectID, '@message' => $message), WATCHDOG_ERROR);
      return FALSE;
    }
  }

  public function getPerson($personID) {
    try {
      $person = $this->request($this->url . '/api/v1/people/' . $personID);
      return $person;
    }
    catch (Exception $e) {
      $message = $e->getMessage();
      watchdog('unfuddle_api', 'Unable to retrieve Unfuddle person !pid. Error message @message', array('!pid' => $personID, '@message' => $message), WATCHDOG_ERROR);
      return FALSE;
    }
  }
  
  //Time Invested.
  public function getTimeInvested($groupBy = NULL, $startDate = NULL, $endDate = NULL) {
    try {
      $filters = array(
        'group_by' => $groupBy,
        'start_date' => $startDate,
        'end_date' => $endDate
      );
      $timeinvested = $this->request($this->url . '/api/v1/account/time_invested', $filters);
      return $timeinvested;
    }
    catch (Exception $e) {
      $message = $e->getMessage();
      watchdog('unfuddle_api', 'Unable to retrieve Unfuddle account time invested.  Error message @message', array('@message' => $message), WATCHDOG_ERROR);
      return FALSE;
    }
  }

  public function getProjectTimeInvested($projectID = NULL, $groupBy = NULL, $startDate = NULL, $endDate = NULL) {
    $projectID = is_null($projectID) ? $this->project : $projectID;
      try {
        $filters = array(
          'group_by' => $groupBy,
          'start_date' => $startDate,
          'end_date' => $endDate
        );
        $timeinvested = $this->request($this->url . '/api/v1/projects/' . $projectID . '/time_invested', $filters);
        return $timeinvested;
      }
      catch (Exception $e) {
        $message = $e->getMessage();
        watchdog('unfuddle_api', 'Unable to retrieve Unfuddle project time invested.  Error message @message', array('@message' => $message), WATCHDOG_ERROR);
        return FALSE;
    }
  }

  public function getTimeByTicket($projectID = NULL, $ticketNum) {
    $projectID = is_null($projectID) ? $this->project : $projectID;
    try {
      $ticket = $this->request($this->url . '/api/v1/projects/' . $projectID . '/tickets/' . $ticketNum . '/time_entries');
      return $ticket;
    }   
    catch (Exception $e) {
      $message = $e->getMessage();
      watchdog('unfuddle_api', 'Unable to retrieve Unfuddle time for ticket number !num for project !id. Error message @message', array('!num' => $ticketNum, '!id' => $projectID, '@message' => $message), WATCHDOG_ERROR);
      return FALSE;
    }   
  }

  // Messages.
  public function getMessages($projectID = NULL) {
    $projectID = is_null($projectID) ? $this->project : $projectID;
    try {
      $messages = $this->request($this->url . '/api/v1/projects/' . $projectID . '/messages');
      return $messages;
    }
    catch (Exception $e) {
      $message = $e->getMessage();
      watchdog('unfuddle_api', 'Unable to retrieve Unfuddle messages. Error message @message', array('@message' => $message), WATCHDOG_ERROR);
      return FALSE;
    }
  }

  public function getMessage($projectID = NULL, $messageID) {
    $projectID = is_null($projectID) ? $this->project : $projectID;
    try {
      $message = $this->request($this->url . '/api/v1/projects/' . $projectID . '/messages/' . $messageID);
      return $message;
    }
    catch (Exception $e) {
      $message = $e->getMessage();
      watchdog('unfuddle_api', 'Unable to retrieve Unfuddle message !mid for project !id. Error message @message', array('!mid' => $messageID, '!id' => $projectID, '@message' => $message), WATCHDOG_ERROR);
      return FALSE;
    }
  }

  public function createMessage($projectID = NULL, $title, $body) {
    $projectID = is_null($projectID) ? $this->project : $projectID;

    $xml = new SimpleXMLElement('<message></message>');
    $xml->addChild('body', $body);
    $xml->addChild('title', $title);

    // POST message.
    try {
      $response = $this->request($this->url . '/api/v1/projects/' . $projectID . '/messages', $xml->asXML(), 'POST');
      watchdog('unfuddle_api', 'Unfuddle message !title created', array('!title' => $title));
      // Extract the message ID from the Location URL.
      return substr($response->headers['Location'], strrpos($response->headers['Location'], '/') + 1);
    }
    catch (Exception $e) {
      $message = $e->getMessage();
      watchdog('unfuddle_api', 'Unfuddle message creation failed with error message @message', array('@message' => $message), WATCHDOG_ERROR);
      return FALSE;
    }
  }


  // Components.
  public function getComponents($projectID = NULL) {
    $projectID = is_null($projectID) ? $this->project : $projectID;
    try {
      $ticket = $this->request($this->url . '/api/v1/projects/' . $projectID . '/components');
      return $ticket;
    }   
    catch (Exception $e) {
      $message = $e->getMessage();
      watchdog('unfuddle_api', 'Unable to retrieve Unfuddle components for project !id. Error message @message', array('!id' => $projectID, '@message' => $message), WATCHDOG_ERROR);
      return FALSE;
    }     
  }

  public function getCategories($projectID = NULL) {
    $projectID = is_null($projectID) ? $this->project : $projectID;
    try {
      $categories = $this->request($this->url . '/api/v1/projects/' . $projectID . '/categories');
      return $categories;
    }
    catch (Exception $e) {
      $message = $e->getMessage();
      watchdog('unfuddle_api', 'Unable to retrieve Unfuddle categories. Error message @message', array('@message' => $message), WATCHDOG_ERROR);
      return FALSE;
    }
  }

  public function getCategory($projectID = NULL, $categoryID) {
    $projectID = is_null($projectID) ? $this->project : $projectID;
    try {
      $category = $this->request($this->url . '/api/v1/projects/' . $projectID . '/categories/' . $categoryID);
      return $category;
    }
    catch (Exception $e) {
      $message = $e->getMessage();
      watchdog('unfuddle_api', 'Unable to retrieve Unfuddle category !cid for project !id. Error message @message', array('!cid' => $categoryID, '!id' => $projectID, '@message' => $message), WATCHDOG_ERROR);
      return FALSE;
    }
  }

  // Milestones.
  public function getMilestones($projectID = NULL) {
    $projectID = is_null($projectID) ? $this->project : $projectID;
    try {
      $milestones = $this->request($this->url . '/api/v1/projects/' . $projectID . '/milestones');
      return $milestones;
    }
    catch (Exception $e) {
      $message = $e->getMessage();
      watchdog('unfuddle_api', 'Unable to retrieve Unfuddle milestones. Error message @message', array('@message' => $message), WATCHDOG_ERROR);
      return FALSE;
    }
  }

  public function getMilestone($projectID = NULL, $milestoneID) {
    $projectID = is_null($projectID) ? $this->project : $projectID;
    try {
      $milestone = $this->request($this->url . '/api/v1/projects/' . $projectID . '/milestones/' . $milestoneID);
      return $milestone;
    }
    catch (Exception $e) {
      $message = $e->getMessage();
      watchdog('unfuddle_api', 'Unable to retrieve Unfuddle milestone !mid for project !id. Error message @message', array('!mid' => $milestoneID, '!id' => $projectID, '@message' => $message), WATCHDOG_ERROR);
      return FALSE;
    }
  }

  // Tickets.
  public function getTickets($projectID = NULL) {
    $projectID = is_null($projectID) ? $this->project : $projectID;
    try {
      $ticket = $this->request($this->url . '/api/v1/projects/' . $projectID . '/tickets');
      return $ticket;
    }
    catch (Exception $e) {
      $message = $e->getMessage();
      watchdog('unfuddle_api', 'Unable to retrieve Unfuddle tickets for project !id. Error message @message', array('!id' => $projectID, '@message' => $message), WATCHDOG_ERROR);
      return FALSE;
    }
  }

  public function getTicket($projectID = NULL, $ticketID) {
    $projectID = is_null($projectID) ? $this->project : $projectID;
    try {
      $ticket = $this->request($this->url . '/api/v1/projects/' . $projectID . '/tickets/' . $ticketID);
      return $ticket;
    }
    catch (Exception $e) {
      $message = $e->getMessage();
      watchdog('unfuddle_api', 'Unable to retrieve Unfuddle ticket !tid for project !id. Error message @message', array('!tid' => $ticketID, '!id' => $projectID, '@message' => $message), WATCHDOG_ERROR);
      return FALSE;
    }
  }

  public function getTicketByNumber($projectID = NULL, $ticketNum) {
    $projectID = is_null($projectID) ? $this->project : $projectID;
    try {
      $ticket = $this->request($this->url . '/api/v1/projects/' . $projectID . '/tickets/by_number/' . $ticketNum);
      return $ticket;
    }
    catch (Exception $e) {
      $message = $e->getMessage();
      watchdog('unfuddle_api', 'Unable to retrieve Unfuddle ticket number !num for project !id. Error message @message', array('!num' => $ticketNum, '!id' => $projectID, '@message' => $message), WATCHDOG_ERROR);
      return FALSE;
    }
  }

  public function createTicket($summary, $description, $fields = array()) {
    // Merge default values.
    $fields += array(
      'priority' => 3,
      'project-id' => $this->project,
      'status' => 'new',
    );

    $xml = new SimpleXMLElement('<ticket></ticket>');
    $xml->addChild('summary', $summary);

    // Unfortunatelly simpleXML doesn't support creation of CDATA
    // nodes by default. In order to create a CDATA node we need to use
    // a dom interface. First we need to create an empty simpleXML
    // desciption node, then get the dom interface for it. After we
    // have the dom node we can use its DOMDocument to create a CDATA
    // node. This we will append to description node.
    $xml->description = NULL;
    $description_node = dom_import_simplexml($xml->description);
    $description_node_owner   = $description_node->ownerDocument;
    $description_node->appendChild($description_node_owner->createCDATASection($description));

    foreach ($fields as $field => $data) {
      $xml->addChild($field, $data);
    }

    // POST message.
    try {
      $response = $this->request($this->url . '/api/v1/projects/' . $fields['project-id'] . '/tickets', $xml->asXML(), 'POST');
      watchdog('unfuddle_api', 'Unfuddle ticket !summary created', array('!summary' => $summary));
      // Extract the ticket ID from the Location URL.
      return substr($response->headers['location'], strrpos($response->headers['location'], '/') + 1);
    }
    catch (Exception $e) {
      $message = $e->getMessage();
      watchdog('unfuddle_api', 'Unfuddle ticket creation failed with error message @message', array('@message' => $message), WATCHDOG_ERROR);
      return FALSE;
    }
  }

  /**
   * Build and perform the request.
   */
  protected function request($url, $data = '', $method = 'GET') {
    $headers = array();
    if ($method == 'GET') {
      if ($data != '') {
        $url .= '?' . http_build_query($data, '', '&');
        $data = '';
      }
      $headers['Content-type'] = 'application/x-www-form-urlencoded';
    }
    else {
      // Leave $data as is, Unfuddle takes XML.
      $headers['Content-type'] = 'application/xml';
    }

    $headers['Authorization'] = 'Basic ' . base64_encode($this->user . ':' . $this->pass);
    switch ($this->format) {
      case 'json':
        $headers['Accept'] = 'application/json';
        break;
      case 'xml':
        $headers['Accept'] = 'application/xml';
        break;
    }
    $options = array(
      'headers' => $headers,
      'method' => $method,
      'data' => $data,
    );

    $response = drupal_http_request($url, $options);
    switch ($response->code) {
      case '200':
        // OK.
        return $this->parseResponse($response->data);
      case '201':
        // Created.
        return $response; // Return the response object. drupal_http_request() does not handle all response codes. See Unfuddle::createTicket() for use.
      case '400':
      default:
        // Bad Request.
        throw new Exception($response->error);
    }
  }

  protected function parseResponse($response) {
    switch ($this->format) {
      case 'json':
        return json_decode($response, TRUE);
      case 'xml':
        return @simplexml_load_string($response);
    }
  }
}
