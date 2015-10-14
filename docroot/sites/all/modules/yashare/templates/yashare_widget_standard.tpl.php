<?php
print '<' . $element['#html_tag']
  . ' id="' . $element['#id'] . '"'
  . ' class="yashare ' . $element['#yashare_theme'] . ' ' . $element['#yashare_type'] . '"'
  . '></' . $element['#html_tag'] . '>';
?>