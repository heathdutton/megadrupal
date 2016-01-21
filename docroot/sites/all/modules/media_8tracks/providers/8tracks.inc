<?php

/**
 * @file
 *  This is an 8tracks provider include file for Embedded Media Video.
 *  Use this as a base for creating new provider files.
 *
 *  When using this, first make the following global replacements:
 *    * Replace 8TRACKS with the name of your provider in all caps.
 *    * Replace 8tracks with the name of your provider in all lower case.
 *    * Replace 8Tracks with the name (to be translated) of your provider in
 *        uppercase.
 *
 *  You then need to go through each function and modify according to the
 *  requirements of your provider's API.
 */

/**
 *  This is the main URL for your provider.
 */
define('EMVIDEO_8TRACKS_MAIN_URL', 'http://www.8tracks.com/');

/**
 *  This is the URL to the API of your provider, if this exists.
 */
define('EMVIDEO_8TRACKS_API_URL', 'http://8tracks.com/developers/api');

/**
 *  This defines the version of the content data array that we serialize
 *  in emaudio_8tracks_data(). If we change the expected keys of that array,
 *  we must increment this value, which will allow older content to be updated
 *  to the new version automatically.
 */
define('EMVIDEO_8TRACKS_DATA_VERSION', 1);

/**
 * hook emaudio_PROVIDER_info
 * This returns information relevant to a specific 3rd party video provider.
 *
 * @return
 *   A keyed array of strings requested by various admin and other forms.
 *    'provider' => The machine name of the provider. This must be the same as
 *      the base name of this filename, before the .inc extension.
 *    'name' => The translated name of the provider.
 *    'url' => The url to the main page for the provider.
 *    'settings_description' => A description of the provider that will be
 *      posted in the admin settings form.
 *    'supported_features' => An array of rows describing the state of certain
 *      supported features by the provider. These will be rendered in a table,
 *      with the columns being 'Feature', 'Supported', 'Notes'. In general,
 *      the 'Feature' column will give the name of the feature, 'Supported'
 *      will be Yes or No, and 'Notes' will give an optional description or
 *      caveats to the feature.
 */
function emaudio_8tracks_info() {
  $features = array(
    array(t('Autoplay'), t('No'), ''),
    array(t('RSS Attachment'), t('No'), ''),
    array(t('Thumbnails'), t('No'), t('')),
  );
  return array(
    'provider' => '8tracks',
    'name' => t('8Tracks'),
    'url' => EMVIDEO_8TRACKS_MAIN_URL,
    'settings_description' => t('These settings specifically affect videos displayed from !8tracks. You can also read more about its !api.', array('!8tracks' => l(t('8Tracks.com'), EMVIDEO_8TRACKS_MAIN_URL), '!api' => l(t("developer's API"), EMVIDEO_8TRACKS_API_URL))),
    'supported_features' => $features,
  );
}

/**
 *  hook emaudio_PROVIDER_settings
 *  This should return a subform to be added to the emaudio_settings() admin
 *  settings page.
 *
 *  Note that a form field set will already be provided at $form['8tracks'],
 *  so if you want specific provider settings within that field set, you should
 *  add the elements to that form array element.
 */
function emaudio_8tracks_settings() {
  // We'll add a field set of player options here. You may add other options
  // to this element, or remove the field set entirely if there are no
  // user-configurable options allowed by the 8tracks provider.
  return $form;
}

/**
 *  hook emaudio_PROVIDER_extract
 *
 *  This is called to extract the video code from a pasted URL or embed code.
 *
 *  We'll be passed a URL or the embed code from a video when an editor pastes
 *  that in the field's textfield. We'll need to either pass back an array of
 *  regex expressions to match, or do the matching ourselves and return the
 *  resulting video code.
 *
 *  @param $parse
 *    An optional string with the pasted URL or embed code.
 *  @return
 *    Either an array of regex expressions to be tested, or a string with the
 *    video code to be used. If the hook tests the code itself, it should
 *    return either the string of the video code (if matched), or an empty
 *    array. Otherwise, the calling function will handle testing the embed code
 *    against each regex string in the returned array.
 */
function emaudio_8tracks_extract($parse = '') {
  // Here we assume that a URL will be passed in the form of
  // http://www.8tracks.com/video/text-video-title
  // or embed code in the form of <object value="http://www.8tracks.com/embed...".

  // We'll simply return an array of regular expressions for Embedded Media
  // Field to handle for us.
  return array(
    // First test for the embed code.
    '@8tracks\.com/(.+)/player_v2"@i',
    // In this expression, we're looking first for text matching the expression
    // between the @ signs. The 'i' at the end means we don't care about the
    // case. Thus, if someone enters http://www.8Tracks.com, it will still
    // match. We escape periods as \., as otherwise they match any character.
    // The text in parentheses () will be returned as the provider video code,
    // if there's a match for the entire expression. In this particular case,
    // ([^?]+) means to match one more more characters (+) that are not a
    // question mark ([^\?]), which would denote a query in the URL.
    '@8tracks\.com/([^\?]+)@i',
  );
}

/**
 *  hook emaudio_PROVIDER_data
 *
 *  Provides an array to be serialised and made available with $item elsewhere.
 *
 *  This data can be used to store any extraneous information available
 *  specifically to the 8tracks provider.
 */
function emaudio_8tracks_data($field, $item) {
  // Initialize the data array.
  $data = array();

  // We are using oEmbed to retrieve a standard set of data from the provider.
  // You should change the URL as specified by the 8tracks provider.
  // If the 8tracks provider does not support oEmbed, you must remove this
  // section entirely, or rewrite it to use their API.
  // See http://oembed.com/ for for information.
  $data = emfield_request_xml('8tracks', 'http://8tracks.com/'. $item['value'] .'.xml', array(), TRUE, FALSE, $item['value']);

  $data['emaudio_8tracks'] = emfield_request_xml('8tracks', 'http://8tracks.com/sets/460486803/play.xml?mix_id='. $data['MIX']['ID'][0], array(), TRUE, FALSE, $item['value'] .':'. $data['MIX']['ID'][0]);

  // Create some version control. Thus if we make changes to the data array
  // down the road, we can respect older content. If allowed by Embedded Media
  // Field, any older content will automatically update this array as needed.
  // In any case, you should account for the version if you increment it.
  $data['emaudio_8tracks_version'] = EMVIDEO_8TRACKS_DATA_VERSION;

  $data['emaudio_8tracks_thumbnail'] = $data['emaudio_8tracks']['TRACK']['PHOTO']['0'];

  // This stores a URL to the video's thumbnail.

  return $data;
}

/**
 *  hook emaudio_PROVIDER_rss
 *
 *  This attaches a file to an RSS feed.
 */
function emaudio_8tracks_rss($item, $teaser = NULL) {
}

/**
 * hook emaudio_PROVIDER_embedded_link($video_code)
 * returns a link to view the video at the provider's site.
 *  @param $code
 *    The string containing the audio code.
 *  @return
 *    A string containing the URL to view the video at the original provider's site.
 */
function emaudio_8tracks_embedded_link($code) {
  return 'http://www.8tracks.com/'. $code;
}

/**
 * The embedded flash displaying the 8tracks video.
 */
function theme_emaudio_8tracks_flash($item, $width, $height, $autoplay) {
  $output = '';
  if ($item['embed']) {
    $output = <<<EMBED
<object classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000" codebase="http://download.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=9,0,28,0" width="$width" height="$height" ><param name="movie" value="http://8tracks.com/{$item['value']}/player_v2"><param name="allowscriptaccess" value="always"><param name="FlashVars" value="bg_color=_000000"><embed FlashVars="bg_color=_000000" src="http://8tracks.com/{$item['value']}/player_v2" pluginspage="http://www.adobe.com/shockwave/download/download.cgi?P1_Prod_Version=ShockwaveFlash" type="application/x-shockwave-flash" width="$width" height="$height" allowscriptaccess="always" ></embed></object>
EMBED;
  }
  return $output;
}

/**
 * hook emaudio_PROVIDER_thumbnail
 * Returns the external url for a thumbnail of a specific video.
 *  @param $field
 *    The field of the requesting node.
 *  @param $item
 *    The actual content of the field from the requesting node.
 *  @return
 *    A URL pointing to the thumbnail.
 */
function emaudio_8tracks_thumbnail($field, $item, $formatter, $node, $width, $height) {
  // In this demonstration, we previously retrieved a thumbnail using oEmbed
  // during the data hook.
  return $data['emaudio_8tracks']['TRACK']['PHOTO']['0'];
}

/**
 *  hook emaudio_PROVIDER_video
 *  This actually displays the full/normal-sized video we want, usually on the
 *  default page view.
 *  @param $embed
 *    The video code for the video to embed.
 *  @param $width
 *    The width to display the video.
 *  @param $height
 *    The height to display the video.
 *  @param $field
 *    The field info from the requesting node.
 *  @param $item
 *    The actual content from the field.
 *  @return
 *    The html of the embedded video.
 */
function emaudio_8tracks_audio($embed, $width, $height, $field, $item, &$node, $autoplay) {
  $output = theme('emaudio_8tracks_flash', $item, $width, $height, $autoplay);
  return $output;
}

/**
 *  hook emaudio_PROVIDER_video
 *
 *  This actually displays the preview-sized video we want, commonly for the
 *  teaser.
 *  @param $embed
 *    The video code for the video to embed.
 *  @param $width
 *    The width to display the video.
 *  @param $height
 *    The height to display the video.
 *  @param $field
 *    The field info from the requesting node.
 *  @param $item
 *    The actual content from the field.
 *  @return
 *    The html of the embedded video.
 */
function emaudio_8tracks_preview($embed, $width, $height, $field, $item, &$node, $autoplay) {
  $output = theme('emaudio_8tracks_flash', $item, $width, $height, $autoplay);
  return $output;
}

/**
 *  Implementation of hook_emfield_subtheme.
 *  This returns any theme functions defined by this provider.
 */
function emaudio_8tracks_emfield_subtheme() {
    $themes = array(
        'emaudio_8tracks_flash'  => array(
            'arguments' => array('item' => NULL, 'width' => NULL, 'height' => NULL, 'autoplay' => NULL),
            'file' => 'providers/8tracks.inc',
            // If you don't provide a 'path' value, then it will default to
            // the emaudio.module path. Obviously, replace 'em8tracks' with
            // the actual name of your custom module.
            'path' => drupal_get_path('module', 'media_8tracks'),
        )
    );
    return $themes;
}
