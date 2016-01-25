<?php

/**
 * @file
 * Example tpl file for theming a single league_game-specific theme
 *
 * Available variables:
 * - $status: The variable to theme (while only show if you tick status)
 * 
 * Helper variables:
 * - $league_game: The LeagueGame object this status is derived from
 * - $gid: ID of the game
 * - $team_a: Title of team A with link
 * - $team_b: Title of team A with link
 * - $nid_a: Node ID
 * - $nid_b: Node ID
 * - $emblem_a: Formated emblem
 * - $emblem_b: Formated emblem
 * - $score_a: Score team A
 * - $score_b: Score team B
 * - $status: 0: pending; 1: live; 2: finalized
 */
?>

<div class="league_game-body league_game-body-<?php print $view_mode ?>" id="result-game-<?php print $gid ?>">
  <?php if ($view_mode == 'teaser'): ?>
  <p class="game-status-<?php print $status ?>">
    <span class="team_a team-<?php print $nid_a ?>"><?php print $team_a ?>: 
      <span class="score score-a"><?php print $score_a ?></span>
    </span>
    &nbsp;-&nbsp;
    <span class="team_b team-<?php print $nid_b ?>"><?php print $team_b ?>: 
      <span class="score score-b"><?php print $score_b ?></span>
    </span>
  </p>
  <?php else: ?>
  <table class="game-status-<?php print $status ?>">
    <thead>
      <th class="team_a team-<?php print $nid_a ?>"><?php print $team_a ?></th>
      <th class="team_b team-<?php print $nid_b ?>"><?php print $team_b ?></th>
    </thead>
    <tr>
      <td>
        <span class="band-left"> <?php print $emblem_a ?></span>
        <span class="score-<?php print $score_a ?> score-a"><?php print $score_a ?></span>
      </td>
      <td>
        <span class="score-<?php print $score_b ?> score-b"><?php print $score_b ?></span>
        <span class="band-right"><?php print $emblem_b ?></span></td>
    </tr>
  </table>
  <?php endif; ?>
</div>
