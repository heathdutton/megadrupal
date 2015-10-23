<?php

/**
 * @file
 * Callback for admin/config/system/banckle/livechat/customize
 */

require_once "banckle_api.php" ;

/**
 * Form builder for the customize form.
 *
 * @see banckle_live_chat_menu()
 *
 * @ingroup forms
 */
function _form_banckle_live_chat_admin_customize($form, &$form_state) {
  if (!variable_get("banckle_live_chat_deployment")) {
    drupal_set_message(t("Banckle Chat for Drupal has not been enabled and activated as yet. Please configure its settings from the <a href='@configure'>Banckle Chat settings page</a>.", array("@configure" => url("admin/config/system/banckle/livechat"))), "error");
    return;
  }

  banckle_api_authenticate(variable_get("banckle_live_chat_username"), variable_get("banckle_live_chat_password"));
  $token = banckle_api_token();
  $deployment = NULL;
  $departments = array();
  $error = NULL;

  if (is_array($token)) {
    drupal_set_message($token["message"], "error");
    return;
  }

  if ($response = banckle_api_request("https://apps.banckle.com/em/api/deployments/" . variable_get("banckle_live_chat_deployment") . ".xml?_token=" . banckle_api_token())) {
    $deployment = new SimpleXMLElement(utf8_encode($response));
  }
  else {
    drupal_set_message(t("Cannot communicate to server. Please try again later"), "error");
    return;
  }

  if ($response = banckle_api_request("https://apps.banckle.com/em/api/departments.xml?_token=" . banckle_api_token())) {
    $response = new SimpleXMLElement(utf8_encode($response));
    foreach ($response->department as $d) {
      $departments[strval($d->id)] = strval($d->displayName);
    }
  }
  else {
    drupal_set_message(t("Cannot communicate to server. Please try again later"), "error");
    return;
  }

  $form["banckle_live_chat_admin_customize"] = array();

  $form["banckle_live_chat_admin_customize"]["basic_info"] = array(
    "#type" => "fieldset",
    "#title" => "Basic Information",
    "#description" => "",
  );

  $form["banckle_live_chat_admin_customize"]["basic_info"]["name"] = array(
    "#type" => "textfield",
    "#title" => "Name",
    "#description" => "",
    "#default_value" => $deployment->name,
  );

  $form["banckle_live_chat_admin_customize"]["basic_info"]["title"] = array(
    "#type" => "textfield",
    "#title" => "Title",
    '#maxlength' => 32,
    "#description" => "",
    "#default_value" => $deployment->title,
  );

  $form["banckle_live_chat_admin_customize"]["basic_info"]["copyright"] = array(
    "#type" => "textfield",
    "#title" => "Copyright",
    "#description" => "",
    "#default_value" => $deployment->copyright,
  );

  $form["banckle_live_chat_admin_customize"]["basic_info"]["departments"] = array(
    "#type" => "select",
    "#title" => "Departments",
    "#description" => "",
    "#multiple" => TRUE,
    "#options" => $departments,
    "#default_value" => explode(";", $deployment->departments),
  );

  $form["banckle_live_chat_admin_customize"]["greeting_messages"] = array(
    "#type" => "fieldset",
    "#title" => "Greeting messages",
    "#description" => "Use {survey} to set a hyperlink to the survey.",
  );

  $form["banckle_live_chat_admin_customize"]["greeting_messages"]["welcome_message"] = array(
    "#type" => "textarea",
    "#rows" => 2,
    "#title" => "Online",
    "#description" => "",
    "#default_value" => $deployment->welcomeMessage,
  );

  $form["banckle_live_chat_admin_customize"]["greeting_messages"]["unavailable_message"] = array(
    "#type" => "textarea",
    "#rows" => 2,
    "#title" => "Offline",
    "#description" => "",
    "#default_value" => $deployment->unavailableMessage,
  );

  $form["banckle_live_chat_admin_customize"]["greeting_messages"]["waiting_message"] = array(
    "#type" => "textarea",
    "#rows" => 2,
    "#title" => "Waiting",
    "#description" => "",
    "#default_value" => $deployment->waitingMessage,
  );

  $form["banckle_live_chat_admin_customize"]["greeting_messages"]["final_message"] = array(
    "#type" => "textarea",
    "#rows" => 2,
    "#title" => "Exit",
    "#description" => "",
    "#default_value" => $deployment->finalMessage,
  );

  $form["banckle_live_chat_admin_customize"]["greeting_messages"]["invite_message"] = array(
    "#type" => "textarea",
    "#rows" => 2,
    "#title" => "Invite",
    "#description" => "",
    "#default_value" => $deployment->inviteMessage,
  );

  $form["banckle_live_chat_admin_customize"]["visitor_invitation"] = array(
    "#type" => "fieldset",
    "#title" => "Visitor Invitation",
    "#description" => "",
  );

  $form["banckle_live_chat_admin_customize"]["visitor_invitation"]["enable_auto_invite"] = array(
    "#type" => "checkbox",
    "#title" => "Enable auto invitation",
    "#description" => "",
    "#default_value" => $deployment->enableAutoInvite == "true",
  );

  $form["banckle_live_chat_admin_customize"]["visitor_invitation"]["invite_timeout"] = array(
    "#type" => "select",
    "#title" => "Automatically invite visitors staying on site for more than",
    "#description" => "",
    "#options" => array(
      "15" => "15 seconds",
      "30" => "30 seconds",
      "45" => "45 seconds",
      "60" => "1 minute",
      "300" => "5 minute",
      "600" => "10 minute",
      "1800" => "30 minute",
    ),
    "#multiple" => FALSE,
    "#default_value" => $deployment->inviteTimeout,
  );

  $form["banckle_live_chat_admin_customize"]["visitor_invitation"]["enable_proactive_invite"] = array(
    "#type" => "checkbox",
    "#title" => "Proactively invite visitors",
    "#description" => "",
    "#default_value" => $deployment->enableProactiveInvite == "true",
  );

  $form["banckle_live_chat_admin_customize"]["visitor_invitation"]["enable_invitation_filter"] = array(
    "#type" => "checkbox",
    "#title" => "Enable page specific invitations (Automatic & Proactive)",
    "#description" => "",
    "#default_value" => $deployment->enableInvitationFilter == "true",
  );

  return system_settings_form($form);
}

/**
 * Form validation handler for the customize form.
 */
function _form_banckle_live_chat_admin_customize_validate($form, &$form_state) {
  $request = "
<deployment>
  <name>" . check_plain($form_state["values"]["name"]) . "</name>
  <title>" . $form_state["values"]["title"] . "</title>
  <copyright>" . $form_state["values"]["copyright"] . "</copyright>
  <welcomeMessage>" . check_plain($form_state["values"]["welcome_message"]) . "</welcomeMessage>
  <waitingMessage>" . check_plain($form_state["values"]["waiting_message"]) . "</waitingMessage>
  <unavailableMessage>" . check_plain($form_state["values"]["unavailable_message"]) . "</unavailableMessage>
  <finalMessage>" . check_plain($form_state["values"]["final_message"]) . "</finalMessage>
  <inviteMessage>" . check_plain($form_state["values"]["invite_message"]) . "</inviteMessage>
  <departments>" . implode(";", array_keys($form_state["values"]["departments"])) . "</departments>
  <exitSurvey />
  <enableAutoInvite>" . ($form_state["values"]["enable_auto_invite"] ? "true" : "false") . "</enableAutoInvite>
  <inviteTimeout>" . $form_state["values"]["invite_timeout"] . "</inviteTimeout>
  <autoInviteImage />
  <enableProactiveInvite>" . ($form_state["values"]["enable_proactive_invite"] ? "true" : "false") . "</enableProactiveInvite>
  <proactiveInviteImage></proactiveInviteImage>
  <enableInvitationFilter>" . ($form_state["values"]["enable_invitation_filter"] ? "true" : "false") . "</enableInvitationFilter>
  <invitationFilterType>0</invitationFilterType>
  <theme>theme-4</theme>
  <linkType>0</linkType>
  <themeFlags>0</themeFlags>
  <creationDate>0</creationDate>
</deployment>
    ";

  $data = tmpfile();
  fwrite($data, $request);
  fseek($data, 0);

  $ch = curl_init();
  curl_setopt($ch, CURLOPT_VERBOSE, 1);
  curl_setopt($ch, CURLOPT_URL, "https://apps.banckle.com/em/api/deployments/" . variable_get("banckle_live_chat_deployment") . ".xml?_token=" . banckle_api_token());
  curl_setopt($ch, CURLOPT_PUT, 1);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
  curl_setopt($ch, CURLOPT_INFILE, $data);
  curl_setopt($ch, CURLOPT_INFILESIZE, drupal_strlen($request));

  $http_result = curl_exec($ch);
  $error = curl_error($ch);
  $http_code = curl_getinfo($ch, CURLINFO_HTTP_CODE);

  fclose($data);
  curl_close($ch);
}
