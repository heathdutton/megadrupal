
(function(){

  var $ = jQuery;
  var d = Drupal.dqx_adminmenu;
  d.directional = {};


  d.squareSum = function(a, b, c) {
    var sum = 0;
    for (var i = 0; i < arguments.length; ++i) {
      sum += arguments[i] * arguments[i];
    }
    return sum;
  };


  d.euclidean = function(a, b, c) {
    var sum = 0;
    for (var i = 0; i < arguments.length; ++i) {
      sum += arguments[i] * arguments[i];
    }
    return Math.sqrt(sum);
  };


  /**
   * Track the mouse direction with a 1px tolerance box.
   * So, if the mouse flickers between two adjacent pixels, this is completely
   * ignored.
   *
   * @param tolerance (optional) :int OR {x:int, y:int}
   *   dimensions of the tolerance box. Default is 1x1.
   */
  d.directional.LazyDirectionTracker = function(tolerance) {

    if (typeof tolerance == 'number') {
      tolerance = {x:tolerance, y:tolerance};
    }
    else if (!tolerance || typeof tolerance != 'object') {
      tolerance = {x:1, y:1};
    }

    var prev = null;
    var px = 0.5 * tolerance.x;
    var py = 0.5 * tolerance.y;

    /**
     * @param x :int
     *   the new x mouse position
     * @param y :int
     *   the new y mouse position
     * @param t :float (?)
     *   timestamp in ms (?)
     *
     * @return :{x:int, y:int}
     *   interpreted relative movement since last moveTo().
     */
    this.moveTo = function(x, y, t) {
      var event = {x:x, y:y, t:t};
      var delta = {x:0, y:0, t:0};
      if (prev) {
        delta.x = event.x - prev.x;
        delta.y = event.y - prev.y;
        delta.t = event.t - prev.t;
        while (delta.x > 0 && px < tolerance.x) {++px; --delta.x;}
        while (delta.x < 0 && px > 0) {--px; ++delta.x;}
        while (delta.y > 0 && py < tolerance.y) {++py; --delta.y;}
        while (delta.y < 0 && py > 0) {--py; ++delta.y;}
        if (px < 0) {px = 0;}
        if (px > tolerance.x) {px = tolerance.x;}
        if (py < 0) {py = 0;}
        if (py > tolerance.y) {py = tolerance.y;}
      }
      prev = event;
      return delta;
    };
  };


  /**
   * A "court" is a group of judges.
   * Every "judge" is responsible for one "direction".
   *
   * The judges observe and interpret the mouse movement, and decide whether the
   * mouse is considered to be moving in a specific direction, and for how long
   * this movement is considered to last.
   *
   * The interpreted movement duration is interpolated from the sequence of
   * discrete mouse move events. The mouse needs to be quiet for a while, until
   * it is considered not moving.
   */
  d.directional.DirectionalityCourt = function() {

    var directionJudges = {};
    var observers = [];

    this.moveDelta = function(delta) {
      for (var k in directionJudges) {
        directionJudges[k].moveDelta(delta);
      }
    };

    this.hasDirection = function(k) {
      return 0 < directionJudges[k].hasDirectionUntil();
    };

    this.hasDirectionUntil = function(k) {
      return directionJudges[k].hasDirectionUntil();
    };

    this.hasAnyOfTheseDirectionsUntil = function(keys) {
      var until = 0;
      for (var i = 0; i < keys.length; ++i) {
        var until_k = this.hasDirectionUntil(keys[i]);
        if (until_k > until) {
          until = until_k;
        }
      }
      return until;
    };

    var observeJudge = function(k, observer) {
      directionJudges[k].observe(function(has_direction){
        observer(k, has_direction);
      });
    };

    this.addDirection = function(k, T, f, debug_bg) {
      directionJudges[k] = new d.directional.DirectionalityJudge(T, f, debug_bg);
      for (var i = 0; i < observers.length; ++i) {
        observeJudge(k, observers[i]);
      }
    };

    this.observe = function(observer) {
      observers.push(observer);
      for (var k in directionJudges) {
        observeJudge(k, observer);
      }
    };

    this.observeOnce = function(observer) {
      for (var k in directionJudges) {
        var until = directionJudges[k].hasDirectionUntil();
        observer(k, 0 < until, true);
      }
    }
  };


  /**
   * The "judge" watches mouse movement in a specific direction,
   * and decides how long the mouse is considered as moving in this direction.
   *
   * @param f :function
   *   Callback
   * @param T :float
   *   Duration
   * @param debug_bg :color
   *   Sth for debug purposes..
   */
  d.directional.DirectionalityJudge = function(T, f, debug_bg) {

    if (debug_bg) {
      var $monitor = $('<li>').appendTo('#dqx_adminmenu-user');
      $monitor.css({background: debug_bg, 'border-left':'4px solid white', height:'10px'});
    }

    // any discrete movement in the respective direction will increase this value.
    // waiting will decrease the value.
    var dir = 0;
    var timer;
    var observers = [];

    var setDirection = function() {
      for (var i = 0; i < observers.length; ++i) {
        observers[i](true);
      }
    };

    var unsetDirection = function() {
      dir = 0;
      for (var i = 0; i < observers.length; ++i) {
        observers[i](false);
      }
    };

    this.moveDelta = function(delta) {
      var abs = {
        x: Math.abs(delta.x),
        y: Math.abs(delta.y),
        t: delta.t
      };

      var dir_before = dir;

      // waiting
      dir -= delta.t / T;
      dir = Math.max(-0.2, dir);

      var dir_f = f(delta, abs);

      // movement
      dir += dir_f;
      dir = Math.max(-0.2, dir);
      dir = Math.min(1, dir);

      if (debug_bg) {
        $monitor.stop();
        $monitor.css('width', dir * 150 + 'px');
        $monitor.animate({width: 0}, dir * T);
      }

      if (dir_before > 0) {
        clearTimeout(timer);
        if (dir <= 0) {
          unsetDirection();
        }
      }
      if (dir > 0) {
        timer = setTimeout(unsetDirection, dir * T);
        if (dir_before <= 0) {
          setDirection();
        }
      }
    };

    this.hasDirectionUntil = function() {
      return dir * T;
    };

    this.observe = function(observer) {
      if (typeof observer == 'function') {
        observers.push(observer);
        observer(dir > 0);
      }
    };
  };


  d.MouseEngine_directional = function($divRoot, menu) {

    var tracker = new d.directional.LazyDirectionTracker(2);
    var court = new d.directional.DirectionalityCourt();

    court.addDirection('r', 450, function(delta, abs){return delta.x * 0.2 - abs.y * 0.12;});
    court.addDirection('l', 450, function(delta, abs){return delta.x * (-0.2) - abs.y * 0.12;});
    court.addDirection('any', 160, function(delta, abs){return abs.y * 1 + abs.x * 1;});

    // The implementation of this selector-logic can give us valuable milliseconds.
    var $root = $(document.getElementById('dqx_adminmenu'));
    var $toplevel_menus = $root.children();
    var ul_lr = {
      l: $root.children().filter('ul.dqx_adminmenu-right'),
      r: $root.children().filter('ul.dqx_adminmenu-left')
    };

    function courtObserver(k, has_direction, is_observe_once){

      switch (k) {
        case 'any':
          $root.removeClass(has_direction ? 'mouse-direction-any-on' : 'mouse-direction-any-off');
          $root.addClass((!has_direction) ? 'mouse-direction-any-on' : 'mouse-direction-any-off');
          break;
        case 'l':
        case 'r':
          ul_lr[k].removeClass(has_direction ? 'mouse-direction-lr-off' : 'mouse-direction-lr-on');
          ul_lr[k].addClass((!has_direction) ? 'mouse-direction-lr-off' : 'mouse-direction-lr-on');
          break;
      }

      if (!menu.isSameItem() && !has_direction) {
        if (!has_direction) {
          var activeItem = menu.activeItem();
          var mouseItem = menu.mouseItem();

          if (!mouseItem) {
            // this would close the menu.
            menu.flushDelayed(250 + activeItem.depth() * 140, 'close');
          }
          else if (mouseItem.depth() < 2) {
            // Item is in a horizontal menu
            if (k == 'any') {
              menu.flush();
            }
          }
          else {
            // Item is in a vertical submenu.
            var lr = mouseItem.isLeft() ? 'r' : 'l';
            if (k == lr) {
              menu.flush();
            }
          }
        }
      }
    }

    court.observe(courtObserver);

    function hoverItemChange() {

      court.observeOnce(courtObserver);
      if (menu.isSameItem()) {
        return;
      }

      var activeItem = menu.activeItem();
      var mouseItem = menu.mouseItem();

      if (!mouseItem) {
        // this would close the menu.
        menu.flushDelayed(450 + activeItem.depth() * 200, 'close');
      }
      else if (!activeItem) {
        // We are just entering the menu.
        menu.flushDelayed(450, 'open');
      }
      else if (menu.isParentChild()) {
        // Either the hovered item is a child of the current item, or vice versa.
        menu.flush();
      }
      else if (mouseItem.depth() < 2) {
        // Item is in a horizontal menu
        if (activeItem.depth() < 2) {
          // Active item is also in the horizontal menu.
          menu.flush();
        }
        else {
          // Active item is in a submenu, but not a child of current item.
          menu.flushDelayed(450);
        }
      }
      else {
        // Hovered item is in a vertical submenu.
        // Let the observer do the job.
      }
    }

    this.mouseMove = function(x, y, hoverItemHasChanged) {

      var now = d.getMillis();
      var delta = tracker.moveTo(x, y, now);
      court.moveDelta(delta);

      if (hoverItemHasChanged) {
        hoverItemChange();
      }
    }
  };


  d.hooks.init_mid.mouse = function(context, settings, $divRoot) {
    var menu = new d.InteractiveMenu($divRoot);
    var mouse_engine = new d.MouseEngine_directional($divRoot, menu);
    menu.attachMouseEngine(mouse_engine);
  };

})();
