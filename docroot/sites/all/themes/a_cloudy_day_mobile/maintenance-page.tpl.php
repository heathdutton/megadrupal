<?php // $Id$ ?>
<?php print "<?xml version='1.0' encoding='UTF-8' ?>"; ?>
<!DOCTYPE html PUBLIC "-//OPENWAVE//DTD XHTML Mobile 1.0//EN"
"http://www.openwave.com/dtd/xhtml-mobile10.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" version="XHTML+RDFa 1.0" dir="<?php print $language->dir; ?>"

<head>
  <?php print $head; ?>
  <meta name="viewport" content="width=device-width; initial-scale=1.0; maximum-scale=1.5; user-scalable=1;"/>
  <title><?php print $head_title; ?></title>
  <?php print $styles; ?>
  <?php print $scripts; ?>
  <link href='http://fonts.googleapis.com/css?family=Lobster' rel='stylesheet' type='text/css' />
</head>
<body>     
  <?php if($main_menu): ?>
    <div id="top">
      <div id="topmenu">
        <?php print theme('links__system_main_menu', array('links' => $main_menu, 'attributes' => array('id' => 'main-menu', 'class' => array('menu', 'inline', 'clearfix')))); ?>
      </div>    
		</div><!-- /top -->
  <?php endif; ?>
  <?php if($secondary_menu): ?>
    <div id="submenu">
      <?php print theme('links__system_secondary_menu', array('links' => $secondary_menu, 'attributes' => array('id' => 'secondary-menu', 'class' => array('menu', 'inline', 'clearfix')))); ?>
    </div>
  <?php endif; ?>  
  <div id="header">
    <div id="headertitle">
      <h1><a href="<?php print $front_page;?>" title="<?php print t('Home') ?>"><?php print $site_name;?></a></h1>
      <div class='site-slogan'>
        <?php print $site_slogan ;?>
      </div>
    </div>
    <?php if ($logo) : ?>
      <div id="logo">
        <a href="<?php print $front_page ?>" title="<?php print t('Home') ?>"><img src="<?php print($logo) ?>" alt="<?php print t('Home') ?>" /></a>
      </div>
    <?php endif; ?>
    <?php if($header_block): ?><div class="block block-theme"><?php print $header_block ?></div><?php endif; ?>
  </div><!-- /header -->
  <?php if($messages): echo "<div id=\"messagebox\">" . $messages . "</div>"; endif; ?>         
  <div id="contentcontainer">
    <div id="container">
      <div id="contentleft">
        <div id="page">
          <?php print $breadcrumb; ?>
          <a id="main-content"></a>
          <?php if ($title): ?>
            <h1 class="title" id="page-title"><?php print $title; ?></h1>
          <?php endif; ?>

          <?php print $content; ?>
          <div class="feedicons">
            <?php echo $feed_icons ?>
          </div>
        </div>
        <?php if ($footer_block): print "<div id=\"footer\">" . $footer_block . "</div>"; endif;?>
      </div><!-- /contentleft -->
    </div><!-- /container -->
  </div><!-- /contentcontainer -->
  <div id="bottompage">
    <div id="skyline"></div>
  </div>    
  </body>
</html>