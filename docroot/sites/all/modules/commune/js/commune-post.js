// Auto upload image & hide upload button.
(function ($) {
  Drupal.behaviors.commune_post = {
    attach: function(context, settings) {
	  /*
      if($.embedly) {
         $.embedly.defaults.key = Drupal.settings.commune.embedly_key;
         $('.wall-embed-input', context).preview();
      }
      */

      $('.commune-action-button a.commune-file', context).click(function(e) {
	  $('.commune-link-area').hide();
          $('.commune-action-button a.commune-link').removeClass("active");
          $(this).toggleClass("active");
	  $('.commune-file-area').toggle();
	  return false;
	  
      });

      $('.commune-action-button a.commune-link', context).click(function(e) {
	  $('.commune-file-area').hide();
          $('.commune-action-button a.commune-file').removeClass("active");
          $(this).toggleClass("active");
	  $('.commune-link-area').toggle();
          return false;
      });

      $('form.commune-post-form', context).submit(function(e) {
         var markup, mentions ;
         var form = this;
         $('.commune-post-textbox', form).mentionsInput('val', function(text) {
					   $('input[name=commune_post_markup]', form).val(text);
         });
         $('.commune-post-textbox', form).mentionsInput('getMentions', function(data) {
						 mentions = JSON.stringify(data);
		         $('input[name=commune_post_mentions]', form).val(mentions);
         });
      });
 
      if(context.nodeName == '#document') {
		    if($.isFunction($.fn.mentionsInput)) {
      		  $('form.commune-post-form .commune-post-textbox', context).mentionsInput({
			    onDataRequest:function(mode, query, callback) {
			       var path = '/commune/users/' + query;
			       $.getJSON(path, '', function(responseData) {
						responseData = _.filter(responseData, function(item) { 
							return item.name.toLowerCase().indexOf(query.toLowerCase()) > -1 
						});
				
						callback.call(this, responseData);
			    	});
			    }
	  		});
		}
      }
    }
  }
})(jQuery);
