<!DOCTYPE html>
<html lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>">
<head>
  <?php print $head; ?>
  <meta name="viewport" content="width=device-width" />
  <title><?php print $head_title; ?></title>
  <?php print $styles; ?>
  <?php print $scripts; ?>
  <!-- IE Fix for HTML5 Tags -->
  <!--[if lt IE 9]>
    <script src="http://html5shiv.googlecode.com/svn/trunk/html5.js"></script>
  <![endif]-->
</head>

<body class="<?php print $classes; ?>" <?php print $attributes;?>>
<div id="maintenance-page">
	<div class="container" class="clearfix">
		<header role="banner" class="clearfix">
			<div id="logo">
			<?php if (theme_get_setting('site_logo') == 'logo_image') { ?>
			<a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home">
			<img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
			</a>
			<?php } elseif (theme_get_setting('site_logo') == 'logo_text') { ?>
			<a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><h1><?php print $site_name; ?></h1></a>
			<?php if ($site_slogan): ?><div id="site-slogan"><?php print $site_slogan; ?></div><?php endif; ?>
			<?php } ?>
			</div>
		</header> <!-- /#header -->

	  <section id="maintenance" role="main">
	  <i class="icon-cog-alt"></i>
	  <br />
		<?php print $content; ?>
	  </section> <!-- /#main -->
	  
	</div> <!-- /.container -->
</div> <!-- /#maintenance -->
</body>
</html>