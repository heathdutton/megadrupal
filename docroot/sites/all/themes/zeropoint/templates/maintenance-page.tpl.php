<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $language->language; ?>" xml:lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>">

<head>
<title><?php print $head_title ?></title>
<?php if (theme_get_setting('grid_responsive') == '1'): ?>
<meta name="HandheldFriendly" content="true" />
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta name="MobileOptimized" content="width" />
<?php endif; ?>
<?php print $head ?>
<?php print $styles ?>
<?php print $scripts ?>
</head>

<body id="<?php print $body_id; ?>" class="<?php print $classes; ?>" <?php print $attributes;?>>

<?php print $page_top; ?>
  
<div id="top_bg">
<div class="sizer0 clearfix"<?php print wrapper_width() ?>>
<div id="top_left">
<div id="top_right">
<div id="headimg">

<div id="header">
<div class="clearfix">
  <?php if ($logo): ?><a href="<?php print check_url($front_page); ?>" title="<?php print t('Home'); ?>"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" class="logoimg" /></a><?php endif; ?>
  <div id="name-and-slogan">
  <?php if ($site_name): ?>
    <h1 id="site-name"><a href="<?php print check_url($front_page); ?>" title="<?php print t('Home'); ?>"><?php print $site_name; ?></a></h1>
  <?php endif; ?>
  <?php if ($site_slogan): ?><div id="site-slogan"><?php print $site_slogan; ?></div><?php endif; ?>
  </div>
</div>
<div class="menuband clearfix">
  <div id="menu" class="menu-wrapper">
  <?php if ($logo || $site_name): ?>
    <a href="<?php print check_url($front_page); ?>" class="pure-menu-heading" title="<?php if ($site_slogan) print $site_slogan; ?>">
      <?php if ($logo): ?><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" class="logomob" /><?php endif; ?>
      <?php if ($site_name) print $site_name; ?>
    </a>
  <?php endif; ?>
  </div>
</div>
</div>

</div></div></div></div></div>

<div id="body_bg">
<div class="sizer0 clearfix"<?php print wrapper_width() ?>>
<div id="body_left">
<div id="body_right">

<div class="clearfix"></div>

<div class="clearfix">
<div id="middlecontainer" class="pure-g">
  <div class="pure-u-1">
    <div id="main">
      <?php print render($title_prefix); ?>
      <?php if ($title): print '<h2 class="title">'. $title .'</h2>'; endif; ?>
      <?php print render($title_suffix); ?>
      <?php print $messages ?>
      <?php print $content; ?>
    </div>
  </div>
</div>
</div>

</div></div></div></div>

<div id="bottom_bg">
<div class="sizer0 clearfix"<?php print wrapper_width() ?>>
<div id="bottom_left">
<div id="bottom_right">

<div id="footer" class="pure-g">
  <div class="pure-u-1">
    <?php if (theme_get_setting('social_links_display')): ?><div id="soclinks"><?php print zeropoint_social_links(); ?></div><?php endif; ?>
  </div>
</div>
<div id="brand"></div>

</div></div></div></div>

<?php print $page_bottom; ?>
<?php print $page_b; ?>

</body>
</html>