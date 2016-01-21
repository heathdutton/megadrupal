<?php

foreach ($my_donate_pages as $my_donate_page) {
  echo '<h2>' . $my_donate_page->Title . '</h2>';
  echo '<div>' . $my_donate_page->Description . '</div>';
  $uri = $my_donate_page->uri();
  echo '<div>' . l(t('Select this campaign'), $uri['path']) . '</div>';
}
