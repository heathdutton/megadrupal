/**
 * cycle activation
 * the variables come from banners.inc
 */
jQuery(document).ready(function($) {

	rotateBanners();

	function rotateBanners(){
    	$('#header-images').cycle({  // rotate banners
    		slideExpr:'img.slide',
			pause:  parseInt(Drupal.settings.business_lite_configure_cycle.js.banner_pause),
			
	
		});		
	}	
	
	function onBefore() {
	
		var link = $(this).parent().attr("href");
		var title = $(this).attr("title");
		var description = $(this).attr("longdesc");

		$('#header-image-title > a').text(title); // set banner title
		$('#header-image-description > a').text(description); // set banner description
		$('.bannerlink').attr({ // set banner link on text
			href: link
        });
	};
	
	// hide banners when css is disabled
	var banner = $("#header-images");
    var cycling= false;

	if($("#pageBorder").css("float") == "left") {  // if we can read this property, then we have css support
       cycling= true;
       rotateBanners();
      }
    else{
       $('img.slide').hide();
      }
    var checkInterval = setInterval(function (banner){ // check css support every second
    if($("#pageBorder").css("float") == "none"){
      if (cycling) {
        $('img.slide').hide();
        banner.cycle("destroy");
        cycling = false;
      }
    }
    else {
      if (!cycling){
        $('img.slide').show();
        rotateBanners();
        cycling = true;
      }
    }
  }, 1000, banner);
});
