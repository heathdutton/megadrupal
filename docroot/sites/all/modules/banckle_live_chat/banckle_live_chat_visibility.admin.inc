<?php

/**
 * @file
 * Callback for admin/config/system/banckle/livechat/visibility
 */

/**
 * Form builder for the visibility form.
 *
 * @see banckle_live_chat_menu()
 *
 * @ingroup forms
 */
function _form_banckle_live_chat_admin_visibility($form, &$form_state) {
  $form["banckle_live_chat_admin_position"] = array(
    "#type" => "select",
    "#title" => "Position",
    "#description" => "",
    "#multiple" => FALSE,
    "#options" => array("top-left" => "Top left", "top-right" => "Top right", "bottom-left" => "Bottom left", "bottom-right" => "Bottom right"),
    "#default_value" => variable_get("banckle_live_chat_admin_position", "bottom-right"),
  );

  $form["banckle_live_chat_admin_user"] = array(
    "#type" => "checkbox",
    "#title" => "Allow user to hide Banckle Chat button for them",
    "#default_value" => variable_get("banckle_live_chat_admin_user", 0),
  );

  $form["banckle_live_chat_admin_pages"] = array(
    "#type" => "textarea",
    "#title" => "Exclude pages",
    "#description" => "Mention pages on which the Banckle Chat button is not visible.",
    "#default_value" => variable_get("banckle_live_chat_admin_pages", ""),
  );
  
  $form["banckle_live_chat_include_pages"] = array(
    "#type" => "textarea",
    "#title" => "Include pages",
    "#description" => "Mention pages on which the Banckle Chat button is visible.",
    "#default_value" => variable_get("banckle_live_chat_include_pages", ""),
  );
  
  // Get all themes
  $themes = list_themes();
  $themes_list = array('Select');
  foreach ($themes AS $theme_name => $theme)
  {
   	if (drupal_theme_access($theme))
    {
		$themes_list[] = $theme_name;
	}	
  }
  
  $form["banckle_live_chat_exclude_theme"] = array(
    "#type" => "select",
    "#title" => "Exclude theme",
	"#options" => drupal_map_assoc($themes_list),
    "#description" => "Mention theme on which the Banckle Chat button is not visible.",
    "#default_value" => variable_get("banckle_live_chat_exclude_theme", ""),
  );

  return system_settings_form($form);
}

/**
 * Form submission handler for the visibility form.
 */
function _form_banckle_live_chat_admin_visibility_submit($form, &$form_state) {
  variable_set("banckle_live_chat_admin_user", intval($form_state["values"]["banckle_live_chat_admin_user"]));
  variable_set("banckle_live_chat_admin_pages", $form_state["values"]["banckle_live_chat_admin_pages"]);
  variable_set("banckle_live_chat_admin_position", $form_state["values"]["banckle_live_chat_admin_position"]);
  variable_set("banckle_live_chat_exclude_theme", $form_state["values"]["banckle_live_chat_exclude_theme"]);
  variable_set("banckle_live_chat_include_pages", $form_state["values"]["banckle_live_chat_include_pages"]);
}
