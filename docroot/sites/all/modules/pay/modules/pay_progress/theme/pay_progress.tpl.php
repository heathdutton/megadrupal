<?php

?>
<?php if ($goal) :?>
  <div class="pay-progress">
    <div>
      <div class="pay-progress-bar">
        <div class="pay-progress-completed" style="width: <?php print $percent; ?>%"></div>
      </div>
    </div>
    <span class="pay-progress-percent"><?php print $percent; ?>%</span>
    <div class="pay-progress-overview">
      Raised <?php print theme('pay_money', $total, $currency); ?> of <?php print theme('pay_money', $goal, $currency); ?>
    </div>
  </div>
<?php endif;?>
