<?php

/**
 * @file
 * Admin forms.
 */

/**
 * FAPI system admin form.
 */
function fb_autopost_global_settings($form, &$form_state) {
  // Data for the Facebook App connection.
  $app_id = variable_get('fb_autopost_app_id', '');
  $secret = variable_get('fb_autopost_app_secret', '');
  $form['app_data'] = array(
    '#type' => 'fieldset',
    '#title' => t('Facebook App data'),
    '#collapsible' => TRUE,
    // Collapse it if all data is in place.
    '#collapsed' => !empty($app_id) && !empty($secret),
  );
  $form['app_data']['fb_autopost_app_id'] = array(
    '#type' => 'textfield',
    '#title' => t('APP ID'),
    '#description' => t('The API key of your Facebook App.'),
    '#default_value' => $app_id,
    '#required' => TRUE,
  );
  $form['app_data']['fb_autopost_app_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('APP Secret'),
    '#description' => t('The API secret of your Facebook App.'),
    '#default_value' => $secret,
    '#required' => TRUE,
  );

  // If we have all the info we need, show next part of the form.
  if (!empty($app_id) && !empty($secret)) {
    $form['page_info'] = array(
      '#type' => 'fieldset',
      '#title' => t('Facebook page information'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    try {
      // Create the FBAutpost object with the stored data.
      $fb = facebook_autopost();
      if ($fb_user = $fb->getUser()) {
        // If there is a connected account, get all FB pages related to that
        // account. Make these pages the values of a select.
        try {
          // Proceed knowing you have a logged in user who's authenticated.
          $form['page_info']['fb_user'] = array(
            '#theme' => 'fb_autopost_facebook_profile',
            '#fbprofile_id' => variable_get('fb_autopost_account_id', 'me'),
          );
          $pages_data = $fb->getPagesData(variable_get('fb_autopost_account_id', 'me'), variable_get('fb_autopost_token', ''));
          $form['page_info']['fb_autopost_page'] = array(
            '#type' => 'checkboxes',
            '#title' => t('Pages'),
            '#description' => t('Select the Facebook pages you want to make available to post to.'),
            '#options' => _fb_autopost_get_page_options($pages_data),
            '#default_value' => variable_get('fb_autopost_page', array()),
          );
          $form['page_info']['fb_autopost_pages_access_tokens'] = array(
            '#tree' => TRUE,
          );
          foreach ($pages_data['data'] as $page_data) {
            $form['page_info']['fb_autopost_pages_access_tokens'][$page_data['id']] = array(
              '#type' => 'hidden',
              '#value' => $page_data['access_token'],
            );
          }
        }
        catch (FacebookApiException $e) {
          watchdog('fb_autopost', $e->getMessage(), array(), WATCHDOG_ERROR);
          drupal_set_message($e->getMessage(), 'error');
          $user = NULL;
        }
      }
      else {
        // Invite the user to log in.
        $form['page_info']['login_link'] = array(
          '#prefix' => '<p>' . t('You need to connect your Facebook account to fetch the pages you are allowed to post to.') . '</p>',
          '#type' => 'link',
          '#title' => t('Login to Facebook'),
          '#href' => $fb->getLoginUrl(array(
            'scope' => fb_permissions_get_facebook_permissions(array('manage_pages', 'publish_actions')),
            'redirect_uri' => url('admin/config/services/fbautopost/login', array('absolute' => TRUE)),
          )),
        );
      }
    }
    catch (Exception $e) {
      // Do nothing with the exception.
      drupal_set_message($e->getMessage(), 'error');
    }
  }
  $form['#submit'][] = 'fb_autopost_global_settings_submit';
  return system_settings_form($form);
}

/**
 * Submit callback for fb_autopost_global_settings.
 */
function fb_autopost_global_settings_submit($form, &$form_state) {
  if (isset($form_state['values']['fb_autopost_page'])) {
    $pages = array_values(array_filter($form_state['values']['fb_autopost_page']));
    $at = $form_state['values']['fb_autopost_pages_access_tokens'];
    $form_state['values']['fb_autopost_pages_access_tokens'] = array();
    foreach ($pages as $page_id) {
      $form_state['values']['fb_autopost_pages_access_tokens'][$page_id] = $at[$page_id];
    }
  }
}

/**
 * Helper function to get all the available pages.
 *
 * @param array $pages_data
 *   An array containig the Facebook information about the page as returned
 *   from FBAutopost::getPagesData(…).
 *
 * @return array
 *   Array containing an options friendly array with all writable pages
 */
function _fb_autopost_get_page_options($pages_data) {
  $output = array();
  $data = array_filter($pages_data['data'], function ($item) {
    return $item['category'] != 'Application' && in_array('CREATE_CONTENT', $item['perms']);
  });
  foreach ($data as $value) {
    $output[$value['id']] = theme('fb_autopost_facebook_page', array(
      'name' => $value['name'],
      'id' => $value['id'],
      'category' => $value['category'],
    ));
  }
  return $output;
}
