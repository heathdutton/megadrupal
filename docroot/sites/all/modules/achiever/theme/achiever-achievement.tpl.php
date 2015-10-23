<?php

/**
 * @file
 * Theme for a single achievement.
 *
 * @var string $title
 *  The achievement title.
 * @var string $description
 *  The achievement description.
 * @var string $classes
 *  The css classes for this achievement.
 * @var AchieverAchievement $achievement
 *  The achievement object instance.
 * @var object $account
 *  The user account to whom the achievement belongs to.
 */

?>

<article class="<?php print $classes ?>">
  <h1 class="achiever-achievement-title"><?php print $title; ?></h1>
  <p class="achiever-achievement-description"><?php print $description; ?></p>
</article>
