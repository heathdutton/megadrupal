<?php
/**
 * @file
 * Displays the items of the views universal accordion.
 *
 * @ingroup views_templates
 *
 * Note that the accordion NEEDS <?php print $row ?> to be wrapped by an
 * element, or it will hide all fields on all rows under the first field.
 */
?>

<!-- views-view-universal-accordion.tpl -->
<ol class="top-slider">
	<?php foreach ($rows as $id => $row): ?>
	<li>
		<?php print $row; ?>
	</li>
	<?php endforeach; ?>
</ol>
<!-- // views-view-universal-accordion.tpl -->
