/*
 * jQuery Automatic Image Slider
 * http://www.sohtanaka.com/web-design/automatic-image-slider-w-css-jquery
 */
jQuery(document).ready(function($) {	
    //Show the paging and activate its first link
	$(".paging").show();
	$(".paging a:first").addClass("active");

	//Get size of images, how many there are, then determin the size of the image reel.
	var imageWidth = $(".window").width();
	var imageSum = $(".image_reel img").size();
	var imageReelWidth = imageWidth * imageSum;
	
	//Adjust the image reel to its new size
	$(".image_reel").css({'width' : imageReelWidth});

	//Paging and Slider Function
	rotate = function(){	
	    var triggerID = $active.attr("rel") - 1; //Get number of times to slide
	    var image_reelPosition = triggerID * imageWidth; //Determines the distance the image reel needs to slide
	
	    $(".paging a").removeClass('active'); //Remove all active class
	    $active.addClass('active'); //Add active class (the $active is declared in the rotateSwitch function)
	    
		$(".slidertext").stop(true,true).slideUp('slow', 'easeOutBounce');		
		$(".slidertext").eq( $('.paging a.active').attr("rel") - 1 ).slideDown('slow', 'easeOutBounce');
		
		$(".slidertitle").stop(true,true).slideUp('slow', 'easeOutBounce');		
		$(".slidertitle").eq( $('.paging a.active').attr("rel") - 1 ).slideDown('slow', 'easeOutBounce'); 
		
     //Slider Animation
    $(".image_reel").fadeOut(500);
    $(".image_reel").animate({ left: -image_reelPosition }, 0 );
    $(".image_reel").fadeIn(500);
   
    };

	//Rotation + Timing Event
	rotateSwitch = function(){	
	$(".slidertitle").eq( $('.paging a.active').attr("rel") - 1 ).slideDown('slow', 'easeOutBounce');	
	$(".slidertext").eq( $('.paging a.active').attr("rel") - 1 ).slideDown('slow', 'easeOutBounce');	
	    play = setInterval(function(){ //Set timer - this will repeat itself every 3 seconds
	        $active = $('.paging a.active').next();
	        if ( $active.length === 0) { //If paging reaches the end...
	            $active = $('.paging a:first'); //go back to first
	        }
	        rotate(); //Trigger the paging and slider function
	    }, 5000); //Timer speed in milliseconds (3 seconds)	
	
	};
	
	rotateSwitch(); //Run function on launch

 //On Click
    $(".paging a").click(function() {    
        $active = $(this); //Activate the clicked paging
        //Reset Timer
        clearInterval(play); //Stop the rotation
        rotate(); //Trigger rotation immediately
        rotateSwitch(); // Resume rotation
        return false; //Prevent browser jump to link anchor
    });    

});
