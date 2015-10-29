<?php

/**
 * @file footer.php
 * A footer file converted from the WordPress Whiteboard theme framework.
 */
?>
	<div class="clear"></div>
	</div><!--.container-->
	<div id="footer"><footer>
		<div class="container">
			<div id="footer-content">
        <?php print $footer_message; ?>
        <?php if (!empty($footer)): print $footer; endif; ?>
				<div id="nav-footer" class="nav"><nav>
          <?php if (!empty($footer_menu)): print $footer_menu; endif; ?>
				</nav></div><!--#nav-footer-->
				<p class="clear"><a href="#main">Top</a></p>
				<p>Built using a Drupal port of the <a href="http://whiteboardframework.com/">Whiteboard Framework for Wordpress</a> <span class="amp">&amp;</span> <a href="http://lessframework.com">Less Framework</a>.</p><?php /* Whiteboard Framework is free to use. You are only required to keep a link in the CSS. We do not require a link on the site, though we do greatly appreciate it. Likewise, Less Framework is free to use. Links are not required on the website or in the CSS but are greatly appreciated. */ ?>
			</div><!--#footer-content-->
		</div><!--.container-->
	</footer></div><!--#footer-->
</div><!--#main-->
<?php print $closure ?>
</body>
</html>
