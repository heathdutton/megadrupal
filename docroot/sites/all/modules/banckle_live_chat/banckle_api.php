<?php

/**
 * @file
 * Wrapper function for interaction with Banckle API
 */

/**
 * Prepares Banckle API for authentication. Must be called
 * before banckle_api_token
 *
 * @param $username
 *   A string for Banckle Login ID.
 * @param $password
 *   A string for Banckle account password.
 */
function banckle_api_authenticate($username, $password) {
  $_SESSION["banckle_api_username"] = $username;
  $_SESSION["banckle_api_password"] = $password;
  unset($_SESSION["banckle_api_authentication_token"]);
}

/**
 * Performs authentication and return Banckle Authentication API.
 *
 * banckle_api_authenticate must be called before this.
 *
 * @return
 *   Banckle API token string if authentication was successful
 *   or an associative array containing:
 *   - error: An integer for error code.
 *   - message: A string for error details.
 */
function banckle_api_token() {
  if (isset($_SESSION["banckle_api_authentication_token"])) {
    return $_SESSION["banckle_api_authentication_token"];
  }

  global $base_url;
  $username = $_SESSION["banckle_api_username"];
  $password = $_SESSION["banckle_api_password"];
  $url = "https://apps.banckle.com/api/authenticate?userid=$username&password=$password&platform=drupal&sourceSite=$base_url";
  $token = NULL;

  if ($response = json_decode(banckle_api_request($url), TRUE)) {
    if (isset($response["return"]) && isset($response["return"]["token"])) {
      $token = $_SESSION["banckle_api_authentication_token"] = strval($response["return"]["token"]);
    }
    else {
      if (isset($response["error"]) && isset($response["error"]["code"]) && isset($response["error"]["details"])) {
        return array("error" => intval($response["error"]["code"]), "message" => strval($response["error"]["details"]));
      }
      else {
        return array("error" => -2, "message" => "Unknown error while logging in to Banckle Live Chat servers");
      }
    }
  }
  else {
    return array("error" => -1, "message" => t("Cannot communicate to server"));
  }

  return $token;
}

/**
 * Registers a new Banckle user with give account information.
 *
 * @param $username
 *   A string for Banckle Login ID.
 * @param $password
 *   A string for Password for the new account.
 * @param $email
 *   A string for Email address.
 * @param $domain
 *   A string for Company domain.
 *
 * @return
 *   TRUE if signup is successful or an associative array containing:
 *   - error: An integer for error code.
 *   - message: A string for error details.
 */
function banckle_api_signup($username, $password, $email, $domain) {
  $url = "https://apps.banckle.com/api/registeruser?uid=$username&password=$password&email=$email&domain=$domain";

  if (!$response = json_decode(banckle_api_request($url), TRUE)) {
    return array("error" => -1, "message" => t("Cannot communicate to server"));
  }

  if (!$response["success"]) {
    return array("error" => intval($response["error"]["code"]), "message" => $response["error"]["details"]);
  }

  return TRUE;
}

/**
 * Returns a list of Banckle Live Chat departments
 *
 * @return
 *   List of Departments
 */
function banckle_api_get_departments() {
  $departments = array();

  if ($response = banckle_api_request("https://apps.banckle.com/em/api/departments.xml?_token=" . banckle_api_token())) {
    $response = new SimpleXMLElement(utf8_encode($response));
    foreach ($response->department as $d) {
      $departments[strval($d->id)] = strval($d->displayName);
    }
  }

  return $departments;
}

/**
 * Adds a new Banckle Live Chat department.
 *
 * @param $attributes
 *   Associative array of department attributes.
 */
function banckle_api_add_department($attributes) {
  $url = "https://apps.banckle.com/em/api/departments.js?_token=" . banckle_api_token();
  $request = json_encode($attributes);
  if (!$response = json_decode(banckle_api_request($url, array("method" => "POST", "data" => $request)), TRUE)) {
    return NULL;
  }

  return $response;
}

/**
 * Adds a new Banckle Live Chat deployment.
 *
 * @param $attributes
 *   Associative array of deployment attributes.
 */
function banckle_api_add_deployment($attributes) {
  $url = "https://apps.banckle.com/em/api/deployments.xml?_token=" . banckle_api_token();
  $request = "
<deployment>
  <name>" . $attributes["deployment_name"] . "</name>
  <title>" . $attributes["title"] . "</title>
  <copyright>" . $attributes["copyright"] . "</copyright>
  <welcomeMessage>" . $attributes["welcome_message"] . "</welcomeMessage>
  <waitingMessage>" . $attributes["waiting_message"] . "</waitingMessage>
  <unavailableMessage>" . $attributes["unavailable_message"] . "</unavailableMessage>
  <finalMessage>" . $attributes["final_message"] . "</finalMessage>
  <inviteMessage>" . $attributes["invite_message"] . "</inviteMessage>
  <departments>" . implode(";", array_keys($attributes["departments"])) . "</departments>
  <exitSurvey><!-- --></exitSurvey>
  <enableAutoInvite>" . ($attributes["enable_auto_invite"] ? "true" : "false") . "</enableAutoInvite>
  <inviteTimeout>" . $attributes["invite_timeout"] . "</inviteTimeout>
  <autoInviteImage><!-- --></autoInviteImage>
  <enableProactiveInvite>" . ($attributes["enable_proactive_invite"] ? "true" : "false") . "</enableProactiveInvite>
  <proactiveInviteImage></proactiveInviteImage>
  <enableInvitationFilter>" . ($attributes["enable_invitation_filter"] ? "true" : "false") . "</enableInvitationFilter>
  <invitationFilterType>0</invitationFilterType>
  <theme>theme-4</theme>
  <linkType>0</linkType>
  <themeFlags>0</themeFlags>
  <creationDate>0</creationDate>
</deployment>
    ";

  return banckle_api_request($url, array("method" => "POST", "data" => $request));
}

/**
 * Returns a list of Banckle Live Chat deployments
 *
 * @return
 *   List of Deployments
 */
function banckle_api_get_deployments() {
  $url = "https://apps.banckle.com/em/api/deployments.xml?_token=" . banckle_api_token();
  $deployments = array();

  if ($response = banckle_api_request($url)) {
    $response = new SimpleXMLElement(utf8_encode($response));

    foreach ($response->deployment as $d) {
      $deployments[strval($d->id)] = strval($d->name);
    }
  }

  return $deployments;
}

/**
 * Perform Banckle API request.
 *
 * @param $url
 *   A string for target Banckle API URL.
 * @param $options
 *   Same as drupal_http_request().
 *
 * @return
 *   Raw data that is received in response.
 *
 * @see drupal_http_request()
 */
function banckle_api_request($url, $options = array()) {
//  drupal_set_message("Debug: Request: $url");
  $result = drupal_http_request($url, $options);
  if ($result->code == 200) {
    return $result->data;
  }

  return FALSE;
}
