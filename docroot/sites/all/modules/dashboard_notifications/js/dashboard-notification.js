(function ($) {
  Drupal.behaviors.dashboard_notification_get_message = {
    attach: function(context) {
      $.ajax({
        type: 'GET',
        url: Drupal.settings.basePath + Drupal.settings.pathPrefix + Drupal.settings.dashboard_notification.url_prefix + 'js/dashboard-notification/get-message',
        success: function(response) {
          $('.block-dashboard-notifications').html(response[1].data);
          //console.log("data: "+response[1].data);
          if(response[1].data == ""){
          	$('.block-dashboard-notifications').hide();
          }
        }
      });
    	
      $('body').delegate('div.dashboard-notification-close a', 'click', function() {
        id = $(this).attr('rel');
        $.ajax({
          type: 'GET',
          data: 'message=' + id,
          url: Drupal.settings.basePath + Drupal.settings.dashboard_notification.url_prefix + 'js/dashboard-notification/close-message',
          success: function(response) {
            $('#dashboard-notification-' + id).fadeOut('slow');
            
            // get the count of visible notifications. If zero, hide the title.
            var count_of_divs_with_display_block = 0;
            $( ".block-dashboard-notifications div.dashboard-notification" ).each(function( index ) {
							if($( this ).css('display') == "block"){
								count_of_divs_with_display_block++;
							}
						//	console.log(count_of_divs_with_display_block);
						});
						//console.log("final count"+count_of_divs_with_display_block);
						if(count_of_divs_with_display_block == 1){
							//hide title
							$('.block-dashboard-notifications').fadeOut('slow');
						}
          }
        });
      });
  	}
  };
}(jQuery));
