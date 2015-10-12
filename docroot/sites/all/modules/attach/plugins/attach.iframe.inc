<?php
/**
 * Embed iframe.
 */
function attach_iframe_process($params) {
  $output = '<iframe width="' . $params['width'] . '" scrolling="' . $params['scrolling'] . '" height="' . $params['height'] . '" frameborder="0" marginheight="0" marginwidth="0" src="' . $params['url'] . '">' . t('Loading...') . '</iframe>';
  return '<div class="attach-content" style="width:' . $params['width'] . 'px">' . $output . '</div>';
}
