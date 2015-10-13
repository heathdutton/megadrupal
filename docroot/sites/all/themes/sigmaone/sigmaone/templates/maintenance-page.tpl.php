<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>">
<head>
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
  <?php print $styles; ?>
  <?php print $scripts; ?>
</head>

<body class="<?php print $classes ?>">
<div id="page" class="clearfix">
	<!-- HEADER AREA -->
	<div id="header" class="row newrow lastrow twelvecol clearfix">
		<div id="logo" class="left">
		<?php print '<a href="' .  $base_path . '" title="' . t('Home') . '"><img class="logo" src="' .  $logo  . '" alt="' .  t('Home') . '" /></a>'; ?>
		</div>
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
	</div>


	<!-- MAIN CONTENT AREA -->

	<div id="content" class="row newrow lastrow twelvecol clearfix">
			<!-- ARTICLE AREA -->

			<div id="region-content" class="newrow lastrow twelvecol clearfix">
				<!-- MESSAGES -->
			  <?php if (!empty($messages)) : ?>
				<?php print $messages; ?>
				<?php endif; ?>

				<!-- TITLE -->
				<?php if ($title) : ?>
				<h1 class="relative main-title">
				<?php print $title;?>
				</h1>
				<?php endif; ?>

 			  <!-- MAIN CONTENT AREA -->
				<?php if(!empty($content)) :?>
				  <?php print $content;?>
				<?php endif;?>

			</div>
			<!--article-->
		</div>
	<!-- end main content -->

	<!-- FULL FOOTER AREA -->
	<div id="fullfooter" class="row newrow lastrow twelvecol clearfix">
			<p>
				&copy; 2013, SigmaOne Theme - All rights reserved, by <a
					href="http://www.victheme.com">VicTheme.com</a>
			</p>
	</div>


	</div><!--/page -->
</body>
</html>