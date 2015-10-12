/*
 * TODOs
 * - use more classes in corresponding css for fullheight, shift,
 * - shiftAside cross browser
 * - use transitions and tranformations for browsers who understand them
 *   use js fallback otherwise
 */
(function ( $, M ) {
  'use strict';
  $.fn.mobilemenu = function( options ) {

    var settings = $.extend({}, $.fn.mobilemenu.defaults, options );
    var mobileQuery = window.matchMedia('(min-width: ' + settings.breakpoint + 'px)');
    var hasModernizr = !!M,
        hasHammer = typeof Hammer !== 'undefined';
    var $menu = $(this),
        $body = $('body'),
        $iconContainer = $(settings.iconContainer),
        $closeContainer = $(settings.closeContainer),
        $dim = $(settings.dimElement),
        onFallback = false,
        onMobile = false,
        moveMenu = false,
        throttled,
        $icon,
        $close;

    var sessionStorage = window.sessionStorage || {
      setItem: function (a,b) {},
      getItem: function (a) {}
    };

    // return early if we are empty, ie. the calling selector matched no
    // element
    if ($menu.length < 1) {
      return this;
    }

    // add class on mobile menu as we cannot be sure about it's id
    $menu.addClass('mobile-menu-element');

    // move $menu, when we slide it (i.e. not when collapsible)
    // override with moveMenu if moveMenu is defined
    moveMenu = !settings.collapsible;
    if (settings.moveMenu !== undefined) {
      moveMenu = settings.moveMenu;
    }

    // placeholder used to determine original position of moveable $menu
    var $menuPlaceholder = $('.mobile-menu-placeholder');
    if ($menuPlaceholder.length < 1) {
      $menuPlaceholder = $('<span id="mobile-menu-placeholder"></span>');
      if (moveMenu) {
        $menu.after($menuPlaceholder);
      }
    }

    if (!settings.collapsibleMenu) {
      $body.addClass(settings.mobileMenuSlidingClass);
      // when no collapsibleMenu, we are sliding
    }

    $body.addClass(settings.mobileMenuEnabledClass);

    if (settings.fixedMenu) {
      $body.addClass(settings.mobileMenuFixedClass);
    }

    // ignore shiftBodyAside on browser which do not support CSS3
    // transformations
    if (!hasModernizr || !M.csstransforms) {
      settings.shiftBodyAside = false;
      settings.needTransformsFallback = true;
      onFallback = true;
      $body.addClass(settings.mobileMenuFallbackClass);
    }

    // generate buttons or use elements/containers from settings
    // default: before icon to menu, append close to menu
    if (settings.createIcon) {
      $icon = $('<a href="#">' + settings.iconText + '</a>').attr(settings.iconAttributes);
      if ($iconContainer.length > 0) {
        $iconContainer.append($icon);
      } else {
        $menu.before($icon);
      }
    } else {
      $icon = $(settings.iconElement);
    }
    if (settings.createIcon) {
      $close = $('<a href="#">' + settings.closeText + '</a>').attr(settings.closeAttributes);
      if ($closeContainer.length > 0) {
        $closeContainer.append($close);
      } else {
        $menu.append($close);
      }
    } else {
      $close = $(settings.closeElement);
    }

    // collapsible links/submenus in mobilemenu
    // if the link clicked has a ul.menu sibling (i.e. a submenu)
    // we want to show the submenu
    if (settings.collapsibleSubMenus) {
      $('html').on('click', '.mobile-menu-open .mobile-menu-element li a', function(e) {
        var $a =$(this);
        var $li = $a.closest('li');

        var $ul = $a.siblings('ul');
        if ($ul.length > 0) {
          if ($ul.is(':visible')) {
            $ul.hide();
            $li.removeClass('submenu-open');
          } else {
            $ul.show();
            $li.addClass('submenu-open');
          }

          // lose focus
          $a.blur();
          // stop propagation - we do not want to follow link when it could
          // reveal a submenu
          e.preventDefault();
          e.stopPropagation();
          return false;
        }
      });
    }

    // gets called when switched to mobile
    // sets up the classes, ...
    var switchToMobile = function(mql) {
      // $menu.hide();
      // $icon.show();
      // $close.show();

      cleanClasses();

      $body.addClass(settings.mobileMenuClass);

      if (moveMenu) {
        $menu.prependTo($body);
      }

      if (settings.collapsibleSubMenus) {
        $('ul ul', $menu).css('display', '');
      }
      if (settings.animateMenu) {
        $body.addClass(settings.mobileMenuDirectionClassPrefix + settings.animationFromDirection);
      }

      // callback
      settings.onSwitchToMobile.call($menu, settings, mql);
    }

    // gets called when switched to desktop
    var switchToDesktop = function(mql) {
      // $icon.hide();
      // $close.hide();

      cleanClasses();
      $body.removeClass(settings.mobileMenuClass);
      $body.removeClass(settings.mobileMenuShiftAsideClassPrefix + settings.animationFromDirection)
           .removeClass(settings.mobileMenuDirectionClassPrefix + settings.animationFromDirection);

      if (moveMenu && !settings.fixedMenu) {
        $menuPlaceholder.before($menu);
      } else if (settings.fixedMenu) {
        $menu.prependTo($body);
        $body.addClass(settings.mobileMenuFixedOpenClass);
      }

      // close any open mobile menu
      if ($body.hasClass(settings.mobileMenuOpenClass) && !settings.fixedMenu) {
        // @TODO menuClose animation cannot deal with "jump" in layout
        // so no animation --> remains sync
        if (!onFallback) {
          menuClose();
        } else {
          $body.css(settings.animationFromDirection, '');
          afterClose();
        }
      }

      if (settings.collapsibleSubMenus) {
        $('ul ul', $menu).hide();
      }

      // hide background overlay again
      if (settings.dimBackground) {
        $dim.hide();
      }

      // callback
      settings.onSwitchToDesktop.call($menu, settings, mql);
    }

    var cleanClasses = function () {
      $body.removeClass(settings.mobileMenuOpenClass)
           .removeClass(settings.mobileMenuFixedOpenClass);
    }

    var setMenu = function (mql) {
      if (mql.matches) {
        onMobile = false;
        switchToDesktop(mql);
      } else {
        onMobile = true;
        switchToMobile(mql);
      }
    }

    var initMenu = function (mql) {

      cleanClasses();

      if (settings.fixedMenu && settings.rememberOpenMenu) {
        if (sessionStorage.getItem('menu-open') === '1') {
          $body.addClass(settings.mobileMenuOpenClass);
        }
      }
      if (mql.matches) {
        //$body.removeClass(settings.mobileMenuClass);
        switchToDesktop(mql);
      } else {
        switchToMobile(mql);
      }
    }

    // menu events
    var beforeOpen = function () {
      if (settings.dimBackground) {
        $dim.show();
      }

      settings.beforeOpen.call($menu, settings);
    }
    var afterOpen = function () {
      settings.afterOpen.call($menu, settings);
    }
    var beforeClose = function () {
      settings.beforeClose.call($menu, settings);
    }
    var afterClose = function () {
      // if (mobileQuery.matches) {
      //   $menu.show();
      // } else {
      //   $menu.hide();
      // }
      $body.removeClass(settings.mobileMenuOpenClass);
      if (settings.fixedMenu) {
        sessionStorage.setItem('menu-open', '0');
      }

      if (settings.dimBackground) {
        $dim.hide();
      }

      settings.afterClose.call($menu, settings);
    }
    var menuOpen = function () {

      cleanClasses();

      if (mobileQuery.matches && settings.fixedMenu) {
        $body.addClass(settings.mobileMenuFixedOpenClass);
      } else {
        $body.addClass(settings.mobileMenuOpenClass);
      }

      if (settings.fixedMenu) {
        sessionStorage.setItem('menu-open', '1');
      }

      if (!onFallback) {
        if (onMobile && settings.shiftBodyAside) {
          $body.addClass(settings.mobileMenuShiftAsideClassPrefix + settings.animationFromDirection);
        }
        afterOpen();
      } else {
        var animation = {};
        animation[settings.animationFromDirection] = '0px';
        $menu.animate(animation, settings.animationDuration, afterOpen);
        // reset to prevent unexpected reuse of former values
        animation = {};
      }
    };

    var menuClose = function () {
      if (!onFallback) {
        if (settings.shiftBodyAside) {
          $body.removeClass(settings.mobileMenuShiftAsideClassPrefix + settings.animationFromDirection);
        }
      } else {
        var animation = {};
        animation[settings.animationFromDirection] = '-' + settings.width + 'px';
        $menu.animate(animation, settings.animationDuration, afterClose);

        // reset to prevent unexpected reuse of former values
        animation = {};
      }

      if (settings.fixedMenu) {
        $body.removeClass(settings.mobileMenuFixedOpenClass);
      }

      afterClose();

      $dim.hide();
    };

    // throttled resize handler
    var resizeHandler = function () {
      if (throttled) {
        return;
      }

      throttled = true;

      setTimeout(function() {
        throttled = false;
      }, settings.interval);

      // throttled from here on

      if (!mobileQuery.matches && $body.hasClass(settings.mobileMenuOpenClass)) {
        var position = [];

        // if (settings.shiftBodyAside) {
        //   position[settings.animationFromDirection] = settings.width + 'px';
        //   $body.css(position);
        //   position[settings.animationFromDirection] = '-' + settings.width + 'px';
        //   $menu.css(position);
        // }
      }
    }

    var menuHandler = function (e, action) {
      if (action) {
        if (action === 'open' && 
            !($body.hasClass(settings.mobileMenuOpenClass) ||
              $body.hasClass(settings.mobileMenuFixedOpenClass))) {
          beforeOpen();
          menuOpen();
        } else if (action === 'close' &&
                   ($body.hasClass(settings.mobileMenuOpenClass) ||
                    $body.hasClass(settings.mobileMenuFixedOpenClass))) {
          beforeClose();
          menuClose();
        }
      } else {
        if ($body.hasClass(settings.mobileMenuOpenClass) ||
            $body.hasClass(settings.mobileMenuFixedOpenClass)) {
          beforeClose();
          menuClose();
        } else {
          beforeOpen();
          menuOpen();
        }
      }

      $(this).blur();
      e.preventDefault();
      return false;
    }

    // use Hammer.js if available and we are running on a touch
    // capable device
    if (hasHammer && hasModernizr && M.touch) {
      Hammer(document).on('swiperight', function(e) {
        menuHandler(e, 'open');
      });
      Hammer(document).on('swipeleft', function(e) {
        menuHandler(e, 'close');
      });
    }
    // bind click handler 
    if ($icon.length > 0) {
      $icon.on('click.mobilemenu', menuHandler);
    }
    if ($close.length > 0) {
      $close.on('click.mobilemenu', menuHandler);
    }

    // add breakpoint listener and do inital call
    mobileQuery.addListener(function(mql) {
      setMenu(mql);
    });

    // resize handler
    $(window).on('resize.mobilemenu', resizeHandler);

    // initialize
    setTimeout(function() { initMenu(mobileQuery); }, 1);
    // init callback
    settings.init.call($menu, settings);

    // be a nice jQuery plugin and return myself
    return this;
  }


  $.fn.mobilemenu.defaults = {
    // These are the defaults.
    breakpoint: 780,
    width: 300,
    createIcon: true,
    iconText: 'Menu',
    iconContainer: '',
    iconElement: undefined,
    iconAttributes: { id: 'mobile-menu-icon' },
    createClose: true,
    closeText: 'close',
    closeContainer: '',
    closeElement: undefined,
    closeAttributes: { id: 'mobile-menu-close' },
    mobileMenuClass: 'gone-mobile',
    mobileMenuEnabledClass: 'with-mobile-menu',
    mobileMenuOpenClass: 'mobile-menu-open',
    mobileMenuShiftAsideClassPrefix: 'mobile-menu-shift-aside-',
    mobileMenuDirectionClassPrefix: 'mobile-menu-from-',
    mobileMenuSlidingClass: 'mobile-menu-sliding',
    mobileMenuFallbackClass: 'mobile-menu-fallback',
    mobileMenuFixedClass: 'mobile-menu-fixed',
    mobileMenuFixedOpenClass: 'mobile-menu-fixed-open',
    animateMenu: true,
    animationDuration: 300,
    animationFromDirection: 'left',
    shiftBodyAside: false,
    collapsibleMenu: false, // TODO
    moveMenu: undefined,
    fixedMenu: false,
    rememberOpenMenu: false,
    createDim: false, // TODO
    dimBackground: false,
    dimElement: '',
    interval: 100,
    collapseSubMenus: true,
    collapsibleSubMenus: true, // TODO
    needTransformsFallback: false,
    init: function() {},
    beforeOpen: function() {},
    beforeClose: function() {},
    afterOpen: function() {},
    afterClose: function() {},
    onSwitchToMobile: function() {},
    onSwitchToDesktop: function() {}
  }
}( jQuery, typeof Modernizr !== 'undefined' ? Modernizr : false ));
