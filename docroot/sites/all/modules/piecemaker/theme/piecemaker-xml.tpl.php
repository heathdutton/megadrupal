<?php
print "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
?>
<Piecemaker>
  <Contents>
    <?php print implode("\n",$ContentNodes); ?>
  </Contents>
  <Settings <?php print drupal_attributes($Settings); ?>></Settings>
  <Transitions>
    <?php foreach($Transitions as $transition):?>
    <Transition <?php print drupal_attributes($transition); ?>></Transition> 
    <?php endforeach;?>
  </Transitions>
</Piecemaker>