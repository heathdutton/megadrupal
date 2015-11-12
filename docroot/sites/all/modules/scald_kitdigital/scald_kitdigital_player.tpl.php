<?php
/**
 * @file
 * Default theme implementation for the Scald kit digital Player
 * TODO use configuration variable of the player and maybe HTML5 player.
 */
?>

<div id="flash_kplayer_<?php print $vars['video_id'] ?>" class="flash_kplayer" name="flash_kplayer" data-sig="<?php print $vars['video_id'] ?>" data-playerkey="793d38ece7ea" style="width:400px; height:300px;">
  <object  width="100%" height="100%" type="application/x-shockwave-flash" data="http://sa.kewego.com/swf/kp.swf" name="kplayer_<?php print $vars['video_id'] ?>" id="kplayer_<?php print $vars['video_id'] ?>">
  <param name="bgcolor" value="0x000000" />
  <param name="allowfullscreen" value="true" />
  <param name="allowscriptaccess" value="always" />
  <param name="flashVars" value="language_code=fr&playerKey=793d38ece7ea&configKey=&suffix=&sig=<?php print $vars['video_id'] ?>&autostart=false" />
  <param name="movie" value="http://sa.kewego.com/swf/kp.swf" /><param name="wmode" value="opaque" />
  <param name="SeamlessTabbing" value="false" />
  <video poster="http://api.kewego.com/video/getHTML5Thumbnail/?playerKey=793d38ece7ea&sig=<?php print $vars['video_id'] ?>" height="100%" width="100%" preload="none"  controls="controls">
  </video>
  <script src="http://sa.kewego.com/embed/assets/kplayer-standalone.js"></script>
  <script defer="defer">kitd.html5loader("flash_kplayer_<?php print $vars['video_id'] ?>");</script>
  </object>
</div>
