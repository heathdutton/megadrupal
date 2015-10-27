<?php
/**
 * Implements hook_form_system_theme_settings_alter() function.
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 * @param $form_state
 *   A keyed array containing the current state of the form.
 */
function black_hole_form_system_theme_settings_alter(&$form, &$form_state) {
// Work-around for a core bug affecting admin themes.
  if (isset($form_id)) {
    return;
  }

// Create theme settings form widgets using Forms API

// Layout Settings
  $form['tnt_container']['layout_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Layout settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['tnt_container']['layout_settings']['style'] = array(
    '#type' => 'select',
    '#title' => t('Style'),
    '#default_value' => theme_get_setting('style'),
    '#options' => array(
      'nebula' => t('Nebula'),
      'protostar' => t('Protostar'),
      'star' => t('Star'),
      'planet' => t('Planet'),
      'red_giant' => t('Red Giant'),
      'nebula_pl' => t('Planetary Nebula'),
      'darf' => t('Darf'),
      'supernova' => t('Supernova'),
      'blackhole' => t('Black Hole'),
      'themer' => t('- Themer -'),
    ),
  );

  $form['tnt_container']['layout_settings']['layout-width'] = array(
    '#type' => 'select',
    '#title' => t('Layout width'),
    '#default_value' => theme_get_setting('layout-width'),
    '#description' => t('<em>Fluid width</em> and <em>Fixed width</em> can be customized in _custom/custom-style.css.'),
    '#options' => array(
      0 => t('Adaptive width'),
      1 => t('Fluid width (custom)'),
      2 => t('Fixed width (custom)'),
    ),
  );

  $form['tnt_container']['layout_settings']['sidebarslayout'] = array(
    '#type' => 'select',
    '#title' => t('Sidebars layout'),
    '#default_value' => theme_get_setting('sidebarslayout'),
    '#description' => t('<b>Variable width sidebars (wide)</b>: If only one sidebar is enabled, content width is 250px for the left sidebar and 300px for the right sidebar. If both sidebars are enabled, content width is 160px for the left sidebar and 234px for the right sidebar. <br /> <b>Fixed width sidebars (wide)</b>: Content width is 160px for the left sidebar and 234px for the right sidebar. <br /> <em>Equal width sidebars</em> ca be customized in _custom/custom-style.css. For other details, please refer to README.txt.'),
    '#options' => array(
      0 => t('Variable asyimmetrical sidebars (wide)'),
      1 => t('Fixed asyimmetrical sidebars (wide)'),
      2 => t('Variable asyimmetrical sidebars (narrow)'),
      3 => t('Fixed asyimmetrical sidebars (narrow)'),
      4 => t('Equal width sidebars (custom)'),
    )
  );

  $form['tnt_container']['layout_settings']['themedblocks'] = array(
    '#type' => 'select',
    '#title' => t('Themed blocks'),
    '#default_value' => theme_get_setting('themedblocks'),
    '#options' => array(
      0 => t('Sidebars only'),
      1 => t('Sidebars + User regions'),
      2 => t('User regions only'),
      3 => t('None'),
    )
  );

  $form['tnt_container']['layout_settings']['blockicons'] = array(
    '#type' => 'select',
    '#title' => t('Block icons'),
    '#default_value' => theme_get_setting('blockicons'),
    '#options' => array(
      0 => t('No'),
      1 => t('Yes (32x32 pixels, positive images)'),
      2 => t('Yes (48x48 pixels, positive images)'),
      3 => t('Yes (32x32 pixels, negative images)'),
      4 => t('Yes (48x48 pixels, negative images)'),
    )
  );

  $form['tnt_container']['layout_settings']['pageicons'] = array(
    '#type' => 'checkbox',
    '#title' => t('Page icons'),
    '#default_value' => theme_get_setting('pageicons'),
  );

  $form['tnt_container']['layout_settings']['navpos'] = array(
    '#type' => 'select',
    '#title' => t('Menu style and position'),
    '#default_value' => theme_get_setting('navpos'),
    '#description' => t('"Out of the box" Black Hole will display a static menu. To activate the drop-down menu put the <a href="?q=/admin/structure/block/manage/system/main-menu/configure">Main menu block</a> in the "Drop Down menu" region and set the correct levels to "expanded" (the parent item). <br /> <b>NOTE</b>: Only the static menu can be properly centered.'),
    '#options' => array(
      0 => t('Left'),
      1 => t('Center'),
      2 => t('Right'),
    )
  );

    $form['tnt_container']['layout_settings']['fntsize'] = array(
    '#type' => 'select',
    '#title' => t('Font size'),
    '#default_value' => theme_get_setting('fntsize'),
    '#options' => array(
      0 => t('Normal'),
      1 => t('Large'),
    )
  );

  $form['tnt_container']['layout_settings']['roundcorners'] = array(
    '#type' => 'checkbox',
    '#title' => t('Rounded corners'),
    '#description' => t('Some page elements (comments, search, blocks) and main menu will have rounded corners.'),
    '#default_value' => theme_get_setting('roundcorners'),
  );

  $form['tnt_container']['layout_settings']['headerimg'] = array(
    '#type' => 'checkbox',
    '#title' => t('Header image rotator'),
    '#description' => t('Rotates images in the _custom/headerimg folder.'),
    '#default_value' => theme_get_setting('headerimg'),
  );

  $form['tnt_container']['layout_settings']['loginlinks'] = array(
    '#type' => 'checkbox',
    '#title' => t('Black Hole login/register links'),
    '#default_value' => theme_get_setting('loginlinks'),
  );

  $form['tnt_container']['layout_settings']['devlink'] = array(
    '#type' => 'checkbox',
    '#title' => t('Developer link'),
    '#default_value' => theme_get_setting('devlink'),
  );

// Breadcrumb
  $form['tnt_container']['breadcrumb'] = array(
    '#type' => 'fieldset',
    '#title' => t('Breadcrumb'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['tnt_container']['breadcrumb']['breadcrumb_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display breadcrumb'),
    '#default_value' => theme_get_setting('breadcrumb_display'),
  );

// Author & Date Settings
  $form['tnt_container']['submitted_by'] = array(
    '#type' => 'fieldset',
    '#title' => t('Author & date'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['tnt_container']['submitted_by']['postedby'] = array(
    '#type' => 'select',
//    '#title' => t('Submitted by'),
    '#default_value' => theme_get_setting('postedby'),
    '#description' => t('Change "Submitted by" display on all nodes, site-wide.'),
    '#options' => array(
      0 => t('Date & Username'),
      1 => t('Username only'),
      2 => t('Date only'),
    )
  );

// Social links
  $form['tnt_container']['social_links'] = array(
    '#type' => 'fieldset',
    '#title' => t('Social links'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['tnt_container']['social_links']['social_links_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display social links at the top of the page'),
    '#default_value' => theme_get_setting('social_links_display'),
  );
  $form['tnt_container']['social_links']['social_links_display_links'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display these social links'),
    '#collapsible' => TRUE,
  );
  $form['tnt_container']['social_links']['social_links_display_links']['social_links_display_links_facebook'] = array(
    '#type' => 'checkbox',
    '#title' => t('Facebook'),
    '#default_value' => theme_get_setting('social_links_display_links_facebook'),
  );
  $form['tnt_container']['social_links']['social_links_display_links']['social_links_display_links_facebook_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Link to Facebook page'),
    '#description' => t('Enter the link to your Facebook page.'),
    '#size' => 60,
    '#default_value' => theme_get_setting('social_links_display_links_facebook_link'),
  );
  $form['tnt_container']['social_links']['social_links_display_links']['social_links_display_links_googleplus'] = array(
    '#type' => 'checkbox',
    '#title' => t('Google Plus'),
    '#default_value' => theme_get_setting('social_links_display_links_googleplus'),
  );
  $form['tnt_container']['social_links']['social_links_display_links']['social_links_display_links_googleplus_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Link to Google Plus page'),
    '#description' => t('Enter the link to your Google Plus account.'),
    '#size' => 60,
    '#default_value' => theme_get_setting('social_links_display_links_googleplus_link'),
  );
  $form['tnt_container']['social_links']['social_links_display_links']['social_links_display_links_twitter'] = array(
    '#type' => 'checkbox',
    '#title' => t('Twitter'),
    '#default_value' => theme_get_setting('social_links_display_links_twitter'),
  );
  $form['tnt_container']['social_links']['social_links_display_links']['social_links_display_links_twitter_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Link to Twitter page'),
    '#description' => t('Enter the link to your Twitter account.'),
    '#size' => 60,
    '#default_value' => theme_get_setting('social_links_display_links_twitter_link'),
  );
  $form['tnt_container']['social_links']['social_links_display_links']['social_links_display_links_instagram'] = array(
    '#type' => 'checkbox',
    '#title' => t('Instagram'),
    '#default_value' => theme_get_setting('social_links_display_links_instagram'),
  );
  $form['tnt_container']['social_links']['social_links_display_links']['social_links_display_links_instagram_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Link to Instagram page'),
    '#description' => t('Enter the link to your Instagram account.'),
    '#size' => 60,
    '#default_value' => theme_get_setting('social_links_display_links_instagram_link'),
  );
  $form['tnt_container']['social_links']['social_links_display_links']['social_links_display_links_pinterest'] = array(
    '#type' => 'checkbox',
    '#title' => t('Pinterest'),
    '#default_value' => theme_get_setting('social_links_display_links_pinterest'),
  );
  $form['tnt_container']['social_links']['social_links_display_links']['social_links_display_links_pinterest_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Link to Pinterest page'),
    '#description' => t('Enter the link to your Pinterest account.'),
    '#size' => 60,
    '#default_value' => theme_get_setting('social_links_display_links_pinterest_link'),
  );
  $form['tnt_container']['social_links']['social_links_display_links']['social_links_display_links_linkedin'] = array(
    '#type' => 'checkbox',
    '#title' => t('LinkedIn'),
    '#default_value' => theme_get_setting('social_links_display_links_linkedin'),
  );
  $form['tnt_container']['social_links']['social_links_display_links']['social_links_display_links_linkedin_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Link to LinkedIn page'),
    '#description' => t('Enter the link to your LinkedIn account.'),
    '#size' => 60,
    '#default_value' => theme_get_setting('social_links_display_links_linkedin_link'),
  );
  $form['tnt_container']['social_links']['social_links_display_links']['social_links_display_links_youtube'] = array(
    '#type' => 'checkbox',
    '#title' => t('Youtube'),
    '#default_value' => theme_get_setting('social_links_display_links_youtube'),
  );
  $form['tnt_container']['social_links']['social_links_display_links']['social_links_display_links_youtube_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Link to Youtube page'),
    '#description' => t('Enter the link to your Youtube account.'),
    '#size' => 60,
    '#default_value' => theme_get_setting('social_links_display_links_youtube_link'),
  );
  $form['tnt_container']['social_links']['social_links_display_links']['social_links_display_links_vimeo'] = array(
    '#type' => 'checkbox',
    '#title' => t('Vimeo'),
    '#default_value' => theme_get_setting('social_links_display_links_vimeo'),
  );
  $form['tnt_container']['social_links']['social_links_display_links']['social_links_display_links_vimeo_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Link to Vimeo page'),
    '#description' => t('Enter the link to your Vimeo account.'),
    '#size' => 60,
    '#default_value' => theme_get_setting('social_links_display_links_vimeo_link'),
  );
  $form['tnt_container']['social_links']['social_links_display_links']['social_links_display_links_flickr'] = array(
    '#type' => 'checkbox',
    '#title' => t('Flickr'),
    '#default_value' => theme_get_setting('social_links_display_links_flickr'),
  );
  $form['tnt_container']['social_links']['social_links_display_links']['social_links_display_links_flickr_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Link to Flickr page'),
    '#description' => t('Enter the link to your Flickr account.'),
    '#size' => 60,
    '#default_value' => theme_get_setting('social_links_display_links_flickr_link'),
  );
  $form['tnt_container']['social_links']['social_links_display_links']['social_links_display_links_tumblr'] = array(
    '#type' => 'checkbox',
    '#title' => t('Tumblr'),
    '#default_value' => theme_get_setting('social_links_display_links_tumblr'),
  );
  $form['tnt_container']['social_links']['social_links_display_links']['social_links_display_links_tumblr_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Link to Tumblr page'),
    '#description' => t('Enter the link to your Tumblr account.'),
    '#size' => 60,
    '#default_value' => theme_get_setting('social_links_display_links_tumblr_link'),
  );
  $form['tnt_container']['social_links']['social_links_display_links']['social_links_display_links_skype'] = array(
    '#type' => 'checkbox',
    '#title' => t('Skype'),
    '#default_value' => theme_get_setting('social_links_display_links_skype'),
  );
  $form['tnt_container']['social_links']['social_links_display_links']['social_links_display_links_skype_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Link to Skype page'),
    '#description' => t('Enter the contact link to your Skype account (<b>skype:username?call</b>).'),
    '#size' => 60,
    '#default_value' => theme_get_setting('social_links_display_links_skype_link'),
  );
  $form['tnt_container']['social_links']['social_links_display_links']['social_links_display_links_myother'] = array(
    '#type' => 'checkbox',
    '#title' => t('Other Social Network'),
    '#default_value' => theme_get_setting('social_links_display_links_myother'),
  );
  $form['tnt_container']['social_links']['social_links_display_links']['social_links_display_links_myother_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Link to other social network page (custom)'),
    '#description' => t('Enter the link to other social network page.'),
    '#size' => 60,
    '#default_value' => theme_get_setting('social_links_display_links_myother_link'),
  );

// Development settings
  $form['tnt_container']['themedev'] = array(
    '#type' => 'fieldset',
    '#title' => t('Theme development settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['tnt_container']['themedev']['rebuild_registry'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Rebuild theme registry on every page.'),
    '#default_value' => theme_get_setting('rebuild_registry'),
    '#description'   => t('During theme development, it can be very useful to continuously rebuild the theme registry</a>. <br /> WARNING: this is a huge performance penalty and must be turned off on production websites.'),
  );
  $form['tnt_container']['themedev']['siteid'] = array(
    '#type' => 'textfield',
    '#title' => t('Site ID bodyclass.'),
   	'#description' => t('In order to have different styles of Black Hole in a multisite environment you may find usefull to choose a "one word" site ID and customize the look of each site in custom-style.css file.'),
    '#default_value' => theme_get_setting('siteid'),
    '#size' => 10,
	);

// Info
  $form['tnt_container']['info'] = array(
    '#type' => 'fieldset',
    '#title' => t('Info'),
    '#description'   => t('All Black Hole settings are <b>multilingual variables</b>. You can have different settings for each language.'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

}  
