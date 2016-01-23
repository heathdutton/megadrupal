<?php

/**
 * Pretty much the same as region.tpl but no inner class
 */
?>
<?php if ($content): ?>
 	<nav id="main-navigation" class="<?php print $classes; ?> clearfix" role="navigation">
		<a id="ank-main-navigation"></a>
		<div class="region-inner <?php print $inner_classes; ?>">
   		<?php print $content; ?>
		</div>
	</nav> <!-- /main-navigation //-->
<?php endif; ?>