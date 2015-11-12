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
 * @var $image_path
 *   The path to the image.
 * @var $text
 *   The text to show on the bottom of the ad.
 * @var $class
 *   The class of the add.
 */

$style_string = '';
foreach ($styles as $key => $value) {
  $style_string .= $key . ':' . $value . ';';
}

$img_style = 'border:0;';
if ($ad->width) {
  $img_style .= "width:" . $ad->width . "px;";
}

$link_style_string = '';
foreach ($link_styles as $key => $value) {
  $link_style_string .= $key . ':' . $value . ';';
}

?>

<div class="ms_ad_container <?php print $class; ?>"
     style='<?php print $style_string; ?>'>
  <div class='ms_ad_file_container'>
    <?php if ($ad->link) { ?>
    <a href='<?php print $url; ?>'>
      <?php } ?>
      <img src='<?php print $image_path; ?>' style='<?php print $img_style; ?>'
           alt='<?php print $ad->alt; ?>'/>
      <?php if ($ad->link) { ?>
    </a>
  <?php } ?>
  </div>

  <?php if ($text) { ?>
    <div class='ms_ad_text_container'
         style='text-align:center;padding:5px 2px;'>
      <?php print $text; ?>
    </div>
  <?php } ?>

  <div style='<?php print $link_style_string; ?>'
       onclick="window.location = '<?php print $url; ?>';">

  </div>
</div>
