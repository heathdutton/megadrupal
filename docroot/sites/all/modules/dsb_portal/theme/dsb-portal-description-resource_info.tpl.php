<?php

/**
 * @file
 * Template for rendering a the resource information for a single
 * dsb Portal LOM description.
 *
 * Available variables (none are sanitized):
 * - $description: The \Educa\DSB\Client\Lom\LomDescriptionInterface object to
 *   render.
 * - $rights_table: A pre-rendered HTML table containing all the rights and
 *   license information.
 * - $technical_table: A pre-rendered HTML table containing all the technical
 *   information.
 * - $resource_table: A pre-rendered HTML table containing all resource
 *   information.
 * - $resource_information: Information for accessing the resource. This array
 *   has the following keys:
 *   - cost: The cost information.
 *   - license: The licensing information. Warning: this field can contain
 *     HTML!
 *   - format: The format of the resource.
 *   - size: If available, the data size of the resource.
 *   - location: If available, the physical location of the resource.
 *   - requirements: If available, the requirements in order to use the
 *     resource.
 *   - identifiers: A list of identifiers. Each entry has the following keys:
 *     - catalog: The type of entry. Can be URL, DOI or ISBN.
 *     - entry: The value of the identifier. Its format depends on the value
 *       of catalog.
 *     - title: An optional title, mainly used for URL, where it represents the
 *       desired link title.
 *
 * @see dsb_portal_preprocess_dsb_portal_description_resource_info()
 *
 * @ingroup themeable
 */
?>
<div class="dsb-portal-description-resource-information">
  <?php if (!empty($rights_table)): ?>
    <div class="dsb-portal-description-resource-information__rights">
      <h3><?php print t("Rights", array(), array('context' => 'dsb_portal:view')); ?></h3>
      <?php print $rights_table ?>
    </div>
  <?php endif; ?>

  <?php if (!empty($technical_table)): ?>
    <div class="dsb-portal-description-resource-information__technical">
      <h3><?php print t("Technical information", array(), array('context' => 'dsb_portal:view')); ?></h3>
      <?php print $technical_table ?>
    </div>
  <?php endif; ?>

  <?php if (!empty($resource_table)): ?>
    <div class="dsb-portal-description-resource-information__resource">
      <h3><?php print t("To the resource", array(), array('context' => 'dsb_portal:view')); ?></h3>
      <?php print $resource_table ?>
    </div>
  <?php endif; ?>
</div>
