<?php
/**
 * @file
 * To create user popup.
 *
 */

/**
 * Implements menu callback.
 */
function user_popup_info_display() {
  if (isset($_POST['user_popup_path'])) {
    $final_path = '';
    $path = check_plain($_POST['user_popup_path']);
    $path = ltrim($path, '/');

    // Remove any other unwanted leading path words except user/.
    $strpos = strpos($path, 'user/');
    $path = substr($path, $strpos, strlen($path) - $strpos);

    // Check if this is an original path.
    if (_user_popup_info_check_for_user_url($path)) {
      $final_path = $path;
    }
    else {
      $result = db_select('url_alias', 'ua')
      ->fields('ua', array('source'))
      ->condition('alias', $path, '=')
      ->execute()
      ->fetchCol();

      $final_path = $result[0];
    }

    // Final path is the original user path.
    global $user;
    $uid = 0;
    $cuser = array();
    $pos = _user_popup_info_check_for_user_url($final_path, $cuser);

    if ($pos) {
      $uid = substr($final_path, strlen('user/'));

      if ($uid != 0) {
        if (isset($cuser->uid) && $cuser->uid != 0) {

          // Get user data.
          $user_details['name'] = isset($cuser->realname) ? l($cuser->realname, 'user/' . $cuser->uid) : l($cuser->name, 'user/' . $cuser->uid);
          $user_details['picture'] = isset($cuser->picture) ? _user_popup_info_get_picture($cuser->picture) : _user_popup_info_get_picture('no-pic');

          // Set pic width and height.
          $user_details['pic_width'] = (variable_get('user_popup_info_pic_width', '80') > 0) ? variable_get('user_popup_info_pic_width', '80') . 'px' : '80px';

          $user_details['member_since'] = isset($cuser->created) ? _user_popup_info_get_time_ago($cuser->created) : 'not-a-member';
          $user_details['status'] = isset($cuser->status) ? ($cuser->status == 1 ? 'Active' : 'Blocked') : 'unknown';

          // Display selected user fields.
          $i = 0;
          $user_fields = variable_get('user_popup_info_select_fields', array());
          foreach ($user_fields as $fname => $fval) {
            if ($fval != '0') {
              $value = $cuser->$fname;
              $instance = field_info_instance('user', $fname, 'user');

              $fields[$fname]['label'] = ucfirst($instance['label']);
              $fields[$fname]['value'] = $value[LANGUAGE_NONE][0]['value'];
              $value = '';
            }
          }
          $user_details['fields'] = $fields;
          $user_details['cuser'] = $cuser;

          // Put user details in $variables.
          $variables = array(
            'data' => $user_details,
          );

          // Return theme output.
          echo theme('render_user_popup', $variables);
        }
      }
    }
    else {
      echo 'no-result';
    }
  }
  else {
    drupal_access_denied();
  }
}


/**
 * Helper function to get time ago.
 */
function _user_popup_info_get_time_ago($timestamp) {
  $diff = time() - (int)$timestamp;
  if ($diff == 0) 
    return;

  $intervals = array(
    1 => array('year', 31556926),
    $diff < 31556926 => array('month', 2628000),
    $diff < 2629744 => array('week', 604800),
    $diff < 604800 => array('day', 86400),
    $diff < 86400 => array('hour', 3600),
    $diff < 3600 => array('minute', 60),
    $diff < 60 => array('second',  1),
  );
  $value = floor($diff / $intervals[1][1]);
  return $value . ' ' . $intervals[1][0] . ($value > 1 ? 's +' : '');
}


/**
 * Helper function to get rendered user picture.
 */
function _user_popup_info_get_picture($picture) {
  global $base_url;
  return ($picture->uri) ? image_style_url('thumbnail', $picture->uri) : $base_url . '/' . USER_POPUP_INFO_PATH . '/images/profile.jpg';
}


/**
 * Helper function to check if provided URL belongs to user.
 */
function _user_popup_info_check_for_user_url($path, &$user = array()) {
  $pos = (int)strpos($path, 'user/');
  $substr = substr($path, strlen('user/'));
  $uid = 0;

  // If path contains "user/".
  if ($pos === 0) {

    // Check if this path goes to user profile only.
    if (!strpos($substr, '/')) {
      $uid = (int)substr($path, strlen('user/'));
      $user = user_load($uid);
      return ($user->uid != 0) ? TRUE : FALSE;
    }
    else {
      return FALSE;
    }
  }
  else {
    return FALSE;
  }
}


/**
 * Implements theme callback.
 */
function theme_render_user_popup($variables) {
  $user_details = $variables['data'];

  // Show user popup.
  $output = '';
  if (($user_details['status'] == 'Active')) {
    $output .= '<div class="user-popup-processed-info">';
  }
  else {
    $output .= '<div style="border-left:5px solid #ed541d;background-color:#fef5f1;" class="user-popup-processed-info">';
  }

  $output .= '<div id="user-popup-display-details-pic">';
  $output .= '<span id="user-popup-pic"><img width="' . $user_details['pic_width'] . '" height="' . $user_details['pic_height'] . '" src="' . $user_details['picture'] . '" /></span>';
  $output .= '</div>';
  $output .= '<div id="user-popup-display-details">';
  $output .= '<span id="user-popup-name"><strong>' . $user_details['name'] . '</strong></span>';

  // Whether to show default field values or not.
  if (variable_get('user_popup_info_show_default_user_info', 'yes') == 'yes') {
    $output .= '<br>';
    $output .= '<span id="user-popup-member-since">Member since - ' . $user_details['member_since'] . '</span>';
    $output .= '<br>';
    $output .= '<span id="user-popup-status">Status - ' . $user_details['status'] . '</span>';
  }

  // Display user fields.
  if (count($user_details['fields']) > 0) {
    $i = 0;
    foreach ($user_details['fields'] as $fname => $fval) {
      $output .= '<br>';
      $output .= '<span class="user-popup-user-fields" id="user-popup-user-field-' . ++$i . '">' . $fval['label'] . ' - ' . $fval['value'] . '</span>';
      
    }
  }
  $output .= '</div>';
  $output .= '</div>';
  $output .= '</div>';
  return $output;
}