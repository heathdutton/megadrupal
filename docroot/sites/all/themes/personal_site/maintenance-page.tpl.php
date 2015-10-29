<?php
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
  <head>
    <title><?php print $head_title; ?></title>
    <?php print $head; ?>
    <?php print $styles; ?>
    <?php print $scripts; ?>
  </head>
  <body class="<?php print $classes; ?>">

    <div id="page-wrapper">
      <div id="main-page">

        <div id="header"><div class="section clearfix">

            <div id="logo-area">
              <?php print $feed_icons; ?>
              <?php if ($logo): ?>
                <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
                  <img height="50px" src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
                </a>
              <?php endif; ?>

              <?php if ($site_name || $site_slogan): ?>
                <div id="name-and-slogan">
                  <?php if ($site_name): ?>
                    <?php if ($title): ?>
                      <div id="site-name">
                        <strong>
                          <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
                        </strong>
                      </div>
                    <?php else: /* Use h1 when the content title is empty */ ?>
                      <h1 id="site-name">
                        <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
                      </h1>
                    <?php endif; ?>
                  <?php endif; ?>

                  <?php if ($site_slogan): ?>
                    <div id="site-slogan"><?php print $site_slogan; ?></div>
                  <?php endif; ?>
                </div> <!-- /#name-and-slogan -->
              <?php endif; ?>
            </div>  
          </div></div> <!-- /.section, /#header -->

        <div id="main-wrapper"><div id="main" class="clearfix">
            <?php if ($tabs): ?><div class="tabs"><?php print render($tabs); ?></div><?php endif; ?>
            <?php print render($page['help']); ?>

            <div id="content" class="column"><div class="section">
                <?php if ($title): ?><h1 class="title" id="page-title"><?php print $title; ?></h1><?php endif; ?>
                <?php print render($title_suffix); ?>
                <?php if ($messages): ?>
                  <div id="console"><?php print $messages; ?></div>
                <?php endif; ?>
                <?php if ($help): ?>
                  <div id="help">
                    <?php print $help; ?>
                  </div>
                <?php endif; ?>
                <p>
                  <?php print $content; ?>
                </p>
              </div></div> <!-- /.section, /#content -->
          </div></div> <!-- /#main, /#main-wrapper -->

        <div id="footer"><div class="section">
          <?php print render($page['footer']); ?>
        </div></div> <!-- /.section, /#footer -->

      </div>
    </div>

  </body>
</html>
