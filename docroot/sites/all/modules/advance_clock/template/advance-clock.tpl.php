<div id="<?php echo $variables['images']['id']; ?>" class="clock_container">
  <div class="lbl"><?php echo @$variables['images']['zone']; ?></div>
  <div class="clockHolder">
    <?php if (isset($variables['images']['hour'])): ?>	
      <div class="rotatingWrapper"><?php echo $variables['images']['hour']; ?></div>
    <?php endif; ?>
    <?php if (isset($variables['images']['min'])): ?>	
      <div class="rotatingWrapper"><?php echo $variables['images']['min']; ?></div>
    <?php endif; ?>
    <?php if (isset($variables['images']['sec'])): ?>	
      <div class="rotatingWrapper"><?php echo $variables['images']['sec']; ?></div>
    <?php endif; ?>
    <?php if (isset($variables['images']['clock'])): ?>	
      <?php echo $variables['images']['clock']; ?>
    <?php endif; ?>    
  </div>
  <div class="digital"> <span class="hr"></span><span class="minute"></span> <span class="period"></span> </div>
</div>