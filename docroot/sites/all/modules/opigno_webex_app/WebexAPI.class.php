<?php
/**
 * @file
 * API wrapper class for WebEx XML post.
 *
 * @author Josh Hoegen
 * @author James Aparicio
 */

class WebexAPI {
  public function __construct() {
    $this->webex_user = variable_get('opigno_webex_app_webex_user');
    $this->webex_password = variable_get('opigno_webex_app_webex_password');
    $this->webex_sid = variable_get('opigno_webex_app_webex_sid');
    $this->webex_pid = variable_get('opigno_webex_app_webex_pid');
    $this->webex_url = variable_get('opigno_webex_app_webex_url');
    $this->webex_list_type = 'meeting';
    $this->webex_start_record = 0;
    $this->webex_max_record = 100;
    $this->webex_max_date = date('m/d/Y G:H:i', strtotime('+3months'));
    $this->webex_date_time_zone = 11;
    $this->webex_sort = '<orderBy>STARTTIME</orderBy><orderAD>ASC</orderAD>';
    $this->webex_response = '';
    $this->webex_error_msg = '';
    $this->webex_protocol = 'https';
  }

  protected function getmeetingXML($meetingid) {
    $webex_post = new stdClass();
    $webex_post->UID = $this->webex_user;
    $webex_post->PWD = $this->webex_password;
    $webex_post->SID = $this->webex_sid;
    $webex_post->PID = $this->webex_pid;
    $webex_post->XML = '
<?xml version="1.0" encoding="ISO-8859-1"?>
<serv:message xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
  <header>
    <securityContext>
          <webExID>' . $this->webex_user . '</webExID>
          <password>' . $this->webex_password . '</password>
          <siteID>' . $this->webex_sid . '</siteID>
          <partnerID>' . $this->webex_pid . '</partnerID>
        </securityContext>
  </header>
  <body>
  <bodyContent xsi:type="java:com.webex.service.binding.meeting.GetMeeting">
    <meetingKey>' . $meetingid . '</meetingKey>
  </bodyContent>
  </body>
</serv:message>';
    return $webex_post;

  }

  protected function postgetmeetingXML($meetingid) {
    $post_data = $this->getmeetingXML($meetingid);
    $post_url = $this->webex_url . '/WBXService/XMLService';
    $post_string = '';
    foreach ($post_data as $data_key => $data_value) {
      $post_string .= '' . $data_key . '=' . urlencode($data_value) . '&';
    }
    $post_string = substr($post_string, 0, -1);
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 0);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);
    curl_setopt($ch, CURLOPT_URL, $post_url);
    curl_setopt($ch, CURLOPT_POST, count($post_data));
    curl_setopt($ch, CURLOPT_POSTFIELDS, $post_string);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    $response = curl_exec($ch);
    $this->webex_error_msg = 'ERROR:<br/><b>RESPONSE:</b><span>' . strip_tags($response) . '</span><br/>
        <br/>
        <b>POSTED DATA:</b><span>' . $post_data->XML . '</span>';
    curl_close($ch);
    $this->webex_response = $response;
    return $response;
  }


  protected function buildmeetingXML($node, $isnew) {
    $confName = $node->title;
    $agenda = "Opigno";
    foreach($node->meeting_password as $lang => $value)
    {
      $meetingPassword= $value[0]['value'];
    }
    $maxUserNumber = 100;
    $chat = "true";
    $poll = "true";
    $audioVideo = "true";
    foreach($node->opigno_calendar_date as $lang => $value)
    {
      $startDate= date("m/d/Y h:i:s", strtotime($value[0]['value']));
    }
    //$startDate = date("m/d/Y h:i:s", strtotime($node->opigno_calendar_date[LANGUAGE_NONE][0]['value']));
    $openTime = 900;
    $joinTeleconfBeforeHost = "false";
    $duration = 20;
    $timeZoneID = 4;
    $telephonySupport = "CALLIN";
    $extTelephonyDescription = "Call 1-800-555-1234, Passcode 98765";
    $webex_post = new stdClass();
    $webex_post->UID = $this->webex_user;
    $webex_post->PWD = $this->webex_password;
    $webex_post->SID = $this->webex_sid;
    $webex_post->PID = $this->webex_pid;
    if ($isnew == TRUE) {
      $webex_post->XML = '<?xml version="1.0" encoding="ISO-8859-1"?>
  <serv:message xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
    <header>
       <securityContext>
          <webExID>' . $this->webex_user . '</webExID>
          <password>' . $this->webex_password . '</password>
          <siteID>' . $this->webex_sid . '</siteID>
          <partnerID>' . $this->webex_pid . '</partnerID>
        </securityContext>
    </header>
    <body>
    <bodyContent
      xsi:type="java:com.webex.service.binding.meeting.CreateMeeting">
      <accessControl>
        <meetingPassword>' . $meetingPassword . '</meetingPassword>
      </accessControl>
      <metaData>
        <confName>' . $confName . '</confName>
        <meetingType></meetingType>
        <agenda>' . $agenda . '</agenda>
      </metaData>
      <enableOptions>
        <importDocument>true</importDocument>
        <fileShare>true</fileShare>
        <desktopShare>true</desktopShare>
        <chatPresenter>true</chatPresenter>
        <chatHost>true</chatHost>
        <attendeeRecordMeeting>true</attendeeRecordMeeting>
        <chatAllAttendees>true</chatAllAttendees>
        <chat>' . $chat . '</chat>
        <poll>' . $poll . '</poll>
        <audioVideo>' . $audioVideo . '</audioVideo>
      </enableOptions>
      <schedule>
        <startDate>' . $startDate . '</startDate>
        <openTime>' . $openTime . '</openTime>
        <joinTeleconfBeforeHost>' . $joinTeleconfBeforeHost . '</joinTeleconfBeforeHost>
        <duration>' . $duration . '</duration>
        <timeZoneID>' . $timeZoneID . '</timeZoneID>
      </schedule>
    </bodyContent>
    </body>
  </serv:message>';
    }
    else {
      foreach($node->meeting_id as $lang => $value)
      {
        $meetingid= $value[0]['value'];
      }
      if ($response = $this->postgetmeetingXML($meetingid)) {
        $xml_obj = new SimpleXMLElement($response);
        $meetingType = $xml_obj->children('serv', TRUE)->body->bodyContent->children('meet', TRUE)->metaData->meetingType; //',true);//->meetingType;
      }
      $webex_post->XML = '<?xml version="1.0" encoding="ISO-8859-1"?>
<serv:message xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
<header>
       <securityContext>
          <webExID>' . $this->webex_user . '</webExID>
          <password>' . $this->webex_password . '</password>
          <siteID>' . $this->webex_sid . '</siteID>
          <partnerID>' . $this->webex_pid . '</partnerID>
        </securityContext>
    </header>
<body>
<bodyContent
xsi:type="java:com.webex.service.binding.meeting.SetMeeting">
<accessControl>
        <meetingPassword>' . $meetingPassword . '</meetingPassword>
      </accessControl>
<metaData>
        <confName>' . $confName . '</confName>
        <meetingType>' . $meetingType . '</meetingType>
        <agenda>' . $agenda . '</agenda>
      </metaData>
<participants>
<maxUserNumber>4</maxUserNumber>
<attendees></attendees>
</participants>
<enableOptions>
        <importDocument>true</importDocument>
        <fileShare>true</fileShare>
        <desktopShare>true</desktopShare>
        <chatPresenter>true</chatPresenter>
        <chatHost>true</chatHost>
        <attendeeRecordMeeting>true</attendeeRecordMeeting>
        <chatAllAttendees>true</chatAllAttendees>
        <chat>' . $chat . '</chat>
        <poll>' . $poll . '</poll>
        <audioVideo>' . $audioVideo . '</audioVideo>
      </enableOptions>
<schedule>
        <startDate>' . $startDate . '</startDate>
        <openTime>' . $openTime . '</openTime>
        <joinTeleconfBeforeHost>' . $joinTeleconfBeforeHost . '</joinTeleconfBeforeHost>
        <duration>' . $duration . '</duration>
        <timeZoneID>' . $timeZoneID . '</timeZoneID>
      </schedule>
<meetingkey>' . $meetingid . '</meetingkey>
</bodyContent>
</body>
</serv:message>';
    }
    return $webex_post;
  }

  protected function postmeetingXML($node, $isnew) {
    $post_data = $this->buildmeetingXML($node, $isnew);
    $post_url = $this->webex_url . '/WBXService/XMLService';
    $post_string = '';
    foreach ($post_data as $data_key => $data_value) {
      $post_string .= '' . $data_key . '=' . urlencode($data_value) . '&';
    }
    $post_string = substr($post_string, 0, -1);
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 0);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);
    curl_setopt($ch, CURLOPT_URL, $post_url);
    curl_setopt($ch, CURLOPT_POST, count($post_data));
    curl_setopt($ch, CURLOPT_POSTFIELDS, $post_string);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    $response = curl_exec($ch);
    $this->webex_error_msg = 'ERROR:<br/><b>RESPONSE:</b><span>' . strip_tags($response) . '</span><br/>
        <br/>
        <b>POSTED DATA:</b><span>' . $post_data->XML . '</span>';
    curl_close($ch);
    $this->webex_response = $response;
    return $response;
  }

  public function getmeetingWebex($node, $isnew) {
    $meetingkey = 0;
    if ($response = $this->postmeetingXML($node, $isnew)) {
      $xml_obj = new SimpleXMLElement($response);
      if ($isnew == TRUE) {
        if (!(strpos($xml_obj->asXML(), 'FAILURE') !== FALSE)) {
          $meetingkey = $xml_obj->children('serv', TRUE)->body->bodyContent->children('meet', TRUE)->meetingkey;
        }
      }
      else {
        if ((strpos($xml_obj->asXML(), 'SUCCESS') !== FALSE)) {
          $meetingkey = 1;
        }
      }
    }
    //dpm($node,"node going");
    //dpm($response,"response");
    return (string) $meetingkey;
  }


  protected function buildXML() {
    $webex_post = new stdClass();
    $webex_post->UID = $this->webex_user;
    $webex_post->PWD = $this->webex_password;
    $webex_post->SID = $this->webex_sid;
    $webex_post->PID = $this->webex_pid;
    $webex_post->XML = '<?xml version="1.0" ?>
    <serv:message xmlns:serv="http://www.webex.com/schemas/2002/06/service" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
      <header>
        <securityContext>
          <webExID>' . $this->webex_user . '</webExID>
          <password>' . $this->webex_password . '</password>
          <siteID>' . $this->webex_sid . '</siteID>
          <partnerID>' . $this->webex_pid . '</partnerID>
        </securityContext>
      </header>
      <body><bodyContent xmlns:meet="http://www.webex.com/schemas/2002/06/service/' . strtolower($this->webex_list_type) . '"
        xsi:type="java:com.webex.service.binding.' . strtolower($this->webex_list_type) . '.Lstsummary' . ucwords($this->webex_list_type) . '">
          <listControl>
            <startFrom>' . $this->webex_start_record . '</startFrom>
            <maximumNum>' . $this->webex_max_record . '</maximumNum>
          </listControl>
          <order>
            ' . $this->webex_sort . '
          </order>
          <dateScope>
            <startDateStart>' . $this->webex_date_start . '</startDateStart>
            <startDateEnd>' . $this->webex_max_date . '</startDateEnd>
            <timeZoneID>' . $this->webex_date_time_zone . '</timeZoneID>
          </dateScope>
        </bodyContent>
      </body>
    </serv:message>';
    return $webex_post;
  }

  protected function postXML() {
    $post_data = $this->buildXML();
    $post_url = $this->webex_url . '/WBXService/XMLService';
    $post_string = '';
    foreach ($post_data as $data_key => $data_value) {
      $post_string .= '' . $data_key . '=' . urlencode($data_value) . '&';
    }
    $post_string = substr($post_string, 0, -1);
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 0);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);
    curl_setopt($ch, CURLOPT_URL, $post_url);
    curl_setopt($ch, CURLOPT_POST, count($post_data));
    curl_setopt($ch, CURLOPT_POSTFIELDS, $post_string);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    $response = curl_exec($ch);
    $this->webex_error_msg = 'ERROR:<br/><b>RESPONSE:</b><span>' . strip_tags($response) . '</span><br/>
        <br/>
        <b>POSTED DATA:</b><span>' . $post_data->XML . '</span>';
    curl_close($ch);
    $this->webex_response = $response;
    return $response;
  }

  public function getWebex() {
    $events = FALSE;
    $event_type = strtolower($this->webex_list_type);
    if ($response = $this->postXML()) {
      $xml_obj = new SimpleXMLElement($response);
      $type = "meeting";
      $events = $xml_obj->children('serv', TRUE)->body->bodyContent->children('meet', TRUE)->meeting;
    }
    return $events;
  }


  public function getRegistrationLink($session_key) {
    $initial = substr($this->webex_list_type, 0, 1);
    if ($initial == 'e') {
      $url_suffix = 'AT=SINF&MK=';
    }
    else {
      $url_suffix = 'AT=JM&MK=';
    }
    return $this->webex_url . '/' . $this->webex_subdomain . '/' . $initial . '.php?' . $url_suffix . '' . $session_key;
  }

  public function getErrorMsg() {
    return $this->webex_error_msg;
  }
}
