<div class="commerce-goal-progress">
  <div class="commerce-goal-current">
    <span class="commerce-goal-total"><?php print $current_total_formatted ?></span>
    <span class="commerce-goal-percent"><?php print $current_percent ?>%</span>
  </div>
  <div class="commerce-goal-bar">
    <div class="commerce-goal-complete" style="width: <?php print $current_percent ?>%"></div>
  </div>
  <div class="commerce-goal-goal">Goal: <?php print $goal_formatted ?></div>
</div>
