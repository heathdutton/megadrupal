<?php
/**
 * @file
 * Default simple view template to display a list of rows.
 *
 * - $title : The title of this group of rows.  May be empty.
 * - $options['type'] will either be ul or ol.
 * @ingroup views_templates
 */
?>
<?php print $wrapper_prefix; ?>
<?php if (!empty($title)) : ?>
  <h3><?php print $title; ?></h3>
<?php endif; ?>
<ul class="<?php print $list_class; ?>">
<?php foreach ($rows as $id => $row): ?>
  <li id="<?php print $row['id']; ?>" <?php if (!empty($row['classes'])): ?>class="<?php print $row['classes']; ?>"<?php endif;?>>
    <div class="elastic-grid-thumbnail">
      <a class="veg-thumbnail-link" href="#<?php print $row['id']; ?>"><?php print $row['thumbnail']; ?></a>
      <div class="veg-thumbnail-pointer"></div>
    </div>
    <div class="elastic-grid-expanded"><div class="elastic-grid-expanded-inner"><?php print $row['expanded']; ?></div></div>
  </li>
<?php endforeach; ?>
</ul>
<?php print $wrapper_suffix; ?>
