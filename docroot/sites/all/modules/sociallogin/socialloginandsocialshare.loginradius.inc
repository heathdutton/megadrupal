<?php

/**
 * @file
 * Sdk page callback file for the socialloginandsocialshare module.
 * This class should be used only for Drupal site.
 */
// Define LoginRadius domain
define('LR_DOMAIN', 'api.loginradius.com');

/**
 * Class for Social Authentication.
 *
 * This is the main class to communicate with LogiRadius Unified Social API. It contains functions for Social Authentication with User Profile Data (Basic and Extended)
 *
 * Copyright 2015 LoginRadius Inc. - www.LoginRadius.com
 *
 * This file is part of the LoginRadius SDK package.
 *
 */
class LoginRadius {

  /**
   * LoginRadius function - It validates against GUID format of keys.
   *
   * @param string $value data to validate.
   *
   * @return boolean If valid - true, else - false
   */
  public function loginradius_is_valid_guid($value) {
    return preg_match('/^\{?[A-Z0-9]{8}-[A-Z0-9]{4}-[A-Z0-9]{4}-[A-Z0-9]{4}-[A-Z0-9]{12}\}?$/i', $value);
  }

  /**
   * LoginRadius function - Check, if it is a valid callback i.e. LoginRadius authentication token is set
   *
   * @return boolean True, if a valid callback.
   */
  public function loginradius_is_callback() {
    if (isset($_POST['token'])) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

  /**
   * LoginRadius function - Fetch LoginRadius access token after authentication. It will be valid for the specific duration of time specified in the response.
   *
   * @param string LoginRadius API Secret
   *
   * @return string LoginRadius access token.
   */
  public function loginradius_fetch_access_token($secret) {
    if (!$this->loginradius_is_valid_guid($secret)) {
      return FALSE;
    }
    $validate_url = "https://" . LR_DOMAIN . "/api/v2/access_token?token=" . $_POST['token'] . "&secret=" . $secret;
    $response = json_decode($this->loginradius_get_api_response($validate_url));
    if (isset($response->access_token) && $response->access_token != '') {
      return $response->access_token;
    }
    else {
      return FALSE;
    }
  }

  /**
   * LoginRadius function - To fetch social profile data from the user's social account after authentication. The social profile will be retrieved via oAuth and OpenID protocols. The data is normalized into LoginRadius' standard data format.
   *
   * @param string $access_token LoginRadius access token
   * @param boolean $raw If true, raw data is fetched
   *
   * @return object User profile data.
   */
  public function loginradius_get_user_profiledata($access_token, $raw = FALSE) {
     $validate_url = "https://" . LR_DOMAIN . "/api/v2/userprofile?access_token=" . $access_token;
    if ($raw) {
      $validate_url = "https://" . LR_DOMAIN . "/api/v2/userprofile/raw?access_token=" . $access_token;
      return $this->loginradius_get_api_response($validate_url);
    }
    return json_decode($this->loginradius_get_api_response($validate_url));
  }

  /**
   * LoginRadius function - To get the Albums data from the user's social account. The data will be normalized into LoginRadius' data format.
   *
   * @param string $access_token LoginRadius access token
   * @param boolean $raw If true, raw data is fetched
   *
   * @return object User's albums data.
   */
  public function loginradius_get_photo_albums($access_token, $raw = FALSE) {
    $validate_url = "https://" . LR_DOMAIN . "/api/v2/album?access_token=" . $access_token;
    if ($raw) {
      $validate_url = "https://" . LR_DOMAIN . "/api/v2/album/raw?access_token=" . $access_token;
      return $this->loginradius_get_api_response($validate_url);
    }
    return json_decode($this->loginradius_get_api_response($validate_url));
  }

  /**
   * LoginRadius function - To fetch photo data from the user's social account. The data will be normalized into LoginRadius' data format.
   *
   * @param string $access_token LoginRadius access token
   * @param string $album_id ID of the album to fetch photos from
   * @param boolean $raw If true, raw data is fetched
   *
   * @return object User's photo data.
   */
  public function loginradius_get_photos($access_token, $album_id = '', $raw = FALSE) {
    $validate_url = "https://" . LR_DOMAIN . "/api/v2/photo?access_token=" . $access_token . "&albumid=" . $album_id;
    if ($raw) {
      $validate_url = "https://" . LR_DOMAIN . "/api/v2/photo/raw?access_token=" . $access_token . "&albumid=" . $album_id;
      return $this->loginradius_get_api_response($validate_url);
    }
    return json_decode($this->loginradius_get_api_response($validate_url));
  }

  /**
   * LoginRadius function - To fetch check-ins data from the user's social account. The data will be normalized into LoginRadius' data format.
   *
   * @param string $access_token LoginRadius access token
   * @param boolean $raw If true, raw data is fetched
   *
   * @return object User's check-ins.
   */
  public function loginradius_get_checkins($access_token, $raw = FALSE) {
    $validate_url = "https://" . LR_DOMAIN . "/api/v2/checkin?access_token=" . $access_token;
    if ($raw) {
      $validate_url = "https://" . LR_DOMAIN . "/api/v2/checkin/raw?access_token=" . $access_token;
      return $this->loginradius_get_api_response($validate_url);
    }
    return json_decode($this->loginradius_get_api_response($validate_url));
  }

  /**
   * LoginRadius function - To fetch user's audio files data from the user's social account. The data will be normalized into LoginRadius' data format.
   *
   * @param string $access_token LoginRadius access token
   * @param boolean $raw If true, raw data is fetched
   *
   * @return object User's audio files data.
   */
  public function loginradius_get_audio($access_token, $raw = FALSE) {
    $validate_url = "https://" . LR_DOMAIN . "/api/v2/audio?access_token=" . $access_token;
    if ($raw) {
      $validate_url = "https://" . LR_DOMAIN . "/api/v2/audio/raw?access_token=" . $access_token;
      return $this->loginradius_get_api_response($validate_url);
    }
    return json_decode($this->loginradius_get_api_response($validate_url));
  }

  /**
   * LoginRadius function - Post messages to the user's contacts. After using the Contact API, you can send messages to the retrieved contacts.
   *
   * @param string $access_token LoginRadius access token
   * @param string $to Social ID of the receiver
   * @param string $subject Subject of the message
   * @param string $message Message
   *
   * @return bool True on success, false otherwise
   */
  public function loginradius_send_message($access_token, $provider, $to, $subject, $message) {
    if ($access_token != '') {
      if ($provider == 'twitter') {
        foreach ($to as $id) {
          $validateurl = 'https://' . LR_DOMAIN . '/api/v2/message?' . http_build_query(array(
                'access_token' => $access_token,
                'to' => $id,
                'subject' => $subject,
                'message' => $message
          ));
          json_decode($this->loginradius_get_api_response($validateurl, TRUE));
        }
      }
      else {
        $validateurl = 'https://' . LR_DOMAIN . '/api/v2/message?' . http_build_query(array(
              'access_token' => $access_token,
              'to' => implode(',', $to),
              'subject' => $subject,
              'message' => $message
        ));
         json_decode($this->loginradius_get_api_response($validateurl, TRUE));
      }
      return;
    }
  }

  /**
   * LoginRadius function - To fetch user's contacts/friends/connections data from the user's social account. The data will normalized into LoginRadius' data format.
   *
   * @param string $access_token LoginRadius access token
   * @param integer $next_cursor Offset to start fetching contacts from
   * @param boolean $raw If true, raw data is fetched
   *
   * @return object User's contacts/friends/followers.
   */
  public function loginradius_get_contacts($access_token, $next_cursor = '', $raw = FALSE) {
    $validate_url = "https://" . LR_DOMAIN . "/api/v2/contact?access_token=" . $access_token . "&nextcursor=" . $next_cursor;
    if ($raw) {
      $validate_url = "https://" . LR_DOMAIN . "/api/v2/contact/raw?access_token=" . $access_token . "&nextcursor=" . $next_cursor;
      return $this->loginradius_get_api_response($validate_url);
    }
    return json_decode($this->loginradius_get_api_response($validate_url));
  }

  /**
   * LoginRadius function - To get mention data from the user's social account. The data will be normalized into LoginRadius' data format.
   *
   * @param string $access_token LoginRadius access token
   * @param boolean $raw If true, raw data is fetched
   *
   * @return object User's twitter mentions.
   */
  public function loginradius_get_mentions($access_token, $raw = FALSE) {
    $validate_url = "https://" . LR_DOMAIN . "/api/v2/mention?access_token=" . $access_token;
    if ($raw) {
      $validate_url = "https://" . LR_DOMAIN . "/api/v2/mention/raw?access_token=" . $access_token;
      return $this->loginradius_get_api_response($validate_url);
    }
    return json_decode($this->loginradius_get_api_response($validate_url));
  }

  /**
   * LoginRadius function - To fetch information of the people, user is following on Twitter.
   *
   * @param string $access_token LoginRadius access token
   * @param boolean $raw If true, raw data is fetched
   *
   * @return object Information of the people, user is following.
   */
  public function loginradius_get_following($access_token, $raw = FALSE) {
    $validate_url = "https://" . LR_DOMAIN . "/api/v2/following?access_token=" . $access_token;
    if ($raw) {
      $validate_url = "https://" . LR_DOMAIN . "/api/v2/following/raw?access_token=" . $access_token;
      return $this->loginradius_get_api_response($validate_url);
    }
    return json_decode($this->loginradius_get_api_response($validate_url));
  }

  /**
   * LoginRadius function - To get the event data from the user's social account. The data will be normalized into LoginRadius' data format.
   *
   * @param string $access_token LoginRadius access token
   * @param boolean $raw If true, raw data is fetched
   *
   * @return object User's event data.
   */
  public function loginradius_get_events($access_token, $raw = FALSE) {
    $validate_url = "https://" . LR_DOMAIN . "/api/v2/event?access_token=" . $access_token;
    if ($raw) {
      $validate_url = "https://" . LR_DOMAIN . "/api/v2/event/raw?access_token=" . $access_token;
      return $this->loginradius_get_api_response($validate_url);
    }
    return json_decode($this->loginradius_get_api_response($validate_url));
  }

  /**
   * LoginRadius function - To get posted messages from the user's social account. The data will be normalized into LoginRadius' data format.
   *
   * @param string $access_token LoginRadius access token
   * @param boolean $raw If true, raw data is fetched
   *
   * @return object User's posted messages.
   */
  public function loginradius_get_posts($access_token, $raw = FALSE) {
    $validate_url = "https://" . LR_DOMAIN . "/api/v2/post?access_token=" . $access_token;
    if ($raw) {
      $validate_url = "https://" . LR_DOMAIN . "/api/v2/post/raw?access_token=" . $access_token;
      return $this->loginradius_get_api_response($validate_url);
    }
    return json_decode($this->loginradius_get_api_response($validate_url));
  }

  /**
   * LoginRadius function - To get the followed company's data in the user's social account. The data will be normalized into LoginRadius' data format.
   *
   * @param string $access_token LoginRadius access token
   * @param boolean $raw If true, raw data is fetched
   *
   * @return object Companies followed by user.
   */
  public function loginradius_get_followed_companies($access_token, $raw = FALSE) {
    $validate_url = "https://" . LR_DOMAIN . "/api/v2/company?access_token=" . $access_token;
    if ($raw) {
      $validate_url = "https://" . LR_DOMAIN . "/api/v2/company/raw?access_token=" . $access_token;
      return $this->loginradius_get_api_response($validate_url);
    }
    return json_decode($this->loginradius_get_api_response($validate_url));
  }

  /**
   * LoginRadius function - To get group data from the user's social account. The data will be normalized into LoginRadius' data format.
   *
   * @param string $access_token LoginRadius access token
   * @param boolean $raw If true, raw data is fetched
   *
   * @return object Group data.
   */
  public function loginradius_get_groups($access_token, $raw = FALSE) {
    $validate_url = "https://" . LR_DOMAIN . "/api/v2/group?access_token=" . $access_token;
    if ($raw) {
      $validate_url = "https://" . LR_DOMAIN . "/api/v2/group/raw?access_token=" . $access_token;
      return $this->loginradius_get_api_response($validate_url);
    }
    return json_decode($this->loginradius_get_api_response($validate_url));
  }

  /**
   * LoginRadius function - To get the status messages from the user's social account. The data will be normalized into LoginRadius' data format.
   *
   * @param string $access_token LoginRadius access token
   * @param boolean $raw If true, raw data is fetched
   *
   * @return object Status messages.
   */
  public function loginradius_get_status($access_token, $raw = FALSE) {
    $validate_url = "https://" . LR_DOMAIN . "/api/v2/status?access_token=" . $access_token;
    if ($raw) {
      $validate_url = "https://" . LR_DOMAIN . "/api/v2/status/raw?access_token=" . $access_token;
      return $this->loginradius_get_api_response($validate_url);
    }
    return json_decode($this->loginradius_get_api_response($validate_url));
  }

  /**
   * LoginRadius function - To update the status on the user's wall.
   *
   * @param string $access_token LoginRadius access token
   * @param string $title Title for status message (Optional).
   * @param string $url A web link of the status message (Optional).
   * @param string $imageurl An image URL of the status message (Optional).
   * @param string $status The status message text (Required).
   * @param string $caption Caption of the status message (Optional).
   * @param string $description Description of the status message (Optional).
   *
   * @return boolean Returns true if successful, false otherwise.
   */
  public function loginradius_post_status($access_token, $title, $url, $imageurl, $status, $caption, $description) {
    $url = "https://" . LR_DOMAIN . "/api/v2/status?" . http_build_query(array(
          'access_token' => $access_token,
          'title' => $title,
          'url' => $url,
          'imageurl' => $imageurl,
          'status' => $status,
          'caption' => $caption,
          'description' => $description
    ));
    return json_decode($this->loginradius_get_api_response($url, TRUE));
  }

  /**
   * LoginRadius function - To get videos data from the user's social account. The data will be normalized into LoginRadius' data format.
   *
   * @param string $access_token LoginRadius access token
   * @param boolean $raw If true, raw data is fetched
   *
   * @return object Videos data.
   */
  public function loginradius_get_videos($access_token, $raw = FALSE) {
    $validate_url = "https://" . LR_DOMAIN . "/api/v2/video?access_token=" . $access_token;
    if ($raw) {
      $validate_url = "https://" . LR_DOMAIN . "/api/v2/video/raw?access_token=" . $access_token;
      return $this->loginradius_get_api_response($validate_url);
    }
    return json_decode($this->loginradius_get_api_response($validate_url));
  }

  /**
   * LoginRadius function - To get likes data from the user's social account. The data will be normalized into LoginRadius' data format.
   *
   * @param string $access_token LoginRadius access token
   * @param boolean $raw If true, raw data is fetched
   *
   * @return object Videos data.
   */
  public function loginradius_get_likes($access_token, $raw = FALSE) {
    $validate_url = "https://" . LR_DOMAIN . "/api/v2/like?access_token=" . $access_token;
    if ($raw) {
      $validate_url = "https://" . LR_DOMAIN . "/api/v2/like/raw?access_token=" . $access_token;
      return $this->loginradius_get_api_response($validate_url);
    }
    return json_decode($this->loginradius_get_api_response($validate_url));
  }

  /**
   * LoginRadius function - To get the page data from the user's social account. The data will be normalized into LoginRadius' data format.
   *
   * @param string $access_token LoginRadius access token
   * @param string $page_name Page name
   * @param boolean $raw If true, raw data is fetched
   *
   * @return object Page data.
   */
  public function loginradius_get_pages($access_token, $page_name, $raw = FALSE) {
    $validate_url = "https://" . LR_DOMAIN . "/api/v2/page?access_token=" . $access_token . "&pagename=" . $page_name;
    if ($raw) {
      $validate_url = "https://" . LR_DOMAIN . "/api/v2/page/raw?access_token=" . $access_token . "&pagename=" . $page_name;
      return $this->loginradius_get_api_response($validate_url);
    }
    return json_decode($this->loginradius_get_api_response($validate_url));
  }

  /**
   * LoginRadius function - To fetch data from the LoginRadius API URL.
   *
   * @param string $validate_url Target URL to fetch data from.
   *
   * @return string Data fetched from the LoginRadius API.
   */
  public function loginradius_get_api_response($validate_url, $post = FALSE, $contentType = 'application/x-www-form-urlencoded') {
    global $base_url;
    $data ='';
	if(!empty($post)){
    if(is_array($post)){
      $data = drupal_http_build_query($post);
    }
	else if($contentType == 'application/json'){
	$data  = $post;
	}
	}
    if(function_exists('drupal_http_request') && !in_array('drupal_http_request', $this->loginradius_get_disabled_functions())) {
      $headers = array(
        'Referer' => $base_url,
        'User-Agent' => 'LoginRadius Module',
      );
      $options = array(
        'headers' => $headers,
        'max_redirects' => 5,
      );
      if ($post) {
        $options = array(
          'method' => 'POST',
          'data' => $data,
          'timeout' => 15,
          'headers' => array('Content-Type' => $contentType),
        );
      }


      $result = drupal_http_request($validate_url, $options);
      $jsonresponse = isset($result->data) ? $result->data : '';


    }
     if(empty($jsonresponse)  && in_array('curl', get_loaded_extensions())) {
      $curl_handle = curl_init();
      curl_setopt($curl_handle, CURLOPT_URL, $validate_url);
      curl_setopt($curl_handle, CURLOPT_CONNECTTIMEOUT, 15);
      curl_setopt($curl_handle, CURLOPT_TIMEOUT, 15);
      curl_setopt($curl_handle, CURLOPT_ENCODING, 'json');
      curl_setopt($curl_handle, CURLOPT_SSL_VERIFYPEER, FALSE);
      if ($post) {
        curl_setopt($curl_handle, CURLOPT_HTTPHEADER, array('Content-type: ' . $contentType));
        curl_setopt($curl_handle, CURLOPT_POST, 1);
        curl_setopt($curl_handle, CURLOPT_POSTFIELDS, $data);
      }
      if (ini_get('open_basedir') == '' && (ini_get('safe_mode') == 'Off' or ! ini_get('safe_mode'))) {
        curl_setopt($curl_handle, CURLOPT_FOLLOWLOCATION, 1);
        curl_setopt($curl_handle, CURLOPT_RETURNTRANSFER, TRUE);
        $jsonresponse = curl_exec($curl_handle);
      }
      else {
        curl_setopt($curl_handle, CURLOPT_HEADER, 1);
        $url = curl_getinfo($curl_handle, CURLINFO_EFFECTIVE_URL);
        curl_close($curl_handle);
        $ch = curl_init();
        $url = str_replace('?', '/?', $url);
        curl_setopt($ch, CURLOPT_URL, $url);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
        $jsonresponse = curl_exec($ch);
        curl_close($ch);
      }
    }
    return $jsonresponse;
  }
    function loginradius_get_disabled_functions() {
        $disabled_functions = trim(ini_get('disable_functions'));
        if (drupal_strlen($disabled_functions) == 0) {
            $disabled_functions = array();
        }
        else {
            $disabled_functions = explode(',', $disabled_functions);
            $disabled_functions = array_map('trim', $disabled_functions);
        }
        return $disabled_functions;
    }
}
