/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
(function($) {
  var stub_showing = false;
  
  //Implements function to display the bar and add animations and CSS.
  function headbar_show() {
    if(stub_showing) {
      $('.headbar-stub').slideUp('fast', function() {
        $('.headbar').show();
        $('.headbar').show('bounce', { times:3, distance:15 }, 100);
        //$('body').animate({"marginTop": "2.4em"}, 250);
      });
    }
    else {
      $('.headbar').show();
      $('.headbar').show('bounce', { times:3, distance:15 }, 100);
      //$('body').animate({"marginTop": "2.4em"}, 250);
    }
    $("#header-full-width").css("top","2.8em");
    $("#featured, #breadcrumb").animate({ "margin-top": "28px" }, 250 );
  }
  
  //Implements function to hide the bar and add animations and CSS.
  function headbar_hide() {
    $('.headbar').slideUp('fast', function() {
      $('.headbar-stub').show();
      $('.headbar-stub').show('bounce', { times:3, distance:15 }, 100);
      stub_showing = true;
    });
    if ($(window).width() > 1024) {
      // if width greater than 1024 pull up the body
      $('body').animate({"marginTop": "0px"}, 250);
    }
    $("#featured, #breadcrumb").animate({ "margin-top": "0px" }, 250 );
   }

  Drupal.behaviors.headbar = {
    attach: function (context, settings) {
      var $color = settings.headbar_settings.color;
      var $color_hover = settings.headbar_settings.color_hover;
      var $delaytime = settings.headbar_settings.delaytime;

      $('.headbar, .close-notify, .show-notify').css('background-color', $color);
      $('.headbar-down-arrow').mouseover(function() {
        $('.headbar-down-arrow').css('background-color', $color_hover);
      });
      $('.headbar-down-arrow').mouseout(function() {
        $('.headbar-down-arrow').css('background-color', $color);
      });
      
      window.setTimeout(function() {
        var headbar_cookie = headbarGetCookie("headbar_cookie");
        if(headbar_cookie == "hide") {
          headbar_hide();
        } else {
          headbar_show();
        }

      }, $delaytime);

      $(".show-notify").click(function() {
        headbarSetCookie("headbar_cookie", "show");
        headbar_show();
      });
      
      $(".close-notify").click(function() {
        headbarSetCookie("headbar_cookie", "hide");
        headbar_hide();
      });
    }
  };
  
  //Set cookie on browser.
  function headbarSetCookie(cname,cvalue,exdays){
    path = "path=/";
    var d = new Date();
    d.setTime(d.getTime()+(exdays*24*60*60*1000));
    var expires = "expires="+d.toGMTString();
    document.cookie = cname + "=" + cvalue + "; " + expires +";"+ path ;
  }
  
  //Get cookie which was set.
  function headbarGetCookie(cname){
    var name = cname + "=";
    var ca = document.cookie.split(';');
    for(var i=0; i<ca.length; i++){
      var c = ca[i].trim();
      if (c.indexOf(name)==0) return c.substring(name.length,c.length);
    }
    return "";
  }
})(jQuery);
