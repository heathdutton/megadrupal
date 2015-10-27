<?php

/**
 * @file
 * wechat callback pages.
 */
 
/**
 * auto reply message
 */
function wechat_callback() {
  //watchdog('wechat', '123456'); 
  $we_obj = _wechat_init_obj();
  $we_obj->valid();
  $type = $we_obj->getRev()->getRevType();
  $request_data = $we_obj->getRevData();
  //watchdog('wechat', '123');
  $request_message = wechat_build_request_message($request_data);
  //watchdog('wechat', '123456'); 
  $response_message = wechat_build_response_message($request_message);
  $response_message->send();
  /* remove the old code.
  //print debug($request_message);
  module_invoke_all('wechat_message', $we_obj);
  module_invoke_all('wechat_message_alter', $we_obj);
  if (!is_array($we_obj->Message(''))) { // add default option
    $type = $we_obj->getRev()->getRevType();
    switch ($type)  {
      case Wechat::MSGTYPE_EVENT:
        $event = $we_obj->getRevEvent();
        if (!$event) {
          break;
        }
        elseif ($event['event']=='subscribe') {
          $we_obj->text(variable_get("wechat_follow_message", variable_get("wechat_default_message", 'Hi, WeChat!')));
        }
        elseif ($event['event']=='CLICK') {
          $message = _wechat_menu_default_message($event);
          $message && $we_obj->text($message);
        }
        break;
    }
  }
  if (!is_array($we_obj->Message(''))) {
    $we_obj->text(variable_get("wechat_default_message", "Hi, WeChat!"));
  }
  $we_obj->reply();
  */
  exit;
}


function wechat_build_response_message($request_message) {
  //转换回来
  $request_message_wrapper = entity_metadata_wrapper('wechat_request_message', $request_message);
  $response_message = entity_create('wechat_response_message', array('msg_type' => 'text'));
  $response_message_wrapper = entity_metadata_wrapper('wechat_response_message', $response_message);
  $response_message_wrapper->to_user_name->set($request_message_wrapper->from_user_name->value());
  $response_message_wrapper->from_user_name->set($request_message_wrapper->to_user_name->value());
  $response_message_wrapper->create_time->set(REQUEST_TIME);
  $response_message_wrapper->rm_id->set($request_message->id);
  //我们这里设置一个save标记，其它模块同构后面的钩子可以修改这个值，它表示保存相应消息到数据库中。也可以不保存。
  $response_message->save = TRUE;
  
  module_invoke_all('wechat_build_response_message', $response_message, $request_message);
  //default reply.
  $wechat_content = $response_message_wrapper->wechat_content->value();
  if ($response_message->msg_type == 'text' && empty($wechat_content)) {
    if ($request_message->msg_type == 'event') {
      if ($request_message_wrapper->wechat_event->value() == 'subscribe') {
        //watchdog('wechat', '123456'); 
        $default_message_content = variable_get("wechat_follow_message", variable_get("wechat_default_message", 'Hi, WeChat!'));
        $response_message_wrapper->wechat_content->set($default_message_content); 
      }
      elseif ($request_message_wrapper->wechat_event->value() == 'CLICK') {
        $event = array();
        $event['key'] = $request_message_wrapper->wechat_event_key->value();
		if($event['key'] == 'userbinding'){
          $default_message_content = "点击链接绑定用户：";
		  $default_message_content .= l('绑定账号', 'wechat/binding', array('query' => array('openid' => $request_message->from_user_name), 'absolute' => TRUE));
          $response_message_wrapper->wechat_content->set($default_message_content);		
		}else{
          $default_message_content = _wechat_menu_default_message($event);
          $response_message_wrapper->wechat_content->set($default_message_content);
		}
      }
    }
    $wechat_content = $response_message_wrapper->wechat_content->value();
    if (empty($wechat_content)) {
      $default_message_content = variable_get("wechat_default_message", "Hi, WeChat!");
      $response_message_wrapper->wechat_content->set($default_message_content);
    }
  }
  //The saved Flag default to TRUE. we could change it through hook_wechat_build_response_message.
  if (isset($response_message->save)) {
    $response_message_wrapper->save(true);
  }
  return $response_message;
}

/**
 * ****** unstable *****
 * login user
 */
function wechat_auth() {
  global $user;
  $we_obj = _wechat_init_obj();
  $state = isset($_GET['state']) ? $_GET['state'] : 0;
  $onerror = isset($_GET['onerror']) ? $_GET['onerror'] : '';
  $destination = isset($_GET['destination']) ? $_GET['destination'] : '';
  $code = isset($_GET['code']) ? $_GET['code'] : '';
  // user has logined
  if ($user->uid) {
    drupal_goto($destination);
  }
  
  // error
  if ((!$state) || (!$code)) { // error
    drupal_goto($onerror);
  }

  $access_data = $we_obj->getOauthAccessToken();
  if (empty($access_data)) {
    drupal_goto($onerror);
  }
  // check if already connectted
  if ($curuser = wechat_get_user_by_openid($access_data['openid'])) {
    _wechat_user_login($curuser->uid);
    // todo update user info
    if (empty($destination)) {
      $destination = 'user/' . $curuser->uid;
    }
    drupal_goto($destination);
  }
  
  // auto register
  $user_name = 'wechat_' . substr($access_data['openid'], 0, 15);
  while (user_load_by_name($user_name)) {
    $user_name = $user_name . rand(0, 100);
  }
  $new_user = array(
    'name' => $user_name,
    'pass' => user_password(),
    'mail' => $user_name . '@' . $_SERVER['SERVER_NAME'],
    'init' => $user_name . '@' . $_SERVER['SERVER_NAME'],
    'status' => 1,
  );
  // $account returns user object
  $account = user_save(null, $new_user);
  if (!$account) {
    drupal_goto($onerror);
  }
  _wechat_user_login($account->uid);
  $wechat_user = array(
    'uid' => $user->uid,
    'openid' => $access_data['openid'],
    'extend' => array(),
  );
  // also save user wechat information.
  if ($state == 2) {
    $user_info = $we_obj->getOauthUserinfo($access_data['access_token'], $access_data['openid']);
    if (!empty($user_info)) {
      module_invoke_all('wechat_userinfo', $user_info);
      $wechat_user['nickname'] = $user_info['nickname'];
      $wechat_user['sex'] = $user_info['sex'];
      $wechat_user['province'] = $user_info['province'];
      $wechat_user['city'] = $user_info['city'];
      $wechat_user['country'] = $user_info['country'];
      $wechat_user['headimgurl'] = $user_info['headimgurl'];
    }
  }
  if (!drupal_write_record('wechat_user', $wechat_user)) {
    drupal_goto($onerror);
  }

  drupal_goto($destination);
}

/**
 * find the default message
 */
function _wechat_menu_default_message($event) {
  // get all click menu key
  $key = isset($event['key']) ? $event['key'] : '';
  if (!$key) {
    return '';
  }
  $menu_tree = menu_build_tree('wechat', array('max_depth' => 2));
  foreach ($menu_tree as $val) {
    if ($val['link']['hidden']) {
      continue;
    }
    if (!empty($val['below'])) {
      foreach ($val['below'] as $subval) {
        if ($subval['link']['hidden']) {
          continue;
        }
        if ($subval['link']['options']['attributes']['wechat_key'] == $key) {
          return $subval['link']['options']['attributes']['wechat_default_message'];
        }
      }
    }
    elseif ($val['link']['options']['attributes']['wechat_key'] == $key) {
      return $val['link']['options']['attributes']['wechat_default_message'];
    }
  }
  return '';
}

/**
 * 从Drupal端绑定微信
 */
function wechat_user_binding_from_drupal($current_user) {
  global $user;
  //用户只能访问自己的微信绑定。
  if ($user->uid != $current_user->uid) {
    drupal_set_message(t("您无权访问别人的微信绑定页面."));
    drupal_goto('user/' . $user->uid);
  }
  $output = "";
  $wechat_user = db_query('SELECT * FROM {wechat_user} WHERE uid=:uid', array(':uid' => $current_user->uid))->fetchObject();
  //已经绑定了微信账号。
  if (isset($wechat_user->openid)) {
    $output .= '<p>' . t('您已经绑定了微信账号:') . $wechat_user->openid . '</p>';
  }
  else {
    $output .= '<p>' . t('使用微信，扫一扫下面的二维码，即可绑定微信账号.') . '</p>';
    $img_src = '';
    $access_token = wechat_get_access_token();
    $we_obj = _wechat_init_obj();
    //由于我们使用的wechat SDK，目前没有setAccessToken($access_token)，所以我们这里只能委屈一下了。
    $we_obj->checkAuth('', '', $access_token);
    //我们使用当前用户的uid作为scene_id，这样用户扫描后，我们收到消息，可以通过scene_id，获取到当前的uid。
    $ticket_array = $we_obj->getQRCode($current_user->uid);
    if (!empty($ticket_array['ticket'])) {
      $img_src = $we_obj->getQRUrl($ticket_array['ticket']);
      $output .= '<p><img src="' . $img_src . '" /></p>';
    }
  }
  return $output;
}
/**
 * 从微信端访问Drupal绑定账户
 */
function wechat_user_binding_from_wechat() {
  $output = "";
  global $user;
  if ($user->uid > 0) {
    $output = t('您已经成功登陆，已经绑定成功.');
    return $output;
  }
  $openid = isset($_GET['openid']) ? $_GET['openid'] : '';
  if (empty($openid)) {
    $output = t('非法的绑定链接.');
    return $output;
  }
  
  $current_user = wechat_get_user_by_openid($openid);
  if (!empty($current_user->uid)) {
    _wechat_user_login($current_user->uid);
    $output = t('您的微信号已经绑定了系统用户.');
    return $output;    
  }
  else {
    $binding_form = drupal_get_form('wechat_user_binding_form');
    $output .= drupal_render($binding_form);
    return $output;
  }
}
/**
 * Form builder; the main wechat user binding form.
 *
 * @ingroup forms
 */
function wechat_user_binding_form($form, &$form_state) {
  global $user;

  // If we are already logged on, go to the user page instead.
  if ($user->uid) {
    drupal_goto('user/' . $user->uid);
  }
  
  $openid = isset($_GET['openid']) ? $_GET['openid'] : '';
  // openid:
  $form['openid'] = array('#type' => 'value',
    '#value' => $openid,
  );
  // Display login form:
  $form['name'] = array('#type' => 'textfield',
    '#title' => t('Username'),
    '#size' => 60,
    '#maxlength' => USERNAME_MAX_LENGTH,
    '#required' => TRUE,
  );

  $form['name']['#description'] = t('Enter your @s username.', array('@s' => variable_get('site_name', 'Drupal')));
  $form['pass'] = array('#type' => 'password',
    '#title' => t('Password'),
    '#description' => t('Enter the password that accompanies your username.'),
    '#required' => TRUE,
  );
  $form['#validate'] = user_login_default_validators();
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Account binding'));

  return $form;
}

/**
 * Submit handler for the wechat_user_binding_form. Load $user object and perform standard login
 * tasks. The user is then redirected to the My Account page. Setting the
 * destination in the query string overrides the redirect.
 */
function wechat_user_binding_form_submit($form, &$form_state) {
  global $user;
  $user = user_load($form_state['uid']);
  $openid = $form_state['values']['openid'];
  $result = wechat_connect_user($user->uid, $openid);
  if (!empty($result)) {
    $message = t('已将您的微信号，与系统用户绑定，绑定的微信openid：') . $openid;
    drupal_set_message($message);       
  }
  else {
    $message = t('未能将您的微信号，与系统用户绑定，尝试绑定的微信openid：') . $openid;
    drupal_set_message($message);
  }  
  $form_state['redirect'] = 'user/' . $user->uid;

  user_login_finalize($form_state);
}

function wechat_get_user_list() {
  $next_openid = isset($_GET['next_openid']) ? $_GET['next_openid'] : '';
  $output = "";
  $we_obj = _wechat_init_obj();
  //由于我们使用的wechat SDK，目前没有setAccessToken($access_token)，所以我们这里只能委屈一下了
  $access_token = wechat_get_access_token();
  $we_obj->checkAuth('', '', $access_token);
  $result = $we_obj->getUserList($next_openid);
  if (empty($result)) {
    $output = "没有找到关注的微信用户";
    return $output;
  }
  //print debug($result);
  if (!empty($result['total'])) {
    $output .= "<p>关注的微信用户总数:" . $result['total'] . '</p>';
    $output .= "<p>本次获取的微信用户总数:" . $result['count'] . '</p>';
    $output .= "<p>next_openid:" . $result['next_openid'] . '</p>';
    $output .= "<table>";
    $output .="<tr><td>序号</td><td>openid</td><td>更新用户信息操作</td></tr>";
    foreach ($result['data']['openid'] as $key => $openid) {
      $output .= "<tr><td>" . $key . "</td><td>" . $openid . "</td><td>" . l(t('更新'), 'admin/wechat/user/update/' . $openid) . "</td></tr>";
    }
    $output .= "</table>";
  } 
  
  return $output;
}

function wechat_update_user_info($openid) {
  $we_obj = _wechat_init_obj();
  //由于我们使用的wechat SDK，目前没有setAccessToken($access_token)，所以我们这里只能委屈一下了
  $access_token = wechat_get_access_token();
  $we_obj->checkAuth('', '', $access_token);
  $user_info = $we_obj->getUserInfo($openid);
  //strip special chars, especially 4-Bytes UTF8 chars.
  $user_info['nickname'] = _wechat_strip_utf8mb4(addslashes($user_info['nickname']), '-');
  $output = "";
  if ($user_info) {
    db_merge('wechat_user')
      ->key(array('openid' => $user_info['openid']))
      ->updateFields(array(
          'nickname' => $user_info['nickname'],
          'sex' => $user_info['sex'],
          'province' => $user_info['province'],
          'city' => $user_info['city'],
          'country' => $user_info['country'],
          'headimgurl' => $user_info['headimgurl'],
      ))
      ->insertFields(array(
          'openid' => $user_info['openid'],
          'nickname' => $user_info['nickname'],
          'sex' => $user_info['sex'],
          'province' => $user_info['province'],
          'city' => $user_info['city'],
          'country' => $user_info['country'],
          'headimgurl' => $user_info['headimgurl'],
          'uid' => 0,
          'subscribe' => $user_info['subscribe'],
          'subscribe_time' => $user_info['subscribe_time'],
      ))
      ->execute();
      $output .= t("更新成功，当前昵称：") . $user_info['nickname'];

  }
  return $output;
}

function wechat_get_groups() {
  $output = "";
  $we_obj = _wechat_init_obj();
  //由于我们使用的wechat SDK，目前没有setAccessToken($access_token)，所以我们这里只能委屈一下了
  $access_token = wechat_get_access_token();
  $we_obj->checkAuth('', '', $access_token);
  $result = $we_obj->getGroup();
  if (empty($result)) {
    $output = "没有找到微信分组";
    return $output;
  }
  //print debug($result);
  if (!empty($result['groups'])) {
    $output .= "<table>";
    $output .="<tr><td>id</td><td>name</td><td>用户总数</td><td>操作</td></tr>";
    foreach ($result['groups'] as $key => $group) {
      $output .= "<tr><td>" . $group['id'] . "</td><td>" . $group['name'] . "</td><td>" . $group['count'] . "</td><td>" . l('编辑', 'admin/wechat/group/edit/' . $group['id'] . '/' . $group['name']) . "</td></tr>";
    }
    $output .= "</table>";
  } 
  
  return $output;
}

function wechat_group_add_form() {

  $form['name'] = array('#type' => 'textfield',
    '#title' => t('Group name'),
    '#size' => 60,
    '#required' => TRUE,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Submit'));

  return $form;
}

function wechat_group_add_form_submit($form, &$form_state) {
  $name = $form_state['values']['name'];
  $output = "";
  $we_obj = _wechat_init_obj();
  //由于我们使用的wechat SDK，目前没有setAccessToken($access_token)，所以我们这里只能委屈一下了
  $access_token = wechat_get_access_token();
  $we_obj->checkAuth('', '', $access_token);
  $result = $we_obj->createGroup($name);
  if (!empty($result['group'])) {
    drupal_set_message('id:' . $result['group']['id']);
    drupal_set_message('name:' . $result['group']['name']);
  }
  $form_state['redirect'] = 'admin/wechat/groups';
}

function wechat_group_edit_form($form, &$form_state, $id, $name) {

  $form['id'] = array('#type' => 'textfield',
    '#title' => t('Group ID'),
    '#size' => 60,
    '#default_value' => $id,
    '#disabled' => TRUE,
    '#required' => TRUE,
  );
  
  $form['name'] = array('#type' => 'textfield',
    '#title' => t('Group name'),
    '#size' => 60,
    '#default_value' => $name,
    '#required' => TRUE,
  );
  
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Submit'));

  return $form;
}

function wechat_group_edit_form_submit($form, &$form_state) {
  $id = $form_state['values']['id'];
  $name = $form_state['values']['name'];
  $output = "";
  $we_obj = _wechat_init_obj();
  //由于我们使用的wechat SDK，目前没有setAccessToken($access_token)，所以我们这里只能委屈一下了
  $access_token = wechat_get_access_token();
  $we_obj->checkAuth('', '', $access_token);
  $result = $we_obj->updateGroup($id, $name);
  if (!empty($result)) {
    drupal_set_message(t('update success'));
  }
  else {
    drupal_set_message(t('update failed, reason:') . $we_obj->errMsg);
  }
  $form_state['redirect'] = 'admin/wechat/groups';
}

function wechat_group_member_update_form($form, &$form_state, $openid) {

  $form['openid'] = array('#type' => 'textfield',
    '#title' => t('openid'),
    '#size' => 60,
    '#default_value' => $openid,
    '#disabled' => TRUE,
    '#required' => TRUE,
  );
  $group_options = array(
    '0' => '未分组',
    '1' => '黑名单',
    '2' => '星标组',
  );
  $form['groupid'] = array('#type' => 'select',
    '#title' => t('Group'),
    '#options' => $group_options,
    '#default_value' => 0,
    '#required' => TRUE,
  );
  
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Submit'));

  return $form;
}

function wechat_group_member_update_form_submit($form, &$form_state) {
  $openid = $form_state['values']['openid'];
  $groupid = $form_state['values']['groupid'];
  $output = "";
  $we_obj = _wechat_init_obj();
  //由于我们使用的wechat SDK，目前没有setAccessToken($access_token)，所以我们这里只能委屈一下了
  $access_token = wechat_get_access_token();
  $we_obj->checkAuth('', '', $access_token);
  $result = $we_obj->updateGroupMembers($groupid, $openid);
  if (!empty($result)) {
    drupal_set_message(t('update success'));
  }
  else {
    drupal_set_message(t('update failed, reason:') . $we_obj->errMsg);
  }
  $form_state['redirect'] = 'admin/wechat/userlist';
}

/**
 * 微信返回用户名的nickname会包含特殊字符，在特定的mysql版本下会报错，所以需要预处理nickname，做好兼容处理。
 * Helper function to Strip 4-byte UTF8 characters.
 * copy from http://cgit.drupalcode.org/strip_utf8mb4/tree/strip_utf8mb4.module
 *
 */
function _wechat_strip_utf8mb4($text_data, $replace_text ='') {
  $replacements_done = array();
  // Strip overly long 2 byte sequences, as well as characters
  //  above U+10000 and replace with $replace_text
  $processed_text_data = preg_replace('/[\x00-\x08\x10\x0B\x0C\x0E-\x19\x7F]' .
    '|[\x00-\x7F][\x80-\xBF]+' .
    '|([\xC0\xC1]|[\xF0-\xFF])[\x80-\xBF]*' .
    '|[\xC2-\xDF]((?![\x80-\xBF])|[\x80-\xBF]{2,})' .
    '|[\xE0-\xEF](([\x80-\xBF](?![\x80-\xBF]))|(?![\x80-\xBF]{2})|[\x80-\xBF]{3,})/S',
    $replace_text, $text_data, -1, $replacements_done[]);

  // Strip overly long 3 byte sequences and UTF-16 surrogates and replace with $replace_text
  $processed_text_data = preg_replace('/\xE0[\x80-\x9F][\x80-\xBF]' .
                                          '|\xED[\xA0-\xBF][\x80-\xBF]/S', $replace_text, $processed_text_data, -1, $replacements_done[]);
  
  if (array_sum($replacements_done) > 0) {
    $message = t('Unsupported characters in your text were replaced with "!replacement"', array('!replacement' => $replace_text));
    drupal_set_message($message, 'warning', FALSE);
  }

  return $processed_text_data;
}

function wechat_binding_from_wechat(){
  global $user;

  // If we are already logged on, go to the user page instead.
  if ($user->uid) {
    drupal_goto('user/' . $user->uid);
  }
  
  $output = "";
  $state = isset($_GET['state']) ? $_GET['state'] : 0;
  $code = isset($_GET['code']) ? $_GET['code'] : '';
  $wechat_bind_code = isset($_SESSION['wechat_bind_code']) ? $_SESSION['wechat_bind_code'] : '';  
  if($wechat_bind_code!=$code){
    $we_obj = _wechat_init_obj();
    $access_data = $we_obj->getOauthAccessToken();
    if (empty($access_data['openid'])) {
      $output = "系统出现错误，请重试123";
	  return  $output;
    }
    $openid = $access_data['openid'];
	$_SESSION['wechat_bind_code'] = $code;
	$_SESSION['wechat_bind_openid'] = $openid;
  }
  else{
     $openid = isset($_SESSION['wechat_bind_openid']) ? $_SESSION['wechat_bind_openid'] : '';
  }
  
  //$output .= "openid：" .$openid;
  // check if already connectted
  $curuser = wechat_get_user_by_openid($openid);
  if ($curuser) {
    _wechat_user_login($curuser->uid);
    // todo update user info
    if (empty($destination)) {
      $destination = 'user/' . $curuser->uid;
    }
    drupal_goto($destination);
  }else{
     $output .= "";
	 $wechat_binding_from_wechat_auto_form = drupal_get_form('wechat_binding_from_wechat_auto_form', $openid);
	 $output .= "<div class='wechat-binding-auto-form'>" . drupal_render($wechat_binding_from_wechat_auto_form) .  '</div>';
	 $wechat_binding_from_wechat_bind_form = drupal_get_form('wechat_binding_from_wechat_bind_form', $openid);
	 $output .= "<div class='wechat-binding-bind-form'>" . drupal_render($wechat_binding_from_wechat_bind_form) .  '</div>';
	 $wechat_binding_from_wechat_register_form = drupal_get_form('wechat_binding_from_wechat_register_form', $openid);
	 $output .= "<div class='wechat-binding-register-form'>" . drupal_render($wechat_binding_from_wechat_register_form) .  '</div>';	 
  }  
  return $output;
}

/**
 * 自动创建用户名，将创建的用户名，与微信用户绑定
 *
 */
function wechat_binding_from_wechat_auto_form($form, &$form_state, $openid) {

  // openid:
  $form['openid'] = array('#type' => 'value',
    '#value' => $openid,
  );
  $form['#attached']['css'] = array(
    drupal_get_path('module', 'wechat') . '/wechat.css',
  );  
  $form['#submit'][] = 'wechat_binding_from_wechat_auto_form_submit';
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('自动创建账号并绑定'));

  return $form;
}

function wechat_binding_from_wechat_auto_form_submit($form, &$form_state) {

  // drupal_set_message('123456'); 
  //watchdog('wechat_binding','wechat_binding_from_wechat_auto_form_submit' );
  // auto register
  $openid = $form_state['values']['openid'];
  $user_name = 'wechat_' . substr($openid, 0, 15);
  while (user_load_by_name($user_name)) {
    $user_name = $user_name . rand(0, 100);
  }
  $new_user = array(
    'name' => $user_name,
    'pass' => user_password(),
    'mail' => $user_name . '@' . $_SERVER['SERVER_NAME'],
    'init' => $user_name . '@' . $_SERVER['SERVER_NAME'],
    'status' => 1,
  );
  // $account returns user object
  $account = user_save(null, $new_user);
  if (!$account) {
    $message = t('未能将您的微信号，与系统自动生成的用户绑定，尝试绑定的微信openid：') . $openid;
    drupal_set_message($message); 
	return;
  }
  $ret = wechat_connect_user($account->uid, $openid);
  if(!empty($ret)){
    _wechat_user_login($account->uid);
	$form_state['redirect'] = 'user/' . $account->uid;
  }else{
    $message = t('未能将您的微信号，与系统自动生成的用户绑定，尝试绑定的微信openid：') . $openid;
    drupal_set_message($message);  
  }
  
}
/**
 * 将微信用户与Drupal网站后台已有用户绑定
 *
 */
function wechat_binding_from_wechat_bind_form($form, &$form_state, $openid) {

  // openid:
  $form['openid'] = array('#type' => 'value',
    '#value' => $openid,
  );
  // Display login form:
  $form['name'] = array('#type' => 'textfield',
    '#title' => t('用户名'),
    '#size' => 60,
    '#maxlength' => USERNAME_MAX_LENGTH,
    '#required' => TRUE,
  );

  $form['name']['#description'] = t('请输入您的用户名.');
  $form['pass'] = array('#type' => 'password',
    '#title' => t('密码'),
    '#description' => t('请输入用户名对应的密码.'),
    '#required' => TRUE,
  );
  $form['#validate'] = user_login_default_validators();
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('绑定已有账号'));
  $form['#submit'][] = 'wechat_binding_from_wechat_bind_form_submit';
  return $form;
}

/**
 * Submit handler for the wechat_user_binding_form. Load $user object and perform standard login
 * tasks. The user is then redirected to the My Account page. Setting the
 * destination in the query string overrides the redirect.
 */
function wechat_binding_from_wechat_bind_form_submit($form, &$form_state) {
  global $user;
  $user = user_load($form_state['uid']);
  $openid = $form_state['values']['openid'];
  $result = wechat_connect_user($user->uid, $openid);
  if (!empty($result)) {
    $message = t('已将您的微信号，与系统用户绑定，绑定的微信openid：') . $openid;
    drupal_set_message($message);       
  }
  else {
    $message = t('未能将您的微信号，与系统用户绑定，尝试绑定的微信openid：') . $openid;
    drupal_set_message($message);
  }  
  $form_state['redirect'] = 'user/' . $user->uid;

  user_login_finalize($form_state);
}

/**
 * 将微信用户,与新注册的Drupal网站后台用户绑定
 *
 */
function wechat_binding_from_wechat_register_form($form, &$form_state, $openid) {

  // openid:
  $form['openid'] = array('#type' => 'value',
    '#value' => $openid,
  );
  // Account information.
  $form['account'] = array(
    '#type'   => 'container',
    '#weight' => -10,
  );
  // Only show name field on registration form or user can change own username.
  $form['account']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#maxlength' => USERNAME_MAX_LENGTH,
    '#description' => t('Spaces are allowed; punctuation is not allowed except for periods, hyphens, apostrophes, and underscores.'),
    '#required' => TRUE,
    '#attributes' => array('class' => array('username')),
    '#weight' => -10,
  );

  $form['account']['mail'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail address'),
    '#maxlength' => EMAIL_MAX_LENGTH,
    '#description' => t('A valid e-mail address. All e-mails from the system will be sent to this address. The e-mail address is not made public and will only be used if you wish to receive a new password or wish to receive certain news or notifications by e-mail.'),
    '#required' => TRUE,
  );
  $form['account']['pass'] = array(
    '#type' => 'password_confirm',
    '#size' => 25,
    '#description' => t('To change the current user password, enter the new password in both fields.'),
  );  
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('创建新账号并绑定'));

  return $form;
}

/**
 * Form validation handler for user_account_form().
 *
 * @see user_account_form()
 */
function wechat_binding_from_wechat_register_form_validate($form, &$form_state) {
  if (isset($form_state['values']['name'])) {
    if ($error = user_validate_name($form_state['values']['name'])) {
      form_set_error('name', $error);
    }
    elseif ((bool) db_select('users')->fields('users', array('uid'))->condition('name', db_like($form_state['values']['name']), 'LIKE')->range(0, 1)->execute()->fetchField()) {
      form_set_error('name', t('The name %name is already taken.', array('%name' => $form_state['values']['name'])));
    }
  }
	
  $mail = trim($form_state['values']['mail']);
  form_set_value($form['account']['mail'], $mail, $form_state);

  // Validate the e-mail address, and check if it is taken by an existing user.
  if ($error = user_validate_mail($form_state['values']['mail'])) {
    form_set_error('mail', $error);
  }
  elseif ((bool) db_select('users')->fields('users', array('uid'))->condition('mail', db_like($form_state['values']['mail']), 'LIKE')->range(0, 1)->execute()->fetchField()) {
    form_set_error('mail', t('The e-mail address %email is already registered. <a href="@password">Have you forgotten your password?</a>', array('%email' => $form_state['values']['mail'], '@password' => url('user/password'))));
  }

}

function wechat_binding_from_wechat_register_form_submit($form, &$form_state) {
  // auto register
  $openid = $form_state['values']['openid'];
  $name = $form_state['values']['name'];
  $pass = $form_state['values']['pass'];
  $mail = $form_state['values']['mail'];
  $new_user = array(
    'name' => $name,
    'pass' => $pass,
    'mail' => $mail,
    'init' => $mail,
    'status' => 1,
  );
  // $account returns user object
  $account = user_save(null, $new_user);
  if (!$account) {
    $message = t('未能将您的微信号，与现在注册的用户绑定，尝试绑定的微信openid：') . $openid;
    drupal_set_message($message); 
	return;
  }
  $ret = wechat_connect_user($account->uid, $openid);
  if(!empty($ret)){
    _wechat_user_login($account->uid);
	$form_state['redirect'] = 'user/' . $account->uid;
  }else{
    $message = t('未能将您的微信号，与现在注册的用户绑定，尝试绑定的微信openid：') . $openid;
    drupal_set_message($message);  
  }
  
}
