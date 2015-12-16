<?php

/**
 * @file
 * Contains the administrative functions of the Facebook Wall fetcher module.
 *
 * This file is included by the facebook_wall.module file.
 */
if (variable_get('facebook_wall_access_token') != '') {
  define('FB_ACCESS_TOKEN', variable_get('facebook_wall_access_token'));
} else {
  define('FB_ACCESS_TOKEN', variable_get('facebook_wall_access_token_temp'));
}
define('FB_PAGE_NAME', variable_get('facebook_wall_page_url', 'me'));
define('FB_API_VER', variable_get('facebook_wall_api_version', 'v2.3'));
define('MODULE_PATH', drupal_get_path('module', 'facebook_wall')); 
/**
 * Admin configure form for Facebook login & token access.
 */
function _facebook_wall_login() {
  $form['fbw'] = array(
      '#type' => 'fieldset',
      '#title' => t('APP Setting'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#description' => t('In order to get facebook wall post, you had to first enter your APP Key and App Secret Key to get a valid a Access token of FB API. Create a new') . ' ' . l(t('Facebook Apps'), 'https://developers.facebook.com/apps') . t(", if you don't have it. Facebook Access token are used for accessing FB Graph API. Take note as it will expire in 60 days. (FB no longer offering unlimited offline access token."),
  );

  // FB App settings.
  $form['fbw']['facebook_wall_app_id'] = array(
      '#type' => 'textfield',
      '#title' => t('App ID/ API Key'),
      '#default_value' => variable_get('facebook_wall_app_id'),
      '#description' => t('Your facebook unique App ID/ API Key. Also called OAuth client_id value on FB App settings pages.'),
      '#required' => TRUE,
  );
  $form['fbw']['facebook_wall_app_secret'] = array(
      '#type' => 'textfield',
      '#title' => t('App Secret Key'),
      '#default_value' => variable_get('facebook_wall_app_secret'),
      '#description' => t('Your facebook unique App Secret Key. Also called OAuth client_secret value on FB App settings pages.'),
      '#required' => TRUE,
  );
  $form['fbw']['facebook_api_ver'] = array(
      '#type' => 'textfield',
      '#title' => t('API Version'),
      '#default_value' => variable_get('facebook_wall_api_version','v2.3'),
      '#description' => t('Facebook API version.'),
      '#required' => TRUE,
  );
  // FB Access Token settings.
  if (variable_get('facebook_wall_app_id') != '' && variable_get('facebook_wall_app_secret') != '') {
    $form['token'] = array(
        '#type' => 'fieldset',
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
        '#title' => t('Access Token'),
    );
    // View token option, if it expires with in 3 days.
    if (variable_get('facebook_wall_access_token_expiry') == '' || (int) variable_get('facebook_wall_access_token_expiry') < 259200) {
      $form['token']['facebook_wall_access_token_temp'] = array(
          '#type' => 'textfield',
          '#title' => t('Access Token Temporary'),
          '#size' => 160,
          '#maxlength' => 255,
          '#default_value' => variable_get('facebook_wall_access_token_temp'),
          '#description' => t('Copy the access token from') . ' ' . l(t('Facebook Graph Explorer'), 'https://developers.facebook.com/tools/explorer/' . variable_get('facebook_wall_app_id'), array('attributes' => array('target' => '_blank'))) . ' ' . t('& click on "Get Access Token" an then paste the value of Access token here. Note this token will expires within 1 hour.'),
          '#required' => TRUE,
      );
    }

    if (variable_get('facebook_wall_access_token') != '') {
      $form['token']['facebook_wall_access_token'] = array(
          '#type' => 'textfield',
          '#title' => t('Access Token (Long Term)'),
          '#maxlength' => 255,
          '#size' => 120,
          '#default_value' => variable_get('facebook_wall_access_token'),
          '#description' => t('This filed is read only. Note: Long term FB Access token get are valid for 60 days. (FB no longer offering unlimited offline access token.)'),
          '#attributes' => array('readonly' => 'readonly'),
      );

      if (variable_get('facebook_wall_access_token_expiry') != '') {
        $expiry = facebook_wall_get_expiry();

        if ((int) variable_get('facebook_wall_access_token_expiry') < 259200) {

            $message =  t('Your Access token will expires with in !expire_time !expire_suffix . Please click on <strong>"Get Long term Access token"</strong> button to get access for another 60 days.', array(
                      '!expire_time' => $expiry['time'],
                      '!expire_suffix' => $expiry['suffix'],
                  ));
            watchdog('facebook wall', $message, array(), WATCHDOG_DEBUG);
            if(variable_get('facebook_wall_message_show', 1)) {
                drupal_set_message($message, 'warning');
            }
        }
        $form['token']['facebook_wall_access_token_expiry'] = array(
            '#markup' => '<strong>Long Term access token Expires in : ' . $expiry['time'] . $expiry['suffix'] . '</strong>',
            '#prefix' => '<div style="margin-bottom:20px;" id="div_get_access_token">',
            '#suffix' => '</div>',
        );

        $form['token']['facebook_wall_reset_button'] = array(
            '#type' => 'submit',
            '#value' => t('Reset tokens'),
            '#submit' => array('_facebook_wall_reset_token'),
        );
      }
    }
    // View token option, if it expires with in 3 days.
    if ((variable_get('facebook_wall_access_token_expiry') == '' || (int) variable_get('facebook_wall_access_token_expiry') < 259200) && variable_get('facebook_wall_access_token_temp') != '') {
      $form['token']['facebook_wall_access_button'] = array(
          '#type' => 'submit',
          '#value' => t('Get Long term Access token'),
          '#submit' => array('_facebook_wall_get_access_token'),
      );
    }
  }

  // FB user page id/name setting.
  $form['page'] = array(
      '#type' => 'fieldset',
      '#title' => t('Page or User Profile'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#description' => t('In order to get facebook wall post, you had to first need to have a Access token.'),
  );
  $form['page']['facebook_wall_page_url'] = array(
      '#type' => 'textfield',
      '#title' => t('Facebook Page URL'),
      '#field_prefix' => 'https://www.facebook.com/',
      '#default_value' => variable_get('facebook_wall_page_url'),
      '#description' => t('Enter your facebook page user name. Example: username: "OSSCube" or id: "322122293265" or "me" for logged in user profile.'),
      '#required' => TRUE,
  );

  // FB user wall post setting.
  if (FB_PAGE_NAME != NULL) {
    $form['page']['facebook_wall_test_url'] = array(
        '#markup' => t('Test this Facebook page URL.') . ' ' . l(t('https://www.facebook.com/') . FB_PAGE_NAME, 'https://www.facebook.com/' . FB_PAGE_NAME, array('attributes' => array('target' => '_blank'))) . '<br /><br />',
    );
    $form['wall_setting'] = array(
        '#type' => 'fieldset',
        '#title' => t('Wall Content settings'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#description' => t('In order to get facebook wall post, you had to first need to have a Access token.'),
    );

    $form['wall_setting']['facebook_wall_menu_name'] = array(
        '#type' => 'textfield',
        '#title' => t('Name of wall page'),
        '#size' => 20,
        '#default_value' => variable_get('facebook_wall_menu_name'),
        '#required' => TRUE,
        '#description' => t('Enter the facebook wall page name. Please clear the cache in order to view new page name for') . ' ' . l(('facebook_wall'), 'facebook_wall'),
    );

    // FB wall post type.
    $post_type = array(
        'link' => t('Link'),
        'photo' => t('Photo'),
        'status' => t('Status'),
        'video' => t('Video'),
    );
    $form['wall_setting']['checkbox_msg'] = array(
        '#markup' => '<strong>' . t('FB wall post type') . '</strong>',
    );
    foreach ($post_type as $key => $value) {
      $form['wall_setting']['facebook_wall_post_type_' . $key] = array(
          '#type' => 'checkbox',
          '#title' => $value,
          '#default_value' => variable_get('facebook_wall_post_type_' . $key, 1),
      );
    }
    $form['wall_setting']['facebook_wall_post_limit'] = array(
        '#type' => 'textfield',
        '#title' => t('Wall Page Limit'),
        '#size' => 3,
        '#default_value' => (int) variable_get('facebook_wall_post_limit', 10),
        '#required' => TRUE,
        '#description' => t('Enter the wall post limit (in numeric). Note: Higher the limit, takes more time to fetch post form Facebook API.'),
    );
    $width_options = array('no-style' => t('No Extra Width Style'), 'relative-width' => t('Relative Width Style'), 'pixel-width' => t('Pixel Width Style'));
    $form['wall_setting']['facebook_wall_width_option'] = array(
        '#type' => 'radios',
        '#title' => 'Wall Page Width Setting',
        '#default_value' => variable_get('facebook_wall_width_option', 'no-style'),
        '#options' => $width_options,
        '#description' => t('Choose how the module should set the width of the wall.'),
        '#required' => TRUE,
    );
    $form['wall_setting']['facebook_wall_width_size_relative'] = array(
        '#type' => 'textfield',
        '#title' => t('Wall Page Width Size (Relative)'),
        '#size' => 2,
        '#field_suffix' => t('%'),
        '#default_value' => (int) variable_get('facebook_wall_width_size_relative', 100),
        '#required' => TRUE,
        '#description' => t('Enter the wall width size as a percentage (omit % sign).'),
        '#states' => array(
            'visible' => array(
                ':input[name="facebook_wall_width_option"]' => array('value' => 'relative-width'),
            ),
        ),
    );
    $form['wall_setting']['facebook_wall_width_size'] = array(
        '#type' => 'textfield',
        '#title' => t('Wall Page Width Size'),
        '#size' => 2,
        '#field_suffix' => t('px'),
        '#default_value' => (int) variable_get('facebook_wall_width_size', 500),
        '#required' => TRUE,
        '#description' => t('Enter the wall width size in numeric & between 400px to 800px.'),
        '#states' => array(
            'visible' => array(
                ':input[name="facebook_wall_width_option"]' => array('value' => 'pixel-width'),
            ),
        ),
    );
    $form['wall_setting']['button_visible_msg'] = array(
        '#markup' => '<strong>' . t('Enable Form Control') . '</strong>',
    );
    $form['wall_setting']['facebook_wall_older_post_button'] = array(
        '#type' => 'checkbox',
        '#title' => t('"Get Older Post" Button'),
        '#default_value' => variable_get('facebook_wall_older_post_button', 1),
    );
    $form['wall_setting']['facebook_wall_post_view'] = array(
        '#type' => 'checkbox',
        '#title' => t('"View Post" Link'),
        '#default_value' => variable_get('facebook_wall_post_view', 1),
    );
    $form['wall_setting']['facebook_wall_autoplay_video'] = array(
        '#type' => 'checkbox',
        '#title' => t('Auto Play video'),
        '#default_value' => variable_get('facebook_wall_autoplay_video', 0),
        '#description' => t('It works, if video post status is enabled.'),
    );
    
    $form['wall_setting']['facebook_message_control'] = array(
        '#markup' => '<strong>' . t('Facebook authentication messages Control') . '</strong>',
    );
    $form['wall_setting']['facebook_wall_message_show'] = array(
        '#type' => 'checkbox',
        '#title' => t('Display all facebook authentication messages'),
        '#default_value' => variable_get('facebook_wall_message_show', 1),
        '#description' => t('Facebook error message display while api authentication.'),
    );
    
  }

  return system_settings_form($form);
}

/**
 * Validate form for Facebook login & token access.
 */
function _facebook_wall_login_validate(&$form, &$form_state) {

  // Removing token & time if app id change.
  if (stristr($form_state['values']['facebook_wall_app_id'], variable_get('facebook_wall_app_id')) == FALSE) {
    variable_del('facebook_wall_access_token');
    variable_del('facebook_wall_access_token_expiry');
    variable_del('facebook_wall_access_token_start');
    $form_state['values']['facebook_wall_access_token_temp'] = '';
  }

  if (isset($form_state['values']['facebook_wall_post_limit'])) {
    if (!(is_numeric($form_state['values']['facebook_wall_post_limit']))) {
      form_set_error('facebook_wall_post_limit', t('FB wall post limit should be numeric'));
    } elseif ($form_state['values']['facebook_wall_post_limit'] < 1 || $form_state['values']['facebook_wall_post_limit'] > 999) {
      form_set_error('facebook_wall_post_limit', t('FB wall post limit should be between 1 to 999.'));
    }
  }
  $url = 'https://www.facebook.com/' . $form_state['values']['facebook_wall_page_url'];
  if (!preg_match('|^http(s)?://[a-z0-9-]+(.[a-z0-9-]+)*(:[0-9]+)?(/.*)?$|i', $url)) {
    form_set_error('facebook_wall_page_url', t('FB page URL or user id is not a valid address.'));
  }
  if (isset($form_state['values']['facebook_wall_post_type_link'])) {
    if ($form_state['values']['facebook_wall_post_type_link'] == 0 && $form_state['values']['facebook_wall_post_type_photo'] == 0 && $form_state['values']['facebook_wall_post_type_status'] == 0 && $form_state['values']['facebook_wall_post_type_video'] == 0) {
      form_set_error('facebook_wall_post_type_status', t('Please select atleast one FB Wall post type.'));
    }
  }
  if (isset($form_state['values']['facebook_wall_width_option'])) {
    if ($form_state['values']['facebook_wall_width_option'] == 'relative-width') {
      if (isset($form_state['values']['facebook_wall_width_size_relative'])) {
        if (!(is_numeric($form_state['values']['facebook_wall_width_size_relative']))) {
          form_set_error('facebook_wall_width_size_relative', t('FB wall post width should be numeric (don\'t include the % symbol).'));
        }
        if (($form_state['values']['facebook_wall_width_size_relative'] <= 0) || ($form_state['values']['facebook_wall_width_size_relative'] > 100)) {
          form_set_error('facebook_wall_width_size_relative', t('FB wall post width should be greater than 0% and less than 100%.'));
        }
      }
    } elseif ($form_state['values']['facebook_wall_width_option'] == 'pixel-width') {
      if (isset($form_state['values']['facebook_wall_width_size'])) {
        if (!(is_numeric($form_state['values']['facebook_wall_width_size']))) {
          form_set_error('facebook_wall_width_size', t('FB wall post width should be numeric.'));
        }
        if (($form_state['values']['facebook_wall_width_size'] < 400) || ($form_state['values']['facebook_wall_width_size'] > 800)) {
          form_set_error('facebook_wall_width_size', t('FB wall post width should between 400px and 800px.'));
        }
      }
    }
  }
}

/**
 * Admin configure form for user Page information.
 */
function facebook_wall_page_info() {
  $page_detail = _facebook_wall_page_info_array();
  if ($page_detail != FALSE) {
    $img_url = _facebook_wall_profile_picture($page_detail->id);
    $form['fbw'] = array(
        '#type' => 'fieldset',
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
        '#title' => t('Facebook User Details'),
    );
    $header = array(
        'key' => array('data' => t('Key')),
        'value' => array('data' => t('Value')),
    );
    // Putting FB array content into table.
    $rows = array(
        isset($img_url) ? array('Profile Picture', '<img src="' . $img_url . '" />') : NULL,
        array('User ID', $page_detail->id),
        array('Full Name', 'value' => $page_detail->name),
        isset($page_detail->about) ? array('About us', $page_detail->about) : NULL,
        isset($page_detail->link) ? array(
            'Profile URL',
            l($page_detail->link, $page_detail->link)) : NULL,
        isset($page_detail->category) ? array('Category', $page_detail->category) : NULL,
        isset($page_detail->likes) ? array('Facebook Likes', $page_detail->likes) : NULL,
        isset($page_detail->website) ? array('Website', $page_detail->website) : NULL,
        isset($page_detail->cover->source) ? array('Profile Picture', '<img src="' . $page_detail->cover->source . '" />') : NULL,
    );
    $form['fbw']['user_table'] = array(
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $rows,
        '#empty' => t('No content available.'),
    );
    
    
    $form['permissions'] = array(
        '#type' => 'fieldset',
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
        '#title' => t('Access token User permissions'),
    );
    
    $permissions_header = array(
        'key' => array('data' => t('Key')),
        'value' => array('data' => t('Value')),
    );
    $rows = array();
    $access_token = _facebook_wall_access_token_permission_info_array();
    if(isset($access_token->data)) {
        foreach($access_token->data as $permission){
            $rows[] = array(
                'key'=> t($permission->permission),
                'value'=> t($permission->status),
            );
        }
    }
    $form['permissions']['access_permission'] = array(
        '#theme' => 'table',
        '#header' => $permissions_header,
        '#rows' => $rows,
        '#empty' => t('No content available.'),
    );
    
    //print '<pre>'; print_r($access_token->data); print '</pre>';
    return $form;
  }
}

/**
 * Generate basic FB page or user information content.
 *
 * @return array
 *   Array of FB page basic on success and FALSE if it isn't.
 */
function _facebook_wall_page_info_array() {
  // FB Graph API CURL calls request.
  $api_url = 'https://graph.facebook.com/'.FB_API_VER.'/' . FB_PAGE_NAME . '?access_token=' . FB_ACCESS_TOKEN;
  return _facebook_wall_error_msg($api_url);
}

/**
 * Facebook wall post content fetcher.
 */
function _facebook_wall_wall_posts() {
  // FB Graph API CURL Calls request.
  $api_url = 'https://graph.facebook.com/'.FB_API_VER.'/' . FB_PAGE_NAME . '/feed?access_token=' . FB_ACCESS_TOKEN . '&limit=' . (int) variable_get('facebook_wall_post_limit', 10);

  // HTML Contains FB current wall post.
  $html = _facebook_wall_theme_html_content($api_url);


  if (FB_ACCESS_TOKEN == '') {
    return;
  } elseif ($html != FALSE) {
    $form['facebook_wall'] = array(
        '#markup' => $html,
    );

    if (variable_get('facebook_wall_older_post_button') == 1) {
      // This will be append by wth an updated wall post value.
      $form['div_append_next_wall_post'] = array(
          '#markup' => '',
          '#prefix' => '<div style="float:left; width: ' . variable_get('facebook_wall_width_size') . 'px;"  id="div_append_next_wall_post">',
          '#suffix' => '</div>',
      );
      $form['facebook_wall_next_wall_post'] = array(
          '#type' => 'button',
          '#value' => t('Get older Posts'),
          '#ajax' => array(
              'callback' => '_facebook_wall_ajax_next_wall_post',
              'wrapper' => 'div_append_next_wall_post',
              'method' => 'append',
          ),
          '#prefix' => '<div style="clear:both; margin-top:10px;">',
          '#suffix' => '</div>',
      );
    }
    return $form;
  } else {
      $message = t("Facebook wall post is not available ! This may caused due to user's content privacy policy.");
      watchdog('facebook wall', $message, array(), WATCHDOG_DEBUG);
      if(variable_get('facebook_wall_message_show', 1)) {
        drupal_set_message($message, 'warning');
      }
  }
}

/**
 * Ajax call to get older FB wall posts.
 */
function _facebook_wall_ajax_next_wall_post($form, $form_state) {

  $api_url = variable_get('facebook_wall_post_pagging_next');

  $html = _facebook_wall_theme_html_content($api_url);
  if ($html != FALSE) {
    return $html;
  }
  return 'Not able to get older posts';
}

/**
 * Generate HTML content from template file.
 *
 * @param string $api_url
 *   FB graph api url with access token and fields.
 *
 * @return array
 *   Array of FB content on success data fetch and FALSE if it isn't.
 */
function _facebook_wall_theme_html_content($api_url) {
  // Contains FB current wall post.
  $wall_post = _facebook_wall_error_msg($api_url);
  if (($wall_post != FALSE) && isset($wall_post->data[0])) {
    variable_set('facebook_wall_post_pagging_next', $wall_post->paging->next);

    // Contains FB user basic informaion.
    $basic_page = _facebook_wall_page_info_array();

    if ($basic_page != FALSE) {
      // Sending two FB array to template file.
      return theme('facebook_wall', array('facebook_wall' => $wall_post, 'facebook_page' => $basic_page));
    }
  } else {
    return FALSE;
  }
}

/**
 * Get FB user profile picture with given FB user id.
 *
 * @param string $user_id
 *   FB particular user ID.
 *
 * @return array
 *   Merge array of FB page basic with profile picture and FALSE if it isn't.
 */
function _facebook_wall_profile_picture($user_id) {
  // FB Graph API CURL calls request.
  $api_url = 'https://graph.facebook.com/'.FB_API_VER.'/' . $user_id . '?access_token=' . FB_ACCESS_TOKEN . '&fields=picture';
  $profile_picture = _facebook_wall_error_msg($api_url);
  if ($profile_picture != FALSE) {
    return $profile_picture->picture->data->url;
  } else {
    return FALSE;
  }
}

/**
 * Admin configure form for Facebook Page Likes.
 */
function _facebook_wall_page_likes() {
  // FB Graph API CURL Calls request.
  $fields = 'link,name,picture';
  $api_url = 'https://graph.facebook.com/'.FB_API_VER.'/' . FB_PAGE_NAME . '/likes?fields=' . $fields . '&access_token=' . FB_ACCESS_TOKEN;

  $user_likes = _facebook_wall_error_msg($api_url);
  if ($user_likes != FALSE) {
    // Gettings people likes list.
    for ($i = 0; $i < count($user_likes->data); $i++) {
      $rows[] = array(l($user_likes->data[$i]->name, $user_likes->data[$i]->link),
          '<img src=' . $user_likes->data[$i]->picture->data->url . '" width="50" />');
    }

    $header = array(
        'key' => array(
            'data' => t('Username'),
            'field' => 'role',
            'sort' => 'desc',
        ),
        'value' => array(
            'data' => t('Picture'),
        ),
    );

    $output = theme_table(
            array(
                "header" => $header,
                "rows" => $rows,
                "attributes" => array(),
                "sticky" => FALSE,
                "caption" => "",
                "colgroups" => array(),
                "empty" => t("No records available!"),
            )
    );
    return $output;
  }
}

/**
 * Function for getting the expiry time of the User Access Token.
 */
function facebook_wall_get_expiry() {
  $suffix = " Minutes";
  $time_diff = time() - variable_get('facebook_wall_access_token_start');
  $rem_time = variable_get('facebook_wall_access_token_expiry') - $time_diff;
  $time_remaining = (int) ($rem_time / 60);
  if ($time_remaining <= 0) {
    _facebook_wall_reset_token();
  }
  if ($time_remaining > 60) {
    $time_remaining = $time_remaining / 60;
    $suffix = " Hours";
    if ($time_remaining > 24) {
      $time_remaining = $time_remaining / 24;
      $suffix = " Days";
    }
  }
  $time_left['time'] = (int) ($time_remaining);
  $time_left['suffix'] = $suffix;
  return $time_left;
}

/**
 * Extending the User Access Token From 1 Hour to 2 Months.
 */
function _facebook_wall_get_access_token() {
  // Facebok App ID/API Key.
  $app_id = variable_get('facebook_wall_app_id');
  // Facebook App Secret.
  $app_secret = variable_get('facebook_wall_app_secret');

  // Get Access token for 2 months access.
  $url = 'https://graph.facebook.com/oauth/access_token?client_id=' . $app_id . '&client_secret=' . $app_secret . '&grant_type=fb_exchange_token&fb_exchange_token=' . variable_get('facebook_wall_access_token_temp');
  $response = drupal_http_request($url);
  $fb_data = json_decode($response->data);
  if (isset($fb_data->error)) {
      $message = $fb_data->error->type . ' : ' . $fb_data->error->code . ' ! ' . $fb_data->error->message;
      watchdog('facebook wall', $message, array(), WATCHDOG_DEBUG);
      if(variable_get('facebook_wall_message_show', 1)) {
          drupal_set_message($message, 'warning');
      }  

  } else {
    $response = $response->data;
    $access_data = explode("&", $response);
    $access_token = explode("=", $access_data[0]);
    $expiry = explode("=", $access_data[1]);

    if (isset($access_token[1])) {
      variable_set('facebook_wall_access_token', $access_token[1]);
      variable_set('facebook_wall_access_token_expiry', $expiry[1]);
      variable_set('facebook_wall_access_token_start', time());
      variable_set('facebook_wall_access_token_temp', ''); // unset temparoy toekn value  
    }
  }
}

/**
 * Function to set video auto play setting 
 */
function _facebook_video_settings($video_scouce) {

  if (variable_get('facebook_wall_autoplay_video', '0')) {
    $op = 'true';
  } else {
    $op = 'false';
  }

  if (stristr($video_scouce, 'autoplay=1')) {
    $video_scouce = str_replace('autoplay=1', 'autoplay=' . variable_get('facebook_wall_autoplay_video', '0'), $video_scouce);
  } elseif (stristr($video_scouce, '?autoplay=0')) {
    $video_scouce = str_replace('?autoplay=0', '?autoplay=' . variable_get('facebook_wall_autoplay_video', '0'), $video_scouce);
  } elseif (stristr($video_scouce, 'auto_play=true')) {
    $video_scouce = str_replace("auto_play=true", "auto_play=$op", $video_scouce);
  } elseif (stristr($video_scouce, 'auto_play=false')) {
    $video_scouce = str_replace("auto_play=false", "auto_play=$op", $video_scouce);
  } else {
  $video_scouce = '';  
  }
  return $video_scouce;
}

/**
 * Function for reset facebook Access tokens
 */
function _facebook_wall_reset_token() {
  variable_set('facebook_wall_access_token', '');
  variable_set('facebook_wall_access_token_expiry', '');
  variable_set('facebook_wall_access_token_start', '');
  variable_set('facebook_wall_access_token_temp', '');
}

/**
 * List permissions on access token of user.
 *
 * @return array
 *   Array of FB page basic on success and FALSE if it isn't.
 */
function _facebook_wall_access_token_permission_info_array() {
    // FB Graph API CURL calls request.
    $api_url = 'https://graph.facebook.com/'.FB_API_VER.'/me/permissions?access_token=' . FB_ACCESS_TOKEN;
    return _facebook_wall_error_msg($api_url);
}