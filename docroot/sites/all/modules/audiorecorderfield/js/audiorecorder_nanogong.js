(function ($) {
  Drupal.behaviors.audiorecorderfield = {};
  Drupal.behaviors.audiorecorderfield.attach = function(context, settings) {
	if (!deployJava.isWebStartInstalled("1.4.2"))
    {
      //Java is not Enabled
      $(".nanogong-fallback").show();
      $(".nanogong-recorder").hide();
      $(".audiorecorderfield-upload").hide();
    }
  };
})(jQuery);

function submitVoice(field_name, langcode, delta, field_raw) { 
	var field_id = field_name+"-"+langcode+"-"+delta;
    var file_id = field_raw+'['+langcode+']['+delta+'][fid]';
    //Remove any error messages
	jQuery("#edit-field-"+field_id+" .error").remove();
	//Disable the button
	jQuery("#nanogong-button-"+field_id).attr('disabled', 'disabled');	
	
	if(jQuery("#nanogong-button-"+field_id).val()=='Upload'){
	//Upload button
		
		// Find the applet object 
		var applet = jQuery("#nanogong-"+field_id).get(0); 
		// Tell the applet to post the voice recording to the backend PHP code 
		var path=Drupal.settings.basePath +"?q=audiorecorderfield_file_receive";

		var fid = applet.sendGongRequest("PostToForm", path, "voicefile", "", "temp"); 
		if (fid == null || fid == ""){
		jQuery("#nanogong-button-"+field_id).removeAttr("disabled");
        //jQuery("#edit-field-"+field_id+"-ajax-wrapper .form-managed-file").prepend('<div class="messages error file-upload-js-error">Failed to submit the voice recording!</div>');
        jQuery("#edit-field-"+field_id).prepend('<div class="messages error file-upload-js-error">Failed to submit the voice recording!</div>');

		} 
		else{
			jQuery.ajax({
					type: "GET",
					url: Drupal.settings.basePath +"?q=audiorecorderfield_file_preview",
					data: "fid=" + fid+"&field_id=" + field_id,
					dataType: 'json',
					success: function (data) {
                        NanogongPreview(data, field_id);
                    }
				});
                
			jQuery('input[name="'+file_id+'"]').val(fid);
		} 
	}
	else{
		//Remove button
	jQuery("#edit-field-"+field_id+"-wrapper .filefield-file-info").remove();
	//jQuery("#edit-field-"+field_id+"-fid").val("");
    jQuery('input[name="'+file_id+'"]').val("");
	jQuery("#nanogong-button-"+field_id).removeAttr("disabled");
	jQuery("#nanogong-button-"+field_id).val('Upload');
	//Remove and reload the applet
    jQuery.ajax({
				    type: "GET",
					url: Drupal.settings.basePath +"?q=audiorecorderfield_recorder_reload",
                    data: "langcode="+langcode + "&delta=" + delta +  "&field_raw=" + field_raw,
					dataType: 'json',
					success:  function (data) {
                      jQuery("#audiorecorderfield-"+field_id+"-wrapper").html(data.recorder);
                      Drupal.attachBehaviors("#audiorecorderfield-"+field_id+"-wrapper");
                    }
	});
   }
} 

function NanogongPreview(data, field_id){
	//Remove Upload button and add Remove button
	jQuery("#nanogong-button-"+field_id).removeAttr("disabled");
	jQuery("#nanogong-button-"+field_id).val('Remove');
	jQuery("#nanogong-"+field_id+"-wrapper").remove();
    jQuery("#nanogong-button-"+field_id).before(data.player);
}