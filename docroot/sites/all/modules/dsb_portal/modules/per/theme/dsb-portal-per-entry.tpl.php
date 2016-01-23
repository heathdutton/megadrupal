<?php

/**
 * @file
 * Template for rendering a single dsb Portal PER entry.
 *
 * Available variables (if not specifically noted, none are sanitized):
 * - $discipline: The discipline, or course.
 * - $object: The subject of the course.
 * - $code: The PER curriculum identifier.
 * - $url_part: If provided, the URL part for linking to the official PER site.
 * - $description: The course description.
 *
 * @ingroup themeable
 */
?>
<dl class="dsb-portal-per-entry">
  <dt class="dsb-portal-per-entry__identifier">
    <?php print dsb_portal_per_theme_per_curriculum_code_filter_link($code); ?>:
  </dt>

  <dd class="dsb-portal-per-entry__info">
    <div class="dsb-portal-per-entry__info__subject">
      <?php print check_plain($discipline); ?><?php if (!empty($object)): ?>,<?php endif; ?>

      <?php if (!empty($object)) {
        print check_plain($object);
      } ?>
    </div>

    <div class="dsb-portal-per-entry__info__description">
      <?php print check_plain($description); ?>
    </div>

    <div class="entry__info__link">
      <?php print dsb_portal_per_theme_per_site_link(isset($url_part) ? $url_part : NULL); ?>
    </div>
  </dd>
</dl>
