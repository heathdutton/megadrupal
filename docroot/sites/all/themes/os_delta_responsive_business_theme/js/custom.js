jQuery(document).ready(function () {
 if (jQuery("[rel=tooltip]").length) {jQuery("[rel=tooltip]").tooltip();}
 	jQuery('.fancybox').fancybox();

	var clsName = jQuery('#videoBox').attr('class');
    var options = {videoId: clsName};
    jQuery('#videoBox').tubular(options);

// ______________________________________global responsive menu
	var gmenu = jQuery('.mainMenu ul.menu');
		gmenu.find('ul').has('li').removeClass('menu');
		gmenu.find('li').has('ul').addClass('parent');
		gmenu.find('li').has('a.active').addClass('active');
		gmenu.find('li.parent > a').removeAttr('href').next('ul').hide();
		gmenu.find('li.parent > a').append('<span class="arrow"></span>');

		function clickOrHover(){
			if (jQuery(window).width() >= 980) return true;
			return false
		}
		    gmenu.find('li.parent').hover(function() {
		        if (clickOrHover() && !jQuery(this).children('ul').is(':visible')) {
		            jQuery(this).children('ul').stop().slideDown(100);
		        }
		    },
		    function() {
		        if (clickOrHover() && jQuery(this).children('ul').is(':visible')) {
		            jQuery(this).children('ul').slideUp(100);
		        }
		    });

		    gmenu.find("li.parent a").click(function(){
		        var checkElement = jQuery(this).next();
		        if (checkElement.is('ul')) {
		            if (!checkElement.is(':visible')) {
		                checkElement.slideDown(100);
		            } else {
		                checkElement.slideUp(100);
		            }
		        }
		    });
// __________________________________

 });