<?php

/**
 * @file
 * Callback for admin/config/system/banckle/livechat/deactivate
 */

/**
 * Form builder for the deactivate form.
 *
 * @see banckle_live_chat_menu()
 *
 * @ingroup forms
 */
function _form_banckle_live_chat_admin_deactivate($form, &$form_state) {
  if (!variable_get("banckle_live_chat_deployment")) {
    drupal_set_message(t("Banckle Chat for Drupal has not been activated."));
    return;
  }

  drupal_set_message(t("This will deactivate Banckle Chat on your Drupal website. Click <strong>deactivate</strong> to confirm.", array("deactivate" => t("Deactivate"))), "warning", FALSE);

  $form["banckle_live_chat_admin_deactivate"] = array(
    "#type" => "submit",
    "#value" => "Deactivate",
  );

  return $form;
}

/**
 * Form submission handler for the deactivate form.
 */
function _form_banckle_live_chat_admin_deactivate_submit($form, &$form_state) {
  variable_del("banckle_live_chat_username");
  variable_del("banckle_live_chat_password");
  variable_del("banckle_live_chat_deployment");

  drupal_get_messages();
  drupal_set_message(t("Banckle Chat has been successfully deactivated on your Drupal website."));

  if (!isset($form_state["redirect"]) || !$form_state["redirect"]) {
    $form_state["redirect"] = "admin/config/system/banckle/livechat";
  }

  drupal_flush_all_caches();
}
