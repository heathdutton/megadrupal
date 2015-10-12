(function ($) {
  $(document).delegate(".stt-share-embed, .stt-share-link", "focus", function() {
    var inp = this;
    setTimeout(function(){ inp.select() }, 100);
  });
})(jQuery);
