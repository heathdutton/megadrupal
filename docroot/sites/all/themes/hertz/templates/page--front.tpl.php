<div id="content">
<?php include ($directory."/includes/header.php"); ?>
<!-- START SLIDER -->
<?php if (theme_get_setting('home_slider')): ?>
<div class="container clearfix">
<div id="slider">
	<div class="main_view">
		<div class="window">
			<div class="image_reel">
				<img src="<?php echo theme_get_setting('slider_one_image'); ?>" />
				<img src="<?php echo theme_get_setting('slider_two_image'); ?>" />
				<img src="<?php echo theme_get_setting('slider_three_image'); ?>" />
			</div>
			<div class="slider-title">
				<div class="slidertitle"><?php echo theme_get_setting('slider_one_title'); ?></div>
				<div class="slidertitle"><?php echo theme_get_setting('slider_two_title'); ?></div>
				<div class="slidertitle"><?php echo theme_get_setting('slider_three_title'); ?></div>
			</div>
			<div class="slider-text">
				<div class="slidertext" style="display: none;"><?php echo theme_get_setting('slider_one_desc'); ?></div>
				<div class="slidertext" style="display: none;"><?php echo theme_get_setting('slider_two_desc'); ?></div>
				<div class="slidertext" style="display: none;"><?php echo theme_get_setting('slider_three_desc'); ?></div>
			</div>
		</div>        
		<div class="paging">
			<a rel="1" href="#">1</a>
			<a rel="2" href="#">2</a>
			<a rel="3" href="#">3</a>
		</div>
	</div>
</div> <!-- end slider -->
</div>
<?php endif; ?>
<div class="clear"></div>
<!-- END SLIDER -->
<div class="container clearfix">
	<?php print $messages; ?>
	<?php if ($page['homepage_header']): ?><!-- / start homepage header block -->
		<div class="page-header">
			<?php print render($page['homepage_header']); ?>
		</div> <!-- / end homepage header -->
		<div class="clear"></div>
	<?php endif; ?>
	<div class="full">
	<?php if ($page['homepage_one']): ?><!-- / start homepage first block -->
		<div class="one_three">
			<?php print render($page['homepage_one']); ?>
		</div> <!-- / end homepage first block -->
	<?php endif; ?>

	<?php if ($page['homepage_two']): ?><!-- / start homepage second block -->
		<div class="one_three">
			<?php print render($page['homepage_two']); ?>
		</div> <!-- / end homepage first block -->
	<?php endif; ?>


	<?php if ($page['homepage_three']): ?><!-- / start homepage Third block -->
		<div class="one_three_last">
			<?php print render($page['homepage_three']); ?>
		</div> <!-- / end homepage first block -->
	<?php endif; ?>
	</div>
	<div class="clear"></div>
		<?php if ($page['homepage_content']): ?><!-- / start homepage content block -->
		<div class="full">
			<?php print render($page['homepage_content']); ?>
		</div> <!-- / end homepage content block -->
	<?php endif; ?>
	<div class="clear"></div>
</div> <!-- /.container -->
</div> <!-- /#content -->
<?php include ($directory."/includes/footer.php"); ?>