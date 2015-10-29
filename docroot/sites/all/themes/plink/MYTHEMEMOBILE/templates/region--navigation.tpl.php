<?php

/**
 * Pretty much the same as region.tpl but no inner class
 */
?>
<?php if ($content): ?>
 	<nav id="main-navigation" class="clearfix" style="clear:both;" data-role="navigation">
		<a id="ank-main-navigation"></a>
		<div class="region-inner" data-role="navbar">
   		<?php print $content; ?>
		</div>
	</nav> <!-- /main-navigation //-->
<?php endif; ?>