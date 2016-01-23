<?php
  $slide1_head = check_plain(theme_get_setting('slide1_head'));
  $slide1_desc = filter_xss_admin(theme_get_setting('slide1_desc'));
  $slide1_url = check_plain(theme_get_setting('slide1_url'));
  $slide1_img = check_plain(theme_get_setting('slide1_image_url'));
  $slide1_alt = check_plain(theme_get_setting('slide1_alt'));
  
  $slide2_head = check_plain(theme_get_setting('slide2_head'));
  $slide2_desc = filter_xss_admin(theme_get_setting('slide2_desc'));
  $slide2_url = check_plain(theme_get_setting('slide2_url'));
  $slide2_img = check_plain(theme_get_setting('slide2_image_url'));
  $slide2_alt = check_plain(theme_get_setting('slide2_alt'));
  
  $slide3_head = check_plain(theme_get_setting('slide3_head'));
  $slide3_desc = filter_xss_admin(theme_get_setting('slide3_desc'));
  $slide3_url = check_plain(theme_get_setting('slide3_url'));
  $slide3_img = check_plain(theme_get_setting('slide3_image_url'));
  $slide3_alt = check_plain(theme_get_setting('slide3_alt'));
  
// Default values in case the alt text is not populated.
  if ($slide1_alt == ""):
    $slide1_alt = "slide 1";
  endif;
  if ($slide2_alt == ""):
    $slide2_alt = "slide 2";
  endif;
  if ($slide3_alt == ""):
    $slide3_alt = "slide 3";
  endif;
?>

<div id="slider" role="complementary">
<ul class="slides">
  <li>
    <div class="post slide1">
    <div class="entry-container">
      <div class="entry-header"><div class="entry-title"><a href="<?php print url($slide1_url); ?>"><?php print $slide1_head; ?></a></div></div>
      <div class="entry-summary"><?php print $slide1_desc; ?></div>
      <div class="clearfix"></div>
    </div>
    <a href="<?php print url($slide1_url); ?>">
  <?php if($slide1_img != '') { ?>
    <img src="<?php print $slide1_img; ?>" class="slide-image" alt="<?php print $slide1_alt; ?>" /></a>
  <?php } else { ?>
    <img src="<?php print base_path() . drupal_get_path('theme', 'zeropoint') . '/_custom/sliderimg/slide-1.jpg'; ?>" class="slide-image" alt="<?php print $slide1_alt; ?>" /></a>
  <?php } ?>
    <div class="clearfix"></div>
    </div>
  </li>
  <li>
    <div class="post slide2">
    <div class="entry-container">
      <div class="entry-header"><div class="entry-title"><a href="<?php print url($slide2_url); ?>"><?php print $slide2_head; ?></a></div></div>
      <div class="entry-summary"><?php print $slide2_desc; ?></div>
      <div class="clearfix"></div>
    </div>
    <a href="<?php print url($slide2_url); ?>">
  <?php if($slide2_img != '') { ?>
    <img src="<?php print $slide2_img; ?>" class="slide-image" alt="<?php print $slide2_alt; ?>" /></a>
  <?php } else { ?>
    <img src="<?php print base_path() . drupal_get_path('theme', 'zeropoint') . '/_custom/sliderimg/slide-2.jpg'; ?>" class="slide-image" alt="<?php print $slide2_alt; ?>" /></a>
  <?php } ?>
    <div class="clearfix"></div>
    </div>
  </li>
  <li>
    <div class="post slide3">
    <div class="entry-container">
      <div class="entry-header"><div class="entry-title"><a href="<?php print url($slide3_url); ?>"><?php print $slide3_head; ?></a></div></div>
      <div class="entry-summary"><?php print $slide3_desc; ?></div>
      <div class="clearfix"></div>
    </div>
    <a href="<?php print url($slide3_url); ?>">
  <?php if($slide3_img != '') { ?>
    <img src="<?php print $slide3_img; ?>" class="slide-image" alt="<?php print $slide3_alt; ?>" /></a>
  <?php } else { ?>
    <img src="<?php print base_path() . drupal_get_path('theme', 'zeropoint') . '/_custom/sliderimg/slide-3.jpg'; ?>" class="slide-image" alt="<?php print $slide3_alt; ?>" /></a>
  <?php } ?>
    <div class="clearfix"></div>
    </div>
  </li>
</ul>
</div>
