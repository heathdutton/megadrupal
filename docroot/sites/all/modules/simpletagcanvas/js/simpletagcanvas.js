/**
 * Defines the TagCanvas Block
 */
(function ($) {
  Drupal.behaviors.simpletagcanvas = {
    attach: function(context, settings) {

      var tagcanvas_option_shape = Drupal.settings.simpletagcanvas.simpletagcanvas_shape;
      var tmp_tagcanvas_option_mousewheelzoom = Number(Drupal.settings.simpletagcanvas.simpletagcanvas_mousewheelzoom);
      var tagcanvas_option_mousewheelzoom = Boolean(tmp_tagcanvas_option_mousewheelzoom);
      var tagcanvas_option_lock = Drupal.settings.simpletagcanvas.simpletagcanvas_lock;
      var tagcanvas_option_minspeed = Drupal.settings.simpletagcanvas.simpletagcanvas_minspeed;
      var tagcanvas_option_maxspeed = Drupal.settings.simpletagcanvas.simpletagcanvas_maxspeed;

      if(!$('#mySimpleTagCanvas').tagcanvas({
          shape:     tagcanvas_option_shape,
          wheelZoom: tagcanvas_option_mousewheelzoom,
          lock:      tagcanvas_option_lock,
          minSpeed:  tagcanvas_option_minspeed,
          maxSpeed:  tagcanvas_option_maxspeed,
          textColour: null,
          outlineColour: '#ff0000',
          reverse: true,
          depth: 0.8,
        },'tags')) {
          // something went wrong, hide the canvas container
          $('#myCanvasContainer').hide();
        }

        // getting the canvas element
        var c = $('#mySimpleTagCanvas');
        var ct = c.get(0).getContext('2d');
        var container = $(c).parent();

         //handling the canvas resize
        $(window).resize(respondCanvas);
        function respondCanvas(){
          // makign the canvas fill its container
          //max width
          c.attr('width', $(container).width());
          //max height
          c.attr('height', ($(container).height()));
        }
        respondCanvas();
      }
    }
  })(jQuery);
