<?php 
$selector = '#' . $settings['id'] ;
$count = 0 ;
$is_first = true ;
?>
<div id="<?php print $settings['id'] ?>" data-interval="<?php print $settings['interval']; ?>" data-wrap="<?php print $settings['wrap']; ?>" data-pause="<?php print $settings['pause']; ?>" class="carousel slide" data-ride="carousel">
  <!-- Indicators -->
  <?php if ( $settings['nav']) : ?>
  <ol class="carousel-indicators">
  <?php  foreach ( $items as $key => $item ): ?>
  	<?php 
  	    if ( $is_first ) {
  			$active = 'active' ;
  			$is_first = false ;
  		} else {
			$active = '';
			
		}
  	?>
    <li data-target="<?php print $selector; ?>" data-slide-to="<?php print $count?>" class="<?php print $active; ?>"></li>
    <?php $count++; ?>
  <?php endforeach; ?>
  </ol>
  <?php endif; ?>
  <div class="carousel-inner"  role="listbox">
  <?php  
    $is_first = true ;
    foreach ( $items as $key => $item ):  
    ?>
    <div class="item <?php if ( $is_first ) print "active"; ?>"><?php print render($item) ?></div>
  <?php 
  	$is_first = false ;
  	endforeach; 
  ?>
  </div>  
  <a class="left carousel-control" href="<?php print $selector; ?>" data-slide="prev">
    <span class="glyphicon glyphicon-chevron-left"></span>
  </a>
  <a class="right carousel-control" href="<?php print $selector; ?>" data-slide="next">
    <span class="glyphicon glyphicon-chevron-right"></span>
  </a>
</div>