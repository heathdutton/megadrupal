<?php
/**
 * @file
 * Template to take selfi.
 *
 * Available variables:
 * - $video_width: The width of the video element.
 * - $video_height: The height of the video element.
 */
?>
<div class="video-container">
  <div class="selfi">
    <canvas id="canvas"></canvas>
    <video id="selfi_video" width=<?php print $video_width;?> height=<?php print $video_height;?>></video>
  </div>
  <div class="actions">
    <input type="button" id="startbutton" class="form-submit" value="Start camera">
    <input type="button" id="takepicture" class="form-submit" disabled="true" value="Take picture">
  </div>
</div>
