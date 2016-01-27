<?php

class OEMPROAPI {

  var $version = "1.0";
  var $errorMessage;
  var $error_code;

  /**
   * Cache the information on the API location on the server
   */
  var $apiUrl;

  /**
   * Default to a 300 second timeout on server calls
   */
  var $timeout = 300;

  /**
   * Default to a 8K chunk size
   */
  var $chunkSize = 8192;

//  /**
//   * Cache the user username so we only have to log in once per client instantiation
//   */
//  var $username;
//
//  /**
//   * Cache the user password so we only have to log in once per client instantiation
//   */
//  var $password;

  /**
   * Cache the SessionID so we only have to log in once per client instantiation
   */
  var $SessionID;

  /**
   * Cache the user logged in status so we only have to log in once per client instantiation
   */
  var $userLogged;

  /**
   * Connect to the Oempro API for a given list.
   *
   * @param string $apiurl Your Oempro instance api URL
   * @param string $username Your Oempro instance username
   * @param string $password Your Oempro instance password
   */
  function OEMPROAPI($apiurl, $username, $password) {
    $this->apiUrl = parse_url($apiurl);
    $this->userLogin($username, $password);
  }

  function setTimeout($seconds) {
    if (is_int($seconds)) {
      $this->timeout = $seconds;
      return true;
    }
  }

  function getTimeout() {
    return $this->timeout;
  }

  /**
   * Campaigns
   */

  /**
   * Campaign.Create
   * Create new campaign to send out
   *
   * Request Parameters
   * SessionID = {string} (required)
   *   Session ID which is needed to authenticate the access. It's optional and required for desktop applications only
   * Command = {Campaign.Create} (required)
   *   API Command
   * ResponseFormat = {JSON | XML} (required)
   *   Response format
   * JSONPCallBack = {bool}
   *   Send this parameter for cross-domain. More info about JSONP can be found here: http: //en.wikipedia.org/wiki/JSON#JSONP
   * CampaignName = {string} (required)
   *   Name of the campaign
   *
   * Response Parameters
   * Success = {true | false} - States if there is an error or not
   * ErrorCode = {integer} - If there is an error, error's code
   * CampaignID = {integer} - ID of the new campaign record
   *
   * Error Codes
   * 1 - Campaign name is missing
   * 99998 - Authentication failure or session expired
   * 99999 - Not enough privileges
   */
  function campaignCreate($campaign_name) {
    // Setup POST parameters
    $post_params = array();
    $post_params['SessionID'] = $this->SessionID;
    $post_params['Command'] = "Campaign.Create";
    $post_params['ResponseFormat'] = "JSON";
    $post_params['CampaignName'] = $campaign_name;

    $response = $this->callServer($post_params);
    $serial = json_decode($response[1], TRUE);
//    oempro_debug($arrResponse, $serial);

    if ($response[0] === FALSE) {
      $errors[1] = 'Campaign name is missing';
      $errors[99998] = 'Authentication failure or session expired';
      $errors[99999] = 'Not enough privileges';

      $error_code = $serial['ErrorCode'];
      $this->throwErrors($error_code, $errors);
//      drupal_set_message('Failed to connect to the server. ErrorCode: ' . $error_code);
    }
    else {
      return $serial['CampaignID'];
    }
  }

  /**
   * Campaign.Update
   * Update campaign details
   *
   * Request Parameters
   * SessionID = {string} (required)
   *   Session ID which is needed to authenticate the access. It's optional and required for desktop applications only
   * Command = {Campaign.Update} (required)
   *   API Command
   * ResponseFormat = {JSON | XML} (required)
   *   Response format
   * JSONPCallBack = {bool}
   *   Send this parameter for cross-domain. More info about JSONP can be found here: http: //en.wikipedia.org/wiki/JSON#JSONP
   * CampaignID = {integer} (required)
   *   Campaign ID to update
   * CampaignStatus = {Draft | Ready | Sending | Paused | Pending Approval | Sent | Failed} (required)
   *   Set the cmapaign status
   * CampaignName = {string} (required)
   *   Name of the campaign
   * RelEmailID = {integer} (required)
   *   The email content of campaign (email ID)
   * ScheduleType = {Not Scheduled | Immediate | Future | Recursive} (required)
   *   Type of the schedule
   * SendDate = {YYYY-MM-DD} (required)
   *   Date to send campaign
   * SendTime = {HH: MM: SS} (required)
   *   Time to send campaign
   * SendTimeZone = {string} (required)
   *   Time zone of the schedule date
   * ScheduleRecDaysOfWeek = {0 | 1 | 2 | 3 | 4 | 5 | 6 | 7} (required)
   *   (Recursive scheduling) separate values with comma (,). Enter 0 for every day
   * ScheduleRecDaysOfMonth = {0 | 1 | 2 | 3 | 4 | 5 | ... | 31} (required)
   *   (Recursive scheduling) separate with comma (,). Enter 0 for every day
   * ScheduleRecMonths = {0 | 1 | 2 | ... | 12} (required)
   *   (Recursive scheduling) separate with comma (,). Enter 0 for every month
   * ScheduleRecHours = {0 | 1 | 2 | ... | 23} (required)
   *   (Recursive scheduling) separate with comma (,)
   * ScheduleRecMinutes = {0 | 1 | 2 | ... | 59} (required)
   *   (Recursive scheduling) separate with comma (,)
   * ScheduleRecSendMaxInstance = {integer} (required)
   *   (Recursive scheduling) number of times to repeat campaign sending (enter 0 for unlimited)
   * ApprovalUserExplanation = {string} (required)
   *   User explanation for the campaign if campaign is pending for approval
   * GoogleAnalyticsDomains = {string} (required)
   *   Domains to track with Google Analytics (seperate domains with line break (\n))
   * RecipientListsAndSegments = {string} (required)
   *   Target subscriber lists and segments. Each segment and list is seperated by comma. Format: ListID: SegmentID,ListID: SegmentID Ex: 3: 0,3: 2
   *
   * Response Parameters
   * Success = {true | false} - States if there is an error or not
   * ErrorCode = {integer} - If there is an error, error's code
   *
   * Error Codes
   * 1 - Missing campaign ID
   * 2 - Invalid campaign ID
   * 3 - Invalid campaign status value
   * 4 - Invalid email ID
   * 5 - Invalid campaign schedule type value
   * 6 - Missing send date
   * 7 - Missing send time
   * 8 - Day of month or day of week must be provided for recursive scheduling
   * 9 - Months must be provided for recursive scheduling
   * 10 - Hours must be provided for recursive scheduling
   * 11 - Minutes must be provided for recursive scheduling
   * 12 - Number of times to repeat must be provided for recursive scheduling
   * 13 - Time zone for scheduling is missing
   * 99998 - Authentication failure or session expired
   * 99999 - Not enough privileges
   */
  function campaignUpdate($campaign_id, $campaign_status, $campaign_name, $email_content, $schedule_type, $send_date, $send_time, $send_timezone, $days_of_week, $days_of_month, $months, $hours, $minutes, $no_of_repeats, $approval_explanation, $ga_domains, $lists) {
    // Setup POST parameters
    $post_params = array();
    $post_params['SessionID'] = $this->SessionID;
    $post_params['Command'] = "Campaign.Update";
    $post_params['ResponseFormat'] = "JSON";
    $post_params['CampaignID'] = $campaign_id;
    $post_params['CampaignStatus'] = $campaign_status;
    $post_params['CampaignName'] = $campaign_name;
    $post_params['RelEmailID'] = $email_content;
    $post_params['ScheduleType'] = $schedule_type;
    $post_params['SendDate'] = $send_date;
    $post_params['SendTime'] = $send_time;
    $post_params['SendTimeZone'] = $send_timezone;
    $post_params['ScheduleRecDaysOfWeek'] = $days_of_week;
    $post_params['ScheduleRecDaysOfMonth'] = $days_of_month;
    $post_params['ScheduleRecMonths'] = $months;
    $post_params['ScheduleRecHours'] = $hours;
    $post_params['ScheduleRecMinutes'] = $minutes;
    $post_params['ScheduleRecSendMaxInstance'] = $no_of_repeats;
    $post_params['ApprovalUserExplanation'] = $approval_explanation;
    $post_params['GoogleAnalyticsDomains'] = $ga_domains;
    $post_params['RecipientListsAndSegments'] = $lists;

    $response = $this->callServer($post_params);
    $serial = json_decode($response[1], TRUE);
//    oempro_debug($response, $serial);

    if ($response[0] === FALSE) {
      $errors[1] = 'Missing campaign ID';
      $errors[2] = 'Invalid campaign ID';
      $errors[3] = 'Invalid campaign status value';
      $errors[4] = 'Invalid email ID';
      $errors[5] = 'Invalid campaign schedule type value';
      $errors[6] = 'Missing send date';
      $errors[7] = 'Missing send time';
      $errors[8] = 'Day of month or day of week must be provided for recursive scheduling';
      $errors[9] = 'Months must be provided for recursive scheduling';
      $errors[10] = 'Hours must be provided for recursive scheduling';
      $errors[11] = 'Minutes must be provided for recursive scheduling';
      $errors[12] = 'Number of times to repeat must be provided for recursive scheduling';
      $errors[13] = 'Time zone for scheduling is missing';
      $errors[99998] = 'Authentication failure or session expired';
      $errors[99999] = 'Not enough privileges';

      $error_code = $serial['ErrorCode'];
      $this->throwErrors($error_code, $errors);
//      drupal_set_message('Failed to connect to the server. ErrorCode: ' . $error_code);
    }
    else {
      return $serial['CampaignID'];
    }
  }

//    Campaigns.Get
//    Campaign.Get
//    Campaigns.Delete
//    Campaigns.Archive.GetURL

  /**
   * Custom Fields
   */

  /**
   * CustomFields.Get
   * Retrieves custom fields of given subscriber list.
   *
   * Request Parameters
   * SessionID = {string} (required) - Session ID which is needed to authenticate the access.
   *   It's optional and required for desktop applications only
   * Command = {CustomFields.Get} (required) - API Command
   * ResponseFormat = {JSON | XML} (required) - Response format
   * JSONPCallBack = {bool} - Send this parameter for cross-domain.
   *   More info about JSONP can be found here: http: //en.wikipedia.org/wiki/JSON#JSONP
   * OrderField = {field name of custom field} (required) - Order field
   * OrderType = {ASC | DESC} (required) - Order type
   * SubscriberListID = {integer} (required) - Subscriber list id
   *
   * Response Parameters
   * Success = {true | false} - States if there is an error or not
   * ErrorCode = {integer} - If there is an error, error's code
   * TotalCustomFields = {integer} - Total number of custom fields of subscriber list
   * CustomFields = {mixed} - Returned custom fields
   *
   * Error Codes
   * 1 - Subscriber list id is missing
   * 99998 - Authentication failure or session expired
   * 99999 - Not enough privileges
   */
  function customFieldsGet($subscriber_list_id) {
    // Setup POST parameters
    $post_params = array();
    $post_params['SessionID'] = $this->SessionID;
    $post_params['Command'] = "CustomFields.Get";
    $post_params['ResponseFormat'] = "JSON";
    $post_params['OrderField'] = "CustomFieldID";
    $post_params['OrderType'] = "ASC";
    $post_params['SubscriberListID'] = $subscriber_list_id;

    $response = $this->callServer($post_params);
    $serial = json_decode($response[1], TRUE);
//    oempro_debug(array('OEMPROAPI.class.php' => 'subscriberGet'), array('Line' => '1438'));
//    oempro_debug($response, $serial);

    if ($response[0] === FALSE) {
      $errors[1] = 'Subscriber list id is missing';
      $errors[99998] = 'Authentication failure or session expired';
      $errors[99999] = 'Not enough privileges';

      $error_code = $serial['ErrorCode'];
      $this->throwErrors($error_code, $errors);
//      drupal_set_message('Failed to connect to the server. ErrorCode: ' . $error_code);

      return FALSE;
    }
    else {
//      if ($serial['Success'] == TRUE) {
        return $serial['CustomFields'];
//      }
//      else {
//        drupal_set_message('Failed to retrieve custom fields');
//        return FALSE;
//      }
    }
  }

  /**
   * List Integration
   */
  /**
   * ListIntegration.AddURL
   * Adds a web service url for list subscription or unsubscription events.
   * @link http://docs.octeth.com/docs/oempro/developers/api/commands/listintegration.addurl/
   * 
   * @param
   * Request Parameters
   * SessionID = {string} (required)
   *   Session ID which is needed to authenticate the access. It's optional and required for desktop applications only
   * Command = {ListIntegration.AddURL} (required)
   *   API Command
   * ResponseFormat = {JSON | XML} (required)
   *   Response format
   * JSONPCallBack = {bool}
   *   Send this parameter for cross-domain. More info about JSONP can be found here: http: //en.wikipedia.org/wiki/JSON#JSONP
   * SubscriberListID = {integer} (required)
   *   ID of subscriber list
   * Event = {subscription | unsubscription} (required)
   *   Event of the integration
   * URL = {string} (required)
   *   URL of the web service
   * 
   * @return
   * Response Parameters
   * Success = {true | false} - States if there is an error or not
   * ErrorCode = {integer} - If there is an error, error's code
   * WebServiceIntegrationID = {integer} - ID of new web service integration url
   * 
   * Error Codes
   * 1 - Subscriber list id is missing
   * 2 - URL is missing
   * 3 - Event type is missing
   * 4 - Invalid subscriber list id
   * 99998 - Authentication failure or session expired
   * 99999 - Not enough privileges 
   */
  function listIntegrationAddURL($list_id, $event, $url) {
    // Setup POST parameters
    $post_params = array();
    $post_params['SessionID'] = $this->SessionID;
    $post_params['Command'] = "ListIntegration.AddURL";
    $post_params['ResponseFormat'] = "JSON";
    $post_params['SubscriberListID'] = $list_id;
    $post_params['Event'] = $event;
    $post_params['URL'] = $url;

    $response = $this->callServer($post_params);
    $serial = json_decode($response[1], TRUE);
//    oempro_debug(array('OEMPROAPI.class.php' => 'listIntegrationAddURL'), array('Line' => '369'));
//    oempro_debug($response, $serial);

    if ($response[0] === FALSE) {
      $errors[1] = 'Web service integration url ids are missing';
      $errors[99998] = 'Authentication failure or session expired';
      $errors[99999] = 'Not enough privileges';

      $error_code = $serial['ErrorCode'];
      $this->throwErrors($error_code, $errors);
//      drupal_set_message('Failed to connect to the server. ErrorCode: ' . $error_code);

      return FALSE;
    }
    else {
//      if ($serial['Success'] == TRUE) {
        return $serial;
//      }
//      else {
//        drupal_set_message('Failed to retrieve list integration id');
//        return FALSE;
//      }
    }
  }

//    ListIntegration.GetURLs
//    ListIntegration.TestURL
//    ListIntegration.DeleteURLs

  /**
   * ListIntegration.DeleteURLs
   * Deletes given web service urls.
   * @link http://docs.octeth.com/docs/oempro/developers/api/commands/listintegration.deleteurls/
   * 
   * @param
   * Request Parameters
   * SessionID = {string} (required)
   *   Session ID which is needed to authenticate the access. It's optional and required for desktop applications only
   * Command = {ListIntegration.DeleteURLs} (required)
   *   API Command
   * ResponseFormat = {JSON | XML} (required)
   *   Response format
   * JSONPCallBack = {bool}
   *   Send this parameter for cross-domain. More info about JSONP can be found here: http: //en.wikipedia.org/wiki/JSON#JSONP
   * URLs = {integer} (required)
   *   Comma delimeted url ids.
   * 
   * Response Parameters
   * Success = {true | false} - States if there is an error or not
   * ErrorCode = {integer} - If there is an error, error's code
   * 
   * Error Codes
   * 1 - Web service integration url ids are missing
   * 99998 - Authentication failure or session expired
   * 99999 - Not enough privileges
   */
  function listIntegrationDeleteURLs($urls) {
    // Setup POST parameters
    $post_params = array();
    $post_params['SessionID'] = $this->SessionID;
    $post_params['Command'] = "ListIntegration.DeleteURLs";
    $post_params['ResponseFormat'] = "JSON";
    $post_params['URLs'] = $urls;

    $response = $this->callServer($post_params);
    $serial = json_decode($response[1], TRUE);
//    oempro_debug(array('OEMPROAPI.class.php' => 'listIntegrationDeleteURLs'), array('Line' => '435'));
//    oempro_debug($response, $serial);

    if ($response[0] === FALSE) {
      $errors[1] = 'Web service integration url ids are missing';
      $errors[99998] = 'Authentication failure or session expired';
      $errors[99999] = 'Not enough privileges';

      $error_code = $serial['ErrorCode'];
      $this->throwErrors($error_code, $errors);
//      drupal_set_message('Failed to connect to the server. ErrorCode: ' . $error_code);

      return FALSE;
    }
    else {
//      if ($serial['Success'] == TRUE) {
        return $serial;
//      }
//      else {
//        drupal_set_message('Failed to retrieve list integration success response');
//        return FALSE;
//      }
    }
  }

//    ListIntegration.GenerateSubscriptionFormHTMLCode
//    ListIntegration.GenerateUnsubscriptionFormHTMLCode

  /**
   * Lists
   */

  /**
   * Lists.Get
   * Retrieves subscriber lists of logged in user.
   * @link http://docs.octeth.com/docs/oempro/developers/api/commands/lists.get/
   *
   * Request Parameters
   * SessionID = {string} (required) - Session ID which is needed to authenticate the access.
   *   It's optional and required for desktop applications only
   * Command = {Lists.Get} (required) - API Command
   * ResponseFormat = {JSON | XML} (required) - Response format
   * JSONPCallBack = {bool} - Send this parameter for cross-domain.
   *   @see More info about JSONP can be found here: http: //en.wikipedia.org/wiki/JSON#JSONP
   * OrderField = {field name of subscriber list} (required) - Order field
   * OrderType = {ASC | DESC} (required) - Order type
   *
   * Response Parameters
   * Success = {true | false} - States if there is an error or not
   * ErrorCode = {integer} - If there is an error, error's code
   * TotalListCount = {integer} - Total number of subscriber lists
   * Lists = {mixed} - Returned lists
   *
   * Error Codes
   * 99998 - Authentication failure or session expired
   * 99999 - Not enough privileges
   */
  function listsGet() {
    // Setup POST parameters
    $post_params = array();
    $post_params['SessionID'] = $this->SessionID;
    $post_params['Command'] = "Lists.Get";
    $post_params['ResponseFormat'] = "JSON";
    $post_params['OrderField'] = "ListID";
    $post_params['OrderType'] = "ASC";

    $response = $this->callServer($post_params);
    $serial = json_decode($response[1], TRUE);
//    oempro_debug(array('OEMPROAPI.class.php' => 'subscriberGet'), array('Line' => '503'));
//    oempro_debug($response, $serial);

    if ($response[0] === FALSE) {
      $errors[99998] = 'Authentication failure or session expired';
      $errors[99999] = 'Not enough privileges';

      $error_code = $serial['ErrorCode'];
      $this->throwErrors($error_code, $errors);
//      drupal_set_message('Failed to connect to the server. ErrorCode: ' . $error_code);

      return FALSE;
    }
    else {
//      if ($serial['Success'] == TRUE) {
        return $serial;
//      }
//      else {
//        drupal_set_message('Failed to retrieve subscribers lists');
//        return FALSE;
//      }
    }
  }

  /**
   * List.Get
   * Retrieves subscriber list.
   * @link http://docs.octeth.com/docs/oempro/developers/api/commands/list.get/
   *
   * Request Parameters
   * SessionID = {string} (required)
   *   Session ID which is needed to authenticate the access. It's optional and required for desktop applications only
   * Command = {List.Get} (required)
   *   API Command
   * ResponseFormat = {JSON | XML} (required)
   *   Response format
   * JSONPCallBack = {bool}
   *   Send this parameter for cross-domain. More info about JSONP can be found here: http: //en.wikipedia.org/wiki/JSON#JSONP
   * ListID = {integer} (required)
   *   ID of subscriber list to retrieve
   *
   * Response Parameters
   * Success = {true | false} - States if there is an error or not
   * ErrorCode = {integer} - If there is an error, error's code
   * List = {mixed} - Returned list information
   *
   * Error Codes
   * 1 - Subscriber list id is missing
   * 99998 - Authentication failure or session expired
   * 99999 - Not enough privileges
   */
  function listGet($listID) {
    // Setup POST parameters
    $post_params = array();
    $post_params['SessionID'] = $this->SessionID;
    $post_params['Command'] = "List.Get";
    $post_params['ResponseFormat'] = "JSON";
    $post_params['ListID'] = $listID;

    $response = $this->callServer($post_params);
    $serial = json_decode($response[1], TRUE);
//    oempro_debug(array('OEMPROAPI.class.php' => 'subscriberGet'), array('Line' => '564'));
//    oempro_debug($response, $serial);

    if ($response[0] === FALSE) {
      $errors[1] = 'Subscriber list id is missing';
      $errors[99998] = 'Authentication failure or session expired';
      $errors[99999] = 'Not enough privileges';

      $error_code = $serial['ErrorCode'];
      $this->throwErrors($error_code, $errors);
//      drupal_set_message('Failed to connect to the server. ErrorCode: ' . $error_code);

      return FALSE;
    }
    else {
//      if ($serial['Success'] == TRUE) {
        return $serial;
//      }
//      else {
//        drupal_set_message('Failed to retrieve list information');
//        return FALSE;
//      }
    }
  }

  /**
   * Subscribers
   */
  /**
   * Subscriber.Login
   * Logs in the subscriber
   * @link http://docs.octeth.com/docs/oempro/developers/api/commands/subscriber.login/
   * 
   * Request Parameters
   * Command = {Subscriber.Login} (required)
   *   API Command
   * ResponseFormat = {JSON | XML} (required)
   *   Response format
   * JSONPCallBack = {bool}
   *   Send this parameter for cross-domain. More info about JSONP can be found here: http: //en.wikipedia.org/wiki/JSON#JSONP
   * ListID = {integer} (required)
   *   List ID that stores the subscriber information
   * MSubscriberID = {integer} (required)
   *   ID of the subscriber (md5)
   * MEmailAddress = {string} (required)
   *   Email address of the subscriber (md5)
   * EmailAddress = {string} (required)
   *   Email address to validate
   * 
   * Response Parameters
   * Success = {true | false} - States if there is an error or not
   * ErrorCode = {integer} - If there is an error, error's code
   * 
   * Error Codes
   * 1 - Missing list ID
   * 2 - Missing subscriber ID
   * 3 - Missing subscriber email address
   * 4 - Missing validation email address
   * 5 - Invalid list ID
   * 6 - Invalid user
   * 7 - Invalid validation email address
   * 8 - Invalid login information
   * 99998 - Authentication failure or session expired
   * 99999 - Not enough privileges  
   */
  function subscriberLogin($list_id, $email_address, $validation_address) {
    // Setup POST parameters
    $post_params = array();
    $post_params['Command'] = "Subscriber.Login";
    $post_params['ResponseFormat'] = "JSON";
    $post_params['ListID'] = $list_id;
    $post_params['MSubscriberID'] = $subscriber_id;
    $post_params['MEmailAddress'] = $email_address;
    $post_params['EmailAddress'] = $validation_address;

    $response = $this->callServer($post_params);
    $serial = json_decode($response[1], TRUE);
//    oempro_debug(array('OEMPROAPI.class.php' => 'subscriberLogin'), array('Line' => '641'));
//    oempro_debug($response, $serial);

    if ($response[0] === FALSE) {
      $errors[1] = 'Missing list ID';
      $errors[2] = 'Missing subscriber ID';
      $errors[3] = 'Missing subscriber email address';
      $errors[4] = 'Missing validation email address';
      $errors[5] = 'Invalid list ID';
      $errors[6] = 'Invalid user';
      $errors[7] = 'Invalid validation email address';
      $errors[8] = 'Invalid login information';
      $errors[99998] = 'Authentication failure or session expired';
      $errors[99999] = 'Not enough privileges';

      $error_code = $serial['ErrorCode'];
      $this->throwErrors($error_code, $errors);
//      drupal_set_message('Failed to connect to the server. ErrorCode: ' . $error_code);

      return FALSE;
    }
    else {
      return $serial;
    }
  }

  /**
   * Subscriber.GetLists
   * Returns subscriber lists of the user
   * '''Important:''' This command requires subscriber login session
   *    which can be retrieved by running [[Subscriber.Login]] API call.
   * @link http://docs.octeth.com/docs/oempro/developers/api/commands/subscriber.getlists/
   *
   * Request Parameters
   * SessionID = {string} (required)
   *   Session ID which is needed to authenticate the access. It's optional and required for desktop applications only
   * Command = {Subscriber.GetLists} (required)
   *   API Command
   * ResponseFormat = {JSON | XML} (required)
   *   Response format
   * JSONPCallBack = {bool}
   *   Send this parameter for cross-domain. More info about JSONP can be found here: http: //en.wikipedia.org/wiki/JSON#JSONP
   *
   * Response Parameters
   * Success = {true | false} - States if there is an error or not
   * ErrorCode = {integer} - If there is an error, error's code
   * SubscribedLists = {array} - Returns the list of subscriber lists and subscriptions in array
   *
   * Error Codes
   * 1 - Invalid authentication #1
   * 2 - Invalid authentication #2
   * 3 - Invalid subscriber information
   * 99998 - Authentication failure or session expired
   * 99999 - Not enough privileges
   */
  function subscriberGetLists($session_id) {
    // Setup POST parameters
    $post_params = array();
    $post_params['SessionID'] = $session_id;
    $post_params['Command'] = "Subscriber.GetLists";
    $post_params['ResponseFormat'] = "JSON";

    $response = $this->callServer($post_params);
    $serial = json_decode($response[1], TRUE);
//    oempro_debug(array('OEMPROAPI.class.php' => 'subscriberGetLists'), array('Line' => '705'));
//    oempro_debug($response, $serial);

    if ($response[0] === FALSE) {
      $errors[1] = 'Invalid authentication #1';
      $errors[2] = 'Invalid authentication #2';
      $errors[3] = 'Invalid subscriber information';
      $errors[99998] = 'Authentication failure or session expired';
      $errors[99999] = 'Not enough privileges';

      $error_code = $serial['ErrorCode'];
      $this->throwErrors($error_code, $errors);
//      drupal_set_message('Failed to connect to the server. ErrorCode: ' . $error_code);

      return FALSE;
    }
    else {
      return $serial;
    }
  }
//  Subscriber.Get

  /**
   * Subscriber.Get
   * Retrieve subscriber information
   * @link http://docs.octeth.com/docs/oempro/developers/api/commands/subscriber.get/
   * 
   * Request Parameters
   * SessionID = {string} (required)
   *   Session ID which is needed to authenticate the access. It's optional and required for desktop applications only
   * Command = {Subscribers.Get} (required)
   *   API Command
   * ResponseFormat = {JSON | XML} (required)
   *   Response format
   * JSONPCallBack = {bool}
   *   Send this parameter for cross-domain. More info about JSONP can be found here: http: //en.wikipedia.org/wiki/JSON#JSONP
   * EmailAddress = {string} (required)
   *   Email address of the target subscriber
   * ListID = {integer} (required)
   *   ID of the list which email address is subscribed to
   * 
   * Response Parameters
   * Success = {true | false} - States if there is an error or not
   * ErrorCode = {integer} - If there is an error, error's code
   * ErrorText = {text}
   * SubscriberInformation = {array} - Returns the subscriber information
   * 
   * Error Codes
   * 1 - Missing email address
   * 2 - Missing subscriber list ID
   * 3 - Subscriber doesn't exist
   * 99998 - Authentication failure or session expired
   * 99999 - Not enough privileges
   */
  function subscriberGet($list_id, $email_address) {
    // Setup POST parameters
    $post_params = array();
    $post_params['SessionID'] = $this->SessionID;
    $post_params['Command'] = "Subscriber.Get";
    $post_params['ResponseFormat'] = "JSON";
    $post_params['EmailAddress'] = $email_address;
    $post_params['ListID'] = $list_id;

    $response = $this->callServer($post_params);
    $serial = json_decode($response[1], TRUE);
//    oempro_debug(array('OEMPROAPI.class.php' => 'subscriberGet'), array('Line' => '770'));
//    oempro_debug($response, $serial);

    if ($response[0] === FALSE) {
      $errors[1] = 'Missing email address';
      $errors[2] = 'Missing subscriber list ID';
      $errors[3] = 'Subscriber doesn\'t exist';
      $errors[99998] = 'Authentication failure or session expired';
      $errors[99999] = 'Not enough privileges';

      $error_code = $serial['ErrorCode'];
      $this->throwErrors($error_code, $errors);
//      drupal_set_message('Failed to connect to the server. ErrorCode: ' . $error_code);
      return FALSE;
    }
    else {
//      if ($serial['Success'] == TRUE) {
        return $serial;
//      }
//      else {
//        drupal_set_message('Failed to retrieve subscriber details');
//        return FALSE;
//      }
    }
  }

  /**
   * Subscribers.Get
   * Retrieves subscribers of a subscriber list.
   * @link http://docs.octeth.com/docs/oempro/developers/api/commands/subscribers.get/
   * 
   * Request Parameters
   * SessionID = {string} (required)
   *   Session ID which is needed to authenticate the access. It's optional and required for desktop applications only
   * Command = {Subscribers.Get} (required)
   *   API Command
   * ResponseFormat = {JSON | XML} (required)
   *   Response format
   * JSONPCallBack = {bool}
   *   Send this parameter for cross-domain. More info about JSONP can be found here: http: //en.wikipedia.org/wiki/JSON#JSONP
   * OrderField = {string} (required)
   *   Name of the field to order based on
   * OrderType = {ASC | DESC} (required)
   *   Ascending or descending ordering
   * RecordsFrom = {integer} (required)
   *   Start from (starts from zero)
   * RecordsPerRequest = {integer} (required)
   *   How many rows to return per page
   * SearchField = {string} (required)
   *   Subscriber list field to make the search. Leave empty to disable search
   * SearchKeyword = {string} (required)
   *   The keyword to search in subscriber list database. Leave empty to disable search
   * SubscriberListID = {integer} (required)
   *   List ID of the subscribers
   * SubscriberSegment = {mixed} (required)
   *   Target segment ID or one of the following values: Suppressed, Unsubscribed, Soft bounced, Hard bounced, Opt-in pending, Active.
   * 
   * Response Parameters
   * Success = {true | false} - States if there is an error or not
   * ErrorCode = {integer} - If there is an error, error's code
   * ErrorText = {text}
   * Subscribers = {array} - Returns the list of all subscribers in array
   * TotalSubscribers = {integer} - Returns the total number of subscribers
   * 
   * Error Codes
   * 1 - Missing subscriber list ID
   * 2 - Target segment ID is missing
   * 99998 - Authentication failure or session expired
   * 99999 - Not enough privileges 
   */
  function subscribersGet($subscriber_list_id, $session_id, $order_field = 'EmailAddress', $order_type = 'ASC', $records_from = 0, $records_per_request = 100, $search_field = '', $search_keyword = '', $subscriber_segment = 'Active') {
    // Setup POST parameters
    $post_params = array();
    $post_params['SessionID'] = $this->SessionID;
    $post_params['Command'] = "Subscribers.Get";
    $post_params['ResponseFormat'] = "JSON";
    $post_params['OrderField'] = $order_field;
    $post_params['OrderType'] = $order_type;
    $post_params['RecordsFrom'] = $records_from;
    $post_params['RecordsPerRequest'] = $records_per_request;
    $post_params['SearchField'] = $search_field;
    $post_params['SearchKeyword'] = $search_keyword;
    $post_params['SubscriberListID'] = $subscriber_list_id;
    $post_params['SubscriberSegment'] = $subscriber_segment;

    $response = $this->callServer($post_params);
    $serial = json_decode($response[1], TRUE);
//    oempro_debug(array('OEMPROAPI.class.php' => 'subscriberSubscribe'), array('Line' => '857'));
//    oempro_debug($response, $serial);

    if ($response[0] === FALSE) {
      $errors[1] = 'Missing subscriber list ID';
      $errors[2] = 'Target segment ID is missing';
      $errors[99998] = 'Authentication failure or session expired';
      $errors[99999] = 'Not enough privileges';

      $error_code = $serial['ErrorCode'];
      $this->throwErrors($error_code, $errors);
//      drupal_set_message('Failed to connect to the server. ErrorCode: ' . $error_code);
      return FALSE;
    }
    else {
      return $serial;
    }
  }

//  Subscribers.Import
  /**
   * Subscriber.Subscribe
   * Subscribes an email address to provided subscriber list or lists
   * @link http://docs.octeth.com/docs/oempro/developers/api/commands/subscriber.subscribe/
   * 
   * Request Parameters
   * SessionID = {string} (optional)
   *   Session ID which is needed to authenticate the access. It's optional and required for desktop applications only
   * Command = {Subscriber.Subscribe} (required)
   *   API Command
   * ResponseFormat = {JSON | XML} (required)
   *   Response format
   * JSONPCallBack = {bool}
   *   Send this parameter for cross-domain. More info about JSONP can be found here: http: //en.wikipedia.org/wiki/JSON#JSONP
   * ListID = {integer} (required)
   *   Target List ID
   * EmailAddress = {string} (required)
   *   Email address to subscribe
   * CustomFieldX = {array} (required)
   *   Additional information about the subscriber. Replace X with the ID number of the custom field.
   * IPAddress = {string} (required)
   *   IP address of subscriber
   * 
   * Response Parameters
   * Success = {true | false} - States if there is an error or not
   * SubscriberID = {integer} - Subscriber ID of the email address will be returned if the result is success
   * RedirectURL = {string} - Target URL to redirect user after the process
   * ErrorCode = {integer} - If there is an error, error's code
   * ErrorCustomFieldID = {integer} - If there's an error with one of the provided custom fields, custom field ID number will be provided
   * ErrorCustomFieldTitle = {integer} - If there's an error with one of the provided custom fields, custom field title will be provided
   * ErrorCustomFieldDescription = {integer} - If there's an error with one of the provided custom fields, description of the error message is provided 
   * 
   * Error Codes
   * 1 - Target subscriber list ID is missing
   * 2 - Email address is missing
   * 3 - IP address of subscriber is missing
   * 4 - Invalid subscriber list ID
   * 5 - Invalid email address
   * 6 - One of the provided custom fields is empty. Custom field ID and title is provided as an additional output parameter
   * 7 - One of the provided custom field value already exists in the database. Please enter a different value. Custom field ID and title is provided as an additional output parameter
   * 8 - One of the provided custom field value failed in validation checking. Custom field ID and title is provided as an additional output parameter
   * 9 - Email address already exists in the list
   * 10 - Unknown error occurred
   * 11 - Invalid user information
   * 99998 - Authentication failure or session expired
   * 99999 - Not enough privileges 
   */
  function subscriberSubscribe($list_id, $email_address, $custom_fields = array(), $ip_address = '127.0.0.1') {
    // Setup POST parameters
    $post_params = array();
    $post_params['Command'] = "Subscriber.Subscribe";
    $post_params['ResponseFormat'] = "JSON";
    $post_params['SessionID'] = $this->SessionID;
    $post_params['ListID'] = $list_id;
    $post_params['EmailAddress'] = $email_address;
    foreach ($custom_fields as $key => $value) {
      $post_params['CustomField' . $key] = $value;
    }
    $post_params['IPAddress'] = $ip_address;

    $response = $this->callServer($post_params);
    $serial = json_decode($response[1], TRUE);
//    oempro_debug(array('OEMPROAPI.class.php' => 'subscriberSubscribe'), array('Line' => '939'));
//    oempro_debug($response, $serial);

    if ($response[0] === FALSE) {
      $errors[1] = 'Target subscriber list ID is missing';
      $errors[2] = 'Email address is missing';
      $errors[3] = 'IP address of subscriber is missing';
      $errors[4] = 'Invalid subscriber list ID';
      $errors[5] = 'Invalid email address';
      $errors[6] = 'One of the provided custom fields is empty. Custom field ID and title is provided as an additional output parameter';
      $errors[7] = 'One of the provided custom field value already exists in the database. Please enter a different value. Custom field ID and title is provided as an additional output parameter';
      $errors[8] = 'One of the provided custom field value failed in validation checking. Custom field ID and title is provided as an additional output parameter';
      $errors[9] = 'Email address already exists in the list';
      $errors[10] = 'Unknown error occurred';
      $errors[11] = 'Invalid user information';
      $errors[99998] = 'Authentication failure or session expired';
      $errors[99999] = 'Not enough privileges';

      $error_code = $serial['ErrorCode'];
      $this->throwErrors($error_code, $errors);
//      drupal_set_message('Failed to connect to the server. ErrorCode: ' . $error_code);
      return FALSE;
    }
    else {
//      if ($serial['Success'] == TRUE) {
//        return $serial['SubscriberInformation'];
        return $serial;
//      }
//      else {
//        drupal_set_message('Failed to retrieve subscriber details');
//        return FALSE;
//      }
    }
  }

//  Subscriber.OptIn
  /**
   * Subscriber.Unsubscribe
   * Unsubscribe the subscriber from the list
   * @link http://docs.octeth.com/docs/oempro/developers/api/commands/subscriber.unsubscribe/
   * 
   * Request Parameters
   * SessionID = {string} (optional)
   *   Session ID which is needed to authenticate the access. It's optional and required for desktop applications only
   * Command = {Subscriber.Unsubscribe} (required)
   *   API Command
   * ResponseFormat = {JSON | XML} (required)
   *   Response format
   * JSONPCallBack = {bool}
   *   Send this parameter for cross-domain. More info about JSONP can be found here: http: //en.wikipedia.org/wiki/JSON#JSONP
   * ListID = {integer} (required)
   *   Subscriber List ID
   * CampaignID = {integer} (required)
   *   If link is generated for an email campaign, campaign ID should be provided for statistics
   * EmailID = {integer}
   *   If email ID is provided, the unsubscription statistics will be registered to that email and owner A/B split testing campaign
   * SubscriberID = {integer} (required)
   *   Subscriber ID or email address must be provided
   * EmailAddress = {string} (required)
   *   Subscriber ID or email address must be provided
   * IPAddress = {string} (required)
   *   IP address of the user who has requested to unsubscribe
   * Preview = {1 | 0} (required)
   *   If set to 1, unsubscription process will be simulated
   * 
   * Response Parameters
   * Success = {true | false} - States if there is an error or not
   * RedirectURL = {string} - Target URL to redirect user after the process
   * ErrorCode = {integer} - If there is an error, error's code
   * 
   * Error Codes
   * 1 - Subscriber list ID is missing
   * 2 - IP address is missing
   * 3 - Email address or subscriber ID must be provided
   * 4 - Invalid subscriber list ID
   * 5 - Invalid user information
   * 6 - Invalid email address format
   * 7 - Invalid subscriber ID or email address. Subscriber information not found in the database
   * 8 - Invalid campaign ID
   * 9 - Email address already unsubscribed
   * 10 - Invalid email ID
   * 99998 - Authentication failure or session expired
   * 99999 - Not enough privileges  
   */
  function subscriberUnsubscribe($list_id, $email_address, $campaign_id = NULL, $email_id = NULL, $subscriber_id = NULL, $ip_address = '127.0.0.1', $preview = FALSE) {
    // Setup POST parameters
    $post_params = array();
    $post_params['Command'] = "Subscriber.Unsubscribe";
    $post_params['ResponseFormat'] = "JSON";
    $post_params['SessionID'] = $this->SessionID;
    $post_params['ListID'] = $list_id;
    $post_params['CampaignID'] = $campaign_id;
    $post_params['EmailID'] = $email_id;
    $post_params['SubscriberID'] = $subscriber_id;
    $post_params['EmailAddress'] = $email_address;
    $post_params['IPAddress'] = $ip_address;
    $post_params['Preview'] = $preview;

    $response = $this->callServer($post_params);
    $serial = json_decode($response[1], TRUE);
//    oempro_debug(array('OEMPROAPI.class.php' => 'subscriberUnsubscribe'), array('Line' => '1039'));
//    oempro_debug($response, $serial);

    if ($response[0] === FALSE) {
      $errors[1] = 'Subscriber list id is missing';
      $errors[2] = 'IP address is missing';
      $errors[3] = 'Email address or subscriber ID must be provided';
      $errors[4] = 'Invalid subscriber list ID';
      $errors[5] = 'Invalid user information';
      $errors[6] = 'Invalid email address format';
      $errors[7] = 'Invalid subscriber ID or email address. Subscriber information not found in the database';
      $errors[8] = 'Invalid campaign ID';
      $errors[9] = 'Email address already unsubscribed';
      $errors[10] = 'Invalid email ID';
      $errors[99998] = 'Authentication failure or session expired';
      $errors[99999] = 'Not enough privileges';

      $error_code = $serial['ErrorCode'];
      $this->throwErrors($error_code, $errors);
//      drupal_set_message('Failed to connect to the server. ErrorCode: ' . $error_code);
      return FALSE;
    }
    else {
//      if ($serial['Success'] == TRUE) {
        return $serial;
//      }
//      else {
//        drupal_set_message('Failed to retrieve delete success response');
//        return FALSE;
//      }
    }
  }

  /**
   * Subscriber.Update
   * Update subscriber information in the target list
   * @link http://docs.octeth.com/docs/oempro/developers/api/commands/subscriber.update/
   * 
   * Request Parameters
   * SessionID = {string} (required)
   *   Session ID which is needed to authenticate the access. It's optional and required for desktop applications only
   * Command = {Subscriber.Update} (required)
   *   API Command
   * ResponseFormat = {JSON | XML} (required)
   *   Response format
   * JSONPCallBack = {bool}
   *   Send this parameter for cross-domain. More info about JSONP can be found here: http: //en.wikipedia.org/wiki/JSON#JSONP
   * SubscriberID = {integer} (required)
   *   Target subscriber ID
   * SubscriberListID = {integer} (required)
   *   Owner subscription list ID
   * EmailAddress = {string} (required)
   *   Email address
   * Fields = {array} (required)
   *   Custom field IDs with prefix of 'CustomField'. Ex: Fields[CustomField28]
   * Access = {subscriber | admin} (required)
   *   User (or subscriber) authentication
   * 
   * Response Parameters
   * Success = {true | false} - States if there is an error or not
   * ErrorCode = {integer} - If there is an error, error's code
   * ErrorCustomFieldID = {integer} - If there's an error with one of the provided custom fields, custom field ID number will be provided
   * ErrorCustomFieldTitle = {integer} - If there's an error with one of the provided custom fields, custom field title will be provided
   * ErrorCustomFieldDescription = {integer} - If there's an error with one of the provided custom fields, description of the error message is provided
   * 
   * Error Codes
   * 1 - Missing subscriber ID
   * 2 - Missing subscriber list ID
   * 3 - Missing email address
   * 4 - Invalid email address
   * 5 - Invalid subscriber list ID
   * 6 - Invalid subscriber ID
   * 7 - Subscriber already exists
   * 8 - One of the provided custom fields is empty. Custom field ID and title is provided as an additional output parameter
   * 9 - One of the provided custom field value already exists in the database. Please enter a different value. Custom field ID and title is provided as an additional output parameter
   * 10 - One of the provided custom field value failed in validation checking. Custom field ID and title is provided as an additional output parameter
   * 99998 - Authentication failure or session expired
   * 99999 - Not enough privileges  
   */
  function subscriberUpdate($subscriber_id, $subscriber_list_id, $email_address, $custom_fields) {
    // Setup POST parameters
    $post_params = array();
    $post_params['SessionID'] = $this->SessionID;
    $post_params['Command'] = "Subscriber.Update";
    $post_params['ResponseFormat'] = "JSON";
    $post_params['SubscriberID'] = $subscriber_id;
    $post_params['SubscriberListID'] = $subscriber_list_id;
    $post_params['EmailAddress'] = $email_address;
    foreach ($custom_fields as $key => $value) {
      $post_params['Fields[CustomField' . $key . ']'] = $value;
    }
    $post_params['Access'] = "admin";

    $response = $this->callServer($post_params);
    $serial = json_decode($response[1], TRUE);
//    oempro_debug(array('OEMPROAPI.class.php' => 'subscriberUpdate'), array('Line' => '1134'));
//    oempro_debug($response, $serial);

    if ($response[0] === FALSE) {
      $errors[1] = 'Missing subscriber ID';
      $errors[2] = 'Missing subscriber list ID';
      $errors[3] = 'Missing email address';
      $errors[4] = 'Invalid email address';
      $errors[5] = 'Invalid subscriber list ID';
      $errors[6] = 'Invalid subscriber ID';
      $errors[7] = 'Subscriber already exists';
      $errors[8] = 'One of the provided custom fields is empty. Custom field ID and title is provided as an additional output parameter';
      $errors[9] = 'One of the provided custom field value already exists in the database. Please enter a different value. Custom field ID and title is provided as an additional output parameter';
      $errors[10] = 'One of the provided custom field value failed in validation checking. Custom field ID and title is provided as an additional output parameter';
      $errors[99998] = 'Authentication failure or session expired';
      $errors[99999] = 'Not enough privileges';

      $error_code = $serial['ErrorCode'];
      $this->throwErrors($error_code, $errors);
//      drupal_set_message('Failed to connect to the server. ErrorCode: ' . $error_code);
      return FALSE;
    }
    else {
//      if ($serial['Success'] == TRUE) {
        return $serial;
//      }
//      else {
//        drupal_set_message('Failed to retrieve update success response');
//        return FALSE;
//      }
    }
  }
//  Subscribers.Delete

  /**
   * Users 
   */
// User.AddCredits
// User.Get
// User.Update

  /**
   * User.Login
   * Verifies the provided username and password then logs the user in.
   * @link http://docs.octeth.com/docs/oempro/developers/api/commands/user.login/
   *
   * @param string $username
   * @param string $password
   * @param
   * Request Parameters
   * Command = {User.Login} (required) - API Command
   * ResponseFormat = {JSON | XML} (required) - Response format
   * JSONPCallBack = {bool} - Send this parameter for cross-domain.
   * Username = {string} (required) - Username of the user to be logged in
   * Password = {string} (required) - Password of the user to be logged in
   * RememberMe = {string} (required) - Stores the login information in the cookie
   * Captcha = {string} (required) - If captcha image verification is enabled, direct API login will be disabled
   * DisableCaptcha = {true | false} (optional) If this parameter is passed in, it will by-pass the captcha validation
   *
   * @return
   * Response Parameters
   * Success = {true | false} - States if there is an error or not
   * ErrorCode = {integer} - If there is an error, error's code
   * SessionID = {string} - SessionID of the logged in user
   * AdminInfo = {array} - Provided if the login information is correct. Contains the logged in administrator account information
   *
   * Error Codes
   * 1 Username is missing
   * 2 Password is missing
   * 3 Invalid login information
   * 4 Invalid image verification
   * 5 Image verification failed
   * 99998 Authentication failure or session expired
   * 99999 Not enough privileges
   */
  function userLogin($username, $password) {
    // Setup POST parameters
    $post_params = array();
    $post_params['Command'] = "User.Login";
    $post_params['ResponseFormat'] = "JSON";
    $post_params['Username'] = $username;
    $post_params['Password'] = $password;
    $post_params['RememberMe'] = 'yes';
    $post_params['DisableCaptcha'] = "true";

    $response = $this->callServer($post_params);
    $serial = json_decode($response[1], TRUE);
//    oempro_debug(array('OEMPROAPI.class.php' => 'userLogin'), array('Line' => '1221'));
//    oempro_debug($response, $serial);

    if ($response[0] === FALSE) {

      $errors[1] = 'Username is missing';
      $errors[2] = 'Password is missing';
      $errors[3] = 'Invalid login information';
      $errors[4] = 'Invalid image verification';
      $errors[5] = 'Image verification failed';
      $errors[99998] = 'Authentication failure or session expired';
      $errors[99999] = 'Not enough privileges';

      $error_code = $serial['ErrorCode'];
      $this->throwErrors($error_code, $errors);
//      drupal_set_message('Failed to connect to the server. ErrorCode: ' . $error_code);
    }
    else {
      $this->SessionID = $serial['SessionID'];
      $this->userLogged = true;
      return $serial;
    }
  }
//    User.Create
//    User.PasswordRemind
//    User.PasswordReset
//    User.Switch
//    User.PaymentPeriods
//    User.PaymentPeriods.Update
//    Users.Delete
//    Users.Get

  /**
   * POSTs given array to a remote URL with CURL
   *
   * @return array(1:bool, 2:response text)
   */
  function callServer($ArrayPostParameters, $HTTPRequestType = 'POST', $HTTPAuth = FALSE, $HTTPAuthUsername = '', $HTTPAuthPassword = '', $ConnectTimeOutSeconds = 5, $ReturnHeaders = false) {
    $URL = $this->apiUrl["host"] . "/api.php?";
//    oempro_debug($ArrayPostParameters, array('from'=>'callServer'));

    $PostParameters = http_build_query($ArrayPostParameters);
    $CurlHandler = curl_init();
    curl_setopt($CurlHandler, CURLOPT_URL, $URL);

    if ($HTTPRequestType == 'GET') {
      curl_setopt($CurlHandler, CURLOPT_HTTPGET, TRUE);
    }
    elseif ($HTTPRequestType == 'PUT') {
      curl_setopt($CurlHandler, CURLOPT_PUT, TRUE);
    }
    elseif ($HTTPRequestType == 'DELETE') {
      curl_setopt($CurlHandler, CURLOPT_CUSTOMREQUEST, 'DELETE');
    }
    else {
      curl_setopt($CurlHandler, CURLOPT_POST, TRUE);
      curl_setopt($CurlHandler, CURLOPT_POSTFIELDS, $PostParameters);
    }

    curl_setopt($CurlHandler, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($CurlHandler, CURLOPT_CONNECTTIMEOUT, $ConnectTimeOutSeconds);
    curl_setopt($CurlHandler, CURLOPT_TIMEOUT, $ConnectTimeOutSeconds);
    curl_setopt($CurlHandler, CURLOPT_USERAGENT, 'Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.9.0.3) Gecko/2008092417 Firefox/3.0.3');

    // The option doesn't work with safe mode or when open_basedir is set.
    if ((ini_get('safe_mode') != FALSE) && (ini_get('open_basedir') != FALSE)) {
      curl_setopt($CurlHandler, CURLOPT_FOLLOWLOCATION, TRUE);
    }

    if ($ReturnHeaders == TRUE) {
      curl_setopt($CurlHandler, CURLOPT_HEADER, TRUE);
    }
    else {
      curl_setopt($CurlHandler, CURLOPT_HEADER, false);
    }

    if ($HTTPAuth == true) {
      curl_setopt($CurlHandler, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
      curl_setopt($CurlHandler, CURLOPT_USERPWD, $HTTPAuthUsername . ':' . $HTTPAuthPassword);
    }

    $RemoteContent = curl_exec($CurlHandler);

    if (curl_error($CurlHandler) != '') {
      return array(FALSE, curl_error($CurlHandler));
    }

    curl_close($CurlHandler);

    return array(TRUE, $RemoteContent);
  }

  function throwErrors($error_codes, $error_msgs) {
    $throw_codes = '';
    $throw_msgs = '';
    $message_array = array();
    if (is_array($error_codes)) {
      foreach($error_codes as $int_error) {
        $message_array[] = $error_msgs[$int_error];
      }
      $throw_codes = implode(' / ', $error_codes);
      $throw_msgs = implode(' / ', $message_array);
    }
    else {
      $throw_codes = $error_codes;
      $throw_msgs = $error_msgs[$error_codes];
    }

    $this->errorMessage = $throw_msgs; 
    $this->error_code = $throw_codes;
    drupal_set_message('Problems connecting to the Mail System. Error no:' . $throw_codes . ' - ' . $throw_msgs);
  }

}