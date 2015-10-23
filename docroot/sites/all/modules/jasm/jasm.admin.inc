<?php
/**
 * @file
 * Provides the JASM administrative interface.
 */
function jasm_admin_settings() {
  $jasm = jasm_get_values();

  $form = array();
  $form['#tree'] = TRUE;

  // GENERAL SETTINGS
  $form['jasm'] = array(
    '#type' => 'vertical_tabs',
  );

  $form['jasm_general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General display settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => -11,
    '#group' => 'jasm',
  );

  $form['jasm_general']['service_list_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Override default block title'),
    '#default_value' => $jasm['jasm_general']['service_list_title'],
    '#description' => t('By default, the service list block title is "Social". Include a custom title here.'),
    '#required' => FALSE,
  );


  $form['jasm_general']['blockwidth'] = array(
    '#type' => 'textfield',
    '#title' => t('Social block <strong>width</strong>'),
    '#default_value' => $jasm['jasm_general']['blockwidth'],
    '#size' => 3,
    '#maxlength' => 3,
    '#description' => t("The pixel width of the widget (facebook, twitter, etc.) for display. The minimum supported plugin width for Facebook like box is 292px."),
    '#required' => FALSE,
  );

  $form['jasm_general']['blockheight'] = array(
    '#type' => 'textfield',
    '#title' => t('Social block <strong>height</strong>'),
    '#default_value' => $jasm['jasm_general']['blockheight'],
    '#size' => 3,
    '#maxlength' => 3,
    '#description' => t("The pixel height of the widget (facebook, twitter, etc.) for display (ideal height, some scripts override this value)"),
    '#required' => FALSE,
  );

  $form['jasm_general']['inline'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable inline service list'),
    '#default_value' => $jasm['jasm_general']['inline'],
    '#description' => t('Display the JASM service list (both block or template variable) inline instead of an unordered list'),
    '#required' => FALSE,
  );

  $form['jasm_general']['fc_webicon'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable SVG icons'),
    '#default_value' => $jasm['jasm_general']['fc_webicon'],
    '#description' => t('Display the icons by <a href="http://fairheadcreative.com/" title="Fairhead Creative home page">Fairhead Creative\'s</a> scaleable vector graphics icons. <a href="http://zurb.com/playground/social-webicons" title="Download the icon set">Download the icon set</a> and place them in sites/all/libraries/webicons. Also requires <a href="http://drupal.org/project/modernizr" title="Modernizr on Drupal.org">Modernizr</a> to work correctly.'),
    '#required' => FALSE,
  );

  $form['jasm_general']['service_list_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Override default block title'),
    '#default_value' => $jasm['jasm_general']['service_list_title'],
    '#description' => t('By default, the service list block title is "Social". Include a custom title here.'),
    '#required' => FALSE,
  );

  $form['jasm_general']['pref_icon_display'] = array(
    '#type' => 'select',
    '#title' => t('Service list display preference'),
    '#default_value' => $jasm['jasm_general']['pref_icon_display'],
    '#description' => t('Display the default icon set with the service list.'),
    '#required' => FALSE,
    '#options' => array(
      'text_only' => t('Display text only'),
      'icon_text' => t('Display with icons and text'),
      'icon_only' => t('Display the icon only'),
    ),
  );

  $form['jasm_general']['service_list_preview'] = array(
    '#title' => t('Service List preview'),
    '#markup' => '<h4>Service List preview</h4>' . jasm_service_list(),
  );

  //FACEBOOK SETTINGS
  $form['facebook'] = array(
    '#type' => 'vertical_tabs',
  );

  $form['jasm_facebook'] = array(
    '#type' => 'fieldset',
    '#title' => t('Facebook'),
    '#weight' => -10,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'jasm',
  );

  $form['jasm_facebook']['name'] = array(
    '#type' => 'hidden',
    '#title' => t('Service name'),
    '#default_value' => $jasm['jasm_facebook']['name'],
    '#description' => t('The name of the 3rd party or service. This is used to generate css class name(s) and the titles of the links in the <em>JASM Service List</em>.'),
    '#required' => FALSE,
  );

  $form['jasm_facebook']['basepath'] = array(
    '#type' => 'hidden',
    '#title' => t('Base path'),
    '#default_value' => $jasm['jasm_facebook']['basepath'],
    '#description' => t('The base path or url for the service. Used to construct hyperlinks to the service.'),
    '#required' => FALSE,
  );

  $form['jasm_facebook']['inlist'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled in service list'),
    '#default_value' => $jasm['jasm_facebook']['inlist'],
    '#description' => t('Include a <em>Facebook</em> link in the JASM social list'),
    '#required' => FALSE,
  );

  $form['jasm_facebook']['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => $jasm['jasm_facebook']['weight'],
    '#delta' => 10,
    '#description' => t('Optional. Re-order the items in the JASM list by adjusting the weight.'),
  );

  $form['jasm_facebook']['href'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook <em>Page</em> path'),
    '#default_value' => $jasm['jasm_facebook']['href'],
    '#description' => t('Relative to the facebook domain, ie. Everything after "http://www.facebook.com/"'),
    '#required' => FALSE,
  );

  $form['jasm_facebook']['fbid'] = array(
    '#type' => 'textfield',
    '#title' => t('The Facebook Page ID'),
    '#default_value' => $jasm['jasm_facebook']['fbid'],
    '#description' => t("The Facebook Page ID."),
    '#required' => FALSE,
  );

  $form['jasm_facebook']['colorscheme'] = array(
    '#type' => 'select',
    '#title' => t('Color scheme'),
    '#default_value' => $jasm['jasm_facebook']['colorscheme'],
    '#description' => t('The preferred color scheme for your widget'),
    '#required' => FALSE,
    '#options' => array(
      'light' => t('Light'),
      'dark' => t('Dark'),
    ),
  );

  $form['jasm_facebook']['faces'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show faces'),
    '#default_value' => $jasm['jasm_facebook']['faces'],
    '#description' => t('Show/Hide faces of Facebook Page fans.'),
    '#required' => FALSE,
  );

  $form['jasm_facebook']['border_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Border color'),
    '#default_value' => $jasm['jasm_facebook']['border_color'],
    '#description' => t("A hex value for the border color of the Facebook box."),
    '#required' => FALSE,
  );

  $form['jasm_facebook']['stream'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show stream'),
    '#default_value' => $jasm['jasm_facebook']['stream'],
    '#description' => t('Show/Hide Stream of Facebook Page wall.'),
    '#required' => FALSE,
  );

  $form['jasm_facebook']['header'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show header'),
    '#default_value' => $jasm['jasm_facebook']['header'],
    '#description' => t('Show/Hide "Find us on Facebook".'),
    '#required' => FALSE,
  );

  $form['jasm_facebook']['likebox_preview'] = array(
    '#title' => t('Like box preview'),
    '#markup' => '<h4>Like box preview</h4>' . jasm_facebook_likebox(),
  );

  // FLICKR SETTINGS
  $form['jasm_flickr'] = array(
    '#type' => 'fieldset',
    '#title' => t('Flickr Account'),
    '#weight' => -9,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'jasm',
  );

  $form['jasm_flickr']['name'] = array(
    '#type' => 'hidden',
    '#title' => t('Service name'),
    '#default_value' => $jasm['jasm_flickr']['name'],
    '#description' => t('The name of the 3rd party or service. This is used to generate css class name(s) and the titles of the links in the <em>JASM Service List</em>.'),
    '#required' => FALSE,
  );

  $form['jasm_flickr']['basepath'] = array(
    '#type' => 'hidden',
    '#title' => t('Base path'),
    '#default_value' => $jasm['jasm_flickr']['basepath'],
    '#description' => t('The base path or url for the service. Used to construct hyperlinks to the service.'),
    '#required' => FALSE,
  );

  $form['jasm_flickr']['inlist'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled in service list'),
    '#default_value' => $jasm['jasm_flickr']['inlist'],
    '#description' => t('Include a <em>Flickr</em> link in the JASM social list'),
    '#required' => FALSE,
  );

  $form['jasm_flickr']['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => $jasm['jasm_flickr']['weight'],
    '#delta' => 10,
    '#description' => t('Optional. Re-order the items in the JASM list by adjusting the weight.'),
  );

  $form['jasm_flickr']['href'] = array(
    '#type' => 'textfield',
    '#title' => t('Flickr username'),
    '#default_value' => $jasm['jasm_flickr']['href'],
    '#description' => t(''),
    '#required' => FALSE,
  );

  // FOURSQUARE SETTINGS

  $form['jasm_4sqr'] = array(
    '#type' => 'fieldset',
    '#title' => t('Foursquare (venue)'),
    '#weight' => -8,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'jasm',
  );

  $form['jasm_4sqr']['name'] = array(
    '#type' => 'hidden',
    '#title' => t('Service name'),
    '#default_value' => $jasm['jasm_4sqr']['name'],
    '#description' => t('The name of the 3rd party or service. This is used to generate css class name(s) and the titles of the links in the <em>JASM Service List</em>.'),
    '#required' => FALSE,
  );

  $form['jasm_4sqr']['basepath'] = array(
    '#type' => 'hidden',
    '#title' => t('Base path'),
    '#default_value' => $jasm['jasm_4sqr']['basepath'],
    '#description' => t('The base path or url for the service. Used to construct hyperlinks to the service.'),
    '#required' => FALSE,
  );

  $form['jasm_4sqr']['inlist'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled in service list'),
    '#default_value' => $jasm['jasm_4sqr']['inlist'],
    '#description' => t('Include a <em>Foursquare</em> link in the JASM social list'),
    '#required' => FALSE,
  );

  $form['jasm_4sqr']['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => $jasm['jasm_4sqr']['weight'],
    '#delta' => 10,
    '#description' => t('Optional. Re-order the items in the JASM list by adjusting the weight.'),
  );

  $form['jasm_4sqr']['href'] = array(
    '#type' => 'textfield',
    '#title' => t('Foursquare venue id'),
    '#default_value' => $jasm['jasm_4sqr']['href'],
    '#description' => t('The unique id of the venue you manage on Foursquare. Usually the number following http://www.foursquare.com/venue/ when visiting a venue page on foursquare.'),
    '#required' => FALSE,
  );

  $form['jasm_4sqr']['placewidget_code'] = array(
    '#type' => 'textfield',
    '#title' => t('PlaceWidget location code'),
    '#default_value' => $jasm['jasm_4sqr']['placewidget_code'],
    '#description' => t('The code generated by http://www.placewidget.com. Get the valie from the widget code (between "k=" and "&s"). See help for details.'),
    '#required' => FALSE,
  );

  $form['jasm_4sqr']['interval'] = array(
    '#type' => 'select',
    '#title' => t('Update interval'),
    '#default_value' => $jasm['jasm_4sqr']['interval'],
    '#description' => t('PlaceWidget update interval. A longer interval will make the widget load faster, but data won\'t be as up to date.'),
    '#required' => FALSE,
    '#options' => array(
      1 => '1 minute',
      15 => '15 minutes',
      30 => '30 minutes',
      60 => '1 hour',
      360 => '6 hours',
      720 => '12 hours',
    ),
  );

  // INSTAGRAM SETTINGS

  $form['jasm_instagram'] = array(
    '#type' => 'fieldset',
    '#title' => t('Instagram'),
    '#weight' => -5,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'jasm',
  );

  $form['jasm_instagram']['name'] = array(
    '#type' => 'hidden',
    '#title' => t('Service name'),
    '#default_value' => $jasm['jasm_instagram']['name'],
    '#description' => t('The name of the 3rd party or service. This is used to generate css class name(s) and the titles of the links in the <em>JASM Service List</em>.'),
    '#required' => FALSE,
  );

  $form['jasm_instagram']['basepath'] = array(
    '#type' => 'hidden',
    '#title' => t('Base path'),
    '#default_value' => $jasm['jasm_instagram']['basepath'],
    '#description' => t('The base path or url for the service. Used to construct hyperlinks to the service.'),
    '#required' => FALSE,
  );

  $form['jasm_instagram']['inlist'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include in list'),
    '#default_value' => $jasm['jasm_instagram']['inlist'],
    '#description' => t('Include an <em>Instagram</em> link in the JASM social list'),
    '#required' => FALSE,
  );

  $form['jasm_instagram']['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => $jasm['jasm_instagram']['weight'], 
    '#delta' => 10, 
    '#description' => t('Optional. Re-order the items in the JASM list by adjusting the weight.'),
  );

  $form['jasm_instagram']['href'] = array(
    '#type' => 'textfield',
    '#title' => t('Instagram username'),
    '#default_value' => $jasm['jasm_instagram']['href'],
    '#description' => t(''),
    '#required' => FALSE,
  );

//  TODO: Ask for fields that may be useful and use the InstagramAPI to generate a widget
//  $form['jasm_instagram']['snapwidget_code'] = array(
//    '#type' => 'textfield',
//    '#title' => t('Snapwidget page id'),
//    '#default_value' => $jasm['jasm_instagram']['snapwidget_code'],
//    '#description' => t('The code generated by http://www.snapwidget.com. Get the value from the widget code (after "?u="). See help for details.'),
//    '#required' => FALSE,
//  );

  // RSS SETTINGS
  $form['jasm_rss'] = array(
    '#type' => 'fieldset',
    '#title' => t('Subscription settings (RSS)'),
    '#weight' => -7,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'jasm',
  );

  $form['jasm_rss']['name'] = array(
    '#type' => 'hidden',
    '#title' => t('Service name'),
    '#default_value' => $jasm['jasm_rss']['name'],
    '#description' => t('The name of the 3rd party or service. This is used to generate css class name(s) and the titles of the links in the <em>JASM Service List</em>.'),
    '#required' => FALSE,
  );

  $form['jasm_rss']['inlist'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled in service list'),
    '#default_value' => $jasm['jasm_rss']['inlist'],
    '#description' => t('Include an <em>RSS</em> link in the JASM social list'),
    '#required' => FALSE,
  );

  $form['jasm_rss']['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => $jasm['jasm_rss']['weight'],
    '#delta' => 10,
    '#description' => t('Optional. Re-order the items in the JASM list by adjusting the weight.'),
  );

  $form['jasm_rss']['basepath'] = array(
    '#type' => 'textfield',
    '#title' => t('Base path'),
    '#default_value' => $jasm['jasm_rss']['basepath'],
    '#description' => t('The base path or url for the service. Defaults to this website\'s base path. Do not include http://'),
    '#required' => FALSE,
  );

  $form['jasm_rss']['href'] = array(
    '#type' => 'textfield',
    '#title' => t('RSS url'),
    '#default_value' => $jasm['jasm_rss']['href'],
    '#description' => t('The path to your main feed relative to your base path. Defaults to rss.xml (ie. all nodes promoted to the front page). Do not include the opening /'),
    '#required' => FALSE,
  );

  // TWITTER SETTINGS
  $form['jasm_twitter'] = array(
    '#type' => 'fieldset',
    '#title' => t('Twitter'),
    '#weight' => -6,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'jasm',
  );

  $form['jasm_twitter']['name'] = array(
    '#type' => 'hidden',
    '#title' => t('Service name'),
    '#default_value' => $jasm['jasm_twitter']['name'],
    '#description' => t('The name of the 3rd party or service. This is used to generate css class name(s) and the titles of the links in the <em>JASM Service List</em>.'),
    '#required' => FALSE,
  );

  $form['jasm_twitter']['basepath'] = array(
    '#type' => 'hidden',
    '#title' => t('Base path'),
    '#default_value' => $jasm['jasm_twitter']['basepath'],
    '#description' => t('The base path or url for the service. Used to construct hyperlinks to the service.'),
    '#required' => FALSE,
  );

  $form['jasm_twitter']['inlist'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled in service list'),
    '#default_value' => $jasm['jasm_twitter']['inlist'],
    '#description' => t('Include a <em>Twitter</em> link in the JASM social list'),
    '#required' => FALSE,
  );

  $form['jasm_twitter']['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => $jasm['jasm_twitter']['weight'],
    '#delta' => 10,
    '#description' => t('Optional. Re-order the items in the JASM list by adjusting the weight.'),
  );

  $form['jasm_twitter']['href'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#default_value' => $jasm['jasm_twitter']['href'],
    '#description' => t('Your Twitter username'),
    '#required' => FALSE,
  );

  $form['jasm_twitter']['pref_pollresults'] = array(
    '#type' => 'checkbox',
    '#title' => t('Poll results'),
    '#default_value' => $jasm['jasm_twitter']['pref_pollresults'],
    '#description' => t('Poll Twitter results in widgets'),
    '#required' => FALSE,
  );

  $form['jasm_twitter']['pref_scrollbar'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include scroll bar'),
    '#default_value' => $jasm['jasm_twitter']['pref_scrollbar'],
    '#description' => t('Display a scroll bar within widget(s)'),
    '#required' => FALSE,
  );

  $form['jasm_twitter']['pref_timed'] = array(
    '#type' => 'select',
    '#title' => t('Behaviour'),
    '#default_value' => $jasm['jasm_twitter']['pref_timed'],
    '#description' => t('The preferred method to load and display tweets in a widget(s)'),
    '#required' => FALSE,
    '#options' => array(
      'all' => t('Load all Tweets'),
      'default' => t('Timed with an interval'),
    ),
  );

  $form['jasm_twitter']['pref_loop'] = array(
    '#type' => 'checkbox',
    '#title' => t('Loop Tweets'),
    '#default_value' => $jasm['jasm_twitter']['pref_loop'],
    '#description' => t('Whether or not Tweets should loop when displaying <em>Timed Tweets</em>. Has no effect when loading all tweets'),
    '#required' => FALSE,
  );

  $form['jasm_twitter']['pref_fluid_width'] = array(
    '#type' => 'checkbox',
    '#title' => t('Fluid width widget'),
    '#default_value' => $jasm['jasm_twitter']['pref_fluid_width'],
    '#description' => t('Override the general settings and render a fluid width widget.'),
    '#required' => FALSE,
  );

  $form['jasm_twitter']['pref_tweet_interval'] = array(
    '#type' => 'select',
    '#title' => t('Timed interval'),
    '#default_value' => $jasm['jasm_twitter']['pref_tweet_interval'],
    '#description' => t('The interval between displaying <em>Timed Tweets</em>.'),
    '#required' => FALSE,
    '#options' => array(
      2 => '2 seconds',
      3 => '3 seconds',
      4 => '4 seconds',
      5 => '5 seconds',
      6 => '6 seconds',
      7 => '7 seconds',
      8 => '8 seconds',
      9 => '9 seconds',
      10 => '10 seconds',
      11 => '11 seconds',
      12 => '12 seconds',
      13 => '13 seconds',
      14 => '14 seconds',
      15 => '15 seconds',
      16 => '16 seconds',
      17 => '17 seconds',
      18 => '18 seconds',
      19 => '19 seconds',
      20 => '20 seconds',
    ),
  );

  $form['jasm_twitter']['pref_tweet_number'] = array(
    '#type' => 'select',
    '#title' => t('Number of Tweets'),
    '#default_value' => $jasm['jasm_twitter']['pref_tweet_number'],
    '#description' => t('Number of Tweets to display in widget(s).'),
    '#required' => FALSE,
    '#options' => array(
      1 => '1 tweet',
      2 => '2 tweets',
      3 => '3 tweets',
      4 => '4 tweets',
      5 => '5 tweets',
      6 => '6 tweets',
      7 => '7 tweets',
      8 => '8 tweets',
      9 => '9 tweets',
      10 => '10 tweets',
      11 => '11 tweets',
      12 => '12 tweets',
      13 => '13 tweets',
      14 => '14 tweets',
      15 => '15 tweets',
      16 => '16 tweets',
      17 => '17 tweets',
      18 => '18 tweets',
      19 => '19 tweets',
      20 => '20 tweets',
    ),
  );

  $form['jasm_twitter']['pref_avatars'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display avatars'),
    '#default_value' => $jasm['jasm_twitter']['pref_avatars'],
    '#description' => t('Whether or not to display the author avatar next to tweets.'),
    '#required' => FALSE,
  );

  $form['jasm_twitter']['pref_timestamp'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display timestamp'),
    '#default_value' => $jasm['jasm_twitter']['pref_timestamp'],
    '#description' => t('Whether or not to display the time published.'),
    '#required' => FALSE,
  );

  $form['jasm_twitter']['pref_hashtags'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display hashtags'),
    '#default_value' => $jasm['jasm_twitter']['pref_hashtags'],
    '#description' => t('Whether or not to display hashtags within tweets.'),
    '#required' => FALSE,
  );

  $form['jasm_twitter']['color_shellback'] = array(
    '#type' => 'textfield',
    '#title' => t('Shell background color'),
    '#default_value' => $jasm['jasm_twitter']['color_shellback'],
    '#description' => t('The main background color.'),
    '#required' => FALSE,
  );

  $form['jasm_twitter']['color_shelltext'] = array(
    '#type' => 'textfield',
    '#title' => t('Shell text color'),
    '#default_value' => $jasm['jasm_twitter']['color_shelltext'],
    '#description' => t('The text color on the main background color.'),
    '#required' => FALSE,
  );

  $form['jasm_twitter']['color_tweetback'] = array(
    '#type' => 'textfield',
    '#title' => t('Tweet background color'),
    '#default_value' => $jasm['jasm_twitter']['color_tweetback'],
    '#description' => t('The background color inside the shell of the widget(s).'),
    '#required' => FALSE,
  );

  $form['jasm_twitter']['color_tweettext'] = array(
    '#type' => 'textfield',
    '#title' => t('Tweet color'),
    '#default_value' => $jasm['jasm_twitter']['color_tweettext'],
    '#description' => t('The text color of Tweet text.'),
    '#required' => FALSE,
  );

  $form['jasm_twitter']['color_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Link color'),
    '#default_value' => $jasm['jasm_twitter']['color_link'],
    '#description' => t('The text color of Tweet text.'),
    '#required' => FALSE,
  );

  // Twitter Follow Button Settings
  //  $form['jasm_twitter']['but_markup'] = array(
  //    '#value' => t('The following settings apply to the Twitter Follow button.'),
  //  );

  $form['jasm_twitter']['but_show_count'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display followers count'),
    '#default_value' => $jasm['jasm_twitter']['but_show_count'],
    '#description' => t('Whether or not to display the amount of followers on the Twitter Follow button.'),
    '#required' => FALSE,
  );

  $form['jasm_twitter']['but_color_butcolor'] = array(
    '#type' => 'select',
    '#title' => t('Follow Button Color'),
    '#default_value' => $jasm['jasm_twitter']['but_color_butcolor'],
    '#description' => t('The Twitter Follow button is available in two preset colors depending on your background color, ie. Blue or Grey'),
    '#required' => FALSE,
    '#options' => array(
      'blue' => 'Blue',
      'grey' => 'Grey',
    ),
  );

  $form['jasm_twitter']['but_color_textcolor'] = array(
    '#type' => 'textfield',
    '#title' => t('Text color'),
    '#default_value' => $jasm['jasm_twitter']['but_color_textcolor'],
    '#description' => t('If you have specified to show the amount of followers above, you can specify the text color\'s hex value to override the default blue or grey presets. Leave blank for default.'),
    '#required' => FALSE,
    '#field_prefix' => '#',
    '#size' => 6,
  );

  $form['jasm_twitter']['but_color_linkcolor'] = array(
    '#type' => 'textfield',
    '#title' => t('Text color'),
    '#default_value' => $jasm['jasm_twitter']['but_color_linkcolor'],
    '#description' => t('You can also override the link color of your username with a hex value. Leave blank for default.'),
    '#required' => FALSE,
    '#field_prefix' => '#',
    '#size' => 6,
  );

  $form['jasm_twitter']['but_lang'] = array(
    '#type' => 'select',
    '#title' => t('Follow Button language'),
    '#default_value' => $jasm['jasm_twitter']['but_color_butcolor'],
    '#description' => t('The Twitter Follow button has some available languages'),
    '#required' => FALSE,
    '#options' => array(
      'en' => 'English',
      'fr' => 'French',
      'de' => 'German',
      'it' => 'Italian',
      'es' => 'Spanish',
      'ko' => 'Korean',
      'ja' => 'Japanese',
    ),
  );

  // TODO: Create a JavaScript/iFrame version selector
  $form['jasm_twitter']['but_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Follow button width'),
    '#default_value' => $jasm['jasm_twitter']['but_width'],
    '#size' => 3,
    '#maxlength' => 3,
    '#description' => t("The preferred pixel width of the <em>Twitter Follow button</em> (if used). Leave blank for default (250px). (see https://dev.twitter.com/docs/follow-button)."),
    '#required' => FALSE,
  );

  $form['jasm_twitter']['but_text_align'] = array(
    '#type' => 'select',
    '#title' => t('Follow button alignment'),
    '#default_value' => $jasm['jasm_twitter']['but_text_align'],
    '#description' => t('Align the follow button text left or right.'),
    '#required' => FALSE,
    '#options' => array(
      'left' => t('Left'),
      'right' => t('Right'),
    ),
  );

  $form['jasm_twitter_faves'] = array(
    '#type' => 'fieldset',
    '#title' => t('Twitter Faves Widget settings'),
    '#weight' => -5,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'jasm',
  );

  $form['jasm_twitter_faves']['faves_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Faves widget title'),
    '#default_value' => $jasm['jasm_twitter_faves']['faves_title'],
    '#description' => t('The title to display in the <em>Twitter Faves Widget</em> block'),
    '#required' => FALSE,
  );

  $form['jasm_twitter_faves']['faves_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Faves widget subject'),
    '#default_value' => $jasm['jasm_twitter_faves']['faves_subject'],
    '#description' => t('The subject to display in the <em>Twitter Faves Widget</em> block'),
    '#required' => FALSE, 
  );

  $form['jasm_twitter_search'] = array(
    '#type' => 'fieldset',
    '#title' => t('Twitter Search Widget settings'),
    '#weight' => -5,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'jasm', 
  );

  $advancedlinkOptions = array(
    'attributes' => array('target' => '_blank')
  );
  $advancedlink = l(t('Try some advanced queries'), 'https://support.twitter.com/articles/71577', $advancedlinkOptions);

  $form['jasm_twitter_search']['search_query'] = array(
    '#type' => 'textfield',
    '#title' => t('Search query'),
    '#default_value' => $jasm['jasm_twitter_search']['search_query'],
    '#description' => t('The string or text to search for on Twitter. ' . $advancedlink),
    '#required' => FALSE,
  );

  $form['jasm_twitter_search']['search_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Search widget title'),
    '#default_value' => $jasm['jasm_twitter_search']['search_title'],
    '#description' => t('The title to display in the <em>Twitter Search Widget</em> block'),
    '#required' => FALSE,
  );

  $form['jasm_twitter_search']['search_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Search widget subject'),
    '#default_value' => $jasm['jasm_twitter_search']['search_subject'],
    '#description' => t('The subject to display in the <em>Twitter Search Widget</em> block'),
    '#required' => FALSE, 
  );

  // YOUTUBE SETTINGS
  $form['jasm_youtube'] = array(
    '#type' => 'fieldset',
    '#title' => t('YouTube channel'),
    '#weight' => -5,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'jasm',
  );

  $form['jasm_youtube']['name'] = array(
    '#type' => 'hidden',
    '#title' => t('Service name'),
    '#default_value' => $jasm['jasm_youtube']['name'],
    '#description' => t('The name of the 3rd party or service. This is used to generate css class name(s) and the titles of the links in the <em>JASM Service List</em>.'),
    '#required' => FALSE,
  );

  $form['jasm_youtube']['basepath'] = array(
    '#type' => 'hidden',
    '#title' => t('Base path'),
    '#default_value' => $jasm['jasm_youtube']['basepath'],
    '#description' => t('The base path or url for the service. Used to construct hyperlinks to the service.'),
    '#required' => FALSE,
  );

  $form['jasm_youtube']['inlist'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled in service list'),
    '#default_value' => $jasm['jasm_youtube']['inlist'],
    '#description' => t('Include a <em>YouTube</em> link in the JASM social list'),
    '#required' => FALSE,
  );

  $form['jasm_youtube']['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => $jasm['jasm_youtube']['weight'],
    '#delta' => 10,
    '#description' => t('Optional. Re-order the items in the JASM list by adjusting the weight.'),
  );

  $form['jasm_youtube']['href'] = array(
    '#type' => 'textfield',
    '#title' => t('YouTube channel name'),
    '#default_value' => $jasm['jasm_youtube']['href'],
    '#description' => t('Or your YouTube account name. Typically this would follow the YouTube domain on your channel page, e.g. http://www.youtube.com/samsungblog'),
    '#required' => FALSE,
  );

  // FLATTR SETTINGS 

  $form['jasm_flattr'] = array(
    '#type' => 'fieldset',
    '#title' => t('Flattr'),
    '#weight' => -4,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'jasm',
  );

  $form['jasm_flattr']['inlist'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled in service list'),
    '#default_value' => $jasm['jasm_flattr']['inlist'],
    '#description' => t('Include a <em>Flattr</em> link in the JASM social list'),
    '#required' => FALSE,
  );

  $form['jasm_flattr']['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => $jasm['jasm_flattr']['weight'],
    '#delta' => 10,
    '#description' => t('Optional. Re-order the items in the JASM list by adjusting the weight.'),
  );

  $form['jasm_flattr']['name'] = array(
    '#type' => 'hidden',
    '#title' => t('Service name'),
    '#default_value' => $jasm['jasm_flattr']['name'],
    '#description' => t('The name of the 3rd party or service. This is used to generate css class name(s) and the titles of the links in the <em>JASM Service List</em>.'),
    '#required' => FALSE,
  );

  $form['jasm_flattr']['basepath'] = array(
    '#type' => 'hidden',
    '#title' => t('Base path for constructing URL'),
    '#default_value' => $jasm['jasm_flattr']['basepath'],
    '#description' => t('The base path or url for the service. Used to construct hyperlinks to the service.'),
    '#required' => FALSE,
  );

  $form['jasm_flattr']['href'] = array(
    '#type' => 'textfield',
    '#title' => t('Your Flattr profile username'),
    '#default_value' => $jasm['jasm_flattr']['href'],
    '#description' => t('The username you use to log in to Flattr.com"'),
    '#required' => FALSE,
  );

  // INSTAGRAM SETTINGS

  $form['jasm_instagram'] = array(
    '#type' => 'fieldset',
    '#title' => t('Instagram'),
    '#weight' => -5,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'jasm',
  );

  $form['jasm_instagram']['name'] = array(
    '#type' => 'hidden',
    '#title' => t('Service name'),
    '#default_value' => $jasm['jasm_instagram']['name'],
    '#description' => t('The name of the 3rd party or service. This is used to generate css class name(s) and the titles of the links in the <em>JASM Service List</em>.'),
    '#required' => FALSE,
  );

  $form['jasm_instagram']['basepath'] = array(
    '#type' => 'hidden',
    '#title' => t('Base path'),
    '#default_value' => $jasm['jasm_instagram']['basepath'],
    '#description' => t('The base path or url for the service. Used to construct hyperlinks to the service.'),
    '#required' => FALSE,
  );

  $form['jasm_instagram']['inlist'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled in service list'),
    '#default_value' => $jasm['jasm_instagram']['inlist'],
    '#description' => t('Include an <em>Instagram</em> link in the JASM social list'),
    '#required' => FALSE,
  );

  $form['jasm_instagram']['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => $jasm['jasm_instagram']['weight'], 
    '#delta' => 10, 
    '#description' => t('Optional. Re-order the items in the JASM list by adjusting the weight.'),
  );

  $form['jasm_instagram']['href'] = array(
    '#type' => 'textfield',
    '#title' => t('Instagram username'),
    '#default_value' => $jasm['jasm_instagram']['href'],
    '#description' => t(''),
    '#required' => FALSE,
  );

//  TODO: Ask for fields that may be useful and use the InstagramAPI to generate a widget
//  $form['jasm_instagram']['snapwidget_code'] = array(
//    '#type' => 'textfield',
//    '#title' => t('Snapwidget page id'),
//    '#default_value' => $jasm['jasm_instagram']['snapwidget_code'],
//    '#description' => t('The code generated by http://www.snapwidget.com. Get the value from the widget code (after "?u="). See help for details.'),
//    '#required' => FALSE,
//  );

  // FEEDBURNER SETTINGS

  $form['jasm_feedburner'] = array(
    '#type' => 'fieldset',
    '#title' => t('Feedburner'),
    '#weight' => -7,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'jasm',
  );

  $form['jasm_feedburner']['inlist'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled in service list'),
    '#default_value' => $jasm['jasm_feedburner']['inlist'],
    '#description' => t('Include a <em>Feedburner</em> link in the JASM social list'),
    '#required' => FALSE,
  );

  $form['jasm_feedburner']['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => $jasm['jasm_feedburner']['weight'],
    '#delta' => 10,
    '#description' => t('Optional. Re-order the items in the JASM list by adjusting the weight.'),
  );

  $form['jasm_feedburner']['name'] = array(
    '#type' => 'hidden',
    '#title' => t('Service name'),
    '#default_value' => $jasm['jasm_feedburner']['name'],
    '#description' => t('The name of the 3rd party or service. This is used to generate css class name(s) and the titles of the links in the <em>JASM Service List</em>.'),
    '#required' => FALSE,
  );

  $form['jasm_feedburner']['basepath'] = array(
    '#type' => 'hidden',
    '#title' => t('Base path for constructing URL'),
    '#default_value' => $jasm['jasm_feedburner']['basepath'],
    '#description' => t('The base path or url for the service. Used to construct hyperlinks to the service.'),
    '#required' => FALSE,
  );

  $form['jasm_feedburner']['href'] = array(
    '#type' => 'textfield',
    '#title' => t('Feed address'),
    '#default_value' => $jasm['jasm_feedburner']['href'],
    '#description' => t('The feed address as you filled it in on Feedburner. Do not include http://feeds.feedburner.com/'),
    '#required' => FALSE,
  );

  $form['jasm_feedburner']['override_widget'] = array(
    '#type' => 'checkbox',
    '#title' => t('Override default widget'),
    '#default_value' => $jasm['jasm_feedburner']['override_widget'],
    '#description' => t('Feedburner provides widget code from the <em>Publicize</em> &raquo; <em>Email Subscriptions</em>. Enable this feature to strip the widget code of styles and branding.'),
    '#required' => FALSE,
  );

  $form['jasm_feedburner']['custom_block_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom subscription block title'),
    '#default_value' => $jasm['jasm_feedburner']['custom_block_title'],
    '#description' => t('The title the block should have if the default widget is overridden.'),
    '#required' => FALSE,
  );

  // LinkedIN
  $form['jasm_linkedin'] = array(
    '#type' => 'fieldset',
    '#title' => t('LinkedIN'),
    '#weight' => -4,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'jasm',
  );

  $form['jasm_linkedin']['inlist'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled in service list'),
    '#default_value' => $jasm['jasm_linkedin']['inlist'],
    '#description' => t('Include a <em>LinkedIN</em> link in the JASM social list'),
    '#required' => FALSE,
  );

  $form['jasm_linkedin']['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => $jasm['jasm_linkedin']['weight'],
    '#delta' => 10,
    '#description' => t('Optional. Re-order the items in the JASM list by adjusting the weight.'),
  );

  $form['jasm_linkedin']['name'] = array(
    '#type' => 'hidden',
    '#title' => t('Service name'),
    '#default_value' => $jasm['jasm_linkedin']['name'],
    '#description' => t('The name of the 3rd party or service. This is used to generate css class name(s) and the titles of the links in the <em>JASM Service List</em>.'),
    '#required' => FALSE,
  );

  $form['jasm_linkedin']['basepath'] = array(
    '#type' => 'hidden',
    '#title' => t('Base path for constructing URL'),
    '#default_value' => $jasm['jasm_linkedin']['basepath'],
    '#description' => t('The base path or url for the service. Used to construct hyperlinks to the service.'),
    '#required' => FALSE,
  );

  $form['jasm_linkedin']['href'] = array(
    '#type' => 'textfield',
    '#title' => t('Your LinkedIN company ID'),
    '#default_value' => $jasm['jasm_linkedin']['href'],
    '#description' => t('The unique ID your company page on LinkedIN has'),
    '#required' => FALSE,
  );

  // VKontakte
  $form['jasm_vkontakte'] = array(
    '#type' => 'fieldset',
    '#title' => t('VKontakte'),
    '#weight' => 0,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'jasm',
  );

  $form['jasm_vkontakte']['inlist'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled in service list'),
    '#default_value' => $jasm['jasm_vkontakte']['inlist'],
    '#description' => t('Include a <em>VKontakte</em> link in the JASM social list'),
    '#required' => FALSE,
  );

  $form['jasm_vkontakte']['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => $jasm['jasm_vkontakte']['weight'],
    '#delta' => 10,
    '#description' => t('Optional. Re-order the items in the JASM list by adjusting the weight.'),
  );

  $form['jasm_vkontakte']['name'] = array(
    '#type' => 'hidden',
    '#title' => t('Service name'),
    '#default_value' => $jasm['jasm_vkontakte']['name'],
    '#description' => t('The name of the 3rd party or service. This is used to generate css class name(s) and the titles of the links in the <em>JASM Service List</em>.'),
    '#required' => FALSE,
  );

  $form['jasm_vkontakte']['basepath'] = array(
    '#type' => 'hidden',
    '#title' => t('Base path for constructing URL'),
    '#default_value' => $jasm['jasm_vkontakte']['basepath'],
    '#description' => t('The base path or url for the service. Used to construct hyperlinks to the service.'),
    '#required' => FALSE,
  );

  $form['jasm_vkontakte']['href'] = array(
    '#type' => 'textfield',
    '#title' => t('Your VKontakte ID'),
    '#default_value' => $jasm['jasm_vkontakte']['href'],
    '#description' => t('The unique ID your profile page has on VKontakte'),
    '#required' => FALSE,
  );

  // Custom services (aka BYOS "Bring Your Own Service" or DIYS "Do It Yourself Service")

  $form['jasm_custom_01'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom service 01'),
    '#weight' => 8,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'jasm',
  );

  $form['jasm_custom_01']['inlist'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled in service list'),
    '#default_value' => $jasm['jasm_custom_01']['inlist'],
    '#description' => t('Include a <em>Custom</em> link in the JASM social list'),
    '#required' => FALSE,
  );

  $form['jasm_custom_01']['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => $jasm['jasm_custom_01']['weight'],
    '#delta' => 10,
    '#description' => t('Optional. Re-order the items in the JASM list by adjusting the weight.'),
  );

  $form['jasm_custom_01']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Service name'),
    '#default_value' => $jasm['jasm_custom_01']['name'],
    '#description' => t('The name of the 3rd party or service. This is used to generate css class name(s) and the titles of the links in the <em>JASM Service List</em>.'),
    '#required' => FALSE,
  );

  $form['jasm_custom_01']['basepath'] = array(
    '#type' => 'hidden',
    '#title' => t('Base path for constructing URL'),
    '#default_value' => $jasm['jasm_custom_01']['basepath'],
    '#description' => t('The value of this field is being overriden by the value of the href variable.'),
    '#required' => FALSE,
  );

  $form['jasm_custom_01']['href'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom service url'),
    '#default_value' => $jasm['jasm_custom_01']['href'],
    '#description' => t('The full url of the custom service. Do not include "http://"'),
    '#required' => FALSE,
  );

  $form['jasm_custom_02'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom service 02'),
    '#weight' => 10,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'jasm',
  );

  $form['jasm_custom_02']['inlist'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled in service list'),
    '#default_value' => $jasm['jasm_custom_02']['inlist'],
    '#description' => t('Include a <em>Custom</em> link in the JASM social list'),
    '#required' => FALSE,
  );

  $form['jasm_custom_02']['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => $jasm['jasm_custom_02']['weight'],
    '#delta' => 10,
    '#description' => t('Optional. Re-order the items in the JASM list by adjusting the weight.'),
  );

  $form['jasm_custom_02']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom service url'),
    '#default_value' => $jasm['jasm_custom_01']['href'],
    '#description' => t('The full url of the custom service. Do not include "http://"'),
    '#required' => FALSE,
  );

  $form['jasm_custom_02'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom service 02'),
    '#weight' => 9,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'jasm',
  );

  $form['jasm_custom_02']['inlist'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled in service list'),
    '#default_value' => $jasm['jasm_custom_02']['inlist'],
    '#description' => t('Include a <em>Custom</em> link in the JASM social list'),
    '#required' => FALSE,
  );

  $form['jasm_custom_02']['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => $jasm['jasm_custom_02']['weight'],
    '#delta' => 10,
    '#description' => t('Optional. Re-order the items in the JASM list by adjusting the weight.'),
  );

  $form['jasm_custom_02']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Service name'),
    '#default_value' => $jasm['jasm_custom_02']['name'],
    '#description' => t('The name of the 3rd party or service. This is used to generate css class name(s) and the titles of the links in the <em>JASM Service List</em>.'),
    '#required' => FALSE,
  );

  $form['jasm_custom_02']['basepath'] = array(
    '#type' => 'hidden',
    '#title' => t('Base path for constructing URL'),
    '#default_value' => $jasm['jasm_custom_02']['basepath'],
    '#description' => t('The value of this field is being overriden by the value of the href variable.'),
    '#required' => FALSE,
  );

  $form['jasm_custom_02']['href'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom service url'),
    '#default_value' => $jasm['jasm_custom_02']['href'],
    '#description' => t('The full url of the custom service. Do not include "http://"'),
    '#required' => FALSE,
  );

  $form['jasm_custom_03'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom service 03'),
    '#weight' => 10,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'jasm',
  );

  $form['jasm_custom_03']['inlist'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled in service list'),
    '#default_value' => $jasm['jasm_custom_03']['inlist'],
    '#description' => t('Include a <em>Custom</em> link in the JASM social list'),
    '#required' => FALSE,
  );

  $form['jasm_custom_03']['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => $jasm['jasm_custom_03']['weight'],
    '#delta' => 10,
    '#description' => t('Optional. Re-order the items in the JASM list by adjusting the weight.'),
  );

  $form['jasm_custom_03']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Service name'),
    '#default_value' => $jasm['jasm_custom_03']['name'],
    '#description' => t('The name of the 3rd party or service. This is used to generate css class name(s) and the titles of the links in the <em>JASM Service List</em>.'),
    '#required' => FALSE,
  );

  $form['jasm_custom_03']['basepath'] = array(
    '#type' => 'hidden',
    '#title' => t('Base path for constructing URL'),
    '#default_value' => $jasm['jasm_custom_03']['basepath'],
    '#description' => t('The value of this field is being overriden by the value of the href variable.'),
    '#required' => FALSE,
  );

  $form['jasm_custom_03']['href'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom service url'),
    '#default_value' => $jasm['jasm_custom_03']['href'],
    '#description' => t('The full url of the custom service. Do not include "http://"'),
    '#required' => FALSE,
  );

  return system_settings_form($form);
}