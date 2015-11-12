<?php
/**
 * @file
 * Block content template for linkedin group posts
 *
 * @param object $response
 * @param string $group_id
 * @param string $image_path
 */

?>
<div class="founder-forum-container">
  <?php if ($response): global $base_url;
    $counter = 1;
    $class = 'row'; ?>
      <?php if (isset($response->post)) : ?>
        <?php foreach ($response->post as $key => $value) : ?>
          <?php if (sizeof($response->post) == $counter) : ?>
            <?php $class = 'last'; ?>
          <?php endif; ?>
          <div class="founder-forum-row clear <?php print $class; ?>">
          <?php if (!empty($value->creator->{'picture-url'})) : ?>  
            <div class="float-l founder-forum-image"><?php print theme('image', array('path' => $value->creator->{'picture-url'})) ?></div>
          <?php else: ?>
            <div class="float-l founder-forum-image"><?php print theme('image', array('path' => $image_path)) ?></div>
          <?php endif; ?>
          <div class="float-l founder-forum-data">
          <?php print l($value->title, $value->{'site-group-post-url'}, array('attributes' => array('target' => '_blank'))); ?>
          <?php if (isset($value->summary)): ?> <p><?php print check_plain($value->summary) ?></p><?php endif; ?>
          </div>
          <?php $counter++; ?>
          </div>
        <?php endforeach; ?>
      <?php endif; ?>
  <?php endif; ?>
 <div class="founder-forum-view-all"><?php print l(t('View all'), "http://www.linkedin.com/groups", array('query' => array('gid' => $group_id), 'attributes' => array('target' => '_blank'))); ?></div>
</div>
