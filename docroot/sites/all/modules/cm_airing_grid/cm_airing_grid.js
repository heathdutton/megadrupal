(function ($) {

    Drupal.behaviors.airingGridAiringDetail = {
	attach: function (context, settings) {
	    $('.cmag', context).click(function() {
		$('#airing_detail_div').html("<h2>Loading...</h2>");
		var cm_agd_url = '/cm_airing_grid_detail/'+ $(this).attr('id');
		$('.cm_airing_grid_blank').css({"border":"none"});
		$(this).parent().parent().parent().parent().parent().
		    css({"border":"8px solid red"});
		$.getJSON(cm_agd_url, function(data){
		    $('#airing_detail_div').html(data.airing_detail);
		});
		$('html, body').animate({scrollTop:$('#airing_detail_div').
					 offset().top - 75}, 'slow');
		$('div.contextual-links-wrapper').hide();
	    });
	    $(document).ready(function() {
		$('.cm_airing_grid_selected_airing').parent().parent().
		    parent().parent().parent().css({"border":"8px solid red"});
	    });
	}};    

 
}) (jQuery);
