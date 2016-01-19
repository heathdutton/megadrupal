
This is a highly flexible and easy extendable filter module to embed any type
of audio in your site using a simple tag. Other modules can add audio
sites/formats (called codecs) using an easy plug-in architecture.

========= Installation =========

Enable the module on the modules page.

Go to admin/config/content/formats and configure the text format(s) that should
be allowed to use this filter. Check the box to enable Audio Filter and save.
Some simple settings are available if you configure the text format. There you
can change the default size and auto play settings.

Make sure that Audio Filter is processed before "Convert URLs to links".
You can do this by dragging and dropping Audio Filter to the top of the
processing order list. Do this even if it's allready on top, just to make sure!

If you're using the "Limit allowed HTML tags" filter, make sure Audio Filter is
processed after that filter.

To enable WYSIWYG support, go to the WYSIWYG settings for each input format and
enable the Audio Filter button.

========= Usage =========

Single audio: [audio:url]
This will output the audio using the default settings.

Random audio from multiple URL's: [audio:url,url]
This will output one of the specified audios each time.

You can also set some parameters in the call:

[audio:url width:X height:Y align:left/right autoplay:1/0]
This will override the default settings for this audio.

========= Developers =========

This module calls hook_codec_info(), so you can add your own codecs.

Example:

function MODULE_codec_info() {
  $codecs = array();
  // You can offer multiple audio formats in one module.
  $codecs['youtube'] = array(
    // Will be used some day in user information.
    'name' => t('YouTube'),

    // Special instructions for end users. Optional.
    'instructions' => t('Any special instructions that users need to know about
    to get this codec working correctly.'),

    // The callback that will output the right embed code.
    'callback' => 'MODULE_youtube',

    // HTML5 callback, for returning something that's device agnostic.
    // @SEE audio_filter_youtube_html5.
    'html5_callback' => 'MODULE_service_html5',

    // Regexp can be an array. $audio['codec']['delta'] will be set to the key.
    'regexp' => '/youtube\.com\/watch\?v=([a-z0-9]+)/i',

    // Ratio for resizing within user-given width and height (ratio = width / height)
    'ratio' => 425 / 355,
  );
  return $codecs;
}

And this will be your callback function:

function MODULE_youtube($audio) {
  // $audio contains the audio URL in source, the codec (as above) and also
  // [code][matches] with the result of the regexp and [codec][delta] with the
  // key of the matched regexp.
  $audio['source'] = 'http://www.youtube.com/v/' . $audio['codec']['matches'][1] . ($audio['autoplay'] ? '&autoplay=1' : '');

  // Outputs a general <object...> for embedding flash players. Needs width,
  // height, source and optionally align (left or right) and params (a list of
  // <param...> attributes)
  return audio_filter_flash($audio);
}

========= Troubleshooting =========

If audios don't show up, try disabling any browser plugins, such as AdBlock.

========= Credits =========

Audio filter icon from http://www.famfamfam.com/lab/icons/silk/
Entire module is a rip-off of http://drupal.org/project/video_filter
