<?php

/**
 * @file
 * Groundwork Framework's Implementation to display a single Drupal page while offline.
 *
 * All the available variables are mirrored in page.tpl.php.
 *
 * @see template_preprocess()
 * @see template_preprocess_maintenance_page()
 */
?>
<!DOCTYPE html>
<!--[if IEMobile 7]><html class="iem7" lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>"><![endif]-->
<!--[if lte IE 6]><html class="lt-ie10 lt-ie9 lt-ie8 lt-ie7"  lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>"><![endif]-->
<!--[if (IE 7)&(!IEMobile)]><html class="lt-ie10 lt-ie9 lt-ie8 ie7" lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>"><![endif]-->
<!--[if IE 8]><html class="lt-ie10 lt-ie9 ie8"  lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>"><![endif]-->
<!--[if IE 9]><html class="lt-ie10 ie9"  lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>"><![endif]-->
<!--[if (gte IE 10)|(gt IEMobile 7)]><!--><html  lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>"><!--<![endif]-->

<head>
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>

  <meta name="MobileOptimized" content="width">
  <meta name="HandheldFriendly" content="true">
  <meta name="viewport" content="width=device-width">
  <?php print $styles; ?>
  <?php print $scripts; ?>
  <!-- IE Fix for HTML5 Tags -->
  <!--[if lt IE 9]>
  <script src="http://html5shiv.googlecode.com/svn/trunk/html5.js"></script>
  <![endif]-->
</head>

<body class="<?php print $classes; ?>" <?php print $attributes;?>>
  <div id="skip-link">
    <a href="#main-content" class="visually-hidden focusable"><?php print t('Skip to main content'); ?></a>
  </div>

<div id="page">

  <div id="header-wrapper" class="wrapper">
    <header id="header">
        <div id="branding" role="banner">
          <?php if ($logo): ?>
            <a id="site-logo" href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home">
              <img src="<?php print $logo; ?>" alt="<?php print t('Site logo'); ?>">
            </a>
          <?php endif; ?>
          <?php if ($site_name): ?>
            <?php if ($title): ?>
              <p id="site-name">
                <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home">
                  <?php print $site_name; ?>
                </a>
              </p>
            <?php else: /* Use h1 when the content title is empty */ ?>
              <h1 id="site-name">
                <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home">
                  <?php print $site_name; ?>
                </a>
              </h1>
            <?php endif; ?>
          <?php endif; ?>
          <?php if ($site_slogan): ?>
            <p id="site-slogan">
              <?php print $site_slogan; ?>
            </p>
          <?php endif; ?>
        </div><!--/#branding -->
       <div id="header-annex" class="region region-header-annex">
  </div><!-- /#header-annex -->
    </header><!--/#header -->
  </div><!--/#header-wrapper -->

  <div id="main-wrapper" class="wrapper">
    <div id="main-container">
        <main id="main" role="main">
          <?php print $messages; ?>
          <?php if ($title): ?>
           <h1 class="title page-title">
              <?php print $title; ?>
            </h1>
          <?php endif; ?>
          <?php print $content; ?>
        </main><!--/#main -->
    </div><!--/#main-container -->
  </div><!--/#main-wrapper -->

  <?php print $theme_attribution; ?>

</div><!--/#page -->

</body>
</html>
