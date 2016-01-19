(function($) {
  Drupal.behaviors.orderedList = {
    attach: function() {
      var widgets = $('.form-type-ordered-list').not('.processed');
      widgets.each(function() {
        var target = $(this)
          , input = $('input', target)
          , left = $('.button-left a', target)
          , right = $('.button-right a', target)
          , up = $('.button-up a', target)
          , down = $('.button-down a', target)
          , available = $('.items-available ul', target)
          , selected = $('.items-selected ul', target);
        !selected.length && (selected = $('<ul/>').appendTo($('.items-selected', target)));
        !available.length && (available = $('<ul/>').appendTo($('.items-available', target)));
        available.add(selected).find('li').click(function() {
          $(this).toggleClass('selected');
        });
        left.click(function() {
          $('li.selected', available).removeClass('selected').appendTo(selected);
          return false;
        });
        right.click(function() {
          $('li.selected', selected).each(function() {
            var self = $(this).removeClass('selected').detach()
              , index = $(this).attr('index')
              , items = $('li', available);
            items.each(function() {
              if (index < $(this).attr('index')) {
                self.insertBefore($(this));
                return false;
              }
            });
            !self.parent().length && self.appendTo(available);
          });
          return false;
        });
        up.click(function() {
          $('li.selected', selected).each(function() {
            var target = $(this);
            target.prevAll().not('.selected').length && target.prev().insertAfter(target);
          });
          return false;
        });
        down.click(function() {
          $($('li.selected', selected).get().reverse()).each(function() {
            var target = $(this);
            target.nextAll().not('.selected').length && target.next().insertBefore(target);
          });
          return false;
        });
        left.add(right).add(up).add(down).click(function() {
          var value = [];
          $('li', selected).each(function() {
            value.push($(this).attr('key'));
          });
          input.val(value.join('|'));
        });
        target.addClass('processed');
      });
    }
  };
})(jQuery);
