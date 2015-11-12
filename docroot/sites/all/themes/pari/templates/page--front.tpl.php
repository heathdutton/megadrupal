<header id="header">
	<?php include ($directory."/includes/header.tpl.php"); ?>
	<?php if (theme_get_setting('enable_slider')): ?>
		<?php include ($directory."/includes/slider.tpl.php"); ?>
	<?php endif; ?>
	<div class="scroll"><div class="arrow-down jump-icon"></div></div>
</header><!--/header -->
<section id="home-content" role="main" class="page-content">
	<div id="homepage-header">
		<div class="container">
			<?php print $messages; ?>
			<?php if ($page['homepage_header']): ?><!-- / start homepage header block -->
				<div class="homepage-header">
					<?php print render($page['homepage_header']); ?>
				</div> <!-- / end homepage header -->
			<?php endif; ?>
			<div class="clear"></div>
		</div><!--/.container -->
	</div><!--/#homepage-header -->	
	<div class="container">
	<?php if ($page['homepage_one']): ?>
		<div class="one_three">
			<?php print render($page['homepage_one']); ?>
		</div> <!-- / end homepage first block -->
	<?php endif; ?>

	<?php if ($page['homepage_two']): ?>
		<div class="one_three">
			<?php print render($page['homepage_two']); ?>
		</div> <!-- / end homepage second block -->
	<?php endif; ?>

	<?php if ($page['homepage_three']): ?>
		<div class="one_three_last">
			<?php print render($page['homepage_three']); ?>
		</div> <!-- / end homepage third block -->
	<?php endif; ?>
	</div> <!--/.container -->
	<div class="clear"></div>
		<?php if ($page['homepage_content']): ?>
		<div class="container homepage-content">
			<?php print render($page['homepage_content']); ?>
		</div> <!-- / .container -->
	<?php endif; ?>
</section> <!--/section -->
<?php include ($directory."/includes/footer.tpl.php"); ?>
