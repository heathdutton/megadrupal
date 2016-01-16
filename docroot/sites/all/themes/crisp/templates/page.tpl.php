<?php
/**
* @file
* This the page.tpl.php template. 
* 
*This template outputs the content between and excluding body tags. 
*
*`
*/	
?>
<!-- header starts -->
<div id="header">
 <!-- content_wrapper starts -->
 <div class = "content_wrapper">
   <div id ="logo" span="left">
    <img src="<?php print $logo; ?>" />   
    </div>
 
 <!--menu region starts here -->
    <span class="right"><?php print render($page['menu']); ?></span> 
  <!--menu region ends here -->

 </div>
 <!-- content_wrapper ends -->
</div>
<!-- header ends -->

<!-- sub nav starts -->
  <div id="sub_header">
	<!-- wrapper starts -->
      <div class="content_wrapper">
        <!--feature header starts here optional -->
      <?php if ($page['header_feature']): ?>
         <div id="feature_header">
        <?php print render($page['header_feature']); ?>
        </div>
      <?php endif; ?>
      <!--feature header ends here  -->
 
  <!--feature header left starts  optional -->
   <?php if ($page['left_header']): ?>
     <div id ="left_header" class="left">
       <?php print render($page['left_header']); ?>
      </div>
  <?php endif; ?>
<!-- feature header left ends -->

 <!-- feature header starts  optional -->
   <?php if ($page['right_header']): ?>
      <div id ="right_header" class="right">
       <?php print render($page['right_header']); ?>
      </div>
  <?php endif; ?>
<!-- feature header right ends -->
 
 </div>
<!--wrapper ends -->
 
</div>
<!-- sub header ends -->

 <!-- main content starts -->
  <div id="main_content_outer" class="content_wrapper">
   <div id="main_content" class="left">
   <!-- content starts -->
      <?php if ($page['content_top']): ?>
         <?php print render($page['content_top']); ?>
      <?php endif; ?>
    
       <?php if ($page['content_ribbon']): ?>
           <?php print render($page['content_ribbon']); ?>
       <?php endif; ?>
    
        <?php print render($page['content']); ?>
   
      <?php if ($page['content_bottom']): ?>
        <?php print render($page['content_bottom']); ?>
       <?php endif; ?>
    
      </div>
  <!-- content ends -->
   
   <!-- sidebar starts -->
    <div id="side_bar" class="right">
     <?php print render($page['side_bar']); ?>
    </div>
   <!-- sidebar ends -->
 </div>
  <!-- main content ends -->  
  
  
  <div class="clearer">&nbsp;</div>  
  
 <!-- footer starts --> 
 <div id = "footer">
      <div class="content_wrapper">
     
       <!-- footer left starts -->
       <span class ="left"><?php print render($page['footer_left']); ?></span>
       <!--footer left ends -->
   
    <!-- footer right starts -->
    <span class ="right" id="side_bar"><?php print render($page['footer_right']); ?></span>
      <!--footer right ends -->
   </div>

</div> 
<!--footer ends -->
   






