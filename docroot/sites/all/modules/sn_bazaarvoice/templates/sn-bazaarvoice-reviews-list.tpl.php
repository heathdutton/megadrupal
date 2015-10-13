<?php 
/**
 * @file
 * Template file to render single review box.
 */

?>
<h2><?php print t('Reviews & Ratings');?></h2>
<?php print $reviews_list;?>
<?php if (isset($no_reviews_available)):?>
<h2>
	<?php print $no_reviews_available;?>
</h2>
<?php endif;?>
