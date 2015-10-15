/**
 * @file
 * Mobile_navigation Library functionality.
 *
 * Author - Christian Galicia
 * Licenses: GPLv2
 */

/*! matchMedia() polyfill - Test a CSS media type/query in JS. Authors & copyright (c) 2012: Scott Jehl, Paul Irish, Nicholas Zakas, David Knight. Dual MIT/BSD license */

window.matchMedia || (window.matchMedia = function() {
  "use strict";

  // For browsers that support matchMedium api such as IE 9 and webkit
  var styleMedia = (window.styleMedia || window.media);

  // For those that don't support matchMedium
  if (!styleMedia) {
    var style       = document.createElement('style'),
        script      = document.getElementsByTagName('script')[0],
        info        = null;

    style.type  = 'text/css';
    style.id    = 'matchmediajs-test';

    script.parentNode.insertBefore(style, script);

    // 'style.currentStyle' is used by IE <= 8 and 'window.getComputedStyle' for all other browsers
    info = ('getComputedStyle' in window) && window.getComputedStyle(style) || style.currentStyle;

    styleMedia = {
      matchMedium: function(media) {
        var text = '@media ' + media + '{ #matchmediajs-test { width: 1px; } }';

        // 'style.styleSheet' is used by IE <= 8 and 'style.textContent' for all other browsers
        if (style.styleSheet) {
          style.styleSheet.cssText = text;
        } else {
          style.textContent = text;
        }

        // Test if media query is true or false
        return info.width === '1px';
      }
    };
  }

  return function(media) {
    return {
      matches: styleMedia.matchMedium(media || 'all'),
      media: media || 'all'
    };
  };
}());


(function($) {
  // Check for iPad/Iphone/Andriod
  var instances = {};

  // Default options
  var defaults = {
    breakpoint : "all and (min-width: 740px) and (min-device-width: 740px), (max-device-width: 800px) and (min-width: 740px) and (orientation:landscape)",
    //"all and (max-width: 740px) and (max-device-width: 740px), (max-device-width: 800px) and (min-width: 740px) and (orientation:landscape)",

    //menuSelector : "#superfish-1",
    //"#megamenu-main-menu",
    //"#main-menu",

    mainPageSelector : "body",

    menuPlugin : "slideMenu",
    //"default",
    //"accordion",

    showEffect : "fixed_left",
    //"expand_down",  /* Expanded from the position where the menu collapsed button is */
    //"fixed_top",
    //"fixed_right",
    //"fixed_bottom",
    //"drawer_left",
    //"drawer_right",
    //"drawer_top",
    showItems : "all",
    //"active_trail",
    tabHandler : false,

    menuWidth : "65",

    specialClasses : true,

    useMask : true,
    
    menuLabel : "Menu"
  };

  $(document).ready(function() {
    $("body").prepend("<div id='debugInfo'></div>");
    $("#debugInfo").css({
      position : "fixed",
      right : "50px",
      top : "50px"
    });
  });

  // Main Class
  var mobile_menu = function(elem, options) {
    var settings, current = "", width, height;

    this.methods = {

      /* destroy */
      destroy : function() {

      },

      cloneMenu : function() {

        switch (_this.settings.showEffect) {
          case "expand_down":
            $(_this.target).before("<div id='mobile-menu-wrapper'><div id='mobile-menu-inner'></div></div>");
            //_this.settings.useMask = false;
          break;

          case "fixed_top":
          case "drawer_top":
          case "fixed_left":
          case "drawer_left":
          case "fixed_right":
          case "drawer_right":
          case "fixed_bottom":
            $("body").prepend("<div id='mobile-menu-wrapper'><div id='mobile-menu-inner'></div></div>");
          break;

        }

        var classes = "";
        switch (_this.settings.menuPlugin) {
          case "basic":
            classes = "mobile-menu-basic";
          break;

          case "accordion":
            classes = "mobile-menu-accordion";
          break;

          case "slideMenu":
            classes = "mobile-menu-slide";
          break;

        }

        // Cloning
        $('#mobile-menu-inner').append("<ul id='mobile-menu' class='" + classes + "'>" + $(_this.target).html() + "</ul>");
        // Clearing up other plugins classes
        $('#mobile-menu li, #mobile-menu ul, #mobile-menu a').attr({
          id : "",
          style : ""
        }).removeClass('active');
        // Adding item-with-ul to submenu items
        $("#mobile-menu li").each(function() {
          if ($(this).find("ul").length) {
            $(this).addClass("item-with-ul");
          }
        });

        $("#mobile-menu li.item-with-ul > *:first-child").each(function() {
          if ($(this).get(0).tagName == "A") {
            $(this).wrap("<div></div>");
          }
          $(this).parent().addClass("submenu-title");
        });

        if (_this.settings.tabHandler) {
          var show = true, dir_class = "";
          switch (_this.settings.showEffect) {
            case "expand_down":
              show = false;
            break;

            case "fixed_top":
            case "drawer_top":
              dir_class = "top";
            break;

            case "fixed_left":
            case "drawer_left":
              dir_class = "left";
            break;

            case "fixed_right":
            case "drawer_right":
              dir_class = "right";
            break;

            case "fixed_bottom":
              dir_class = "bottom";
            break;

          }
          if (show) {
            $("#mobile-menu-inner").prepend("<a id='menu-attached-button' class='mobile-menu-button " + dir_class + "'>" + _this.settings.menuLabel + "</a>");
            $("#menu-attached-button").click(function(e) {
              _this.menuToggle(this);
              e.stopPropagation();
              e.preventDefault();
            });
          }
        }

        $('#mobile-menu-inner').hide();

        // Add special classes to menu.
        if (_this.settings.specialClasses) {
          function addClassesToUl(ul, parentClass) {

            var ii = 1;
            $(ul).find("> li").each(function() {
              if (parentClass == "") {
                var cls = "menuitem-" + ii;
              } else {
                var cls = "menuitem-" + parentClass + "-" + ii;
              }

              $(this).addClass(cls);

              if ($(this).hasClass("item-with-ul")) {

                if (parentClass == "") {
                  addClassesToUl($(this).find("> ul"), ii);
                } else {
                  addClassesToUl($(this).find("> ul"), parentClass + "-" + ii);
                }

              }
              ii++;

            });

          }

          addClassesToUl($("#mobile-menu"), "");
        }

      },

      collapseInAButton : function() {

        if (!$("#collapsed-menu-button").length) {
          // Create button
          if (_this.settings.showEffect == "expand_down") {
            $("#mobile-menu-wrapper").before("<a id='collapsed-menu-button' class='mobile-menu-button'>" + _this.settings.menuLabel + "</a>");
          } else {
            $(_this.target).before("<a id='collapsed-menu-button' class='mobile-menu-button'>" + _this.settings.menuLabel + "</a>");
          }

          $("#mobile-menu-inner").hide();
/*
          $("#collapsed-menu-button").unbind("click").click(function(e) {
            _this.menuToggle(this);
            e.stopPropagation();
            e.preventDefault();
          });
  */        
          if ('ontouchstart' in document.documentElement) {
            $('#collapsed-menu-button').on('touchstart', function(e) {
              _this.menuToggle(this);
              e.stopPropagation();
              e.preventDefault();
            });
          } else {
            $("#collapsed-menu-button").unbind('click').click(function(e) {
              _this.menuToggle(this);
              e.stopPropagation();
              e.preventDefault();
            });
          }
        }
      },

      // Build Slide Menu plugin
      slideMenuBuild : function() {

        if (!$("#mobile-menu").length) {
          _this.cloneMenu();
          $("#mobile-menu-inner").addClass("slidemenu");
          $("#mobile-menu .item-with-ul ul").hide();
          $("#mobile-menu").addClass("mobile-menu-slide").wrap("<div id='mobile-menu-display'></div>").wrap("<div id='slide-menu-clip'></div>");
          $("#mobile-menu-display").css({
            overflow : "hidden"
          });
          $("#mobile-menu-inner").css({
            width : _this.settings.menuWidth + "%"
          });

          function buildNewSub(sub) {
            var no = $(".mobile-menu-slide").length, id = "mobile-menu-" + no, mm_w = $("#mobile-menu-inner").width();

            var ul = $($('<div></div>').html($(sub).clone())).html();

            $("#slide-menu-clip").append("<ul id='" + id + "' class='mobile-menu mobile-menu-slide'>" + ul + "</ul>");
            $("#" + id + " > li > ul").show();
            $("#" + id + " > li > *").first().addClass("return-link");
            $("#" + id + " > li > ul").css({
              padding : 0
            });

            $("#slide-menu-clip").css({
              width : (mm_w * (no + 1)),
              float : "left"
            });
            $(".mobile-menu-slide").css({
              width : mm_w,
              float : "left"
            });

            $('#slide-menu-clip').animate({
              "marginLeft" : -(mm_w * no)
            }, 'normal');

            //$("#"+id+" .item-with-ul").unbind('click');
            bindClick("#" + id);

            $("#" + id + " .return-link").unbind('click').click(function(e) {
              $('#slide-menu-clip').animate({
                "marginLeft" : -($("#mobile-menu-inner").width() * ($(".mobile-menu-slide").length - 2))
              }, 'normal', function() {
                $(".mobile-menu-slide:last").remove();
                $("#slide-menu-clip").css({
                  width : ($("#mobile-menu-inner").width() * ($(".mobile-menu-slide").length + 1)),
                  float : "left"
                });
              });

              e.preventDefault();
              e.stopPropagation();
            });

          }

          function bindClick(item) {
            $(item).find(".item-with-ul").unbind('click').click(function(e) {
              buildNewSub(this);
              e.preventDefault();
              e.stopPropagation();
            });
            $(item).find("li a").unbind('click').click(function(e) {
              e.stopPropagation();
            });
          }

          bindClick("#mobile-menu");
        }

      },

      accordionBuild : function() {
        if (!$("#mobile-menu").length) {
          //var menu = ;
          _this.cloneMenu();
          $("#mobile-menu-inner").addClass("accordion").css({
            width : _this.settings.menuWidth + "%"
          });
          $("#mobile-menu").width(_this.width);
          // _this.settings.showItems possible values: all, active_trail
          if (_this.settings.expandActive) {
            $("#mobile-menu .item-with-ul:not(.active-trail) ul").hide();
            $("#mobile-menu .item-with-ul.active-trail").addClass("active");
          } else {
            $("#mobile-menu .item-with-ul ul").hide();
          }

          $("#mobile-menu .item-with-ul").unbind('click').click(function(e) {
            if (_this.settings.showItems == "active_trail") {
              $("#mobile-menu .item-with-ul ul").not($(this).find("ul")).not($(this).parents()).slideUp("fast");
            }
            $(this).find("ul").first().slideToggle(function() {
              $("#mobile-menu .item-with-ul").removeClass("active");
              $("#mobile-menu .item-with-ul ul").each(function() {
                if ($(this).css("display") != "none") {
                  $(this).parent("li").addClass("active");
                }
              });

              switch (_this.settings.showEffect) {
                case "drawer_right":
                  $(_this.settings.mainPageSelector).css({
                    overflow : "hidden",
                    height : $("#mobile-menu").height()
                  });
                break;

              }
            });

            e.stopPropagation();
          });
          $("#mobile-menu .item-with-ul a").unbind('click').click(function(e) {
            e.stopPropagation();
          });
        }
      },

      basicBuild : function() {
        if (!$("#mobile-menu").length) {
          _this.cloneMenu();
        }
      },

      menuToggle : function(button) {
        if ($(button).hasClass("active")) {/* If it's openned */
          if (_this.settings.menuPlugin == "accordion" && _this.settings.showItems == "active_trail") {
            $("#mobile-menu .item-with-ul:not(.active-trail) ul").slideUp().parent("li").removeClass("active");
          }

          if (_this.settings.useMask) {
            $("#mobile-navigation-mask").fadeOut("slow");
          }

          switch (_this.settings.showEffect) {
            case "expand_down":
              $("#mobile-menu-inner").slideUp("fast");
            break;

            case "drawer_top":
              $('#slide-menu-clip').animate({
                marginLeft : 0,
              }, 'fast', function() {
                $(".mobile-menu-slide").not("#mobile-menu").remove();

              });
              $("#mobile-menu-inner #mobile-menu").slideUp();

            break;

            case "drawer_left":
              $("#mobile-menu-inner").animate({
                left : "-" + _this.settings.menuWidth + "%"
              }, "slow", function() {
                // Animation complete.
              });
              $(_this.settings.mainPageSelector).animate({
                left : "0"
              }, "slow", function() {
                // Animation complete.
                $(this).css({
                  overflow : "inherit",
                  height : "auto",
                  position : "relative",
                  width : "auto"
                })
              });
            break;

            case "drawer_right":
              // cerrar
              $("#mobile-menu-inner").animate({
                left : "100%"
              }, "slow", function() {
                // Animation complete.
                $("#mobile-menu").css({
                  display : "none",
                  width : 0
                });
                $(this).css({
                  width : 0
                });
              });
              $(_this.settings.mainPageSelector).animate({
                left : "0"
              }, "slow", function() {
                // Animation complete.
                $(this).css({
                  overflow : "auto",
                  height : "auto"
                })
              });

              $('#slide-menu-clip').animate({
                marginLeft : 0,

              }, 'normal', function() {
                $(".mobile-menu-slide").not("#mobile-menu").remove();
              });

            break;

            case "fixed_top":
              $("#mobile-menu-inner").animate({
                top : "-" + _this.height + "px"
              }, "fast", function() {
                // Animation complete.
              });
            break;

            case "fixed_left":
              $("#mobile-menu-inner").animate({
                left : "-" + _this.settings.menuWidth + "%"
              }, "fast", function() {
                // Animation complete.
              });
            break;

            case "fixed_right":
              $("#mobile-menu-inner").animate({
                right : "-" + _this.settings.menuWidth + "%"
              }, "fast", function() {
                // Animation complete.
              });
            break;

            case "fixed_bottom":
              $("#mobile-menu-inner").animate({
                bottom : "-" + _this.height + "px"
              }, "fast", function() {
                // Animation complete.
              });
            break;

          }
          $(".mobile-menu-button").removeClass("active");

        } else {
          if (_this.settings.useMask) {
            $("#mobile-navigation-mask").fadeIn("slow").addClass("active");
          }

          switch (_this.settings.showEffect) {
            case "expand_down":
              $("#mobile-menu-inner").slideDown("fast");
            break;

            case "drawer_top":
              $("#mobile-menu-inner #mobile-menu").slideDown();
            break;

            case "drawer_left":
              $(_this.settings.mainPageSelector).css({
                overflow : "hidden",
                height : $("#mobile-menu").height(),
                position : "fixed",
                width: "100%"
              }).animate({
                left : _this.settings.menuWidth + "%",
              }, "slow", function() {
              });
              $("#mobile-menu-inner").animate({
                left : 0
              }, "slow", function() {
                // Animation complete.
              });
            break;

            case "drawer_right":
              $("#mobile-menu").show().css({
                display : "block",
                width : "100%"
              });

              $("#mobile-menu-inner").css({
                width : _this.settings.menuWidth + "%"
              }).animate({
                left : (100 - _this.settings.menuWidth) + "%"
              }, "slow", function() {
                // Animation complete.
              });

              $('#slide-menu-clip').animate({
                width : $("#mobile-menu-inner").width() + "px"
              });
              $(_this.settings.mainPageSelector).animate({
                left : "-" + _this.settings.menuWidth + "%"
              }, "slow", function() {
                // Animation complete.
                $(this).css({
                  overflow : "hidden",
                  height : $("#mobile-menu").height()
                });
              });
            break;

            case "fixed_top":
              $("#mobile-menu-inner").animate({
                top : "0"
              }, "fast", function() {
                // Animation complete.
              });
            break;

            case "fixed_left":
              $("#mobile-menu-inner").animate({
                left : "0"
              }, "fast", function() {
                // Animation complete.
              });
            break;

            case "fixed_right":
              $("#mobile-menu-inner").animate({
                right : "0"
              }, "fast", function() {
                // Animation complete.
              });
            break;

            case "fixed_bottom":
              $("#mobile-menu-inner").animate({
                bottom : "0"
              }, "fast", function() {
                // Animation complete.
              });
            break;

          }

          $(".mobile-menu-button").addClass("active");
        }
      },

      mobileSet : function() {
        $(_this.target).hide();

        $("#mobile-menu-wrapper").show();
        $("#collapsed-menu-button").show();

        if ($("#collapsed-menu-button.active").length > 0) {
          if (_this.settings.useMask) {
            $("#mobile-navigation-mask").show();
          }
        } else {
          if (_this.settings.useMask) {
            $("#mobile-navigation-mask").hide();
          }
        }

        switch (_this.settings.showEffect) {
          case "expand_down":
            //$("#mobile-menu.inner").css({width: "100%"});
          break;

          case "drawer_right":
            $("#mobile-menu").css({
              minHeight : $(window).height() + "px"
            });
            if ($("#collapsed-menu-button.active").length > 0) {
              $(_this.settings.mainPageSelector).css({
                left : "-" + _this.settings.menuWidth + "%"
              });
              $("#mobile-menu-inner").css({
                width : _this.settings.menuWidth + "%"
              });

              $(_this.settings.mainPageSelector).css({
                left : "-" + _this.settings.menuWidth + "%",
                overflow : "hidden",
                height : $("#mobile-menu").height()
              });
              $("#mobile-menu-inner").css({
                left : (100 - _this.settings.menuWidth) + "%"
              });

              /*if (_this.settings.useMask) {
               $("#mobile-navigation-mask").show();
               }*/
            } else {
              $("#mobile-menu-inner").css({
                width : 0
              });
              $("#mobile-menu").css({
                display : "none",
                width : 0
              });
              /*
               if (_this.settings.useMask) {
               $("#mobile-navigation-mask").hide();
               }*/
            }

          break;

          case "drawer_left":
            $("#mobile-menu").css({
              minHeight : $(window).height() + "px"
            });
            if ($("#collapsed-menu-button.active").length > 0) {
              $(_this.settings.mainPageSelector).css({
                left : _this.settings.menuWidth + "%",
                width: "100%"
              });
              $(_this.settings.mainPageSelector).css({
                position : "fixed"
              });
            }
          break;

          case "drawer_top":
            $(_this.settings.mainPageSelector).css({
              left : "0",
              overflow : "visible",
              height : "auto"
            });

            $("#mobile-menu-wrapper").css({
              width : "100%",
              float : "left"
            });

          break;

          case "fixed_top":
            //$("#mobile-menu.inner").css({width: "100%"});
          break;

          case "fixed_left":
            if ($("#collapsed-menu-button.active").length > 0) {
              $("#mobile-menu-inner").animate({
                left : "0"
              }, "fast", function() {
                // Animation complete.
              });

            } else {
              $("#mobile-menu-inner").css({
                left : "-" + _this.settings.menuWidth + "%"
              }, "fast", function() {
                // Animation complete.
              });
            }
          break;

          case "fixed_right":
            $("#mobile-menu-wrapper").show();
          break;

          case "fixed_bottom":
            $("#mobile-menu-wrapper").show();
            $("#mobile-menu-inner").css({
              width : _this.settings.menuWidth + "%"
            });
          break;

        }

        switch (_this.settings.menuPlugin) {
          case "slideMenu":
            var no = $(".mobile-menu-slide").length, mm_w = $("#mobile-menu-inner").width();

            $("#slide-menu-clip").css({
              width : (mm_w * no)
            });
            $(".mobile-menu-slide").css({
              width : mm_w
            });

            $('#slide-menu-clip').css({
              "marginLeft" : -(mm_w * (no - 1))
            }, 'normal');

          break;

          case "accordion":
          break;

        }

      },

      mobileUnset : function() {
        $("#mobile-menu-wrapper").hide();
        $("#collapsed-menu-button").hide();

        switch (_this.settings.showEffect) {
          case "expand_down":
          break;

          case "drawer_right":
            $(_this.settings.mainPageSelector).css({
              left : 0,
              height : "auto",
              overflow : "auto"
            });
          break;

          case "drawer_left":
            $(_this.settings.mainPageSelector).css({
              left : 0,
              position : "relative",
              height : "auto",
              width : "auto"
            });
          break;

          case "drawer_top":
          case "fixed_top":
          case "fixed_left":
          case "fixed_right":
          case "fixed_bottom":
          break;

        }

        if (_this.settings.useMask) {
          $("#mobile-navigation-mask").hide();
        }

        $(_this.target).show();
      },

      // Verify if a query is valid for current browser width.
      checkQuery : function(query) {
        return window.matchMedia(query).matches;
      }, // End of checkQuery
      initialSet : function() {
        var menu_width = _this.width = $("#mobile-menu-inner").outerWidth() + ($("#mobile-menu-inner").outerWidth() / 10), menu_height = _this.height = $("#mobile-menu-inner").outerHeight();

        if (_this.settings.useMask) {
          //$(_this.settings.mainPageSelector).
          $("#mobile-menu-wrapper").css({
            position : "relative"
          }).after("<div id='mobile-navigation-mask'></div>");
          $("#mobile-navigation-mask").click(function() {
            _this.menuToggle(this);
          }).hide();
        }

        switch (_this.settings.showEffect) {
          case "expand_down":
          break;

          case "fixed_top":
            $("#mobile-menu-inner").css({
              position : "absolute",
              top : "-" + menu_height + "px",
              left : "0",
              display : "block"
            });
          break;

          case "drawer_top":
            $("#mobile-menu-inner").css({
              position : "relative",
              display : "block"
            });
            $("#mobile-menu-inner #mobile-menu").slideUp();
          break;

          case "fixed_left":
            $("#mobile-menu-inner").css({
              position : "absolute",
              left : "-" + _this.settings.menuWidth + "%",
              top : "0",
              display : "block"
            });

          break;

          case "fixed_right":
            $("#mobile-menu-inner").css({
              position : "fixed",
              right : "-" + _this.settings.menuWidth + "%",
              top : "0",
              display : "block"
            });
          break;

          case "drawer_left":
            $("#mobile-menu-inner").css({
              position : "absolute",
              left : "-" + _this.settings.menuWidth + "%",
              top : "0",
              display : "block"
            });
          break;

          case "drawer_right":
            $("#mobile-menu-inner").css({
              position : "absolute",
              left : "100%",
              top : "0",
              display : "block"
            });
            $(_this.settings.mainPageSelector).css({
              position : "relative"

            });

          break;

          case "fixed_bottom":
            $("#mobile-menu-inner").css({
              position : "fixed",
              bottom : "-" + menu_height + "px",
              left : "0",
              display : "block"
            });
          break;
        }
      },
      onResize : function() {
        if (_this.checkQuery(_this.settings.breakpoint)) {
          if (current != "normal") {
            _this.mobileUnset();
          }
          _this.current = "normal";

        } else {
          if (current != "mobile" && !$("#mobile-menu").length) {
            switch (_this.settings.menuPlugin) {
              case "basic":
                _this.basicBuild();
              break;

              case "accordion":
                _this.accordionBuild();
              break;

              case "slideMenu":
                _this.slideMenuBuild();
              break;

            }
            _this.collapseInAButton();
            _this.initialSet();
          }
          _this.current = "mobile";
          _this.mobileSet();

        }
      },

      // INIT
      init : function(elem, opts) {
        _this.target = elem;

        // Merge Default options with the specified
        _this.settings = $.extend(true, {}, defaults, opts);
        _this.onResize();

        $(window).resize(_this.onResize);
      } // end of init
    };
    // end of methods

    var _this = this.methods;

  };
  // end of mobile_menu class

  // $.FN
  $.fn.mobile_menu = function(opts) {
    var id = $(this).attr('id');

    if (!(!$.support.leadingWhitespace)) {
      //Not IE7 nor IE8.
      if (instances[id] === undefined) {
        instances[id] = new mobile_menu(this, opts);
        return instances[id].methods.init(this, opts);
      } else if (instances[id]) {
        return instances[id].methods;
      } else {
        return $(this);
      }
    }
  };
  // end of $.fn.mapSvg

})(jQuery);
