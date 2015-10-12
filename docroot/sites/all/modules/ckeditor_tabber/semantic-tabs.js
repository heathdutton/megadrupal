(function ($) {
  // variables global to this plugin's scope
  var tabIndex = 0;
  var instanceId = 0;

  $.fn.tabs = function () {
    this.each(function () {
      var el = $(this);
      var current;

      var numTabs = el.find('dt').length;
      el.find('> dt').each(function () {
        var tabId = 'tab-' + instanceId + '-' + tabIndex;
        // Populate DTs with anchor links
        $(this).wrapInner('<a href="#' + tabId + '" />')
          .css('width', (100 / numTabs) + '%');
        // ID attribute on DTs
        $(el.find('> dt').get(tabIndex)).attr('id', tabId);
        tabIndex = tabIndex + 1;
      });
      el.find('> dd').hide();

      // Remove text nodes so that display:inline-block behave nicely
      var reBlank = /^\s*$/;
      var walk = function (node) {
        var child, next;
        switch (node.nodeType) {
          case 3: // Text node
            if (reBlank.test(node.nodeValue)) {
              node.parentNode.removeChild(node);
            }
            break;
          case 1: // Element node
          case 9: // Document node
            child = node.firstChild;
            while (child) {
              next = child.nextSibling;
              walk(child);
              child = next;
            }
            break;
        }
      }
      walk($(this)[0]);

      // Initialise tab depending on current hash
      var hash = location.hash;
      if (el.find('dt a[href="' + hash + '"]').length) {
        current = el.find('a[href="' + hash + '"]').parent().addClass('current');
      } else {
        current = el.find('> dt:first').addClass('current');
      }

      // Calculate height
      var currentHeight = current.next('dd').show().height();
      var dlHeight = el.find('> dt').height();
      var dtHeight = current.height();
      el.css('height', dlHeight + currentHeight + dtHeight + 3); // 3 is borders

      instanceId = instanceId + 1;
    });

    // onclick event
    $('dl.ckeditor-tabber dt a').click(function (e) {
      e.preventDefault();

      var el = $(this).parents('dl.ckeditor-tabber');
      el.find('.current').removeClass('current').next('dd').hide(0);
      var current = $(this).parent('dt').addClass('current');
      var currentHeight = current.next('dd').show(0).height();
      var dlHeight = el.removeAttr('style').height();
      var dtHeight = $(this).parents('dt').height();

      el.css('height', dlHeight + currentHeight + dtHeight + 4);

      // Update hash with pushState or fallback
      if (history.pushState) {
        history.pushState({}, "", $(this).attr('href'));
      } else {
        var scrollV = document.body.scrollTop,
          scrollH = document.body.scrollLeft;
        location.hash = $(this).attr('href');
        document.body.scrollTop = scrollV;
        document.body.scrollLeft = scrollH;
      }
    });
  }
})(jQuery);
(function ($) {
  Drupal.behaviors.ckeditorTabs = {
    attach: function (context) {
      $(Drupal.settings.ckeditor_tabber.elements, context).tabs();
    }
  };
}(jQuery));
