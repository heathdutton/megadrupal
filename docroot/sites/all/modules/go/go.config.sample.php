<?php
/**
 * @file go.config.sample.php
 *
 * Sample configuration for features of go.module
 */

// #####################
// Disable autoload feature
// #####################
define('GO_DISABLE_AUTOLOAD', TRUE);

// #####################
// Simple Google Analytics
// #####################
define('GO_GOOGLE_ANALYTICS', 'UA-12345');

// #####################
// Simple slideshow
//
// More example at ./docs/slideshow.txt
// ####################
$conf['go_slideshow__views__frontpage_slider__page'] = array(
  'slideExpr' => '.views-row',
  'pager' => 'after',
  'pagerExpr' => '.views-field-field-image',
  'activePagerClass' => 'pager-current',
);

// #####################
// Simple 403/404
//
//  403 => Redirect to Login page
//  404 => Redirect to Search page
// #####################
define('GO_403', 1);
define('GO_404', 'search/content');

// #####################
// Keep /node page, do not redirect to front page
// #####################
define('GO_SKIP_NODE_TO_FRONT', 1);

// #####################
// Disable mail sending
//
// @see drupal_mail()
// @see drupal_mail_system()
// #####################
// Disable all of mail sending…
$conf['mail_system']['default-system'] = 'Go\MailSystem\Disable';

// To forward email sending
$conf['go_mail_fwd_to']['default-system'] = 'godebug@mailinator.com';
$conf['mail_system']['default-system'] = 'Go\MailSystem\Foward';

// To forward email to Hipchat room
$conf['go_mail_fwd_to_hipchat']['default-system'] = 'RoomName';
$conf['mail_system']['default-system'] = 'Go\MailSystem\Foward';

// Configure for specific mail sending of your Drupal system
// Replace 'default-system' with $module . '_' . $key
// With $module and $key are same to drupal_mail($module, $key, …)
$conf['go_mail_fwd_to_hipchat'][$module . '_' . $key] = 'RoomName';
$conf['mail_system'][$module . '_' . $key] = 'Go\MailSystem\Foward';

// #####################
// Select default format for formatted text fields.
//
// 0, 1, 2 is ID of user role.
// #####################

// Defaut for all fields
$conf['go_text_formats']['roles'] = array(
  0 => 'plain_text',
  1 => 'filtered_html',
  2 => 'full_html',
);

// Get prefered text-format for user on specific field.
$conf['go_text_formats']['node']['article']['body'] = array(
  0 => 'plain_text',
  1 => 'filtered_html',
  2 => 'full_html',
);
