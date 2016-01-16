<!DOCTYPE html PUBLIC "-//WAPFORUM//DTD XHTML Mobile 1.2//EN" "http://www.openmobilealliance.org/tech/DTD/xhtml-mobile11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title><?php print $head_title ?></title>
<style type="text/css">
  hr { clear: both; border:solid; border-width:1px; border-bottom-color:#007300; border-top-color:#ffffff; border-left-color:#ffffff; border-right-color:#ffffff;}
  .accesskey { text-decoration: underline; }
  img { border: 0; }
  a { text-decoration: none; }
  .header { font-size: small; background-color: <?php print theme_get_setting('mobi_header_background_color') ?>; color: blue; border-bottom: solid 1px blue; margin: 0 0 5px 0; padding: 2px; }
  .content { background-color: <?php print theme_get_setting('mobi_content_background_color') ?>; }
  .footer { font-size: small; background-color: <?php print theme_get_setting('mobi_footer_background_color') ?>; color: blue; border-top: solid 1px blue; margin: 10px 0 0 0; }
<?php if ($sidebar_left) { ?>
  .sidebar-left { background-color: <?php print theme_get_setting('mobi_left_background_color') ?>; }
<?php } ?>
<?php if ($sidebar_right) { ?>
  .sidebar-right { background-color: <?php print theme_get_setting('mobi_right_background_color') ?>; }
<?php } ?>
</style>
</head>
<body>
<div class="header">
  <?php if ($logo) { ?><img src="<?php print $logo ?>" alt="logo" /><br /><?php } ?>
  <?php if (isset($primary_links)) { ?><?php print theme('links', $primary_links) ?><?php } ?>
</div>
<?php if ($left) { ?><div class="sidebar-left"><?php print $left ?></div><hr /><?php } ?>
<div class="content">
  <?php if ($breadcrumb) { print $breadcrumb; } ?>
  <?php if ($mission) { print $mission; } ?>
  <?php if ($title) { print '<h2>'. $title .'</h2>'; } ?>
  <?php if ($tabs) { print $tabs; } ?>
  <?php if (isset($tabs2)) { print $tabs2; } ?>
  <?php if ($help) { print $help; } ?>
  <?php if ($messages) { print $messages; } ?>
  <?php print $content ?>
  <?php print $feed_icons ?>
</div>
<?php if ($right) { ?><hr /><div class="sidebar-right"><?php print $right ?></div><?php } ?>
<div class="footer">
  <?php if (isset($primary_links)) { ?><?php print theme('links', $primary_links) ?><?php } ?>
  <?php print $footer_message ?>
</div>
<?php print $closure; ?>
</body>
</html>
