<?php

/**
 * @file
 * Admin callbacks and forms for the post module.
 */

/**
 * Form definition for admin/config/content/post.
 *
 * @see hook_menu()
 */
function post_settings() {
  $path = drupal_get_path('module', 'post');
  
  $form['#attached']['css'] = array(
    $path . '/post.admin.css',
  );

  $settings = variable_get('post_settings', array());
  $node_types = node_type_get_types();
  $types = array();

  // Put together an array of node types for use in #options.
  foreach ($node_types as $type) {
    $types[$type->type] = $type->name;
  }

  $form['publisher_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Publisher Key'),
    '#description' => t('To get a publisher key, go to the @post site and click on "Join Now". Once logged in, you will find your publisher key in the code section. Look for the following line: &lt;script src="http://i.po.st/static/script/post-widget.js#publisherKey=[publisher key]" type="text/javascript"&gt;&lt;/script&gt; The string located at [published key] in the above url is your Publisher Key.', array('@post', l(t('Po.st'), 'http://www.po.st'))),
    '#required' => TRUE,
    '#default_value' => isset($settings['publisher_key']) ? $settings['publisher_key'] : '',
  );

  // Vertical tabs for each widget Style.
  $form['style'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => '98',
    '#prefix' => '<p><strong>Button Styles</strong></p>',
  );

  // Single Po.st button.
  $form['post'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#title' => t('Po.st Share Button'),
    '#group' => 'style',
    'example' => array(
      '#type' => 'markup',
      '#markup' => theme('image', array(
        'path' => $path . '/images/post_share.png',
        'alt' => 'Po.st Share Button',
        'attributes' => array(
          'class' => 'post_example',
        ),
      )),
    ),
    'post_enabled' => array(
      '#type' => 'checkbox',
      '#title' => t('Enabled'),
      '#default_value' => isset($settings['post']['post_enabled']) ? $settings['post']['post_enabled'] : '0',
    ),
    'post_icon_size' => array(
      '#type' => 'select',
      '#title' => t('Icon Size'),
      '#description' => t('Pixel size of images for social sharing'),
      '#options' => array(
        '16' => '16x16',
        '24' => '24x24',
        '32' => '32x32',
      ),
      '#default_value' => isset($settings['post']['post_icon_size']) ? $settings['post']['post_icon_size'] : '24',
    ),
  );

  // Standard social sharing buttons.
  $form['buttons'] = array(
    '#type' => 'fieldset',
    '#title' => t('Social Share Buttons'),
    '#group' => 'style',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    'example' => array(
      '#type' => 'markup',
      '#markup' => theme('image', array(
        'path' => $path . '/images/social_buttons.png',
        'alt' => 'Social Share Buttons',
        'attributes' => array(
          'class' => 'post_example',
        ),
      )),
    ),
    'buttons_enabled' => array(
      '#type' => 'checkbox',
      '#title' => t('Enabled'),
      '#default_value' => isset($settings['buttons']['buttons_enabled']) ? $settings['buttons']['buttons_enabled'] : '0',
    ),
    'buttons_icon_size' => array(
      '#type' => 'select',
      '#title' => t('Icon Size'),
      '#description' => t('Pixel size of images for social sharing'),
      '#options' => array(
        '16' => '16x16',
        '24' => '24x24',
        '32' => '32x32',
      ),
      '#default_value' => isset($settings['buttons']['buttons_icon_size']) ? $settings['buttons']['buttons_icon_size'] : '24',
    ),
  );

  // Expanded buttons with counts.
  $form['expanded'] = array(
    '#type' => 'fieldset',
    '#title' => t('Expanded Buttons'),
    '#group' => 'style',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    'help' => array(
      '#type' => 'markup',
      '#markup' => t('Services available for this style: Google Plus, Facebook, Twitter, LinkedIn, Po.st'),
    ),
    'example' => array(
      '#type' => 'markup',
      '#markup' => theme('image', array(
        'path' => $path . '/images/expanded_buttons.png',
        'alt' => 'Expanded Buttons',
        'attributes' => array(
          'class' => 'post_example',
        ),
      )),
    ),
    'expanded_enabled' => array(
      '#type' => 'checkbox',
      '#title' => t('Enabled'),
      '#default_value' => isset($settings['expanded']['expanded_enabled']) ? $settings['expanded']['expanded_enabled'] : '0',
    ),
  );

  // Vertical buttons with counts and floating share bar.
  $form['vertical'] = array(
    '#type' => 'fieldset',
    '#title' => t('Vertical Buttons'),
    '#group' => 'style',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    'help' => array(
      '#type' => 'markup',
      '#markup' => t('Services available for this style: Google Plus, Facebook, Twitter, LinkedIn, Po.st'),
    ),
    'example' => array(
      '#type' => 'markup',
      '#markup' => theme('image', array(
        'path' => $path . '/images/vertical_buttons.png',
        'alt' => 'Vertical Buttons',
        'attributes' => array(
          'class' => 'post_example',
        ),
      )),
    ),
    'vertical_enabled' => array(
      '#type' => 'checkbox',
      '#title' => t('Enabled'),
      '#default_value' => isset($settings['vertical']['vertical_enabled']) ? $settings['vertical']['vertical_enabled'] : '0',
    ),
    'vertical_position' => array(
      '#type' => 'select',
      '#title' => t('Float Position'),
      '#options' => array(
        'left' => 'Left',
        'right' => 'Right',
      ),
      '#default_value' => isset($settings['vertical']['vertical_position']) ? $settings['vertical']['vertical_position'] : 'left',
      '#description' => t('Display the vertical floating bar on the left or right side of the content'),
    ),
    'vertical_color' => array(
      '#type' => 'jquery_colorpicker',
      '#title' => 'Float Bar Background Color',
      '#default_value' => isset($settings['vertical']['vertical_color']) ? $settings['vertical']['vertical_color'] : 'ffffff',
    ),
    'vertical_border' => array(
      '#type' => 'jquery_colorpicker',
      '#title' => 'Float Bar Border Color',
      '#default_value' => isset($settings['vertical']['vertical_border']) ? $settings['vertical']['vertical_border'] : 'cccccc',
    ),
  );

  // Vertical tabs for additional settings.
  $form['additional_settings'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => '99',
    '#prefix' => '<p><strong>Additional Configuration</strong></p>',
  );

  // Global settings that affect each widget style.
  $form['settings'] = array(
    '#type' => 'fieldset',
    '#group' => 'additional_settings',
    '#title' => t('Additional Settings'),
    '#collapsible' => TRUE,
    'copy_paste_tracking' => array(
      '#type' => 'checkbox',
      '#title' => t('Copy/Paste Tracking'),
      '#description' => t('Enable/Disable copy and paste share tracking functionality'),
      '#default_value' => isset($settings['copy_paste_tracking']) ? $settings['copy_paste_tracking'] : '1',
    ),
    'post_counter' => array(
      '#type' => 'checkbox',
      '#title' => t('Po.st Counter'),
      '#description' => t('Enable/Disable counter on Po.st button'),
      '#default_value' => isset($settings['post_counter']) ? $settings['post_counter'] : '1',
    ),
    'weight' => array(
      '#type' => 'textfield',
      '#title' => t('Weight'),
      '#default_value' => isset($settings['weight']) ? $settings['weight'] : '-1',
      '#description' => t('The weight determines where the buttons will display on the page.'),
      '#attributes' => array(
        'class' => array('post_weight'),
      ),
    ),
  );

  // Enabled social sharing services.
  $form['services'] = array(
    '#type' => 'fieldset',
    '#group' => 'additional_settings',
    '#title' => t('Enabled Services'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    'help' => array(
      '#type' => 'markup',
      '#markup' => t('Select from the services below to display in the Po.st social sharing widget.'),
    ),
    'enabled_services' => array(
      '#type' => 'checkboxes',
      '#options' => post_get_services_list(),
      '#default_value' => isset($settings['enabled_services']) ? $settings['enabled_services'] : array(
        'pw_googleplus',
        'pw_facebook',
        'pw_twitter',
        'pw_email',
      ),
    ),
  );

  // Content types tp display social sharing widget.
  $form['types'] = array(
    '#type' => 'fieldset',
    '#group' => 'additional_settings',
    '#title' => t('Content Types'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    'help' => array(
      '#type' => 'markup',
      '#markup' => t('Select which content types the Po.st social sharing widget will appear on.'),
    ),
    'content_types' => array(
      '#type' => 'checkboxes',
      '#options' => $types,
      '#default_value' => isset($settings['content_types']) ? $settings['content_types'] : array(),
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => '100',
  );

  $form['#validate'][] = 'post_settings_validate';
  $form['#submit'][] = 'post_settings_submit';

  return $form;
}

/**
 * Form validate handler for post_settings().
 *
 * @see post_settings()
 */
function post_settings_validate($form, &$form_state) {
  $enabled_count = 0;
  $v = $form_state['values'];

  if (!preg_match('/^[A-Za-z0-9]{5,40}$/', $v['publisher_key'])) {
    form_set_error('publisher_key', 'Invalid publisher key entered.');
  }

  foreach ($v as $k => $value) {
    if (strpos($k, '_enabled') !== FALSE && $value === 1) {
      $enabled_count++;
    }
  }

  if ($enabled_count > 1) {
    form_set_error('post_enabled', 'Only one button style can be enabled.');
  }
}

/**
 * Form submit handler for post_settings().
 *
 * @see post_settings()
 */
function post_settings_submit($form, &$form_state) {
  // Keep form state when form is rebuilt.
  $form_state['redirect'] = FALSE;
  $settings = array();

  $settings['publisher_key'] = $form_state['values']['publisher_key'];
  $settings['copy_paste_tracking'] = $form_state['values']['copy_paste_tracking'];
  $settings['post_counter'] = $form_state['values']['post_counter'];
  $settings['weight'] = $form_state['values']['weight'];

  $settings['enabled_services'] = $settings['content_types'] = array();

  foreach ($form_state['values']['enabled_services'] as $service => $value) {
    if ($value !== 0) {
      $settings['enabled_services'][] = check_plain($service);
    }
  }

  foreach ($form_state['values']['content_types'] as $type => $value) {
    if ($value !== 0) {
      $settings['content_types'][] = $type;
    }
  }

  // Assign settings for each widget style if enabled is selected.
  foreach ($form_state['values'] as $k => $v) {
    if (!empty($v) && $form_state['values']['post_enabled'] == 1 && strpos($k, 'post_') !== FALSE) {
      $settings['post'][$k] = $v;
    }
    elseif (!empty($v) && $form_state['values']['buttons_enabled'] == 1 && strpos($k, 'buttons_') !== FALSE) {
      $settings['buttons'][$k] = $v;
    }
    elseif (!empty($v) && $form_state['values']['expanded_enabled'] == 1 && strpos($k, 'expanded_') !== FALSE) {
      $settings['expanded'][$k] = $v;
    }
    elseif (!empty($v) && $form_state['values']['vertical_enabled'] == 1 && strpos($k, 'vertical_') !== FALSE) {
      $settings['vertical'][$k] = $v;
    }
  }

  variable_set('post_settings', $settings);

  drupal_set_message(t('Settings Saved'));
}

/**
 * Returns an array of available services.
 */
function post_get_services_list() {
  return array(
    'pw_googleplus' => 'Google+',
    'pw_facebook' => 'Facebook',
    'pw_twitter' => 'Twitter',
    'pw_email' => 'Email',
    'pw_print' => 'Print',
    'pw_stumble_upon' => 'Stumble Upon',
    'pw_linkedin' => 'LinkedIn',
    'pw_google_bookmarks' => 'Google Bookmarks',
    'pw_microsoft_messenger' => 'Microsoft Messenger',
    'pw_myspace' => 'MySpace',
    'pw_delicious' => 'Delicious',
    'pw_digg' => 'Digg',
    'pw_orkut' => 'Orkut',
    'pw_gmail' => 'Gmail',
    'pw_blogger' => 'Blogger',
    'pw_reddit' => 'Reddit',
    'pw_yahoo_mail' => 'Yahoo! Mail',
    'pw_tumblr' => 'Tumblr',
    'pw_hotmail' => 'Hotmail',
    'pw_aol_mail' => 'AOL Mail',
    'pw_live_journal' => 'LiveJournal',
    'pw_posterous' => 'Posterous',
    'pw_aol_lifestream' => 'AOL Lifestream',
    'pw_wordpress' => 'Wordpress',
    'pw_google_buzz' => 'Google Buzz',
    'pw_vkontakte' => 'VKontakte',
    'pw_baidu' => 'Baidu',
    'pw_mail.ru' => 'Mail.ru',
    'pw_hyves' => 'Hyves',
  );
}
