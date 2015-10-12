/**
 * @file
 * Randomness.
 */

(function($) {

  var default_options = {
    test: 'test',
  };

  $.randomness = function(options) {

    if ($.randomness.set_options(options)) {
      if ($.randomness.init($.randomness.options.mode)) {
        $.randomness.start();
      }
    }
    return;
  };

  $.randomness.start = function() {

    for (x = 0; x < $.randomness.options.particle_count; x++){
      $.randomness.animation(
        $.randomness.init_item(x)
      );
    }
  };

  $.randomness.init_item = function(index){

    opts = {
     top: $.randomness.offset_top + $.randomness.get_random_int(($.randomness.canvas_height)),
     left:  $.randomness.offset_left + $.randomness.get_random_int(($.randomness.canvas_width)),
     'z-index': $.randomness.get_random_int($.randomness.options.particle_count) * $.randomness.options.z_index_mod
    }

    if (typeof($.randomness.options.mode) == "undefined" || $.randomness.options.mode == undefined || $.randomness.options.mode == null || $.randomness.options.mode == "point") {

      item = $('<span>').css(opts).addClass("randomness-particle").html(".");
      $($.randomness.options.canvas).append(item);
      return item;
    }
    else if ($.randomness.options.mode == 'image') {

      idx = x;
      if ($.randomness.options.particle_random == false && x >= ($.randomness.options.images).length) {
        idx = x - (parseInt(x / ($.randomness.options.images).length) * ($.randomness.options.images).length)
      }
      if ($.randomness.options.particle_random == true) {
        idx = $.randomness.get_random_int(($.randomness.options.images).length);
      }
      img = $.randomness.options.images[idx];

      var img_src = "";
      if (img != undefined) {
        img_src = img;
      }

      item = $('<img>').attr({'src': img_src}).addClass("randomness-particle");
      item.css(opts);
      $($.randomness.options.canvas).append(item);

      return item;
    }
    else if ($.randomness.options.mode == 'link_custom') {

      idx = x;
      if ($.randomness.options.particle_random == false && x >= ($.randomness.options.links_custom).length) {
        idx = x - (parseInt(x / ($.randomness.options.links_custom).length) * ($.randomness.options.links_custom).length)
      }
      if ($.randomness.options.particle_random == true) {
        idx = $.randomness.get_random_int(($.randomness.options.links_custom).length);
      }
      link_custom = $.randomness.options.links_custom[idx];

      var a_href = "";
      var a_html = "";
      var a_title = "";
      var a_class = "";
      if (link_custom != undefined) {
        a_href = link_custom.href;
        a_html = link_custom.label;
        a_title = link_custom.title;
        a_class = link_custom.class;
      }

      item = $('<a>').attr({'href': a_href, 'title': a_title}).addClass(a_class).addClass("randomness-particle").html(a_html);
      item.css(opts);
      $($.randomness.options.canvas).append(item);

      return item;
    }
  }

  $.randomness.animation = function(item) {

    var acc_rand_base = $.randomness.options.particle_count;
    var acc_summand = 0;
    var acc_factor = 1000;

    if ($.randomness.options.acceleration == 'slow') {
      acc_rand_base = 10;
      acc_summand = 10;
    }
    else if ($.randomness.options.acceleration == 'fast') {
      acc_rand_base = 10;
      acc_summand = 5;
      acc_factor = 2000;
    }

    $.randomness.canvas_height = $($.randomness.options.canvas).height();
    $.randomness.canvas_width = $($.randomness.options.canvas).width();

    $(item).animate(
      {
        top: $.randomness.offset_top + $.randomness.get_random_int($.randomness.canvas_height),
        left: $.randomness.offset_left + $.randomness.get_random_int($.randomness.canvas_width)
      },
      (($.randomness.get_random_int(acc_rand_base) + acc_summand) * acc_factor),
      $.randomness.options.easing,
      function(){
        $.randomness.animation(item)
      }
    );
//    $(item).animate({
//        top: $.randomness.offset_top + $.randomness.get_random_int(($.randomness.canvas_height)),
//        left: $.randomness.offset_left + $.randomness.get_random_int(($.randomness.canvas_width))
//      },{
//      	duration: (($.randomness.get_random_int(acc_rand_base) + acc_summand) * acc_factor),
//      	/*easing: 'swing',*/
//      	specialEasing: {
//      	  top: 'swing',
//      	  left: 'swing'
//      	},
//      	complete: function() {
//      		$.randomness.animation(item);
//      	}
//      }
//    );
  };

  $.randomness.get_random_int = function(factor) {

    return parseInt(Math.ceil(factor * Math.random()) - 1);
  }

  $.randomness.stop = function(item) {

    $(item).stop();
  };

  $.randomness.set_options = function(options) {

    $.randomness.options = $.extend({}, default_options, options);

    if ($.randomness.options.mode == 'point') {
      $.randomness.options.max_dimension = 1;
    }
    else if ($.randomness.options.mode == 'image') {
      $.randomness.options.max_dimension = $.randomness.options.max_dimension_image;
    }
    else if ($.randomness.options.mode == 'link_custom') {
      $.randomness.options.max_dimension = $.randomness.options.max_dimension_link_custom;
    }

    if ($.randomness.options.particle_random == 'true') {
      $.randomness.options.particle_random = true;
    }
    else {
      $.randomness.options.particle_random = false;
    }

    $.randomness.offset_top = $.randomness.options.offset_top - ($.randomness.options.max_dimension / 4);
    $.randomness.offset_left = $.randomness.options.offset_left - ($.randomness.options.max_dimension / 4);

    $.randomness.canvas_height = $($.randomness.options.canvas).height();
    $.randomness.canvas_width = $($.randomness.options.canvas).width();

    return true;
  };

  $.randomness.init = function(mode) {

    if (mode == 'image') {
      var img = new Object();
      for (i = 0; i < ($.randomness.options.images).length; i++){
        img[i] = new Image();
        img[i].src = $.randomness.options.images[i];
      }
    }
    return true;
  }
})(jQuery);
