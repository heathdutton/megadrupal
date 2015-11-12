<?php
/**
 * @file
 * srrssb.api.php
 */


/**
 * Build the page with Form API.
 */
function page() {

  // . . . . .

  // Add RRSSB share buttons.
  $form['rrssb'] = array(
    '#markup' => srrssb(
      [
        'buttons' => ['googleplus', 'linkedin', 'facebook', 'twitter', 'email'],
        'url' => $url,
        'title' => $title,
        'summary' => $description,
        'hashtags' => $hashtags,
        'lng' => $lng,
      ]),
  );

  // . . . . .

}



/**
 * Returns an html list of the themed links with urls.
 *
 * @param array $options
 *   Associtive array of the options:
 *     - buttons
 *     - url
 *     - title
 *     - summary
 *     - hashtags
 *     - lng
 *     - pinterest_media
 *     - youtube_username
 *     - github_username
 *
 * @return string
 *   A string of markup for the list of buttons.
 */
function srrssb($options = array()) {

  // . . . . .

  return $html;
}



/**
 * Implements hook_srrssb_alter().
 * It is called by: drupal_alter('srrssb', $buttons, $options);
 */
function MODULE_srrssb_alter($buttons, $options) {

}
