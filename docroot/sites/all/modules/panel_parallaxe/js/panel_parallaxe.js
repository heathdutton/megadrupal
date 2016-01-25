/**
 * @file
 * Client site javascript to resize images.
 */

function panel_parallaxe_get_image(url) {
  url = url.split('\\').pop().split('/').pop();
  url = url.substring(0,url.lastIndexOf('?'));
  return url;
}

function fullsize() {
  width = jQuery(window).width();
  height = jQuery(window).height();
  size = "parallaxe";
  if (width > height) {
    if (width <= 640) {
      size = size + "_l_640_480";
    }
    if (width > 640 & width <= 720) {
      size = size + "_l_720_540";
    }
    if (width > 720 & width <= 800) {
      size = size + "_l_800_600";
    }
    if (width > 800 & width <= 1024) {
      size = size + "_l_1024_768";
    }
    if (width > 1024 & width <= 1280) {
      size = size + "_l_1280_960";
    }
    if (width > 1280 & width <= 1366) {
      size = size + "_l_1366_1024";
    }
    if (width > 1366) {
      size = size + "_l_1920_1080";
    }
  }
  else {
    if (width <= 480) {
      size = size + "_p_480_640";
    }
    if (width > 480 & width <= 540) {
      size = size + "_p_540_720";
    }
    if (width > 540 & width <= 600) {
      size = size + "_p_600_800";
    }
    if (width > 600 & width <= 768) {
      size = size + "_p_768_1024";
    }
    if (width > 768 & width <= 960) {
      size = size + "_p_960_1280";
    }
    if (width > 960 & width <= 1024) {
      size = size + "_p_1024_1366";
    }
    if (width > 1024) {
      size = size + "_p_1080_1920";
    }
  }

  if (typeof jQuery("section.module.parallax:nth(0)").css("background-image") != 'undefined') {
    image = panel_parallaxe_get_image(jQuery("section.module.parallax:nth(0)").css("background-image"));
    jQuery("section.module.parallax:nth(0)").css("background-image","url('" + Drupal.settings.panel_parallaxe[image][size] + "')");
  }
  if (typeof jQuery("section.module.parallax:nth(1)").css("background-image") != 'undefined') {
    image = panel_parallaxe_get_image(jQuery("section.module.parallax:nth(1)").css("background-image"));
    jQuery("section.module.parallax:nth(1)").css("background-image","url('" + Drupal.settings.panel_parallaxe[image][size] + "')");
  }
  if (typeof jQuery("section.module.parallax:nth(2)").css("background-image") != 'undefined') {
    image = panel_parallaxe_get_image(jQuery("section.module.parallax:nth(2)").css("background-image"));
    jQuery("section.module.parallax:nth(2)").css("background-image","url('" + Drupal.settings.panel_parallaxe[image][size] + "')");
  }

  if (typeof jQuery("section.module.parallax:nth(0)").css("background-image") != 'undefined') {
    jQuery("section.module.parallax:nth(0)").css("max-height",height * 0.382);
  }
  if (typeof jQuery("section.module.parallax:nth(1)").css("background-image") != 'undefined') {
    jQuery("section.module.parallax:nth(1)").css("max-height",height * 0.382);
  }
  if (typeof jQuery("section.module.parallax:nth(2)").css("background-image") != 'undefined') {
    jQuery("section.module.parallax:nth(2)").css("max-height",height * 0.382);
  }

  jQuery("div .fullsize").css("margin-left", "0px");
  jQuery("div .fullsize").css("margin-right", "0px");

  margin = (jQuery(window).width() - jQuery("div .fullsize").width()) / 2;

  jQuery("div .fullsize").css("margin-left", "-" + margin + "px");
  jQuery("div .fullsize").css("margin-right", "-" + margin + "px");
}

var delay = (function(){
  var timer = 0;
  return function (callback, ms) {
    clearTimeout(timer);
    timer = setTimeout(callback, ms);
  };
})();

jQuery(window).resize(function() {
    delay(function(){
      fullsize();
    }, 500);
});

Drupal.behaviors.panel_parallaxeBehavior = {
  attach: function (context, settings) {
    fullsize();
  }
};
