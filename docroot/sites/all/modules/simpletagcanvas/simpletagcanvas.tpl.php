<?php

/**
 * @file
 * HTML for simpletagcanvas.
 *
 * @see template_preprocess_simpletagcanvas()
 */

?>

  <div id="myCanvasContainer">
    <canvas id="mySimpleTagCanvas" width="<?php print check_plain($width) ?>" height="<?php print check_plain($height) ?>">
      <p>Anything in here will be replaced on browsers that support the canvas element</p>
    </canvas>
  </div>
  <div id="tags">

<?php

  if ($items) :
    foreach ($items as $image) :
      print $image;
    endforeach;
  endif;

?>

</div>
