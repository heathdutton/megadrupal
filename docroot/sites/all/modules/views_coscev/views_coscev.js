/**
 * @file
 * Jquery module file.
 */

(function ($) {

  Drupal.behaviors.views_coscev = {
      attach:function(context) {

        var _w = $(window).width();
        var _h = $(window).height();
        var _body = $('body', context);
        _body.data('previous_window_gap',0);
        _body.data('total_gap',0);
        var displays = Drupal.settings.views_coscev;

        // Get ordered displays
        $('div[class^="coscev-display-"]').each(function(i, n) {
          var _this = $(this);
          var display_id = $(this).attr('class').replace('coscev-display-', '');
          var display = displays[display_id];

          // Set width of the items.
          if(display.max_width !== undefined) {
            var item_width = views_coscev_px_or_percent(display.max_width);
            $(this).find('div.coscev-item').css('width', item_width);
          }

          $.each(display.lines, function(i, n){
            //  Position items.
            var _target = _this.find('.coscev-item.order-' + n.order);
            var dw = _target.outerWidth();
            var dh = _target.outerHeight();

            switch(n.dir)  {
              case 'top-hor-left':
              case 'bottom-hor-left':
                _target.views_coscev_position_me(dw, _w, 'left', n.order, n.type, display);
                break;

              case 'top-hor-right':
              case 'bottom-hor-right':
                _target.views_coscev_position_me(dw, _w, 'right', n.order, n.type, display);
                 break;

              case 'top-vert-left':
              case 'top-vert-right':
                _target.views_coscev_position_me(dh, _h, 'top', n.order, n.type, display);
                break;

              case 'bottom-vert-left':
              case 'bottom-vert-right':
                _target.views_coscev_position_me(dh, _h, 'bottom', n.order, n.type, display);
                 break;

            }
          });
        });

     // Add total body height to the first display container.
        var cont_height = $('body').data('total_gap') + $('body').data('previous_window_gap');
        $('.cont-views-coscev:first').css('height', cont_height);

        $(window).scroll(function(event){
          var scroll_top = $(this).scrollTop();
          views_coscev_make_move(scroll_top);
        });
        
        // Animate from the navigation bar.
        $('#coscev_navigation_bar a').click(function(e) {
        	e.preventDefault();
        	var tget = $(this).attr('href').substr(1);
        	var end_scroll = $('.coscev-display-'+tget+' h3').data('end_title_scroll');
        	end_scroll = end_scroll > 0 ? end_scroll : 0;
        	var start_scroll = $(window).scrollTop();
        	
        	$({foo:start_scroll}).animate({foo:end_scroll}, {
        		easing: 'easeInOutCubic',
        		duration: 1500,
        	    step: function(val) {
        	    	views_coscev_make_move(val);
                	$(window).scrollTop(val);
        	    }
        	});
        });
      }
  };

  //Calculate new title position.
  $.fn.views_coscev_animate_title = function(scrol) {
    this.each(function(n, i) {
      var scrol_start = $(this).data('start_title_scroll');
      var scrol_end = $(this).data('end_title_scroll');
      var diff_scroll = scrol_end - scrol_start;
      var title_settings = $(this).data('title');

      if(scrol > scrol_start && scrol < scrol_end) {
        var current_scrol = scrol - scrol_start;
        var percentage_croll = Math.round((current_scrol / diff_scroll) * 100);
        var wave_move = (50 - (Math.abs(50 - percentage_croll))) * 2;

        $(this).views_coscev_title_move(title_settings.title_effect, wave_move);

      } else {
        $(this).views_coscev_title_reset(title_settings.title_effect);
      }
    });
  }

  //Move title.
  $.fn.views_coscev_title_move = function(setting, amount) {
    if (setting == 'fade') {
      this.css('opacity', amount / 100);
    } else if (setting == 'popout') {
      // the title should popup more than his width
      var _width = this.width() + this.width() / 2;
      var _left = -(this.width()) + (amount * _width / 100);
      this.css('left', _left).css('visibility','visible');
    } else if (setting == 'rotate') {
      // the title should popup more than his width
      var _deg = 90 - (amount * 90 / 100);
      var _left_max = (this.outerWidth() / 2) + this.outerHeight() / 2;
      var _left = -(_left_max) + (amount * _left_max / 100);
      var _top = 100 - (amount * (this.outerWidth() / 2) / 100);
      this.css('transform', 'rotate(' + _deg + 'deg)')
        .css('left', _left)
        .css('top', _top).css('visibility','visible');
    }
  }

  // Reset title to its initial position (hidden).
  $.fn.views_coscev_title_reset = function(setting) {
    if (setting == 'fade') {
      this.css('opacity', 0);
    } else if (setting == 'popout') {
      this.css('left', -(this.width())).css('visibility','hidden');
    } else if (setting == 'rotate') {
      this.css('visibility','hidden');
    }
  }

  // Move the content item.
  $.fn.views_coscev_position_me = function(item_gap, window_gap, css_property, order, type, display) {
    var center = display.center;
    var overlap = views_coscev_px_or_percent(display.overlap);
    var title_space = parseFloat(display.title.title_space) * $(window).width();
    var _body = $('body');

    // Current position = current size + previous distance.
    previous_distance = _body.data('previous_window_gap');
    previous_total = _body.data('total_gap');

    var new_distance = previous_distance + previous_total + item_gap;

    // Correct the distance with overlap.
    new_distance += overlap;
    // Save start and end point for title.
    // End anim position.
    if($('h3').hasClass('end_title_scroll_to_set')) {
      new_distance += title_space;
      $('h3.end_title_scroll_to_set').data('end_title_scroll', new_distance)
        .removeClass('end_title_scroll_to_set');
    }
    // Start anim position.
    if (type == 'title') {
      $(this).css("margin-" + css_property, - (Math.round((title_space / 3) * 2)))
        .parents('.views-coscev').find('h3.coscev-title')
        .data('start_title_scroll', new_distance)
        .data('title', display.title)
        .addClass('end_title_scroll_to_set');
    }

    // Center first item.
    if (center == 1 && $('.coscev-centered').length < 1) {
      //console.log('centered');
      new_distance = (item_gap - window_gap) / 2;
      $(this).addClass('coscev-centered');
    }

    $('body').data('previous_window_gap', window_gap);
    $('body').data('total_gap', new_distance);

    $(this).css(css_property, -(new_distance));
  }
  
  // Effectively move the title and the content.
  function views_coscev_make_move(scroll_top) {
	  $('h3.coscev-title').views_coscev_animate_title(scroll_top);
      $('.top-hor-left, .bottom-hor-left').css('left', scroll_top);
      $('.bottom-vert-left, .bottom-vert-right').css('bottom', scroll_top);
      $('.top-vert-left, .top-vert-right').css('top', scroll_top);
      $('.bottom-hor-right, .top-hor-right').css('right', scroll_top);
  }

  // Margins can be relative in percent or absolute in pixels.
  function views_coscev_px_or_percent(value) {
    value = value.toString();
    if (value.indexOf('%') !== -1) {
      var max_size = parseInt(value.replace('%',''));
      value = $(window).width() / 100 * max_size;
    }
    return Math.round(parseInt(value));
  }
}(jQuery));
