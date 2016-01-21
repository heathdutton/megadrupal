<?php
/**
 * @file
 * Logged-in admin form page
 */
function user_details_loggedin_admin_form() {
  // Used to pull the domain name (or base url) for the drupal system.
  global $base_url;

  // Used to rebuild the them registry (to grab the new template file)
  drupal_theme_rebuild();

  // Add the css that is needed for the admin form
  drupal_add_css(drupal_get_path('module', 'user_details') . '/css/user_details_loggedin_admin.css');

  $form['loggedin'] = array(
    '#type' => 'vertical_tabs',
  );

// Template Style
  $form['loggedin']['loggedin_template_style'] = array(
    '#type' => 'fieldset',
    '#title' => 'Template Style',
  );
  $form['loggedin']['loggedin_template_style']['user_details_loggedin_template_style'] = array(
    '#type' => 'select',
    '#title' => t('Template Style'),
    '#options' => array(
      1 => t('Icons Only'),
      2 => t('Text and Icons'),
    ),
    '#default_value' => variable_get('user_details_loggedin_template_style', '1'),
    '#description' => t('Choose the template style you would like to use with your logged-in user block.'),
  );
// Stats
  $form['loggedin']['loggedin_stats'] = array(
    '#type' => 'fieldset',
    '#title' => 'User Stats',
  );
  $form['loggedin']['loggedin_stats']['user_details_loggedin_picture_view'] = array(
    '#type' => 'checkbox',
    '#title' => t('User Picture:'),
    '#default_value' => variable_get('user_details_loggedin_picture_view', FALSE),
    '#description' => t('displays the picture of the logged-in user, if any is available.')
  );
  $form['loggedin']['loggedin_stats']['user_details_loggedin_created_view'] = array(
    '#type' => 'checkbox',
    '#title' => t('Created:'),
    '#default_value' => variable_get('user_details_loggedin_created_view', FALSE),
    '#description' => t('displays the account creation date of the logged-in user.')
  );
  $form['loggedin']['loggedin_stats']['user_details_loggedin_posts_view'] = array(
    '#type' => 'checkbox',
    '#title' => t('Posts:'),
    '#default_value' => variable_get('user_details_loggedin_posts_view', FALSE),
    '#description' => t('displays the number of posts the logged-in user has.'),
  );
  if (module_exists('comment')) {
    $form['loggedin']['loggedin_stats']['user_details_loggedin_comments_view'] = array(
      '#type' => 'checkbox',
      '#title' => t('Comments:'),
      '#default_value' => variable_get('user_details_loggedin_comments_view', FALSE),
      '#description' => t('displays the number of comments the logged-in user has.'),
    );
  }
  else {
    $form['loggedin']['loggedin_stats']['user_details_loggedin_comments_view'] = array(
      '#type' => 'checkbox',
      '#title' => t('Comments:'),
      '#default_value' => variable_get('user_details_loggedin_comments_view', FALSE),
      '#description' => t('enable Comment to enable this option.'),
    );
  }
  if (module_exists('comment')) {
    $form['loggedin']['loggedin_stats']['user_details_loggedin_combined_view'] = array(
      '#type' => 'checkbox',
      '#title' => t('Combined:'),
      '#default_value' => variable_get('user_details_loggedin_combined_view', FALSE),
      '#description' => t('combines the number of posts and comments of the logged-in user.'),
    );
  }
  else {
    $form['loggedin']['loggedin_stats']['user_details_loggedin_combined_view'] = array(
      '#type' => 'checkbox',
      '#title' => t('Combined:'),
      '#default_value' => variable_get('user_details_loggedin_combined_view', FALSE),
      '#description' => t('enable Comment to enable this option.'),
    );
  }
  if (module_exists('userpoints')) {
    $form['loggedin']['loggedin_stats']['user_details_loggedin_points_view'] = array(
      '#type' => 'checkbox',
      '#title' => t('User Points:'),
      '#default_value' => variable_get('user_details_loggedin_points_view', FALSE),
      '#description' => check_plain('displays the amount of User Points the logged-in user has. this will also use the branding from the <a href="' . $base_url . '/admin/config/people/userpoints/settings">User Points configuration</a> page.'),
      '#disabled' => FALSE,
    );
  }
  else {
    $form['loggedin']['loggedin_stats']['user_details_loggedin_points_view'] = array(
      '#type' => 'checkbox',
      '#title' => t('User Points:'),
      '#default_value' => variable_get('user_details_loggedin_points_view', FALSE),
      '#description' => t('install <a href="http://drupal.org/project/userpoints">User Points</a> to use this option.'),
      '#disabled' => TRUE,
    );
  }
  $form['loggedin']['loggedin_stats']['user_details_loggedin_role_view'] = array(
    '#type' => 'checkbox',
    '#title' => t('Role:'),
    '#default_value' => variable_get('user_details_loggedin_role_view', FALSE),
    '#description' => t('displays the logged-in user\'s role name.'),
  );
  if (module_exists('privatemsg')) {
    $form['loggedin']['loggedin_stats']['user_details_loggedin_privatemsg_unread_view'] = array(
      '#type' => 'checkbox',
      '#title' => t('Unread Messages:'),
      '#default_value' => variable_get('user_details_loggedin_privatemsg_unread_view', FALSE),
      '#description' => t('displays the amount of unread messages the logged-in user has.'),
      '#disabled' => FALSE,
    );
  }
  else {
    $form['loggedin']['loggedin_stats']['user_details_loggedin_privatemsg_unread_view'] = array(
      '#type' => 'checkbox',
      '#title' => t('Unread Messages:'),
      '#default_value' => variable_get('user_details_loggedin_privatemsg_view', FALSE),
      '#description' => t('install <a href="http://drupal.org/project/privatemsg">Private Messages</a> to use this option.'),
      '#disabled' => TRUE,
    );
  }

//Built-in Links
  $form['loggedin']['loggedin_builtin_links'] = array(
    '#type' => 'fieldset',
    '#title' => 'Built-in Links',
  );
  $form['loggedin']['loggedin_builtin_links']['user_details_loggedin_front_view'] = array(
    '#type' => 'checkbox',
    '#title' => t('Front:'),
    '#default_value' => variable_get('user_details_loggedin_front_view', FALSE),
    '#description' => t('displays a link leading to "<i><b>%base_url</b></i>".', array(
      '%base_url' => $base_url,
      )
    ),
  );
  $form['loggedin']['loggedin_builtin_links']['user_details_loggedin_front_imgsrc'] = array(
    '#type' => 'textfield',
    '#title' => t('Image Source:'),
    '#default_value' => variable_get('user_details_loggedin_front_imgsrc', "/" . drupal_get_path('module', 'user_details') . "/images/home-icon.png"),
    '#description' => t('<b>Default Value:</b> <i>/' . drupal_get_path('module', 'user_details') . '/images/home-icon.png</i>'),
    '#size' => 75,
    '#maxlength' => 500,
  );
  $form['loggedin']['loggedin_builtin_links']['user_details_loggedin_profile_view'] = array(
    '#type' => 'checkbox',
    '#title' => t('Profile:'),
    '#default_value' => variable_get('user_details_loggedin_profile_view', FALSE),
    '#description' => t('displays a link leading to the logged-in user\'s profile page.'),
  );
  $form['loggedin']['loggedin_builtin_links']['user_details_loggedin_profile_imgsrc'] = array(
    '#type' => 'textfield',
    '#title' => t('Image Source:'),
    '#default_value' => variable_get('user_details_loggedin_profile_imgsrc', "/" . drupal_get_path('module', 'user_details') . "/images/profile-icon.png"),
    '#description' => t('<b>Default Value:</b> <i>/' . drupal_get_path('module', 'user_details') . '/images/profile-icon.png</i>'),
    '#size' => 75,
    '#maxlength' => 500,
  );
  $form['loggedin']['loggedin_builtin_links']['user_details_loggedin_profile_edit_view'] = array(
      '#type' => 'checkbox',
      '#title' => t('Edit Profile:'),
      '#default_value' => variable_get('user_details_loggedin_profile_edit_view', FALSE),
      '#description' => t('displays a link leading to the logged-in user\'s edit profile page.')
  );
  $form['loggedin']['loggedin_builtin_links']['user_details_loggedin_profile_edit_imgsrc'] = array(
    '#type' => 'textfield',
    '#title' => t('Image Source:'),
    '#default_value' => variable_get('user_details_loggedin_profile_edit_imgsrc', "/" . drupal_get_path('module', 'user_details') . "/images/profileedit-icon.png"),
    '#description' => t('<b>Default Value:</b> <i>/' . drupal_get_path('module', 'user_details') . '/images/profile-edit-icon.png</i>'),
    '#size' => 75,
    '#maxlength' => 500,
  );
  if (module_exists('privatemsg')) {
    $form['loggedin']['loggedin_builtin_links']['user_details_loggedin_private_message_view'] = array(
      '#type' => 'checkbox',
      '#title' => t('Private Messages:'),
      '#default_value' => variable_get('user_details_loggedin_private_message_view', FALSE),
      '#description' => t('displays a link leading to the logged-in user\'s private message page.'),
      '#disabled' => FALSE,
    );
    $form['loggedin']['loggedin_builtin_links']['user_details_loggedin_private_message_imgsrc'] = array(
      '#type' => 'textfield',
      '#title' => t('Image Source:'),
      '#default_value' => variable_get('user_details_loggedin_private_message_imgsrc', "/" . drupal_get_path('module', 'user_details') . "/images/private-message-icon.png"),
      '#description' => t('<b>Default Value:</b> <i>/' . drupal_get_path('module', 'user_details') . '/images/private-message-icon.png</i>'),
      '#size' => 75,
      '#maxlength' => 500,
    );
  }
  else {
    $form['loggedin']['loggedin_builtin_links']['user_details_loggedin_privatemsg_view'] = array(
      '#type' => 'checkbox',
      '#title' => t('Private Messages:'),
      '#default_value' => variable_get('user_details_loggedin_privatemsg_view', FALSE),
      '#description' => t('install <a href="http://drupal.org/project/privatemsg">Private Message</a> to enable this option.'),
      '#disabled' => TRUE,
    );
  }
  if (module_exists('blog')) {
    $form['loggedin']['loggedin_builtin_links']['user_details_loggedin_blog_view'] = array(
      '#type' => 'checkbox',
      '#title' => t('Blog:'),
      '#default_value' => variable_get('user_details_loggedin_blog_view', FALSE),
      '#description' => t('displays a link leading to the logged-in user\'s blog page.'),
      '#disabled' => FALSE,
    );
    $form['loggedin']['loggedin_builtin_links']['user_details_loggedin_blog_imgsrc'] = array(
      '#type' => 'textfield',
      '#title' => t('Image Source:'),
      '#default_value' => variable_get('user_details_loggedin_blog_imgsrc', "/" . drupal_get_path('module', 'user_details') . "/images/blog-icon.png"),
      '#description' => t('<b>Default Value:</b> <i>/' . drupal_get_path('module', 'user_details') . '/images/blog-icon.png</i>'),
      '#size' => 75,
      '#maxlength' => 500,
    );
  }
  else {
    $form['loggedin']['loggedin_builtin_links']['user_details_loggedin_blog_view'] = array(
      '#type' => 'checkbox',
      '#title' => t('Blog:'),
      '#default_value' => variable_get('user_details_loggedin_blog_view', FALSE),
      '#description' => t('enable Blog to enable this option.'),
      '#disabled' => TRUE,
    );
  }
  $form['loggedin']['loggedin_builtin_links']['user_details_loggedin_create_view'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create Content:'),
    '#default_value' => variable_get('user_details_loggedin_create_view', FALSE),
    '#description' => t('displays a link to the "/node/add" page'),
  );
  $form['loggedin']['loggedin_builtin_links']['user_details_loggedin_create_imgsrc'] = array(
    '#type' => 'textfield',
    '#title' => t('Image Source:'),
    '#default_value' => variable_get('user_details_loggedin_create_imgsrc', "/" . drupal_get_path('module', 'user_details') . "/images/create-icon.png"),
    '#description' => t('<b>Default Value:</b> <i>/' . drupal_get_path('module', 'user_details') . '/images/create-icon.png</i>'),
    '#size' => 75,
    '#maxlength' => 500,
  );
  if (module_exists('uc_cart')) {
    $form['loggedin']['loggedin_builtin_links']['user_details_loggedin_ubercart_cart_view'] = array(
      '#type' => 'checkbox',
      '#title' => t('Ubercart Cart:'),
      '#default_value' => variable_get('user_details_loggedin_ubercart_cart_view', FALSE),
      '#description' => t('displays a link to the logged-in user\'s cart page of "Ubercart".')
    );
    $form['loggedin']['loggedin_builtin_links']['user_details_loggedin_ubercart_cart_imgsrc'] = array(
      '#type' => 'textfield',
      '#title' => t('Image Source:'),
      '#default_value' => variable_get('user_details_loggedin_ubercart_cart_imgsrc', "/" . drupal_get_path('module', 'user_details') . "/images/ubercart-cart-icon.png"),
      '#description' => t('<b>Default Value:</b> <i>/' . drupal_get_path('module', 'user_details') . '/images/ubercart-cart-icon.png</i>'),
      '#size' => 75,
      '#maxlength' => 500,
    );
  }
  else {
    $form['loggedin']['loggedin_builtin_links']['user_details_loggedin_ubercart_cart_view'] = array(
      '#type' => 'checkbox',
      '#title' => t('Ubercart Cart:'),
      '#default_value' => variable_get('user_details_loggedin_ubercart_cart_view', FALSE),
      '#description' => t('install <a href="http://drupal.org/project/ubercart">Ubercart</a> to enable this option.'),
      '#disabled' => TRUE,
    );
  }
  if (module_exists('search')) {
    $form['loggedin']['loggedin_builtin_links']['user_details_loggedin_search_view'] = array(
      '#type' => 'checkbox',
      '#title' => t('Search:'),
      '#default_value' => variable_get('user_details_loggedin_search_view', FALSE),
      '#description' => t('displays a link to the search page.')
    );
    $form['loggedin']['loggedin_builtin_links']['user_details_loggedin_search_imgsrc'] = array(
      '#type' => 'textfield',
      '#title' => t('Image Source:'),
      '#default_value' => variable_get('user_details_loggedin_search_imgsrc', "/" . drupal_get_path('module', 'user_details') . "/images/search-icon.png"),
      '#description' => t('<b>Default Value:</b> <i>/' . drupal_get_path('module', 'user_details') . '/images/search-icon.png</i>'),
      '#size' => 75,
      '#maxlength' => 500,
    );
  }
  else {
    $form['loggedin']['loggedin_builtin_links']['user_details_loggedin_search_view'] = array(
      '#type' => 'checkbox',
      '#title' => t('Search:'),
      '#default_value' => variable_get('user_details_loggedin_search_view', FALSE),
      '#description' => t('enable the "Search" module to enable this option.'),
      '#disabled' => TRUE,
    );
  }
  $form['loggedin']['loggedin_builtin_links']['user_details_loggedin_admin_view'] = array(
    '#type' => 'checkbox',
    '#title' => t('Admin:'),
    '#default_value' => variable_get('user_details_loggedin_admin_view', FALSE),
    '#description' => t('displays a link to the administration section of the website.')
  );
  $form['loggedin']['loggedin_builtin_links']['user_details_loggedin_admin_imgsrc'] = array(
    '#type' => 'textfield',
    '#title' => t('Image Source:'),
    '#default_value' => variable_get('user_details_loggedin_admin_imgsrc', "/" . drupal_get_path('module', 'user_details') . "/images/admin-icon.png"),
    '#description' => t('<b>Default Value:</b> <i>/' . drupal_get_path('module', 'user_details') . '/images/admin-icon.png</i>'),
    '#size' => 75,
    '#maxlength' => 500,
  );
  if (module_exists('user_details')) {
    $form['loggedin']['loggedin_builtin_links']['user_details_loggedin_user_details_view'] = array(
      '#type' => 'checkbox',
      '#title' => t('User Details:'),
      '#default_value' => variable_get('user_details_loggedin_user_details_view', FALSE),
      '#description' => t('displays a link to the "User Details" section of the website.')
    );
    $form['loggedin']['loggedin_builtin_links']['user_details_loggedin_user_details_imgsrc'] = array(
      '#type' => 'textfield',
      '#title' => t('Image Source:'),
      '#default_value' => variable_get('user_details_loggedin_user_details_imgsrc', "/" . drupal_get_path('module', 'user_details') . "/images/user-details-icon.png"),
      '#description' => t('<b>Default Value:</b> <i>/' . drupal_get_path('module', 'user_details') . '/images/user-details-icon.png</i>'),
      '#size' => 75,
      '#maxlength' => 500,
    );
  }
  else {
    $form['loggedin']['loggedin_builtin_links']['user_details_loggedin_user_details_view'] = array(
      '#type' => 'checkbox',
      '#title' => t('User Details:'),
      '#default_value' => variable_get('user_details_loggedin_user_details_view', FALSE),
      '#description' => t('install <a href="http://drupal.org/project/user_details">User Details</a> to enable this option.'),
      '#disabled' => TRUE,
    );
  }
  if (module_exists('panels')) {
    $form['loggedin']['loggedin_builtin_links']['user_details_loggedin_panels_view'] = array(
      '#type' => 'checkbox',
      '#title' => t('Panels:'),
      '#default_value' => variable_get('user_details_loggedin_panels_view', FALSE),
      '#description' => t('displays a link to the "Panels" section of the website.')
    );
    $form['loggedin']['loggedin_builtin_links']['user_details_loggedin_panels_imgsrc'] = array(
      '#type' => 'textfield',
      '#title' => t('Image Source:'),
      '#default_value' => variable_get('user_details_loggedin_panels_imgsrc', "/" . drupal_get_path('module', 'user_details') . "/images/panels-icon.png"),
      '#description' => t('<b>Default Value:</b> <i>/' . drupal_get_path('module', 'user_details') . '/images/panels-icon.png</i>'),
      '#size' => 75,
      '#maxlength' => 500,
    );
  }
  else {
    $form['loggedin']['loggedin_builtin_links']['user_details_loggedin_panels_view'] = array(
      '#type' => 'checkbox',
      '#title' => t('Panels:'),
      '#default_value' => variable_get('user_details_loggedin_panels_view', FALSE),
      '#description' => t('install <a href="http://drupal.org/project/panels">Panels</a> to enable this option.'),
      '#disabled' => TRUE,
    );
  }
  if (module_exists('views')) {
    $form['loggedin']['loggedin_builtin_links']['user_details_loggedin_views_view'] = array(
      '#type' => 'checkbox',
      '#title' => t('Views:'),
      '#default_value' => variable_get('user_details_loggedin_views_view', FALSE),
      '#description' => t('displays a link to the "Views" section of the website.')
    );
    $form['loggedin']['loggedin_builtin_links']['user_details_loggedin_views_imgsrc'] = array(
      '#type' => 'textfield',
      '#title' => t('Image Source:'),
      '#default_value' => variable_get('user_details_loggedin_views_imgsrc', "/" . drupal_get_path('module', 'user_details') . "/images/views-icon.png"),
      '#description' => t('<b>Default Value:</b> <i>/' . drupal_get_path('module', 'user_details') . '/images/views-icon.png</i>'),
      '#size' => 75,
      '#maxlength' => 500,
    );
  }
  else {
    $form['loggedin']['loggedin_builtin_links']['user_details_loggedin_views_view'] = array(
      '#type' => 'checkbox',
      '#title' => t('Views:'),
      '#default_value' => variable_get('user_details_loggedin_views_view', FALSE),
      '#description' => t('install <a href="http://drupal.org/project/views">Views</a> to enable this option.'),
      '#disabled' => TRUE,
    );
  }
  $form['loggedin']['loggedin_builtin_links']['user_details_loggedin_performance_view'] = array(
    '#type' => 'checkbox',
    '#title' => t('Performance:'),
    '#default_value' => variable_get('user_details_loggedin_performance_view', FALSE),
    '#description' => t('displays a link to the performance section of the website.')
  );
  $form['loggedin']['loggedin_builtin_links']['user_details_loggedin_performance_imgsrc'] = array(
    '#type' => 'textfield',
    '#title' => t('Image Source:'),
    '#default_value' => variable_get('user_details_loggedin_performance_imgsrc', "/" . drupal_get_path('module', 'user_details') . "/images/performance-icon.png"),
    '#description' => t('<b>Default Value:</b> <i>/' . drupal_get_path('module', 'user_details') . '/images/performance-icon.png</i>'),
    '#size' => 75,
    '#maxlength' => 500,
  );
  $form['loggedin']['loggedin_builtin_links']['user_details_loggedin_logout_view'] = array(
    '#type' => 'checkbox',
    '#title' => t('Logout:'),
    '#default_value' => variable_get('user_details_loggedin_logout_view', FALSE),
    '#description' => t('displays a link to allow the logged-in user to logout.')
  );
  $form['loggedin']['loggedin_builtin_links']['user_details_loggedin_logout_imgsrc'] = array(
    '#type' => 'textfield',
    '#title' => t('Image Source:'),
    '#default_value' => variable_get('user_details_loggedin_logout_imgsrc', "/" . drupal_get_path('module', 'user_details') . "/images/logout-icon.png"),
    '#description' => t('<b>Default Value:</b> <i>/' . drupal_get_path('module', 'user_details') . '/images/logout-icon.png</i>'),
    '#size' => 75,
    '#maxlength' => 500,
  );

//Custom Links
  $form['loggedin']['loggedin_custom_links'] = array(
    '#type' => 'fieldset',
    '#title' => 'Custom Links',
  );
  $form['loggedin']['loggedin_custom_links']['link_one_header'] = array(
    '#prefix' => '<div class="header"><h3>',
    '#markup' => t('Link One'),
    '#suffix' => '</h3></div>',
  );
  $form['loggedin']['loggedin_custom_links']['user_details_loggedin_link_one_type'] = array(
    '#type' => 'select',
    '#title' => t('Link Type:'),
    '#options' => array(
      0 => t('Disabled'),
      1 => t('Admin Link'),
      2 => t('User Link'),
    ),
    '#default_value' => variable_get('user_details_loggedin_link_one_type', '0'),
  );
  $form['loggedin']['loggedin_custom_links']['user_details_loggedin_link_one_url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL Path:'),
    '#default_value' => variable_get('user_details_loggedin_link_one_url', ''),
    '#size' => 75,
    '#maxlength' => 500,
  );
  $form['loggedin']['loggedin_custom_links']['user_details_loggedin_link_one_imgsrc'] = array(
    '#type' => 'textfield',
    '#title' => t('Image path:'),
    '#default_value' => variable_get('user_details_loggedin_link_one_imgsrc', ''),
    '#size' => 75,
    '#maxlength' => 500,
  );
  $form['loggedin']['loggedin_custom_links']['user_details_loggedin_link_one_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Image title:'),
    '#default_value' => variable_get('user_details_loggedin_link_one_title', ''),
    '#size' => 75,
    '#maxlength' => 500,
  );
  $form['loggedin']['loggedin_custom_links']['link_two_header'] = array(
    '#prefix' => '<div class="header"><h3>',
    '#markup' => t('Link Two'),
    '#suffix' => '</h3></div>',
  );
  $form['loggedin']['loggedin_custom_links']['user_details_loggedin_link_two_type'] = array(
    '#type' => 'select',
    '#title' => t('Link Type:'),
    '#options' => array(
      0 => t('Disabled'),
      1 => t('Admin Link'),
      2 => t('User Link'),
    ),
    '#default_value' => variable_get('user_details_loggedin_link_two_type', '0'),
  );
  $form['loggedin']['loggedin_custom_links']['user_details_loggedin_link_two_url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL path:'),
    '#default_value' => variable_get('user_details_loggedin_link_two_url', ''),
    '#size' => 75,
    '#maxlength' => 500,
  );
  $form['loggedin']['loggedin_custom_links']['user_details_loggedin_link_two_imgsrc'] = array(
    '#type' => 'textfield',
    '#title' => t('Image path:'),
    '#default_value' => variable_get('user_details_loggedin_link_two_imgsrc', ''),
    '#size' => 75,
    '#maxlength' => 500,
  );
  $form['loggedin']['loggedin_custom_links']['user_details_loggedin_link_two_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Image title:'),
    '#default_value' => variable_get('user_details_loggedin_link_two_title', ''),
    '#size' => 75,
    '#maxlength' => 500,
  );
  $form['loggedin']['loggedin_custom_links']['link_three_header'] = array(
    '#prefix' => '<div class="header"><h3>',
    '#markup' => t('Link Three'),
    '#suffix' => '</h3></div>',
  );
  $form['loggedin']['loggedin_custom_links']['user_details_loggedin_link_three_type'] = array(
    '#type' => 'select',
    '#title' => t('Link Type:'),
    '#options' => array(
      0 => t('Disabled'),
      1 => t('Admin Link'),
      2 => t('User Link'),
    ),
    '#default_value' => variable_get('user_details_loggedin_link_three_type', '0'),
  );
  $form['loggedin']['loggedin_custom_links']['user_details_loggedin_link_three_url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL Path:'),
    '#default_value' => variable_get('user_details_loggedin_link_three_url', ''),
    '#size' => 75,
    '#maxlength' => 500,
  );
  $form['loggedin']['loggedin_custom_links']['user_details_loggedin_link_three_imgsrc'] = array(
    '#type' => 'textfield',
    '#title' => t('Image Path:'),
    '#default_value' => variable_get('user_details_loggedin_link_three_imgsrc', ''),
    '#size' => 75,
    '#maxlength' => 500,
  );
  $form['loggedin']['loggedin_custom_links']['user_details_loggedin_link_three_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Image Title:'),
    '#default_value' => variable_get('user_details_loggedin_link_three_title', ''),
    '#size' => 75,
    '#maxlength' => 500,
  );
  $form['loggedin']['loggedin_custom_links']['link_four_header'] = array(
    '#prefix' => '<div class="header"><h3>',
    '#markup' => t('Link Four'),
    '#suffix' => '</h3></div>',
  );
  $form['loggedin']['loggedin_custom_links']['user_details_loggedin_link_four_type'] = array(
    '#type' => 'select',
    '#title' => t('Link Type:'),
    '#options' => array(
      0 => t('Disabled'),
      1 => t('Admin Link'),
      2 => t('User Link'),
    ),
    '#default_value' => variable_get('user_details_loggedin_link_four_type', '0'),
  );
  $form['loggedin']['loggedin_custom_links']['user_details_loggedin_link_four_url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL path:'),
    '#default_value' => variable_get('user_details_loggedin_link_four_url', ''),
    '#size' => 75,
    '#maxlength' => 500,
  );
  $form['loggedin']['loggedin_custom_links']['user_details_loggedin_link_four_imgsrc'] = array(
    '#type' => 'textfield',
    '#title' => t('Image path:'),
    '#default_value' => variable_get('user_details_loggedin_link_four_imgsrc', ''),
    '#size' => 75,
    '#maxlength' => 500,
  );
  $form['loggedin']['loggedin_custom_links']['user_details_loggedin_link_four_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Image title:'),
    '#default_value' => variable_get('user_details_loggedin_link_four_title', ''),
    '#size' => 75,
    '#maxlength' => 500,
  );
  $form['loggedin']['loggedin_custom_links']['link_five_header'] = array(
    '#prefix' => '<div class="header"><h3>',
    '#markup' => t('Link Five'),
    '#suffix' => '</h3></div>',
  );
  $form['loggedin']['loggedin_custom_links']['user_details_loggedin_link_five_type'] = array(
    '#type' => 'select',
    '#title' => t('Link Type'),
    '#options' => array(
      0 => t('Disabled'),
      3 => t('Admin Link'),
      2 => t('User Link'),
    ),
    '#default_value' => variable_get('user_details_loggedin_link_five_type', '0'),
  );
  $form['loggedin']['loggedin_custom_links']['user_details_loggedin_link_five_url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL path:'),
    '#default_value' => variable_get('user_details_loggedin_link_five_url', ''),
    '#size' => 75,
    '#maxlength' => 500,
  );
  $form['loggedin']['loggedin_custom_links']['user_details_loggedin_link_five_imgsrc'] = array(
    '#type' => 'textfield',
    '#title' => t('Image path:'),
    '#default_value' => variable_get('user_details_loggedin_link_five_imgsrc', ''),
    '#size' => 75,
    '#maxlength' => 500,
  );
  $form['loggedin']['loggedin_custom_links']['user_details_loggedin_link_five_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Image title:'),
    '#default_value' => variable_get('user_details_loggedin_link_five_title', ''),
    '#size' => 75,
    '#maxlength' => 500,
  );

//Content
  $form['loggedin']['loggedin_content'] = array(
    '#type' => 'fieldset',
    '#title' => 'User Content',
  );
  $content_amount = variable_get('user_details_loggedin_content_amount');
  $form['loggedin']['loggedin_content']['user_details_loggedin_content_view'] = array(
    '#type' => 'checkbox',
    '#title' => t('Content:'),
    '#default_value' => variable_get('user_details_loggedin_content_view', FALSE),
    '#description' => t('Lists the last %content_amount updated pages authored by the user.', array(
      '%content_amount' => $content_amount,
      )
    )
  );
  $form['loggedin']['loggedin_content']['user_details_loggedin_content_amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Amount:'),
    '#default_value' => variable_get('user_details_loggedin_content_amount', 3),
    '#size' => 2,
    '#maxlength' => 2,
    '#description' => t('Changes the number of recent items listed for the "User Content" list.')
  );
  $form['loggedin']['loggedin_content']['user_details_loggedin_content_truncate'] = array(
    '#type' => 'textfield',
    '#title' => t('Truncate Amount:'),
    '#default_value' => variable_get('user_details_loggedin_content_truncate', 20),
    '#size' => 2,
    '#maxlength' => 2,
    '#description' => t('Changes the length of the node title.')
  );

  return system_settings_form($form);
}