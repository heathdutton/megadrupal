<?php
print '<' . $element['#html_tag']
  . ' id="' . $element['#id'] . '"'
  . ' class="share42init"'
  . ' data-url="' . $element['#url'] . '"'
  . ' data-title="' . $element['#title'] . '"'
  . ' data-image="' . $element['#image'] . '"'
  . ' data-description="' . $element['#description'] . '"'
  . ' data-path="' . $element['#path'] . '"'
  . ' data-icons-file="' . $element['#icons_file'] . '"'
  . ' data-zero-counter="' . $element['#zero_counter'] . '"'
  . ' data-top1="' . $element['#top1'] . '"'
  . ' data-top2="' . $element['#top2'] . '"'
  . ' data-margin="' . $element['#margin'] . '"'
  . '></' . $element['#html_tag'] . '>';
?>