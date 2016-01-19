<?php
/**
 * Embed SWF files.
 */
function attach_flash_process($params) {
  if (module_exists('swftools')) {
    $output = swf($params['file'], array('params' => array('width' => $params['width'], 'height' => $params['height'])));
  }
  else {
    $output = '<object classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000" codebase="http://download.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=6,0,0,0" width="' . $params['width'] . '" height="' . $params['height'] . '">
<param name="movie" value="' . $params['file'] . '">
<param name="quality" value="high">
  <embed src="' . $params['file'] . '" quality="high" width="' . $params['width'] . '" height="' . $params['height'] . '" name="' . $params['file'] . '" type="application/x-shockwave-flash" pluginspace="http://www.macromedia.com/go/getflashplayer"></embed>
</object>';
  }
  return '<div class="attach-content" style="width:' . $params['width'] . 'px">' . $output . '</div>';
}
