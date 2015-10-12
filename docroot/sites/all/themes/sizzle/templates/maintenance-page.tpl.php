<?php
/**
 * @file
 * Default theme implementation to display the maintenance page
 */
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML+RDFa 1.0//EN"
  "http://www.w3.org/MarkUp/DTD/xhtml-rdfa-1.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" version="XHTML+RDFa 1.0" dir="<?php print $language->dir; ?>">
<head>
  <?php print $head; ?>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title><?php print $head_title; ?></title>
  <?php print $styles; ?>
  <?php print $scripts; ?>
</head>
<body class="<?php print $classes; ?>" <?php print $attributes;?>>
  <div id="skip-link">
    <a href="#main-content" class="element-invisible element-focusable"><?php print t('Skip to main content'); ?></a>
  </div>
  <header id="header" class="header" role="header">
    <div class="container">
      <div id="navigation" class="navbar">
        <div class="navbar-inner">
          <div class="container clearfix">
            <?php if ($logo): ?>
              <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo" class="pull-left brand">
                <?php print $site_name; ?>
              </a>
            <?php endif; ?>
        </div>
      </div> <!-- /#navigation -->
    </div>
  </header>

  <div id="main-wrapper">
    <div id="main" class="container">
      <?php if ($title): ?>
        <div class="row-fluid">
          <div class="span12">
            <a id="main-content"></a>
              <div class="page-header clearfix">
                <h1 class="title pull-left" id="page-title"><?php print $title; ?></h1>
                <?php if (isset($steps)): ?>
                  <h5 class="steps pull-right visible-desktop"><?php print $steps; ?></h5>
                <?php endif; ?>
              </div>
          </div>
        </div>
      <?php endif; ?>
      <div class="row-fluid">
        <div class="span3 sidebar visible-desktop">
          <?php print $sidebar_first ?>
        </div>
        <div class="span9 content">
          <?php if (isset($steps)): ?>
            <h5 class="steps visible-phone visible-tablet"><?php print $steps; ?></h5>
          <?php endif; ?>
          <?php if ($messages): ?>
            <div id="messages">
              <?php print $messages; ?>
            </div>
          <?php endif; ?>
          <div id="content">
            <?php print $content; ?>
          </div>
        </div>
      </div>
    </div>
  </div>

  <footer id="footer" class="footer" role="footer">
    <div class="container">
      <div class="row">
        <div class="span12">
          <div class="inner">
            <?php if (isset($copyright)): ?>
              <p class="pull-left"><?php print $copyright ?></p>
            <?php endif; ?>
          </div>
        </div>
      </div>
    </div>
  </footer>
</body>
</html>