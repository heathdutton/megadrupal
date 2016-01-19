<div id="piwik-release-block">
  <?php if ($release['latest']) { ?>
    <div class="latest_release">
      <small><?php print t('Latest release'); ?></small><br />
      <span class="download"><?php print $release['latest_link']; ?></span><br />
      <span class="date"><?php print $release['latest_date'] ?></span>
    </div>
  <?php } ?>
  <?php if ($release['latest_beta']) { ?>
    <div class="latest_beta_release">
      <small><?php print t('Latest beta release'); ?></small><br />
      <span class="download"><?php print $release['latest_beta_link']; ?></span><br />
      <span class="date"><?php print $release['latest_beta_date'] ?></span>
    </div>
  <?php } ?>
</div>
