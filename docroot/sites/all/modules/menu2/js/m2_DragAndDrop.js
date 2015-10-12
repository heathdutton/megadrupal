(function ($) {


  var m2_mouse_x = 0,
      m2_mouse_y = 0,
      m2_body_el = null,
   /* drag process */
      m2_drag_el = null,
      m2_drag_el_start_x = 0,
      m2_drag_el_start_y = 0,
      m2_drag_el_touch_x = 0,
      m2_drag_el_touch_y = 0,
      m2_drag_in_process = false,
   /* drop process */
      m2_drop_el_set = $(''),
      m2_drop_params = {};



  $.fn.m2Drag = function() {
    m2_body_el = $('body');
    this.addClass('m2-drag-el');
    
 /* listen mousedown */
    this.mousedown(function(event){
      if (event.which == 1) {
     /* drag process */
        m2_drag_el = $(this);
        m2_drag_el_start_x = m2_drag_el.get(0).offsetLeft;
        m2_drag_el_start_y = m2_drag_el.get(0).offsetTop;
        m2_drag_el_touch_x = $(document).scrollLeft() + event.clientX - m2_drag_el_start_x;
        m2_drag_el_touch_y = $(document).scrollTop() + event.clientY - m2_drag_el_start_y;
        m2_drag_el.addClass('m2-drag-el-in-drag-process');
        m2_body_el.addClass('m2-in-drag-process');
        if (m2_drop_params.drag_parent_el) {
          m2_drag_el.closest(m2_drop_params.drag_parent_el).addClass('m2-drag-el-parent');
        }
        m2_is_intersect_cache_init();
        m2_drag_in_process = true;
     /* drop process */
        m2_drop_draw_hover();
      }
    });

 /* listen mousemove */
    $(document).mousemove(function(event) {
      m2_mouse_x = event.clientX;
      m2_mouse_y = event.clientY;
      if (m2_drag_in_process && m2_drag_el) {
     /* drag process */
        m2_drag_el.get(0).style.left = ($(document).scrollLeft() + m2_mouse_x - m2_drag_el_touch_x) + 'px';
        m2_drag_el.get(0).style.top = ($(document).scrollTop() + m2_mouse_y - m2_drag_el_touch_y) + 'px';
     /* drop process */
        m2_drop_draw_hover();
      }
    });

 /* listen scroll */
    $(window).scroll(function() {
      if (m2_drag_in_process && m2_drag_el) {
     /* drag process */
        m2_drag_el.get(0).style.left = ($(document).scrollLeft() + m2_mouse_x - m2_drag_el_touch_x) + 'px';
        m2_drag_el.get(0).style.top = ($(document).scrollTop() + m2_mouse_y - m2_drag_el_touch_y) + 'px';
     /* drop process */
        m2_drop_draw_hover();
      }
    });

 /* listen mouseup */
    $(document).mouseup(function(){
   /* drop process */
      if (m2_drag_in_process && m2_drag_el && typeof m2_drop_params.on_drop == 'function') {
        var is_found_active_el = false;
        m2_drop_el_set.each(function(){
          if (m2_is_intersect(m2_drag_el, $(this))) {
            m2_drag_el.get(0).style.left = null;
            m2_drag_el.get(0).style.top = null;
            m2_drop_params.on_drop(m2_drag_el, $(this));
            is_found_active_el = true;
            return false;
          }
        });
        if (is_found_active_el == false) {
          m2_drag_el.animate({left:m2_drag_el_start_x, top:m2_drag_el_start_y}, 100, function(){
            $(this).get(0).style.left = null;
            $(this).get(0).style.top = null;
          });
        }
      }
   /* drag process */
      m2_body_el.removeClass('m2-in-drag-process');
      $('.m2-drag-el-in-drag-process').removeClass('m2-drag-el-in-drag-process');
      $('.m2-drag-el-parent').removeClass('m2-drag-el-parent');
      $('.m2-drop-el-hover').removeClass('m2-drop-el-hover');
      m2_drag_el = null;
      m2_drag_in_process = false;
    });

 /* document on load */
    $(document).ready(function(){
      m2_body_el.addClass('m2-drag-and-drop-processed');
    });

  };


  $.fn.m2Drop = function(params) {
    this.addClass('m2-drop-el');
    m2_drop_el_set = this;
    if (params) {
      m2_drop_params = params;
    }
  };


/* draw hover state for drop place */
  function m2_drop_draw_hover(){
    var drag_el_mid_cache_x = m2_drag_el.get(0).offsetLeft + parseInt(m2_drag_el.get(0).clientWidth  / 2),
        drag_el_mid_cache_y = m2_drag_el.get(0).offsetTop + parseInt(m2_drag_el.get(0).clientHeight / 2);
    m2_drop_el_set.removeClass('m2-drop-el-hover');
    m2_drop_el_set.each(function(){
      if (m2_is_intersect(m2_drag_el, $(this), drag_el_mid_cache_x, drag_el_mid_cache_y)) {
        $(this).addClass('m2-drop-el-hover');
        return false;
      }
    });
  }


/* intersecting */
  function m2_is_intersect_cache_init(){
    m2_drop_el_set.each(function(){
      var drop_el_dom = $(this).get(0);
      drop_el_dom.min_x = drop_el_dom.offsetLeft;
      drop_el_dom.min_y = drop_el_dom.offsetTop;
      drop_el_dom.max_x = drop_el_dom.min_x + drop_el_dom.clientWidth;
      drop_el_dom.max_y = drop_el_dom.min_y + drop_el_dom.clientHeight;
    });
  }

  function m2_is_intersect(drag_el, drop_el, drag_el_mid_cache_x, drag_el_mid_cache_y){
    var drag_el_mid_x = (drag_el_mid_cache_x ? drag_el_mid_cache_x : drag_el.get(0).offsetLeft + parseInt(drag_el.get(0).clientWidth  / 2)),
        drag_el_mid_y = (drag_el_mid_cache_y ? drag_el_mid_cache_y : drag_el.get(0).offsetTop + parseInt(drag_el.get(0).clientHeight / 2)),
        drop_el_min_x = drop_el.get(0).min_x,
        drop_el_min_y = drop_el.get(0).min_y,
        drop_el_max_x = drop_el.get(0).max_x,
        drop_el_max_y = drop_el.get(0).max_y;
    return drag_el_mid_y > drop_el_min_y && drag_el_mid_y < drop_el_max_y &&
           drag_el_mid_x > drop_el_min_x && drag_el_mid_x < drop_el_max_x;
  }


})(jQuery);