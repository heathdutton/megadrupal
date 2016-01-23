<?php

/**
 * @file
 * Template for rendering a the miscellaneous information for a single
 * dsb Portal LOM description.
 *
 * Available variables (none are sanitized):
 * - $description: The \Educa\DSB\Client\Lom\LomDescriptionInterface object to
 *   render.
 * - $table: A pre-rendered HTML table containing all the below information.
 * - $misc_information: Miscellaneous information, like authors and language.
 *   This is an array with the following keys:
 *   - language:s An array of language names in which the resource is available.
 *   - contributors: An array of contributors. Each contributor has the
 *     following keys:
 *     - role: The role of the contributor.
 *     - display_name: The name of the contributor.
 *     - filter_name: An optional name that can be used to filter search results
 *       by this contributor.
 *     - organization_name: An optional organization name this contributor
 *       belongs to.
 *   - version: Version information.
 *   - keywords: An array of keywords.
 *
 * @see dsb_portal_preprocess_dsb_portal_description_misc_info()
 *
 * @ingroup themeable
 */
?>
<div class="dsb-portal-description-misc-information">
  <?php print $table ?>
</div>
