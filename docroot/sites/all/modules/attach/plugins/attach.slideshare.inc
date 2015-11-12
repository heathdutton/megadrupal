<?php
/**
 * Embed slideshare resource.
 */
function attach_slideshare_process($params) {
  $output = '<iframe width="' . $params['width'] . '" scrolling="no" height="' . $params['height'] . '" frameborder="0" marginheight="0" marginwidth="0" src="http://www.slideshare.net/slideshow/embed_code/' . $params['id'] . '"></iframe>';
  return '<div class="attach-content" style="width:' . $params['width'] . 'px">' . $output . '</div>';
}
