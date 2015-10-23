<?php
/**
 * Implements hook_form_system_theme_settings_alter() function.
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 * @param $form_state
 *   A keyed array containing the current state of the form.
 */
function abc_form_system_theme_settings_alter(&$form, &$form_state) {
// Work-around for a core bug affecting admin themes.
  if (isset($form_id)) {
    return;
  }

// Create the form using Forms API


// Logo Settings
  $form['logo']['logoabc'] = array(
    '#type' => 'select',
    '#description' => t('Choose the default letter as logo.'),
    '#default_value' => theme_get_setting('logoabc'),
    '#options' => array(
      'a' => 'A',
      'b' => 'B',
      'c' => 'C',
      'd' => 'D',
      'e' => 'E',
      'f' => 'F',
      'g' => 'G',
      'h' => 'H',
      'i' => 'I',
      'j' => 'J',
      'k' => 'K',
      'l' => 'L',
      'm' => 'M',
      'n' => 'N',
      'o' => 'O',
      'p' => 'P',
      'q' => 'Q',
      'r' => 'R',
      's' => 'S',
      't' => 'T',
      'u' => 'U',
      'v' => 'V',
      'w' => 'W',
      'x' => 'X',
      'y' => 'Y',
      'z' => 'Z',
    ),
  );
  $form['logo']['logocol'] = array(
    '#type' => 'select',
    '#description' => t('Choose the default logo color.'),
    '#default_value' => theme_get_setting('logocol'),
    '#options' => array(
      'pink'   => t('Pink Logo'),
      'green'  => t('Green Logo'),
      'orange' => t('Orange Logo'),
      'blue'   => t('Blue Logo'),
      'grey'   => t('Grey Logo'),
    ),
  );


// Pure Grid settings
  $form['tnt_container']['puregrid'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pure Grid settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['tnt_container']['puregrid']['css_zone'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('<b>Use Yahoo CDN</b> to serve the responsive CSS files. If you use https leave this option unchecked to load any responsive CSS files locally.'),
    '#default_value' => theme_get_setting('css_zone')
  );
  $form['tnt_container']['puregrid']['wrapper'] = array(
    '#type' => 'textfield',
    '#title' => t('Layout width'),
   	'#description' => t('Set the width of the layout in <b>em</b> (preferably), px or percent. Leave empty or 100% for fluid layout.'),
    '#default_value' => theme_get_setting('wrapper'),
    '#size' => 10,
	);
  $form['tnt_container']['puregrid']['first_width'] = array(
    '#type' => 'select',
    '#title' => t('First (left) sidebar width'),
   	'#description' => t('Set the width of the first (left) sidebar.'),
    '#default_value' => theme_get_setting('first_width'),
    '#options' => array(
      4 => t('narrow'),
      5 => t('NORMAL'),
      6 => t('wide'),
      7 => t('wider'),
    ),
	);
  $form['tnt_container']['puregrid']['second_width'] = array(
    '#type' => 'select',
    '#title' => t('Second (right) sidebar width'),
   	'#description' => t('Set the width of the second (right) sidebar.'),
    '#default_value' => theme_get_setting('second_width'),
    '#options' => array(
      4 => t('narrow'),
      5 => t('NORMAL'),
      6 => t('wide'),
      7 => t('wider'),
    ),
	);
  $form['tnt_container']['puregrid']['grid_responsive'] = array(
    '#type'          => 'select',
    '#title'         => t('Non-Responsive/Responsive Grid'),
    '#default_value' => theme_get_setting('grid_responsive'),
    '#options' => array(
      0 => t('Non-Responsive'),
      1 => t('Responsive'),
    ),
  );
  $form['tnt_container']['puregrid']['mobile_blocks'] = array(
    '#type' => 'select',
    '#title' => t('Hide blocks on mobile devices'),
   	'#description' => t('If the theme is responsive and there are many blocks you may want to hide some of them when on mobile devices.'),
    '#default_value' => theme_get_setting('mobile_blocks'),
    '#options' => array(
      0 => t('Show all blocks'),
      1 => t('Hide blocks on user regions 1-4'),
      2 => t('Hide blocks on user regions 1-4 and left sidebar'),
      3 => t('Hide blocks on all user regions'),
      4 => t('Hide blocks on all user regions and left sidebar'),
      5 => t('Hide blocks on all user regions and both sidebars'),
    ),
	);


// Layout Settings
  $form['tnt_container']['layout_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Layout settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );


// Background Settings
  $form['tnt_container']['layout_settings']['background_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Background settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['tnt_container']['layout_settings']['background_settings']['bgimg'] = array(
    '#type' => 'checkbox',
    '#title' => t('Image'),
    '#default_value' => theme_get_setting('bgimg'),
  );
  $form['tnt_container']['layout_settings']['background_settings']['bgcol'] = array(
    '#type' => 'select',
    '#title' => t('Color'),
    '#default_value' => theme_get_setting('bgcol'),
    '#options' => array(
      0 => t('None (custom)'),
      1 => t('Pink'),
      2 => t('Green'),
      3 => t('Yellow'),
      4 => t('Blue'),
      5 => t('Black & White'),
    )
  );
  $form['tnt_container']['layout_settings']['background_settings']['bgpoz'] = array(
    '#type' => 'select',
    '#title' => t('Position'),
    '#default_value' => theme_get_setting('bgpoz'),
    '#options' => array(
      0 => t('Static'),
      1 => t('Fixed'),
    )
  );

// Title settings
  $form['tnt_container']['layout_settings']['title_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Title settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['tnt_container']['layout_settings']['title_settings']['sncol'] = array(
    '#type' => 'select',
    '#title' => t('Site Name Color'),
    '#default_value' => theme_get_setting('sncol'),
    '#options' => array(
      0 => t('None (custom)'),
      1 => t('Pink'),
      2 => t('Green'),
      3 => t('Orange'),
      4 => t('Blue'),
      5 => t('Grey'),
    )
  );
  $form['tnt_container']['layout_settings']['title_settings']['ntcol'] = array(
    '#type' => 'select',
    '#title' => t('Node Title Color'),
    '#default_value' => theme_get_setting('ntcol'),
    '#options' => array(
      0 => t('None (custom)'),
      1 => t('Pink'),
      2 => t('Green'),
      3 => t('Orange'),
      4 => t('Blue'),
      5 => t('Grey'),
    )
  );

// Block settings
  $form['tnt_container']['layout_settings']['block_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Block settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['tnt_container']['layout_settings']['block_settings']['btcol'] = array(
    '#type' => 'select',
    '#title' => t('Block Title Color'),
    '#default_value' => theme_get_setting('btcol'),
    '#options' => array(
      0 => t('None (custom)'),
      1 => t('Pink'),
      2 => t('Green'),
      3 => t('Orange'),
      4 => t('Blue'),
      5 => t('Grey'),
      6 => t('Multicolor'),
    )
  );
  $form['tnt_container']['layout_settings']['block_settings']['blockicons'] = array(
    '#type' => 'select',
    '#title' => t('Block icons'),
    '#default_value' => theme_get_setting('blockicons'),
    '#options' => array(
      0 => t('No'),
      1 => t('Yes'),
    )
  );
  $form['tnt_container']['layout_settings']['block_settings']['themedblocks'] = array(
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

// Menu style and colous
  $form['tnt_container']['layout_settings']['menu_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Menu settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['tnt_container']['layout_settings']['menu_settings']['navpos'] = array(
    '#type' => 'select',
    '#title' => t('Drop-down and secondary menus position'),
    '#default_value' => theme_get_setting('navpos'),
    '#options' => array(
      0 => t('Left'),
      1 => t('Center'),
      2 => t('Right'),
    )
  );
  $form['tnt_container']['layout_settings']['menu_settings']['menu_colors'] = array(
    '#type' => 'fieldset',
    '#title' => t('Menu item colors'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['tnt_container']['layout_settings']['menu_settings']['menu_colors']['fbcol'] = array(
    '#type' => 'select',
    '#title' => t('First item'),
    '#default_value' => theme_get_setting('fbcol'),
    '#options' => array(
      0 => t('None (custom)'),
      1 => t('Pink'),
      2 => t('Green'),
      3 => t('Orange'),
      4 => t('Blue'),
      5 => t('Dark Blue'),
      6 => t('Light Grey'),
      7 => t('Grey'),
      8 => t('Dark Grey'),
      9 => t('Black'),
    )
  );
  $form['tnt_container']['layout_settings']['menu_settings']['menu_colors']['mbcol'] = array(
    '#type' => 'select',
    '#title' => t('Menu items'),
    '#default_value' => theme_get_setting('mbcol'),
    '#options' => array(
      0 => t('None (custom)'),
      1 => t('Pink'),
      2 => t('Green'),
      3 => t('Orange'),
      4 => t('Blue'),
      5 => t('Dark Blue'),
      6 => t('Light Grey'),
      7 => t('Grey'),
      8 => t('Dark Grey'),
      9 => t('Black'),
    )
  );
  $form['tnt_container']['layout_settings']['menu_settings']['menu_colors']['lbcol'] = array(
    '#type' => 'select',
    '#title' => t('Last item'),
    '#default_value' => theme_get_setting('lbcol'),
    '#options' => array(
      0 => t('None (custom)'),
      1 => t('Pink'),
      2 => t('Green'),
      3 => t('Orange'),
      4 => t('Blue'),
      5 => t('Dark Blue'),
      6 => t('Light Grey'),
      7 => t('Grey'),
      8 => t('Dark Grey'),
      9 => t('Black'),
    )
  );
  $form['tnt_container']['layout_settings']['menu_settings']['menu_colors']['mrcol'] = array(
    '#type' => 'select',
    '#title' => t('Rollover'),
    '#default_value' => theme_get_setting('mrcol'),
    '#options' => array(
      0 => t('None (custom)'),
      1 => t('Pink'),
      2 => t('Green'),
      3 => t('Orange'),
      4 => t('Blue'),
      5 => t('Dark Blue'),
      6 => t('Light Grey'),
      7 => t('Grey'),
      8 => t('Dark Grey'),
      9 => t('Black'),
    )
  );
  $form['tnt_container']['layout_settings']['menu_settings']['menu_colors']['trcol'] = array(
    '#type' => 'select',
    '#title' => t('Tertiary menu items color'),
    '#default_value' => theme_get_setting('trcol'),
    '#options' => array(
      0 => t('None (custom)'),
      1 => t('Pink'),
      2 => t('Green'),
      3 => t('Orange'),
      4 => t('Blue'),
      5 => t('Grey'),
    )
  );
  $form['tnt_container']['layout_settings']['menu_settings']['menu2'] = array(
    '#type' => 'checkbox',
    '#title' => t('Duplicate the Main Menu at the bottom of the page.'),
    '#default_value' => theme_get_setting('menu2'),
  );

// the rest of layout settings
  $form['tnt_container']['layout_settings']['fntsize'] = array(
    '#type' => 'select',
    '#title' => t('Font size'),
    '#default_value' => theme_get_setting('fntsize'),
    '#options' => array(
      0 => t('Small'),
      1 => t('Normal'),
      2 => t('Large'),
    )
  );
  $form['tnt_container']['layout_settings']['pageicons'] = array(
    '#type' => 'checkbox',
    '#title' => t('Page icons'),
    '#default_value' => theme_get_setting('pageicons'),
  );
  $form['tnt_container']['layout_settings']['roundcorners'] = array(
    '#type' => 'checkbox',
    '#title' => t('Rounded corners'),
    '#description' => t('Some page elements (comments, search, blocks) and main menu will have rounded corners.'),
    '#default_value' => theme_get_setting('roundcorners'),
  );
  $form['tnt_container']['layout_settings']['loginlinks'] = array(
    '#type' => 'checkbox',
    '#title' => t('Login/register links'),
    '#default_value' => theme_get_setting('loginlinks'),
  );
    $form['tnt_container']['layout_settings']['devlink'] = array(
    '#type' => 'checkbox',
    '#title' => t('Developer link'),
    '#default_value' => theme_get_setting('devlink'),
  );
  $form['tnt_container']['layout_settings']['breadcrumb_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display breadcrumb'),
    '#default_value' => theme_get_setting('breadcrumb_display'),
  );
  $form['tnt_container']['layout_settings']['postedby'] = array(
    '#type' => 'select',
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
    '#title' => t('Display social links at the bottom of the page'),
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
    '#description'   => t('During theme development, it can be very useful to continuously <a href="https://drupal.org/node/173880#theme-registry">rebuild the theme registry</a>. <br /> <div class="alert alert-warning messages warning"><b>WARNING</b>: this is a huge performance penalty and must be turned off on production websites.</div>'),
  );
  $form['tnt_container']['themedev']['siteid'] = array(
    '#type' => 'textfield',
    '#title' => t('Site ID body class.'),
   	'#description' => t('In order to have different styles of ABC theme in a multisite/multi-theme environment you may find usefull to choose an "ID" and customize the look of each site/theme in custom-style.css file.'),
    '#default_value' => theme_get_setting('siteid'),
    '#size' => 10,
	);

// Info
  $form['tnt_container']['info'] = array(
    '#type' => 'fieldset',
    '#description'   => t('<div class="messages info">Some of the theme settings are <b>multilingual variables</b>. You may have different settings for each language.</div>'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

}  
