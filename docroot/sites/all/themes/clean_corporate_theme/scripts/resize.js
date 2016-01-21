(function($){
  $(window).resize(function(event){
    function set_default_size(item, i, arr) {
      $(item).height("initial");
    }
    function max_h(item, i, arr) {
      var current_height = $(item).height();
      if(current_height > max_height) { max_height = current_height;}
    }
    function set_max_size(item, i, arr) {
      $(item).height(max_height);
    }
    var win = (this);
    var elements = ["#columns", ".region-sidebar-first", ".region-sidebar-second"];
    if ($(win).width() >= 1000) {
      max_height = 0;
      elements.forEach(max_h);
      elements.forEach(set_max_size);
    }
    else {
      elements.forEach(set_default_size);
    }
  });
  $(window).load(function() {
    function max_h(item, i, arr) {
      var current_height = $(item).height();
      if(current_height > max_height) { max_height = current_height;}
    }
    function set_max_size(item, i, arr) {
      $(item).height(max_height);
    }
    var win = (this);
    var elements = ["#columns", ".region-sidebar-first", ".region-sidebar-second"];
    if ($(win).width() >= 1000) {
      max_height = 0;
      elements.forEach(max_h);
      elements.forEach(set_max_size);
    }
  });
})(jQuery);
