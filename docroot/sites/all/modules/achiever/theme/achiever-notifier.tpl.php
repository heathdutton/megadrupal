<?php

/**
 * @file
 * Theme for achievement notifications.
 *
 * @var string $classes
 *  The css classes for this achievement.
 * @var AchieverAchievement[] $achievements
 *  The list of achievements to notify the user.
 * @var object $account
 *  The user account to whom the achievements belong to.
 */

?>

<div class="<?php print $classes ?>">
  <div class="achiever-notifier-container">
    <h1 class="achiever-notifier-title"><?php print t('Congratulations'); ?></h1>
    <p class="achiever-notifier-description"><?php print t('You unlocked the following achievements'); ?></p>

    <div class="achiever-notifier-achievements">
      <?php foreach ($achievements as $achievement): ?>
        <article class="achiever-notifier-achievement <?php print achiever_css_identifier($achievement->getKey()); ?> achiever-unlocked">
          <h1 class="achiever-notifier-achievement-title"><?php print $achievement->getTitle(); ?></h1>
          <p class="achiever-notifier-achievement-description"><?php print $achievement->getDescription(); ?></p>
        </article>
      <?php endforeach; ?>
    </div>

    <div class="achiever-actions">
      <a class="achiever-action-profile" href="<?php print url('user/' . $account->uid); ?>"><?php print t('View My Achievements'); ?></a>
      <a class="achiever-action-close" href="<?php print url(current_path()); ?>"><?php print t('Close'); ?></a>
    </div>
  </div>
</div>
