(function ($) {
	
	$(document).ready(function () { 
		console.log('in swipe_photo_gallery.js');
		
		if($("#slideshow-page").length){
		
			document.addEventListener('touchstart', touchStart);
			document.addEventListener('keydown', keyPress);
			var $m = $('body').modal();
	        var api = $m.data('modal');

	        console.log('var $m = $(body).modal()');
	        console.log($m);
	        console.log('api = $m.data(modal)');
	        console.log(api);

	        console.log('#slideshow-page');
	      
	    	page_path = Drupal.settings.myModule.page_path;
	    	console.log(page_path);

	        $.get(page_path + "?ajax=true", function(response){

	        	var html = response.split("!SPLIT!");
	        	html = '<div class="photo-gallery-img cur-img">' + html[0] + '</div>' + html[1] + '<div class="photo-gallery-nav vertical-nav" id="nav-top"></div><div class="photo-gallery-nav vertical-nav" id="nav-bottom"></div><div class="photo-gallery-nav horizontal-nav" id="nav-left"></div><div class="photo-gallery-nav horizontal-nav" id="nav-right"></div>';
	        	if (('ontouchstart' in window) || (navigator.maxTouchPoints > 0) || (navigator.msMaxTouchPoints > 0)) {
					html += '<div id="swipe-block" class="swipe-block"></div>';
				}
				console.log('start API');
				api.opts.wrapperClass = 'modal-photo-gallery';
	            api.opts.width = '100%';
	            api.opts.height = '100%';
	            api.opts.maxWidth = '100%',
	            api.opts.maxHeight = '100%',
	            api.opts.closeText = '';
	            api.opts.fixed = true;
	            console.log('before api.open');
	            api.open(html);
	            console.log('after api.open');
	            img_size();
	            var series = $(".series-val").val();
	            var nid = $(".nid").val();
	            if(series.length > 0)
	            	series_init(nid, series);
	            $(".main-nids div:first").addClass("active");
			});

			$(".vertical-nav:not(.disabled)").live("click", function(){
				var series = $(".series-val").val();
				if(series.length>0) {
					$(".photo-gallery-nav").addClass("disabled");					   		
					var direction = $(this).attr("id");
					vertical_nav(direction);
				}
				return false;
			});

			$(".horizontal-nav:not(.disabled)").live("click", function(){
				$(".photo-gallery-nav").addClass("disabled");					   		
				var direction = $(this).attr("id");			
				horizontal_nav(direction);
				return false;
			});

			$$("#swipe-block:not(.disabled)").swipeLeft(function() {
				event.preventDefault();
				$("#swipe-block").addClass("disabled");
			    horizontal_nav("nav-right");
			});

			$$("#swipe-block:not(.disabled)").swipeRight(function() {
				event.preventDefault();
				$("#swipe-block").addClass("disabled");
			    horizontal_nav("nav-left");
			});

			$$("#swipe-block:not(.disabled)").swipeUp(function() {
				event.preventDefault();
				var series = $(".series-val").val();
				if(series.length>0) {
					$("#swipe-block").addClass("disabled");
				    vertical_nav("nav-bottom");
				}
			});

			$$("#swipe-block:not(.disabled)").swipeDown(function() {
				event.preventDefault();
				var series = $(".series-val").val();
				if(series.length>0) {
					$("#swipe-block").addClass("disabled");
				    vertical_nav("nav-top");
				}
			});

			$(".modal-photo-gallery .modal-close").live("click", function(event) {
				document.removeEventListener('touchstart', touchStart);
				document.removeEventListener('keydown', keyPress);
				closeModal(event);
                                if(history.length > 2){
                                    history.back();
                                }else{
                                    location="http://carolsachs.com/home";
                                }				
		    });	

			$(".photo-gallery-tax a").live("click", function(event) {
				return false;
		    });
		}
	});

	function touchStart(event) {
        var attr = event.target.className;
        if(attr!="modal-close") {
        	event.preventDefault();
        }

        return true;
    }

	function closeModal(e) {

	    var $m = $('body').modal(),
	    api = $m.data('modal');
	    api.hide('',e);
	}

	function img_size() {
		$(".modal-content").children(".photo-gallery-img").each(function(ind, element){
			var img = $(element).find("img");
			img.attr({"width":"", "height":""});
			//img.css({"max-width":"100%", "max-height":"100%"});
			img.css({"max-width":"85%", "max-height":"85%"});
		});
	}

	function series_init(nid, series) {
		$.ajax({
			type: "POST",
			//url: "overview",
			url: page_path,
			data: "nid=" + nid + "&series=" + series,
			success: function(response){
				$(".modal-content .series-nids").remove();
				$(".modal-content").append(response);			
			}
		});
	}

	function gallery_nav(next_nid, direction) {
		direction = direction.replace("nav-", "");
        $(".modal-content").append('<div class="photo-gallery-img next-img"></div>');
		switch (direction) {
		   	case "top":
		   		var position = -window.innerHeight * 2;
				$(".next-img").css("top", position + "px");
				$(".next-img").html($('.main-nids .active .series-nid[name="' + next_nid + '"]').val());
				img_size();
                $(".next-img img").on('load', function(){
					$(".cur-img").stop().animate({bottom: position + "px"}, {
						duration: 500, 
						complete: function(){
							$(this).remove();										
						}
					});
                	$(".next-img").stop().animate({top: 0}, {
						duration: 500,
						complete: function(){
                            $(".nid").val(next_nid);
							$(".photo-gallery-nav, #swipe-block").removeClass("disabled");
						}
					}).removeClass("next-img").addClass("cur-img");            
                });
				break
		   	case "bottom":
		   		var position = -window.innerHeight * 2;
				$(".next-img").css("bottom", position + "px");
				$(".next-img").html($('.main-nids .active .series-nid[name="' + next_nid + '"]').val());
				img_size();

                $(".next-img img").on('load', function(){
					$(".cur-img").stop().animate({top: position+"px"}, {
						duration: 500, 
						complete: function(){
							$(this).remove();										
						}
					});
					$(".next-img").stop().animate({bottom: 0}, {
					duration: 500,
					complete: function(){
                        $(".nid").val(next_nid);
						$(".photo-gallery-nav, #swipe-block").removeClass("disabled");
					}
					}).removeClass("next-img").addClass("cur-img");
                });
				break
		   	case "left":
			   	var position = -window.innerWidth * 2;
				$(".next-img").css("left", position + "px");
				$(".next-img").html($('.main-nids .active .series-nid[name="' + next_nid + '"]').val());
                                                
				img_size();
                $(".next-img img").on('load', function(){
				$(".cur-img").stop().animate({right: position+"px"}, {
					duration: 500, 
					complete: function(){
						$(this).remove();										
					}
				});
				$(".next-img").stop().animate({left: 0}, {
					duration: 500,
					complete: function(){
                        $(".nid").val(next_nid);
						$(".photo-gallery-nav, #swipe-block").removeClass("disabled");
					}
				}).removeClass("next-img").addClass("cur-img");
                                                });
				break
		   	case "right":
		   		var position = -window.innerWidth*2;
				$(".next-img").css("right", position+"px");
				$(".next-img").html($('.main-nids .active .series-nid[name="'+next_nid+'"]').val());
                                                
				img_size();
				$(".next-img img").on('load', function(){
					$(".cur-img").stop().animate({left: position+"px"}, {
						duration: 500, 
						complete: function(){
							$(this).remove();										
						}
					});
					$(".next-img").stop().animate({right: 0}, {
						duration: 500,
						complete: function(){
                            $(".nid").val(next_nid);
							$(".photo-gallery-nav, #swipe-block").removeClass("disabled");
						}
					}).removeClass("next-img").addClass("cur-img");
                });
				break
		   	default:
				break
		}					
	}

	function vertical_nav(direction) {

		var series = $(".series-val").val();
		if(series.length>0){
			var nid = $(".modal-photo-gallery .nid").val();
			var next_nid = (direction == "nav-top") ? $(".main-nids .active .series-nid[name='"+nid+"']").prev(".series-nid").attr('name') : $(".main-nids .active .series-nid[name='"+nid+"']").next(".series-nid").attr('name');
			if (next_nid == undefined) {
				next_nid = (direction == "nav-top") ? $(".main-nids .active").find(".series-nid").last().attr('name') : $(".main-nids .active").find(".series-nid").first().attr('name');
			}
			
			gallery_nav(next_nid, direction); 				
		}
	}

	function horizontal_nav(direction) {

		var nid = $(".nid").val();
		var next_nid = -1
		if(direction == "nav-left") {
			next_nid = $(".main-nids .active").prev("div").attr('id');
			next_nid = (next_nid == null) ? $(".main-nids .nod:last").attr('id') : next_nid;
		}
		else if(direction == "nav-right") {
			next_nid = $(".main-nids .active").next("div").attr('id');
			next_nid = (next_nid == null) ? $(".main-nids .nod:first").attr('id') : next_nid;
		}
		$(".main-nids .active").removeClass("active");
		$(".main-nids .nod[id = "+next_nid+"]").addClass("active");
		
		$('.tax-top, .tax-bottom').html('<a href="">'+$('.main-nids .active .series_name').val()+'</a>');
                    var left_name = $('.main-nids .active').prev('.nod');
                    left_name = $(left_name).length==0 ? $(".main-nids .nod:last") : left_name;
                    $('.tax-left').html('<a href="">'+$(left_name).find('.series_name').val()+'</a>');
		
                    var right_name = $('.main-nids .active').next('.nod');
                    right_name = $(right_name).length==0 ? $(".main-nids .nod:first") : right_name;
                    $('.tax-right').html('<a href="">'+$(right_name).find('.series_name').val()+'</a>');
                    
		gallery_nav(next_nid, direction);
	}

	function keyPress(event) {
		event.preventDefault();
		if(!$(".photo-gallery-nav").hasClass("disabled")){
			switch (event.which) {
		        case 37:
		        	$(".photo-gallery-nav").addClass("disabled");
		            horizontal_nav("nav-left");
		            break;
		        case 38:
		        	var series = $(".series-val").val();
					if(series.length>0) {
			        	$(".photo-gallery-nav").addClass("disabled");
			            vertical_nav("nav-top");
			        }
		            break;    
		        case 39:
		        	$(".photo-gallery-nav").addClass("disabled");
		            horizontal_nav("nav-right");
		            break;
	            case 40:
	            	var series = $(".series-val").val();
					if(series.length>0) {
			        	$(".photo-gallery-nav").addClass("disabled");
			            vertical_nav("nav-bottom");
			        }
		            break;
		        case 27:
		        	$(".modal-photo-gallery .modal-close").click();
		            break;
		    }
		}
	}
})(jQuery);