<?php
/**
 * @file
 * Template for a single news item.
 */
?>
<li class="timeline" data-id="<?php print $vars['nid']; ?>">
  <span class="activity_item news_activity">
    <span class="timeline-badge views-field-picture">
      <span class="glyphicon glyphicon-user"></span>
      <?php if (isset($vars['picture'])): ?>
        <?php print $vars['picture']; ?>
      <?php endif; ?>
    </span>

    <span class="timeline-panel">
      <span class="timeline-heading">
        <a href="<?php print $vars['link']; ?>">

          <h5 class="timeline-title">
            <strong><?php print $vars['username']; ?></strong>
            <?php print $vars['between_text']; ?>
            <strong><?php print $vars['title']; ?></strong>
          </h5>

          <p>
            <small class="text-muted time-ago">
              <i class="glyphicon glyphicon-transfer"></i>
              <?php print $vars['time_ago']; ?>
            </small>
          </p>

          <?php if ($vars['body']): ?>
            <blockquote>
              <?php print $vars['body']; ?>
            </blockquote>
          <?php endif; ?>
        </a>
      </span>

      <div class="status-update-actions">
        <?php if (!empty($vars['like_flag'])): ?>
          <div class="status-update-like">
            <?php print $vars['like_flag']; ?>
          </div>
        <?php endif; ?>
        <?php if (!empty($vars['likes'])): ?>
          <div class="status-update-likes">
            (<?php print $vars['likes']; ?>)
          </div>
        <?php endif; ?>
      </div>

      <?php if (!empty($vars['gallery'])): ?>
        <?php print $vars['gallery']; ?>
      <?php endif; ?>

      <?php if (!empty($vars['files'])): ?>
        <?php print $vars['files']; ?>
      <?php endif; ?>
    </span>
  </span>
</li>
