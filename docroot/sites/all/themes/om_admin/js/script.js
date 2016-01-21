jQuery(document).ready(function($){
	var topYloc = null;
  //positions top-link scroller button
  $(window).scroll(function () { 
	  var scrollTop = $(document).scrollTop();
	  scrollTop = parseInt(scrollTop);
	
	  var offset = topYloc+scrollTop+"px";  
	  $("#page-link").animate({top:offset},{duration:500,queue:false});
  });

	//back to top scroll function. Any link with a hash (#) will scroll to that id on the page
	$('a[href*=#]').click(function() {
		if (location.pathname.replace(/^\//,'') == this.pathname.replace(/^\//,'') && location.hostname == this.hostname) {
			var $target = $(this.hash);
			$target = $target.length && $target || $('[name=' + this.hash.slice(1) +']');
			if ($target.length) {
			  var targetOffset = $target.offset().top;
				  //targetOffset = targetOffset - 90;
				$('html,body').animate({scrollTop: targetOffset -25}, 500);
				return false;
			}
		}
	});
	//gets positions top-link scroller button
	topYloc = parseInt($("#page-link").css("top").substring(0,$("#page-link").css("top").indexOf("px")));
});
