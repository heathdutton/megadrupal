
jQuery(function($) {

  $(document).bind({
    dragover: function(e) {
      e.preventDefault();
    }, // dragover

    dragleave: function(e) {
      e.preventDefault();
    }, // dragleave

    drop: function(e) {
      if ('INPUT' != e.target.nodeName) {
        e.preventDefault();
      }
    } // drop
  });

});
