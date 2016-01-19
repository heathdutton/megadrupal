// ViewPort script
jQuery(function($) {
  // Create the iFrame
  // @todo: js map, reduce methods
  var browser_limitations = $.browser.msie || $.browser.mozilla || $.browser.opera;
  if (isAdaptiveTesterWindow() || browser_limitations) {
    return;
  }
  var block_id = 'adaptive-tester-block';
  var container = '#' + block_id + ' .adaptive-wrapper';
  
  add_adaptive_layout_tester_wrapper_html();
  var docWidth  = $(document).width();
  
  $('.adaptive-init-button').click(function(){
    $('#' + block_id).removeClass('hidden');
    var iframe = $(document.createElement('iframe'));

    // Set iFrame attributes
    iframe.addClass('viewport');
    iframe.attr({
      src: window.location.href + '#adaptive-tester'
    });
    // Append to body
    $(container).append(iframe);
    set_container_parameters(getCurrentWidth(), 0);
  });
  
  $(container).find('.panel li').live("click", function() {
    // For any list item add/remove 'active' class
    $(this).addClass('active').siblings().removeClass('active');
    // If the class is the reset button, reset the width
    // Else, animate the viewport and add any scrollbar widths
    var width = ($(this).hasClass('reset')) ? docWidth : this.getAttribute('data-width');
    set_container_parameters(width, 'slow');
  });
  
  $(container).find('.close.button').click(function() {
    $(container).find('iframe').remove();
    $('#' + block_id).addClass('hidden');
  });
  
  // New width submit
  $('.custom-width-wrapper .submit-button').click(function() {
      // Get the input value (Submit)
      var inputValue = $('#width').val();
      var hasSpace = /\s/g.test(inputValue);
      // If the input doesn't have a value, show error
      // Else, create a new list element and append data-* attribute
      var msg_container = $('<div class="form-messages"></div>');
      $('.custom-width-wrapper').append(msg_container);
      if(!inputValue) {
        $('#width').addClass('error');
        add_form_messages('Input width value');
      }
      else {
        // Check if any letters or spaces are in the value
        // Append li attributes with custom width
        if(isNaN(inputValue) || hasSpace) {
          add_form_messages('Numbers only please, don\'t add the \'px\'');
          $('#width').val('');
        }
        else {
          var li = $('<li>' + inputValue + 'px</li>');
          li.attr({
            "data-width": inputValue,
            "class": 'custom-width'
          });
          $('.custom-width-wrapper .form-messages').remove();
          $(container).find('.panel').append(li);
        }
      }
      return false;
  });
  
  $(window).resize(function(){
    set_container_parameters(getCurrentWidth(), 0);
  });
  
  function isAdaptiveTesterWindow() {
    if (window.location.hash == '#adaptive-tester') {
      return true;
    }
    return false;
  }
  
  function add_adaptive_layout_tester_wrapper_html() {
    var viewports = ['320', '480', '540', '600', '768', '900', '1024', '1280', '1440'];
    var html = '<div class="adaptive-init-button"></div>';
    html += '<div id="' + block_id + '" class="hidden">';
    html += '<div class="adaptive-bg"></div>';
    html += '<div class="adaptive-wrapper">';
    html += '<div class="close button">Close Window</div>'
    html += '<ul class="panel">';
    html += '<li class="reset active">Reset</li>';
    var current_item;
    for (var key in viewports) {
      current_item = viewports[key];
      html += '<li data-width="' + current_item + '">' + current_item + "px" + '</li>';
    }
    html += '</ul>';
    html += '</div>';
    html += '</div>';
    $('body').append(html);
    add_adaptive_layout_tester_custom_width_form();
  }
  
  function add_adaptive_layout_tester_custom_width_form() {
    var custom_width_wrapper, input, button, messages;
    
    custom_width_wrapper = $('<div class="custom-width-wrapper"></div>');
    
    input = $('<input />');
    input.attr({
      type : 'text',
      id : 'width',
      placeholder: 'Input your custom width'
    });
    
    button = $('<input />');
    button.attr({
      type : 'button',
      "class": 'submit-button'
    });
    button.val('Add');
    
    custom_width_wrapper.append(input, button);
    custom_width_wrapper.insertBefore($(container).find('.panel'));
  }
  
  function get_scrollbar_width() {
    var scrollBars = document.createElement('div');
    scrollBars.className = 'scroll-test';
    document.body.appendChild(scrollBars);

    // Find the scrollbar width
    var scrollbarWidth = scrollBars.offsetWidth - scrollBars.clientWidth;
    document.body.removeChild(scrollBars);
    return scrollbarWidth;
  }
  
  function set_container_parameters(width, speed) {
    var positionLeft, frameWidth;
    var winWidth = $(window).width.bind($(window));
    var winHeight = $(window).height.bind($(window));
    var scrollBarWidth = get_scrollbar_width();
    var container_padding = $(container).outerHeight() - $(container).height();
    var positionTop = 0.05*winHeight();
    
    if (width == winWidth()) {
      frameWidth = winWidth();
      positionLeft = 0;
    }
    else {
      frameWidth = +width + +scrollBarWidth;
      positionLeft = (winWidth() - frameWidth) / 2;
      positionLeft = (positionLeft > 0) ? positionLeft : 0;
    }
    
    $(container).animate({width: frameWidth, left: positionLeft, top: positionTop},{
      duration: speed,
      complete: function(){
        var frameHeight = 0.9*winHeight() - container_padding - $(container).find('.panel').outerHeight() - $('.custom-width-wrapper').height();
        $(container).find('iframe').css({height: frameHeight});
      },
      step: function(now, fx){
        if (fx.prop == 'width') {
          $(container).find('iframe').css('width', now);
          var frameHeight = 0.9*winHeight() - container_padding - $(container).find('.panel').outerHeight() - $('.custom-width-wrapper').height();
          $(container).find('iframe').css('height', frameHeight);
        }
      }
    });
  }
  
  // Get current width based on active item of panel.
  function getCurrentWidth() {
    var current_item = $(container).find('.panel li.active');
    var width = (current_item.hasClass('reset')) ? $(window).width() : current_item.attr('data-width');
    return width;
  }
  
  function add_form_messages(message){
    var msg_container = $('<div class="form-messages">' + message + '</div>');
    $('.custom-width-wrapper .form-messages').replaceWith(msg_container);
  }
});