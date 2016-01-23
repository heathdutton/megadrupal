<?php

/**
 * @file
 * Template for rendering a the relationship information for a single
 * dsb Portal LOM description.
 *
 * Available variables (none are sanitized):
 * - $description: The \Educa\DSB\Client\Lom\LomDescriptionInterface object to
 *   render.
 * - $table: A pre-rendered HTML table containing all the below information.
 *
 * @see dsb_portal_preprocess_dsb_portal_description_relationship_info()
 *
 * @ingroup themeable
 */
?>
<div class="dsb-portal-description-relationship-information">
  <?php print $table ?>
</div>
