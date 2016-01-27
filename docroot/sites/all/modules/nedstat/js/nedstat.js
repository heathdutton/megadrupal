(function($) {
  $(document).ready(function(){
      //hide nodetype counternames
    $('#nedstat-countername-node-type-settings').hide();

    if ($('#edit-nedstat-general-countername-generate-type-path').val() == '') {
      $('#nedstat-countername-node-type-settings').show();
    }

    $('#edit-nedstat-general-countername-generate-type-path').bind('change', function() {
    });

  });
})(jQuery);
