<?php

/**
 * @filesource
 *   Defines admin settings for facebook you share.
 */

/**
 * Admin settings Form
 * @param $settings as 'general'
 *
 */
function facebook_admin($settings = 'general') {
  $types = array();
  $types = node_type_get_names();
  // Getting the active/user selected content types from the General Settings
  $active_types = variable_get('fb_content_types', array());
  $active_types = array_filter($active_types);
  
  if (is_array($types)) {
    // If there are active content types create and render the general and advanced settings accordingly.
    if (count($types) > 0) {
      switch ($settings) {
        case 'general':
          // User is on general settings page.
          $output = drupal_get_form('facebook_you_share_settings_general');
          break;
        case 'advanced':
          // User is on advanced settings page.
          if (empty($active_types)) {
            $output = t('There are no content types selected in the general settings.' . ' ' .
                    'Go back to General Settings, select some content types and Save Configuration.');
          }
          // If the user does not select any content type for sharing, display the below message.
          else {
            $output = drupal_get_form('facebook_you_share_settings_advanced');
          }
          break;
      }
    }
  }

  // If there are no content types, display a message
  else {
    $output = t('Currently, there are no content types. Click ' .
        '<a href="@add_types">here</a> to add some content types. ',
        array(
            '@add_types' => url('admin/structure/types/add'),
        ));
    }

  return $output;
}

/**
 * Function to return the form for general settings after validating the
 * presence content types
 * @param Array $form
 *   Form displayed to user/admin
 *
 * @param Array $form_state
 *   submitted values in form.
 *
 */
function facebook_you_share_settings_general($form, &$form_state) {
  $form['facebook_app'] = array(
      '#type' => 'fieldset',
      '#title' => t('Facebook Application settings'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
  );
  $form['facebook_app']['fb_app_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Facebook App ID'),
      '#default_value' => variable_get('fb_app_id', ''),
      '#description' => t('Facebook application ID that you will find on your Facebook Application\'s page'),
      '#required' => TRUE,
  );
  $form['facebook_app']['fb_redirect_url'] = array(
      '#type' => 'textfield',
      '#title' => t('Facebook Redirect URL'),
      '#default_value' => variable_get('fb_redirect_url', ''),
      '#description' => t('Facebook Redirect URL, URL to which Facebook will return after you share the link'),
      '#required' => TRUE,
  );

  $form['content_types'] = array(
      '#type' => 'fieldset',
      '#title' => t('Content Types'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
  );
  $form['content_types']['fb_content_types'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Content types'),
      '#description' => t('Select the content types on which you want to enable Facebook You Share.'),
      '#options' => node_type_get_names(),
      '#required' => TRUE,
      '#default_value' => variable_get('fb_content_types', array()),
  );

  $form['facebookshare_location'] = array(
      '#type' => 'fieldset',
      '#title' => t('Location'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
  );
  $form['facebookshare_location']['share_location'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Location'),
      '#description' => t('Where to show the Facebook You Share button.'),
      '#options' => array(
          'content' => t('Full View'),
          'teasers' => t('Teaser View'),
      ),
      '#default_value' => variable_get('share_location', array()),
  );

  /**
   * Userpoints settings START HERE.
   */

   $form['fb_userpoints']['userpoints_integration'] = array(
       '#type' => 'fieldset',
       '#title' => module_exists('userpoints') ? t('!Points for Sharing', userpoints_translation()) : t('Userpoints Setting'),
       '#access' => user_access('administer userpoints'),
       '#collapsible' => TRUE,
       '#collapsed' => FALSE,
   );

  // Add description
  if (!module_exists('userpoints')) {
  $desc = t('Enable <a href="@link">Userpoints</a> integrations with ' .
      'Facebook You Share module',
      array('@link' => 'http://drupal.org/project/userpoints'));
  }
  else {
  $desc = t('Enable <a href="@link">Userpoints</a> integrations with ' .
      'Facebook You Share module.<br />',
      array('@link' => 'http://drupal.org/project/userpoints'));
  $desc .= t('If checked, !points can be awarded for sharing content on Facebook.',
      userpoints_translation());
  }

  $form['fb_userpoints']['userpoints_integration']['fb_share_userpoints_check'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable'),
      '#description' => $desc,
      '#default_value' => variable_get('fb_share_userpoints_check', FALSE),
      '#disabled' => !module_exists('userpoints'),
  );

  $form['fb_userpoints']['userpoints_integration']['fb_share_userpoints_points'] = array(
      '#type'          => 'textfield',
      '#title'         => module_exists('userpoints') ? t('Default !Points for sharing', userpoints_translation()) : t('Default Points for Sharing'),
      '#default_value' => variable_get('fb_share_userpoints_points', 10),
      '#description'   => module_exists('userpoints') ? t('Set the default number of !points to be awarded when a user share content.', userpoints_translation()) : t('Set the default number of points to be awarded when a user share content.'),
      '#size'          => 30,
      // Conditionally hide radio buttons.
      '#states' => array(
      'visible' => array(
      ':input[name="fb_share_userpoints_check"]' => array(
      'checked' => TRUE
        ),
      ),
    )
  );

  /**
  * Userpoints settings END HERE.
  */

 return system_settings_form($form);

}

/**
 * Function to return the form for advanced settings after validating
 * the presence of active content types.
 *
 * @param Array $form
 *   Form displayed to user/admin
 *
 * @param Array $form_state
 *   submitted values in form.
 */
function facebook_you_share_settings_advanced($form, &$form_state) {
  $active_types = array();
  // Get the active (selected) content types
  $active_types = variable_get('fb_content_types', array());
  $active_types = array_filter($active_types);
  foreach ($active_types as $active_content_type => $value) {
    if (!empty($value)) {
        $form[$active_content_type] = array(
            '#type' => 'fieldset',
            '#title' => t('Facebook Post settings for @content_type_name',
                         array('@content_type_name' => $active_content_type)),
            '#collapsible' => TRUE,
            '#collapsed' => TRUE,
        );

        if (module_exists('token')) {
          $form[$active_content_type]['view']['token_help'] = array(
              '#title' => t('Replacement patterns'),
              '#type' => 'fieldset',
              '#collapsible' => TRUE,
              '#collapsed' => TRUE,
          );

          $form[$active_content_type]['view']['token_help']['help'] = array(
              '#theme' => 'token_tree',
              '#token_types' => array('node', 'site', 'user'),
              '#global_types' => TRUE,
              '#click_insert' => TRUE,
          );
        }

        $form[$active_content_type][$active_content_type . '_post_title'] = array(
            '#type' => 'textfield',
            '#title' => t('Title of the post'),
            '#default_value' => variable_get($active_content_type . '_post_title', '[node:title]'),
            '#description' => t('Title of the post which will also be a link, default is the node title. e.g. [node:title]'),
        );
        $form[$active_content_type][$active_content_type . '_post_link'] = array(
            '#type' => 'textfield',
            '#title' => t('Link of the title'),
            '#element_validate' => array('post_link_validate'),
            '#default_value' => variable_get($active_content_type . '_post_link', '[node:url]'),
            '#description' => t('Link to which the title will take user\'s from Facebook, ' .
                'default it will link to the respective node' .
                '<br />Allowed url tokens [node:url], [site:url]'
            ),
            '#required' => TRUE,
        );

        $form[$active_content_type][$active_content_type . '_post_caption'] = array(
            '#type' => 'textfield',
            '#title' => t('Caption'),
            '#default_value' => variable_get($active_content_type . '_post_caption', '[site:url]'),
            '#description' => t('Text just below the Title, default is the site url. e.g. [site:url]'),
        );
        $form[$active_content_type][$active_content_type . '_post_description'] = array(
            '#type' => 'textfield',
            '#title' => t('Description'),
            '#default_value' => variable_get($active_content_type . '_post_description', '[node:body]'),
            '#description' => t('The body of the facebook post, default is the node body. e.g. [node:body]'),
        );
        $form[$active_content_type][$active_content_type . '_post_display'] = array(
            '#type' => 'textfield',
            '#title' => t('Display mode'),
            '#element_validate' => array('post_display_validate'),
            '#default_value' => variable_get($active_content_type . '_post_display', 'page'),
            '#description' => t('The display mode of the facebook dialog. ' .
                'Display options available are page and popup. ' .
                'Allowed values are page, popup or iframe. e.g. page'
            ),
            '#required' => TRUE,
        );

        // @TODO make the image path relative
        $form[$active_content_type][$active_content_type . '_post_image'] = array(
            '#type' => 'textfield',
            '#title' => t('Image in the Post.'),
            '#default_value' => variable_get($active_content_type . '_post_image', theme_get_setting('logo')),
            '#description' => t('The image to appear in the user\'s post, provide the ' .
                'Absolute URL of the image, default it will take the ' .
                'images available from the site.<br />' .
                'Tokens can be used but provide only URL tokens ' .
                'e.g. The web-accessible URL for the user picture will be [user:picture:url]'),
        );
      }
    }
   return system_settings_form($form);
  }

/**
 * Validation function for post link
 */
function post_link_validate($element, &$form_state) {
  if (($element['#value'] != '[node:url]' && $element['#value'] != '[site:url]') &&
    !preg_match("/(http|https|ftp|ftps)\:\/\/[a-zA-Z0-9\-\.]+\.[a-zA-Z]{2,3}(\/\S*)?/", $element['#value'])) {

    form_set_error($element['#name'], 'The link format is not correct');
  }
}

/**
 * Validation of display mode
 */
function post_display_validate($element, &$form_state) {
  if ($element['#value'] != 'page' && $element['#value'] != 'popup') {
    form_set_error($element['#name'], t('The display mode should be dialog or popup'));
  }
}
