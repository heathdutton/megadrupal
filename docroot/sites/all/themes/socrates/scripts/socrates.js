
(function($) {

	$(document).ready(function(){

		if($('.pane-node-field-idea-image .field-items .field-item').length > 3) {
			$('.pane-node-field-idea-image .field-items').bxSlider({
				default: false,
				pager: false,
				slideMargin: 10,
				minSlides: 1,
				maxSlides: 3,
				slideWidth: 190,
			});
		}
		//header behaviour while administrator is logged in 
			//if user can see admin toolbar give normal menu margin on top depending if second row is horizontal
			if($('.navbar-administration #navbar-item--2-tray').hasClass('navbar-active') && $('.navbar-administration #navbar-item--2-tray').hasClass('navbar-tray-horizontal'))
				{
					$('body.navbar-administration header#navbar, .main-container').css('margin-top','78px');
				}
				else
				{
					$('body.navbar-administration header#navbar, .main-container').css('margin-top','39px');
				}
			//or vertical
			$('.navbar-administration a.navbar-icon').click(function(){
				if($('#navbar-item--2-tray').hasClass('navbar-active') && $('.navbar-administration #navbar-item--2-tray').hasClass('navbar-tray-horizontal'))
				{
					$('body header#navbar, .main-container').css('margin-top','39px');
				}
				else
				{
					$('body header#navbar, .main-container').css('margin-top','78px');
				}
				if($('#navbar-item--2-tray').hasClass('navbar-tray-vertical'))
				{
					$('body header#navbar, .main-container').css('margin-top','39px');
				}
			});
			//when user clicks on navbar toggle to toggle position horizontal or vertical
			$('.navbar-administration .navbar-toggle').click(function(){
				if($(this).val()=='horizontal'){
					$('body header#navbar, .main-container').css('margin-top','78px');
				}
				else{
					$('body header#navbar, .main-container').css('margin-top','39px');	
				}
			});
		
	});     

})(jQuery);   
