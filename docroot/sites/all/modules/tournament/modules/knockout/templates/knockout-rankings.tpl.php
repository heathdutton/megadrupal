<h2><?php print t('Final standings'); ?></h2>
<div class="knockout-rankings-participants">
  <?php foreach ($participants as $participant): ?>
    <div class="rankings-participant rank-<?php print $participant['rank']; ?>">
      <?php if (isset($participant['picture'])): ?>
        <div class="rankings-picture">
          <?php print $participant['picture']; ?>
        </div>
      <?php endif; ?>
      
      <div class="rankings-name-wrapper">
        <div class="rankings-rank">
          <?php print $participant['rank_ordinal']; ?>
        </div>
        <div class="rankings-name">
          <?php print $participant['label']; ?>
        </div>
        <span class="matches">
          <?php print $participant['matches']; ?>
        </span>
        <span class="tournaments">
          <?php print $participant['tournaments']; ?>
        </span>
      </div>
    </div>
  <?php endforeach; ?>
</div>
<?php print render($close_link); ?>

