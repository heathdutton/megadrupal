<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>">
<head>
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
  <?php print $styles; ?>
  <?php print $scripts; ?>
</head>
<body class="<?php print $classes; ?>" <?php print $attributes;?>>

<div id="wrapper">

		<div id="header">
      <div id="header-inner">

        <a href="<?php print $front_page ?>" id="logo" rel = "home">
          <?php if ($logo): ?>
            <img src="<?php print $logo; ?>" alt="<?php print $site_name; ?>" title="<?php print $site_name; ?>" />
          <?php endif; ?>
        </a>

        <?php if ($site_name): ?>
          <a href="<?php print $front_page; ?>" id="site-name" title="<?php print $site_name; ?>" rel = "home">
            <?php echo $site_name; ?>
          </a>
        <?php endif; ?>

        <?php if ($site_slogan): ?>
          <div id = "site-slogan">
            <?php echo $site_slogan; ?>
          </div>
        <?php endif; ?>

			</div> <!-- /#header-inner -->
		</div> <!-- /#header -->    

    <div id="container-wrapper" class="clearfix">
      <div id="container" class="clearfix">

        <div id="main">
          <div id = "main-inner">

            <?php if ($title): ?>
              <?php print render($title_prefix); ?>
              <h1><?php print $title; ?></h1>
              <?php print render($title_suffix); ?>
            <?php endif; ?>

            <?php print render($tabs); ?>
            <?php print render($tabs2); ?>
            <?php print $messages; ?>

            <?php print $content; ?>

          </div> <!-- /#main-inner -->
        </div> <!-- /#main -->

      </div> <!-- /#container -->
    </div> <!-- /#container-wrapper -->

</div> <!-- /#wrapper -->

</body>
</html>