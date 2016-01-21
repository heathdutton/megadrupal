<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $language->language; ?>" xml:lang="<?php print $language->language; ?>">

<head>
  <title><?php print $head_title; ?></title>
  <?php print $head; ?>
  <?php print $styles; ?>
  <!--[if IE]>
    <link rel="stylesheet" href="<?php print $base_path . $directory; ?>/ie.css" type="text/css">
  <![endif]-->
  <?php print $scripts; ?>
</head>

<body class="<?php print $body_classes; ?>">

  <div id="page"><div id="page-inner">

    <a name="top" id="navigation-top"></a>
    <div id="skip-to-nav"><a href="#navigation"><?php print t('Skip to Navigation'); ?></a></div>

    <div id="header"><div id="header-inner" class="clear-block">

      <?php if ($logo || $site_name || $site_slogan): ?>
        <div id="logo-title">

          <?php if ($logo): ?>
            <div id="logo"><a href="<?php print $front_page ?>" title="<?php print t('Home'); ?>" rel="home"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" id="logo-image" /></a></div>
          <?php endif; ?>

          <?php if ($site_name): ?>
            <?php
              // Use an H1 only on the homepage
              $tag = $is_front ? 'h1' : 'div';
            ?>
            <<?php print $tag; ?> id='site-name'><strong>
              <a href="<?php print $front_page ?>" title="<?php print t('Home'); ?>" rel="home">
                <?php print $site_name; ?>
              </a>
            </strong></<?php print $tag; ?>>
          <?php endif; ?>

          <?php if ($site_slogan): ?>
            <div id='site-slogan'><?php print $site_slogan; ?></div>
          <?php endif; ?>

        </div> <!-- /#logo-title -->
      <?php endif; ?>

 	<?php if ($search_box || $primary_links || $secondary_links || $navbar): ?>
        <div id="navbar"><div id="navbar-inner">

          <a name="navigation" id="navigation"></a>

          <?php if ($search_box): ?>
            <div id="search-box">
            <div id="search-label"><?php print t('Search') ?></div>
              <?php print $search_box; ?>
            </div> <!-- /#search-box -->
            <span class="clear"></span>
          <?php endif; ?>

          <?php if ($primary_links): ?>
            <div id="primary">
              <?php print theme('links', $primary_links); ?>
            </div> <!-- /#primary -->
            <span class="clear"></span>
          <?php endif; ?>

          <?php if ($secondary_links): ?>
            <div id="secondary">
              <?php print theme('links', $secondary_links); ?>
            </div> <!-- /#secondary -->
          <?php endif; ?>


        </div></div> <!-- /#navbar-inner, /#navbar -->
      <?php endif; ?>
      <?php if ($header): ?>
        <div id="header-blocks">
          <?php print $header; ?>
        </div> <!-- /#header-blocks -->
      <?php endif; ?>
    </div></div> <!-- /#header-inner, /#header -->
    <div id="header-image">
      <?php 
      $breadcrumb2 = str_replace(' › ', ' // ', $breadcrumb);
      $breadcrumb3 = str_replace('<div class="breadcrumb">', '', $breadcrumb2);
      if ($is_front) {print '<div class="breadcrumb">// ' . t('Home') . '</div>';} else { echo '<div class="breadcrumb">// ' . str_replace('</div>', '', $breadcrumb3) . $title . '</div>' ;} ?>
    </div>

    <div id="main"><div id="main-inner" class="clear-block<?php if ($search_box || $primary_links || $secondary_links || $navbar) { print ' with-navbar'; } ?>">

      <div id="content"><div id="content-inner">



        <?php if ($title or $tabs or $help or $show_messages && $messages): ?>
          <div id="content-header">
            <?php if ($title): ?>
              <h1 class="title"><?php print $title; ?></h1>
            <?php endif; ?>
            <?php print $messages; ?>
            <?php if ($tabs): ?>
              <div class="tabs"><?php print $tabs; ?></div>
            <?php endif; ?>
            <?php print $help; ?>
          </div> <!-- /#content-header -->
        <?php endif; ?>

        <div id="content-area">
          <?php print $content; ?>
        </div>

        <?php if ($feed_icons): ?>
          <div class="feed-icons"><?php print $feed_icons; ?></div>
        <?php endif; ?>

      </div></div> <!-- /#content-inner, /#content -->

      <div id="right-container">
             <?php if ($mission): ?>
          <div id="mission"><?php print $mission; ?></div>
        <?php endif; ?>

      <?php if ($left): ?>
        <div id="sidebar-left"><div id="sidebar-left-inner">
          <?php print $left; ?>
        </div></div> <!-- /#sidebar-left-inner, /#sidebar-left -->
      <?php endif; ?>

      <?php if ($right): ?>
        <div id="sidebar-right"><div id="sidebar-right-inner">
          <?php print $right; ?>
        </div></div> <!-- /#sidebar-right-inner, /#sidebar-right -->
      <?php endif; ?>
    </div>
    </div></div> <!-- /#main-inner, /#main -->

    <div id="footer"><div id="footer-inner">

      <div id="footer-message"><?php echo $footer_message . $footer ?></div>
    <div style="float:right;"><a href="http://www.avioso.com">Theme port</a> sponsored by Duplika <a href="http://www.duplika.com">Web Hosting</a>.</div>
    <span class="clear"></span>
    </div></div> <!-- /#footer-inner, /#footer -->

    <?php print $closure; ?>

  </div></div> <!-- /#page-inner, /#page -->

</body>
</html>
