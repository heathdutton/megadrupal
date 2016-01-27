<?php

$first_site=false;
?>
 <!-- Alexa Graph Widget from http://www.alexa.com/siteowners/widgets/graph -->

  <script type="text/javascript"
    src="http://widgets.alexa.com/traffic/javascript/graph.js"></script>

  <script type="text/javascript">/*
  <![CDATA[*/

     // USER-EDITABLE VARIABLES
     // enter up to 3 domains, separated by a space
     var sites      = [
                       <?php 
     					if ($siteurl1){
     						print "'".$siteurl1."'";
     						$first_site=true;
     					}
     					if  ($siteurl2){
     						if(!$first_site){
     							print "'".$siteurl2."'";$first_site=true;
     						}else{
     							print ",'".$siteurl2."'";
     						}
     					}
     					if ($siteurl3){
     						if(!$first_site){
     							print "'".$siteurl3."'";$first_site=true;
     						}else{
     							print ",'".$siteurl3."'";
     						}
     					}
     					?>	
                      ];
           
     var opts = {
        width:      <?php print $width; ?>,  // width in pixels (max 400)
        height:     <?php print $height; ?>,  // height in pixels (max 300)
        type:       <?php print "'".$type."'"; ?>,  // "r" Reach, "n" Rank, "p" Page Views
        range:      <?php print "'".$range."'"; ?>, // "7d", "1m", "3m", "6m", "1y", "3y", "5y", "max"
        bgcolor:    <?php print "'".$bgcolor."'"; ?> // hex value without "#" char (usually "e6f3fc")
     };
     // END USER-EDITABLE VARIABLES
     AGraphManager.add( new AGraph(sites, opts) );

  //]]></script>

  <!-- end Alexa Graph Widget -->