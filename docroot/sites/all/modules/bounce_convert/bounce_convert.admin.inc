<?php

/**
 * @file
 * All functions within this file are related to admin
 */

/**
 * All functions within this file need the bellow files.
 */
define('BOUNCE_CONVERT_CONTENT_TYPE', 'bounce_convert_campaign');

/**
 * Return all publish webforms.
 * @return array
 *   Node type "webform".
 */
function bounce_convert_webforms() {
  $nodes = node_load_multiple(array(), array(
    'type' => 'webform',
    'status' => 1,
  ));
  $webforms = array();
  // @todo: This needs validation callback to make sure the user has selected
  // a valid weboform, and not left the default '- Select a value -'.
  $webforms[0] = '- Select a value -';
  foreach ($nodes as $nid => $node) {
    $webforms[$nid] = check_plain($node->title);
  }
  return $webforms;
}

/**
 * Return campagins per uesr role type.
 *
 * @param array $user_rids
 *   Role ids of login user.
 *
 * @return bool
 *   Whether to show or not the campaign.
 */
function bounce_convert_check_campaigns($user_rids) {
  // Default value to not show the modal.
  $bounce_convert_show_moadal = FALSE;

  // Get role ids instead of role names.
  $user_rids = array_keys($user_rids);

  // Fetch only published and active campagins(ie:status = 1 and active = yes).
  $query = new EntityFieldQuery();
  $result = $query
      ->entityCondition('entity_type', 'node')
      ->propertyCondition('status', 1)
      ->fieldCondition('field_bcc_active', 'value', 'Yes', '=')
      ->execute();

  // Check if campaign exists.
  if (!empty($result)) {
    foreach ($result['node'] as $obj_node) {
      $node = node_load($obj_node->nid, $obj_node->vid);
      // Filter campaigns by user role
      // If user role matches with campaign role, show modal ELSE don't
      if (!bounce_convert_check_user_role($node, $user_rids)) {
        continue;
      }

      /*
       * Check for: User selected option from bellow options:
       * 0 - show modal on all pages except bellow.
       * 1 - show modal only on bellow listed pages.
       * 2 - show modal only if PHP returns TRUE.
       * NULL - mean show on all pages (site wide).
       */
      $bounce_convert_pages_option = field_get_items('node', $node, 'field_bcc_pages_option');
      // Check if not NULL, at least one option is selected.
      if (!empty($bounce_convert_pages_option)) {
        // Get user selected option.
        $bounce_convert_pages_option = $bounce_convert_pages_option[0]['value'];
        // Fetch path list.
        $paths = bounce_convert_path_list($node, $bounce_convert_pages_option);
        $bounce_convert_show_moadal = bounce_convert_check_page_path($paths, $bounce_convert_pages_option);
        if ($bounce_convert_show_moadal) {
          $properties = bounce_convert_get_webform_properties($node);
          return $properties;
        }
      }
      else {
        // Show modal on all pages if no option is selected.
        return bounce_convert_get_webform_properties($node);
      }
    }
  }
  return FALSE;
}

/**
 * Return a list of paths.
 *
 * @param object $node
 *   Contains information about campaign.
 *
 * @return array
 *   list of path/pages for particular campaign.
 */
function bounce_convert_path_list($node, $bounce_convert_pages_option) {

  $bounc_convert_paths = array();

  /*
   * List of pages by using their paths. Entered one path per line by admin.
   * Creating array of path by preg_replace and explode.
   */
  if (!empty($node->field_bcc_pages_list)) {
    // If not PHP code Evaluate then Create array of paths.
    if ($bounce_convert_pages_option != 2) {
      $bounce_convert_field_visibility_settings = field_get_items('node', $node, 'field_bcc_pages_list');
      $bounce_convert_field_visibility_settings = $bounce_convert_field_visibility_settings[0]['safe_value'];

      $bounce_convert_field_visibility_settings = preg_replace('#\s+#', ',', trim($bounce_convert_field_visibility_settings));
      $bounc_convert_paths = explode(',', $bounce_convert_field_visibility_settings);
    }
    else {
      // Return PHP code as it is.
      $bounce_convert_field_visibility_settings = field_get_items('node', $node, 'field_bcc_pages_list');
      $bounc_convert_paths = $bounce_convert_field_visibility_settings[0]['safe_value'];
    }
  }
  return $bounc_convert_paths;
}

/**
 * Fetch node object properties.
 *
 * @param object $node
 *   Contains information about campaign.
 *
 * @return array
 *   Webform properties.
 */
function bounce_convert_get_webform_properties($node) {
  $campaign_node_id = $node->nid;

  $bounce_convert_webfrom_nid = field_get_items('node', $node, 'field_bcc_webform');
  $bounce_convert_webfrom_nid = $bounce_convert_webfrom_nid[0]['value'];

  $bounce_convert_cookie_expiration = field_get_items('node', $node, 'field_bcc_cookie_expiration');
  $bounce_convert_cookie_expiration = $bounce_convert_cookie_expiration[0]['value'];

  $field_bcc_background_color = field_get_items('node', $node, 'field_bcc_background_color');
  $field_bcc_background_color = isset($field_bcc_background_color[0]['value']) ? $field_bcc_background_color[0]['value'] : '#000';

  $field_bcc_background_opacity = field_get_items('node', $node, 'field_bcc_background_opacity');
  $field_bcc_background_opacity = isset($field_bcc_background_opacity[0]['value']) ? $field_bcc_background_opacity[0]['value'] : 0.5;

  $field_bcc_modal_width = field_get_items('node', $node, 'field_bcc_modal_width');
  $field_bcc_modal_width = isset($field_bcc_modal_width[0]['value']) ? (int) $field_bcc_modal_width[0]['value'] : 800;

  $field_bcc_modal_height = field_get_items('node', $node, 'field_bcc_modal_height');
  $field_bcc_modal_height = isset($field_bcc_modal_height[0]['value']) ? (int) $field_bcc_modal_height[0]['value'] : 500;

  $data['campaign_node_id'] = $campaign_node_id;
  $data['webform_id'] = $bounce_convert_webfrom_nid;
  $data['campaign_visit_cookie'] = $bounce_convert_cookie_expiration;
  $data['modal_bgcolor'] = $field_bcc_background_color;
  $data['modal_opacity'] = $field_bcc_background_opacity;
  $data['modal_width'] = $field_bcc_modal_width;
  $data['modal_height'] = $field_bcc_modal_height;

  return $data;
}

/**
 * Check if campaign is for particular role users.
 *
 * @param object $node
 *   Node containing campaign properties.
 *
 * @param array $user_rids
 *   User role ids to match with campaign assigned roles.
 *
 * @return bool
 *   Return TRUE if no role is set or user role matches.
 */
function bounce_convert_check_user_role($node, $user_rids) {
  // Check if role is set for campaing.
  if (!empty($node->field_bcc_specific_user_roles)) {

    // Get all roles which is set for that particular campaign.
    foreach ($node->field_bcc_specific_user_roles[LANGUAGE_NONE] as $role) {
      $campaign_roles[] = $role['value'];
    }

    // Check if user role matches campaign role.
    $result = array_intersect($campaign_roles, $user_rids);
    // If match return TRUE.
    if (!empty($result)) {
      return TRUE;
    }
    else {
      // If user role DOESN'T matches campaign role return FALSE.
      return FALSE;
    }
  }
  else {
    // If no role is set for campaign then show to all roles/users.
    return TRUE;
  }
}

/**
 * Wheater show modal on current page/path or not.
 *
 * @param array $paths
 *   List of paths/pages to show campaign on.
 *
 * @param array $bounce_convert_pages_option
 *   Only listed pages/Except Listed.
 *
 * @return bool
 *   Whether path matched or not for a campaign.
 */
function bounce_convert_check_page_path($paths, $bounce_convert_pages_option) {
  /*
   * $bounce_convert_pages_option (0) - All pages except those listed
   * $bounce_convert_pages_option (1) - Only the listed pages
   * $bounce_convert_pages_option (2) - PHP code returns TRUE
   */
  if (empty($paths) && $bounce_convert_pages_option == 0) {
    return TRUE;
  }
  if (empty($paths) && $bounce_convert_pages_option == 1) {
    return FALSE;
  }
  if (empty($paths) && $bounce_convert_pages_option == 2) {
    return TRUE;
  }
  // PHP code option is selected and code is given.
  if (!empty($paths) && $bounce_convert_pages_option == 2) {
    return bounce_convert_check_php_code($paths);
  }

  $bounce_convert_return = FALSE;
  // Exact match eg. admin/modules.
  if (in_array(current_path(), $paths)) {

    switch ($bounce_convert_pages_option) {
      case 0:
        $bounce_convert_return = FALSE;
        break;

      case 1:
        $bounce_convert_return = TRUE;
        break;

    }
    return $bounce_convert_return;
  }
  else {

    // Wild card character path eg. admin/*
    foreach ($paths as $key => $path) {
      $path = $paths[$key] = str_replace('*', '', $path);
      // TODO: drupal_is_front_page() not working with overlay module.
      if (($path == '&lt;front&gt;' || $path == '<front>') && drupal_is_front_page()) {
        switch ($bounce_convert_pages_option) {
          case 0:
            $bounce_convert_return = FALSE;
            break;

          case 1:
            $bounce_convert_return = TRUE;
            break;

        }
        return $bounce_convert_return;
      }

      if (substr($path, -1) == '/') {
        $path = substr($path, 0, -1);
      }
      $pos = strpos(current_path(), $path);
      if ($pos !== FALSE) {
        switch ($bounce_convert_pages_option) {
          case 0:
            $bounce_convert_return = FALSE;
            break;

          case 1:
            $bounce_convert_return = TRUE;
            break;

        }
        return $bounce_convert_return;
      }
      else {
        switch ($bounce_convert_pages_option) {
          case 0:
            $bounce_convert_return = TRUE;
            break;

          case 1:
            $bounce_convert_return = FALSE;
            break;

        }
        return $bounce_convert_return;
      }
    }
  }
}

/**
 * Evaluate user given PHP code.
 *
 * @param string $code
 *   PHP code which need to be evolated.
 *
 * @return bool
 *   Whether PHP return TURE or FALSE.
 */
function bounce_convert_check_php_code($code) {
  $code = html_entity_decode($code);
  $result = php_eval($code);

  if ($result == TRUE) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * Add custom fields for content type campaign.
 */
function bounce_convert_add_fields() {
  foreach (bounce_convert_field_default_field_bases() as $field) {
    field_create_field($field);
  }
  foreach (bounce_convert_field_default_field_instances() as $fieldinstance) {
    $fieldinstance['entity_type'] = 'node';
    $fieldinstance['bundle'] = BOUNCE_CONVERT_CONTENT_TYPE;
    field_create_instance($fieldinstance);
  }
}

/**
 * Delete custom fields for content type campaign.
 */
function bounce_convert_delete_fields() {
  foreach (array_keys(bounce_convert_field_default_field_bases()) as $field) {
    field_delete_field($field);
  }
  $instances = field_info_instances('node', BOUNCE_CONVERT_CONTENT_TYPE);
  foreach ($instances as $fieldinstance) {
    field_delete_instance($fieldinstance);
  }
}

/**
 * When user submits campaign an entry is made in DB.
 *
 * @param array $form
 *   Contains form elements.
 *
 * @param array $form_state
 *   Contains form fields value.
 */
function bounce_convert_user_conversion(&$form, &$form_state) {
  $bounce_convert_nid = $form_state['values']['details']['nid'];
  $bounce_convert_sid = $form_state['values']['details']['sid'];
  $ip_address = ip_address();
  $bounce_convert_id = db_query("SELECT MAX(id) FROM {bounce_convert_impression} WHERE webform_id = :webform_id AND ip_address = :ip_address", array(':webform_id' => $bounce_convert_nid, ':ip_address' => $ip_address))->fetchField();

  // Update existing record with impression.
  db_update('bounce_convert_impression')
      ->fields(array(
        'sid' => $bounce_convert_sid,
        'conversion' => 1,
      ))
      ->condition('id', $bounce_convert_id, '=')
      ->execute();
}

/**
 * Return impression/conversion data array from DB.
 *
 * @param string $report_duration
 *   Time duration.
 *
 * @param string $db_column
 *   Table column name.
 *
 * @return array
 *   Resut containing impressions and conversions.
 */
function bounce_convert_chart_data($report_duration, $db_column, $nid) {
  $data = '';

  // If any time duration is selected from filter options.
  switch ($report_duration) {
    case 'today':
      $y = 6;
      for ($i = 0; $i <= 23; $i++) {
        $start_of_the_day = date('Y-m-d h A', mktime($i, 0, 0, date("m"), date("d"), date("Y")));
        $start_of_the_day_str = strtotime($start_of_the_day);
        $y = $i + 1;
        $end_of_the_day = date('Y-m-d h A', mktime($y, 0, 0, date("m"), date("d"), date("Y")));
        $end_of_the_day_str = strtotime($end_of_the_day);
        $query = db_select('bounce_convert_impression', 'bci');
        $query->condition('campaign_id', $nid, '=')
            ->condition($db_column, 1, '=')
            ->condition('timestamp', $start_of_the_day_str, '>=')
            ->condition('timestamp', $end_of_the_day_str, '<=')
            ->addExpression('SUM(' . $db_column . ')', $db_column);
        $result = $query->execute();

        foreach ($result as $res) {
          $data[] = isset($res->$db_column) ? (int) $res->$db_column : 0;
        }
      }
      return $data;

    case 'yesterday':
      $y = 6;
      for ($i = 0; $i <= 23; $i++) {
        $start_of_the_day = date('Y-m-d h A', mktime($i, 0, 0, date("m"), date("d") - 1, date("Y")));
        $start_of_the_day_str = strtotime($start_of_the_day);
        $y = $i + 1;
        $end_of_the_day = date('Y-m-d h A', mktime($y, 0, 0, date("m"), date("d") - 1, date("Y")));
        $end_of_the_day_str = strtotime($end_of_the_day);

        $query = db_select('bounce_convert_impression', 'bci');
        $query->condition('campaign_id', $nid, '=')
            ->condition($db_column, 1, '=')
            ->condition('timestamp', $start_of_the_day_str, '>=')
            ->condition('timestamp', $end_of_the_day_str, '<=')
            ->addExpression('SUM(' . $db_column . ')', $db_column);
        $result = $query->execute();

        foreach ($result as $res) {
          $data[] = isset($res->$db_column) ? (int) $res->$db_column : 0;
        }
      }
      return $data;

    case '7days':
      for ($i = 7; $i > 0; $i--) {
        $start_of_the_day = date('Y-m-d', mktime(0, 0, 0, date("m"), date("d") - $i, date("Y")));
        $start_of_the_day_str = strtotime($start_of_the_day);
        $y = $i - 1;
        $end_of_the_day = date('Y-m-d', mktime(0, 0, 0, date("m"), date("d") - $y, date("Y")));
        $end_of_the_day_str = strtotime($end_of_the_day);

        $query = db_select('bounce_convert_impression', 'bci');
        $query->condition('campaign_id', $nid, '=')
            ->condition($db_column, 1, '=')
            ->condition('timestamp', $start_of_the_day_str, '>=')
            ->condition('timestamp', $end_of_the_day_str, '<=')
            ->addExpression('SUM(' . $db_column . ')', $db_column);
        $result = $query->execute();

        foreach ($result as $res) {
          $data[] = isset($res->$db_column) ? (int) $res->$db_column : 0;
        }
      }
      return $data;

    case '30days':
      for ($i = 30; $i > 0; $i--) {
        $start_of_the_day = date('Y-m-d', mktime(0, 0, 0, date("m"), date("d") - $i, date("Y")));
        $start_of_the_day_str = strtotime($start_of_the_day);
        $y = $i - 1;
        $end_of_the_day = date('Y-m-d', mktime(0, 0, 0, date("m"), date("d") - $y, date("Y")));
        $end_of_the_day_str = strtotime($end_of_the_day);

        $query = db_select('bounce_convert_impression', 'bci');
        $query->condition('campaign_id', $nid, '=')
            ->condition($db_column, 1, '=')
            ->condition('timestamp', $start_of_the_day_str, '>=')
            ->condition('timestamp', $end_of_the_day_str, '<=')
            ->addExpression('SUM(' . $db_column . ')', $db_column);
        $result = $query->execute();

        foreach ($result as $res) {
          $data[] = isset($res->$db_column) ? (int) $res->$db_column : 0;
        }
      }
      return $data;

    case 'this-month':
      for ($i = 0; $i < date("d"); $i++) {
        $start_of_the_day = date('Y-m-d', mktime(0, 0, 0, date("m"), $i + 1, date("Y")));
        $start_of_the_day_str = strtotime($start_of_the_day);
        $y = $i + 1;
        $end_of_the_day = date('Y-m-d', mktime(0, 0, 0, date("m"), $y + 1, date("Y")));
        $end_of_the_day_str = strtotime($end_of_the_day);

        $query = db_select('bounce_convert_impression', 'bci');
        $query->condition('campaign_id', $nid, '=')
            ->condition($db_column, 1, '=')
            ->condition('timestamp', $start_of_the_day_str, '>=')
            ->condition('timestamp', $end_of_the_day_str, '<=')
            ->addExpression('SUM(' . $db_column . ')', $db_column);
        $result = $query->execute();

        foreach ($result as $res) {
          $data[] = isset($res->$db_column) ? (int) $res->$db_column : 0;
        }
      }
      return $data;

    case 'last-month':
      $last_month_days = bounce_convert_count_last_month_days();
      for ($i = 0; $i < $last_month_days; $i++) {

        $start_of_the_day = date('Y-m-d', mktime(0, 0, 0, date("m") - 1, $i + 1, date("Y")));
        $start_of_the_day_str = strtotime($start_of_the_day);
        $y = $i + 1;
        $end_of_the_day = date('Y-m-d', mktime(0, 0, 0, date("m") - 1, $y + 1, date("Y")));
        $end_of_the_day_str = strtotime($end_of_the_day);

        $query = db_select('bounce_convert_impression', 'bci');
        $query->condition('campaign_id', $nid, '=')
            ->condition($db_column, 1, '=')
            ->condition('timestamp', $start_of_the_day_str, '>=')
            ->condition('timestamp', $end_of_the_day_str, '<=')
            ->addExpression('SUM(' . $db_column . ')', $db_column);
        $result = $query->execute();

        foreach ($result as $res) {
          $data[] = isset($res->$db_column) ? (int) $res->$db_column : 0;
        }
      }
      return $data;

    default:
      for ($i = 7; $i > 0; $i--) {
        $start_of_the_day = date('Y-m-d', mktime(0, 0, 0, date("m"), date("d") - $i, date("Y")));
        $start_of_the_day_str = strtotime($start_of_the_day);
        $y = $i - 1;
        $end_of_the_day = date('Y-m-d', mktime(0, 0, 0, date("m"), date("d") - $y, date("Y")));
        $end_of_the_day_str = strtotime($end_of_the_day);

        $query = db_select('bounce_convert_impression', 'bci');
        $query->condition('campaign_id', $nid, '=')
            ->condition($db_column, 1, '=')
            ->condition('timestamp', $start_of_the_day_str, '>=')
            ->condition('timestamp', $end_of_the_day_str, '<=')
            ->addExpression('SUM(' . $db_column . ')', $db_column);
        $result = $query->execute();

        foreach ($result as $res) {
          $data[] = isset($res->$db_column) ? (int) $res->$db_column : 0;
        }
      }
      return $data;
  }
}

/**
 * Create campaign report for chart.
 *
 * @param array $report_duration
 *   Time duration.
 *
 * @return array
 *   Contains labels per filter type(ie:for days,Months,hours etc.)
 */
function bounce_convert_chart_labels($report_duration) {
  // If any time duration is selected from filter options.
  $labels = array();
  switch ($report_duration) {
    case '7days':
      for ($i = 6; $i >= 0; $i--) {
        $labels[] = date('m/d', mktime($i, 0, 0, date("m"), date("d") - $i, date("Y")));
      }
      return $labels;

    case 'today':
      $y = 1;
      for ($i = 0; $i <= 23; $i++) {
        if ($y == 3 || $i == 0) {
          $labels[] = date('m/d @ hA', mktime($i, 0, 0, date("m"), date("d"), date("Y")));
          $y = 1;
        }
        else {
          $y++;
          $labels[] = '';
        }
      }
      return $labels;

    case 'yesterday':
      $y = 1;
      for ($i = 0; $i <= 23; $i++) {
        if ($y == 3 || $i == 0) {
          $labels[] = date('m/d @ hA', mktime($i, 0, 0, date("m"), date("d") - 1, date("Y")));
          $y = 1;
        }
        else {
          $y++;
          $labels[] = '';
        }
      }
      return $labels;

    case '30days':
      $y = 1;
      for ($i = 29; $i >= 0; $i--) {
        if ($y == 2 || $i == 0) {
          $labels[] = date('m/d', mktime($i, 0, 0, date("m"), date("d") - $i, date("Y")));
          $y = 1;
        }
        else {
          $y++;
          $labels[] = '';
        }
      }
      return $labels;

    case 'this-month':
      for ($i = 0; $i < date("d"); $i++) {
        $labels[] = date('m/d', mktime(0, 0, 0, date("m"), $i + 1, date("Y")));
      }
      return $labels;

    case 'last-month':
      $y = 1;
      $last_month_days = bounce_convert_count_last_month_days();
      for ($i = 0; $i < $last_month_days; $i++) {
        if ($y == 2 || $i == 0) {
          $labels[] = date('m/d', mktime(0, 0, 0, date("m") - 1, 1 + $i, date("Y")));
          $y = 1;
        }
        else {
          $y++;
          $labels[] = '';
        }
      }
      return $labels;

    default:
      for ($i = 6; $i >= 0; $i--) {
        $labels[] = date('m/d', strtotime("-$i days"));
      }
      return $labels;
  }
}

/**
 * Total days of last month.
 *
 * @return int
 *   Number of days from last month.
 */
function bounce_convert_count_last_month_days() {
  if (date("m") == 1 || date("m") == 01 || date("m") == '01') {
    $last_month_days = cal_days_in_month(CAL_GREGORIAN, date("m"), date("Y"));
  }
  else {
    $last_month_days = cal_days_in_month(CAL_GREGORIAN, date("m") - 1, date("Y") - 1);
  }
  return $last_month_days;
}

/**
 * Get nicely formatted converion percentage.
 *
 * @return float
 *   Conversion Percentage * (Conversion/Submission) * 100.
 */
function bounce_convert_overall_percentage($bounce_convert_total_conversion, $bounce_convert_total_impression) {
  if ($bounce_convert_total_conversion > 0) {
    $total_percentage = ($bounce_convert_total_conversion / $bounce_convert_total_impression) * 100;
  }
  else {
    $total_percentage = 0;
  }
  return number_format($total_percentage, 2);
}
