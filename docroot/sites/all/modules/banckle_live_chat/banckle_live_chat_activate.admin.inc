<?php

/**
 * @file
 * Callback for admin/config/system/banckle/livechat/activate
 */

require_once "banckle_api.php";

/**
 * Form builder for the activate form.
 *
 * @see banckle_live_chat_menu()
 *
 * @ingroup forms
 */
function _form_banckle_live_chat_admin_activate($form, &$form_state) {
  if (empty($form_state["step"])) {
    $form_state["step"] = 1;
    $form_state["next_step"] = 0;
    $form_state["step_information"] = array(
      1 => array(
        "title" => "Account",
        "form" => "_form_banckle_live_chat_admin_activate_step1",
      ),
      2 => array(
        "title" => "Signup",
        "form" => "_form_banckle_live_chat_admin_activate_step2",
      ),
      3 => array(
        "title" => "Signin",
        "form" => "_form_banckle_live_chat_admin_activate_step3",
      ),
      4 => array(
        "title" => "Create Department",
        "form" => "_form_banckle_live_chat_admin_activate_step4",
      ),
      5 => array(
        "title" => "Create Deployment",
        "form" => "_form_banckle_live_chat_admin_activate_step5",
      ),
      6 => array(
        "title" => "Select Deployment",
        "form" => "_form_banckle_live_chat_admin_activate_step6",
      ),
      7 => array(
        "title" => "Finish",
        "form" => "_form_banckle_live_chat_admin_activate_step7",
      ),
    );
    $form_state["last_step"] = count($form_state["step_information"]);
  }

  $step = &$form_state["step"];
  $form = $form_state["step_information"][$step]["form"]($form, $form_state);

  if ($step < count($form_state["step_information"])) {
    $form['next'] = array(
      '#type' => 'submit',
      '#value' => t("Next"),
      '#name' => "next",
      '#submit' => array('_form_banckle_live_chat_admin_activate_next'),
    );
  }
  else {
    $form['finish'] = array(
      '#type' => 'submit',
      '#value' => t('Finish'),
    );
  }

  if (function_exists($form_state['step_information'][$step]['form'] . '_validate')) {
    $form['next']['#validate'] = array($form_state['step_information'][$step]['form'] . '_validate');
  }

  drupal_set_title(t("Activate: subtitle", array("subtitle" => $form_state["step_information"][$step]["title"])), PASS_THROUGH);

  return $form;
}

/**
 * Form submission handler for next button of activate form.
 */
function _form_banckle_live_chat_admin_activate_next($form, &$form_state) {
  $step = &$form_state["step"];
  $form_state["step_information"][$step]["stored_values"] = $form_state["values"];

  if ($step < count($form_state["step_information"])) {
    if ($form_state["next_step"] > 0) {
      $step = $form_state["next_step"];
    }
    else {
      $step++;
    }

    if (!empty($form_state["step_information"][$step]["stored_values"])) {
      $form_state["values"] = $form_state["step_information"][$step]["stored_values"];
    }
    else {
      $form_state["values"] = array();
    }

    $form_state["rebuild"] = TRUE;
  }

  $form_state["next_step"] = 0;
  drupal_set_title(t("Activate: subtitle", array("subtitle" => $form_state["step_information"][$step]["title"])), PASS_THROUGH);
}

/**
 * Form builder for the step 1 of activate form.
 */
function _form_banckle_live_chat_admin_activate_step1($form, &$form_state) {
  return array(
    "initialize" => array(
      "#type" => "radios",
      "#options" => array("signin" => t("I want to use an existing Banckle account"), "signup" => t("I want to create a new Banckle account")),
    ),
  );
}

/**
 * Form validation handler for the step 1 of activate form.
 */
function _form_banckle_live_chat_admin_activate_step1_validate($form, &$form_state) {
  switch (check_plain($form_state["values"]["initialize"])) {
    case "signup":
      $form_state["next_step"] = 2;
      break;
    case "signin":
      $form_state["next_step"] = 3;
      break;
    default:
      form_set_error("initialize", t("Please select a valid option"));
      return;
  }

  variable_del("banckle_live_chat_username");
  variable_del("banckle_live_chat_password");
  variable_del("banckle_live_chat_deployment");
}

/**
 * Form builder for the step 2 of activate form.
 */
function _form_banckle_live_chat_admin_activate_step2($form, &$form_state) {
  return array(
    "username" => array(
      "#type" => "textfield",
      "#title" => t("Banckle Login ID"),
      "#description" => t("You will use login ID to sign into your Banckle account or configure Banckle marketplace apps. Please make sure that there are no spaces and it has atleast 6 characters."),
      "#required" => TRUE,
      '#maxlength' => 99,
    ),
    "email" => array(
      "#type" => "textfield",
      "#title" => t("Email Address"),
      "#description" => t("Your email address will be used to send you authentication email. So please make sure that you provide a valid email address. We never share or disclose your email address to any third party."),
      "#required" => TRUE,
    ),
    "password" => array(
      "#type" => "password",
      "#title" => t("Password"),
      "#required" => TRUE,
    ),
    "confirm_password" => array(
      "#type" => "password",
      "#title" => t("Confirm Password"),
      "#required" => TRUE,
    ),
    "domain" => array(
      "#type" => "textfield",
      "#title" => t("Domain Name"),
      "#description" => t("It is mandatory to provide a valid domain name (e.g. www.example.com) in order to configure your Banckle account. If you dont have a domain you may simply write your \"username.tmp\". For example if your username is \"albert\" you will provide Domain Name as \"albert.tmp\". "),
      "#required" => TRUE,
    ),
  );
}

/**
 * Form validation handler for the step 2 of activate form.
 */
function _form_banckle_live_chat_admin_activate_step2_validate($form, &$form_state) {
  $username = trim(check_plain($form_state["values"]["username"]));
  if ($username != $form_state["values"]["username"]) {
    form_set_error("username", t("Invalid characters found in Banckle Login ID."));
    return;
  }

  if (drupal_strlen($username) < 6) {
    form_set_error("username", t("Username is too short, minimum 6 characters are required"));
    return;
  }

  $password = check_plain($form_state["values"]["password"]);
  if ($password != $form_state["values"]["password"]) {
    form_set_error("password", t("Invalid characters found in Password."));
    return;
  }

  if (drupal_strlen($password) < 8) {
    form_set_error("password", t("Password is too short, minimum 8 characters are required"));
    return;
  }

  $email = $form_state["values"]["email"];
  if (!valid_email_address($email)) {
    form_set_error("email", t("Invalid email address."));
    return;
  }

  $domain = $form_state["values"]["domain"];
  if (!dns_get_record($domain)) {
    form_set_error("domain", t("Invalid domain name."));
    return;
  }

  if ($password != check_plain($form_state["values"]["confirm_password"])) {
    form_set_error("password");
    form_set_error("confirm_password");
    drupal_set_message(t("Password and Confirm Password must have same value."), "error");
    return;
  }

  $signup = banckle_api_signup($username, $password, $email, $domain);
  if (is_array($signup)) {
    switch ($signup["error"]) {
      case 3001:
        form_set_error("username", filter_xss($signup["message"]));
        break;
      case 3002:
        form_set_error("email", filter_xss($signup["message"]));
        break;
      case 4002:
        form_set_error("domain", filter_xss($signup["message"]));
        break;
      case 1002:
        form_set_error("domain", filter_xss($signup["message"]));
        break;
      default:
        form_set_error("", filter_xss($signup["message"]));
        break;
    }

    return;
  }

  variable_set("banckle_live_chat_username", $username);
  variable_set("banckle_live_chat_password", $password);
  banckle_api_authenticate($username, $password);
  $form_state["next_step"] = 4;
}

/**
 * Form builder for the step 3 of activate form.
 */
function _form_banckle_live_chat_admin_activate_step3($form, &$form_state) {
  return array(
    "username" => array(
      "#type" => "textfield",
      "#title" => t("Banckle Login ID"),
      "#description" => t("Please specify your Banckle account with admin rights."),
      "#required" => TRUE,
    ),
    "password" => array(
      "#type" => "password",
      "#title" => t("Password"),
      "#required" => TRUE,
    ),
  );
}

/**
 * Form validation handler for the step 3 of activate form.
 */
function _form_banckle_live_chat_admin_activate_step3_validate($form, &$form_state) {
  $username = check_plain($form_state["values"]["username"]);
  $password = check_plain($form_state["values"]["password"]);
  banckle_api_authenticate($username, $password);
  $token = banckle_api_token();

  if (is_array($token)) {
    switch ($token["error"]) {
      case 2001:
        form_set_error("username", $token["message"]);
        break;
      case 2002:
        form_set_error("password", $token["message"]);
        break;
      default:
        form_set_error("username");
        form_set_error("password");
        drupal_set_message($token["message"], "error");
        break;
    }

    return;
  }

  variable_set("banckle_live_chat_username", $username);
  variable_set("banckle_live_chat_password", $password);

  if (count(banckle_api_get_departments())) {
    if (count(banckle_api_get_deployments())) {
      $form_state["next_step"] = 6;
    }
    else {
      $form_state["next_step"] = 5;
    }
  }
  else {
    $form_state["next_step"] = 4;
  }
}

/**
 * Form builder for the step 4 of activate form.
 */
function _form_banckle_live_chat_admin_activate_step4($form, &$form_state) {
  return array(
    "department_name" => array(
      "#type" => "textfield",
      "#title" => t("Department Name"),
      "#required" => TRUE,
      "#default_value" => "NewDepartment",
    ),
  );
}

/**
 * Form validation handler for the step 4 of activate form.
 */
function _form_banckle_live_chat_admin_activate_step4_validate($form, &$form_state) {
  $name = check_plain($form_state["values"]["department_name"]);
  $department = banckle_api_add_department(array(
        "displayName" => $name,
        "members" => array(
          variable_get("banckle_live_chat_username")
        ),
      ));

  if (!$department && !isset($department["id"])) {
    form_set_error("name", t("The department was not created, please try again."));
    return;
  }

  variable_set("banckle_live_chat_department", $department["id"]);

  $form_state["next_step"] = 5;
}

/**
 * Form builder for the step 5 of activate form.
 */
function _form_banckle_live_chat_admin_activate_step5($form, &$form_state) {
  return array(
    "basic_info" => array(
      "#type" => "fieldset",
      "#title" => "Basic Information",
      "deployment_name" => array(
        "#type" => "textfield",
        "#title" => "Name",
        "#description" => "",
        "#default_value" => "New Deployment",
        "#required" => TRUE,
      ),
      "title" => array(
        "#type" => "textfield",
        '#maxlength' => 32,
        "#title" => "Title",
        "#description" => "",
        "#default_value" => "Live Chat",
        "#required" => TRUE,
      ),
      "copyright" => array(
        "#type" => "textfield",
        "#title" => "Copyright",
        "#default_value" => "Copyright is reserved by Banckle.",
        "#description" => "",
      ),
      "departments" => array(
        "#type" => "select",
        "#title" => "Departments",
        "#description" => "",
        "#required" => TRUE,
        "#multiple" => TRUE,
        "#options" => banckle_api_get_departments(),
        "#default_value" => variable_get("banckle_live_chat_department"),
      ),
    ),
    "greeting_messages" => array(
      "#type" => "fieldset",
      "#title" => "Greeting messages",
      "#description" => "Use {survey} to set a hyperlink to the survey.",
      "welcome_message" => array(
        "#type" => "textarea",
        "#rows" => 2,
        "#title" => "Online",
        "#description" => "",
        "#default_value" => "",
      ),
      "unavailable_message" => array(
        "#type" => "textarea",
        "#rows" => 2,
        "#title" => "Offline",
        "#description" => "",
        "#default_value" => "",
      ),
      "waiting_message" => array(
        "#type" => "textarea",
        "#rows" => 2,
        "#title" => "Waiting",
        "#description" => "",
        "#default_value" => "",
      ),
      "final_message" => array(
        "#type" => "textarea",
        "#rows" => 2,
        "#title" => "Exit",
        "#description" => "",
        "#default_value" => "",
      ),
      "invite_message" => array(
        "#type" => "textarea",
        "#rows" => 2,
        "#title" => "Invite",
        "#description" => "",
        "#default_value" => "",
      ),
    ),
    "visitor_invitation" => array(
      "#type" => "fieldset",
      "#title" => "Visitor Invitation",
      "enable_auto_invite" => array(
        "#type" => "checkbox",
        "#title" => "Enable auto invitation",
        "#description" => "",
      ),
      "invite_timeout" => array(
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
      ),
      "enable_proactive_invite" => array(
        "#type" => "checkbox",
        "#title" => "Proactively invite visitors",
        "#description" => "",
        "#default_value" => TRUE,
      ),
      "enable_invitation_filter" => array(
        "#type" => "checkbox",
        "#title" => "Enable page specific invitations (Automatic & Proactive)",
        "#description" => "",
      ),
    ),
  );
}

/**
 * Form validation handler for the step 5 of activate form.
 */
function _form_banckle_live_chat_admin_activate_step5_validate($form, &$form_state) {
  if (!count($form_state["values"]["departments"])) {
    form_set_error("departments", "Please select atleast one department.");
  }

  $attributes = array();
  foreach ($form_state["values"] as $k => $v) {
    if (is_string($v)) {
      $attributes[$k] = check_plain($v);
    }
  }

  if (banckle_api_add_deployment($form_state["values"]) == FALSE) {
    form_set_error("", t("The deployment was not created, please try again."));
    return;
  }

  $deployments = array_keys(banckle_api_get_deployments());

  if (count($deployments) > 0) {
    variable_set("banckle_live_chat_deployment", $deployments[0]);
    $form_state["next_step"] = 7;
  }
  else {
    form_set_error("", t("The deployment was not created, please try again."));
  }
}

/**
 * Form builder for the step 6 of activate form.
 */
function _form_banckle_live_chat_admin_activate_step6($form, &$form_state) {
  return array(
    "banckle_live_chat_deployment" => array(
      "#type" => "select",
      "#title" => "Select Deployment",
      "#required" => TRUE,
      "#multiple" => FALSE,
      "#options" => banckle_api_get_deployments(),
    ),
  );
}

/**
 * Form validation handler for the step 6 of activate form.
 */
function _form_banckle_live_chat_admin_activate_step6_validate($form, &$form_state) {
  if (!$form_state["values"]["banckle_live_chat_deployment"]) {
    form_set_error("banckle_live_chat_deployment", t("You must select a deployment"));
  }

  variable_set("banckle_live_chat_deployment", $form_state["values"]["banckle_live_chat_deployment"]);
  $form_state["next_step"] = 7;
}

/**
 * Form builder for the step 7 of activate form.
 */
function _form_banckle_live_chat_admin_activate_step7($form, &$form_state) {
  drupal_set_message(t("Banckle Chat has been activated on your Drupal website. Click <strong>finish</strong> to complete the process.", array("finish" => t("Finish"))), "status", FALSE);
  return array();
}

/**
 * Form validation handler for the step 7 of activate form.
 */
function _form_banckle_live_chat_admin_activate_step7_validate($form, &$form_state) {

}

/**
 * Form submission handler for the activate form.
 */
function _form_banckle_live_chat_admin_activate_submit($form, &$form_state) {
  if (!isset($form_state["redirect"]) || !$form_state["redirect"]) {
    $form_state["redirect"] = "admin/config/system/banckle/livechat";
  }

  drupal_get_messages();
  drupal_flush_all_caches();
}
