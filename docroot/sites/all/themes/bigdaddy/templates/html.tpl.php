<!DOCTYPE html>
<html class="no-js" lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>">
<head>
  <title><?php print $head_title; ?></title>
  <?php print $head; ?>
  <?php print $styles; ?>

  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <meta http-equiv="X-UA-Compatible" content="IE=Edge;chrome=1" />

  <!--[if lt IE 9]>
    <script src="http://html5shiv.googlecode.com/svn/trunk/html5.js"></script>
  <![endif]-->

</head>

<body class="<?php print $classes; ?>" <?php print $attributes;?>>
<!--[if lt IE 9]>
<p class="outdated">Hi dear visitor! Sorry to disturb your navigation, but you are currently using an outdated browser. <br /><a href="http://www.whatbrowser.org/" target="_blank">Please try a different one</a> to improve your online experience!</p>
<![endif]-->

  <div id="skip-link">
    <a href="#main-content" class="element-invisible element-focusable"><?php print t('Skip to main content'); ?></a>
  </div>
  <?php print $page_top; ?>
  <?php print $page; ?>
  <?php print $page_bottom; ?>

  <?php print $scripts; ?>

</body>
</html>