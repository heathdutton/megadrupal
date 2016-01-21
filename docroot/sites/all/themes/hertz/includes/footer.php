<div class="clear"></div>
<footer id="footer" role="contentinfo">
	<div class="wrap">	
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
	<div class="wrap">	
		<?php if (theme_get_setting('footer_copyright')): ?>
			<div id="copyright">Copyright &copy; <?php echo date("Y"); ?>, <?php print $site_name; ?></div>
		<?php endif; ?>
		<?php print render($page['footer']) ?>
	</div> <!-- /.wrap -->
</div> <!-- /#footer-bottom -->