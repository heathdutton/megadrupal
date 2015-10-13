<?php
// $Id$
/**
 * @file
 * Implements the default action settings display theme
 *
 * Incoming variables:  $action_settings, an array of settings.
 */

?>
<div class="hotfolder_action_settings">
<?php
  $human_name = 'No action selected';
  $description = '';
  $settings = array();
  if (!empty($action_settings)) {
    $human_name = isset($action_settings['human_name']) ? $action_settings['human_name'] : '';
    $description = isset($action_settings['description']) ? $action_settings['description'] : '';
    $settings = isset($action_settings['settings']) ? $action_settings['settings'] : array();
  }
  $result = '';
  $result .= '<div>' . $human_name . '</div>';
  $result .= '<div><i>' . $description . '</i></div>';
  $result .= '<ul>';
  foreach ($settings as $setting_name => $setting_values) {
    $result .= '<li>' . $setting_values['human_name'] . ' : ' . $setting_values['value'] . '</li>';
  }
  $result .= '</ul>';
  print $result;
?>
</div>
