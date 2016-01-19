<div id="node-<?php print $node->nid; ?>" class="imageGallery-ng4">

	<h1><?php print $node->title; ?></h1>
	<p class="desc"><?php print $node->field_optdesc['und'][0]['value']; ?></p>

	<div class="imgGallery">
		<?php
		$imgcount = count($node->field_images['und']);
			$k = 0;
		for ($i = 0; $i < $imgcount; $i++) {
		    $image_uri = $node->field_images['und'][$i]['uri'];
			$masthead_raw = image_style_url('gallery-4-column', $image_uri);
			$alt = $node->field_images['und'][$i]['alt'];
		if($k == 0) echo '<div class="rowImages">';
	    		$k++;
			?>
			<a href="<?php print file_create_url($node->field_images['und'][$i]['uri']); ?>" rel="group-<?php print $node->nid; ?>" class="fancybox">
				<?php if(!empty($alt)): ?>
				  	<span class="lens">
				  		<h5><?php print $alt; ?></h5>
				  	</span>
				<?php endif; ?>
			  	<img class="cover" src="<?php print $masthead_raw; ?>" alt="img" />
			</a>
		    <?php 
		    if($k == 4) {
				echo '</div>';
			$k = 0;
		    }
		} ?>
	</div>
</div>