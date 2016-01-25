<?php

/**
 * @file
 * Template file to customize markup in prettyphoto.
 */
?>
<div class="pp_pic_holder">
  <div class="ppt">&nbsp;</div>
  <div class="pp_top">
    <div class="pp_left"></div>
    <div class="pp_middle"></div>
    <div class="pp_right"></div>
  </div>
  <div class="pp_content_container">
    <div class="pp_left">
      <div class="pp_right">
        <div class="pp_content">
          <div class="pp_loaderIcon"></div>
          <div class="pp_fade">
            <a href="#" class="pp_expand" title="<?php print $expand_title; ?>"><?php print $expand; ?></a>
            <div class="pp_hoverContainer">
              <a class="pp_next" href="#"><?php print $next; ?></a>
              <a class="pp_previous" href="#"><?php print $prev; ?></a>
            </div>
            <div id="pp_full_res"></div>
            <div class="pp_details">
              <div class="pp_nav">
                <a href="#" class="pp_arrow_previous"><?php print $prev; ?></a>
                <p class="currentTextHolder">0/0</p>
                <a href="#" class="pp_arrow_next"><?php print $next; ?></a>
              </div>
              <p class="pp_description"></p>
              <div class="pp_social">{pp_social}</div>
              <a class="pp_close" href="#"><?php print $close; ?></a>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
  <div class="pp_bottom">
    <div class="pp_left"></div>
    <div class="pp_middle"></div>
    <div class="pp_right"></div>
  </div>
</div>
<div class="pp_overlay"></div>
