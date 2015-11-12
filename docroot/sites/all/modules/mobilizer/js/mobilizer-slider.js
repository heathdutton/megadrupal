jQuery(document).ready(function (){
	jQuery('.select-all').html("Delete");
	jQuery('input[type="checkbox"]').click(function(){
		if(jQuery(".form-checkbox:checked").length > 0){
			jQuery('#delete_images').show();
		}else{
			jQuery('#delete_images').hide();
		}
		
	});
});
