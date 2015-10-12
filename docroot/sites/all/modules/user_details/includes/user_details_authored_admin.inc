<?php
/**
 * @file
 * Authored admin form page
 */
function user_details_authored_admin_form() {
  global $base_url; // Used to pull the domain name (or base url) for the drupal system.

  drupal_theme_rebuild(); // Used to rebuild the theme registry (to grab the new template file)

  // Add the css that is needed for the admin form
  drupal_add_css(drupal_get_path('module', 'user_details') . '/css/user_details_authored_admin.css');

  $form['authored'] = array(
    '#type' => 'vertical_tabs',
  );

// Template Style
  $form['authored']['authored_template_style'] = array(
    '#type' => 'fieldset',
    '#title' => 'Template Style',
  );
  $form['authored']['authored_template_style']['user_details_authored_template_style'] = array(
    '#type' => 'select',
    '#title' => t('Template Style'),
    '#options' => array(
      1 => t('Icons Only'),
      2 => t('Text and Icons'),
    ),
    '#default_value' => variable_get('user_details_authored_template_style', '1'),
    '#description' => t('Choose the template style you would like to use with your authored user block.'),
  );

// Stats
  $form['authored']['authored_stats'] = array(
    '#type' => 'fieldset',
    '#title' => 'User Stats',
  );
  $form['authored']['authored_stats']['user_details_authored_picture_view'] = array(
    '#type' => 'checkbox',
    '#title' => t('User Picture:'),
    '#default_value' => variable_get('user_details_authored_picture_view', FALSE),
    '#description' => t('displays the picture of the logged-in user, if any is available.')
  );
  $form['authored']['authored_stats']['user_details_authored_created_view'] = array(
    '#type' => 'checkbox',
    '#title' => t('Created:'),
    '#default_value' => variable_get('user_details_authored_created_view', FALSE),
    '#description' => t('displays the account creation date of the logged-in user.')
  );
  $form['authored']['authored_stats']['user_details_authored_posts_view'] = array(
    '#type' => 'checkbox',
    '#title' => t('Posts:'),
    '#default_value' => variable_get('user_details_authored_posts_view', FALSE),
    '#description' => t('displays the number of posts the logged-in user has.'),
  );
  if (module_exists('comment')) {
    $form['authored']['authored_stats']['user_details_authored_comments_view'] = array(
      '#type' => 'checkbox',
      '#title' => t('Comments:'),
      '#default_value' => variable_get('user_details_authored_comments_view', FALSE),
      '#description' => t('displays the number of comments the logged-in user has.'),
    );
  }
  else {
    $form['authored']['authored_stats']['user_details_authored_comments_view'] = array(
      '#type' => 'checkbox',
      '#title' => t('Comments:'),
      '#default_value' => variable_get('user_details_authored_comments_view', FALSE),
      '#description' => t('enable Comment to enable this option.'),
    );
  }
  if (module_exists('comment')) {
    $form['authored']['authored_stats']['user_details_authored_combined_view'] = array(
      '#type' => 'checkbox',
      '#title' => t('Combined:'),
      '#default_value' => variable_get('user_details_authored_combined_view', FALSE),
      '#description' => t('combines the number of posts and comments of the logged-in user.'),
    );
  }
  else {
    $form['authored']['authored_stats']['user_details_authored_combined_view'] = array(
      '#type' => 'checkbox',
      '#title' => t('Combined:'),
      '#default_value' => variable_get('user_details_authored_combined_view', FALSE),
      '#description' => t('enable Comment to enable this option.'),
    );
  }
  if (module_exists('userpoints')) {
    $form['authored']['authored_stats']['user_details_authored_points_view'] = array(
      '#type' => 'checkbox',
      '#title' => t('User Points:'),
      '#default_value' => variable_get('user_details_authored_points_view', FALSE),
      '#description' => check_plain('displays the amount of User Points the logged-in user has. this will also use the branding from the <a href="' . $base_url . '/admin/config/people/userpoints/settings">User Points configuration</a> page.'),
      '#disabled' => FALSE,
    );
  }
  else {
    $form['authored']['authored_stats']['user_details_authored_points_view'] = array(
      '#type' => 'checkbox',
      '#title' => t('User Points:'),
      '#default_value' => variable_get('user_details_authored_points_view', FALSE),
      '#description' => t('install <a href="http://drupal.org/project/userpoints">User Points</a> to use this option.'),
      '#disabled' => TRUE,
    );
  }
  $form['authored']['authored_stats']['user_details_authored_role_view'] = array(
    '#type' => 'checkbox',
    '#title' => t('Role:'),
    '#default_value' => variable_get('user_details_authored_role_view', FALSE),
    '#description' => t('displays the logged-in user\'s role name.'),
  );

//Built-in Links
  $form['authored']['authored_builtin_links'] = array(
    '#type' => 'fieldset',
    '#title' => 'Built-in Links',
  );
  $form['authored']['authored_builtin_links']['user_details_authored_profile_view'] = array(
    '#type' => 'checkbox',
    '#title' => t('Profile:'),
    '#default_value' => variable_get('user_details_authored_profile_view', FALSE),
    '#description' => t('displays a link leading to the logged-in user\'s profile page.'),
  );
  $form['authored']['authored_builtin_links']['user_details_authored_profile_imgsrc'] = array(
    '#type' => 'textfield',
    '#title' => t('Image Source:'),
    '#default_value' => variable_get('user_details_authored_profile_imgsrc', "/" . drupal_get_path('module', 'user_details') . "/images/profile-icon.png"),
    '#description' => t('<b>Default Value:</b> <i>/' . drupal_get_path('module', 'user_details') . '/images/profile-icon.png</i>'),
    '#size' => 75,
    '#maxlength' => 500,
  );
  if (module_exists('privatemsg')) {
    $form['authored']['authored_builtin_links']['user_details_authored_privatemsg_view'] = array(
      '#type' => 'checkbox',
      '#title' => t('Private Messages:'),
      '#default_value' => variable_get('user_details_authored_privatemsg_view', FALSE),
      '#description' => t('displays a link leading to the logged-in user\'s private message page.'),
      '#disabled' => FALSE,
    );
    $form['authored']['authored_builtin_links']['user_details_authored_privatemsg_imgsrc'] = array(
      '#type' => 'textfield',
      '#title' => t('Image Source:'),
      '#default_value' => variable_get('user_details_authored_privatemsg_imgsrc', "/" . drupal_get_path('module', 'user_details') . "/images/privatemsg-icon.png"),
      '#description' => t('<b>Default Value:</b> <i>/' . drupal_get_path('module', 'user_details') . '/images/privatemsg-icon.png</i>'),
      '#size' => 75,
      '#maxlength' => 500,
    );
  }
  else {
    $form['authored']['authored_builtin_links']['user_details_authored_privatemsg_view'] = array(
      '#type' => 'checkbox',
      '#title' => t('Private Messages:'),
      '#default_value' => variable_get('user_details_authored_privatemsg_view', FALSE),
      '#description' => t('install <a href="http://drupal.org/project/privatemsg">Private Message</a> to enable this option.'),
      '#disabled' => TRUE,
    );
  }
  if (module_exists('blog')) {
    $form['authored']['authored_builtin_links']['user_details_authored_blog_view'] = array(
      '#type' => 'checkbox',
      '#title' => t('Blog:'),
      '#default_value' => variable_get('user_details_authored_blog_view', FALSE),
      '#description' => t('displays a link leading to the logged-in user\'s blog page.'),
      '#disabled' => FALSE,
    );
    $form['authored']['authored_builtin_links']['user_details_authored_blog_imgsrc'] = array(
      '#type' => 'textfield',
      '#title' => t('Image Source:'),
      '#default_value' => variable_get('user_details_authored_blog_imgsrc', "/" . drupal_get_path('module', 'user_details') . "/images/blog-icon.png"),
      '#description' => t('<b>Default Value:</b> <i>/' . drupal_get_path('module', 'user_details') . '/images/blog-icon.png</i>'),
      '#size' => 75,
      '#maxlength' => 500,
    );
  }
  else {
    $form['authored']['authored_builtin_links']['user_details_authored_blog_view'] = array(
      '#type' => 'checkbox',
      '#title' => t('Blog:'),
      '#default_value' => variable_get('user_details_authored_blog_view', FALSE),
      '#description' => t('enable Blog to enable this option.'),
      '#disabled' => TRUE,
    );
  }
  $form['authored']['authored_builtin_links']['user_details_authored_logout_view'] = array(
    '#type' => 'checkbox',
    '#title' => t('Logout:'),
    '#default_value' => variable_get('user_details_authored_logout_view', FALSE),
    '#description' => t('displays a link to allow the logged-in user to logout.')
  );
  $form['authored']['authored_builtin_links']['user_details_authored_logout_imgsrc'] = array(
    '#type' => 'textfield',
    '#title' => t('Image Source:'),
    '#default_value' => variable_get('user_details_authored_logout_imgsrc', "/" . drupal_get_path('module', 'user_details') . "/images/logout-icon.png"),
    '#description' => t('<b>Default Value:</b> <i>/' . drupal_get_path('module', 'user_details') . '/images/logout-icon.png</i>'),
    '#size' => 75,
    '#maxlength' => 500,
  );

//Custom Links
  $form['authored']['authored_custom_links'] = array(
    '#type' => 'fieldset',
    '#title' => 'Custom Links',
  );
  $form['authored']['authored_custom_links']['link_one_header'] = array(
    '#prefix' => '<div class="header"><h3>',
    '#markup' => t('Link One'),
    '#suffix' => '</h3></div>',
  );
  $form['authored']['authored_custom_links']['user_details_authored_link_one_type'] = array(
    '#type' => 'select',
    '#title' => t('Link Type:'),
    '#options' => array(
      0 => t('Disabled'),
      1 => t('Admin Link'),
      2 => t('User Link'),
    ),
    '#default_value' => variable_get('user_details_authored_link_one_type', '0'),
  );
  $form['authored']['authored_custom_links']['user_details_authored_link_one_url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL Path:'),
    '#default_value' => variable_get('user_details_authored_link_one_url', ''),
    '#size' => 75,
    '#maxlength' => 500,
  );
  $form['authored']['authored_custom_links']['user_details_authored_link_one_imgsrc'] = array(
    '#type' => 'textfield',
    '#title' => t('Image path:'),
    '#default_value' => variable_get('user_details_authored_link_one_imgsrc', ''),
    '#size' => 75,
    '#maxlength' => 500,
  );
  $form['authored']['authored_custom_links']['user_details_authored_link_one_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Image title:'),
    '#default_value' => variable_get('user_details_authored_link_one_title', ''),
    '#size' => 75,
    '#maxlength' => 500,
  );
  $form['authored']['authored_custom_links']['link_two_header'] = array(
    '#prefix' => '<div class="header"><h3>',
    '#markup' => t('Link Two'),
    '#suffix' => '</h3></div>',
  );
  $form['authored']['authored_custom_links']['user_details_authored_link_two_type'] = array(
    '#type' => 'select',
    '#title' => t('Link Type:'),
    '#options' => array(
      0 => t('Disabled'),
      1 => t('Admin Link'),
      2 => t('User Link'),
    ),
    '#default_value' => variable_get('user_details_authored_link_two_type', '0'),
  );
  $form['authored']['authored_custom_links']['user_details_authored_link_two_url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL path:'),
    '#default_value' => variable_get('user_details_authored_link_two_url', ''),
    '#size' => 75,
    '#maxlength' => 500,
  );
  $form['authored']['authored_custom_links']['user_details_authored_link_two_imgsrc'] = array(
    '#type' => 'textfield',
    '#title' => t('Image path:'),
    '#default_value' => variable_get('user_details_authored_link_two_imgsrc', ''),
    '#size' => 75,
    '#maxlength' => 500,
  );
  $form['authored']['authored_custom_links']['user_details_authored_link_two_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Image title:'),
    '#default_value' => variable_get('user_details_authored_link_two_title', ''),
    '#size' => 75,
    '#maxlength' => 500,
  );
  $form['authored']['authored_custom_links']['link_three_header'] = array(
    '#prefix' => '<div class="header"><h3>',
    '#markup' => t('Link Three'),
    '#suffix' => '</h3></div>',
  );
  $form['authored']['authored_custom_links']['user_details_authored_link_three_type'] = array(
    '#type' => 'select',
    '#title' => t('Link Type:'),
    '#options' => array(
      0 => t('Disabled'),
      1 => t('Admin Link'),
      2 => t('User Link'),
    ),
    '#default_value' => variable_get('user_details_authored_link_three_type', '0'),
  );
  $form['authored']['authored_custom_links']['user_details_authored_link_three_url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL Path:'),
    '#default_value' => variable_get('user_details_authored_link_three_url', ''),
    '#size' => 75,
    '#maxlength' => 500,
  );
  $form['authored']['authored_custom_links']['user_details_authored_link_three_imgsrc'] = array(
    '#type' => 'textfield',
    '#title' => t('Image Path:'),
    '#default_value' => variable_get('user_details_authored_link_three_imgsrc', ''),
    '#size' => 75,
    '#maxlength' => 500,
  );
  $form['authored']['authored_custom_links']['user_details_authored_link_three_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Image Title:'),
    '#default_value' => variable_get('user_details_authored_link_three_title', ''),
    '#size' => 75,
    '#maxlength' => 500,
  );
  $form['authored']['authored_custom_links']['link_four_header'] = array(
    '#prefix' => '<div class="header"><h3>',
    '#markup' => t('Link Four'),
    '#suffix' => '</h3></div>',
  );
  $form['authored']['authored_custom_links']['user_details_authored_link_four_type'] = array(
    '#type' => 'select',
    '#title' => t('Link Type:'),
    '#options' => array(
      0 => t('Disabled'),
      1 => t('Admin Link'),
      2 => t('User Link'),
    ),
    '#default_value' => variable_get('user_details_authored_link_four_type', '0'),
  );
  $form['authored']['authored_custom_links']['user_details_authored_link_four_url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL path:'),
    '#default_value' => variable_get('user_details_authored_link_four_url', ''),
    '#size' => 75,
    '#maxlength' => 500,
  );
  $form['authored']['authored_custom_links']['user_details_authored_link_four_imgsrc'] = array(
    '#type' => 'textfield',
    '#title' => t('Image path:'),
    '#default_value' => variable_get('user_details_authored_link_four_imgsrc', ''),
    '#size' => 75,
    '#maxlength' => 500,
  );
  $form['authored']['authored_custom_links']['user_details_authored_link_four_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Image title:'),
    '#default_value' => variable_get('user_details_authored_link_four_title', ''),
    '#size' => 75,
    '#maxlength' => 500,
  );
  $form['authored']['authored_custom_links']['link_five_header'] = array(
    '#prefix' => '<div class="header"><h3>',
    '#markup' => t('Link Five'),
    '#suffix' => '</h3></div>',
  );
  $form['authored']['authored_custom_links']['user_details_authored_link_five_type'] = array(
    '#type' => 'select',
    '#title' => t('Link Type'),
    '#options' => array(
      0 => t('Disabled'),
      3 => t('Admin Link'),
      2 => t('User Link'),
    ),
    '#default_value' => variable_get('user_details_authored_link_five_type', '0'),
  );
  $form['authored']['authored_custom_links']['user_details_authored_link_five_url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL path:'),
    '#default_value' => variable_get('user_details_authored_link_five_url', ''),
    '#size' => 75,
    '#maxlength' => 500,
  );
  $form['authored']['authored_custom_links']['user_details_authored_link_five_imgsrc'] = array(
    '#type' => 'textfield',
    '#title' => t('Image path:'),
    '#default_value' => variable_get('user_details_authored_link_five_imgsrc', ''),
    '#size' => 75,
    '#maxlength' => 500,
  );
  $form['authored']['authored_custom_links']['user_details_authored_link_five_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Image title:'),
    '#default_value' => variable_get('user_details_authored_link_five_title', ''),
    '#size' => 75,
    '#maxlength' => 500,
  );

//Content
  $form['authored']['authored_content'] = array(
    '#type' => 'fieldset',
    '#title' => 'User Content',
  );
  $content_amount = variable_get('user_details_authored_content_amount');
  $form['authored']['authored_content']['user_details_authored_content_view'] = array(
    '#type' => 'checkbox',
    '#title' => t('Content:'),
    '#default_value' => variable_get('user_details_authored_content_view', FALSE),
    '#description' => t('Lists the last %content_amount updated pages authored by the user.', array(
      '%content_amount' => $content_amount,
      )
    )
  );
  $form['authored']['authored_content']['user_details_authored_content_amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Amount:'),
    '#default_value' => variable_get('user_details_authored_content_amount', 3),
    '#size' => 2,
    '#maxlength' => 2,
    '#description' => t('Changes the number of recent items listed for the "User Content" list.')
  );
  $form['authored']['authored_content']['user_details_authored_content_truncate'] = array(
    '#type' => 'textfield',
    '#title' => t('Truncate Amount:'),
    '#default_value' => variable_get('user_details_authored_content_truncate', 20),
    '#size' => 2,
    '#maxlength' => 2,
    '#description' => t('Changes the length of the node title.')
  );

  return system_settings_form($form);
}