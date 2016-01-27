<?php
$width_prop = theme_get_setting('sooperfish_width', $theme); //subtract margin and border
if ($width_prop) {
  $CSS .= "\n\n";
  $CSS .= "body .primary-menu ul ul {width:" . $width_prop . "px;}\n";
  $CSS .= "body .primary-menu ul.menu ul.menu li {width:" . $width_prop . "px;}\n";
  $CSS .= "body .primary-menu ul ul ul {margin-left:" . $width_prop . "px;}\n";
}