<?php 
/**
 * @file
 * Template file to render single review box.
 */
?>
<div class='review-item'>
	<div class='left'>
		<div class='rating'>
			<?php print theme('rating_widget', array('rating_value' => $review['rating'], 'full_node_class' => ''));?>
		</div>
		<div class='author'><?php print $review['author']?></div>
		<div class='submitted'><?php print $review['submission']?></div>
	</div>
	<div class='right'>
		<h2><?php print $review['title']?></h2>
		<div class='review-text'><?php print $review['review_text']?></div>
	</div>
</div>
