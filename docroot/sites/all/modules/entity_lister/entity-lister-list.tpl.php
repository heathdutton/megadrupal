<?php

if ($above) {
  print $pager;
}

print $list;

if ($below) {
  print $pager;
}

if ($total) {
  print '<p>' . t('The query yielded @count items', array('@count' => $total)) . '</p>';
}
