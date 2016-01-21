<div class="accordion-group">
  <div class="accordion-heading">
    <a class="accordion-toggle" data-toggle='collapse' data-parent="#single-accordion"
      href="#accordion-<?php print $key; ?>"><?php print $title; ?></a>
  </div>
  <div id="accordion-<?php print $key; ?>" class="accordion-body collapse">
    <div class="accordion-inner">
       <?php print $content; ?>
    </div>
  </div>
</div>
