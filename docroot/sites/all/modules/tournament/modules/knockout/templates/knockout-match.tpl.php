<div class="match <?php print implode(' ', $match_class); ?>">
  <div class="participant top <?php print implode(' ', $top['class']); ?>" participant-id="<?php print $top['id']?>">
    <span class="name">
      <span class="label">
        <?php if (!empty($top['seed'])): ?>
          <?php print $top['seed'] . '. '; ?>
        <?php endif; ?>
        <?php print !empty($top['display_name']) ? $top['display_name'] : $top['name']; ?>
      </span>
    </span>
    <span class="score"><?php echo $top['score']; ?></span>
  </div>

  <div class='match-divider'><?php print t('Match !position', array('!position' => $match['position'])); ?></div>

  <?php if (!empty($score_link)): ?>
    <?php print $score_link; ?>
  <?php endif; ?>

  <?php if (!empty($match['nid'])): ?>
    <?php print $details_link; ?>
  <?php endif; ?>

  <div class="participant bottom <?php print implode(' ', $bottom['class']); ?>" participant-id="<?php print $bottom['id']?>">
    <span class="name">
      <span class="label">
        <?php if (!empty($bottom['seed'])): ?>
          <?php print $bottom['seed'] . '. '; ?>
        <?php endif; ?>
        <?php print !empty($bottom['display_name']) ? $bottom['display_name'] : $bottom['name']; ?>
      </span>
    </span>
    <span class="score"><?php print $bottom['score']; ?></span>
  </div>
</div>