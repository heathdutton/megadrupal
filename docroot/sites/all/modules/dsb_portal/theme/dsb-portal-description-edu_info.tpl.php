<?php

/**
 * @file
 * Template for rendering a the educational information for a single
 * DSB Portal LOM description.
 *
 * Available variables (none are sanitized):
 * - $description: The \Educa\DSB\Client\Lom\LomDescriptionInterface object to
 *   render.
 * - $table: A pre-rendered HTML table containing all the below information.
 * - $edu_information: Educational information, like type of the resource,
 *   intended end users, etc. This array has the following keys:
 *   - description: A description of the resource.
 *   - resource_types: A list of resource types, keyed by their Ontology ID and
 *     the value being the human-readable name.
 *   - intended_user_roles: A list of intended user roles, keyed by their
 *     Ontology ID and the value being the human-readable value.
 *   - age_ranges: A list of intended age ranges.
 *   - duration: A duration value.
 *   - learning_time: The number of lessons necessary.
 *   - difficulty: The difficulty of the resource, pedagogically speaking.
 *
 * @see dsb_portal_preprocess_dsb_portal_description_resource_info()
 *
 * @ingroup themeable
 */
?>
<div class="dsb-portal-description-edu-information">
  <?php print $table ?>
</div>
