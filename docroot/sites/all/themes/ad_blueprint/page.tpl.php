<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/2002/REC-xhtml1-20020801/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $language->language ?>" xml:lang="<?php print $language->language ?>">
<head>
  <title><?php print $head_title ?></title>
  <?php print $head ?>
  <?php print $styles ?>
  <?php if ($right);else echo "<style type=\"text/css\" media=\"all\">#main {margin-right:0;} #page{background-image:none;}</style>"; ?>
  <?php if ($left);else echo "<style type=\"text/css\" media=\"all\">#main {margin-left:0;} #page2{background-image:none;}</style>"; ?>
  <?php if ($right);elseif($left);else echo "<style type=\"text/css\" media=\"all\">#main {margin:0;} #page2{background-image:none;}</style>"; ?>
  <!--[if IE]>
  <style>#sidebar-right {position:relative;left:-1px;}
  #page2 {border-bottom:2px solid #CCC;}
  .site-name {position:relative;top:-20px;}
  .site-slogan {position:relative;top:-35px;}</style>
  <![endif]-->
  <?php print $scripts ?>
  <script type="text/javascript"><?php /* Needed to avoid Flash of Unstyle Content in IE */ ?> </script>
</head>

<body class="<?php print $body_classes; ?>">
  <div id="wrapper">
  <?php if ($logo) { ?><a href="<?php print $front_page ?>" title="<?php print t('Home') ?>"><img src="<?php print $logo ?>" alt="<?php print t('Home') ?>" /></a><?php } ?>
    <div id="header-region">
      <?php print $header ?>
    </div>
  <div id="header"> 
    <?php if ($search_box) : ?>
      <div class="search-box"><?php print $search_box ?></div>
    <?php endif; ?>
    <?php if ($site_name) { ?>
      <h1 class='site-name'><a href="<?php print $front_page ?>" title="<?php print t('Home') ?>"><?php print $site_name ?></a></h1>
    <?php } ?>
    <?php if ($site_slogan) { ?>
      <div class='site-slogan'><?php print $site_slogan ?></div>
    <?php } ?>
  </div>
  <?php if (isset($primary_links)) { ?>
    <div id="primary"><?php print theme('links', $primary_links, array('class' =>'links', 'id' => 'navlist')) ?></div>
  <?php } ?>
  <div id="page"><div id="page2">
    <?php if ($left) { ?>
      <div id="sidebar-left" class="clearfix">
      <?php print $left ?>
      </div>
    <?php } ?>
    <div id="main" class="clearfix">
      <?php if ($mission) { ?><div id="mission"><?php print $mission ?></div><?php } ?>
      <div class="inner">
        <?php print $breadcrumb ?>
        <h1 class="title"><?php print $title ?></h1>
        <?php if ($tabs){ ?><div class="tabs"><?php print $tabs ?></div><?php } ?>
        <?php print $help ?>
        <?php if ($show_messages && $messages) print $messages; ?>
        <?php print $content; ?>
        <?php print $feed_icons; ?>
      </div>
    </div>
    <?php if ($right): ?>
      <div id="sidebar-right" class="clearfix">
      <?php print $right ?>
      </div>
    <?php endif; ?>
    <br clear="all"/>
    <span class="clear"></span>
  </div>
  </div>
  </div>
  <br clear="all"/>
  <div id="footer">
  <?php print $footer;?><br/>
    <a href="http://www.fireandknowledge.org">Theme</a> <a href="http://www.avioso.com">port</a> sponsored by Duplika <a href="www.duplika.com">Web Hosting</a>.
  <?php print $closure ?>
  </div>
</body>
</html>
