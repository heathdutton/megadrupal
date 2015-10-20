<?php
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $language->language ?>" xml:lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
<head>
  <title><?php print $head_title ?></title>
  <meta http-equiv="Content-Style-Type" content="text/css" />
  <?php print $head ?>
  <?php print $styles ?>
  <?php print $scripts ?>
</head>
<body<?php if (!$right) { print " class='wide-page'"; } ?>>
<div id="container">
<div id="page">
  <div id="main">
    <div id="content">

      <?php if ($mission) { ?><div id="mission"><?php print $mission ?></div><?php } ?>
      <div class="tabs"><?php print $tabs ?></div>
      <?php if ($title) { ?><h1 class="title"><?php print $title ?></h1><?php } ?>
      <?php print $help ?>
      <?php if ($show_messages && $messages) { print $messages; } ?>
      <?php print $content ?>
      <?php print $breadcrumb ?>
    </div>
    <div id="right">
      <?php if ($left) { ?>
        <div id="sidebar-left">
          <?php print $left ?>
        </div>
      <?php } ?>
      <?php if ($right) { ?>
        <div id="sidebar-right">
          <?php print $search_box ?>
          <?php print $right ?>
        </div>
      <?php } ?>
    </div>
    <div class="clear-both"></div>
  </div>
  <div id="header">
    <div id="title">
      <?php if ($site_name) { ?><h1><a href="<?php print $base_path ?>" title="<?php print t('Home') ?>"><?php print $site_name ?></a></h1><?php } ?>
      <?php if ($site_slogan) { ?><div class='site-slogan'><?php print $site_slogan ?></div><?php } ?>
      <div id="title-spacer"></div>

    </div>
    <div id="nav">
      <?php if (isset($primary_links)) { ?><?php print theme('links', $primary_links, array('class' =>'links', 'id' => 'navlist')) ?><?php } ?>
    </div>
    <div id="headerLinks">
    <?php print $header ?>
    </div>
  </div>

</div>
</div>
<div id="footer">
<div id="footerInner">
    <?php print $footer_message . $footer ?>

  </div>
   <!-- do not remove the following footer information -->
    <div id="themeInfo">
    design by <a href="http://www.dropshipaccess.com">drop ship</a> access
    <a href="http://www.dropshipaccess.com"><img src="<?php echo base_path() . path_to_theme() . "/images/dsalogo.jpg"; ?>" alt="dropshipping" border="0" /></a>
    </div>
    <!-- end footer information -->
</div>
<?php print $closure ?>
</body>
</html>
