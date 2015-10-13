(function($){
	Drupal.behaviors.custom={
		attach:function(context) {
			$('#nlname').val("Name");
			$('#nlname').attr("style","color:silver");
			$('#nlname').focus(function() {
				if($(this).val()=="Name"){
					$(this).val("");
					$('#nlname').attr("style","color:#878787");
				}				
			});			
		    $("#nlname").blur(function() {
		    	if($(this).val()==""){
		    		$(this).val("Name");
		    		$('#nlname').attr("style","color:silver");
		    	}
			});
			$('#nlemail').val("Email");
			$('#nlemail').attr("style","color:silver");
			$('#nlemail').focus(function() {
				if($(this).val()=="Email"){
					$(this).val("");
					$('#nlemail').attr("style","color:#878787");
				}				
			});			
		    $("#nlemail").blur(function() {
		    	if($(this).val()==""){
		    		$(this).val("Email");
		    		$('#nlemail').attr("style","color:silver");
		    	}
			});
			$('#edit-search-block-form--2').val("Search");
			$('#edit-search-block-form--2').attr("style","color:silver");
			$('#edit-search-block-form--2').focus(function() {
				if($(this).val()=="Search"){
					$(this).val("");
					$('#edit-search-block-form--2').attr("style","color:#878787");
				}				
			});			
		    $("#edit-search-block-form--2").blur(function() {
		    	if($(this).val()==""){
		    		$(this).val("Search");
		    		$('#edit-search-block-form--2').attr("style","color:silver");
		    	}
			});
			$('#edit-subject').val("Subject");
			$('#edit-subject').attr("style","color:silver");
			$('#edit-subject').focus(function() {
				if($(this).val()=="Subject"){
					$(this).val("");
					$('#edit-subject').attr("style","color:#878787");
				}				
			});			
		    $('#edit-subject').blur(function() {
		    	if($(this).val()==""){
		    		$(this).val("Subject");
		    		$('#edit-subject').attr("style","color:silver");
		    	}
			});
			$('#edit-comment-body-und-0-value').val("Comment");
			$('#edit-comment-body-und-0-value').attr("style","color:silver");
			$('#edit-comment-body-und-0-value').focus(function() {
				if($(this).val()=="Comment"){
					$(this).val("");
					$('#edit-comment-body-und-0-value').attr("style","color:#878787");
				}				
			});			
		    $('#edit-comment-body-und-0-value').blur(function() {
		    	if($(this).val()==""){
		    		$(this).val("Comment");
		    		$('#edit-comment-body-und-0-value').attr("style","color:silver");
		    	}
			});
			$('#edit-name').val("Name");
			$('#edit-name').attr("style","color:silver");
			$('#edit-name').focus(function() {
				if($(this).val()=="Name"){
					$(this).val("");
					$('#edit-name').attr("style","color:#878787");
				}				
			});			
		    $('#edit-name').blur(function() {
		    	if($(this).val()==""){
		    		$(this).val("Name");
		    		$('#edit-name').attr("style","color:silver");
		    	}
			});
			$('#edit-mail').val("Email");
			$('#edit-mail').attr("style","color:silver");
			$('#edit-mail').focus(function() {
				if($(this).val()=="Email"){
					$(this).val("");
					$('#edit-mail').attr("style","color:#878787");
				}				
			});			
		    $('#edit-mail').blur(function() {
		    	if($(this).val()==""){
		    		$(this).val("Email");
		    		$('#edit-mail').attr("style","color:silver");
		    	}
			});
			$('#edit-message').val("Your Message");
			$('#edit-message').attr("style","color:silver");
			$('#edit-message').focus(function() {
				if($(this).val()=="Your Message"){
					$(this).val("");
					$('#edit-message').attr("style","color:#878787");
				}				
			});			
		  $('#edit-message').blur(function() {
		    	if($(this).val()==""){
		    		$(this).val("Your Message");
		    		$('#edit-message').attr("style","color:silver");
		    	}
			});
			if($('#aboutus_middle_right .content').height()>$('#aboutus_middle_left .content').height()) {
				$('#aboutus_middle_left .content').height($('#aboutus_middle_right .content').height()+30);
				$('#aboutus_middle_right .content').height($('#aboutus_middle_left .content').height());
			}
			else {
				$('#aboutus_middle_right .content').height($('#aboutus_middle_left .content').height()+30);
				$('#aboutus_middle_left .content').height($('#aboutus_middle_right .content').height());
			}
		}
	};
})(jQuery);
