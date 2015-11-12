<?php
/**
 * Drawingfield element template file.
 */

/**
 * A hidden field is created to capture base64 code to convert it into image.
 */
?>
<div class="form-group"> 
    <label for="<?php echo $element['#id'] ?>"><?php echo $element['#title'] ?> </label>   
    <div class="literally export" style="width:<?php echo $element['#width'] ?>px;height:<?php echo $element['#height'] ?>px;"></div> 
	 <input type='hidden' name='<?php echo $element['#name'] ?>' id='<?php echo $element['#id'] ?>' class='output' value='<?php if(isset($element['#default_value'])): echo $element['#default_value']; endif;?>'>
</div>
