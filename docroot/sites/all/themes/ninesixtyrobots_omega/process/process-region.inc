<?php
/**
 * Processing for the region templates.
 */
function ninesixtyrobots_omega_alpha_process_region(&$vars) {
 $theme = alpha_get_theme();
 
 // Only create variables for the regions which need them.
 switch ($vars['elements']['#region']) {
   case 'content':
   // Content region variables (region--content.tpl.php)
   if ($blocks = block_get_blocks_by_region('help')) {
     $vars['help'] = $blocks;
   }

   case 'footer_bottom':
   // Footer bottom region variables (region--footer-bottom.tpl.php)
   $vars['secondary_menu'] = $theme->page['secondary_menu'];

   case 'branding':
   // Header region variables (region--header.tpl.php)
   $vars['main_menu'] = $theme->page['main_menu'];
   $vars['front_page'] = $theme->page['front_page'];

   // Check if the theme is using Twitter.
   $use_twitter = theme_get_setting('use_twitter');

   // If the theme uses Twitter pull it in and display it in the slogan.
   if ($use_twitter) {
     if ($cache = cache_get('ninesixtyrobots_omega_tweets')) {
       $data = $cache->data;
     }
     else {
       $query = theme_get_setting('twitter_search_term');
       $query = drupal_encode_path($query);

       $response = drupal_http_request('http://search.twitter.com/search.json?q=' . $query);
       if ($response->code == 200) {
         $data = json_decode($response->data);
         // Set a 5 minute cache on retrieving tweets.
         // Note if this isn't updating on your site *run cron*.
         cache_set('ninesixtyrobots_omega_tweets', $data, 'cache', 300);
       }
     }
     $tweet = $data->results[array_rand($data->results)];
     // Create the actual variable finally.
     $vars['site_slogan'] = check_plain(html_entity_decode($tweet->text));
   }
   break;
 }
}
