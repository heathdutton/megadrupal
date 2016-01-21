<?php 
//    Copyright 2008 Avioso Limited

//    This file is part of .AD Novus.

//    .AD Novus is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.

//    .AD Novus is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License
//    along with .AD Novus.  If not, see <http://www.gnu.org/licenses/>.
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/2002/REC-xhtml1-20020801/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $language->language ?>" xml:lang="<?php print $language->language ?>">
<head>
  <title><?php print $head_title ?></title>
  <?php print $head ?>
  <?php print $styles ?>
  <?php if ($right);else echo "<style type=\"text/css\" media=\"all\">#main {margin-right:0;}</style>"; ?>
  <?php if ($left);else echo "<style type=\"text/css\" media=\"all\">#main {margin-left:0;}</style>"; ?>
  <?php if ($right);elseif($left);else echo "<style type=\"text/css\" media=\"all\">#main {margin:0;}</style>"; ?>
  <!--[if IE]>
  <style>#sidebar-right {position:relative;left:-1px;}
  body {background-position: 0 242px;}
  #header {min-width:950px;}
  #primary {min-width:950px;}
  #main,#sidebar-left,#sidebar-right {
_display: inline; /* display inline or double your floated margin! [1] */
_overflow: hidden; /* in ie6, overflow auto is broken [2] and so is overflow visible [3] */
_overflow-y: visible;
}
  </style>
  <![endif]-->
  <?php print $scripts ?>
  <script type="text/javascript"></script>
</head>

<body class="<?php print $body_classes; ?>">
 <?php if (isset($primary_links)) { ?> 
    <div id="primary">
    <?php print preg_replace('/<a (.*?)>(.*?)<\\/a>/i','<a \\1><span>\\2</span></a>',theme('links', $primary_links, array('class' =>'links', 'id' => 'navlist'))); ?></div>
  <?php } ?>

    <div id="header-region">
      <?php print $header ?>
    </div>
  <div id="header"><div id="h2">
    <?php if ($logo) { ?><a href="<?php print $front_page ?>" title="<?php print t('Home') ?>"><img src="<?php print $logo ?>" alt="<?php print t('Home') ?>" /></a><?php } ?>

    <?php if ($search_box) : ?>
      <div class="search-box"><?php print $search_box ?></div>
    <?php endif; ?>
    
      <h1 class='site-name'><a href="<?php print $front_page ?>" title="<?php print t('Home') ?>"><?php print $site_name ?></a></h1>
    
    <span class="clear"></span>
    <?php if ($site_slogan) { ?>
      <div class='site-slogan'><?php print $site_slogan ?></div>
    <?php } ?>
  </div></div>
 <div id="wrapper">
  
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

  <br clear="all"/>
  <div id="footer">
  <?php print $footer_message;?><br/>
    <a href="http://www.rockettheme.com/Templates/Free_Templates/Novus_-_Free_Joomla_Template/">Theme</a> <a href="http://www.avioso.com">port</a> sponsored by Duplika <a href="http://www.duplika.com">Web Hosting</a>.
  <?php print $closure ?>
  </div>
</body>
</html>
