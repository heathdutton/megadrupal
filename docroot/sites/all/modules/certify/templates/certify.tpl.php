<?php foreach ($certificates as $certificate): ?>
  <?php print theme('certifysingle', array('certificate' => $certificate)) ?>
<?php endforeach ?>
