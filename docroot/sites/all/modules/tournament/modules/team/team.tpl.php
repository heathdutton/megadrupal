<?php

/**
 * @file
 * Default theme implementation to display a team.
 *
 * Available variables:
 * - $name: the (sanitized) name of the team.
 * - $user: Themed username of team creator output from theme_username().
 * - $team_url: Direct url of the current team.
 *
 * There are many more available variables, but lend largely from the core node module.
 * See node.tpl.php for further details.
 *
 * @see template_preprocess_team()
 */
?>
<div id="team-<?php print $team->tid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>

  <?php if ($teaser): ?>
      <h2><a href="<?php print $team_url; ?>"><?php print $name; ?></a></h2>
  <?php endif; ?>

  <div class="team-content">
    <?php print render($content); ?>
  </div>

</div>
