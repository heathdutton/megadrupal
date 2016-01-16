<?php
class textPNG {
	var $font = 'Abduction II.ttf'; //default font. directory relative to script directory.
	var $msg = "undefined"; // default text to display.
	var $size = 24;
	var $rot = 0; // rotation in degrees.
	var $pad = 10; // padding.
	var $transparent = 1; // transparency set to on.
	var $red = 255; // white text...
	var $grn = 255;
	var $blu = 255;
	var $bg_red = 0; // on black background.
	var $bg_grn = 0;
	var $bg_blu = 0;
        var $output = '';

function draw() {
	$width = 0;
	$height = 0;
	$offset_x = 0;
	$offset_y = 0;
	$bounds = array();
	$image = "";
	
	// determine font height.
	$bounds = ImageTTFBBox($this->size, $this->rot, $this->font, "W");
	if ($this->rot < 0) {
		$font_height = abs($bounds[7]-$bounds[1]);		
	} else if ($this->rot > 0) {
		$font_height = abs($bounds[1]-$bounds[7]);
	} else {
		$font_height = abs($bounds[7]-$bounds[1]);
	}

	// determine bounding box.
	$bounds = ImageTTFBBox($this->size, $this->rot, $this->font, $this->msg);
	if ($this->rot < 0) {
		$width = abs($bounds[4]-$bounds[0]);
		$height = abs($bounds[3]-$bounds[7]);
		$offset_y = $font_height;
		$offset_x = 0;
		
	} else if ($this->rot > 0) {
		$width = abs($bounds[2]-$bounds[6]);
		$height = abs($bounds[1]-$bounds[5]);
		$offset_y = abs($bounds[7]-$bounds[5])+$font_height;
		$offset_x = abs($bounds[0]-$bounds[6]);
		
	} else {
		$width = abs($bounds[4]-$bounds[6]);
		$height = abs($bounds[7]-$bounds[1]);
		$offset_y = $font_height;;
		$offset_x = 0;
	}
	
	$image = imagecreate($width+($this->pad*2)+1,$height+($this->pad*2)+1);
	
	$background = ImageColorAllocate($image, $this->bg_red, $this->bg_grn, $this->bg_blu);
	$foreground = ImageColorAllocate($image, $this->red, $this->grn, $this->blu);
	
	if ($this->transparent) ImageColorTransparent($image, $background);
	ImageInterlace($image, false);
	
	// render it.
	ImageTTFText($image, $this->size, $this->rot, $offset_x+$this->pad, $offset_y+$this->pad, $foreground, $this->font, $this->msg);
	
	// output PNG object.
        if(!empty($this->output)){
	imagePNG($image, $this->output);
        } else {
	imagePNG($image);
        }
        imagedestroy($image);
}
}

?>
