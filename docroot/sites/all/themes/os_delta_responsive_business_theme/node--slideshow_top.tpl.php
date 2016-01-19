<div class="slider1"> 
		<i id="arrow-left1" class="fa fa-chevron-circle-left"></i>
		<i id="arrow-right1" class="fa fa-chevron-circle-right"></i>
	<div class="swiper-container1">
		<div id="node-<?php print $node->nid; ?>" class="swiper-wrapper">
		<?php
			if (!empty($node->field_body_slider_top['und'])) {
				$imgcount = count($node->field_body_slider_top['und']);
				for ($i = 0; $i < $imgcount; $i++) {
					echo '<div class="block-imageblock">';
					print $node->field_body_slider_top['und'][$i]['value'];
					echo '</div>';
	    		}
			}
		?>
		</div>
	</div> <!--swiper-container1-->
		<div id="pagination1"></div>
</div> <!--slider1-->