<?php

/**
 * @file
 * Default theme implementation to display a single Drupal page while offline.
 *
 * All the available variables are mirrored in html.tpl.php and page.tpl.php.
 * Some may be blank but they are provided for consistency.
 *
 * @see template_preprocess()
 * @see template_preprocess_maintenance_page()
 *
 * @ingroup themeable
 */
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">

<head>
  <title><?php print $head_title; ?></title>
  <?php print $head; ?>
  <?php print $styles; ?>
  <?php print $scripts; ?>
</head>
<body class="<?php print $classes; ?>">

  <?php if (!empty($header)): ?>
    <div id="header">
        <?php print $header; ?>
    </div><!-- /#header -->
  <?php endif; ?>


  <?php if (!empty($sidebar_first)): ?>
    <div id="sidebar_maintenance">
      <?php print $sidebar_first; ?>
    </div> <!-- /sidebar-first -->
  <?php endif; ?>

  <div id="main_content_maintenance">
    <div class="inner">
      <?php if (!empty($title)): ?><h1 class="title" id="page-title"><?php print $title; ?></h1><?php endif; ?>
      <?php if (!empty($messages)): print $messages; endif; ?>
      <?php print $content; ?>
    </div> <!-- /inner -->
  </div> <!-- /content-content -->

  <?php if (!empty($sidebar_second)): ?>
    <div id="sidebar-second">
      <?php print $sidebar_second; ?>
    </div> <!-- /sidebar-second -->
  <?php endif; ?>


  <?php if (!empty($footer)): ?>
  <div id="footer-wrapper">
    <div id="footer">
       <?php print $footer; ?>
    </div> <!-- /footer -->
  </div> <!-- /footer-wrapper -->
  <?php endif; ?>

</body>
</html>
