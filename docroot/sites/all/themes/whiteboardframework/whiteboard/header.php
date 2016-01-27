<?php

/**
 * @file header.php
 * A header file converted from the WordPress Whiteboard theme framework.
 */
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/ 1999/xhtml">
<head>
  <title><?php print $head_title; ?></title>
  <?php print $head; ?>
  <?php print $styles; ?>
  <?php print $scripts; ?>
  <script type="text/javascript"><?php /* Needed to avoid Flash of Unstyled Content in IE */ ?> </script>

	<link rel="profile" href="http://gmpg.org/xfn/11" />
	<meta name="viewport" content="width=device-width; initial-scale=1"/><?php /* Add "maximum-scale=1" to fix the Mobile Safari auto-zoom bug on orientation changes, but keep in mind that it will disable user-zooming completely. Bad for accessibility. */ ?>
	<?php /* The HTML5 Shim is required for older browsers, mainly older versions IE */ ?>
	<!--[if lt IE 9]>
		<script src="http://html5shim.googlecode.com/svn/trunk/html5.js"></script>
	<![endif]-->
</head>

<body class="<?php print $body_classes; ?>">
<div class="hide">
	<p><a href="#content">Skip to Content</a></p><?php /* used for accessibility, particularly for screen reader applications */ ?>
</div><!--.none-->
<div id="main" class="<?php print $classes; ?>"<?php print $attributes; ?>><!-- this encompasses the entire Web site -->
	<div id="header"><header>
		<div class="container">
			<div id="title">
        <?php if ($logo): ?>
          <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
            <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>"/>
          </a>
        <?php endif; ?>
        <?php if ($site_name || $site_slogan): ?>
          <div id="name-and-slogan">
            <?php if ($site_name): ?>
              <?php if ($title): ?>
                <div id="site-name">
                  <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><?php print $site_name; ?></a>
                </div>
              <?php else: /* Use h1 when the content title is empty */ ?>
                <h1 id="site-name">
                  <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><?php print $site_name; ?></a>
                </h1>
              <?php endif; ?>
            <?php endif; ?>
    
            <?php if ($site_slogan): ?>
              <div id="site-slogan"><?php print $site_slogan; ?></div>
            <?php endif; ?>
          </div>
        <?php endif; ?>
			</div><!--#title-->
			<?php if ($main_menu || $secondary_menu): ?>
        <div id="navigation" class="menu <?php if (!empty($main_menu)) {print "with-primary";} if (!empty($secondary_menu)) {print " with-secondary";} ?>">
          <?php print theme('links', array('links' => $main_menu, 'attributes' => array('id' => 'primary', 'class' => array('links', 'clearfix', 'main-menu')))); ?>
          <?php print theme('links', array('links' => $secondary_menu, 'attributes' => array('id' => 'secondary', 'class' => array('links', 'clearfix', 'sub-menu')))); ?>
        </div>
      <?php endif; ?>
      <?php if ($page['header']): ?>
        <div id="header-region">
          <?php print render($page['header']); ?>
        </div>
      <?php endif; ?><!--#header-image-->
			<div class="clear"></div>
		</div><!--.container-->
	</header></div><!--#header-->
	<div class="container">
