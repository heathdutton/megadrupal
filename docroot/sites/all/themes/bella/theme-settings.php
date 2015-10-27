<?php
/**
 * @file
 * Theme setting callbacks for the Bella theme.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 *
 * @param $form
 *   The form.
 * @param $form_state
 *   The form state.
 */
function bella_form_system_theme_settings_alter(&$form, &$form_state)  {
  $theme_id_shortcut = 'bella';
  $theme_override_footer_message_function = $theme_id_shortcut . '_override_footer_message';
  $theme_custom_include_file = $theme_id_shortcut . '.custom.inc';
  include_once(drupal_get_path('theme', $theme_id_shortcut) . '/template.php');
  $form['styles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom Settings'),
    '#description' => t('Use these settings to change what and how information is displayed in your theme.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  // Full pages
  $form['styles']['full_pages'] = array(
    '#type' => 'fieldset',
    '#title' => t('Full pages'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['styles']['full_pages']['full_pages_hide_titles'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide titles on full pages'),
    '#default_value' => theme_get_setting('full_pages_hide_titles'),
    '#description' => t('If selected, normal page titles will never appear. Does not apply to the titles of teasers within a full page.'),
  );
  // Social links
  $form['styles']['social_links'] = array(
    '#type' => 'fieldset',
    '#title' => t('Social links'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['styles']['social_links']['social_links_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display social links at the top of the page?'),
    '#default_value' => theme_get_setting('social_links_display'),
  );
  $form['styles']['social_links']['social_links_display_links'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display these social links'),
    '#description' => t('If %display is checked, choose which links you wish to be displayed.', array('%display' => t('Display social links at the top of the page?'))),
  );
  $form['styles']['social_links']['social_links_display_links']['social_links_display_links_facebook'] = array(
    '#type' => 'checkbox',
    '#title' => t('Facebook'),
    '#default_value' => theme_get_setting('social_links_display_links_facebook'),
  );
  $form['styles']['social_links']['social_links_display_links']['social_links_display_links_facebook_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Link to Facebook page'),
    '#description' => t('Enter the link to your Facebook page.'),
    '#size' => 60,
    '#default_value' => theme_get_setting('social_links_display_links_facebook_link'),
  );
  $form['styles']['social_links']['social_links_display_links']['social_links_display_links_googleplus'] = array(
    '#type' => 'checkbox',
    '#title' => t('Google Plus'),
    '#default_value' => theme_get_setting('social_links_display_links_googleplus'),
  );
  $form['styles']['social_links']['social_links_display_links']['social_links_display_links_googleplus_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Link to Google Plus page'),
    '#description' => t('Enter the link to your Google Plus account.'),
    '#size' => 60,
    '#default_value' => theme_get_setting('social_links_display_links_googleplus_link'),
  );
  $form['styles']['social_links']['social_links_display_links']['social_links_display_links_twitter'] = array(
    '#type' => 'checkbox',
    '#title' => t('Twitter'),
    '#default_value' => theme_get_setting('social_links_display_links_twitter'),
  );
  $form['styles']['social_links']['social_links_display_links']['social_links_display_links_twitter_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Link to Twitter page'),
    '#description' => t('Enter the link to your Twitter account.'),
    '#size' => 60,
    '#default_value' => theme_get_setting('social_links_display_links_twitter_link'),
  );
  $form['styles']['social_links']['social_links_display_links']['social_links_display_links_instagram'] = array(
    '#type' => 'checkbox',
    '#title' => t('Instagram'),
    '#default_value' => theme_get_setting('social_links_display_links_instagram'),
  );
  $form['styles']['social_links']['social_links_display_links']['social_links_display_links_instagram_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Link to Instagram page'),
    '#description' => t('Enter the link to your Instagram account.'),
    '#size' => 60,
    '#default_value' => theme_get_setting('social_links_display_links_instagram_link'),
  );
  $form['styles']['social_links']['social_links_display_links']['social_links_display_links_pinterest'] = array(
    '#type' => 'checkbox',
    '#title' => t('Pinterest'),
    '#default_value' => theme_get_setting('social_links_display_links_pinterest'),
  );
  $form['styles']['social_links']['social_links_display_links']['social_links_display_links_pinterest_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Link to Pinterest page'),
    '#description' => t('Enter the link to your Pinterest account.'),
    '#size' => 60,
    '#default_value' => theme_get_setting('social_links_display_links_pinterest_link'),
  );
  $form['styles']['social_links']['social_links_display_links']['social_links_display_links_linkedin'] = array(
    '#type' => 'checkbox',
    '#title' => t('LinkedIn'),
    '#default_value' => theme_get_setting('social_links_display_links_linkedin'),
  );
  $form['styles']['social_links']['social_links_display_links']['social_links_display_links_linkedin_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Link to LinkedIn page'),
    '#description' => t('Enter the link to your LinkedIn account.'),
    '#size' => 60,
    '#default_value' => theme_get_setting('social_links_display_links_linkedin_link'),
  );
  $form['styles']['social_links']['social_links_display_links']['social_links_display_links_youtube'] = array(
    '#type' => 'checkbox',
    '#title' => t('Youtube'),
    '#default_value' => theme_get_setting('social_links_display_links_youtube'),
  );
  $form['styles']['social_links']['social_links_display_links']['social_links_display_links_youtube_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Link to Youtube page'),
    '#description' => t('Enter the link to your Youtube account.'),
    '#size' => 60,
    '#default_value' => theme_get_setting('social_links_display_links_youtube_link'),
  );
  $form['styles']['social_links']['social_links_display_links']['social_links_display_links_vimeo'] = array(
    '#type' => 'checkbox',
    '#title' => t('Vimeo'),
    '#default_value' => theme_get_setting('social_links_display_links_vimeo'),
  );
  $form['styles']['social_links']['social_links_display_links']['social_links_display_links_vimeo_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Link to Vimeo page'),
    '#description' => t('Enter the link to your Vimeo account.'),
    '#size' => 60,
    '#default_value' => theme_get_setting('social_links_display_links_vimeo_link'),
  );
  $form['styles']['social_links']['social_links_display_links']['social_links_display_links_flickr'] = array(
    '#type' => 'checkbox',
    '#title' => t('Flickr'),
    '#default_value' => theme_get_setting('social_links_display_links_flickr'),
  );
  $form['styles']['social_links']['social_links_display_links']['social_links_display_links_flickr_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Link to Flickr page'),
    '#description' => t('Enter the link to your Flickr account.'),
    '#size' => 60,
    '#default_value' => theme_get_setting('social_links_display_links_flickr_link'),
  );
  $form['styles']['social_links']['social_links_display_links']['social_links_display_links_tumblr'] = array(
    '#type' => 'checkbox',
    '#title' => t('Tumblr'),
    '#default_value' => theme_get_setting('social_links_display_links_tumblr'),
  );
  $form['styles']['social_links']['social_links_display_links']['social_links_display_links_tumblr_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Link to Tumblr page'),
    '#description' => t('Enter the link to your Tumblr account.'),
    '#size' => 60,
    '#default_value' => theme_get_setting('social_links_display_links_tumblr_link'),
  );
  $form['styles']['social_links']['social_links_display_links']['social_links_display_links_skype'] = array(
    '#type' => 'checkbox',
    '#title' => t('Skype'),
    '#default_value' => theme_get_setting('social_links_display_links_skype'),
  );
  $form['styles']['social_links']['social_links_display_links']['social_links_display_links_skype_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Link to Skype page'),
    '#description' => t('Enter the contact link to your Skype account. Typical format: <b>skype:myusername?call</b> where <i>myusername</i> is your Skype user name.'),
    '#size' => 60,
    '#default_value' => theme_get_setting('social_links_display_links_skype_link'),
  );
  $form['styles']['social_links']['social_links_display_links']['social_links_display_links_rss'] = array(
    '#type' => 'checkbox',
    '#title' => t('RSS'),
    '#default_value' => theme_get_setting('social_links_display_links_rss'),
  );
  $form['styles']['social_links']['social_links_display_links']['social_links_display_links_rss_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Link to RSS page'),
    '#description' => t('Enter the URL for the RSS feed you would like the RSS link to lead to. Does not necessarily have to originate from this site.'),
    '#size' => 60,
    '#default_value' => theme_get_setting('social_links_display_links_rss_link'),
  );
  // Breadcrumb
  $form['styles']['breadcrumb'] = array(
    '#type' => 'fieldset',
    '#title' => t('Breadcrumb'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['styles']['breadcrumb']['breadcrumb_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display breadcrumb'),
    '#default_value' => theme_get_setting('breadcrumb_display'),
  );
  $form['styles']['breadcrumb']['breadcrumb_separator'] = array(
    '#type' => 'textfield',
    '#title' => t('Breadcrumb separator'),
    '#description' => t('Customize the separator character used in the breadcrumb trail. Remember to surround the separator code with a space on each side. Allowed HTML tags: &lt;div&gt; &lt;span&gt;<br/>Examples: <b> &amp;#8674; </b> or <b>&amp;nbsp;&amp;hearts;&amp;nbsp;</b> or <b>&lt;span class="custom-breadcrumb-divider"&gt;&lt;/span&gt;</b>'),
    '#size' => 60,
    '#default_value' => theme_get_setting('breadcrumb_separator'),
  );
  // Menus
  $form['styles']['menus'] = array(
    '#type' => 'fieldset',
    '#title' => t('Menus'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['styles']['menus']['menus_show_expanded_main_menu'] = array(
    '#type' => 'checkbox',
    '#title' => t('Expand main menu'),
    '#description' => t('If selected, the main menu will appear as an expanded dropdown menu.'),
    '#default_value' => theme_get_setting('menus_show_expanded_main_menu'),
  );
  // Front
  $form['styles']['frontpage'] = array(
    '#type' => 'fieldset',
    '#title' => t('Front page'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['styles']['frontpage']['frontpage_show_title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show the title on the front page'),
    '#description' => t('If not selected, the main title on the front page will not appear.'),
    '#default_value' => theme_get_setting('frontpage_show_title'),
  );
  $form['styles']['frontpage']['frontpage_hide_no_content_message'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide <i>No front page content has been created yet</i>'),
    '#description' => t('If selected, the message on the front page that normally displays <i>No front page content has been created yet</i> will not appear.'),
    '#default_value' => theme_get_setting('frontpage_hide_no_content_message'),
  );
  // Teasers
  $form['styles']['teasers'] = array(
    '#type' => 'fieldset',
    '#title' => t('Teasers'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['styles']['teasers']['frontpage_teasers_separators'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display a separator between teasers on the front page'),
    '#description' => t('If selected, a separator will appear in between the teasers on the front page.'),
    '#default_value' => theme_get_setting('frontpage_teasers_separators'),
  );
  $form['styles']['teasers']['sitewide_teasers_separators'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display a separator between teasers throughout the entire site'),
    '#description' => t('If selected, a separator will appear in between the teasers throughout the entire site.'),
    '#default_value' => theme_get_setting('sitewide_teasers_separators'),
  );
  $form['styles']['teasers']['teasers_userpics'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display user pictures inside teasers'),
    '#description' => t('If <em>User pictures in posts</em> is selected above, should the user pictures appear on the teasers as well as on full pages?'),
    '#default_value' => theme_get_setting('teasers_userpics'),
  );
  // Code customizations: stylesheets and .inc files
  $form['styles']['custom_files'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom files'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['styles']['custom_files']['custom_stylesheets'] = array(
    '#type' => 'checkbox',
    '#title' => t('Scan for custom stylesheets'),
    '#description' => t('If this box is checked, your site will scan the <b>@themedir</b> directory for customized stylesheets.', array('@themedir' => drupal_get_path('theme', $theme_id_shortcut))),
    '#default_value' => theme_get_setting('custom_stylesheets'),
  );
  $form['styles']['custom_files']['custom_includes'] = array(
    '#type' => 'checkbox',
    '#title' => t('Scan for custom .inc (PHP) files'),
    '#description' => t('If this box is checked, your site will scan the <b>@themedir</b> directory for a file called %incfile. If found, the file will be included it as if it were a second template.php file. Allows custom functions to be defined without having to hard-code them into the template.php file.', array('@themedir' => drupal_get_path('theme', $theme_id_shortcut), '%incfile' => $theme_custom_include_file)),
    '#default_value' => theme_get_setting('custom_includes'),
  );
  // Footer
  $form['styles']['footer_message'] = array(
    '#type' => 'fieldset',
    '#title' => t('Footer message'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['styles']['footer_message']['footer_message_auto_message'] = array(
    '#type' => 'checkbox',
    '#title' => t('Automatic footer message'),
    '#description' => t('If this box is checked, this site\'s footer message will be automatically prepared and the following will be displayed instead:') . ' ' . $theme_override_footer_message_function() . '<br/>' . t('Useful if you would like the copyright date to automatically update itself.'),
    '#default_value' => theme_get_setting('footer_message_auto_message'),
  );
  // Feed icons
  $form['styles']['feed_icons'] = array(
    '#type' => 'fieldset',
    '#title' => t('Feed icons'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['styles']['feed_icons']['feed_icons_display_icons'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display feed icons'),
    '#description' => t('If this box is not checked, RSS feed icons that would normally appear toward the bottom of the page will be hidden, but the feed itself will still be available.'),
    '#default_value' => theme_get_setting('feed_icons_display_icons'),
  );
  // Search box
  $form['styles']['search_box'] = array(
    '#type' => 'fieldset',
    '#title' => t('Search box'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['styles']['search_box']['search_box_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display search box'),
    '#description' => t('If this box is checked, the search box will be displayed in the upper-right corner of the page to anyone who has permission to search.'),
    '#default_value' => theme_get_setting('search_box_display'),
  );
}
