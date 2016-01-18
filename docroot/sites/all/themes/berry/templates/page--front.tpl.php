<div id="header-top">
	<div class="container" class="clearfix">
		<?php if ($logo): ?>
		  <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" id="logo">
			<img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
		  </a>
		<?php endif; ?>
		<div id="social">
			<?php if (theme_get_setting('social_icons')): ?>
				<li><a href="<?php echo theme_get_setting('linkedin_username'); ?>" class="linkedin icon" target="_blank"></a></li>
				<li><a href="<?php echo theme_get_setting('googleplus_username'); ?>" class="googleplus icon" target="_blank"></a></li>
				<li><a href="<?php echo theme_get_setting('twitter_username'); ?>" class="twitter icon" target="_blank"></a></li>
				<li><a href="<?php echo theme_get_setting('facebook_username'); ?>" class="facebook icon" target="_blank"></a></li>
			<?php endif; ?>
				<li>
					<?php if ($page['search_box']): ?><!-- start search box region -->
						<div class="search-box">
							<?php print render($page['search_box']); ?>
						</div> <!-- end search box region -->
					<?php endif; ?>
				</li>
		</div> <!-- end #social -->
	</div> <!-- /.container -->
</div> <!-- /#header-top -->
<nav id="navigation" class="navigation">
	<div class="container clearfix">
		<?php 
		$main_menu_tree = menu_tree(variable_get('menu_main_links_source', 'main-menu')); 
		print drupal_render($main_menu_tree);
		?>
	</div>
</nav> <!-- /#navigation -->
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
<footer id="footer" role="contentinfo" class="clearfix">
	<div class="container clearfix">	
		<?php if ($page['footer_one']): ?>
			<div class="footer-one">
				<?php print render($page['footer_one']); ?>
			</div>
		<?php endif; ?>
		
		<?php if ($page['footer_two']): ?>
			<div class="footer-two">
				<?php print render($page['footer_two']); ?>
			</div>
		<?php endif; ?>
		
		<?php if ($page['footer_three']): ?>	
			<div class="footer-three">
				<?php print render($page['footer_three']); ?>
			</div>
		<?php endif; ?>
	</div> <!-- /.container -->
</footer> <!-- /#footer -->
<div class="clear"></div>
<div id="footer-bottom">
	<div class="container" class="clearfix">	
		<?php if (theme_get_setting('footer_copyright')): ?>
			<div id="copyright">Copyright &copy; <?php echo date("Y"); ?>, <?php print $site_name; ?></div>
		<?php endif; ?>
		<?php print render($page['footer']) ?>
	</div> <!-- /.container -->
</div>

