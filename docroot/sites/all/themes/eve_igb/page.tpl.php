<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $language ?>" xml:lang="<?php print $language ?>">
<head>
  <title><?php print $head_title?></title>
  <?php

//print $head;

// We don't really need all this....
// @@@ Replace with stylesheet override files!
//print $styles;

// Switch in a css for igb / ogb.
print '<link rel="stylesheet" type="text/css" href="'. base_path() . drupal_get_path('theme', 'eve_igb') .'/'. (isEvET()?'igb.css':'ogb.css') .'" />';

?>
</head>
<body>

<h1 class="center">
<?php
  print check_plain($site_name);
  if (isset($site_slogan)) {
    print ': '. check_plain($site_slogan);
  }
?>
</h1>
<?php
print '<h2 class="center">'. $title ."</h2>\n";

print "<hr />\n\n";

print '<h3 class="center">'. eve_igb_menu_links($primary_links) .'</h3>';
if (!empty($secondary_links)) {
  print '<div class="center">'. eve_igb_menu_links($secondary_links,' ') .'</div>';
}

print "<hr />\n<br />\n";


// xxx $secondary_links, $search_box
//
// $header = not used?????


// xxx mission

print "<h5>$breadcrumb</h5>\n";


// xxx igb tabs
print $tabs;

// xxx other stuff
// $help
// $messages

print $content;

print "<h6>$footer_message</h6>\n";

// ???
print $closure;
?>
</div>

</body>
</html>
