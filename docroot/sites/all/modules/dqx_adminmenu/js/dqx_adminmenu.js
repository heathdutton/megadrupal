
Drupal.dqx_adminmenu = {
  hooks: {
    init_early: {},
    init_mid: {},
    init_late: {},
    init_latest: {},
    focusItem: {},
    linkField: {}
  }
};


(function(){

  var d = Drupal.dqx_adminmenu;
  var $ = jQuery;


  d.getMillis = function() {
    return (new Date()).getTime();
  };


  d.makeTimer = function() {
    return new d.Timer();
  };


  d.Timer = function() {

    var t0;

    this.start = function() {
      t0 = d.getMillis();
      return this;
    };

    this.isRunning = function() {
      return (t0 !== undefined);
    };

    this.reset = function() {
      var t1 = d.getMillis();
      var duration;
      if (t0 !== undefined) {
        duration = t1 - t0;
      }
      t0 = t1;
      return duration;
    };

    this.logReset = (console && console.log) ? function(txt) {
      var duration = this.reset();
      if (duration !== undefined) {
        // console.log(txt, duration);
      }
      return this;
    } : function() {};

    this.stamp = this.logReset;
  };


  d.compareElements = function($ele1, $ele2) {
    if ($ele1 == null || $ele2 == null || typeof $ele1 != 'object' || typeof $ele2 != 'object') {
      return $ele1 == $ele2;
    }
    if ($ele1.length != $ele2.length) {
      return false;
    }
    for (var i = 0; i < $ele1.length; ++i) {
      if ($ele1[i] != $ele2[i]) {
        return false;
      }
    }
    return true;
  };


  d.countParentItems = function($item) {
    var n_parents;
    if ($item && $item.length) {
      n_parents = 0;
      $item = $item.parent();
      while ($item && $item.length && !$item.is('#dqx_adminmenu, body')) {
        if ($item.is('li.expandable, tr.expandable')) {
          ++n_parents;
        }
        $item = $item.parent();
      }
    }
    return n_parents;
  };


  d.init = function($divRoot) {

    var timer = d.makeTimer().start();

    function invokePhase(hook) {
      for (var f in hook) {
        if (typeof hook[f] == 'function') {
          hook[f]($divRoot, Drupal.settings.dqx_adminmenu, $divRoot);
          timer.logReset(f);
        }
      }
    };

    invokePhase(d.hooks.init_early);
    invokePhase(d.hooks.init_mid);
    invokePhase(d.hooks.init_late);
    // Before we fire expensive hooks, we let other scripts do their job.
    setTimeout(function(){
      timer.logReset('--- wait ---');
      invokePhase(d.hooks.init_latest);
    }, 20);
  };


  d.hooks.init_late.fixRedirectDestination = function(context, settings, $divRoot) {
    $('a.dqx_adminmenu-redirect', $divRoot).each(function(){
      var href = this.href;
      if (href.split('?').length < 2) {
        this.href += '?' + settings.destination;
      }
      else if (href.split('?destination=').length > 1) {
        // do nothing
      } else if (href.split('?')[1].split('&destination=').length > 1) {
        // do nothing
      } else {
        this.href += '&' + settings.destination;
      }
    });
  };


  /**
   * Focus a specific item in the menu.
   *
   * @param $item
   *   The li or tr element to activate.
   * @param hard :boolean
   *   This function is called with hard = true, only if the mouse movement
   *   indicates the clear intention to focus this item.
   *   Otherwise, we might still focus the item, if an unintended focusing does
   *   not harm the user experience.
   * @param state :object
   *   The current state of the menu.
   */
  d.focusItem = function($item, hard) {
    if (!$item || !$item.length || !$item.is('li, tr')) {
      // The mouse left the menu entirely, but only the 'hard' param can tell us
      // if this was intended.
      if (hard) {
        d.unfocusAll();
      }
    }
    else {
      if (hard || $item.is('.expandable')) {
        var depth = d.countParentItems($item);

        var $open_sibling_submenus = $([]);
        var $open_sibling_items = $([]);
        var $scan = $item;
        var scan_i = 0;
        do {
          var $scan_siblings = $scan.siblings('li, tr');
          $open_sibling_submenus = $open_sibling_submenus.add($('ul.show, table.show', $scan_siblings));
          $open_sibling_items = $open_sibling_items.add($scan_siblings);
          scan_i += 0.5;
          $scan = $scan.parent().parent();
          if (!$scan.is('li, tr')) break;
          scan_i += 0.5;
        } while (true);

        if (depth == 0) {
          hard = hard || ($open_sibling_submenus.length == 1);
        }
        else {
          hard = hard || ($open_sibling_submenus.length == 0);
        }
        if (hard) {
          $open_sibling_submenus.removeClass('show');
          var $submenu = $('> ul, > td > ul, > td > div > ul, > table, > td > table, > td > div > table', $item);
          $submenu.addClass('show');
          $('ul.show, table.show', $submenu).removeClass('show');
          $open_sibling_items.removeClass('hover');
          $item.addClass('hover');
          hard = true;
        }
      }

      // trickle up
      $element = $item.parent();
      while (!$element.is('#dqx_adminmenu')) {
        if ($element.is('li, tr')) {
          // hide siblings
          $siblings = $element.siblings('li, tr');
          $('ul.show, table.show', $siblings).removeClass('show');
        }
        else if ($element.is('ul, table')) {
          // show parent
          $element.addClass('show');
        }
        $element = $element.parent();
      }
    }

    return hard;
  };


  d.unfocusAll = function() {
    $('#dqx_adminmenu ul.show, #dqx_adminmenu table.show').removeClass('show');
  }


  /**
   * A simple mouse engine.
   * This thing is later replaced by the directional mouse engine.
   * It is only here for .. yeah, why is it here?
   * Mostly to show that mouse engines are pluggable.
   */
  d.MouseEngine_simple = function() {
    /**
     * @param $element
     *   The li or tr element that is being hovered, or null for body hover.
     * @param x, y
     *   x and y mouse position.
     */
    this.mouseMove = function($element, x, y) {
      if ($element) {
        if ($element.is('.expandable')) {
          d.focusItem($element, true);
        }
      }
      else {
        d.unfocusAll();
      }
    };
  };


  /**
   * This one waits a bit, before it closes a submenu.
   *
   * This thing is later replaced by the directional mouse engine.
   * It is only here for .. yeah, why is it here?
   * Mostly to show that mouse engines are pluggable.
   *
   * @param delay
   *   The delay in milliseconds
   */
  d.MouseEngine_superfish = function(delay) {

    // default delay duration
    if (typeof delay != 'number') {
      delay = 2600;
    }

    var timer;
    var $recentElement = true;

    /**
     * @param $element
     *   The li, tr, #dqx_adminmenu or body element that is being hovered.
     * @param x, y
     *   x and y mouse position.
     */
    this.mouseMove = function($element, x, y) {
      if (!d.compareElements($element, $recentElement)) {
        $recentElement = $element;
        clearTimeout(timer);
        var hard = $element && $element.is('.expandable');
        var immediate = d.focusItem($element, hard);
        if (!immediate) {
          setTimeout(function(){
            d.focusItem($recentElement, true);
          }, delay);
        }
      }
    };
  };


  d.getItem = function($element) {
    var $item = false;
    while ($element.length && !$element.is('li, tr, #dqx_adminmenu, body')) {
      $element = $element.parent();
      // if ($element.is('lis
    }
    return $element;
  };


  d.isGhostEvent = function(event) {
    if (
      event.screenX == 0 && event.screenY == 0
      && event.pageX == 0 && event.pageY == 0
      && event.clientX == 0 && event.clientY == 0
    ) {
      // This is probably a "ghost" event.
      // Happens a lot in Chromium, if you click a select dropdown.
      // Ignore.
      return true;
    }
  };


  d.itemLink = function(item) {
    switch (item.tagName) {
      case 'TR':
        return $(item).children('td').children('a');
      case 'LI':
        return $(item).children('a');
    }
  };


  d.MenuItem = function($trail, isLeft) {

    this.element = function() {
      return $trail[0];
    };

    this.depth = function() {
      return $trail.length;
    };

    this.isIdentical = function(item) {
      return $trail[0] === item;
    };

    this.isLeft = function() {
      return isLeft;
    };

    this.isRight = function() {
      return !isLeft;
    };

    this.parentItem = function(n) {
      if (typeof n != 'number') {
        n = 1;
      }
      if ($trail.length > n) {
        return new d.MenuItem($trail.slice(n), isLeft);
      }
    };

    this.elementByDepth = function(depth) {
      return $trail[$trail.length - depth];
    };

    this.trail = function() {
      return $trail;
    };

    this.trailEnd = function(depth) {
      return $trail.slice(0, $trail.length - depth);
    };

    this.lastExpandable = function(depth) {
      return $trail.filter('.expandable')[0];
    };

    this.href = function() {
      switch ($trail[0].tagName) {
        case 'TR':
          return $($trail[0]).children('td').children('a').attr('href');
        case 'LI':
          return $($trail[0]).children('a').attr('href');
      }
    };

    this.trailPaths = function() {
      var paths = [];
      $trail.each(function(){
        paths.push(d.itemLink(this).attr('pathname'));
      });
      return paths;
    };
  };


  d.wrapMenuItem = function(item) {
    var $item = $(item);
    // jQuery.fn.add() would screw up the order of elements.
    var $trail = $item.children().parents().filter(function(){
      // We need the filter function,
      // because a simple selector would screw up the order of trail items.
      return $(this).is('li, tr');
    });
    var isLeft = $($trail[$trail.length - 1].parentNode).is('#dqx_adminmenu > ul.dqx_adminmenu-left');
    var wrapped = new d.MenuItem($trail, isLeft);
    return wrapped;
  };


  d.commonParentItem = function(a, b) {
    var minDepth = Math.min(a.depth(), b.depth());
    var commonDepth = 0;
    for (var depth = 1; depth <= minDepth; ++depth) {
      if (a.elementByDepth(depth) === b.elementByDepth(depth)) {
        commonDepth = depth;
      }
    }
    if (commonDepth) {
      return a.parentItem(a.depth() - commonDepth);
    }
  };


  d.inSameTrail = function(a, b) {
    var minDepth = Math.min(a.depth(), b.depth());
    return a.elementByDepth(minDepth) === b.elementByDepth(minDepth);
  };


  /**
   * Check if an element is an (indirect) parent of another element.
   * Also returns true, if both are identical.
   */
  d.elementIsParentOf = function(parent, child) {
    var c1 = child;
    while (child && child.tagName) {
      if (parent == child) {
        return true;
      }
      child = child.parentNode;
    }
    return false;
  }


  /**
   * Given an event.target element, we find the corresponding menu item,
   * or we return null.
   */
  d.resolveMouseTarget = function(root, element) {
    if (!d.elementIsParentOf(root, element)) {
      return null;
    }
    while (element && element.tagName) {
      switch (element.tagName) {

        case 'LI':
        case 'TR':
          // This is the item we are looking for.
          return element;

        case 'TABLE':
        case 'UL':
          // TODO: Why do we catch these types?
          // We are somewhere within the menu, but not on a specific item.
          return true;

        case 'BODY':
          // We are clearly outside the menu.
          return null;

        default:
          // Nothing. Continue.
      }
      element = element.parentNode;
    }
    // We are somewhere within the menu, but not on a specific item.
    return true;
  }


  d.InteractiveMenu = function($divRoot) {

    var activeItem;
    var mouseItem;
    var commonParentItem;
    var isParentChild = false;

    var timer;
    var timeToFlush;
    var timeoutKey;

    function setMouseItem(item) {
      if (activeItem && (activeItem.element() === item)) {
        mouseItem = activeItem;
        isParentChild = true;
        commonParentItem = activeItem;
      }
      else {
        mouseItem = item ? d.wrapMenuItem(item) : null;
        if (activeItem && mouseItem) {
          isParentChild = d.inSameTrail(activeItem, mouseItem);
          commonParentItem = d.commonParentItem(activeItem, mouseItem);
        }
        else {
          isParentChild = false;
          commonParentItem = null;
        }
      }
    }

    function flush() {
      if (activeItem === mouseItem) {
        return;
      }
      var prevDepth = activeItem ? activeItem.depth() : 0;
      var depth = mouseItem ? mouseItem.depth() : 0;
      if (depth !== prevDepth) {
        $divRoot.removeClass('active-item-depth-' + prevDepth);
        $divRoot.addClass('active-item-depth-' + depth);
        $divRoot[depth > 0 ? 'addClass' : 'removeClass']('open');
      }
      var commonParentDepth = commonParentItem ? commonParentItem.depth() : 0;
      activeItem && activeItem.trailEnd(commonParentDepth).removeClass('expanded');
      mouseItem && mouseItem.trailEnd(commonParentDepth).addClass('expanded');
      var activeExpandable = activeItem ? activeItem.lastExpandable() : null;
      var mouseExpandable = mouseItem ? mouseItem.lastExpandable() : null;
      if (activeExpandable !== mouseExpandable) {
        activeExpandable && $(activeExpandable).removeClass('trail-end');
        mouseExpandable && $(mouseExpandable).addClass('trail-end');
      }
      for (var k in d.hooks.focusItem) {
        d.hooks.focusItem[k](mouseItem, this);
      }
      activeItem = mouseItem;
      commonParentItem = mouseItem;
      isParentChild = true;
      // d.focusItem(activeItem ? $(activeItem.element()) : null, true);
      clearTimeout(timer);
      timeToFlush = null;
      timeoutKey = null;
    }

    this.isSameItem = function() {
      return (activeItem === mouseItem) || (!activeItem && !mouseItem);
    };

    this.activeItem = function() {
      return activeItem;
    };

    this.mouseItem = function() {
      return mouseItem;
    };

    this.isParentChild = function() {
      return isParentChild;
    };

    this.flush = function() {
      flush();
    };

    this.flushDelayed = function(delay, key) {
      if (key) {
        var now = d.getMillis();
        if (key == timeoutKey) {
          if (now + delay >= timeToFlush) {
            // Setting the timer would increase the time to wait
            return;
          }
        }
        timeoutKey = key;
        timeToFlush = now + delay;
      }
      clearTimeout(timer);
      timer = setTimeout(this.flush, delay);
    };

    this.attachMouseEngine = function(mouse_engine) {

      function mouseMove(event) {
        if (d.isGhostEvent(event)) return;
        var element = d.resolveMouseTarget($divRoot[0], event.target);
        if (element === true) {
          // We are on a TABLE or UL..
          // TODO: What does this mean for us? Does it even happen?
          // Register the movement, but don't do anything else.
          mouse_engine.mouseMove(event.pageX, event.pageY, false);
        }
        else if (element) {
          // We are on a LI or TR menu item.
          var isDifferentItem = !mouseItem || !mouseItem.isIdentical(element);
          setMouseItem(element);
          mouse_engine.mouseMove(event.pageX, event.pageY, isDifferentItem);
        }
        else {
          // We are outside the menu.
          var isDifferentItem = mouseItem ? true : false;
          setMouseItem(null);
          mouse_engine.mouseMove(event.pageX, event.pageY, isDifferentItem);
        }
      }

      /**
       * Mouse left the browser window.
       * Attention: Can be triggered at unexpected occasions.
       */
      function mouseMove_out(event) {
        if (event.relatedTarget || event.toElement) {
          // This is not really out of the browser window, just out of some element.
          return;
        }
        var isDifferentItem = mouseItem ? true : false;
        setMouseItem(null);
        mouse_engine.mouseMove(event.pageX, event.pageY, isDifferentItem);
      }

      $('body').mousemove(mouseMove);
      // Attention: This can be triggered at unexpected occasions.
      $(document).mouseout(mouseMove_out);
    };
  };


  d.hooks.init_early.dropLeftRight = function(context, settings, $divRoot) {
    $('> div > ul', $divRoot).each(function(){
      var $ul = $(this);
      $ul.addClass($ul.is('#dqx_adminmenu-user') ? 'dqx_adminmenu-right' : 'dqx_adminmenu-left');
    });
  };


  d.hooks.init_mid.mouse = function(context, settings, $divRoot) {
    var menu = new d.InteractiveMenu($divRoot);
    // TODO:
    //   d.MouseEngine_superfish is currently not supported.
    //   It is replaced immediately by d.MouseEngine_directional.
    //   We should either remove this pseudo-pluggability, or fix the
    //   _simple and _superfish engines.
    var mouse_engine = new d.MouseEngine_superfish(menu);
    menu.attachMouseEngine(mouse_engine);
  };


  d.InteractiveLinkField = function($divRoot){

    var mouseLink;
    var observers = [];

    this.setMouseLink = function(element) {
      if (element === mouseLink) return;
      mouseLink = element;
      for (var i = 0; i < observers.length; ++i) {
        observers[i](mouseLink);
      }
    }

    this.observe = function(observer) {
      observers.push(observer),
      observer(mouseLink, true);
    };
  };


  d.hooks.init_late.linkField = function(context, settings, $divRoot) {

    var linkField = new d.InteractiveLinkField($divRoot);
    for (var k in d.hooks.linkField) {
      d.hooks.linkField[k](linkField, context, settings, $divRoot);
    }

    $('body').mouseover(function(event){
      if (d.isGhostEvent(event)) return;

      var element = event.target;
      var link;

      while (element && element.tagName) {
        switch (element.tagName) {
          case 'A':
            link = element;
            while (element && element.tagName) {
              if (element === $divRoot[0]) {
                linkField.setMouseLink(link);
                return;
              }
              element = element.parentNode;
            }
            // fall through
          case 'BODY':
          case 'TR':
          case 'TD':
          case 'DIV':
          case 'LI':
          case 'UL':
            linkField.setMouseLink(null);
            return;
        }
        element = element.parentNode;
      }
    });
  };


  d.hooks.linkField.tooltip = function(linkField, context, settings, $divRoot) {

    var $tooltip = $('<div id="dqx_adminmenu-tooltip">bla');
    $tooltip.appendTo('body');
    $tooltip.css('display', 'none');

    function observeMouseLink(mouseLink) {
      if (mouseLink) {
        if (mouseLink.dqx_adminmenu_title === undefined) {
          var title = $(mouseLink).attr('title');
          mouseLink.dqx_adminmenu_title = title ? title : false;
          $(mouseLink).attr('title', null);
        }
        var text = mouseLink.dqx_adminmenu_title;
      }
      if (text) {
        $tooltip.html(text);
        $tooltip.show();
      }
      else {
        $tooltip.html('');
        $tooltip.hide();
      }
    }

    linkField.observe(observeMouseLink);
  };


  d.extractCelltipLabels = function($table) {
    var columnLabels = [];
    function write(iCol, text) {
      if (columnLabels[iCol] === undefined) {
        columnLabels[iCol] = '';
      }
      if (text !== undefined) {
        columnLabels[iCol] += text;
      }
    }
    $('> thead > tr', $table).each(function(){
      var $tr = $(this);
      var iCol = 0;
      $('> td, > th', $tr).each(function(){
        var $tx = $(this);
        var colspan = $tx.attr('colspan');
        colspan = colspan ? colspan : 1;
        if (colspan == 1) {
          write(iCol, $tx.html());
          ++iCol;
        }
        else {
          for (var i = 0; i < colspan; ++i) {
            write(iCol);
            ++iCol;
          }
        }
      });
    });
    return columnLabels;
  };


  d.hooks.linkField.celltip = function(linkField, context, settings, $divRoot) {

    var nothingToDo = true;

    $('table.celltip', $divRoot).each(function(){
      var $table = $(this);
      this.celltipLabels = d.extractCelltipLabels($table);
      nothingToDo = false;
    });

    if (nothingToDo) return;

    var $celltip = $('<div class="celltip">').hide();
    var $celltipArrow = $('<div class="celltip-arrow"><div>').appendTo($celltip);
    var $celltipInner = $('<div class="celltip-inner">').appendTo($celltip);

    function findCellIndex(td, tr) {
      var index = null;
      var i = 0;
      $(tr).children('td').each(function(){
        if (this === td) {
          index = i;
        }
        var colspan = $(this).attr('colspan');
        i += colspan ? colspan : 1;
      });
      return index;
    }

    function cellHover(td, tr, celltipLabels) {
      var $tdCelltip = $(tr).children('td.td-celltip');
      var index = findCellIndex(td, tr);
      var label = celltipLabels[index];
      if (label) {
        $celltip.appendTo($tdCelltip);
        $celltipInner.html(label);
        $celltip.show();
      }
      else {
        $celltipInner.html('');
        $celltip.hide();
      }
    }

    function linkHover(mouseLink){
      var element = mouseLink;
      var tr, td, celltipLabels;
      while (element.tagName) {
        switch (element.tagName) {
          case 'TD':
            td = element;
            break;
          case 'TR':
            tr = element;
            break;
          case 'TABLE':
            celltipLabels = element.celltipLabels;
            if (celltipLabels && tr && td) {
              cellHover(td, tr, celltipLabels);
            }
            return;
          case 'UL':
          case 'LI':
          case 'BODY':
            $celltipInner.html('');
            $celltip.hide();
            return;
        }
        element = element.parentNode;
      }
    };

    linkField.observe(function(mouseLink){
      if (mouseLink) {
        linkHover(mouseLink);
      }
      else {
        $celltipInner.html('');
        $celltip.hide();
      }
    });
  };


  /**
   * A more performant way to check all items
   */
  d.markActiveTrail = function(item, check) {
    var $children, $links;
    var $item = $(item);
    switch (item.tagName) {
      case 'TR':
        $children = $item
          .children('td.td-submenu')
          .children('div')
          .children('ul, table')
          .children('li, tr')
        ;
        $links = $item.children('td').children('a');
        break;
      case 'LI':
        $children = $item
          .children('ul, table')
          .children('li, tr')
        ;
        $links = $item.children('a');
        break;
    }
    if (!$children.length) return;
    var active = false;
    var activeTrail = false;
    $children.each(function(){
      activeTrail = d.markActiveTrail(this, check) || activeTrail;
    });
    $links.each(function(){
      if (check(this)) {
        $(this).addClass('active');
        active = true;
        activeTrail = true;
      }
    });
    if (activeTrail) {
      $item.addClass('active');
      if (active) {
        $item.addClass('active-trail');
      }
    }
  };


  /**
   * Performance:
   * This was measured to take around 200 millis.
   * (depending on client machine and size of the menu)
   * Reason is, every link has to be inspected separately.
   */
  d.hooks.init_latest.activePage = function(context, settings, $divRoot) {

    // For a path of aa/bb/cc, the partials and matches will look like this:
    //
    // index          0   1   2      3
    // fragments      aa  bb  cc
    // inv(partials)      aa  aa/bb  aa/bb/cc
    // matches        []  []  []     []

    var path = document.location.pathname;
    var fragments = path.split('/');
    var matches = [[]];
    var partial = fragments[0];
    var partials = {};

    // We gain a few millis, by using for() instead of each().
    for (var i = 1;; ++i) {
      partials[partial] = i;
      matches[i] = [];
      if (i > fragments.length) break;
      partial += '/' + fragments[i];
    }

    var $links = $('a', $divRoot[0]);

    if (true) {
      for (var i = 0; i < $links.length; ++i) {
        var link = $links[i];
        var index = partials[link.pathname];
        if (index) {
          matches[index].push(link);
        }
      }
    }
    else {
      $links.each(function(){
        var link = this;
        var i = partials[link.pathname];
        if (i) {
          matches[i].push(link);
        }
      });
    }

    var $found;
    for (var i = fragments.length; i > 0; --i) {
      if (matches[i].length) {
        $found = $(matches[i]);
        if (i == fragments.length) {
          $found.addClass('active');
        }
        $found.parents('li, tr').addClass('active-trail');
        break;
      }
    }
  };


  d.actionSubmit = function(action) {
    // to be replaced.
  };

  d.extractDrupalPath = function(pathname) {
    // to be replaced.
  };


  d.hooks.init_late.actionForm = function(context, settings, $divRoot) {
    var $form = $(settings.hidden_action_form).appendTo($divRoot);
    $form.attr('action', document.location.href);
    var $action_field = $('input[name=action]', $form);
    d.actionSubmit = function(action) {
      $action_field.val(action);
      $form.submit();
    };
    var test_pathname = $('#dqx_adminmenu-test-link', $form)[0].pathname;
    d.extractDrupalPath = d.buildFunction_extractDrupalPath(test_pathname);
  };


  d.buildFunction_extractDrupalPath = function(test_pathname) {
    // We know that the test pathname is "en/admin" or "/en/admin",
    // or just "/admin" or "admin", depending on browser and language.
    var fragments = test_pathname.split('admin');
    if (fragments.pop() != '') {
      throw "fail.";
    }
    else {
      var prefix = fragments.join('admin');
      return function(link_or_pathname) {
        var pathname = (typeof link_or_pathname == 'object') ? link_or_pathname.pathname : link_or_pathname;
        var fragments = pathname.split(prefix);
        if (fragments.shift() != '') {
          throw "fail.";
        }
        else {
          return fragments.join(prefix);
        }
      }
    }
  };


  d.hooks.init_late.actionLinks = function(context, settings, $divRoot) {
    $('a.dqx_adminmenu-action', $divRoot).each(function(){
      var $a = $(this);
      var path = d.extractDrupalPath(this.pathname, settings.base_url);
      $a.attr('href', 'javascript:Drupal.dqx_adminmenu.actionSubmit("' + path.replace('"', '\"') + '");');
    });
  };


  /**
   * See http://phpjs.org/functions/preg_quote:491
   */
  d.preg_quote = function(str, delimiter) {
    return (str + '').replace(new RegExp('[.\\\\+*?\\[\\^\\]$(){}=!<>|:\\' + (delimiter || '') + '-]', 'g'), '\\$&');
  };


  d.LinkBuilder = function(adminUrl, language) {
    var adminLink = $('<a>').attr('href', adminUrl)[0];
    var adminPathRegex = new RegExp('^' + d.preg_quote(adminLink.pathname).replace(/admin$/, '(.*)') + '$');
    var current = document.location.pathname.match(adminPathRegex)[1];

    this.processLink = function(link) {
      link.pathname = link.pathname.replace(/admin$/, current);
      if (link.href == document.location.href) {
        $(link).addClass('active');
        $(link).addClass('active-trail');
      }
    }
  };


  d.hooks.init_late.linkVariation = function(context, settings, $divRoot){
    var linkBuilder = new d.LinkBuilder(settings.admin_url, settings.language);
    $('a.dqx_adminmenu-link-variation', context).each(function(){
      linkBuilder.processLink(this);
    });
  };


  d.menufy = function($body, url) {
    var $divRoot = $('<div>').attr('id', 'dqx_adminmenu-wrapper');
    var $divInner = $('<div>').attr('id', 'dqx_adminmenu').prependTo($divRoot);
    $divRoot.prependTo('body');
    $('body').addClass('dqx_adminmenu');
    $.ajax({
      url: url,
      cache: true,
      dataType: 'text',
      success: function(xml) {
        if (xml) {
          $divRoot.html(xml);
          $('> div > ul', $divRoot); // .css('display', 'none').fadeIn(400);
          d.init($divRoot);
        }
      }
    });
  };


  /*
   * stuff to run when the page loads.
   */
  Drupal.behaviors.dqx_adminmenu = {
    attach: function(context){
      $('body', context).each(function(){
        if (Drupal.settings.dqx_adminmenu && Drupal.settings.dqx_adminmenu.xml_url) {
          var url = Drupal.settings.dqx_adminmenu.xml_url;
          d.menufy($(this), url);
        }
      });
    }
  };
})();



