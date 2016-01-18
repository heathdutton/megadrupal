<?php

function zenstrap_preprocess_node(&$variables) {
  if ($variables['display_submitted']) {
    $variables['submitted'] = '<br/><span class="author">' . $variables['name'] . '</span>' .
     '<br/><span class="submission-date">' . $variables['date']  . '</span>';
  }
}
