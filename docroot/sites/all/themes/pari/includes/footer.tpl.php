<footer id="footer">
	<div class="container">
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
		
		<?php if ($page['footer_four']): ?>	
			<div class="footer-four">
				<?php print render($page['footer_four']); ?>
			</div>
		<?php endif; ?>
	</div><!--/.container -->
</footer>
<div class="clear"></div>
<div id="footer-bottom">
	<div class="container">
		<?php if(theme_get_setting('footer_copyright_show')): ?>
				<?php if (theme_get_setting('footer_copyright_text')=='') { ?>
					<div class="copyright">Copyright &copy; <?php echo date("Y"); ?>, <?php print $site_name; ?></div>
				<?php } else { ?>
					<div class="copyright"><?php echo theme_get_setting('footer_copyright_text'); ?></div>
				<?php };
		endif;
		if (theme_get_setting('social_icons')): ?>
			<div class="social-icons">
				<a href="<?php echo theme_get_setting('twitter_username'); ?>" target="_blank" rel="nofollow"><i class="icon-twitter"></i></a> <a href="<?php echo theme_get_setting('facebook_username'); ?>" target="_blank" rel="nofollow"><i class="icon-facebook"></i></a> <a href="<?php echo theme_get_setting('googleplus_username'); ?>" target="_blank" rel="nofollow"><i class="icon-gplus"></i></a>
			</div>
		<?php endif; ?>
	<div class="clear"></div>
		<?php if ($page['footer']): ?>
			<div class="footer-bottom">
				<?php print render($page['footer']); ?>
			</div><!--/.footer-bottom -->
		<?php endif; ?>
	</div><!--/.container -->
</div><!--/#footer-bottom -->