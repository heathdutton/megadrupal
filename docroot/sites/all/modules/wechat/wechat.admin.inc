<?php

/**
 * @file
 * wechat admin pages.
 */
 
function wechat_config_form($form, $form_state) {
  $form['wechat_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Token'),
    '#description' => t('The token that you use in wechat public platform.'),
    '#default_value' => variable_get('wechat_token', ''),
    '#required' => TRUE,
  );
  $form['wechat_appid'] = array(
    '#type' => 'textfield',
    '#title' => t('AppID'),
    '#description' => t('Account APPID'),
    '#default_value' => variable_get('wechat_appid', ""),
    '#required' => TRUE,
  );
  $form['wechat_appsecret'] = array(
    '#type' => 'textfield',
    '#title' => t('AppSecret'),
    '#description' => t('Account AppSecret'),
    '#default_value' => variable_get('wechat_appsecret', ""),
    '#required' => TRUE,
  );
  return system_settings_form($form);
}
function wechat_default_message_config_form($form, $form_state) {
  $form['wechat_default_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Auto reply message'),
    '#description' => t('The WeChat module will send this message to user if no other Modules'),
    '#default_value' => variable_get('wechat_default_message', "hello, I'm wechat"),
    '#required' => TRUE,
  );
  $form['wechat_follow_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Auto reply message after follow.'),
    '#description' => '',
    '#default_value' => variable_get('wechat_follow_message', "hello, I'm wechat"),
    '#required' => FALSE,
  );
  return system_settings_form($form);
}

function wechat_update_menu_form($form, $form_state) {
  $wechat_menu = l('Wechat Menu', 'admin/structure/menu/manage/wechat', array('attributes' => array('target' => '_blank')));
  $form['#prefix'] = t('管理菜单，请点击!url;<br />完成编辑后，点击这里更新菜单。（微信菜单不能做到实时更新，会有一定延时。)', array('!url' => $wechat_menu));
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update Menu'),
    '#submit' => array('wechat_admin_update_menu_submit'),
  );
  return $form;
}

/**
 * update wechat menu
 */
function wechat_admin_update_menu_submit($form, &$form_state) {
  $menu_arr = _wechat_menu_arr();
  if (empty($menu_arr['button'])) {
    drupal_set_message(t('Empty WeChat Menu'), 'error');
  }
  else {
    $we_obj = _wechat_init_obj();
    if ($we_obj->createMenu($menu_arr)) {
      drupal_set_message(t('Update menu success.'));
    }
    else {
      drupal_set_message($we_obj->errMsg . '-' . $we_obj->errCode, 'error');
    }
  }
  return;
}

/**
 * drupal menu to wechat menu array
 */
function _wechat_menu_arr() {
  $menu_tree = menu_build_tree('wechat', array('max_depth' => 2));
  $menu_arr = array();
  $menu_arr['button'] = array();
  foreach ($menu_tree as $val) {
    if ($val['link']['hidden']) {
      continue;
    }
    $element = array();
    $element['name'] = $val['link']['link_title'];
    $element['sub_button'] = array();
    if (!empty($val['below'])) {
      foreach ($val['below'] as $subval) {
        if ($subval['link']['hidden']) {
          continue;
        }
        $element['sub_button'][] = _wechat_menu_item($subval);
      }
    }
    if (empty($element['sub_button'])) {
      unset($element['sub_button']);
      $element = _wechat_menu_item($val);
    }
    $menu_arr['button'][] = $element;
  }
  return $menu_arr;
}

/**
 * Drupal menu to wechat menu
 */
function _wechat_menu_item($subval) {
  $subelement = array();
  $subelement['name'] = $subval['link']['link_title'];
  if (isset($subval['link']['options']['attributes']['wechat_type'])
  && $subval['link']['options']['attributes']['wechat_type'] == 'view') {
   // $subelement['type'] = 'click';
  //  $subelement['key'] = $subval['link']['options']['attributes']['wechat_key'];
    $subelement['type'] = 'view';
    $subelement['url'] = $subval['link']['options']['attributes']['wechat_url'];	
  }
  else {
    $subelement['type'] = $subval['link']['options']['attributes']['wechat_type'];
    $subelement['key'] = $subval['link']['options']['attributes']['wechat_key'];
  }
  return $subelement;
}

/**
 * UI controller.
 */
 
class WechatRequestMessageTypeUIController extends EntityDefaultUIController {

}

/**
 * UI controller.
 */
 
class WechatResponseMessageTypeUIController extends EntityDefaultUIController {

}

/**
 * Generates the profile type editing form.
 */
function wechat_request_message_type_form($form, &$form_state, $message_type, $op = 'edit') {

  if ($op == 'clone') {
    $message_type->label .= ' (cloned)';
    $message_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $message_type->label,
    '#description' => t('The human-readable name of this message type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($message_type->type) ? $message_type->type : '',
    '#maxlength' => 32,
    '#disabled' => $message_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'wechat_request_message_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this message type. It must only contain lowercase letters, numbers, and underscores.'),
  );


  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save message type'),
    '#weight' => 40,
  );
  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => $message_type->weight,
    '#description' => t('When showing message types, those with lighter (smaller) weights get listed before message types with heavier (larger) weights.'),
    '#weight' => 10,
  );

  if (!$message_type->isLocked() && $op != 'add' && $op != 'clone') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete message type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('wechat_request_message_type_form_submit_delete')
    );
  }
  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function wechat_request_message_type_form_submit(&$form, &$form_state) {
  $wechat_request_message_type = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  $wechat_request_message_type->save();
  $form_state['redirect'] = 'admin/wechat/config/request-messages';
}

/**
 * Form API submit callback for the delete button.
 */
function wechat_request_message_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/wechat/config/request-messages/manage/' . $form_state['message_type']->type . '/delete';
}

/**
 * Generates the profile type editing form.
 */
function wechat_response_message_type_form($form, &$form_state, $message_type, $op = 'edit') {
  //print debug($message_type);
  if ($op == 'clone') {
    $message_type->label .= ' (cloned)';
    $message_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $message_type->label,
    '#description' => t('The human-readable name of this message type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($message_type->type) ? $message_type->type : '',
    '#maxlength' => 32,
    '#disabled' => $message_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'wechat_response_message_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this message type. It must only contain lowercase letters, numbers, and underscores.'),
  );


  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save message type'),
    '#weight' => 40,
    '#submit' => array('wechat_response_message_type_form_submit')
  );
  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => $message_type->weight,
    '#description' => t('When showing message types, those with lighter (smaller) weights get listed before message types with heavier (larger) weights.'),
    '#weight' => 10,
  );

  if (!$message_type->isLocked() && $op != 'add' && $op != 'clone') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete message type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('wechat_response_message_type_form_submit_delete')
    );
  }
  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function wechat_response_message_type_form_submit(&$form, &$form_state) {
  $wechat_response_message_type = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  //print debug($wechat_response_message_type);
  $wechat_response_message_type->save();
  $form_state['redirect'] = 'admin/wechat/config/response-messages';
}

/**
 * Form API submit callback for the delete button.
 */
function wechat_response_message_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/wechat/config/response-messages/manage/' . $form_state['message_type']->type . '/delete';
}

/**
 * Returns a breadcrumb submission form.
 */
function wechat_response_message_add($type) {
  $message = entity_create('wechat_response_message', array('msg_type' => $type));;

  if (isset($_GET['rm_id'])) {
    $message->rm_id = $_GET['rm_id'];
  }

  $output = drupal_get_form('wechat_response_message_edit_form', $message);

  return $output;
}

/**
 * Form callback: create or edit a wechat_response_message.
 *
 * @param $message
 *   The message object to edit.
 */
function wechat_response_message_edit_form($form, &$form_state, $message) {
  // Add the default field elements.
  if (empty($message->rm_id)) {
    $form['to_user_name'] = array(
      '#type' => 'textfield',
      '#title' => t('To user name'),
      //'#default_value' => isset($message->to_user_name) ? $message->to_user_name : '',
      '#maxlength' => 255,
      '#required' => TRUE,
      '#weight' => -5,
    );
  }
  else{
    $request_message = wechat_request_message_load($message->rm_id);
    $message->to_user_name = $request_message->from_user_name;
    $message->from_user_name = $request_message->to_user_name;
  }

  // Add the field related form elements.
  $form_state['message'] = $message;
  field_attach_form('wechat_response_message', $message, $form, $form_state);

  $form['#attached']['css'] = array(
    drupal_get_path('module', 'wechat') . '/wechat.css',
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 40,
  );
  if (!empty($message->id)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete message'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('wechat_response_message_edit_form_submit_delete')
    );
  }

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'wechat_response_message_edit_form_validate';
  return $form;
}


/**
 * Form API validate callback for the model form
 */
function wechat_response_message_edit_form_validate(&$form, &$form_state) {
  $message = $form_state['message'];
  
  // Notify field widgets to validate their data.
  field_attach_form_validate('wechat_response_message', $message, $form, $form_state);
}


/**
 * Form API submit callback for the model form.
 * 
 * @todo remove hard-coded link
 */
function wechat_response_message_edit_form_submit(&$form, &$form_state) {
  
  $message = &$form_state['message'];
  
  //$message->to_user_name = $form_state['values']['to_user_name'];
  
  // Notify field widgets.
  field_attach_submit('wechat_response_message', $message, $form, $form_state);
  
  // Save the breadcrumb
  wechat_response_message_save($message);
  
 // $array_data = wechat_response_message_obj_to_array($message);
  $message->sendCustomMessage();
  /*
  $appid = variable_get('wechat_appid', "");
  $appsecret =variable_get('wechat_appsecret', "");
  $access_token = wechat_get_access_token($appid, $appsecret);
  $we_obj = _wechat_init_obj();
  $we_obj->checkAuth($appid, $appsecret, $access_token);
  $result = $we_obj->sendCustomMessage($array_data);
  //$result = $we_obj->http_post('https://api.weixin.qq.com/cgi-bin/message/custom/send?access_token=' . $access_token, $json_data);

  if (!empty($result)) {
    //Todo: Error.
  }
  */
  drupal_set_message(t('wechat_response_message saved.'));
  $form_state['redirect'] = '';
}

