<?php
/**
 * @file
 * Commerce dressing room page tpl.
 *
 * @ingroup commerce_dressing_room
 */
?>

<div id="description"><?php print $description; ?></div>
<div id="message"><?php print $message; ?></div>
<div id="info" style="display:none">
  <p><?php print $info1; ?></p>
  <p><?php print $info2; ?></p>
  <img src="<?php print $cdr_path; ?>/img/mediastream.png"/>
</div>

<div id="cam-container">
  <!-- Slideshow -->
  <div id="banner-fade">
    <ul class="bjqs">
      <?php print $images_without_background; ?>
    </ul>
  </div>
  <video id="webcam" autoplay width="640" height="480"></video>

  <video id="video-demo" controls="controls" poster="<?php print $cdr_path; ?>/videos/jsdetection.jpg" width="640" height="480"  onclick="if(/Android/.test(navigator.userAgent))this.play();">
    <!--  @TODO: change these videos  -->
    <source src="<?php print $cdr_path; ?>/videos/jsdetection.mp4" type="video/mp4" />
    <source src="<?php print $cdr_path; ?>/videos/jsdetection.ogv" type="video/ogg" />
    <source src="<?php print $cdr_path; ?>/videos/jsdetection.webm" type="video/webm" />
    <?php print $video_demo; ?>
  </video>

  <canvas id="canvas-blended" width="640" height="480"></canvas>
  <canvas id="canvas-source" width="640" height="480"></canvas>

  <div id="btnregion">
    <div id="back"><img id="btnregionback" src="<?php print $cdr_path; ?>/img/buttons.png"/></div>
    <div id="front">
      <img id="button0" src="<?php print $cdr_path; ?>/img/button_left.png"/>
      <img id="button1" src="<?php print $cdr_path; ?>/img/button_right.png"/>
    </div>
  </div>
</div>
