<div class="clear"></div>
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
		<?php if ($page['footer']): ?>
			<div class="footer">
				<?php print render($page['footer']); ?>
			</div>
		<?php endif; ?><!-- /end footer block -->
		<div class="clear"></div>
		<?php if (theme_get_setting('footer_copyright')): ?>
			<div id="copyright"><div class="copyright">Copyright &copy; <?php echo date("Y"); ?>, <?php print $site_name; ?></div></div>
		<?php endif; ?>
		<?php if (theme_get_setting('social_icons')): ?>
			<div id="footer-icons">
				<li><a href="<?php echo check_plain (theme_get_setting('facebook_username')); ?>" class="facebook icon" target="_blank"></a></li>
				<li><a href="<?php echo check_plain (theme_get_setting('twitter_username')); ?>" class="twitter icon" target="_blank"></a></li>
			</div>
		<?php endif; ?>
	</div> <!-- /.container -->
</div>