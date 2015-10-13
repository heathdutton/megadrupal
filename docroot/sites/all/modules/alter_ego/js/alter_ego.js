(function ($) {
  Drupal.AlterEgo = Drupal.AlterEgo || {};
  
  Drupal.AlterEgo.avatarPopupCache = new Array();
  Drupal.AlterEgo.avatarPopupLoading = false;
  Drupal.AlterEgo.avatarPopupAid = 0;
  
  Drupal.AlterEgo.autoAttach = function() {
	  
	  $("#select-avatar-block-char-arrow").mouseenter(
		function() {
			$(this).css("background-position-y", "-144px");
			$("#select-avatar-block-pulldown").show();
    		// Position pulldown below name
    	    $("#select-avatar-block-pulldown").position({
    	      of: $("#select-avatar-block-name"),
    	      my: "left top",
    	      at: "left bottom",
    	      collision: "none"
    	    });
		}
	  );
	  $("#select-avatar-block-pulldown").mouseleave(
	    function() {
	    	$("#select-toon-block-char-arrow").css("background-position-y", "4px");
	    	$("#select-avatar-block-pulldown").hide();
	    }
	  );
	  // JavaScript is enabled.  Hide select form.
	  $("#alter-ego-set-active-avatar-form").css('display','none');
	  $("#select-avatar-block-char-arrow").css('display','inline');
	  
	  // Make the replacement select DIV trigger the hidden select form.
	  $("#select-avatar-block-pulldown").find(".select-avatar-block-select-item").click(function() {
	    $("#alter-ego-set-active-avatar-form SELECT").val($(this).attr('rel'));
	    $("#alter-ego-set-active-avatar-form").submit();
	    return false;
	  });
	  
	  // Add our popup div to the end of the document.
	  $("BODY").append('<div id="alter-ego-popup"></div>');
      
      // Loop over links and find /avatar/%avatar links.
      $("DIV.content").find("a.avatar-popup").each(function (index) {
    	  //if($(this).attr('href') && ($(this).attr('href').substr(0,8) == '/avatar/' || $(this).attr('href').substr(0,11) == '/?q=avatar/') && !$(this).hasClass('np')) {
    	  var aid = parseInt($(this).attr('rel'));
    	  
    	  if(aid) {
    		  if(aid) {
    			// Remove native browser popup
    			$(this).attr('title','');
    			$(this).mousemove(function(ev) {
    				if(Drupal.AlterEgo.avatarPopupLoading == false) {
    					if(Drupal.AlterEgo.avatarPopupAid != aid) {
    						// We are looking at a new toon.
		    				var found = -999;
		    				for(var i=0;i<Drupal.AlterEgo.avatarPopupCache.length;i++) {
		    					if(Drupal.AlterEgo.avatarPopupCache[i][0] == aid) {
		    						found = i;
		    					}
		    				}
		    				if(found != -999) {
		    					$("#alter-ego-popup").html(Drupal.AlterEgo.avatarPopupCache[found][1]); 
		    					Drupal.AlterEgo.avatarPopupAid = aid;
		    				} else {
		    				  $("#alter-ego-popup").html('<div class="summary-popup"><div class="popup-text-wrapper">loading...</div></div>');
		    				  $("#alter-ego-popup").css('display','block');
		    				  
		    				  var popup_url = 'avatar/' + aid + '/popup';
		    				// See if we are not using clean URLs
		    				  if ($(this).attr('href').indexOf('?q=') != -1) {
		    					popup_url = Drupal.settings.basePath + '?q=' + popup_url; 
		    				  }
		    				  else {
		    					popup_url = Drupal.settings.basePath + popup_url;
		    				  }
		    				  
		    				  
		    				  $("#alter-ego-popup").load(popup_url, function(txt) {
		    					Drupal.AlterEgo.avatarPopupCache.push(new Array(aid,txt));
		    					Drupal.AlterEgo.avatarPopupLoading = false;
		      				    Drupal.AlterEgo.avatarPopupAid = aid;
		      		          });
		    				  // Don't start new requests until this is done loading.
		    				  Drupal.AlterEgo.avatarPopupLoading = true;
		    				}
	    				}
	    				
    				}
    				$("#alter-ego-popup").css('display','block');
    				$("#alter-ego-popup").position({my: "left top", at: "bottom", of: $(this), offset: "0 0", collision: "flip"});
    				//$("#alter-ego-popup").position({my: "left top", at: "bottom right", of: $(ev), offset: "10 0", collision: "flip"});
    			});
    			$(this).mouseleave(function (ev) {
    				$("#alter-ego-popup").css('display','none');
    			});
    		  }
    	  }
      });	  
	};

	$(Drupal.AlterEgo.autoAttach);
})(jQuery);