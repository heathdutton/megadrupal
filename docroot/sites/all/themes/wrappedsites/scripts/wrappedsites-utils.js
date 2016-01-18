jQuery(document).ready(function() {
 jQuery('#edit-colors-settings .form-item.form-type-textfield input ').ColorPicker({
	onSubmit: function(hsb, hex, rgb, el) {
		jQuery(el).val(hex);
		jQuery(el).ColorPickerHide();
                refreshColors();
	},
	onBeforeShow: function () {
		jQuery(this).ColorPickerSetColor(this.value);
	},
        onChange: function (hsb, hex, rgb) {
        
        refreshColors();
        
      }
        
        
      
        
})
.bind('keyup', function(){
	jQuery(this).ColorPickerSetColor(this.value);
});

});



function refreshColors() {
    
    jQuery('#edit-colors-settings .form-item.form-type-textfield input ').each(function() {
      
       jQuery(this).css('backgroundColor', '#'+ jQuery(this).val());
       
   });

    

}