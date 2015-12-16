<?php

/**
 * @file
 *   Template file for the facebook like widget.
 */

print '<iframe src="http://www.facebook.com/plugins/like.php?href=' . urlencode(drupal_strip_dangerous_protocols($params['url'])) . '&amp;layout=button_count&amp;show_faces=false&amp;width=100&amp;action=like&amp;font=arial&amp;colorscheme=light" scrolling="no" frameborder="0" allowTransparency="true" style="border:none; overflow:hidden; width:120px; height:20px"></iframe>';