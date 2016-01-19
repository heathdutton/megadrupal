<?php

/**
 * @file
 * Admin page callbacks for the restrict_page_ip module.
 */

/**
 * Page callback function for admin/config/people/restrict_page_ip.
 */
function restrict_page_ip_settings() {
  $rows = array();
  $header = array(t('Page'), t('IP'), t('Access Control'), t('Operations'));
  $result = db_select('restrict_page_ip', 'rpi')->fields('rpi')->execute();
  $destination = drupal_get_destination();
  foreach ($result as $data) {
    $row = array();
    $row['data']['page'] = $data->page;
    $row['data']['ips'] = nl2br($data->ips);
    $row['data']['access_control'] = ($data->access_control == 1) ? 'Whitelist' : 'Blacklist';
    $operations = array();
    $operations['edit'] = array(
      'title' => t('edit'),
      'href' => "admin/config/people/restrict_page_ip/" . check_plain($data->id) . "/edit",
      'query' => $destination,
    );
    $operations['delete'] = array(
      'title' => t('delete'),
      'href' => "admin/config/people/restrict_page_ip/" . check_plain($data->id) . "/delete",
      'query' => $destination,
    );
    $row['data']['operations'] = array(
      'data' => array(
        '#theme' => 'links',
        '#links' => $operations,
        '#attributes' => array('class' => array('links', 'inline', 'nowrap')),
      ),
    );
    $rows[] = $row;
  }
  $build['restrict_page_ip_form'] = drupal_get_form('restrict_page_ip_form');
  $build['system_ip_blocking_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No Pages are available to Whitelist/Blacklist.'),
  );
  return $build;
}

/**
 * Generate Access Denied Message form.
 */
function restrict_page_ip_form($form, &$form_state) {
  $form['restrict_page_ip_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Message to show on Access Denied Page'),
    '#description' => t('Available variables are: [restrict_page_ip:page], [restrict_page_ip:ip]'),
    '#default_value' => variable_get('restrict_page_ip_message', t('Your IP [restrict_page_ip:ip] is not authorized to access the page [restrict_page_ip:page].
Please contact site administrator, if you need privileges to access the page [restrict_page_ip:page].
Sorry for the inconvinence.')),
    '#required' => TRUE,
  );
  return system_settings_form($form);
}

/**
 * Page callback; Displays the administration settings.
 */
function restrict_page_ip_edit_form($restrict_page_ip = NULL) {
  if ($restrict_page_ip) {
    drupal_set_title($restrict_page_ip->page);
    $output = drupal_get_form('restrict_page_ip_admin_form', $restrict_page_ip);
  }
  else {
    $output = drupal_get_form('restrict_page_ip_admin_form');
  }
  return $output;
}

/**
 * Generate Restrict IP administrator settings form.
 */
function restrict_page_ip_admin_form($form, &$form_state, $restrict_page_ip = NULL) {

  if (empty($restrict_page_ip)) {
    $restrict_page_ip = (object) $restrict_page_ip;
    $restrict_page_ip->page = '';
    $restrict_page_ip->ips = '';
    $restrict_page_ip->access_control = NULL;
    $restrict_page_ip->id = NULL;
  }

  $form['page'] = array(
    '#type' => 'textfield',
    '#title' => t('Page URL'),
    '#description' => t("Specify page by using their relative path. The '*' character is a wildcard. Example paths are blog for the blog page and blog/* for every personal blog."),
    '#default_value' => $restrict_page_ip->page,
    '#size' => 120,
    '#maxlength' => 120,
    '#required' => TRUE,
  );
  $form['ips'] = array(
    '#type' => 'textarea',
    '#title' => t('IP Address List'),
    '#description' => t('Enter the list of IP Addresses that are allowed or restricted to access the above page. Enter one IP address per line. You may also enter a range of IP addresses in the format AAA.BBB.CCC.XXX-AAA.BBB.CCC.YYY'),
    '#default_value' => $restrict_page_ip->ips,
    '#required' => TRUE,
  );
  $form['access_control'] = array(
    '#type' => 'radios',
    '#title' => t('Access Control'),
    '#description' => t('<strong>Whitelist</strong> : Allow people from the given IPs to access the page <br> <strong>Blacklist</strong> : Restrict people from the given IPs to access the page'),
    '#options' => array(1 => t('Whitelist'), 2 => t('Blacklist')),
    '#default_value' => $restrict_page_ip->access_control,
    '#required' => TRUE,
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  if ($restrict_page_ip->id) {
    $form['id'] = array(
      '#type' => 'hidden',
      '#value' => $restrict_page_ip->id,
    );
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('restrict_page_ip_delete_confirm_submit'),
    );
  }
  $form['#submit'][] = 'restrict_page_ip_edit_form_submit';
  $form['#validate'][] = 'restrict_page_ip_edit_form_validate';
  return $form;
}

/**
 * Validate function for settings form.
 */
function restrict_page_ip_edit_form_validate($form, &$form_state) {
  $invalid_ip = $opposite_ip_exist = $invalid_range = FALSE;
  $opposite_ips = array();
  $page_path = $form_state['values']['page'];
  $normal_path = drupal_get_normal_path($page_path);
  if ($page_path <> $normal_path) {
    $page_path = $form_state['values']['page'] = $normal_path;
  }

  if (!drupal_valid_path($page_path) && !preg_match('/\/\*/', $page_path)) {
    form_set_error('page', t("The page path '@page' is invalid.", array('@page' => $form_state['values']['page'])));
  }

  $id = isset($form_state['values']['id']) ? $form_state['values']['id'] : '';
  // Check if same Access Control is already present for the page.
  if ($rule = db_select('restrict_page_ip', 'rpi')
    ->fields('rpi', array('id'))
    ->condition('rpi.page', $form_state['values']['page'])
    ->condition('rpi.access_control', $form_state['values']['access_control'])
    ->condition('rpi.id', $id, '<>')
    ->execute()
    ->fetchAssoc()) {
    $access_control_label = ($form_state['values']['access_control'] == 1) ? "Whitelisted" : "Blacklist";
    form_set_error('page', t("The page '@page' is already added in @access_control_label. <a href='@edit_link' target='_blank'>Click here</a> to add your ips in the existing", array(
      '@page' => $form_state['values']['page'],
      '@access_control_label' => $access_control_label,
      '@edit_link' => url("/admin/config/people/restrict_page_ip/{$rule['id']}/edit"),
    )));
  }
  // Check if opposite Access Control is
  // already present and their ips intersect.
  elseif (strlen(trim($form_state['values']['ips']))) {
    $invalid_ip = FALSE;
    $ip_addresses = explode(PHP_EOL, trim($form_state['values']['ips']));
    $opposite_access_control = ($form_state['values']['access_control'] == 1) ? 2 : 1;
    $query = db_select('restrict_page_ip', 'rpi')
      ->fields('rpi', array('id', 'ips'))
      ->condition('rpi.page', $form_state['values']['page'])
      ->condition('rpi.access_control', $opposite_access_control);
    if (isset($form_state['values']['id'])) {
      $query->condition('rpi.id', $form_state['values']['id'], '<>');
    }
    $opposite_rule = $query->execute()
      ->fetchAssoc();
    if ($opposite_rule) {
      $opposite_rule_ips = explode(PHP_EOL, trim($opposite_rule['ips']));
      foreach ($opposite_rule_ips as $opposite_rule_ip) {
        if (strpos($opposite_rule_ip, '-') !== FALSE) {
          $opposite_ips['range'][] = $opposite_rule_ip;
        }
        else {
          $opposite_ips['single'][] = $opposite_rule_ip;
        }
      }
    }
    foreach ($ip_addresses as $ip) {
      $ip = trim($ip);
      // Validating range of IPs.
      if (strpos($ip, '-') !== FALSE) {
        $ip_range = explode('-', $ip);
        $ip_range_start = trim($ip_range[0]);
        $ip_range_end = trim($ip_range[1]);

        if (filter_var($ip_range_start, FILTER_VALIDATE_IP) == FALSE) {
          $invalid_ip = TRUE;
          break;
        }
        if (filter_var($ip_range_end, FILTER_VALIDATE_IP) == FALSE) {
          $invalid_ip = TRUE;
          break;
        }
        $ip_start_piece = substr($ip_range_start, 0, strrpos($ip_range_start, '.') + 1);
        $ip_end_piece = substr($ip_range_end, 0, strrpos($ip_range_end, '.') + 1);
        if ($ip_start_piece != $ip_end_piece) {
          $invalid_range = TRUE;
        }
        if (restrict_page_ip_compare_ip_group($ip, $opposite_ips, $ip_start_piece, 'range')) {
          $opposite_ip_exist = TRUE;
          break;
        }
      }
      else {
        // Validating single IP.
        $ip_piece = substr($ip, 0, strrpos($ip, '.') + 1);
        if (filter_var(trim($ip), FILTER_VALIDATE_IP) == FALSE) {
          $invalid_ip = TRUE;
          break;
        }
        if (restrict_page_ip_compare_ip_group($ip, $opposite_ips, $ip_piece, 'single')) {
          $opposite_ip_exist = TRUE;
          break;
        }
      }
    }
    if ($invalid_ip) {
      form_set_error('ips', t('Enter a valid IP addresses.'));
    }
    if ($opposite_ip_exist) {
      $access_control_label = ($opposite_access_control == 1) ? "Whitelisted" : "Blacklist ";
      form_set_error('ips', t('IP already listed on @access_control for the page "@page". <a href="@edit_url" target="_blank">Click here</a> to check.', array(
        '@access_control' => $access_control_label,
        '@page' => $form_state['values']['page'],
        '@edit_url' => url("/admin/config/people/restrict_page_ip/{$opposite_rule['id']}/edit"),
        ))
      );
    }
    if ($invalid_range) {
      form_set_error('ips', t('IP range is not in proper format. Eg
          AAA.BBB.CCC.XXX-AAA-BBB-CCC-YYY'));
    }
  }
}

/**
 * Submit function for settings form.
 */
function restrict_page_ip_edit_form_submit($form, &$form_state) {
  $form_values = $form_state['values'];
  $form_values['page'] = drupal_get_normal_path($form_values['page']);
  $to_replace = array(
    '/\\\\\*/',
  );
  $replacements = array('.*');
  $patterns_quoted = preg_quote($form_values['page'], '/');
  $pattern = '^' . preg_replace($to_replace, $replacements, $patterns_quoted) . '$';
  $form_values['pattern'] = $pattern;
  if (empty($form_values['id'])) {
    drupal_write_record('restrict_page_ip', $form_values);
  }
  else {
    form_state_values_clean($form_state);
    drupal_write_record('restrict_page_ip', $form_values, array('id'));
  }
  $ip_addresses = explode(PHP_EOL, trim($form_values['ips']));
  $ips = implode(', ', $ip_addresses);
  $access_control = ($form_values['access_control'] == 1) ? 'Whitelisted' : 'Blacklisted';
  drupal_set_message(t('The IP address %ips has been %access_control.', array('%ips' => $ips, '%access_control' => $access_control)));
  $form_state['redirect'] = 'admin/config/people/restrict_page_ip';
}

/**
 * Confirm function for settings form.
 */
function restrict_page_ip_delete_confirm($form, &$form_state, $restrict_page_ip) {
  $form['id'] = array(
    '#type' => 'value',
    '#value' => $restrict_page_ip->id,
  );
  return confirm_form($form, t('Are you sure you want to delete "%page" url?', array('%page' => $restrict_page_ip->page)), 'admin/config/people/ip-blocking', t('This action cannot be undone.'), t('Delete'), t('Cancel'));
}

/**
 * Confirm Submit function for settings form.
 */
function restrict_page_ip_delete_confirm_submit($form, &$form_state) {
  db_delete('restrict_page_ip')
    ->condition('id', $form_state['values']['id'])
    ->execute();
  drupal_set_message(t('The page access control has been deleted.'));
  $form_state['redirect'] = 'admin/config/people/restrict_page_ip';
}

/**
 * To Check whether two range of ips have common ips.
 *
 * @param string $range1
 *   The ranges of ips.
 * @param string $range2
 *   The ranges of ips.
 *
 * @return bool
 *   Returns TRUE if two range of ips have common ips or FALSE if not found.
 */
function restrict_page_ip_compare_range_range($range1, $range2) {
  $ip1_range = restrict_page_ip_start_end($range1);
  $ip2_range = restrict_page_ip_start_end($range2);
  $ip1_range = range($ip1_range['range_start'], $ip1_range['range_end']);
  $ip2_range = range($ip2_range['range_start'], $ip2_range['range_end']);
  $range_intersect = array_intersect($ip1_range, $ip2_range);
  if ($range_intersect) {
    return TRUE;
  }
  return FALSE;
}

/**
 * To Check whether a IP is present in the range of ips.
 *
 * @param string $single
 *   The ip address.
 * @param string $range
 *   The ranges of ips.
 *
 * @return bool
 *   Returns TRUE if the ip($single) is found on the range of
 *   given ips($range) or FALSE if not found.
 */
function restrict_page_ip_compare_ip_range($single, $range) {
  $range_ip_array = explode('-', $range);
  // Separating start and end IPs of the range from the
  // format AAA.BBB.CCC.XXX-AAA.BBB.CCC.YYY.
  $ip_min = sprintf('%u', ip2long(trim($range_ip_array[0])));
  $ip_max = sprintf('%u', ip2long(trim($range_ip_array[1])));
  $ip = sprintf('%u', ip2long(trim($single)));
  // Checks wheteher the IP falls on the given range.
  if ($ip >= $ip_min && $ip <= $ip_max) {
    return TRUE;
  }
  return FALSE;
}

/**
 * To Check whether two ips are same.
 *
 * @param string $single1
 *   The ip address.
 * @param string $single2
 *   The ip address.
 *
 * @return bool
 *   Returns TRUE if the ip($single) is found on the range of
 *   given ips($range) or FALSE if not found.
 */
function restrict_page_ip_compare_ip_ip($single1, $single2) {
  $ip1 = sprintf('%u', ip2long(trim($single1)));
  $ip2 = sprintf('%u', ip2long(trim($single2)));
  // Checks wheteher two ips are same.
  if ($ip1 == $ip2) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Helper function to get IP range.
 */
function restrict_page_ip_start_end($range) {
  $ip_range = explode('-', $range);
  $ip1 = $ip_range[0];
  $ip2 = $ip_range[1];
  $ip1_pieces = explode('.', $ip1);
  $ip2_pieces = explode('.', $ip2);
  $ip_range_start = $ip1_pieces[3];
  $ip_range_end = $ip2_pieces[3];
  return array('range_start' => $ip_range_start, 'range_end' => $ip_range_end);
}

/**
 * To Check whether two ips are same.
 *
 * @param string $ip
 *   The ip address.
 * @param array $opposite_ips
 *   The ip address.
 * @param string $ip_piece
 *   The piece of ip.
 * @param string $mode
 *   The mode of ips.
 *
 * @return bool
 *   Returns TRUE if the ip($single) is found on the range of given ips($range)
 *   or FALSE if not found.
 */
function restrict_page_ip_compare_ip_group($ip, array $opposite_ips, $ip_piece, $mode) {
  if ($mode == "single") {
    $function1 = 'restrict_page_ip_compare_ip_ip';
    $function2 = 'restrict_page_ip_compare_ip_range';
  }
  else {
    $function1 = 'restrict_page_ip_compare_ip_range';
    $function2 = 'restrict_page_ip_compare_range_range';
  }
  if (isset($opposite_ips['single']) &&
    $matches = preg_grep("/^{$ip_piece}/", $opposite_ips['single'])) {
    foreach ($matches as $opp_single) {
      if ($function1($opp_single, $ip)) {
        return TRUE;
      }
    }
  }
  if (isset($opposite_ips['range']) &&
    $matches = preg_grep("/^{$ip_piece}/", $opposite_ips['range'])) {
    foreach ($matches as $opp_range) {
      if ($function2($ip, $opp_range)) {
        return TRUE;
      }
    }
  }
  return FALSE;
}
