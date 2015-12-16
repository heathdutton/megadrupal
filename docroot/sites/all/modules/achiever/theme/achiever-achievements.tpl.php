<?php

/**
 * @file
 * Theme for a list of achievements.
 *
 * @var $classes
 *  The css classes for this achievement
 * @var AchieverAchievement[] $achievements
 *  The achievements object instance.
 * @var array $rendered_achievements
 *  The rendered achievements list.
 * @var object $account
 *  The user account to whom the achievement belongs to.
 * @var bool $is_current_user
 *  TRUE if the achievement belong to currently authenticated user.
 */

?>

<section class="<?php print $classes ?>">
  <h1 class="achiever-achievements-title"><?php print t('Achievements'); ?></h1>
  <p class="achiever-achievements-description">
    <?php if ($is_current_user): ?>
      <?php print t("My achievements"); ?>
    <?php else: ?>
      <?php print t("The list of achievements"); ?>
    <?php endif; ?>
  </p>

  <div class="achiever-achievements-list">
    <?php foreach ($rendered_achievements as $rendered_achievement): ?>
      <?php print $rendered_achievement; ?>
    <?php endforeach; ?>
  </div>

</section>
