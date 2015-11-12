<?php

/** 
 * @file
 * Administration page callbacks for the drconnect module.
 */

/** 
 * Draugiem.lv Passport settings form.
 */
function drconnect_admin_settings() {
  $form['global'] = array(
    '#type' => 'fieldset',
    '#title' => t('Draugiem.lv API Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['global']['drconnect_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Draugiem.lv application ID'),
    '#default_value' => variable_get('drconnect_key', NULL),
    '#description' => t('Your application ID number'),
  );
  $form['global']['drconnect_skey'] = array(
    '#type' => 'textfield',
    '#title' => t('Draugiem.lv API key'),
    '#default_value' => variable_get('drconnect_skey', NULL),
    '#description' => t('Do not share your API key with anyone'),
  );
  $form['site'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings for !site', array('!site' => variable_get('site_name', t('Your Site')))),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  // Login button image
  $form['site']['drconnect_login_button'] = array(
    '#type' => 'container',
  );
  $form['site']['drconnect_login_button']['drconnect_login_button_style'] = array(
    '#type' => 'radios',
    '#title' => t('Login button image'),
    '#options' => array(
      'default' => t('Default'),
      'custom' => t('Custom'),
    ),
    '#default_value' => variable_get('drconnect_login_button_style', 'default'),
  );
  $form['site']['drconnect_login_button']['drconnect_login_button_img'] = array(
    '#title' => t('Custom login button style'),
    '#type' => 'radios',
    '#default_value' => variable_get('drconnect_login_button_img', 1),
    '#states' => array(
      'visible' => array(
        ':input[name="drconnect_login_button_style"]' => array(
          'value' => 'custom',
        ),
      ),
    ),
  );
  $imgpath = 'http://www.draugiem.lv/applications/img/logos/passport/pase';
  $imgext = '.png';
  $i = 1;
  while (@GetImageSize($imgpath . $i . $imgext)) {
    $form['site']['drconnect_login_button']['drconnect_login_button_img']['#options'][$i] = '<img src="' . $imgpath . $i . $imgext .'">';
    $i++;
  }
  // Link Draugiem.lv profile to an existing account.
  $form['site']['drconnect_link_option'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show link option'),
    '#default_value' => variable_get('drconnect_link_option', 1),
    '#description' => t('Show option to link Draugiem.lv profile to an existing account'),
  );
  // Enable fast registration.
  $form['site']['drconnect_fast_register'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable fast user registration'),
    '#description' => t('New user account is created automatically (Doesn\'t require e-mail)'),
    '#default_value' => variable_get('drconnect_fast_register', 0),
  );
  $form = system_settings_form($form);
  return $form;
}

/**
 * Callback for Draugiem.lv Passport user settings tab.
 *
 * @param stdClass $account
 *   User object.
 */
function drconnect_account_link($account) {
  $druid = _get_user_druid($account->uid);
  $params = array();
  if (drconnect_draugiem_client(drconnect_api_filepath())->getSession()) {
    // Draugiem.lv session is active.
    $params['session'] = TRUE;
    if ($druid) {
      // Link between site account and Draugiem.lv profile exists.
      $action = 'unlink';
      $params['druid'] = $druid;
    }
    else {
      // No link between site account and Draugiem.lv profile.
      $action = 'link';
    }
  }
  else {
    // No active Draugiem.lv session.
    $params['session'] = FALSE;
    if ($druid) {
      // Link between site account and Draugiem.lv profile exists.
      $action = 'unlink';
      $params['druid'] = $druid;
    }
    else {
      // No link between site account and Draugiem.lv profile.
      $action = 'login';
    }
  }
  return drupal_get_form('drconnect_account_link_form', $action, $params);
}

/**
 * Draugiem.lv Passport user settings form.
 */
function drconnect_account_link_form($form, &$form_state, $action, $variables) {
  $form = array();
  $form['info'] = array(
    '#type' => 'container',
  );
  if ($action == 'unlink') {
    if ($variables['session']) {
      // Active Draugiem.lv session exists.
      drconnect_draugiem_client(drconnect_api_filepath())->getSession();
      $druid = drconnect_get_druid();
      if ($variables['druid'] == $druid) {
        // Account is linked with the Draugiem.lv active session profile.
        $druser = drconnect_draugiem_client(drconnect_api_filepath())->getUserData();
        $name = $druser['name'] . ' ' . $druser['surname'];
        $image = theme_image(array('path' => $druser['img'], 'alt' => $name, 'title' => $name, 'attributes' => array()));
        $form['info']['profile'] = array(
          '#markup' => t('Profile this account is linked with:'),
          '#prefix' => '<div>',
          '#suffix' => '</div>',
        );
        $form['info']['profile_name'] = array(
          '#markup' => $name,
          '#prefix' => '<div>',
          '#suffix' => '</div>',
        );
        $form['info']['profile_image'] = array(
          '#markup' => $image,
          '#prefix' => '<div>',
          '#suffix' => '</div>',
        );
      }
      else {
        // Account is linked to other Draugiem.lv profile than active session.
        global $user;
        $form['info']['already_linked'] = array(
          '#markup' => t('This account is already linked with another Draugiem.lv profile'),
          '#prefix' => '<div>',
          '#suffix' => '</div>',
        );
        $form['info']['profile_link'] = array(
          '#type' => 'link',
          '#title' => t('View profile'),
          '#href' => 'http://www.draugiem.lv/user/' . _get_user_druid($user->uid),
          '#attributes' => array(
            'target' => '_blank',
          ),
        );
      }
    }
    else {
      // No active Draugiem.lv session.
      $no_session = t('No active Draugiem.lv session.');
      $form['info']['no_session'] = array(
        '#markup' => $no_session,
        '#prefix' => '<div>',
        '#suffix' => '</div>',
      );
      $profile = 'http://www.draugiem.lv/user/' . $variables['druid'];
      $form['info']['profile'] = array(
        '#type' => 'link',
        '#title' => t('View'),
        '#href' => $profile,
        '#prefix' => t('Profile this account is linked with:') . ' ',
        '#attributes' => array(
          'target' => '_blank',
        ),
      );
    }
    $form['unlink'] = array(
      '#type' => 'submit',
      '#value' => t('Unlink'),
      '#prefix' => '<div>',
      '#suffix' => '</div>',
    );
  }
  elseif ($action == 'login') {
    // Account not linked to a Draugiem.lv profile and no active Draugiem.lv
    // session is detected.
    $form['info']['login'] = array(
      '#markup' => t('Login to a Draugiem.lv profile'),
      '#prefix' => '<div>',
      '#suffix' => '</div>',
    );
    global $base_url;
    $redirect = $base_url . '/drconnect/authorized';
    if (variable_get('drconnect_login_button_style', 'default') == 'default') {
      $login_button = drconnect_draugiem_client(drconnect_api_filepath())->getLoginButton($redirect);
    }
    else {
      $login_button = drconnect_login_button($redirect);
    }
    $form['info']['login_button'] = array(
      '#markup' => $login_button,
    );
  }
  elseif ($action == 'link') {
    drconnect_draugiem_client(drconnect_api_filepath())->getSession();
    $druser = drconnect_draugiem_client(drconnect_api_filepath())->getUserData();
    $name = $druser['name'] . ' ' . $druser['surname'];
    $image = theme_image(array('path' => $druser['img'], 'alt' => $name, 'title' => $name, 'attributes' => array()));
    $form['info']['profile'] = array(
      '#markup' => t('Active Draugiem.lv session:'),
      '#prefix' => '<div>',
      '#suffix' => '</div>',
    );
    $form['info']['profile_name'] = array(
      '#markup' => $name,
      '#prefix' => '<div>',
      '#suffix' => '</div>',
    );
    $form['info']['profile_image'] = array(
      '#markup' => $image,
      '#prefix' => '<div>',
      '#suffix' => '</div>',
    );
    $form['link'] = array(
      '#type' => 'submit',
      '#value' => t('Link'),
      '#prefix' => '<div>',
      '#suffix' => '</div>',
    );
  }
  return $form;
}

function drconnect_account_link_form_submit($form, &$form_state) {
  global $user;
  if ($form_state['triggering_element']['#value'] == t('Unlink')) {
    // Remove the link between site account and Draugiem.lv profile.
    $query = db_delete('drconnect_users');
    $query->condition('uid', $user->uid);
    $query->execute();
    drupal_set_message(t('Link between site account and Draugiem.lv profile removed'));
  }
  elseif ($form_state['triggering_element']['#value'] == t('Link')) {
    // Create the link between site account and Draugiem.lv profile.
    drconnect_draugiem_client(drconnect_api_filepath())->getSession();
    $druid = drconnect_get_druid();
    $query = db_insert('drconnect_users');
    $query->fields(array(
      'uid' => $user->uid,
      'druid' => $druid,
      'timestamp' => REQUEST_TIME,
    ));
    $query->execute();
    drupal_set_message(t('Link between site account and Draugiem.lv profile created'));
  }
  $form_state['redirect'] = 'user/' . $user->uid . '/drconnect';
}
