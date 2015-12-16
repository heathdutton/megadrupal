<div id="thermometer">

  <div class="track">
    <div class="goal">
      <div class="amount"><?php echo $goal; ?></div>
    </div>
    <div class="progress">
      <div class="amount"><?php echo $progress; ?></div>
    </div>
  </div>

</div>


<?php if($button_text): ?>
  <a class="btn" href="<?php echo $button_url; ?>" <?php echo ($button_window ? 'target="_blank"' : ''); ?>><?php echo $button_text; ?></a>
<?php endif; ?>
