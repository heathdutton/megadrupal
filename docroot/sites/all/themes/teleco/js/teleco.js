/* $Id$ */
(function ($) {
  $(document).ready(function(){
		$( ".nav-toggles" ).click(function() {
  		$("#primary").toggle()
	});
			$( ".nav2-toggles" ).click(function() {
  		$("#secondary-links").toggle()
	});
	$("#primary ul li a.active").each(function(index){
	  if(index == 0){
	    var text_val = $(this).html();
	    $("a.nav-toggles").html(text_val);
	  }
	});
	$("#secondary-links ul li a.active").each(function(index){
	  if(index == 0){
	    var text_val = $(this).html();
	    $("a.nav2-toggles").html(text_val);
	  }
	});
	var count_images = 0;
	$('#header-slide .slides img').each(function(index){
     count_images = count_images + 1;
	});
	
    setInterval(function () {
	  var already_active = $('#header-slide').hasClass("active");
	  if(already_active == false){
	  var increament_val = 0;
	  $('#header-slide .slides img').each(function(index){
		if(increament_val == 0){
		  var is_active = $(this).hasClass('active');
		  if(is_active == true){
		    $(this).removeClass('active');
			$('#header-slide .dots span').removeClass("active");
			$('#header-slide .dots span img').attr('src', 'sites/all/themes/teleco/slider/slider-dot.png');
		    index = index + 1;
			var check_id = parseInt($(this).attr('id'));
			if(count_images-1 == check_id){
				index = 0;
			}
		    $('#header-slide .slides img.slider-'+index).addClass("active");
			$('#header-slide .dots span.slider-'+index).addClass("active");
			$('#header-slide .dots span.active img').attr('src', 'sites/all/themes/teleco/slider/slider-dot-active.png');
			increament_val = 1;
		  }
		}
	  });
	  }
     },3000);
	 $('#header-slide .dots img').click(function(){
	   var has_active = $(this).parent().hasClass("active");
	   if(has_active == false){
	     var parent_class = $(this).parent().attr("class");
	       $('#header-slide .slides img').removeClass('active');
		   $('#header-slide .dots span').removeClass("active");
		   $('#header-slide .dots span img').attr('src', 'sites/all/themes/teleco/slider/slider-dot.png');
		
	       $('#header-slide .slides img.'+parent_class).addClass('active');
		   $('#header-slide .dots span.'+parent_class).addClass("active");
		   $('#header-slide .dots span.'+parent_class+' img').attr('src', 'sites/all/themes/teleco/slider/slider-dot-active.png');
		   $('#header-slide').addClass("active");
	       setTimeout(function(){
		     $('#header-slide').removeClass("active");			  
	       },3000);
	   }
	  });
	
	
	
});
})(jQuery);


sfHover = function() {
	var sfEls = document.getElementById("primary-inner").getElementsByTagName("LI");
	for (var i=0; i<sfEls.length; i++) {
		sfEls[i].onmouseover=function() {
			this.className+=" sfhover";
		}
		sfEls[i].onmouseout=function() {
			this.className=this.className.replace(new RegExp(" sfhover\\b"), "");
		}
	}
}
if (window.attachEvent) window.attachEvent("onload", sfHover);


$(document).ready(function() {
  // Delayed mouseout.
  $('#primary-inner li').hover(function() {
    // Stop the timer.
    clearTimeout(this.sfTimer);
    // Display child lists.
    $('> ul', this).css({left: '', display: 'block'})
      // Immediately hide nephew lists.
      .parent().siblings('li').children('ul').css({left: '', display: 'none'});
  }, function() {
    // Start the timer.
    var uls = $('> ul', this);
    this.sfTimer = setTimeout(function() {
      uls.css({left: '-999em', display: 'none'});
    }, 400);
  });
});
