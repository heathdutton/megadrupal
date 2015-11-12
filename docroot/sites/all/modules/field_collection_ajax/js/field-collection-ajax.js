(function ($) {

Drupal.ajax.prototype.commands.markTableChanged = function (ajax, response, status) {
  var table = Drupal.tableDrag[response.selector];
  if (!table) {
    return;
  }

  if (!table.changed) {
    $(Drupal.theme('tableDragChangedWarning')).insertBefore(table.table).hide().fadeIn('slow');
    table.changed = true;
  }
};

Drupal.ajax.prototype.commands.fcajaxScrollTop = function (ajax, response, status) {
  // Borrowed from Views, but modified because Views would only
  // scroll upward assuming that it was always about pagers.
  // Scroll to the top of the element.
  var offset = $(response.selector).offset();
  // We can't guarantee that the scrollable object should be
  // the body, as the view could be embedded in something
  // more complex such as a modal popup. Recurse up the DOM
  // and scroll the first element that has a non-zero top.
  var scrollTarget = response.selector;
  while ($(scrollTarget).scrollTop() == 0 && $(scrollTarget).parent()) {
    scrollTarget = $(scrollTarget).parent();
  }

  // We need to leave some space at the top, this fudge does the trick.
  var fudge = 100;
  // The scroll also needs to happen relatively slowly or the user
  // gets disoriented.
  $(scrollTarget).animate({scrollTop: (offset.top - fudge)}, 1000);
};

})(jQuery);
