(function ($) {

/**
 * Toggle the visibility of a fieldset using smooth animations.
 */
Drupal.toggleFieldset = function (fieldset) {
  // jQueryUI Classes
  var options = {
      openClass: 'ui-corner-top ui-state-active ui-no-border-bottom',
      closeClass: 'ui-corner-all ui-state-default ',
      iconOpen: 'ui-icon-triangle-1-s',
      iconClose: 'ui-icon-triangle-1-e'
  };
  
  var $fieldset = $(fieldset),
      legend = $fieldset.children('legend');
  if ($fieldset.is('.collapsed')) {
    var $content = $('> .fieldset-wrapper', fieldset).hide();
    
    // jQueryUI additonal action
    legend
      .removeClass(options.closeClass)
      .addClass(options.openClass)
      .find('.ui-icon')
      .removeClass(options.iconClose)
      .addClass(options.iconOpen);
    
    $fieldset
      .removeClass('collapsed')
      .trigger({ type: 'collapsed', value: false })
      .find('> legend span.fieldset-legend-prefix').html(Drupal.t('Hide'));
    $content.slideDown({
      duration: 'fast',
      easing: 'linear',
      complete: function () {
        Drupal.collapseScrollIntoView(fieldset);
        fieldset.animating = false;
      },
      step: function () {
        // Scroll the fieldset into view.
        Drupal.collapseScrollIntoView(fieldset);
      }
    });
  }
  else {
    $fieldset.trigger({ type: 'collapsed', value: true });
    $('> .fieldset-wrapper', fieldset).slideUp('fast', function () {
      $fieldset
        .addClass('collapsed')
        .find('> legend span.fieldset-legend-prefix').html(Drupal.t('Show'));
      
      // jQueryUI additonal action
      legend
        .addClass(options.closeClass)
        .removeClass(options.openClass)
        .find('.ui-icon')
        .addClass(options.iconClose)
        .removeClass(options.iconOpen);
      
      fieldset.animating = false;
    });
  }
};

/**
 * Scroll a given fieldset into view as much as possible.
 */
Drupal.collapseScrollIntoView = function (node) {
  var h = document.documentElement.clientHeight || document.body.clientHeight || 0;
  var offset = document.documentElement.scrollTop || document.body.scrollTop || 0;
  var posY = $(node).offset().top;
  var fudge = 55;
  if (posY + node.offsetHeight + fudge > h + offset) {
    if (node.offsetHeight > h) {
      window.scrollTo(0, posY);
    }
    else {
      window.scrollTo(0, posY + node.offsetHeight - h + fudge);
    }
  }
};

Drupal.behaviors.collapse = {
  attach: function (context, settings) {
    var options = {
        openClass: 'ui-corner-top ui-state-active ui-no-border-bottom',
        closeClass: 'ui-corner-all ui-state-default ',
        hoverClass: 'ui-state-hover',
        iconOpen: 'ui-icon-triangle-1-s',
        iconClose: 'ui-icon-triangle-1-e'
    };
    
    $('fieldset.collapsible', context).once('collapse', function () {
      var $fieldset = $(this),
          legend = $fieldset.children('legend');
      // Build jQuery UI Class
      
      // Expand fieldset if there are errors inside, or if it contains an
      // element that is targeted by the uri fragment identifier. 
      var anchor = location.hash && location.hash != '#' ? ', ' + location.hash : '';
      if ($('.error' + anchor, $fieldset).length) {
        $fieldset.removeClass('collapsed');
        
        // jQueryUI additonal action
        legend
          .removeClass(options.closeClass)
          .addClass(options.openClass)
          .find('.ui-icon')
          .removeClass(options.iconClose)
          .addClass(options.iconOpen);
      }

      var summary = $('<span class="summary"></span>');
      $fieldset.
        bind('summaryUpdated', function () {
          var text = $.trim($fieldset.drupalGetSummary());
          summary.html(text ? ' (' + text + ')' : '');
        })
        .trigger('summaryUpdated');

      // Turn the legend into a clickable link, but retain span.fieldset-legend
      // for CSS positioning.
      var $legend = $('> legend .fieldset-legend', this);

      $('<span class="fieldset-legend-prefix element-invisible"></span>')
        .append($fieldset.hasClass('collapsed') ? Drupal.t('Show') : Drupal.t('Hide'))
        .prependTo($legend)
        .after(' ');

      // .wrapInner() does not retain bound events.
      var $link = $('<a class="fieldset-title" href="#"></a>')
        .prepend($legend.contents())
        .appendTo($legend)
        .click(function () {
          var fieldset = $fieldset.get(0);
          // Don't animate multiple times.
          if (!fieldset.animating) {
            fieldset.animating = true;
            Drupal.toggleFieldset(fieldset);
          }
          return false;
        });

      $legend.append(summary);
      
      // Bind mousehover
      legend.bind('mouseenter', function() {
        $(this).addClass(options.hoverClass);
      }).bind('mouseout', function() {
        $(this).removeClass(options.hoverClass);
      });
    });
  }
};

})(jQuery);
