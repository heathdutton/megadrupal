<?php

/**
 * @file
 * User interface routines for choosing a user from a list
 */

function mm_ui_user_list_form($mmtids) {
  if (!user_access('access user profiles'))
    return '';

  $title = t('User Search');
  drupal_set_title($title, PASS_THROUGH);
  $content = array(array(
    '#theme' => 'html_tag',
    '#tag' => 'h2',
    '#value' => $title,
    '#attributes' => array(),
  ));
  $content['user_list'] = array(
    '#prefix' => '<div class="content">',
    '#suffix' => '</div>',
  );
  $content['user_list']['form'] = drupal_get_form('mm_ui_user_list');
  return $content;
}

function mm_ui_user_list($form, &$form_state) {
  _mm_ui_userlist_setup(array(0 => ''), $form, 'userlist', t("User's name:"), TRUE, '');
  $form['userlist-choose']['#title'] = '';
  $form['userlist']['#mm_list_hide_left_pane'] = TRUE;
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('View user')
  );
  return $form;
}

function mm_ui_user_list_submit($form, &$form_state) {
  $uid = mm_ui_mmlist_key0($form_state['values']['userlist']);

  if (isset($uid) && ($home = mm_content_get(array('f.flag' => 'user_home', 'f.data' => $uid), MM_GET_FLAGS))) {
    mm_redirect_to_mmtid($home[0]->mmtid);
    $form_state['redirect'] = '';
  }
  else {
    form_set_error('userlist-choose', t('The selected user does not have a homepage.'));
  }
}

