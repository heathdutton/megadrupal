/**
 * @file
 * Drujax js library.
 *
 * This library enables you to add your own custom ajax handlers.
 */

var Drujax = (function($) {
  var p = {},
  _restricted = [],
  _wrapper = '#drujax-main',
  _link = 'a';

  /**
   * Jquery drujax Plugin.
   * Creates jQuery Address links.
   */
  $.fn.drujax = function(fn) {
    var sel;

    if (typeof fn === 'string') {
      sel = fn;
      fn = undefined;
    }

    if (!$(this).attr('address')) {
      var f = function(e) {
        if (e.shiftKey || e.ctrlKey || e.metaKey || e.which === 2) {
          return true;
        }
        if ($(this).is('a')) {
          // Parse path.
          var value = fn ? fn.call(this) :
            /address:/.test($(this).attr('rel')) ? $(this).attr('rel').split('address:')[1].split(' ')[0] :
            $.address.state() !== undefined && !/^\/?$/.test($.address.state()) ?
            $(this).attr('href').replace(new RegExp('^(.*' + $.address.state() + '|\\.)'), '') :
            $(this).attr('href').replace(/^(#\!?|\.)/, '');

          // Check if path is restricted.
          if (!Drujax.isRestricted(value) && value.indexOf('/admin') !== 0 && value.indexOf('/user') !== 0 && value.indexOf('/add') < 0 && value.indexOf('/edit') < 0 && value.indexOf('/delete') < 0) {
            e.preventDefault();
            $.address.value(value);
          } else {
            return true;
          }

        }
      };

      $(sel ? sel : this).live('click', f);
    }

    return this;

  };

  /**
   * Initialize Drujax library.
   */
  function init() {
    initAddress();
  }

  /**
   * Initialize jQuery Address event handler.
   */
  function initAddress() {
    var init = true,
            state = window.history.pushState !== undefined;
    var prevpath = '/';
    $(_link).drujax();
    $.address.state('').init(function() {
    }).change(function(event) {
      $('a:not(.admin-link)').each(function() {
        if ($(this).attr('href') === ($.address.state() + event.path)) {
          $(this).addClass('selected');
        } else {
          $(this).removeClass('selected');
        }
      });

      if (state && init) {

        init = false;

      } else {
        // Loads the page content and inserts it into the content area.
        if(prevpath !== event.value){
          // Set the active link.
          prevpath = event.value;
          $('.active, .active-trail').removeClass('active').removeClass('active-trail');
          $('a[href="' + event.value + '"]').addClass('active');
          // Load the content.
          $.ajax({
            url: $.address.state() + event.value,
            error: function(XMLHttpRequest, textStatus, errorThrown) {
              _errorHandlerExec(XMLHttpRequest.responseText, errorThrown, event.value);
            },
            success: function(data, textStatus, XMLHttpRequest) {
              _handlerExec(data, event.value);
            }
          });
        }
      }

    });
  }

  // Private functions

  /**
   * Default ajax handler
   */
  var _handler = function(data, path) {
      $(_wrapper).html(data.content[_wrapper]);
  };

  /**
   * Default error handler
   */
  var _errorHandler = function(data, error, path) {
    alert(data);
  };

  /**
   * Handler execution
   */
  var _handlerExec = function(data, path) {
    document.title = data.title;
    if (!path.match('^.*overlay.*')) {
      _handler(data, path);
    }
  };

  /**
   * Error Handler execution
   */
  var _errorHandlerExec = function(data, error, path) {
    if (!path.match('^.*overlay.*')) {
      _errorHandler(data, error, path);
    }
  };

  //public functions

  /**
   * Set your custom handler.
   * 
   * @param function func
   *   The handler function.
   */
  p.setHandler = function(func) {
    _handler = func;
  };

  /**
   * Set your custom error handler.
   * 
   * @param function func
   *   The error handler function.
   */
  p.setErrorHandler = function(func) {
    _errorHandler = func;
  };

  /**
   * Checks if a path is restricted.
   * 
   * @param string path
   *   The path you want to check.
   * 
   * @return bool
   *   Returns true if the path is restricted.
   */
  p.isRestricted = function(path){
      path = path.split('?')[0];
      var patterns = _restricted.patterns;
      // Check the page restriction type
      var filter_type = _restricted.type == '0';

      // Set default Boolean value base on filter_type
      var path_restricted = !filter_type;

      // Check $patterns against the current path.
      for(var i in patterns){
        // Turn pattern in to regex.
        var pattern = patterns[i];
        pattern = '^(/)?' + pattern
        .replace('<front>', '/')
        .replace('*','.*') + '$';
        if(path.match(pattern)){
            // Match set value based on the $filter_type.
            path_restricted = filter_type;
        }
      }
      return path_restricted;
  }

  // Presets object.
  p.preset = {};

  /**
   * Example fade Handler.
   *
   * @param object data
   *   The json data object.
   *
   * @param string path
   *   The path that belongs to the json object.
   */
  p.preset.fade = function(data,path){
    var wrapper = $(_wrapper);
    // Fade out.
    wrapper.animate({opacity:0}, 500, function() {
      // When fade out is done set content + fade in.
      wrapper.html(data.content[_wrapper]);
      $(this).animate({opacity:1}, 500);
    });
    // Scroll to top of page.
    $('body,html').scrollTop(0);
  }

  /**
   * Example slide from top Handler.
   *
   * @param object data
   *   The json data object.
   *
   * @param string path
   *   The path that belongs to the json object.
   */
  p.preset.from_top = function(data,path){
    var content = data.content[_wrapper];
    var wrapper = $(_wrapper);
    // Give the wrapper a fixed height and set overflow to hidden.
    wrapper.height(wrapper.height()).css({
      'position':'relative',
      'overflow':'hidden'
    });
    // Setup a simple div structure so we make our transition more easily.
    var newContent = '<div class="drujax-slide" style="position:absolute;width:100%;">\n\
      <div class="drujax-new">' + content + '</div>\n\
      <div class="drujax-old">' + wrapper.html() + '</div>\n\
    </div>';
    // Put the div structure in the drujax wrapper (just temporarly)
    $(_wrapper)
      .html(newContent);

    // Set the necessary selectors for our div structure
    var contentOld = $(_wrapper + ' .drujax-old');
    var contentNew = $(_wrapper + ' .drujax-new');
    var contentSlide = $(_wrapper + ' .drujax-slide');

    // Setup the starting position and animate to the end position.
    contentSlide.css({top:-contentOld.position().top})
    .animate({top:0},500,function(){
      // When animation is done override the div structure
      wrapper.html(content);
    });

    // Animate the wrapper to the newHeight of our content
    wrapper.animate({height:contentNew.height()}, 500);
    // Scroll to top of page.
    $('body,html').scrollTop(0);
  }

  /**
   * Example slide from bottom Handler.
   *
   * @param object data
   *   The json data object.
   *
   * @param string path
   *   The path that belongs to the json object.
   */
  p.preset.from_bottom = function(data,path) {
    var content = data.content[_wrapper];
    var wrapper = $(_wrapper);
    // Give the wrapper a fixed height and set overflow to hidden.
    wrapper.height(wrapper.height()).css({
      'position':'relative',
      'overflow':'hidden'
    });
    var newContent = '<div class="drujax-slide" style="position:absolute;width:100%;">\n\
      <div class="drujax-old">' + wrapper.html() + '</div>\n\
      <div class="drujax-new">' + content + '</div>\n\
    </div>';
    // Put the div structure in the drujax wrapper (just temporarly)
    $(_wrapper)
      .css({'position':'relative'})
      .html(newContent);

    // Set the necessary selectors for our div structure
    var contentOld = $(_wrapper + ' .drujax-old');
    var contentNew = $(_wrapper + ' .drujax-new');
    var contentSlide = $(_wrapper + ' .drujax-slide');

    // Setup the starting position and animate to the end position.
    contentSlide
    .animate({top:-contentNew.position().top}, 500, function(){
      wrapper.html(content);
    });

    // Animate the wrapper to the newHeight of our content
    wrapper.animate({height:contentNew.height()}, 500);
    // Scroll to top of page.
    $('body,html').scrollTop(0);
  }

  /**
   * Example slide from left Handler.
   *
   * @param object data
   *   The json data object.
   *
   * @param string path
   *   The path that belongs to the json object.
   */
  p.preset.from_left = function(data,path) {
    var content = data.content[_wrapper];
    var wrapper = $(_wrapper);
    // Give the wrapper a fixed height and set overflow to hidden.
    wrapper.height(wrapper.height()).css({
      'position':'relative',
      'overflow':'hidden'
    });
    var newContent = '<div class="drujax-slide" style="position:absolute;width:200%;">\n\
      <div class="drujax-new" style="float:left; width:50%;">' + content + '</div>\n\
      <div class="drujax-old" style="float:left; width:50%;">' + wrapper.html() + '</div>\n\
      <div style="clear:both"></div>\n\
    </div>';
    // Put the div structure in the drujax wrapper (just temporarly)
    $(_wrapper)
      .css({'position':'relative'})
      .html(newContent);

    // Set the necessary selectors for our div structure
    var contentOld = $(_wrapper + ' .drujax-old');
    var contentNew = $(_wrapper + ' .drujax-new');
    var contentSlide = $(_wrapper + ' .drujax-slide');

    // Setup the starting position and animate to the end position.
    contentSlide
    .css({left:-contentOld.position().left})
    .animate({left:0},500,function(){
      wrapper.html(content);
    });

    // Animate the wrapper to the newHeight of our content
    wrapper.animate({height:contentNew.height()}, 500);
    // Scroll to top of page.
    $('body,html').scrollTop(0);
  }

  /**
   * Example slide from right Handler.
   *
   * @param object data
   *   The json data object.
   *
   * @param string path
   *   The path that belongs to the json object.
   */
  p.preset.from_right = function(data,path) {
    var content = data.content[_wrapper];
    var wrapper = $(_wrapper);
    // Give the wrapper a fixed height and set overflow to hidden.
    wrapper.height(wrapper.height()).css({
      'position':'relative',
      'overflow':'hidden'
    });
    var newContent = '<div class="drujax-slide" style="position:absolute;width:200%;">\n\
      <div class="drujax-old" style="float:left; width:50%;">' + wrapper.html() + '</div>\n\
      <div class="drujax-new" style="float:left; width:50%;">' + content + '</div>\n\
      <div style="clear:both"></div>\n\
    </div>';
    // Put the div structure in the drujax wrapper (just temporarly)
    $(_wrapper)
      .css({'position':'relative'})
      .html(newContent);

    // Set the necessary selectors for our div structure
    var contentOld = $(_wrapper + ' .drujax-old');
    var contentNew = $(_wrapper + ' .drujax-new');
    var contentSlide = $(_wrapper + ' .drujax-slide');

    // Setup the starting position and animate to the end position.
    contentSlide
    .animate({left:-contentNew.position().left}, 500, function(){
      wrapper.html(content);
    });

    // Animate the wrapper to the newHeight of our content
    wrapper.animate({height:contentNew.height()}, 500);
    // Scroll to top of page.
    $('body,html').scrollTop(0);
  }

  // Drujax initializer.
  Drupal.behaviors.drujax = {
    attach: function(context, settings){
      // Set the drujax preset.
      if (Drujax.preset[settings.drujax.handler] !== undefined) {
        Drujax.setHandler(Drujax.preset[settings.drujax.handler]);
      }
      // Set the wrapper id.
      _wrapper = settings.drujax.wrapper;
      // Set the restricted paths
      _restricted = $.parseJSON(settings.drujax.restricted);
      // Link selector
      _link = settings.drujax.link
      // initialize drujax.
      init();
    }
  };

  return p;
})(jQuery);
