<?php

/**
 * @file
 * Template for rendering a single dsb Portal LOM description.
 *
 * Available variables (if not specifically noted, none are sanitized):
 * - $description: The \Educa\DSB\Client\Lom\LomDescriptionInterface object to
 *   render.
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
 *     - version: Version information.
 *   - keywords: An array of keywords.
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
 * - $classification_information: An array of information about classification.
 *   Each item is part of a curriculum ("Plan d'étude", or "Lehrplan"). Each
 *   item is can have the following keys:
 *   - source: The source of the curriculum. "educa" is the default and
 *     standard curriculum. "per" is the PER curriculum, etc.
 *   - educational level: (optional) The educational level data. Not all
 *     curricula provide this information.
 *   - discipline: (optional) The disciplines covered, with hierarchical
 *     information based on the educational level. Not all curricula
 *     provide this information.
 *   Each entry in the above trees has the following keys:
 *   - entry: A list of human-readable data, keyed by language key. In some
 *     cases, the data is an array with meta-data, but in most cases it is
 *     simply a string.
 *   - children: An array of sub-entries, which have the same keys.
 *
 * @see dsb-portal-description-misc_info.tpl.php
 * @see dsb-portal-description-resource_info.tpl.php
 * @see dsb-portal-description-relationship_info.tpl.php
 * @see dsb-portal-description-classification_info.tpl.php
 * @see dsb-portal-description-edu_info.tpl.php
 *
 * @ingroup themeable
 */
?>
<div class="dsb-portal-description">
  <div class="dsb-portal-descrpition__partner-data">
    <?php print dsb_portal_theme_owner_filter_link($description, TRUE); ?>
  </div>

  <div class="dsb-portal-description__information">
    <?php if ($description->getPreviewImage()): ?>
      <div class="dsb-portal-description__information__preview-image">
        <?php print theme('image', array(
          'path' => $description->getPreviewImage(),
          'alt' => t("Preview image for LOM object @title", array(
            '@title' => $description->getTitle(),
          )),
        )); ?>

        <?php if ($copyright = $description->getField('technical.previewImage.copyright')): ?>
          <span class="dsb-portal-description__information__preview-image__copyright">
            <?php print check_plain($copyright); ?>
          </span>
        <?php endif; ?>
      </div>
    <?php endif; ?>

    <div class="dsb-portal-description__information__body">
      <?php print strip_tags($description->getDescription()); ?>
    </div>
  </div>

  <div class="dsb-portal-description__data">
    <?php if (!empty($misc_information)): ?>
      <div class="dsb-portal-description__data__misc-information">
        <h2><?php print t("General information", array(), array('context' => 'dsb_portal:view')); ?></h2>

        <?php print theme('dsb_portal_description_misc_info', array(
          'description' => $description,
          'misc_information' => $misc_information,
        )); ?>
      </div>
    <?php endif; ?>

    <?php if (!empty($edu_information)): ?>
      <div class="dsb-portal-description__data__edu-information">
        <h2><?php print t("Educational information", array(), array('context' => 'dsb_portal:view')); ?></h2>

        <?php print theme('dsb_portal_description_edu_info', array(
          'description' => $description,
          'edu_information' => $edu_information,
        )); ?>
      </div>
    <?php endif; ?>

    <?php if (!empty($classification_information)): ?>
      <div class="dsb-portal-description__data__classification-information">
        <h2><?php print t("Curricula", array(), array('context' => 'dsb_portal:view')); ?></h2>

        <?php print theme('dsb_portal_description_classification_info', array(
          'description' => $description,
          'classification_information' => $classification_information,
        )); ?>
      </div>
    <?php endif; ?>

    <?php if (!empty($resource_information)): ?>
      <div class="dsb-portal-description__data__resource-information">
        <h2><?php print t("Resource information", array(), array('context' => 'dsb_portal:view')); ?></h2>

        <?php print theme('dsb_portal_description_resource_info', array(
          'description' => $description,
          'resource_information' => $resource_information,
        )); ?>
      </div>
    <?php endif; ?>

    <?php if (!empty($relationship_information)): ?>
      <div class="dsb-portal-description__data__relationship-information">
        <h2><?php print t("Relationships", array(), array('context' => 'dsb_portal:view')); ?></h2>

        <?php print theme('dsb_portal_description_relationship_info', array(
          'description' => $description,
          'relationship_information' => $relationship_information,
        )); ?>
      </div>
    <?php endif; ?>
  </div>
</div>
