(function($) {
  if (typeof $.setSelectionRange != 'function') {
    $.fn.setSelectionRange = function(selectionStart, selectionEnd) {
      var element = this;
      if (this instanceof jQuery) {
        element = this.get(0);
      }
      if (element.setSelectionRange) {
        $(element).focus();
        element.setSelectionRange(selectionStart, selectionEnd);
      }
      else if (this.createTextRange) {
        var range = element.createTextRange();
        range.collapse(true);
        range.moveEnd('character', selectionEnd);
        range.moveStart('character', selectionStart);
        range.select();
      }
    }

    $.fn.setCursorToPos = function(pos) {
      $(this).setSelectionRange(pos, pos);
    }
  }
})(jQuery);