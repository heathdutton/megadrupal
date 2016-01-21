<?php
// $Id: theme-settings.php,v 1.17 2010/11/27 01:36:29 shannonlucas Exp $
/**
 * @file
 * Provides the settings for the Nitobe theme.
 */

require_once drupal_get_path("theme", "nitobe") . "/nitobe.utils.inc";

/**
 * Exposes Nitobe's theme settings.
 *
 * @param array &$form
 *   Nested array of form elements that comprise the form.
 * @param array &$form_state
 *   A keyed array containing the current state of the form. The arguments that
 *   drupal_get_form() was originally called with are available in the array
 *   $form_state['build_info']['args'].
 */
function nitobe_form_system_theme_settings_alter(&$form, &$form_state) {
  $form["nitobe_general_settings"] = array(
    "#type"  => "fieldset",
    "#title" => "General Nitobe settings",
  );

  // -- What ordering should be used for the content and sidebars?
  $default = theme_get_setting("nitobe_content_placement", "nitobe");
  $desc    = t("Where should the sidebars be placed?");
  $options = array(
    "center" => "On either side of the content region.",
    "right"  => "Right of the content region.",
    "left"   => "Left of the content region.",
  );

  $form["nitobe_general_settings"]['nitobe_content_placement'] = array(
    "#type"          => "select",
    "#title"         => t("Sidebar placement"),
    "#options"       => $options,
    "#default_value" => $default,
    "#description"   => $desc,
  );

  // -- How many page items to put in the pager widgets.
  $default = theme_get_setting("nitobe_pager_page_count", "nitobe");
  $options = range(3, 10);
  $desc    = t("The number of default pages to include in pager controls. If you are using a three column layout, a lower number here will work better.");

  $form["nitobe_general_settings"]['nitobe_pager_page_count'] = array(
    "#type"           => 'select',
    "#title"          => t('Pager item count'),
    "#options"        => drupal_map_assoc($options),
    "#default_value"  => $default,
    "#description"    => $desc,
  );
  
  _nitobe_add_header_settings($form);
  _nitobe_add_time_format_settings($form);
}


/**
 * Adds the date/time format settings to the settings form.
 *
 * @param array &$form
 *   Nested array of form elements that comprise the form.
 */
function _nitobe_add_time_format_settings(&$form) {
  $options = array(
    "attributes" => array("target" => "_blank"),
    "absolute"   => TRUE,
    "external"   => TRUE,
  );
  
  $subs = array(
    "!link" => l(t("date()"), "http://php.net/manual/en/function.date.php", 
                 $options),
  );

  $desc = t("These settings are used to control how dates are rendered on nodes and comments. Refer to the documentation for PHP's !link function for details on date formats.",
            $subs);
  $form["nitobe_datetime_settings"] = array(
    "#type"  => "fieldset",
    "#title" => "Date and time formats",
    "#description" => $desc,
  );
  
  // -- The node timestamp format
  $default = theme_get_setting("nitobe_node_datestamp_format", "nitobe");
  $desc = t("This controls how dates are rendered in a node's header.");
  $form["nitobe_datetime_settings"]["nitobe_node_datestamp_format"] = array(
    "#type"         => "textfield",
    "#title"       => t("Node date format"),
    "#size"        => 32,
    "#max_length"  => 64,
    "#description" => $desc,
    "#default_value" => $default,
  );

  // -- The comment attribution date format
  $default = theme_get_setting("nitobe_comment_date_format", "nitobe");
  $desc = t("This controls how a comment's date is rendered in the author attribution text.");
  $form["nitobe_datetime_settings"]["nitobe_comment_date_format"] = array(
    "#type"        => "textfield",
    "#title"       => t("Comment date format"),
    "#size"        => 32,
    "#max_length"  => 64,
    "#description" => $desc,
    "#default_value" => $default,
  );
  
  // -- The comment attribution time format
  $default = theme_get_setting("nitobe_comment_time_format", "nitobe");
  $desc = t("This controls how a comment's time is rendered in the author attribution text.");
  $form["nitobe_datetime_settings"]["nitobe_comment_time_format"] = array(
    "#type"        => "textfield",
    "#title"       => t("Comment date format"),
    "#size"        => 32,
    "#max_length"  => 64,
    "#description" => $desc,
    "#default_value" => $default,
  );
}


/**
 * Adds the header settings to the settings form.
 *
 * @param array $form
 *   The form element array to add the settings fields to.
 */
function _nitobe_add_header_settings(&$form) {
  $form["nitobe_header_settings"] = array(
    "#type"  => "fieldset",
    "#title" => "Page header settings",
  );

  // -- Should the alternating color title effect be applied?
  $default = theme_get_setting("nitobe_title_effect", "nitobe");
  $desc    = t("Should the title be adjusted to apply an alternate color to every other word and remove inter-word spacing?");
  $form["nitobe_header_settings"]["nitobe_title_effect"] = array(
    "#type"           => "checkbox",
    "#title"          => t("Apply title effect"),
    "#default_value"  => $default,
    "#description"    => $desc,
  );

  // -- Get the header image list.
  $images  = _nitobe_get_header_list(TRUE);
  $options = array("<random>" => "<Random Header Image>");

  foreach ($images as $filename => $data) {
    $options[$filename] = $data->pretty_name;
  }

  // -- The setting for the header image.
  $current = theme_get_setting("nitobe_header_image", "nitobe");
  $default = in_array($current, array_keys($options)) ? $current : "<random>";
  $form["nitobe_header_settings"]["nitobe_header_image"] = array(
    "#type"           => "select",
    "#title"          => t("Header image"),
    "#options"        => $options,
    "#default_value"  => $default,
  );

  // -- Show the header image if the mastead region has content?
  $default = theme_get_setting("nitobe_masthead_always_show", "nitobe");
  $desc    = t("By default, if there is block content in the Masthead region, the header image will not be displayed. Check this box to cause the header image to be displayed as the region's background image.");
  $form["nitobe_header_settings"]["nitobe_masthead_always_show"] = array(
    "#type"           => "checkbox",
    "#title"          => t("Always show masthead image"),
    "#default_value"  => $default,
    "#description"    => $desc,
  );
}
