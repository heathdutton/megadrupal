<?php
/**
 * @file
 * 	Template for Similar Products Panels plugin
 */

if ($item->SimilarProducts->SimilarProduct): ?>
<div class="SimilarProducts" >
<ul>
<?php foreach ($item->SimilarProducts->SimilarProduct as $sim): ?>
	<li><a rel="nofollow" href="<?php print url(AMAZON_STORE_PATH ."/item/{$sim->ASIN}") ?>"><?php print $sim->Title ?></a></li>
<?php endforeach; ?>
</ul>
</div>
<?php endif; ?>