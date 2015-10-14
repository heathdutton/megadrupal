<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $language->language ?>" xml:lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">

<head>
<title><?php print $head_title ?></title>
<?php if (theme_get_setting('grid_responsive') == '1'): ?>
<meta name="HandheldFriendly" content="true" />
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta name="MobileOptimized" content="width" />
<?php endif ?>
<?php print $head ?>
<?php print $styles ?>
<?php print $scripts ?>
</head>

<body id="<?php print $body_id ?>" class="<?php print $classes ?>" <?php print $attributes ?>>

<?php print $page_top ?>

<div id="mmenu_bg">
<div class="sizer0 clearfix"<?php print wrapper_width() ?>>
<div class="menuband clearfix">
  <div id="menu" class="menu-wrapper">
<?php if ($logo || $site_name): ?>
  <a href="<?php print check_url($front_page) ?>" class="pure-menu-heading" title="<?php if ($site_slogan) print $site_slogan ?>"><?php if ($logo): ?><img src="<?php print $logo ?>" alt="<?php print t('Home') ?>" class="logomob" /><?php endif ?><b><?php if ($site_name) print $site_name ?></b></a>
<?php endif ?>
  </div>
</div></div></div>

<div id="body_bg">
<div class="sizer0 clearfix"<?php print wrapper_width() ?>>

<div class="clearfix">
<div id="middlecontainer" class="pure-g">
  <div class="pure-u-1">
    <div id="main">
      <?php print render($title_prefix) ?>
      <?php if ($title): print '<h2 class="title">'. $title .'</h2>'; endif ?>
      <?php print render($title_suffix) ?>
      <?php print $messages ?>
      <?php print $content ?>
    </div>
  </div>
</div>
</div>

<div class="space"></div>
</div></div>

<div class="sizer0 clearfix"<?php print wrapper_width() ?>>
<div id="footer" class="pure-g">
<div class="<?php print resp_class() ?>1-5"></div>
<div class="<?php print resp_class() ?>3-5"></div>
<div class="<?php print resp_class() ?>1-5"></div></div>
</div>

<?php print $page_bottom ?>

</body>
</html>