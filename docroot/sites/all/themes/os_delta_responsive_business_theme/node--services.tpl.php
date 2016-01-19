<div id="node-<?php print $node->nid; ?>" class="ourPrice">

	<h2><?php print $node->field_price_services['und'][0]['value']; ?></h2>
	<i class="fa <?php print $node->field_icon_name['und'][0]['value']; ?>"></i>
	<h4><?php print $title; ?></h4>

	<?php
		if (!empty($node->field_list_services['und'])) {
			$imgcount = count($node->field_list_services['und']);
			for ($i = 0; $i < $imgcount; $i++) {
				echo '<p>';
				print $node->field_list_services['und'][$i]['value'];
				echo '</p>';
	    	}
		}
	?>

	<a class="readMore" href="<?php print $node->field_button_link['und'][0]['value']; ?>">
		<?php print $node->field_button_name['und'][0]['value']; ?>
	</a>
</div>