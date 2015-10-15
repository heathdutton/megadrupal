<?php
/**
 * When no tags are found, we give back an empty template.
 * There is nothing to show then.
 * 
 * $tags is empty if the http call didn't work or when we didn't
 * find a title or a description.
 * 
 * see template_preprocess_opengraph_filter.
 */
if (!count($tags)) {
  return;
}
?>
<div class="<?php print $classes; ?>"> 
  <div class="opengraph-filter-inner clearfix">
  <?php if (!empty($tags['image'])): ?>
  <div class="left">
    <?php print $image; ?>
  </div>
  <div class="right">
  <?php endif; ?>
  <?php if (!empty($tags['title'])): ?>
  <div class="title">
    <a href="<?php print $tags['url']; ?>" title="<?php print $tags['title']; ?>"><?php print $tags['title']; ?></a>
  </div>
  <?php endif; ?>
  <?php if (!empty($tags['description'])): ?>
  <div class="description">
    <a href="<?php print $tags['url']; ?>"<?php print !empty($tags['title'])?' title="' . $tags['title'] . '"':''?>><?php print $tags['description']; ?></a>
  </div>
  <?php endif; ?>
  <?php if (!empty($tags['image'])): ?>
  </div>
  <?php endif; ?>
  </div>
</div>