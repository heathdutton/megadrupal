<?php

/**
 * @file
 * To use Colourise maintenance page you have to edit your "settings.php" file.
 * Please read the theme "README.txt" file for more informations.
 */

?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">

  <head>

    <title><?php print $head_title ?></title>
    <?php print $head ?>
    <?php print $styles ?>
    <?php print $scripts ?>

    <!--[if lte IE 6]>
      <link type="text/css" rel="stylesheet" media="all" href="<?php print $base_path . $directory ?>/css/ie6.css" />
    <![endif]-->

    <?php if (theme_get_setting('colourise_iepngfix')) : ?>
    <!--[if lte IE 6]>
      <script type="text/javascript"> 
        $(document).ready(function(){ 
          $(document).pngFix(); 
        }); 
      </script> 
    <![endif]-->
    <?php endif; ?>

  </head>

  <body class="<?php print $body_classes ?>">

    <div id="page">

      <div id="header" class="clearfix">

      <?php if ($site_name): ?>
        <h1 id="site-name">
          <a href="<?php print check_url($front_page) ?>" title="<?php print t('Home') ?>">
            <?php print $site_name ?>
          </a>
        </h1>
      <?php endif; ?>

      <?php if ($site_slogan): ?>
        <p id="slogan"><?php print $site_slogan ?></p>
      <?php endif; ?>

      </div> <!-- /header -->

      <div id="content-inner" class="inner">

        <?php if ($title): ?>
          <h1 class="title"><?php print $title ?></h1>
        <?php endif; ?>

        <?php if (!empty($messages)): ?><?php print $messages ?><?php endif; ?>

        <?php if (!empty($help)): ?><?php print $help ?><?php endif; ?>

        <div id="maintainance-content">
          <?php print $content ?>
        </div>

      </div> <!-- /content-inner -->

      <div id="footer" class="clearfix">

        <?php if ($footer_message): ?>
          <p id="site-info"><?php print $footer_message ?></p>
        <?php endif; ?>

        <?php print $closure ?>
      </div> <!-- /footer -->

    </div> <!-- /page -->
  </body>
</html>