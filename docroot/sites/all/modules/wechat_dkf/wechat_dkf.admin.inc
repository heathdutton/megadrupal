<?php

/**
 * @file
 * wechat admin pages.
 */
/**
 * Returns a breadcrumb submission form.
 */
function wechat_dkf_wechatkf_callback() {
  $output = "客服列表";
  $kf_list = wechat_dkf_get_kflist();
  //print debug($kf_list);
  if (!empty($kf_list)) {
    $output .= '<table>';
	$output .= '<tr><td>账号</td><td>头像</td><td>ID</td><td>昵称</td></tr>';
	foreach($kf_list['kf_list'] as $kf){
	  $output .= '<tr>';
	  $output .= '<td>'. $kf["kf_account"].'</td>';
	  $output .= '<td><img src="'. $kf["kf_headimgurl"].'" width="50px" height="50px"/></td>';
	  $output .= '<td>'. $kf["kf_id"].'</td>';
	  $output .= '<td>'. $kf["kf_nick"].'</td>';
	  $output .= '</tr>';
	}
	$output .= '</table>';
  }

  return $output;
} 

/**
 * Returns a breadcrumb submission form.
 */
function wechat_dkf_wechatkf_add() {
  $wechat_kf = entity_create('wechat_kf', array('type' => 'wechat_kf'));;

  $output = drupal_get_form('wechat_dkf_wechatkf_edit_form', $wechat_kf);

  return $output;
}

/**
 * Returns a breadcrumb submission form.
 */
function wechat_dkf_wechatkf_edit() {
  $id = arg(5);
  //$wechat_kf = entity_create('wechat_kf', array('type' => 'wechat_kf'));;
  $wechat_kf = wechat_dkf_wechatkf_load($id);
  $output = drupal_get_form('wechat_dkf_wechatkf_edit_form', $wechat_kf);

  return $output;
}
/**
 * Form callback: create or edit a wechat_response_message.
 *
 * @param $wechat_kf
 *   The wechat_kf object to edit.
 */
function wechat_dkf_wechatkf_edit_form($form, &$form_state, $wechat_kf) {
  // Add the default field elements.
  $form['kf_account'] = array(
    '#type' => 'textfield',
    '#title' => t('Kf account'),
    '#default_value' => isset($wechat_kf->kf_account) ? $wechat_kf->kf_account : '',
	'#disabled' => empty($wechat_kf->kf_account) ? FALSE : TRUE,
    '#maxlength' => 255,
    '#required' => TRUE,
  );
  
  $form['nickname'] = array(
    '#type' => 'textfield',
    '#title' => t('Nickname'),
    '#default_value' => isset($wechat_kf->nickname) ? $wechat_kf->nickname : '',
    '#maxlength' => 255,
    '#required' => TRUE,
  );
  
  $form['password'] = array(
    '#type' => 'textfield',
    '#title' => t('Password'),
    '#default_value' => isset($wechat_kf->password) ? $wechat_kf->password : '',
    '#maxlength' => 255,
    '#required' => TRUE,
  );  
  // Add the field related form elements.
  $form_state['wechat_kf'] = $wechat_kf;
  field_attach_form('wechat_kf', $wechat_kf, $form, $form_state);

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 40,
  );
  if (!empty($wechat_kf->id)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete wechat kf'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('wechat_dkf_wechatkf_edit_form_submit_delete')
    );
  }

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'wechat_dkf_wechatkf_edit_form_validate';
  return $form;
}


/**
 * Form API validate callback for the model form
 */
function wechat_dkf_wechatkf_edit_form_validate(&$form, &$form_state) {
  $wechat_kf = $form_state['wechat_kf'];
  
  // Notify field widgets to validate their data.
  field_attach_form_validate('wechat_kf', $wechat_kf, $form, $form_state);
}


/**
 * Form API submit callback for the model form.
 * 
 * @todo remove hard-coded link
 */
function wechat_dkf_wechatkf_edit_form_submit(&$form, &$form_state) {
  
  $wechat_kf = &$form_state['wechat_kf'];
  $wechat_kf->kf_account = $form_state['values']['kf_account'];
  $wechat_kf->nickname = $form_state['values']['nickname'];
  $wechat_kf->password = $form_state['values']['password'];
  $wechat_kf->kf_id = "";
  // Notify field widgets.
  field_attach_submit('wechat_kf', $wechat_kf, $form, $form_state);
  //print debug($wechat_kf);
  // Save the wechat_kf
  wechat_dkf_wechatkf_save($wechat_kf);
  if (!empty($wechat_kf->id)) {
    wechat_dkf_update_kf_account($wechat_kf);
  }
  else {
    wechat_dkf_add_kf_account($wechat_kf);
  }
 // $form_state['redirect'] = '';
}

function wechat_dkf_wechatkf_edit_form_submit_delete(&$form, &$form_state) {
  
  $wechat_kf = &$form_state['wechat_kf'];
  wechat_dkf_wechatkf_delete($wechat_kf);
  wechat_dkf_delete_kf_account($wechat_kf);

  $form_state['redirect'] = 'admin/wechat/config';
}

