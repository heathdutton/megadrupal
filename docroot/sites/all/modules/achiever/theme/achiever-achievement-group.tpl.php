<?php

/**
 * @file
 * Theme for an achievement that contains sub-achievements.
 *
 * @var string $title
 *  The achievement title.
 * @var string $description
 *  The achievement description.
 * @var int $unlocked
 *  The number of sub-achievements that have been unlocked.
 * @var int $total
 *  The total number of sub-achievements.
 * @var string $classes
 *  The css classes for this achievement.
 * @var AchieverAchievement $achievement
 *  The achievement object instance.
 * @var AchieverAchievement[] $achievements
 *  A list of sub-achievements of the achievement.
 * @var object $account
 *  The user account to whom the achievement belongs to.
 */

?>

<article class="<?php print $classes ?>">
  <h1 class="achiever-achievement-title"><?php print $title; ?></h1>
  <p class="achiever-achievement-description"><?php print $description; ?></p>

  <div class="achiever-achievement-levels">
    <?php if ($unlocked != $total): ?>
      <span><?php print t('<strong>Level @num</strong> of @total', array('@num' => $unlocked, '@total' => $total)); ?></span>
    <?php else: ?>
      <strong><?php print t('Completed'); ?></strong>
    <?php endif; ?>
  </div>

  <div class="achiever-sub-achievements">
    <ul>
      <?php foreach ($achievements as $sub_achievement): ?>
        <li class="achiever-sub-achievement <?php print ($sub_achievement->isUnlocked($account) ? 'achiever-unlocked' : 'achiever-locked'); ?>">
          <strong class="achiever-sub-achievement-title">
            <?php print $sub_achievement->getTitle(); ?>
          </strong>
          <span class="achiever-sub-achievement-description">
            <?php print $sub_achievement->getDescription(); ?>
          </span>
        </li>
      <?php endforeach; ?>
    </ul>
  </div>
</article>
