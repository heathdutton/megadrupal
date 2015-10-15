<?php

/**
 * @file
 * Template for the checkout steps widget.
 *
 * Variables:
 * ----------
 * @var $ad
 *   The ad object.
 * @var $account
 *   The user object.
 * @var $url
 *   The link url.
 * @var $styles
 *   An array of CSS styles to apply.
 * @var $link_styles
 *   An array of CSS styles for the link div.
 * @var $flash_path
 *   The path to the flash file.
 * @var $bottom_text
 *   The text to show on the bottom of the ad.
 * @var $class
 *   The class of the add.
 */

$style_string = '';
foreach ($styles as $key => $value) {
  $style_string .= $key . ':' . $value . ';';
}

$link_style_string = '';
foreach ($link_styles as $key => $value) {
  $link_style_string .= $key . ':' . $value . ';';
}

?>

<div class="ms_ad_container <?php print $class; ?>"
     style='<?php print $style_string; ?>'>
  <div class='ms_ad_file_container'>
    <object classid='clsid:d27cdb6e-ae6d-11cf-96b8-444553540000'
            codebase='http://download.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=10,0,0,0'
            id='ms_ad_<?php print $ad->id; ?>' align='middle'>
      <param name='allowScriptAccess' value='sameDomain'/>
      <param name='allowFullScreen' value='false'/>
      <param name='movie' value='<?php print $flash_path; ?>'/>
      <param name='quality' value='high'/>
      <param name='wmode' value='transparent'/>

      <?php if ($ad->width) { ?>
        <param name='width' value='<?php print $ad->width; ?>'/>
      <?php } ?>
      <?php if ($ad->background) { ?>
        <param name='bgcolor' value='<?php print $ad->background; ?>'/>
      <?php } ?>
      <embed src='<?php print $flash_path; ?>' quality='high'
        <?php if ($ad->background) { ?> bgcolor='<?php print $ad->background; ?>' <?php } ?>
        <?php if ($ad->width) { ?> width='<?php print $ad->width; ?> <?php } ?>
        name=' ms_ad_<?php print $ad->id; ?> wmode='transparent' align='middle'
             allowScriptAccess='sameDomain'
             allowFullScreen='false' type='application/x-shockwave-flash'
             pluginspage='http://www.adobe.com/go/getflashplayer'/>
    </object>
  </div>

  <?php if ($bottom_text) { ?>
    <div class='ms_ad_text_container'
         style='text-align:center;padding:5px 2px;'>
      <?php print $bottom_text; ?>
    </div>
  <?php } ?>

  <div style='<?php print $link_style_string; ?>'
       onclick="window.location = '<?php print $url; ?>';"></div>
</div>


