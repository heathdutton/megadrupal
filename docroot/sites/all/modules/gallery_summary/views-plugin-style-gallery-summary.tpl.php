<?php
/**
 * @file views-plugin-style-gallery-summary.tpl.php
 * view template to display a group of summary lines with an attached display
 *
 * This wraps items in a span if set to inline, or a div if not.
 *
 * @ingroup views_templates
 */
?>
<?php foreach ($rows as $row):?>
  <?php print (!empty($options['inline']) ? '<span' : '<div') . ' class="views-summary views-gallery-summary">'; ?>
    <?php if (!empty($row->separator)) { print $row->separator; } ?>
    <a href="<?php print $row->url; ?>"><?php print $row->link; ?></a>
    <?php if (!empty($options['count'])): ?>
      (<?php print $row->count; ?>)
    <?php endif; ?>
    <?php if (!empty($row->gallery)): ?>
      <?php print $row->gallery; ?>
    <?php endif; ?>
  <?php print !empty($options['inline']) ? '</span>' : '</div>'; ?>
<?php endforeach; ?>
