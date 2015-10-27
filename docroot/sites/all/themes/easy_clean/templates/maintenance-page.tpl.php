<?php

/**
 * @file
 * Default theme implementation to display a single Drupal page while offline.
 *
 * All the available variables are mirrored in html.tpl.php and page.tpl.php.
 * Some may be blank but they are provided for consistency.
 *
 * @see template_preprocess()
 * @see template_preprocess_maintenance_page()
 */

$html_attributes = ' lang="' . $language->language . '" dir="' . $language->dir . '"';
?>
<!DOCTYPE html>
<!--[if IE 7 ]><html<?php print $html_attributes; ?> class="no-js ie7"><![endif]-->
<!--[if IE 8 ]><html<?php print $html_attributes; ?> class="no-js ie8"><![endif]-->
<!--[if IE 9 ]><html<?php print $html_attributes; ?> class="no-js ie9"><![endif]-->
<!--[if gt IE 9]><!--><html<?php print $html_attributes; ?> class="no-js"><!--<![endif]-->
<head>
<?php print $head; ?>
  
<!-- Always force latest IE rendering engine (even in intranet) & Chrome Frame  -->
<meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">

<!--  Mobile viewport optimized: j.mp/bplateviewport -->
<meta name="viewport" content="width=device-width, initial-scale=1.0">

<!-- Prevent blocking -->
<!--[if IE 6]><![endif]-->

<title><?php print $head_title; ?></title>
<?php print $styles; ?>
<?php print $scripts; ?>

<!--[if lt IE 9]>
  <script src="http://html5shiv.googlecode.com/svn/trunk/html5.js"></script>
<![endif]-->

</head>
<body class="<?php print $classes; ?>" <?php print $attributes;?>>

  <div id="page">
    <div id="header">
      <div id="logo-title">

        <?php if (!empty($logo)): ?>
          <a href="<?php print $base_path; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
            <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
          </a>
        <?php endif; ?>

        <div id="name-and-slogan">
          <?php if (!empty($site_name)): ?>
            <h1 id="site-name">
              <a href="<?php print $base_path ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
            </h1>
          <?php endif; ?>

          <?php if (!empty($site_slogan)): ?>
            <div id="site-slogan"><?php print $site_slogan; ?></div>
          <?php endif; ?>
        </div> <!-- /name-and-slogan -->
      </div> <!-- /logo-title -->

      <?php if (!empty($header)): ?>
        <div id="header-region">
          <?php print $header; ?>
        </div>
      <?php endif; ?>

    </div> <!-- /header -->

    <div id="container" class="clearfix">

      <?php if (!empty($sidebar_first)): ?>
        <div id="sidebar-first" class="column sidebar">
          <?php print $sidebar_first; ?>
        </div> <!-- /sidebar-first -->
      <?php endif; ?>

      <div id="main" class="column"><div id="main-squeeze">

        <div id="content">
          <?php if (!empty($title)): ?><h1 class="title" id="page-title"><?php print $title; ?></h1><?php endif; ?>
          <?php if (!empty($messages)): print $messages; endif; ?>
          <div id="content-content" class="clearfix">
            <?php print $content; ?>
          </div> <!-- /content-content -->
        </div> <!-- /content -->

      </div></div> <!-- /main-squeeze /main -->

      <?php if (!empty($sidebar_second)): ?>
        <div id="sidebar-second" class="column sidebar">
          <?php print $sidebar_second; ?>
        </div> <!-- /sidebar-second -->
      <?php endif; ?>

    </div> <!-- /container -->

    <div id="footer-wrapper">
      <div id="footer">
        <?php if (!empty($footer)): print $footer; endif; ?>
      </div> <!-- /footer -->
    </div> <!-- /footer-wrapper -->

  </div> <!-- /page -->

</body>
</html>
